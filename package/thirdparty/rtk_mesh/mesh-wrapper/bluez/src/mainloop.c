/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2011-2014  Intel Corporation
 *  Copyright (C) 2002-2010  Marcel Holtmann <marcel@holtmann.org>
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <stdbool.h>

#include "shared/mainloop.h"
#include "shared/util.h"
#include "shared/queue.h"

#include "trace.h"

#define MAX_EPOLL_EVENTS 10

static int epoll_fd;
static int epoll_terminate;
static int exit_status;

struct mainloop_data {
	int fd;
	uint32_t events;
	mainloop_event_func callback;
	mainloop_destroy_func destroy;
	void *user_data;
};

static struct queue *mainloop_list;

struct timeout_data {
	int fd;
	mainloop_timeout_func callback;
	mainloop_destroy_func destroy;
	void *user_data;
};

struct signal_data {
	int fd;
	sigset_t mask;
	mainloop_signal_func callback;
	mainloop_destroy_func destroy;
	void *user_data;
};

static struct signal_data *signal_data;

void mainloop_init2(void)
{
	unsigned int i;

	epoll_fd = epoll_create1(EPOLL_CLOEXEC);

	mainloop_list = queue_new();
	if (!mainloop_list)
		DFU_PRINT_ERROR0("Couldn't alloc mainloop_list queue");

	epoll_terminate = 0;
}

void mainloop_quit2(void)
{
	epoll_terminate = 1;
}

void mainloop_exit_success2(void)
{
	exit_status = EXIT_SUCCESS;
	epoll_terminate = 1;
}

void mainloop_exit_failure2(void)
{
	exit_status = EXIT_FAILURE;
	epoll_terminate = 1;
}

static void signal_callback(int fd, uint32_t events, void *user_data)
{
	struct signal_data *data = user_data;
	struct signalfd_siginfo si;
	ssize_t result;

	if (events & (EPOLLERR | EPOLLHUP)) {
		mainloop_quit2();
		return;
	}

	result = read(fd, &si, sizeof(si));
	if (result != sizeof(si))
		return;

	if (data->callback)
		data->callback(si.ssi_signo, data->user_data);
}

static bool match_by_fd(const void *data, const void *user_data)
{
	const struct mainloop_data *md = data;
	int fd = PTR_TO_INT(user_data);

	return md->fd == fd;
}

static void destroy_mainloop_data(void *user_data)
{
	struct mainloop_data *data = user_data;

	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, data->fd, NULL);

	if (data->destroy)
		data->destroy(data->user_data);

	free(data);
}

int mainloop_run2(void)
{
	unsigned int i;

	if (signal_data) {
		if (sigprocmask(SIG_BLOCK, &signal_data->mask, NULL) < 0)
			return EXIT_FAILURE;

		signal_data->fd = signalfd(-1, &signal_data->mask,
						SFD_NONBLOCK | SFD_CLOEXEC);
		if (signal_data->fd < 0)
			return EXIT_FAILURE;

		if (mainloop_add_fd2(signal_data->fd, EPOLLIN,
				signal_callback, signal_data, NULL) < 0) {
			close(signal_data->fd);
			return EXIT_FAILURE;
		}
	}

	exit_status = EXIT_SUCCESS;

	while (!epoll_terminate) {
		struct epoll_event events[MAX_EPOLL_EVENTS];
		int n, nfds;

		nfds = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1);
		if (nfds < 0)
			continue;

		for (n = 0; n < nfds; n++) {
			struct mainloop_data *data = events[n].data.ptr;
			void *ptr;
			ptr = queue_find(mainloop_list, NULL, data);
			if (!ptr)
			{
				DFU_PRINT_ERROR0("Couldn't fd in mainloop_list");
				continue;
			}
			data->callback(data->fd, events[n].events,
							data->user_data);
		}
	}

	if (signal_data) {
		mainloop_remove_fd2(signal_data->fd);
		close(signal_data->fd);

		if (signal_data->destroy)
			signal_data->destroy(signal_data->user_data);
	}

	queue_destroy(mainloop_list, destroy_mainloop_data);
	mainloop_list = NULL;
	close(epoll_fd);
	epoll_fd = 0;

	return exit_status;
}

int mainloop_add_fd2(int fd, uint32_t events, mainloop_event_func callback,
				void *user_data, mainloop_destroy_func destroy)
{
	struct mainloop_data *data;
	struct epoll_event ev;
	int err;

	if (fd < 0 || !callback)
		return -EINVAL;

	data = malloc(sizeof(*data));
	if (!data)
		return -ENOMEM;

	memset(data, 0, sizeof(*data));
	data->fd = fd;
	data->events = events;
	data->callback = callback;
	data->destroy = destroy;
	data->user_data = user_data;

	memset(&ev, 0, sizeof(ev));
	ev.events = events;
	ev.data.ptr = data;

	err = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, data->fd, &ev);
	if (err < 0) {
		free(data);
		return err;
	}

	if (!queue_push_tail(mainloop_list, data)) {
		DFU_PRINT_ERROR0("Couldn't add data to mainloop_list");
		epoll_ctl(epoll_fd, EPOLL_CTL_DEL, data->fd, NULL);
		free(data);
		return -1;
	}

	return 0;
}

int mainloop_modify_fd2(int fd, uint32_t events)
{
	struct mainloop_data *data;
	struct epoll_event ev;
	int err;

	if (fd < 0)
		return -EINVAL;

	data = queue_find(mainloop_list, match_by_fd, INT_TO_PTR(fd));
	if (!data) {
		DFU_PRINT_ERROR0("Couldn't find mainloop data");
		return -ENXIO;
	}

	memset(&ev, 0, sizeof(ev));
	ev.events = events;
	ev.data.ptr = data;

	err = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, data->fd, &ev);
	if (err < 0)
		return err;

	data->events = events;

	return 0;
}

int mainloop_remove_fd2(int fd)
{
	struct mainloop_data *data;
	int err;

	if (fd < 0)
		return -EINVAL;

	data = queue_find(mainloop_list, match_by_fd, INT_TO_PTR(fd));
	if (!data) {
		DFU_PRINT_ERROR0("Couldn't find mainloop data");
		return -ENXIO;
	}

	queue_remove(mainloop_list, data);

	err = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, data->fd, NULL);

	if (data->destroy)
		data->destroy(data->user_data);

	free(data);

	return err;
}

static void timeout_destroy(void *user_data)
{
	struct timeout_data *data = user_data;

	close(data->fd);
	data->fd = -1;

	if (data->destroy)
		data->destroy(data->user_data);

	free(data);
}

static void timeout_callback(int fd, uint32_t events, void *user_data)
{
	struct timeout_data *data = user_data;
	uint64_t expired;
	ssize_t result;

	if (events & (EPOLLERR | EPOLLHUP))
		return;

	result = read(data->fd, &expired, sizeof(expired));
	if (result != sizeof(expired))
		return;

	if (data->callback)
		data->callback(data->fd, data->user_data);
}

static inline int timeout_set(int fd, unsigned int msec)
{
	struct itimerspec itimer;
	unsigned int sec = msec / 1000;

	memset(&itimer, 0, sizeof(itimer));
	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = 0;
	itimer.it_value.tv_sec = sec;
	itimer.it_value.tv_nsec = (msec - (sec * 1000)) * 1000 * 1000;

	return timerfd_settime(fd, 0, &itimer, NULL);
}

int mainloop_add_timeout(unsigned int msec, mainloop_timeout_func callback,
				void *user_data, mainloop_destroy_func destroy)
{
	struct timeout_data *data;

	if (!callback)
		return -EINVAL;

	data = malloc(sizeof(*data));
	if (!data)
		return -ENOMEM;

	memset(data, 0, sizeof(*data));
	data->callback = callback;
	data->destroy = destroy;
	data->user_data = user_data;

	data->fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (data->fd < 0) {
		free(data);
		return -EIO;
	}

	if (msec > 0) {
		if (timeout_set(data->fd, msec) < 0) {
			close(data->fd);
			free(data);
			return -EIO;
		}
	}

	if (mainloop_add_fd2(data->fd, EPOLLIN | EPOLLONESHOT,
				timeout_callback, data, timeout_destroy) < 0) {
		close(data->fd);
		free(data);
		return -EIO;
	}

	return data->fd;
}

int mainloop_modify_timeout(int id, unsigned int msec)
{
	if (msec > 0) {
		if (timeout_set(id, msec) < 0)
			return -EIO;
	}

	if (mainloop_modify_fd2(id, EPOLLIN | EPOLLONESHOT) < 0)
		return -EIO;

	return 0;
}

int mainloop_remove_timeout(int id)
{
	return mainloop_remove_fd2(id);
}

int mainloop_set_signal2(sigset_t *mask, mainloop_signal_func callback,
				void *user_data, mainloop_destroy_func destroy)
{
	struct signal_data *data;

	if (!mask || !callback)
		return -EINVAL;

	data = malloc(sizeof(*data));
	if (!data)
		return -ENOMEM;

	memset(data, 0, sizeof(*data));
	data->callback = callback;
	data->destroy = destroy;
	data->user_data = user_data;

	data->fd = -1;
	memcpy(&data->mask, mask, sizeof(sigset_t));

	free(signal_data);
	signal_data = data;

	return 0;
}

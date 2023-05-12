#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/timerfd.h>

#include "shared/mainloop.h"
#include "shared/timeout.h"
#include "shared/queue.h"
#include "os_sched.h"

#include "os_timer.h"
#include "trace.h"

typedef void (*timer_callback_t) (void *);

struct timer_wrapper {
	int fd;

	char *name;
	uint32_t interval_ms;
	uint32_t timer_id;
	bool reload;
	timer_callback_t callback;
	bool state;
};

//static GSList *timers = NULL;
static void timeout_callback(int fd, uint32_t events, void *user_data)
{
	struct timer_wrapper *timer;

	uint64_t expired;
	ssize_t result;

	if(user_data == NULL)
	{
		APP_PRINT_ERROR0("timer is not exist");
		return;
	}
	timer = user_data;
	if (events & (EPOLLERR | EPOLLHUP))
		return;

	result = read(timer->fd, &expired, sizeof(expired));
	if (result != sizeof(expired))
		return;

	if((timer!=NULL)&&(!timer->reload))
	{
		timer->state = false;
	}

	if (timer->callback)
		timer->callback(user_data);
}

bool os_timer_create(void **pp_handle, const char *p_timer_name,
		     uint32_t timer_id, uint32_t interval_ms, bool reload,
		     void (*p_timer_callback) (void *))
{
	int fd;
	struct timer_wrapper *timer = (void *)malloc(sizeof(*timer));

	if (!timer) {
		APP_PRINT_ERROR0("malloc timer error");
		return false;
	}

	timer->name = strdup(p_timer_name);
	timer->timer_id = timer_id;
	timer->interval_ms = interval_ms;
	timer->reload = reload;
	timer->callback = p_timer_callback;
	timer->state = false;

	//fd = timerfd_create(CLOCK_REALTIME, 0);
	fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (fd == -1) {
		APP_PRINT_ERROR0("timerfd_create error");
		goto err1;
	}
	timer->fd = fd;

	if (mainloop_add_fd2(timer->fd, EPOLLIN,
			    timeout_callback, timer, NULL) < 0) {
		APP_PRINT_ERROR1("Add timer fd %d to mainloop error\n",
			timer->fd);
		goto err2;
	}

	//printf("Create timer, fd %d\n", timer->fd);

	*pp_handle = timer;

	//g_slist_prepend(timers, timer);

	return true;
err2:
	close(timer->fd);
err1:
	free(timer);
	return false;
}

bool os_timer_start(void **pp_handle)
{
	struct timer_wrapper *timer;

	struct itimerspec itimer;
	unsigned int sec;
	long nsec;

	timer = *pp_handle;

	if(timer == NULL)
	{
		APP_PRINT_ERROR1("%s: timer is not exist",__func__);
		return false;
	}

	sec = timer->interval_ms / 1000;
	nsec = (timer->interval_ms - sec * 1000) * 1000000;

	memset(&itimer, 0, sizeof(itimer));

	if (timer->reload) {
		itimer.it_interval.tv_sec = sec;
		itimer.it_interval.tv_nsec = nsec;
	}

	itimer.it_value.tv_sec = sec;
	itimer.it_value.tv_nsec = nsec;

	timerfd_settime(timer->fd, 0, &itimer, NULL);

	timer->state = true;

	return true;
}

bool os_timer_stop(void **pp_handle)
{
	struct timer_wrapper *timer;
	struct itimerspec itimer;

	timer = *pp_handle;
	if(timer == NULL)
	{
		APP_PRINT_ERROR1("%s: timer is not exist",__func__);
		return false;
	}
	timer->state = false;

	/* disarms the timer */
	memset(&itimer, 0, sizeof(itimer));
	timerfd_settime(timer->fd, 0, &itimer, NULL);

	return true;
}

bool os_timer_delete(void **pp_handle)
{
	struct timer_wrapper *timer = *pp_handle;

	if(timer == NULL)
	{
		APP_PRINT_ERROR1("%s: timer is not exist",__func__);
		return false;
	}
	//printf("Delete timer, fd %d\n", timer->fd);

	mainloop_remove_fd2(timer->fd);
	os_timer_stop(pp_handle);	// stop timer first?
	close(timer->fd);
	free(timer);
	*pp_handle = NULL;
	//printf("----delete timer ---\n");
	return true;
}

bool os_timer_restart(void **pp_handle, uint32_t interval_ms)
{
	struct timer_wrapper *timer = *pp_handle;

	if(timer == NULL)
	{
		APP_PRINT_ERROR1("%s: timer is not exist",__func__);
		return false;
	}

	if (timer->state)
		os_timer_stop(pp_handle);

	timer->interval_ms = interval_ms;

	os_timer_start(pp_handle);

	return true;
}

bool os_timer_id_get(void **pp_handle, uint32_t * id)
{
	struct timer_wrapper *timer = *pp_handle;

	if(timer == NULL)
	{
		APP_PRINT_ERROR1("%s: timer is not exist",__func__);
		return false;
	}

	*id = timer->timer_id;
	return true;
}

bool os_timer_state_get(void **pp_handle, uint32_t *p_timer_state)
{
	struct timer_wrapper *timer = *pp_handle;
	if(timer != NULL)
	{
		*p_timer_state =(uint32_t) timer->state;
	}
	else
	{
		*p_timer_state = 0;
		APP_PRINT_TRACE1("%s: timer is not exist",__func__);
		return false;
	}
	return true;
}
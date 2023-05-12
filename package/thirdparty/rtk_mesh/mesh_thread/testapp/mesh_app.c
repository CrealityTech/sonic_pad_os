/*
 *  Copyright (C) 2019 Realtek Semiconductor Corporation.
 *
 *  Author: Alex Lu <alex_lu@realsil.com.cn>
 */
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <getopt.h>
#include <stdbool.h>
//#include <wordexp.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/signalfd.h>

#include <monitor/bt.h>
#include <shared/io.h>
#include <shared/mainloop.h>
//#include <shared/queue.h>
#include "app_cmd.h"
#include "RTKMeshTypes.h"
#include "app_handler.h"

#include "trace.h"

#define COLOR_OFF	"\x1B[0m"
#define COLOR_BLUE	"\x1B[0;34m"

int __main_terminated;
int app_mesh_msg_pipe[2];
struct queue *mesh_event_msgq = NULL;
pthread_mutex_t app_mutex = PTHREAD_MUTEX_INITIALIZER;

static void print_prompt(void)
{
	printf(COLOR_BLUE "[mesh]" COLOR_OFF "# ");
	fflush(stdout);
}

static int sigfd_setup(void)
{
	sigset_t mask;
	int fd;

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGTERM);

	if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
		perror("Failed to set signal mask");
		return -1;
	}

	fd = signalfd(-1, &mask, 0);
	if (fd < 0) {
		perror("Failed to create signal descriptor");
		return -1;
	}

	return fd;
}

bool is_hex(char *p)
{
	bool hex = false;

	if (p[0] == 'x' || p[0] == 'X') {
		hex = true;
	} else if ((p[0] == '0') && (p[1] == 'x' || p[1] == 'X')) {
		hex = true;
	}

	return hex;
}

static void process_cmdline(char *input_str, uint32_t len)
{
	char *cmd, *arg;
	int i;
	char *argv[16];
	int argc = 0;
	char *p;
	int index = 0;

	/* If user enter CTL + d, program will read an EOF and len
	 * is zero */
	if (!len) {
		printf("empty command\n");
		goto done;
	}

	if (!strlen(input_str))
		goto done;

	if (input_str[0] == '\n' || input_str[0] == '\r')
		goto done;

	if (input_str[len - 1] == '\n' || input_str[len - 1] == '\r')
		input_str[len - 1] = 0;

	/* rtb_printf("input_str: %s\n", input_str); */

	cmd = strtok_r(input_str, " ", &arg);
	if (!cmd)
		goto done;

	if (arg) {
		int len = strlen(arg);
		if (len > 0 && arg[len - 1] == ' ')
			arg[len - 1] = '\0';
	}
	/* rtb_printf("cmd %s -\n", cmd); */
	/* rtb_printf("arg %s -\n", arg); */
	argc = 0;
	while ((p = strtok_r(NULL," ",&arg))) {
		if (*p) {
			printf("Command parameter %d: %s\n", argc, p);
			argv[argc++] = p;
		}
	}
	for (index = 0; cmd_table[index].cmd; index++) {
		if (strcmp(cmd, cmd_table[index].cmd))
			continue;

		if (cmd_table[index].func) {
			user_cmd_parse_value_t v;
			memset(&v, 0, sizeof(v));
			v.pcommand = cmd;
			v.para_count = argc;
			for (i = 0; i < argc; i++) {
				v.pparameter[i] = argv[i];
				if (is_hex(argv[i]))
					v.dw_parameter[i] = strtoul(argv[i], NULL, 16);
				else
					v.dw_parameter[i] = strtoul(argv[i], NULL, 10);
				printf("pp[%d]: |%s|, dw[%d]: %u\n", i,
				       v.pparameter[i], i, v.dw_parameter[i]);
			}
			cmd_table[index].func(&v);
			goto done;
		}
	}

	if (strcmp(cmd, "help")) {
		printf("Invalid command\n");
		goto done;
	}

	printf("Available commands:\n");

	for (i = 0; cmd_table[i].cmd; i++) {
		if (cmd_table[i].desc)
			printf("  %s %-*s %s\n", cmd_table[i].cmd,
					(int)(25 - strlen(cmd_table[i].cmd)),
					cmd_table[i].arg ? : "",
					cmd_table[i].desc ? : "");
	}
done:
	print_prompt();
	return;
}

static void stdin_read_handler(int fd)
{
	ssize_t read;
	size_t len = 0;
	char *line = NULL;

	read = getline(&line, &len, stdin);
	if (read < 0)
		return;

	if (read <= 1) {
		print_prompt();
		return;
	}

	line[read-1] = '\0';

	process_cmdline(line, strlen(line) + 1);
	free(line);
}

static void app_mesh_msg_pipe_handler(int fd )
{
	uint8_t event;
	ssize_t result;
	void* pevent = NULL;
	pthread_mutex_lock(&app_mutex);
	result = read(fd, &event, 1);
	if (!result) {
		printf("%s: No message read\n", __func__);
		pthread_mutex_unlock(&app_mutex);
		return;
	}
	if (queue_peek_head(mesh_event_msgq)) {
		pevent = queue_pop_head(mesh_event_msgq);
	}
	pthread_mutex_unlock(&app_mutex);
	if(pevent == NULL)
	{
		printf("%s: No message read in msg queue\n", __func__);
		return;
	}
	printf("%s--\n", __func__);
	switch (event) {
	case APP_HANDLE_MESH_EVET:
		app_mesh_event_handler((MESH_BT_EVENT_T*)pevent);
		break;
	case APP_HANDLE_LLS_DATA:
		app_light_lightness_client_data_handler((MESH_MODEL_LLS_RECV_DATA_T*)pevent);
		break;
	case APP_HANDLE_GOOS_DATA:
		generic_on_off_client_data_handler((MESH_MODEL_GOOS_RECV_DATA_T*)pevent);
		break;
	case APP_HANDLE_CTL_TEMPRATUE:
		light_ctl_client_data_handler((MESH_MODEL_LC_RECV_DATA_T*)pevent);
		break;
	case APP_HANDLE_CFG_SERVICE_STATUS:
		config_client_data_handler((MESH_MODEL_RECV_DATA_T*)pevent);
		break;
	case APP_HANDLE_HSL_DATA:
		light_hsl_client_data_handler((MESH_MODEL_RECV_DATA_T *)pevent);
		break;
	default:
		printf("Unknown message 0x%02x\n", event);
		break;
	}
	if(pevent != NULL)
	{
		free(pevent);
		pevent = NULL;

	}
	return;
}
static void wait_for_state_timeout_handler()
{
	struct itimerspec itimer;
	memset(&itimer, 0, sizeof(itimer));
	timerfd_settime(waitForStateTimer_fd, 0, &itimer, NULL);

	printf("**** retry to get lingt on/off status...\n");
	rtkmesh_generic_on_off_get(element_index_record, dst_record, appkeyindex_g_record);
	inquiry_retry_count --;
	if(inquiry_retry_count > 0)
	{
		itimer.it_interval.tv_sec = 0;
		itimer.it_interval.tv_nsec = 0;

		itimer.it_value.tv_sec = timer_interval/1000000000;
		itimer.it_value.tv_nsec = timer_interval - (timer_interval/1000000000)*1000000000;

		timerfd_settime(waitForStateTimer_fd, 0, &itimer, NULL);
	}

	return;
}

static void on_off_timeout_handler()
{
	struct itimerspec itimer;
	static uint8_t OnOff = 0;
	int32_t ret = 0;
	generic_transition_time_t trans_time;

	OnOff = !OnOff;

	memset(&trans_time,0,sizeof(generic_transition_time_t));
	memset(&itimer, 0, sizeof(itimer));
	timerfd_settime(OnOffTestTimer_fd, 0, &itimer, NULL);


	{
		ret = rtkmesh_generic_on_off_set(element_index_record, dst_record, appkeyindex_g_record, OnOff,
										0, trans_time, 0, 1);
		if(ret == 0)
		{

			struct itimerspec itimer;
			memset(&itimer, 0, sizeof(itimer));

			itimer.it_interval.tv_sec = 0;
			itimer.it_interval.tv_nsec = 0;

			itimer.it_value.tv_sec = timer_interval/1000000000;
			itimer.it_value.tv_nsec = timer_interval- (timer_interval/1000000000)*1000000000;

			timerfd_settime(waitForStateTimer_fd, 0, &itimer, NULL);
			inquiry_retry_count = status_get_retry;
			printf(">>>>mesh goo set success\n");
		}
		else
		{
			printf(">>>>mesh goo set fail, err code %d\n", ret);
		}

	}
	goo_send_count ++;
	need_one_response = 1;

	printf("---------------S: %d, R: %d\n",goo_send_count,goo_recv_count);

	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = 0;

	itimer.it_value.tv_sec = 10;
	itimer.it_value.tv_nsec = 0;

	timerfd_settime(OnOffTestTimer_fd, 0, &itimer, NULL);

	return;
}

static void queue_entry_data_destroy(void *data)
{
	free(data);
}

int main(int argc, char *argv[])
{
	int sigfd;
	struct pollfd pfd[5];
	char *timername="wait_for_state_timer";

	sigfd = sigfd_setup();
	if (sigfd < 0)
		return EXIT_FAILURE;

	if (pipe(app_mesh_msg_pipe) < 0) {
		printf("Pipe error\n");
		close(sigfd);
		return EXIT_FAILURE;
	}
	if (!mesh_event_msgq)
	{
		mesh_event_msgq = queue_new();
	}

	waitForStateTimer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (waitForStateTimer_fd == -1) {
		printf("timerfd_create error\n");
		return -1;
	}

	OnOffTestTimer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (OnOffTestTimer_fd == -1) {
		printf("timerfd_create error\n");
		return -1;
	}

	pfd[0].fd = sigfd;
	pfd[0].events = POLLIN | POLLHUP | POLLERR;
	pfd[1].fd = fileno(stdin);
	pfd[1].events = POLLIN | POLLHUP | POLLERR;
	pfd[2].fd = app_mesh_msg_pipe[0];
	pfd[2].events = POLLIN | POLLHUP | POLLERR;
	pfd[3].fd = waitForStateTimer_fd;
	pfd[3].events = POLLIN | POLLHUP | POLLERR;
	pfd[4].fd = OnOffTestTimer_fd;
	pfd[4].events = POLLIN | POLLHUP | POLLERR;

	print_prompt();

	while (!__main_terminated) {
		pfd[0].revents = 0;
		pfd[1].revents = 0;
		pfd[2].revents = 0;
		pfd[3].revents = 0;
		pfd[4].revents = 0;
		if (poll(pfd, 5, -1) == -1) {
			if (errno == EINTR)
				continue;
			APP_PRINT_ERROR1("Poll error: %s", strerror(errno));
			return EXIT_FAILURE;
		}

		if (pfd[0].revents & (POLLHUP | POLLERR)) {
			APP_PRINT_ERROR0("Poll rdhup or hup or err");
			return EXIT_FAILURE;
		}

		if (pfd[1].revents & (POLLHUP | POLLERR)) {
			APP_PRINT_ERROR0("Poll rdhup or hup or err");
			return EXIT_FAILURE;
		}

		if (pfd[0].revents & POLLIN) {
			struct signalfd_siginfo si;
			ssize_t ret;

			ret = read(pfd[0].fd, &si, sizeof(si));
			if (ret != sizeof(si))
				return EXIT_FAILURE;
			switch (si.ssi_signo) {
			case SIGINT:
			case SIGTERM:
				__main_terminated = 1;
				break;
			}
		}

		if (pfd[1].revents & POLLIN)
			stdin_read_handler(pfd[1].fd);

		if (pfd[2].revents & POLLIN)
			app_mesh_msg_pipe_handler(pfd[2].fd);

		if (pfd[3].revents & POLLIN)
			wait_for_state_timeout_handler();

		if (pfd[4].revents & POLLIN)
			on_off_timeout_handler();

	}
	printf("--\n");
	quit_mesh_bta_thread();
	close(sigfd);

	pthread_mutex_lock(&app_mutex);
	queue_destroy(mesh_event_msgq, queue_entry_data_destroy);
	mesh_event_msgq = NULL;
	pthread_mutex_unlock(&app_mutex);

	printf("exit\n");
	return 0;
}

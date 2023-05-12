/*
 *  Copyright (C) 2019 Realtek Semiconductor Corporation.
 *
 *  Author: Alex Lu <alex_lu@realsil.com.cn>
 */

#include <poll.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <shared/queue.h>
#include <pthread.h>
#include "trace.h"

/* #define DEBUG */
//#include "log.h"
#include "RTKMeshTypes.h"


static struct queue *mesh_bta_cmdq = NULL;
static pthread_mutex_t cmd_mutex = PTHREAD_MUTEX_INITIALIZER;
static int commfds[2];
static pthread_t mesh_bta_mainloop_tid;
static bcmd_handler_t pmesh_bta_cmd_handler = mesh_bta_cmd_process ;

void set_mesh_bta_mainloop_id(pthread_t tid)
{
	APP_PRINT_TRACE0("set mesh tid");
	if (!mesh_bta_cmdq)
		mesh_bta_cmdq = queue_new();
	else
		APP_PRINT_WARN0("Mesh blue cmd queue already exists");

    //printf("set_mesh_bta_mainloop_id tid =%d",tid);
	if (tid != 0)
		mesh_bta_mainloop_tid = tid;
	else
		APP_PRINT_ERROR0("Invalid thread id 0");
}

void set_mesh_bta_cmd_fd(int fd[2])
{
	commfds[0] = fd[0]; /* 0: bluetooth read/write */
	commfds[1] = fd[1]; /* 1: caller read/write */

	APP_PRINT_TRACE2("commfds %d %d", fd[0], fd[1]);
}

void mesh_bta_register_handler(bcmd_handler_t handler)
{
	pmesh_bta_cmd_handler = handler;
}

static void cmd_destroy(void *data)
{
	free(data);
}

void mesh_bta_clear_commands(void)
{
	pthread_mutex_lock(&cmd_mutex);
	queue_destroy(mesh_bta_cmdq, cmd_destroy);
	mesh_bta_cmdq = NULL;
	pthread_mutex_unlock(&cmd_mutex);
}

int mesh_bta_submit_command_wait(uint16_t opcode, int argc, void **argv)
{
	struct mesh_bta_command *cmd;
	static uint32_t cmd_id = 1;
	uint8_t c = BTA_WAKE_CODE;
	struct pollfd p[1];
	ssize_t ret;
	int result = -1;

	if (mesh_bta_mainloop_tid == 0) {
		APP_PRINT_ERROR0("Please set bluetooth thread id");
		return -1;
	}

	/* Running in the bta mainloop */
	if (mesh_bta_mainloop_tid == pthread_self()) {
		/* call lib api directly
		 * result = xxxx(...)
		 * */
		APP_PRINT_INFO0("Calling api directly");
		if (pmesh_bta_cmd_handler)
			result = pmesh_bta_cmd_handler(opcode, argc, argv);
		else
			result = -1;
		return result;
	}

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return -1;
	cmd->opcode = opcode;
	cmd->argc = argc;
	cmd->argv = argv;
	cmd->req_status = REQ_PENDING;
	cmd->req_result = 0;
	memset(cmd->rparams, 0, sizeof(cmd->rparams));
	if (cmd_id < 1)
		cmd_id = 1;
	cmd->id = cmd_id++;
	pthread_mutex_lock(&cmd_mutex);
	if (!queue_push_tail(mesh_bta_cmdq, cmd)) {
		APP_PRINT_ERROR0("Couldn't enqueue cmd");
		pthread_mutex_unlock(&cmd_mutex);
		goto free;
	}
	/* Wake up the bluetooth thread */
	ret = write(commfds[1], &c, 1);
	if (ret != 1) {
		APP_PRINT_ERROR0("Couldn't wake up bluetooth thread");
		queue_remove(mesh_bta_cmdq, cmd);
		pthread_mutex_unlock(&cmd_mutex);
		goto free;
	}
	pthread_mutex_unlock(&cmd_mutex);

	p[0].fd = commfds[1];
	p[0].events = POLLIN | POLLERR | POLLHUP;

	while (1) {
		p[0].revents = 0;
		result = poll(p, 1, 1800); /* 1800 msec */
		if (result <= 0 || p[0].revents & (POLLERR | POLLHUP)) {
			if (result == 0)
				APP_PRINT_ERROR1("Poll timeout, %04x", cmd->opcode);
			else if (result < 0)
				APP_PRINT_ERROR2("Poll error, %s, opcode %04x",
					 strerror(errno), opcode);
			else
				APP_PRINT_ERROR1("Poll err or hup %d", p[0].revents);
			/* NOTE if the cmd is running too long, the mutex lock
			 * will stick here. That should never happen */
			pthread_mutex_lock(&cmd_mutex);
			queue_remove(mesh_bta_cmdq, cmd);
			result = -1;
			pthread_mutex_unlock(&cmd_mutex);
			break;
		}

		/* Sometime the byte is read by other thread, but that doesn't
		 * matter */
		ret = read(commfds[1], &c, 1);

		pthread_mutex_lock(&cmd_mutex);
		if (cmd->req_status == REQ_DONE) {
			result = cmd->req_result;
			pthread_mutex_unlock(&cmd_mutex);
			break;
		}
		pthread_mutex_unlock(&cmd_mutex);
	}

free:
	free(cmd);

	return result;
}

/* This function runs in bluetooth thread context */
void mesh_bta_run_command(void)
{
	struct mesh_bta_command *cmd = NULL;
	ssize_t ret;
	uint8_t c = BTA_WAKE_CODE;
	int result;

	APP_PRINT_TRACE0(" mesh_bta_run_command run");
    //printf("\nmesh_bta_mainloop_tid = %d,pthread_self=%d\n",mesh_bta_mainloop_tid,pthread_self());

	//if (mesh_bta_mainloop_tid != pthread_self()) {
	//	APP_PRINT_ERROR0("\nRun in the wrong context \n");
	//	
    //  printf("\nin mesh_bta_mainloop_tid = %d,pthread_self=%d\n",mesh_bta_mainloop_tid,pthread_self());
	//	return;
	//}

	pthread_mutex_lock(&cmd_mutex);
	if (queue_peek_head(mesh_bta_cmdq)) {
		cmd = queue_pop_head(mesh_bta_cmdq);
		/* Should never block long time here */
		if (pmesh_bta_cmd_handler)
			result = pmesh_bta_cmd_handler(cmd->opcode, cmd->argc,
						 cmd->argv);
		else
			result = -1;
		cmd->req_status = REQ_DONE;
		cmd->req_result = result;

		APP_PRINT_TRACE1("Wake up thread waiting result=%d",result);
		/* Wake up the thread that issues command */
		ret = write(commfds[0], &c, 1);
		if (ret < 0) {
			if (errno == EINTR || errno == EAGAIN)
				ret = write(commfds[0], &c, 1);
			if (ret < 0) {
				APP_PRINT_ERROR1("Execute %04x failed", cmd->opcode);
				cmd->req_result = -1;
			}
		} else if (ret == 0)
			APP_PRINT_ERROR0("Can't write signal to sk fd");
	}
	pthread_mutex_unlock(&cmd_mutex);

	/* No longer access cmd */
}
void mesh_bta_deinit(void)
{
	mesh_bta_clear_commands();
	mesh_bta_mainloop_tid = 0;
}


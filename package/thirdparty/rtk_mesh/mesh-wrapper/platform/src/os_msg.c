#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "shared/queue.h"  // /bluez/.../queue.h
#include "os_msg.h"
#include "trace.h"

/* queue */
struct queue_wrapper {
	struct queue *queue;
	uint32_t item_size;
	uint32_t item_num;
};

struct data_entry {
	uint8_t *buf;
};

bool os_msg_queue_create(void **pp_handle, uint32_t item_num,
			 uint32_t item_size)
{
	struct queue_wrapper *qw = (void *)malloc(sizeof(struct queue_wrapper));

	if (!qw) {
		*pp_handle = NULL;
		OSIF_PRINT_ERROR0("queue_wrapper malloc error\n");
		return false;
	}

	qw->queue = queue_new();
	if (!qw->queue) {
		OSIF_PRINT_ERROR0("queue malloc error\n");
		free(qw);
		*pp_handle = NULL;
		return false;
	}
	qw->item_size = item_size;
	qw->item_num = item_num;

	*pp_handle = qw;
	return true;
}

bool os_msg_send(void *p_handle, void *p_msg, uint32_t wait_ms)
{
	struct queue_wrapper *qw = p_handle;
	struct data_entry *data = (void *)malloc(sizeof(struct data_entry));

	if (!data) {
		return false;
	}

	data->buf = (uint8_t *) malloc(qw->item_size);
	if (!data->buf) {
		free(data);
		return false;
	}
	memcpy(data->buf, p_msg, qw->item_size);

	return queue_push_tail(qw->queue, data);

}

bool os_msg_recv(void *p_handle, void *p_msg, uint32_t wait_ms)
{
	struct queue_wrapper *qw = p_handle;
	struct data_entry *data = NULL;

	data = queue_pop_head(qw->queue);
	if (!data)
		return false;
	memcpy(p_msg, data->buf, qw->item_size);

	free(data->buf);
	free(data);
	return true;
}

void os_msg_queue_destory(void *p_handle)
{
	struct queue_wrapper *qw = p_handle;

	queue_destroy(qw->queue, NULL);
	free(qw);
}

bool os_event_send(void *p_handle, void *p_msg, uint32_t wait_ms)
{
	ssize_t result = 0;
	result = write(*(int *)p_handle, p_msg, 1);
	if( result == 1)
	{
		return true;
	}
	if(result == 0)
	{
		OSIF_PRINT_ERROR0("err: send event failed");
		return false;
	}
	if( result < 0)
	{
		OSIF_PRINT_ERROR1("err: %s", strerror(errno));
		return false;
	}

	OSIF_PRINT_ERROR0(" never be happened");
	return false;
}

bool os_event_recv(void *p_handle, void *p_msg, uint32_t wait_ms)
{
	ssize_t result = 0;
	result = read(*(int *)p_handle, p_msg, 1);
	if( result == 1)
	{
		return true;
	}
	if(result == 0)
	{
		OSIF_PRINT_ERROR0("err: recv event failed");
		return false;
	}
	if( result < 0)
	{
		OSIF_PRINT_ERROR1("err: %s", strerror(errno));
		return false;
	}
	OSIF_PRINT_ERROR0(" never be happened");
	return false;
}




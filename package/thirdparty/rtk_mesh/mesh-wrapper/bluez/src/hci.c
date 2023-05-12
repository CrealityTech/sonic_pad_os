/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2012-2014  Intel Corporation. All rights reserved.
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
#define LOG_TAG "rtkmesh_shared-hci"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <pthread.h>

#include <stdint.h>

#include "monitor/bt.h"
#include "shared/mainloop.h"
#include "shared/io.h"
#include "shared/util.h"
#include "shared/queue.h"
#include "shared/hci.h"
#include "trace.h"

#define BTPROTO_HCI	1
struct sockaddr_hci {
	sa_family_t	hci_family;
	unsigned short	hci_dev;
	unsigned short  hci_channel;
};
#define HCI_CHANNEL_RAW		0
#define HCI_CHANNEL_USER	1

#define SOL_HCI		0
#define HCI_FILTER	2
struct hci_filter {
	uint32_t type_mask;
	uint32_t event_mask[2];
	uint16_t opcode;
};

struct bt_hci {
	int ref_count;
	struct io *io;
	bool is_stream;
	bool writer_active;
	uint8_t num_cmds;
	unsigned int next_cmd_id;
	unsigned int next_evt_id;
	struct queue *cmd_queue;
	struct queue *rsp_queue;
	struct queue *evt_list;
	uint8_t DataBuf[512];
	uint8_t DataLen;
};

struct cmd {
	unsigned int id;
	uint16_t opcode;
	void *data;
	uint8_t size;
	bt_hci_callback_func_t callback;
	bt_hci_destroy_func_t destroy;
	void *user_data;
};

struct evt {
	unsigned int id;
	uint8_t event;
	bt_hci_callback_func_t callback;
	bt_hci_destroy_func_t destroy;
	void *user_data;
};

static void cmd_free(void *data)
{
	struct cmd *cmd = data;

	if (cmd->destroy)
		cmd->destroy(cmd->user_data);

	free(cmd->data);
	free(cmd);
}

static void evt_free(void *data)
{
	struct evt *evt = data;

	if (evt->destroy)
		evt->destroy(evt->user_data);

	free(evt);
}

static void send_command(struct bt_hci *hci, uint16_t opcode,
						void *data, uint8_t size)
{
	uint8_t type = BT_H4_CMD_PKT;
	struct bt_hci_cmd_hdr hdr;
	struct iovec iov[3];
	int iovcnt;

	//DFU_PRINT_TRACE0("start");
	if (hci->num_cmds < 1)
		return;

	//DFU_PRINT_TRACE1("opcode %04x", opcode);


	hdr.opcode = cpu_to_le16(opcode);
	hdr.plen = size;

	iov[0].iov_base = &type;
	iov[0].iov_len  = 1;
	iov[1].iov_base = &hdr;
	iov[1].iov_len  = sizeof(hdr);

	if (size > 0) {
		iov[2].iov_base = data;
		iov[2].iov_len  = size;
		iovcnt = 3;
	} else
		iovcnt = 2;

	if (io_send2(hci->io, iov, iovcnt) < 0)
		return;

	//DFU_PRINT_TRACE0("end");
	hci->num_cmds--;
}

static bool io_write_callback(struct io *io, void *user_data)
{
	struct bt_hci *hci = user_data;
	struct cmd *cmd;

	//DFU_PRINT_TRACE1("%lu", pthread_self());
	cmd = queue_pop_head(hci->cmd_queue);
	if (cmd) {
		send_command(hci, cmd->opcode, cmd->data, cmd->size);
		queue_push_tail(hci->rsp_queue, cmd);
	}

	hci->writer_active = false;

	return false;
}

static void wakeup_writer(struct bt_hci *hci)
{

	/*if (hci->writer_active)
		return;

	if (hci->num_cmds < 1)
		return;

	if (queue_isempty(hci->cmd_queue))
		return;

	DFU_PRINT_TRACE1("%lu", pthread_self());
	if (!io_set_write_handler2(hci->io, io_write_callback, hci, NULL))
		return;

	DFU_PRINT_TRACE0("step 5");
	hci->writer_active = true;*/
	if(hci->num_cmds < 1)
		return;
		
	if(queue_isempty(hci->cmd_queue))
		return;
#ifdef MESH_ANDROID
	//wait for the hci event of the previous  hci cmd ...
	if(!queue_isempty(hci->rsp_queue))
	{
		DFU_PRINT_ERROR0("wait for the hci event of the previous  hci cmd ... ");
		return;
	}
#endif
	if(io_write_callback(hci->io, hci) == false)
	{
		//DFU_PRINT_ERROR0("io_write_callback false");
	}

}

static bool match_cmd_opcode(const void *a, const void *b)
{
	const struct cmd *cmd = a;
	uint16_t opcode = PTR_TO_UINT(b);

	return cmd->opcode == opcode;
}

static void process_response(struct bt_hci *hci, uint16_t opcode,
					const void *data, size_t size)
{
	struct cmd *cmd;

	if (opcode == BT_HCI_CMD_NOP)
		goto done;

	cmd = queue_remove_if(hci->rsp_queue, match_cmd_opcode,
						UINT_TO_PTR(opcode));
	if (!cmd)
		return;

	if (cmd->callback)
		cmd->callback(data, size, cmd->user_data);

	cmd_free(cmd);

done:
	wakeup_writer(hci);
}

static void process_notify(void *data, void *user_data)
{
	struct bt_hci_evt_hdr *hdr = user_data;
	struct evt *evt = data;

	if (evt->event == hdr->evt)
		evt->callback((uint8_t*)user_data + sizeof(struct bt_hci_evt_hdr),
						hdr->plen, evt->user_data);
}

static void process_event(struct bt_hci *hci, const void *data, size_t size)
{
	const struct bt_hci_evt_hdr *hdr = data;
	const struct bt_hci_evt_cmd_complete *cc;
	const struct bt_hci_evt_cmd_status *cs;

	if (size < sizeof(struct bt_hci_evt_hdr))
		return;

	data = (uint8_t*)data + sizeof(struct bt_hci_evt_hdr);
	size -= sizeof(struct bt_hci_evt_hdr);

	if (hdr->plen != size)
		return;

	switch (hdr->evt) {
	case BT_HCI_EVT_CMD_COMPLETE:
		if (size < sizeof(*cc))
			return;
		cc = data;
		hci->num_cmds = cc->ncmd;
		//DFU_PRINT_TRACE1("complete, %lu", pthread_self());
		process_response(hci, le16_to_cpu(cc->opcode),
						(uint8_t*)data + sizeof(*cc),
						size - sizeof(*cc));
		break;

	case BT_HCI_EVT_CMD_STATUS:
		if (size < sizeof(*cs))
			return;
		cs = data;
		hci->num_cmds = cs->ncmd;
		process_response(hci, le16_to_cpu(cs->opcode), &cs->status, 1);
		break;

	default:
		queue_foreach(hci->evt_list, process_notify, (void *) hdr);
		break;
	}
}

static bool io_read_callback(struct io *io, void *user_data)
{
	struct bt_hci *hci = user_data;
	uint8_t buf[512];
	ssize_t len;
	uint16_t packet_len;
	ssize_t remain_len;
	uint16_t index = 0;
	int fd;

	fd = io_get_fd2(hci->io);
	if (fd < 0)
		return false;

	if (hci->is_stream)
		return false;

	len = read(fd, buf, sizeof(buf));
	if (len < 0)
		return false;

	if (len < 1)
		return true;

	remain_len = len;
	
	while(remain_len !=0 )
	{
		if(hci->DataLen == 0)
		{
			switch(buf[index])
			{
				case BT_H4_EVT_PKT:
				{
					if(remain_len<3)
					{
						memcpy(hci->DataBuf,&buf[index],remain_len);
						hci->DataLen = remain_len;
						remain_len = 0;
					}
					else
					{
						packet_len = buf[index + 2]+3;
						if(packet_len > remain_len)
						{
							memcpy(hci->DataBuf,&buf[index],remain_len);
							hci->DataLen = remain_len;
							remain_len = 0;
						}
						else
						{
							memcpy(hci->DataBuf,&buf[index],packet_len);
							index = index + packet_len;
							remain_len = remain_len - packet_len;
							process_event(hci, hci->DataBuf + 1, packet_len - 1); 
							hci->DataLen = 0;
						}
					}
					break;
				}
				case BT_H4_SCO_PKT:
				{
					if(remain_len<4)
					{
						memcpy(hci->DataBuf,&buf[index],remain_len);
						hci->DataLen = remain_len;
						remain_len = 0;
					}
					else
					{
						packet_len = buf[index + 3]+4;
						if(packet_len > remain_len)
						{
							memcpy(hci->DataBuf,&buf[index],remain_len);
							hci->DataLen = remain_len;
							remain_len = 0;
						}
						else
						{
							memcpy(hci->DataBuf,&buf[index],packet_len);
							index += packet_len;
							remain_len -= packet_len;
							/*
								handle sco pkt;
							*/
							hci->DataLen = 0;
						}
					}
					break;
				}
				case BT_H4_ACL_PKT:
				{
					if(remain_len<5)
					{
						memcpy(hci->DataBuf,&buf[index],remain_len);
						hci->DataLen = remain_len;
						remain_len = 0;
					}
					else
					{
						packet_len = (buf[index + 4]<<8)+buf[index + 3]+5;
						if(packet_len > remain_len)
						{
							memcpy(hci->DataBuf,&buf[index],remain_len);
							hci->DataLen = remain_len;
							remain_len = 0;
						}
						else
						{
							memcpy(hci->DataBuf,&buf[index],packet_len);
							index = index + packet_len;
							remain_len = remain_len - packet_len;
							/*
								handle acl data;
							*/
							hci->DataLen = 0;
						}
					}
					break;
				}
				default:
					break;
			}
		}
		else
		{
			switch(hci->DataBuf[0])
			{
				case BT_H4_EVT_PKT:
				{
					if(hci->DataLen < 3)
					{
						packet_len = buf[2-hci->DataLen]+3-hci->DataLen;
					}
					else
					{
						packet_len = hci->DataBuf[2]+3-hci->DataLen;
					}
					
					if(packet_len > remain_len)
					{
						memcpy((hci->DataBuf+hci->DataLen),&buf[0],remain_len);
						hci->DataLen += remain_len;
						remain_len = 0;
					}
					else
					{
						memcpy((hci->DataBuf+hci->DataLen),&buf[0],packet_len);
						index += packet_len;
						remain_len -= packet_len;
						process_event(hci, hci->DataBuf + 1, packet_len + hci->DataLen  - 1); 
						hci->DataLen = 0;
					}
					break;
				}
				case BT_H4_SCO_PKT:
				{
					if(hci->DataLen < 4)
					{
						packet_len = buf[3-hci->DataLen]+4-hci->DataLen;
					}
					else
					{
						packet_len = hci->DataBuf[3]+4-hci->DataLen;
					}
					
					if(packet_len > remain_len)
					{
						memcpy((hci->DataBuf+hci->DataLen),&buf[0],remain_len);
						hci->DataLen += remain_len;
						remain_len = 0;
					}
					else
					{
						memcpy((hci->DataBuf+hci->DataLen),&buf[0],packet_len);
						index += packet_len;
						remain_len -= packet_len;
						/*
							handle sco pkt;
						*/
						hci->DataLen = 0;
					}
					break;
				}
				case BT_H4_ACL_PKT:
				{
					if(hci->DataLen < 4)
					{
						packet_len = (buf[4-hci->DataLen]<<8)+buf[3-hci->DataLen]+5-hci->DataLen;
					}
					else if(hci->DataLen < 5)
					{
						packet_len = (buf[0]<<8)+hci->DataBuf[3]+5-hci->DataLen;
					}
					else
					{
						packet_len = (hci->DataBuf[4]<<8)+hci->DataBuf[3]+5-hci->DataLen;
					}
					
					if(packet_len > remain_len)
					{
						memcpy((hci->DataBuf+hci->DataLen),&buf[0],remain_len);
						hci->DataLen += remain_len;
						remain_len = 0;
					}
					else
					{
						memcpy((hci->DataBuf+hci->DataLen),&buf[0],packet_len);
						index += packet_len;
						remain_len -= packet_len;
						/*
							handle acl pkt;
						*/
						hci->DataLen = 0;
					}
					break;
				}
				default:
					break;
			}			
		}
	}

	return true;
}

static struct bt_hci *create_hci(int fd)
{
	struct bt_hci *hci;

	if (fd < 0)
		return NULL;

	hci = new0(struct bt_hci, 1);
	hci->io = io_new2(fd);
	if (!hci->io) {
		DFU_PRINT_ERROR0("hci->io is NULL, io_new2 failed");
		free(hci);
		return NULL;
	}

	hci->is_stream = true;
	hci->writer_active = false;
	hci->num_cmds = 1;
	hci->next_cmd_id = 1;
	hci->next_evt_id = 1;

	hci->cmd_queue = queue_new();
	hci->rsp_queue = queue_new();
	hci->evt_list = queue_new();

	memset(hci->DataBuf,0,sizeof(hci->DataBuf));
	hci->DataLen = 0;

	if (!io_set_read_handler2(hci->io, io_read_callback, hci, NULL)) {
		queue_destroy(hci->evt_list, NULL);
		queue_destroy(hci->rsp_queue, NULL);
		queue_destroy(hci->cmd_queue, NULL);
		io_destroy2(hci->io);
		free(hci);
		return NULL;
	}

	return bt_hci_ref2(hci);
}

struct bt_hci *bt_hci_new2(int fd)
{
	struct bt_hci *hci;

	hci = create_hci(fd);
	if (!hci)
		return NULL;

	return hci;
}

static int create_socket(uint16_t index, uint16_t channel)
{
	struct sockaddr_hci addr;
	int fd;

	fd = socket(PF_BLUETOOTH, SOCK_RAW | SOCK_CLOEXEC | SOCK_NONBLOCK,
								BTPROTO_HCI);
	if (fd < 0)
		return -1;

	memset(&addr, 0, sizeof(addr));
	addr.hci_family = AF_BLUETOOTH;
	addr.hci_dev = index;
	addr.hci_channel = channel;

	if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		close(fd);
		return -1;
	}

	return fd;
}

struct bt_hci *bt_hci_new_user_channel(uint16_t index)
{
	struct bt_hci *hci;
	int fd;

	fd = create_socket(index, HCI_CHANNEL_USER);
	if (fd < 0)
		return NULL;

	hci = create_hci(fd);
	if (!hci) {
		close(fd);
		return NULL;
	}

	hci->is_stream = false;

	bt_hci_set_close_on_unref2(hci, true);

	return hci;
}

struct bt_hci *bt_hci_new_raw_device(uint16_t index)
{
	struct bt_hci *hci;
	struct hci_filter flt;
	int fd;

	fd = create_socket(index, HCI_CHANNEL_RAW);
	if (fd < 0)
		return NULL;

	memset(&flt, 0, sizeof(flt));
	flt.type_mask = 1 << BT_H4_EVT_PKT;
	flt.event_mask[0] = 0xffffffff;
	flt.event_mask[1] = 0xffffffff;

	if (setsockopt(fd, SOL_HCI, HCI_FILTER, &flt, sizeof(flt)) < 0) {
		close(fd);
		return NULL;
	}

	hci = create_hci(fd);
	if (!hci) {
		close(fd);
		return NULL;
	}

	hci->is_stream = false;

	bt_hci_set_close_on_unref2(hci, true);

	return hci;
}

struct bt_hci *bt_hci_ref2(struct bt_hci *hci)
{
	if (!hci)
	{
		DFU_PRINT_ERROR0("hci is null");
		return NULL;

	}

	__sync_fetch_and_add(&hci->ref_count, 1);

	return hci;
}

void bt_hci_unref2(struct bt_hci *hci)
{
	if (!hci)
		return;

	if (__sync_sub_and_fetch(&hci->ref_count, 1))
		return;

	queue_destroy(hci->evt_list, evt_free);
	queue_destroy(hci->cmd_queue, cmd_free);
	queue_destroy(hci->rsp_queue, cmd_free);

	io_destroy2(hci->io);

	free(hci);
}

bool bt_hci_set_close_on_unref2(struct bt_hci *hci, bool do_close)
{
	if (!hci)
		return false;

	return io_set_close_on_destroy2(hci->io, do_close);
}

unsigned int bt_hci_send2(struct bt_hci *hci, uint16_t opcode,
				const void *data, uint8_t size,
				bt_hci_callback_func_t callback,
				void *user_data, bt_hci_destroy_func_t destroy)
{
	struct cmd *cmd;

	//DFU_PRINT_TRACE0("start");
	if (!hci)
		return 0;

	cmd = new0(struct cmd, 1);
	cmd->opcode = opcode;
	cmd->size = size;

	if (cmd->size > 0) {
		cmd->data = malloc(cmd->size);
		if (!cmd->data) {
			free(cmd);
			return 0;
		}

		memcpy(cmd->data, data, cmd->size);
	}

	if (hci->next_cmd_id < 1)
		hci->next_cmd_id = 1;

	cmd->id = hci->next_cmd_id++;

	cmd->callback = callback;
	cmd->destroy = destroy;
	cmd->user_data = user_data;

	if (!queue_push_tail(hci->cmd_queue, cmd)) {
		free(cmd->data);
		free(cmd);
		return 0;
	}

	wakeup_writer(hci);

	return cmd->id;
}

static bool match_cmd_id(const void *a, const void *b)
{
	const struct cmd *cmd = a;
	unsigned int id = PTR_TO_UINT(b);

	return cmd->id == id;
}

bool bt_hci_cancel(struct bt_hci *hci, unsigned int id)
{
	struct cmd *cmd;

	if (!hci || !id)
		return false;

	cmd = queue_remove_if(hci->cmd_queue, match_cmd_id, UINT_TO_PTR(id));
	if (!cmd) {
		cmd = queue_remove_if(hci->rsp_queue, match_cmd_id,
							UINT_TO_PTR(id));
		if (!cmd)
			return false;
	}

	cmd_free(cmd);

	wakeup_writer(hci);

	return true;
}

bool bt_hci_flush(struct bt_hci *hci)
{
	if (!hci)
		return false;

	if (hci->writer_active) {
		io_set_write_handler2(hci->io, NULL, NULL, NULL);
		hci->writer_active = false;
	}

	queue_remove_all(hci->cmd_queue, NULL, NULL, cmd_free);
	queue_remove_all(hci->rsp_queue, NULL, NULL, cmd_free);

	return true;
}

unsigned int bt_hci_register2(struct bt_hci *hci, uint8_t event,
				bt_hci_callback_func_t callback,
				void *user_data, bt_hci_destroy_func_t destroy)
{
	struct evt *evt;

	//DFU_PRINT_TRACE0("start");

	if (!hci)
		return 0;

	evt = new0(struct evt, 1);
	evt->event = event;

	if (hci->next_evt_id < 1)
		hci->next_evt_id = 1;

	evt->id = hci->next_evt_id++;

	evt->callback = callback;
	evt->destroy = destroy;
	evt->user_data = user_data;

	if (!queue_push_tail(hci->evt_list, evt)) {
		free(evt);
		return 0;
	}

	return evt->id;
}

static bool match_evt_id(const void *a, const void *b)
{
	const struct evt *evt = a;
	unsigned int id = PTR_TO_UINT(b);

	return evt->id == id;
}

bool bt_hci_unregister2(struct bt_hci *hci, unsigned int id)
{
	struct evt *evt;

	if (!hci || !id)
		return false;

	evt = queue_remove_if(hci->evt_list, match_evt_id, UINT_TO_PTR(id));
	if (!evt)
		return false;

	evt_free(evt);

	return true;
}

bool bt_hci_set_stream2(struct bt_hci *hci, bool is_stream)
{
	if (!hci)
		return false;

	hci->is_stream = is_stream;
	return true;
}

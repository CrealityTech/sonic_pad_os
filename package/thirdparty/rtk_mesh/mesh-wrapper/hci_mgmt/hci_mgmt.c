/*
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2012-2014  Intel Corporation. All rights reserved.
 *  Copyright (C) 2018 Realtek Semiconductor Corporation.
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
 */

#include <stdio.h>
#include <stdint.h>
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
#include <stdbool.h>
#include <unistd.h>

#include <shared/hci.h> //bluez/inc/shared/hci.h
#include "trace.h"

#ifdef MESH_ANDROID
#include <sys/un.h>
#include <utils/Log.h>
#endif
#define BTPROTO_HCI	1
#define SOL_HCI		0
#define HCI_FILTER	2

struct sockaddr_hci {
	sa_family_t	hci_family;
	unsigned short	hci_dev;
	unsigned short  hci_channel;
};

struct hci_mgmt {
	int fd;
	uint16_t dev_id;
	struct bt_hci *hci;
	void *user_data;
};

struct hci_filter {
	uint32_t type_mask;
	uint32_t event_mask[2];
	uint16_t opcode;
};

unsigned int hci_mgmt_send(struct hci_mgmt *mgmt, uint16_t opcode,
			   const void *param, uint8_t size,
			   bt_hci_callback_func_t callback, void *user_data,
			   bt_hci_destroy_func_t destroy)
{
	unsigned int id;

	if (!mgmt || !mgmt->hci) {
		DFU_PRINT_ERROR1("%s: Invalid hci mgmt\n", __func__);
		return 0;
	}

	id = bt_hci_send2(mgmt->hci, opcode, param, size, callback, user_data,
			 destroy);
	if (!id) {
		DFU_PRINT_ERROR0("Failed to send hci command\n");
		return 0;
	}

	return id;
}

unsigned int hci_mgmt_register(struct hci_mgmt *mgmt, uint8_t event,
			       bt_hci_callback_func_t callback, void *user_data,
			       bt_hci_destroy_func_t destroy)
{
	unsigned int id;

	if (!mgmt || !mgmt->hci) {
		DFU_PRINT_ERROR1("%s: Invalid hci mgmt\n", __func__);
		return 0;
	}

	id = bt_hci_register2(mgmt->hci, event, callback, user_data, destroy);

	if (!id) {
		DFU_PRINT_ERROR0("Failed to register hci event callback\n");
		return 0;
	}

	return id;
}

bool hci_mgmt_unregister(struct hci_mgmt *mgmt, unsigned int id)
{
	if (!mgmt || !mgmt->hci) {
		DFU_PRINT_ERROR1("%s: Invalid hci mgmt\n", __func__);
		return 0;
	}

	return bt_hci_unregister2(mgmt->hci, id);
}

void hci_mgmt_release(struct hci_mgmt *mgmt)
{
	DFU_PRINT_INFO0("Release hci mgmt\n");
	if (!mgmt || !mgmt->hci)
		return;

	DFU_PRINT_INFO1("%s: Free resources\n", __func__);
	/* It refers to hci when calling bt_hci_new2() */
	bt_hci_unref2(mgmt->hci);
	free(mgmt);
}

struct hci_mgmt *hci_mgmt_new_default(uint16_t dev_id, int socket_fd)
{
	struct bt_hci *hci;
	struct sockaddr_hci addr;
	struct hci_filter nf;
	int fd;
	uint32_t *p;
	int nr;
	int result;
	struct hci_mgmt *mgmt;

	mgmt = malloc(sizeof(*mgmt));
	if (!mgmt) {
		DFU_PRINT_ERROR0("Failed to allocate hci mgmt\n");
		return NULL;
	}

#ifdef MESH_ANDROID
  if(socket_fd == -1)
    goto err1;
  fd = socket_fd;
#else
	fd = socket(AF_BLUETOOTH, SOCK_RAW | SOCK_CLOEXEC, BTPROTO_HCI);
	if (fd < 0) {
		DFU_PRINT_ERROR0("Create hci socket failed \n");
		goto err1;
	}

	DFU_PRINT_INFO1("hci socket fd is %d\n", fd);

	memset(&addr, 0, sizeof(addr));
	addr.hci_family = AF_BLUETOOTH;
	addr.hci_dev = dev_id;
	result = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (result < 0) {
		DFU_PRINT_ERROR0("bind failed\n");
		goto err2;
	}

	memset(&nf, 0, sizeof(nf));
	nf.type_mask |= (1 << 0x04); /* HCI event */
	p = nf.event_mask;
	nr = 0x0e;
	*(p + (nr >> 5)) |= (1 << (nr & 31));
	nr = 0x0f;
	*(p + (nr >> 5)) |= (1 << (nr & 31));
	nr = 0x3e;
	*(p + (nr >> 5)) |= (1 << (nr & 31));
	result = setsockopt(fd, SOL_HCI, HCI_FILTER, &nf, sizeof(nf));
	if (result < 0) {
		DFU_PRINT_ERROR0("setsockopt failed\n");
		goto err2;
	}
#endif

	hci = bt_hci_new2(fd);
	if (!hci) {
		DFU_PRINT_ERROR0("Create hci raw device error\n");
		goto err2;
	}

	bt_hci_set_close_on_unref2(hci, true);
	bt_hci_set_stream2(hci, false);

	mgmt->fd = fd;
	mgmt->hci = hci;
	mgmt->dev_id = dev_id;
	mgmt->user_data = NULL;

	return mgmt;

err2:
	close(fd);
err1:
	free(mgmt);
	return NULL;
}

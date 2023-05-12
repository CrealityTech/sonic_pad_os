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
#ifndef _HCI_MGMT_H_
#define _HCI_MGMT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <shared/hci.h>

struct hci_mgmt *hci_mgmt_new_default(uint16_t dev_id, int socket_fd);
void hci_mgmt_release(struct hci_mgmt *mgmt);
unsigned int hci_mgmt_register(struct hci_mgmt *mgmt, uint8_t event,
			       bt_hci_callback_func_t callback, void *user_data,
			       bt_hci_destroy_func_t destroy);
bool hci_mgmt_unregister(struct hci_mgmt *mgmt, unsigned int id);
unsigned int hci_mgmt_send(struct hci_mgmt *mgmt, uint16_t opcode,
			   const void *param, uint8_t size,
			   bt_hci_callback_func_t callback, void *user_data,
			   bt_hci_destroy_func_t destroy);

#ifdef __cplusplus
}
#endif

#endif /* _HCI_MGMT_H_ */

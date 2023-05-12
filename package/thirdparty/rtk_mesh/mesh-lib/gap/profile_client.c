/*
 * Copyright (C) 2018 Realtek Semiconductor Corporation.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <btif.h>
#include <trace.h>
#include <gap_le.h>
#include <profile_client.h>
#include "os_sched.h"

T_GAP_CAUSE client_by_uuid_srv_discovery(uint8_t conn_id, T_CLIENT_ID client_id, uint16_t uuid16)
{
	GAP_PRINT_ERROR0("Not supported");
	return GAP_CAUSE_ERROR_UNKNOWN;
}

T_GAP_CAUSE client_all_char_discovery(uint8_t conn_id, T_CLIENT_ID client_id,
                                      uint16_t start_handle,
                                      uint16_t end_handle)
{
	GAP_PRINT_ERROR0("Not supported");
	return GAP_CAUSE_ERROR_UNKNOWN;
}

T_GAP_CAUSE client_all_char_descriptor_discovery(uint8_t conn_id, T_CLIENT_ID client_id,
                                                 uint16_t start_handle, uint16_t end_handle)
{
	GAP_PRINT_ERROR0("Not supported");
	return GAP_CAUSE_ERROR_UNKNOWN;
}

T_GAP_CAUSE client_attr_read(uint8_t conn_id, T_CLIENT_ID client_id, uint16_t  handle)
{
	GAP_PRINT_ERROR0("Not supported");
	return GAP_CAUSE_ERROR_UNKNOWN;
}

T_GAP_CAUSE client_attr_read_using_uuid(uint8_t conn_id, T_CLIENT_ID client_id,
                                        uint16_t start_handle,
                                        uint16_t end_handle, uint16_t uuid16, uint8_t *p_uuid128)
{
	GAP_PRINT_ERROR0("Not supported");
	return GAP_CAUSE_ERROR_UNKNOWN;
}

T_GAP_CAUSE client_attr_write(uint8_t conn_id, T_CLIENT_ID client_id,
                              T_GATT_WRITE_TYPE write_type, uint16_t handle, uint16_t length, uint8_t *p_data)
{
	GAP_PRINT_ERROR0("Not supported");
	return GAP_CAUSE_ERROR_UNKNOWN;
}

bool client_register_spec_client_cb(T_CLIENT_ID *p_out_client_id,
                                    const T_FUN_CLIENT_CBS *client_cbs)
{
	GAP_PRINT_ERROR0("Not supported");
	return false;
}

/*
 * Copyright (C) 2018 Realtek Semiconductor Corporation.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <gap_msg.h>
#include <gap_internal.h>
#include <string.h>
#include <trace.h>
#include <gap_conn_le.h>

bool le_get_conn_info(uint8_t conn_id, T_GAP_CONN_INFO *p_conn_info)
{
	GAP_PRINT_ERROR0("Not supported");
	return false;
}

T_GAP_CAUSE le_disconnect(uint8_t conn_id)
{
	GAP_PRINT_ERROR0("Not supported");
	return GAP_CAUSE_ERROR_UNKNOWN;
}

T_GAP_CAUSE le_get_conn_param(T_LE_CONN_PARAM_TYPE param, void *p_value, uint8_t conn_id)
{
	GAP_PRINT_ERROR0("Not supported");
	return GAP_CAUSE_ERROR_UNKNOWN;
}

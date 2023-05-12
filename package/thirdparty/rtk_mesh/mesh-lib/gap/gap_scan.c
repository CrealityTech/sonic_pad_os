/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_scan.c
  * @brief   This file provides the GAP Central Role related functions.
  * @details
  * @author  jane
  * @date    2016-02-18
  * @version v0.1
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2016 Realtek Semiconductor Corporation</center></h2>
  ******************************************************************************
  */

#include <stdint.h>
#include <gap_msg.h>
#include <gap_internal.h>
#include <gap_scan.h>
#include <btif.h>
#include <trace.h>

uint8_t  gap_param_scan_mode = GAP_SCAN_MODE_ACTIVE;
uint16_t gap_param_scan_interval = 0x10;
uint16_t gap_param_scan_window = 0x10;
uint8_t  gap_param_filter_policy = GAP_SCAN_FILTER_ANY;
uint8_t  gap_param_filter_duplicates = GAP_SCAN_FILTER_DUPLICATE_DISABLE;
uint8_t  gap_param_scan_local_addr_type = BTIF_LOCAL_ADDR_LE_PUBLIC;
bool     gap_set_scan_param = true;

void gap_scan_param_deinit(void)
{
	gap_param_scan_mode = GAP_SCAN_MODE_ACTIVE;
	gap_param_scan_interval = 0x10;
	gap_param_scan_window = 0x10;
	gap_param_filter_policy = GAP_SCAN_FILTER_ANY;
	gap_param_filter_duplicates = GAP_SCAN_FILTER_DUPLICATE_DISABLE;
	gap_param_scan_local_addr_type = BTIF_LOCAL_ADDR_LE_PUBLIC;
	gap_set_scan_param = true;

}
T_GAP_CAUSE le_scan_get_param(T_LE_SCAN_PARAM_TYPE param, void *p_value)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    switch (param)
    {
    case GAP_PARAM_SCAN_LOCAL_ADDR_TYPE:
        *((uint8_t *)p_value) = gap_param_scan_local_addr_type;
        break;

    case GAP_PARAM_SCAN_MODE:
        *((uint8_t *)p_value) = gap_param_scan_mode;
        break;

    case GAP_PARAM_SCAN_INTERVAL:
        *((uint16_t *)p_value) = gap_param_scan_interval;
        break;

    case GAP_PARAM_SCAN_WINDOW:
        *((uint16_t *)p_value) = gap_param_scan_window;
        break;

    case GAP_PARAM_SCAN_FILTER_POLICY:
        *((uint8_t *)p_value) = gap_param_filter_policy;
        break;

    case GAP_PARAM_SCAN_FILTER_DUPLICATES:
        *((uint8_t *)p_value) = gap_param_filter_duplicates;
        break;

    default:
        ret = GAP_CAUSE_INVALID_PARAM;
        break;
    }
    if (ret != GAP_CAUSE_SUCCESS)
    {
        GAP_PRINT_ERROR1("le_scan_get_param: invalid param 0x%x", param);
    }
    return (ret);
}

T_GAP_CAUSE le_scan_set_param(T_LE_SCAN_PARAM_TYPE param, uint8_t len, void *p_value)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    switch (param)
    {
    case GAP_PARAM_SCAN_MODE:
        if (len == sizeof(uint8_t))
        {
            gap_param_scan_mode = *((uint8_t *)p_value);
            gap_set_scan_param = true;
        }
        else
        {
            ret = GAP_CAUSE_INVALID_PARAM;
        }
        break;

    case GAP_PARAM_SCAN_INTERVAL:
        if (len == sizeof(uint16_t))
        {
            gap_param_scan_interval = *((uint16_t *)p_value);
            gap_set_scan_param = true;
        }
        else
        {
            ret = GAP_CAUSE_INVALID_PARAM;
        }
        break;

    case GAP_PARAM_SCAN_WINDOW:
        if (len == sizeof(uint16_t))
        {
            gap_param_scan_window = *((uint16_t *)p_value);
            gap_set_scan_param = true;
        }
        else
        {
            ret = GAP_CAUSE_INVALID_PARAM;
        }
        break;

    case GAP_PARAM_SCAN_FILTER_POLICY:
        if (len == sizeof(uint8_t))
        {
            gap_param_filter_policy = *((uint8_t *)p_value);
            gap_set_scan_param = true;
        }
        else
        {
            ret = GAP_CAUSE_INVALID_PARAM;
        }
        break;


    case GAP_PARAM_SCAN_FILTER_DUPLICATES:
        if (len == sizeof(uint8_t))
        {
            gap_param_filter_duplicates = *((uint8_t *)p_value);
        }
        else
        {
            ret = GAP_CAUSE_INVALID_PARAM;
        }
        break;

    default:
        ret = GAP_CAUSE_INVALID_PARAM;
        break;
    }
    if (ret != GAP_CAUSE_SUCCESS)
    {
        GAP_PRINT_ERROR1("le_scan_set_param: invalid param 0x%x", param);
    }
    return (ret);
}

T_GAP_CAUSE le_scan_start(void)
{
    T_GAP_CAUSE ret = GAP_CAUSE_INVALID_STATE;
    GAP_PRINT_TRACE0("le_scan_start");
    if (gap_device_state.gap_scan_state == GAP_SCAN_STATE_IDLE)
    {
        if (gap_set_scan_param)
        {
            if (btif_le_scan_param_set_req((T_BTIF_LE_SCAN_TYPE)gap_param_scan_mode,
                                           gap_param_scan_interval, gap_param_scan_window,
                                           (T_BTIF_LE_SCAN_FILTER_POLICY)gap_param_filter_policy,
                                           (T_BTIF_LOCAL_ADDR_TYPE)gap_param_scan_local_addr_type))
            {
                ret = GAP_CAUSE_SUCCESS;
            }
        }
        else
        {
            if (btif_le_scan_req(BTIF_LE_SCAN_MODE_ENABLED,
                                 (T_BTIF_LE_SCAN_FILTER_DUPLICATES)gap_param_filter_duplicates))
            {
                ret = GAP_CAUSE_SUCCESS;
            }
        }
        if (ret == GAP_CAUSE_SUCCESS)
        {
            gap_device_state.gap_scan_state = GAP_SCAN_STATE_START;
            /* Inform current device state to app. */
            gap_send_dev_state(BTIF_SUCCESS);
        }
    }
    else
    {
        GAP_PRINT_ERROR1("le_scan_start: invalid state %d", gap_device_state.gap_scan_state);
    }

    return ret;
}

T_GAP_CAUSE le_scan_stop(void)
{
    T_GAP_CAUSE ret = GAP_CAUSE_INVALID_STATE;
    GAP_PRINT_TRACE0("le_scan_stop");
    if (gap_device_state.gap_scan_state == GAP_SCAN_STATE_SCANNING)
    {
        if (btif_le_scan_req(BTIF_LE_SCAN_MODE_DISABLED,
                             (T_BTIF_LE_SCAN_FILTER_DUPLICATES)gap_param_filter_duplicates))
        {
            ret = GAP_CAUSE_SUCCESS;
            gap_device_state.gap_scan_state = GAP_SCAN_STATE_STOP;
            /* Inform current device state to app. */
            gap_send_dev_state(BTIF_SUCCESS);
        }
    }
    else
    {
        GAP_PRINT_ERROR1("le_scan_stop: invalid state %d", gap_device_state.gap_scan_state);
    }

    return ret;
}

static void le_scan_handle_le_scan_rsp(T_BTIF_LE_SCAN_RSP *scan_resp)
{
    if (scan_resp->cause == BTIF_SUCCESS)
    {
        if (gap_device_state.gap_scan_state == GAP_SCAN_STATE_START)
        {
            gap_device_state.gap_scan_state = GAP_SCAN_STATE_SCANNING;
        }
        else if (gap_device_state.gap_scan_state == GAP_SCAN_STATE_STOP)
        {
            gap_device_state.gap_scan_state = GAP_SCAN_STATE_IDLE;
        }
    }
    else
    {
        if (gap_device_state.gap_scan_state == GAP_SCAN_STATE_START)
        {
            gap_device_state.gap_scan_state = GAP_SCAN_STATE_IDLE;
        }
        else if (gap_device_state.gap_scan_state == GAP_SCAN_STATE_STOP)
        {
            gap_device_state.gap_scan_state = GAP_SCAN_STATE_SCANNING;
        }
    }
    /* Inform current device state to app. */
    gap_send_dev_state(scan_resp->cause);

}

static void le_scan_handle_le_scan_param_set_rsp(T_BTIF_LE_SCAN_PARAM_SET_RSP *scan_resp)
{
    bool result = false;
    gap_set_scan_param = false;
    if (scan_resp->cause == BTIF_SUCCESS)
    {
        if (btif_le_scan_req(BTIF_LE_SCAN_MODE_ENABLED,
                             (T_BTIF_LE_SCAN_FILTER_DUPLICATES)gap_param_filter_duplicates))
        {
            result = true;
        }
    }

    if (result == false)
    {
        gap_device_state.gap_scan_state = GAP_SCAN_STATE_IDLE;
        /* Inform current device state to app. */
        gap_send_dev_state(scan_resp->cause);
    }
}

void le_scan_handle_btif_msg(T_BTIF_UP_MSG *p_msg)
{
    switch (p_msg->command)
    {
    case BTIF_MSG_LE_SCAN_RSP:
        le_scan_handle_le_scan_rsp(&p_msg->p.le_scan_rsp);
        break;

    case BTIF_MSG_LE_SCAN_PARAM_SET_RSP:
        le_scan_handle_le_scan_param_set_rsp(&p_msg->p.le_scan_param_set_rsp);
        break;
    default:
        break;
    }
    return;
}


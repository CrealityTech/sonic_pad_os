/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_adv.c
  * @brief   This file provides the GAP role Peripheral functions.
  * @details
  * @author  Ranhui
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
#include <gap_le.h>
#include <string.h>
#include <gap_adv.h>
#include <btif.h>
#include <trace.h>
#include <gap_scheduler.h>

#define ADV_STEP_ADV_DATA       0x01
#define ADV_STEP_ADV_PARAS      0x02
#define ADV_STEP_SCAN_RSP_DATA  0x04
#define ADV_STEP_ENABLE         0x08

#define DEFAULT_MIN_ADV_INTERVAL     0x0020	// default minimun advertising interval, 20 milliseconds
#define DEFAULT_MAX_ADV_INTERVAL     0x0400	// default maximun advertising interval, 10.24 seconds

/** Add all private variables here **/
uint8_t gap_param_adver_data_len = 3;

uint8_t gap_param_adver_data[GAP_MAX_ADV_LEN] = {
	0x02,	// length of this data
	GAP_ADTYPE_FLAGS,	// AD Type = Flags
	(GAP_ADTYPE_FLAGS_LIMITED | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED),	// Limited Discoverable & BR/EDR not supported
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0
};

uint8_t gap_param_scan_rsp_data_len = 0;
uint8_t gap_param_scan_rsp_data[GAP_MAX_ADV_LEN] = { 0 };

uint8_t gap_param_adv_event_type = GAP_ADTYPE_ADV_IND;
uint8_t gap_param_adv_direct_addr_type = GAP_REMOTE_ADDR_LE_PUBLIC;
uint8_t gap_param_adv_direct_addr[GAP_BD_ADDR_LEN] = { 0 };

uint8_t gap_param_adv_chan_map = GAP_ADVCHAN_ALL;
uint8_t gap_param_adv_filter_policy = GAP_ADV_FILTER_ANY;
uint16_t gap_param_adv_min_interval = DEFAULT_MIN_ADV_INTERVAL;
uint16_t gap_param_adv_max_interval = DEFAULT_MAX_ADV_INTERVAL;
uint8_t gap_param_adv_local_addr_type = GAP_LOCAL_ADDR_LE_PUBLIC;
uint8_t gap_adv_flag =
    ADV_STEP_ADV_DATA | ADV_STEP_ADV_PARAS | ADV_STEP_SCAN_RSP_DATA;
bool gap_update_param = false;

void gap_adv_param_deinit(void )
{
	gap_param_adv_chan_map = GAP_ADVCHAN_ALL;
	gap_param_adv_filter_policy = GAP_ADV_FILTER_ANY;
	gap_param_adv_min_interval = DEFAULT_MIN_ADV_INTERVAL;
	gap_param_adv_max_interval = DEFAULT_MAX_ADV_INTERVAL;
	gap_param_adv_local_addr_type = GAP_LOCAL_ADDR_LE_PUBLIC;
	gap_adv_flag =
		ADV_STEP_ADV_DATA | ADV_STEP_ADV_PARAS | ADV_STEP_SCAN_RSP_DATA;
	gap_update_param = false;
}
/*
 * Set a GAP Peripheral Role parameter.
 */
T_GAP_CAUSE le_adv_set_param(T_LE_ADV_PARAM_TYPE param, uint8_t len,
			     void *p_value)
{
	T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;

	switch (param) {
	case GAP_PARAM_ADV_LOCAL_ADDR_TYPE:
		if ((len == sizeof(uint8_t)) &&
		    (*((uint8_t *) p_value) <= GAP_LOCAL_ADDR_LE_RAP_OR_RAND)) {
			gap_param_adv_local_addr_type = *((uint8_t *) p_value);
			gap_adv_flag |= ADV_STEP_ADV_PARAS;
		} else {
			ret = GAP_CAUSE_INVALID_PARAM;
		}
		break;

	case GAP_PARAM_ADV_DATA:
		if (len <= GAP_MAX_ADV_LEN) {
			memcpy(gap_param_adver_data, p_value, len);
			gap_param_adver_data_len = len;
			gap_adv_flag |= ADV_STEP_ADV_DATA;
		} else {
			ret = GAP_CAUSE_INVALID_PARAM;
		}
		break;

	case GAP_PARAM_SCAN_RSP_DATA:
		if (len <= GAP_MAX_ADV_LEN) {
			memcpy(gap_param_scan_rsp_data, p_value, len);
			gap_param_scan_rsp_data_len = len;
			gap_adv_flag |= ADV_STEP_SCAN_RSP_DATA;
		} else {
			ret = GAP_CAUSE_INVALID_PARAM;
		}
		break;

	case GAP_PARAM_ADV_EVENT_TYPE:
		if ((len == sizeof(uint8_t)) &&
		    (*((uint8_t *) p_value) <= GAP_ADTYPE_ADV_LDC_DIRECT_IND)) {
			gap_param_adv_event_type = *((uint8_t *) p_value);
			gap_adv_flag |= ADV_STEP_ADV_PARAS;
		} else {
			ret = GAP_CAUSE_INVALID_PARAM;
		}
		break;

	case GAP_PARAM_ADV_DIRECT_ADDR_TYPE:
		if ((len == sizeof(uint8_t)) &&
		    (*((uint8_t *) p_value) <=
		     GAP_REMOTE_ADDR_LE_RANDOM_IDENTITY)) {
			gap_param_adv_direct_addr_type = *((uint8_t *) p_value);
			gap_param_adv_direct_addr_type &= 0x01;
			gap_adv_flag |= ADV_STEP_ADV_PARAS;
		} else {
			ret = GAP_CAUSE_INVALID_PARAM;
		}
		break;

	case GAP_PARAM_ADV_DIRECT_ADDR:
		if (len == GAP_BD_ADDR_LEN) {
			memcpy(gap_param_adv_direct_addr, p_value,
			       GAP_BD_ADDR_LEN);
			gap_adv_flag |= ADV_STEP_ADV_PARAS;
		} else {
			ret = GAP_CAUSE_INVALID_PARAM;
		}
		break;

	case GAP_PARAM_ADV_CHANNEL_MAP:
		if ((len == sizeof(uint8_t)) &&
		    (*((uint8_t *) p_value) <= GAP_ADVCHAN_ALL)) {
			gap_param_adv_chan_map = *((uint8_t *) p_value);
			gap_adv_flag |= ADV_STEP_ADV_PARAS;
		} else {
			ret = GAP_CAUSE_INVALID_PARAM;
		}
		break;

	case GAP_PARAM_ADV_FILTER_POLICY:
		if ((len == sizeof(uint8_t)) &&
		    (*((uint8_t *) p_value) <= GAP_ADV_FILTER_WHITE_LIST_ALL)) {
			gap_param_adv_filter_policy = *((uint8_t *) p_value);
			gap_adv_flag |= ADV_STEP_ADV_PARAS;
		} else {
			ret = GAP_CAUSE_INVALID_PARAM;
		}
		break;

	case GAP_PARAM_ADV_INTERVAL_MIN:
		if (len == sizeof(uint16_t)) {
			gap_param_adv_min_interval = *((uint16_t *) p_value);
			gap_adv_flag |= ADV_STEP_ADV_PARAS;

		} else {
			ret = GAP_CAUSE_INVALID_PARAM;
		}
		break;

	case GAP_PARAM_ADV_INTERVAL_MAX:
		if (len == sizeof(uint16_t)) {
			gap_param_adv_max_interval = *((uint16_t *) p_value);
			gap_adv_flag |= ADV_STEP_ADV_PARAS;
		} else {
			ret = GAP_CAUSE_INVALID_PARAM;
		}
		break;

	default:
		ret = GAP_CAUSE_INVALID_PARAM;
		break;
	}
	if (ret != GAP_CAUSE_SUCCESS) {
		GAP_PRINT_ERROR1("le_adv_set_param: invalid param 0x%x", param);
	}
	return (ret);
}

/*
 * Get a GAP Peripheral Role parameter.
 */
T_GAP_CAUSE le_adv_get_param(T_LE_ADV_PARAM_TYPE param, void *p_value)
{
	T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;

	switch (param) {
	case GAP_PARAM_ADV_LOCAL_ADDR_TYPE:
		*((uint8_t *) p_value) = gap_param_adv_local_addr_type;
		break;

	case GAP_PARAM_ADV_DATA:
		memcpy(p_value, gap_param_adver_data, gap_param_adver_data_len);
		break;

	case GAP_PARAM_SCAN_RSP_DATA:
		memcpy(p_value, gap_param_scan_rsp_data,
		       gap_param_scan_rsp_data_len);
		break;

	case GAP_PARAM_ADV_EVENT_TYPE:
		*((uint8_t *) p_value) = gap_param_adv_event_type;
		break;

	case GAP_PARAM_ADV_DIRECT_ADDR_TYPE:
		*((uint8_t *) p_value) = gap_param_adv_direct_addr_type;
		break;

	case GAP_PARAM_ADV_DIRECT_ADDR:
		memcpy(p_value, gap_param_adv_direct_addr, GAP_BD_ADDR_LEN);
		break;

	case GAP_PARAM_ADV_CHANNEL_MAP:
		*((uint8_t *) p_value) = gap_param_adv_chan_map;
		break;

	case GAP_PARAM_ADV_FILTER_POLICY:
		*((uint8_t *) p_value) = gap_param_adv_filter_policy;
		break;

	case GAP_PARAM_ADV_INTERVAL_MIN:
		*((uint16_t *) p_value) = gap_param_adv_min_interval;
		break;

	case GAP_PARAM_ADV_INTERVAL_MAX:
		*((uint16_t *) p_value) = gap_param_adv_max_interval;
		break;

	default:
		ret = GAP_CAUSE_INVALID_PARAM;
		break;
	}
	if (ret != GAP_CAUSE_SUCCESS) {
		GAP_PRINT_ERROR1("le_adv_get_param: invalid param 0x%x", param);
	}
	return (ret);
}

void start_advertising_by_steps(void)
{
	GAP_PRINT_TRACE1("start_advertising_by_steps: 0x%02x", gap_adv_flag);

	if (gap_adv_flag & ADV_STEP_ADV_PARAS) {
		btif_le_adv_param_set_req((T_BTIF_LE_ADV_TYPE)gap_param_adv_event_type,
		    (T_BTIF_LE_ADV_FILTER_POLICY)gap_param_adv_filter_policy,
		    gap_param_adv_min_interval, gap_param_adv_max_interval,
		    (T_BTIF_LOCAL_ADDR_TYPE)gap_param_adv_local_addr_type,
		    gap_param_adv_direct_addr,
		    (T_BTIF_REMOTE_ADDR_TYPE)gap_param_adv_direct_addr_type,
		    gap_param_adv_chan_map);
		gap_adv_flag &= ~ADV_STEP_ADV_PARAS;
	} else if (gap_adv_flag & ADV_STEP_SCAN_RSP_DATA) {
		btif_le_adv_data_set_req(BTIF_LE_DATA_TYPE_SCAN_RSP,
					 gap_param_scan_rsp_data_len,
					 (uint8_t *)gap_param_scan_rsp_data);
		gap_adv_flag &= ~ADV_STEP_SCAN_RSP_DATA;
	} else if (gap_adv_flag & ADV_STEP_ADV_DATA) {
		btif_le_adv_data_set_req(BTIF_LE_DATA_TYPE_ADV,
					 gap_param_adver_data_len,
					 gap_param_adver_data);
		gap_adv_flag &= ~ADV_STEP_ADV_DATA;
	} else if (gap_adv_flag & ADV_STEP_ENABLE) {
		btif_le_adv_req(BTIF_LE_ADV_MODE_ENABLED);
		gap_adv_flag &= ~ADV_STEP_ENABLE;
	}
}

/*
 * Start advertising
 */
T_GAP_CAUSE le_adv_start(void)
{
	//GAP_PRINT_TRACE0("le_adv_start");
	if ((gap_device_state.gap_adv_state == GAP_ADV_STATE_IDLE) &&
	    (gap_update_param == false)) {
		/* for gatt */
		/*if idle link num is 0 */
		/*if (le_get_idle_link_num() == 0)
		 * {
		 *     // can not establish link
		 *              if ((gap_param_adv_event_type == GAP_ADTYPE_ADV_IND) ||
		 *         (gap_param_adv_event_type == GAP_ADTYPE_ADV_HDC_DIRECT_IND) ||
		 *         (gap_param_adv_event_type == GAP_ADTYPE_ADV_LDC_DIRECT_IND))
		 *     {
		 *         GAP_PRINT_ERROR0("le_adv_start: connection limit exceeded, can't send connectable adv");
		 *         return GAP_CAUSE_CONN_LIMIT;
		 *     }
		 * }
		 */

		gap_adv_flag |= ADV_STEP_ENABLE;
		gap_device_state.gap_adv_state = GAP_ADV_STATE_START;
		start_advertising_by_steps();
		gap_send_dev_state(BTIF_SUCCESS);

		return GAP_CAUSE_SUCCESS;
	} else {
		GAP_PRINT_ERROR1("le_adv_start: invalid state %d",
				 gap_device_state.gap_adv_state);
		return GAP_CAUSE_INVALID_STATE;
	}
}

T_GAP_CAUSE le_adv_update_param(void)
{
	T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;

	if (gap_adv_flag == 0) {
		ret = GAP_CAUSE_INVALID_PARAM;
	}
	if (gap_update_param) {
		ret = GAP_CAUSE_ALREADY_IN_REQ;
	}
	if (((gap_adv_flag & ADV_STEP_ADV_PARAS) &&
	     (gap_device_state.gap_adv_state != GAP_ADV_STATE_IDLE)) ||
	    (gap_device_state.gap_adv_state == GAP_ADV_STATE_START)) {
		ret = GAP_CAUSE_INVALID_STATE;
	}
	if (ret == GAP_CAUSE_SUCCESS) {
		gap_update_param = true;
		start_advertising_by_steps();
		return GAP_CAUSE_SUCCESS;
	} else {
		GAP_PRINT_ERROR1("le_adv_update_param: failed ret %d", ret);
		return ret;
	}
}

/*
 * Stop advertising
 */
T_GAP_CAUSE le_adv_stop(void)
{
	GAP_PRINT_TRACE0("le_adv_stop");
	if ((gap_device_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING) &&
	    (gap_update_param == false)) {
		btif_le_adv_req(BTIF_LE_ADV_MODE_DISABLED);
		gap_device_state.gap_adv_state = GAP_ADV_STATE_STOP;
		gap_send_dev_state(BTIF_SUCCESS);
		return GAP_CAUSE_SUCCESS;
	} else {
		GAP_PRINT_ERROR1("le_adv_stop: invalid state %d",
				 gap_device_state.gap_adv_state);
		return GAP_CAUSE_INVALID_STATE;
	}
}

T_GAP_CAUSE le_adv_set_tx_power(T_GAP_ADV_TX_POW_TYPE type, uint8_t tx_gain)
{
	return GAP_CAUSE_SEND_REQ_FAILED;
}

T_GAP_CAUSE le_vendor_aggressive_enable(uint8_t enable)
{
	 uint8_t len = 1;
	 uint8_t param[1];
	 param[0] = enable;

	  if (btif_vendor_cmd_req(0xFCA6, len, param))
	  {
	      return GAP_CAUSE_SUCCESS;
	  }
	return GAP_CAUSE_SEND_REQ_FAILED;

}

T_GAP_CAUSE le_vendor_one_shot_adv(uint8_t guard_slot, uint16_t guard_usec,
				   uint16_t wait_usec)
{
	 uint8_t len = 1;
	 uint8_t param[1];
	 param[0] = 0x01;
	 
	  if (btif_vendor_cmd_req(0xFC87, len, param))
	  {
	      return GAP_CAUSE_SUCCESS;
	  }	 
	return GAP_CAUSE_SEND_REQ_FAILED;
}

static void le_adv_handle_le_advertise_rsp(T_BTIF_LE_ADV_RSP * adv_rsp)
{
	if (adv_rsp->cause == BTIF_SUCCESS) {
		switch (adv_rsp->adv_mode) {
		case BTIF_LE_ADV_MODE_DISABLED:
			gap_device_state.gap_adv_state = GAP_ADV_STATE_IDLE;
			gap_device_state.gap_adv_sub_state =
			    GAP_ADV_TO_IDLE_CAUSE_STOP;
			break;

		case BTIF_LE_ADV_MODE_ENABLED:
			gap_device_state.gap_adv_state =
			    GAP_ADV_STATE_ADVERTISING;
			break;

		default:
			break;
		}
	} else {
		GAP_PRINT_ERROR1("le_adv_handle_le_advertise_rsp: failed cause 0x%x",
				 adv_rsp->cause);
		switch (adv_rsp->adv_mode) {
		case BTIF_LE_ADV_MODE_DISABLED:
			if (gap_device_state.gap_adv_state ==
			    GAP_ADV_STATE_STOP) {
				gap_device_state.gap_adv_state =
				    GAP_ADV_STATE_ADVERTISING;
			} else {
				gap_device_state.gap_adv_state =
				    GAP_ADV_STATE_IDLE;
				gap_device_state.gap_adv_sub_state =
				    GAP_ADV_TO_IDLE_CAUSE_STOP;
			}
			break;

		case BTIF_LE_ADV_MODE_ENABLED:
			gap_device_state.gap_adv_state = GAP_ADV_STATE_IDLE;
			gap_device_state.gap_adv_sub_state =
			    GAP_ADV_TO_IDLE_CAUSE_STOP;
			break;

		default:
			break;
		}
	}
	gap_send_dev_state(adv_rsp->cause);
}

static void le_adv_handle_le_high_duty_adv_timeout_info(
		T_BTIF_LE_HIGH_DUTY_ADV_TIMEOUT_INFO *timeout_info)
{
	if (gap_device_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING) {
		gap_device_state.gap_adv_state = GAP_ADV_STATE_IDLE;
		gap_device_state.gap_adv_sub_state = GAP_ADV_TO_IDLE_CAUSE_STOP;
		gap_send_dev_state(BTIF_SUCCESS);
	}
}

void le_adv_handle_btif_msg(T_BTIF_UP_MSG * p_msg)
{
	switch (p_msg->command) {
	case BTIF_MSG_LE_ADV_RSP:
		le_adv_handle_le_advertise_rsp(&p_msg->p.le_adv_rsp);
		break;

	case BTIF_MSG_LE_ADV_PARAM_SET_RSP:
	case BTIF_MSG_LE_ADV_DATA_SET_RSP:
		{
			if ((gap_adv_flag == 0) && (gap_update_param == true)) {
                         //when HCI one shot cmd is available,this branch is available.
				gap_update_param = false; 
				gap_sched_adv_params_set_done();
			} else {
				start_advertising_by_steps();
			}
		}
		break;

	case BTIF_MSG_LE_HIGH_DUTY_ADV_TOUT_INFO:
		le_adv_handle_le_high_duty_adv_timeout_info(&p_msg->p.
							    le_high_duty_adv_timeout_info);
		break;

	default:
		break;
	}
	return;
}

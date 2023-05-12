/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_le.c
  * @brief   This file provides the GAP role common functions, used by all GAP roles.
  * @details
  * @author  jane
  * @date    2016-02-18
  * @version v0.1
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2016 Realtek Semiconductor Corporation</center></h2>
  ******************************************************************************
  */

#include <string.h>
#include <unistd.h>
#include <btif.h>
#include <trace.h>
#include <gap.h>
#include <gap_msg.h>
#include <gap_le.h>
#include <gap_internal.h>
#include <gap_storage_le.h>
#include <gap_scheduler.h>

#define GAP_INIT_STEP_SEC                 0x01
#define GAP_INIT_STEP_SERVICE_REG         0x02
#define GAP_INIT_STEP_LOCAL_IRK           0x04
#define GAP_INIT_STEP_PRIVACY_TIMEOUT     0x08
#define GAP_INIT_STEP_SET_RANDOM          0x20
#define GAP_INIT_STEP_SET_DEFAULT_PHY     0x40
#define GAP_INIT_STEP_ENABLE_EXTENDED_ADV 0x80
#define GAP_INIT_STEP_SET_REM_MIN_SCA   0x0100
#define GAP_INIT_STEP_SET_DATA_LEN      0x0200

#define GAP_INIT_STEP_ALL (GAP_INIT_STEP_SEC|GAP_INIT_STEP_SERVICE_REG \
                           |GAP_INIT_STEP_LOCAL_IRK|GAP_INIT_STEP_SET_DATA_LEN)

T_GAP_DEV_STATE gap_device_state = { 0, 0, 0, 0, 0 };
T_GAP_DEV_STATE old_device_state = { 0, 0, 0, 0, 0 };

uint8_t gap_param_dev_name[GAP_DEVICE_NAME_LEN] = "GAP";	/* local device name. */
uint16_t gap_param_appearance = GAP_GATT_APPEARANCE_UNKNOWN;	/* local device appearence. */
uint8_t gap_random_addr[GAP_BD_ADDR_LEN] = { 0 };
uint8_t gap_param_connected_dev_addr[GAP_BD_ADDR_LEN] = { 0 };

uint8_t gap_param_connected_dev_addr_type = 0;
uint16_t gap_param_ds_pool_id = 0;
uint16_t gap_param_ds_data_offset = 0;
uint8_t gap_slave_init_gatt_mtu_req = 0;
uint8_t gap_param_handle_create_conn_ind = 0;
uint8_t gap_param_le_max_wl_size = 0;
int8_t gap_param_le_min_tx_power = 127;
int8_t gap_param_le_max_tx_power = 127;
uint8_t gap_param_default_phys = 0;
uint8_t gap_param_default_tx_phys = 0;
uint8_t gap_param_default_rx_phys = 0;

#if F_BT_LE_5_0_AE_ADV_SUPPORT
uint8_t gap_param_use_extended_adv = false;
#endif
#if F_BT_LE_5_0_SUPPORT
uint16_t gap_param_data_len_tx_oct = 251;
uint16_t gap_param_data_len_tx_time = 0x848;
#endif
T_GAP_SCA_FIELD_ENCODING gap_param_rem_min_sca = GAP_SCA_INVALID;
uint8_t gap_local_features[GAP_LE_SUPPORTED_FEATURES_LEN] =
    { 0, 0, 0, 0, 0, 0, 0, 0 };
uint16_t gap_init_flag = GAP_INIT_STEP_ALL;
bool gap_msg_info_use_msg = true;

static uint8_t gap_param_bd_addr[6];

/* Application callbacks, these callbacks will be used in each role to send msg to application. */
P_FUN_LE_APP_CB gap_app_cb = NULL;

void gap_le_deinit()
{
	memset(&gap_device_state,0,sizeof(T_GAP_DEV_STATE));
	memset(&old_device_state,0,sizeof(T_GAP_DEV_STATE));
	gap_init_flag = GAP_INIT_STEP_ALL;
	gap_msg_info_use_msg = true;
}

uint8_t gap_get_param(T_GAP_PARAM_TYPE param, void *p_value)
{
	uint8_t ret = 0;

	switch (param) {
	case GAP_PARAM_BD_ADDR:
		memcpy(p_value, gap_param_bd_addr, GAP_BD_ADDR_LEN);
		break;

	default:
		ret = 0x03;
		break;
	}

	if (ret != 0) {
		GAP_PRINT_ERROR1("gap_get_param: invalid param 0x%x", param);
	}

	return (ret);
}

void gap_set_param(T_GAP_PARAM_TYPE param, uint8_t len, void *p_value)
{
	T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;

	switch (param) {
	case GAP_PARAM_BD_ADDR:
		if (len > GAP_BD_ADDR_LEN)
			len = GAP_BD_ADDR_LEN;
		memcpy(gap_param_bd_addr, p_value, len);
		break;
	default:
		ret = GAP_CAUSE_INVALID_PARAM;
		break;
	}

	if (ret != GAP_CAUSE_SUCCESS) {
		GAP_PRINT_ERROR1("gap_set_param: invalid param 0x%x", param);
	}
}

T_GAP_CAUSE le_set_gap_param(T_GAP_LE_PARAM_TYPE param, uint8_t len,
			     void *p_value)
{
	T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;

	/* Common param set should be added here. */
	switch (param) {
	case GAP_PARAM_APPEARANCE:
		if (len == sizeof(uint16_t)) {
			gap_param_appearance = *((uint16_t *)p_value);
		} else {
			ret = GAP_CAUSE_INVALID_PARAM;
		}
		break;
	case GAP_PARAM_DEVICE_NAME:
		if (len >= GAP_DEVICE_NAME_LEN) {
			len = GAP_DEVICE_NAME_LEN - 1;
		}
		memcpy(gap_param_dev_name, p_value, len);
		gap_param_dev_name[len] = 0;
		break;
	default:
		ret = GAP_CAUSE_INVALID_PARAM;
		break;
	}
	if (ret != GAP_CAUSE_SUCCESS) {
		GAP_PRINT_ERROR1("le_set_gap_param: invalid param 0x%x", param);
	}
	return (ret);
}

T_GAP_CAUSE le_get_gap_param(T_GAP_LE_PARAM_TYPE param, void *p_value)
{
	T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;

	/* Common param set should be added here. */
	switch (param) {
	case GAP_PARAM_DEV_STATE:
		*((T_GAP_DEV_STATE *) p_value) = gap_device_state;
		break;
	case GAP_PARAM_APPEARANCE:
		*((uint16_t *)p_value) = gap_param_appearance;
		break;
	case GAP_PARAM_DEVICE_NAME:
		break;
	default:
		ret = GAP_CAUSE_INVALID_PARAM;
		break;
	}
	if (ret != GAP_CAUSE_SUCCESS) {
		GAP_PRINT_ERROR1("le_get_gap_param: invalid param 0x%x", param);
	}
	return (ret);
}

void gap_send_dev_state(uint16_t cause)
{
	if ((old_device_state.gap_init_state != gap_device_state.gap_init_state) ||
	    (old_device_state.gap_adv_state != gap_device_state.gap_adv_state) ||
	    (old_device_state.gap_scan_state != gap_device_state.gap_scan_state) ||
	    (old_device_state.gap_conn_state != gap_device_state.gap_conn_state)) {
		GAP_PRINT_TRACE3("gap_send_dev_state: old state 0x%x, new state 0x%x, cause 0x%x",
		     *(uint8_t *)&old_device_state,
		     *(uint8_t *)&gap_device_state, cause);
		gap_send_dev_state_msg(gap_device_state, cause);
		old_device_state = gap_device_state;
	}
}
void gap_handle_le_vendor_cmd_rsp(T_BTIF_VENDOR_CMD_RSP *set_rsp)
{
    /* bool handle = true; */
    if (set_rsp->command == HCI_LE_VENDOR_EXTENSION_FEATURE)
    {
        /* T_LE_CB_DATA cb_data; */
        /* uint8_t msg_type; */
        switch (set_rsp->param[0])
        {
 
        	case HCI_EXT_SUB_ONE_SHOT_ADV:
           	 {
		  	if( set_rsp->cause !=0)
		  	{
		  		APP_PRINT_ERROR1("GAP_MSG_LE_VENDOR_ONE_SHOT_ADV: cause 0x%x",set_rsp->cause);
		  	}
		 	else
		  	{
				APP_PRINT_INFO1("GAP_MSG_LE_VENDOR_ONE_SHOT_ADV: cause 0x%x",set_rsp->cause);
		  	}     
			gap_sched_adv_done(0);
            	}
            break;
        default:
            break;
        }
    }
}

extern int msg_pipe[2];

void gap_stack_ready(void)
{

	GAP_PRINT_INFO0("GAP Ready");
	gap_device_state.gap_init_state = GAP_INIT_STATE_STACK_READY;
	gap_send_dev_state(BTIF_SUCCESS);

}

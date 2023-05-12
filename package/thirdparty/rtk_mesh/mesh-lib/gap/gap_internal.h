/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_internal.h
  * @brief   Head file for internal GAP role common functions and global variables, used by all GAP roles.
  * @details
  * @author  jane
  * @date    2016-02-18
  * @version v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion **/
#ifndef GAP_INTERNAL_H
#define GAP_INTERNAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <gap.h>
#include <gap_le.h>
#include <gap_storage_le.h>
#include <btif.h>
#include "app_msg.h"

#define GAP_MODE_LE_SUPPORT      0x01
#define GAP_MODE_LEGACY_SUPPORT  0x02

#define HCI_LE_VENDOR_EXTENSION_FEATURE 0xFD80
#define HCI_EXT_SUB_ONE_SHOT_ADV 3

/**  @brief Event type definitions.*/
typedef enum
{
	EVENT_MESH = 0x80,
	MESH_INIT_STEP0 = 0xff,
	MESH_INIT_STEP1 = 0xfe,
       MESH_INIT_STEP2 = 0xfd,
       MESH_INIT_STEP3 = 0xfc,
       MESH_INIT_STEP4 = 0xfb,
       MESH_INIT_STEP5 = 0xfa,
} T_MESH_INIT_EVENT_TYPE;

extern T_GAP_DEV_STATE gap_device_state;
extern uint8_t  gap_param_connected_dev_addr[GAP_BD_ADDR_LEN];
extern uint8_t  gap_param_connected_dev_addr_type;
extern P_FUN_LE_APP_CB gap_app_cb;
extern T_LOCAL_IRK gap_local_irk;
extern uint16_t gap_privacy_timeout;
extern uint8_t  gap_param_gen_local_irk_auto;
extern uint8_t  gap_param_handle_create_conn_ind;
extern uint8_t  gap_param_use_extended_adv;
extern uint16_t gap_param_ds_pool_id;
extern uint16_t gap_param_ds_data_offset;
extern uint8_t  gap_param_link_num;
extern bool     gap_msg_info_use_msg;

typedef bool (*P_FUN_GAP_EXTEND_CB)(T_BTIF_UP_MSG *p_msg, bool *release);

void gap_send_dev_state(uint16_t cause);
void gap_stack_ready(void);
void le_scan_handle_btif_msg(T_BTIF_UP_MSG *p_msg);
void le_adv_handle_btif_msg(T_BTIF_UP_MSG *p_msg);

void le_gap_msg_init(void *event_fd, void *qh_io);
void gap_send_dev_state_msg(T_GAP_DEV_STATE new_state, uint16_t cause);

void gap_handle_le_vendor_cmd_rsp(T_BTIF_VENDOR_CMD_RSP *set_rsp);

void gap_stack_prepare(const void *data, uint8_t size, void*user_data);

void gap_send_msg_to_app(T_IO_MSG * p_msg);


#ifdef __cplusplus
}
#endif

#endif /* GAP_INTERNAL_H */

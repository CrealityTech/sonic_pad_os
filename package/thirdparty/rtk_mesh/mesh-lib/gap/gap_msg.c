/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_msg.c
  * @brief   All GAP roles used to transmit messages from GAP to application.
  * @details
  * @author  ranhui
  * @date    2016-02-18
  * @version v0.1
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2016 Realtek Semiconductor Corporation</center></h2>
  ******************************************************************************
  */

#include <stdint.h>
#include <string.h>
#include <gap_msg.h>
#include <trace.h>
#include <app_msg.h>
#include <gap_callback_le.h>
#include <gap_internal.h>
#include <unistd.h>
#include <os_msg.h>

static void* gap_msg_fd;
/* Store the handle from app */
static void *gap_msg_qh_io;

/* Send message to app to deal with
 */
void gap_send_msg_to_app(T_IO_MSG * p_msg)
{
	if (true /* gap_msg_info_use_msg */) {
		uint8_t event = EVENT_IO_TO_APP;
		if((gap_msg_fd == NULL )||(gap_msg_qh_io == NULL ) )
		{
			GAP_PRINT_ERROR0("gap_msg_fd or gap_msg_qh_io is NULL");
		}

		if (!(os_event_send(gap_msg_fd, &event, TIME_ALAWAYS_WAIT))) {
			GAP_PRINT_ERROR1("gap_send_msg_to_app event fail: subtype 0x%x",
			     p_msg->subtype);
			return;
		} else if (os_msg_send(gap_msg_qh_io, p_msg, 0) == false) {
			GAP_PRINT_ERROR1("gap_send_msg_to_app msg fail: subtype 0x%x",
			     p_msg->subtype);

		}
	} else {
		/* Call app callback functions directly */
	}

	return;
}

/*
 * Callback to send state change message to app task.
 */
void gap_send_dev_state_msg(T_GAP_DEV_STATE new_state, uint16_t cause)
{
	T_LE_GAP_MSG bt_msg = { 0 };
	T_IO_MSG bee_io_msg;

	GAP_PRINT_INFO2("gap_send_dev_state_msg new_state=0x%x cause=0x%x",
			*(uint8_t *) & new_state, cause);

	bee_io_msg.type = IO_MSG_TYPE_BT_STATUS;
	bee_io_msg.subtype = GAP_MSG_LE_DEV_STATE_CHANGE;
	bt_msg.msg_data.gap_dev_state_change.new_state = new_state;
	bt_msg.msg_data.gap_dev_state_change.cause = cause;

	memcpy(&bee_io_msg.u.param, &bt_msg, sizeof(bee_io_msg.u.param));
	gap_send_msg_to_app(&bee_io_msg);
}

void le_gap_msg_init(void *event_fd, void *qh_io)
{
	gap_msg_fd = event_fd;
	gap_msg_qh_io = qh_io;
}

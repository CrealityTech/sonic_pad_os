/**
 * system/message_manager/message_manager.c
 *
 * Descript: message manager module
 * Copyright (C) 2012-2016 AllWinnertech Ltd.
 * Author: Sunny <Sunny@allwinnertech.com>
 *
 */

#include "message_manager_i.h"
#include "wakeup_source.h"

s32 message_valid(struct message *pmessage)
{
	/* valid message */
	if (!!pmessage->type)
		return TRUE;

	/* invalid message */
	return FALSE;
}

/**
 * message_manager_init() - initialize message manager.
 *
 * @return: OK if initialize succeeded, others if failed.
 */
s32 message_manager_init(void)
{
	return OK;
}

/**
 * message_manager_exit() - exit message manager.
 *
 * @return: OK if exit succeeded, others if failed.
 */
s32 message_manager_exit(void)
{
	return OK;
}

/**
 * process_message() - process one message.
 *
 * pmessage: the pointer of message frame which we want to process.
 * @return: OK if process message succeeded, other if failed.
 */
static s32 process_message(struct message *pmessage)
{
	s32 result;

	/* process message */
	switch (pmessage->type) {

	case CPU_OP_REQ:
		LOG("cpu op req\n");
		result = cpu_op(pmessage);
		break;

	case SYS_OP_REQ:
		LOG("sys op req\n");
		result = sys_op(pmessage);
		break;

	case CLEAR_WAKEUP_SRC_REQ:
		LOG("clear wakeup src req\n");
		result = clear_wakeup_src(pmessage);
		break;

	case SET_WAKEUP_SRC_REQ:
		LOG("set wakeup src req\n");
		result = set_wakeup_src(pmessage);
		break;

	case SET_DEBUG_LEVEL_REQ:
		INF("set debug level request\n");
		result = set_debug_level(pmessage->paras[0]);
		break;

	case SET_UART_BAUDRATE:
		INF("set uart baudrate request\n");
		result = uart_set_baudrate(pmessage->paras[0]);
		break;

	case SET_DRAM_CRC_PARAS:
		INF("set dram crc paras request\n");
		result = standby_set_dram_crc_paras(pmessage->paras[0], pmessage->paras[1], pmessage->paras[2]);
		break;

	case MESSAGE_LOOPBACK:
		INF("loopback message request\n");
		result = OK;
		break;

	default:
		ERR("imt [%x]\n", pmessage->type);
		hexdump("msg", (char *)pmessage, sizeof(struct message));
		result = -ESRCH;
		break;
	}

	pmessage->result = result;

	/* synchronous message, should feedback process result */
	if (pmessage->attr & (MESSAGE_ATTR_SOFTSYN | MESSAGE_ATTR_HARDSYN))
		hwmsgbox_feedback_message(pmessage, SEND_MSG_TIMEOUT);

	return OK;
}

/**
 * message_coming_notify() - notify system that one message coming.
 *
 * pmessage: the pointer of coming message frame.
 * @return: OK if notify succeeded, other if failed.
 */
s32 message_coming_notify(struct message *pmessage)
{
	/*
	 * be careful, here we keep cpu interrupt on when processing message,
	 * and we should avoid interrupt as far as possible.
	 */

	/* ar100 receive message from ac327 */
	INF("MESSAGE FROM AC327\n");
	INF("state:%x\n", pmessage->state);
	INF("type:%x\n", pmessage->type);
	INF("attr:%x\n", pmessage->attr);
	INF("count: 0x%x\n", pmessage->count);

	if (process_message(pmessage) != OK)
		WRN("message [%x, %x] process fail\n", pmessage, pmessage->type);

	return OK;
}


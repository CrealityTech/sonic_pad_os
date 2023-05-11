/**
 * driver\hwmsgbox\hwmsgbox-extended\hwmsgbox-extended.c
 *
 * Descript: hardware message-box driver for new IP.
 * Copyright (C) 2012-2016 AllWinnertech Ltd.
 * Author: Sunny <Sunny@allwinnertech.com>
 *
 */

#include "hwmsgbox-extended.h"


/*
*********************************************************************************************************
*                                           CLEAR PENDING
*
* Description:  clear the receiver interrupt pending of message-queue.
*
* Arguments  :  queue   : the number of message-queue which we want to clear.
*               user    : the user which we want to clear.
*
* Returns    :  OK if clear pending succeeded, others if failed.
*********************************************************************************************************
*/
#define hwmsgbox_clear_receiver_pending(queue)\
	writel((0x1 << (queue * 2)), MSGBOX_ARM_TO_RISC_IRQ_STATUS_REG)


/*
*********************************************************************************************************
*                                           INITIALIZE HWMSGBOX
*
* Description:  initialize hwmsgbox.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize hwmsgbox succeeded, others if failed.
*********************************************************************************************************
*/
s32 hwmsgbox_init(void)
{
	/*enable msgbox clock and set reset as de-assert state.*/
	ccu_set_mclk_onoff(CCU_MOD_CLK_MSGBOX, CCU_CLK_ON);
	ccu_set_mclk_reset(CCU_MOD_CLK_MSGBOX, CCU_CLK_NRESET);

	return OK;
}

/*
*********************************************************************************************************
*                                           EXIT HWMSGBOX
*
* Description:  exit hwmsgbox.
*
* Arguments  :  none.
*
* Returns    :  OK if exit hwmsgbox succeeded, others if failed.
*********************************************************************************************************
*/
s32 hwmsgbox_exit(void)
{
	/* disable msgbox clock and set reset as assert state. */
	ccu_set_mclk_reset(CCU_MOD_CLK_MSGBOX, CCU_CLK_RESET);
	ccu_set_mclk_onoff(CCU_MOD_CLK_MSGBOX, CCU_CLK_OFF);

	return OK;
}

s32 hwmsgbox_wait_queue_not_full(u32 queue, u32 timeout)
{
	while (readl(MSGBOX_RISC_TO_ARM_MSG_STATUS_REG(queue)) == 8) {
		/*
		 * message-queue fifo is full,
		 * wait 1ms for message-queue process.
		 */
		if (timeout == 0) {
			return -ETIMEOUT;
		}
		time_mdelay(1);
		timeout--;
	}
	return OK;
}

s32 hwmsgbox_wait_queue_not_empty(u32 queue, u32 timeout)
{
	while (readl(MSGBOX_ARM_TO_RISC_MSG_STATUS_REG(queue)) == 0) {
		/*
		 * message-queue fifo is empty,
		 * wait 1ms for message-queue process.
		 */
		if (timeout == 0) {
			return -ETIMEOUT;
		}
		time_mdelay(1);
		timeout--;
	}
	return OK;
}

/*
*********************************************************************************************************
*                                       SEND MESSAGE BY HWMSGBOX
*
* Description:  send one message to another processor by hwmsgbox.
*
* Arguments  :  pmessage    : the pointer of sended message frame.
*               timeout     : the wait time limit when message fifo is full.
*
* Returns    :  OK if send message succeeded, other if failed.
*********************************************************************************************************
*/
s32 hwmsgbox_send_message(struct message *pmessage, u32 timeout)
{
	s32 ret;
	u32 i;
	u32 value;

	ASSERT(pmessage != NULL);

	if (pmessage->attr & MESSAGE_ATTR_HARDSYN) {
		/* use ar100 hwsyn transmit channel */
		INF("send syn message\n");

		/* first send message header and misc */
		ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_RISC_SYN_TX_CH, timeout);
		if (ret != OK)
			return ret;
		value = pmessage->state | (pmessage->attr << 8) |
			(pmessage->type << 16) | (pmessage->result << 24);
		writel(value, MSGBOX_RISC_TO_ARM_MSG_REG(HWMSGBOX_RISC_SYN_TX_CH));

		ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_RISC_SYN_TX_CH, timeout);
		if (ret != OK)
			return ret;
		value = pmessage->count;
		writel(value, MSGBOX_RISC_TO_ARM_MSG_REG(HWMSGBOX_RISC_SYN_TX_CH));

		/* then send message paras */
		for (i = 0; i < pmessage->count; i++) {
			ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_RISC_SYN_TX_CH, timeout);
			if (ret != OK)
				return ret;
			writel(pmessage->paras[i], MSGBOX_RISC_TO_ARM_MSG_REG(HWMSGBOX_RISC_SYN_TX_CH));
		}

		/* after send, wait feedback, hwsyn messsage must feedback use syn rx channel */

		/* first receive message header and misc */
		ret = hwmsgbox_wait_queue_not_empty(HWMSGBOX_RISC_SYN_RX_CH, timeout);
		if (ret != OK)
			return ret;
		value = readl(MSGBOX_ARM_TO_RISC_MSG_REG(HWMSGBOX_RISC_SYN_RX_CH));
		pmessage->state = value & 0xff;
		pmessage->attr = (value >> 8) & 0xff;
		pmessage->type = (value >> 16) & 0xff;
		pmessage->result = (value >> 24) & 0xff;

		ret = hwmsgbox_wait_queue_not_empty(HWMSGBOX_RISC_SYN_RX_CH, timeout);
		if (ret != OK)
			return ret;
		value = readl(MSGBOX_ARM_TO_RISC_MSG_REG(HWMSGBOX_RISC_SYN_RX_CH));
		pmessage->count = value & 0xff;

		/* then receive message paras */
		for (i = 0; i < pmessage->count; i++) {
			ret = hwmsgbox_wait_queue_not_empty(HWMSGBOX_RISC_SYN_RX_CH, timeout);
			if (ret != OK)
				return ret;
			pmessage->paras[i] = readl(MSGBOX_ARM_TO_RISC_MSG_REG(HWMSGBOX_RISC_SYN_RX_CH));
		}
		return OK;
	} else {
		/* asyn message use asyn tx channel */
		INF("send asyn message\n");

		/* first send message header and misc */
		ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_RISC_ASYN_TX_CH, timeout);
		if (ret != OK)
			return ret;
		value = pmessage->state | (pmessage->attr << 8) |
			(pmessage->type << 16) | (pmessage->result << 24);
		writel(value, MSGBOX_RISC_TO_ARM_MSG_REG(HWMSGBOX_RISC_ASYN_TX_CH));

		ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_RISC_ASYN_TX_CH, timeout);
		if (ret != OK)
			return ret;
		value = pmessage->count;
		writel(value, MSGBOX_RISC_TO_ARM_MSG_REG(HWMSGBOX_RISC_ASYN_TX_CH));

		/* then send message paras */
		for (i = 0; i < pmessage->count; i++) {
			ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_RISC_ASYN_TX_CH, timeout);
			if (ret != OK)
				return ret;
			writel(pmessage->paras[i], MSGBOX_RISC_TO_ARM_MSG_REG(HWMSGBOX_RISC_ASYN_TX_CH));
		}
		return OK;
	}
}

int hwmsgbox_feedback_message(struct message *pmessage, u32 timeout)
{
	s32 ret;
	u32 i;
	u32 value;

	ASSERT(pmessage != NULL);

	if (pmessage->attr & MESSAGE_ATTR_HARDSYN) {
		/* use ac327 hard syn receiver channel */
		INF("send feedback message\n");

		/* first send message header and misc */
		ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_ARM_SYN_RX_CH, timeout);
		if (ret != OK)
			return ret;
		value = pmessage->state | (pmessage->attr << 8) |
			(pmessage->type << 16) | (pmessage->result << 24);
		writel(value, MSGBOX_RISC_TO_ARM_MSG_REG(HWMSGBOX_ARM_SYN_RX_CH));

		ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_ARM_SYN_RX_CH, timeout);
		if (ret != OK)
			return ret;
		value = pmessage->count;
		writel(value, MSGBOX_RISC_TO_ARM_MSG_REG(HWMSGBOX_ARM_SYN_RX_CH));

		/* then send message paras */
		for (i = 0; i < pmessage->count; i++) {
			ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_ARM_SYN_RX_CH, timeout);
			if (ret != OK)
				return ret;
			writel(pmessage->paras[i], MSGBOX_RISC_TO_ARM_MSG_REG(HWMSGBOX_ARM_SYN_RX_CH));
		}
		INF("feedback hard syn message : %x\n", pmessage->type);
		return OK;
	}

	/* invalid syn message */
	return -ESRCH;
}


/*
*********************************************************************************************************
*                                        QUERY MESSAGE
*
* Description:  query message of hwmsgbox syn channel by hand, mainly for.
*
* Arguments  :  none.
*
* Returns    :  the point of message, NULL if timeout.
*********************************************************************************************************
*/
s32 hwmsgbox_query_message(struct message *pmessage, u32 timeout)
{
	u32 i;
	u32 value;

	/* query ar100 asyn received channel */
	if (!!readl(MSGBOX_ARM_TO_RISC_MSG_STATUS_REG(HWMSGBOX_RISC_ASYN_RX_CH))) {
		LOG("query asyn msg\n");
		/* first receive message header and misc */
		while (!readl(MSGBOX_ARM_TO_RISC_MSG_STATUS_REG(HWMSGBOX_RISC_ASYN_RX_CH)))
			;
		value = readl(MSGBOX_ARM_TO_RISC_MSG_REG(HWMSGBOX_RISC_ASYN_RX_CH));
		pmessage->state = value & 0xff;
		pmessage->attr = (value >> 8) & 0xff;
		pmessage->type = (value >> 16) & 0xff;
		pmessage->result = (value >> 24) & 0xff;

		while (!readl(MSGBOX_ARM_TO_RISC_MSG_STATUS_REG(HWMSGBOX_RISC_ASYN_RX_CH)))
			;
		value = readl(MSGBOX_ARM_TO_RISC_MSG_REG(HWMSGBOX_RISC_ASYN_RX_CH));
		pmessage->count = value & 0xff;

		/* then receive message paras */
		for (i = 0; i < pmessage->count; i++) {
			while (!readl(MSGBOX_ARM_TO_RISC_MSG_STATUS_REG(HWMSGBOX_RISC_ASYN_RX_CH)))
				;
			pmessage->paras[i] = readl(MSGBOX_ARM_TO_RISC_MSG_REG(HWMSGBOX_RISC_ASYN_RX_CH));
		}

		/* clear pending */
		hwmsgbox_clear_receiver_pending(HWMSGBOX_RISC_ASYN_RX_CH);
		return OK;
	}

	/* query ar100 syn received channel */
	if (!!readl(MSGBOX_ARM_TO_RISC_MSG_STATUS_REG(HWMSGBOX_ARM_SYN_TX_CH))) {
		LOG("query syn msg\n");
		/* first receive message header and misc */
		while (!readl(MSGBOX_ARM_TO_RISC_MSG_STATUS_REG(HWMSGBOX_ARM_SYN_TX_CH)))
			;
		value = readl(MSGBOX_ARM_TO_RISC_MSG_REG(HWMSGBOX_ARM_SYN_TX_CH));
		pmessage->state = value & 0xff;
		pmessage->attr = (value >> 8) & 0xff;
		pmessage->type = (value >> 16) & 0xff;
		pmessage->result = (value >> 24) & 0xff;

		while (!readl(MSGBOX_ARM_TO_RISC_MSG_STATUS_REG(HWMSGBOX_ARM_SYN_TX_CH)))
			;
		value = readl(MSGBOX_ARM_TO_RISC_MSG_REG(HWMSGBOX_ARM_SYN_TX_CH));
		pmessage->count = value & 0xff;

		/* then receive message paras */
		for (i = 0; i < pmessage->count; i++) {
			while (!readl(MSGBOX_ARM_TO_RISC_MSG_STATUS_REG(HWMSGBOX_ARM_SYN_TX_CH)))
				;
			pmessage->paras[i] = readl(MSGBOX_ARM_TO_RISC_MSG_REG(HWMSGBOX_ARM_SYN_TX_CH));
		}

		/* clear pending */
		hwmsgbox_clear_receiver_pending(HWMSGBOX_ARM_SYN_TX_CH);
		return OK;
	}

	/* no valid message */
	return FAIL;
}


s32 hwmsgbox_super_standby_init(void)
{

	hwmsgbox_exit();

	return OK;
}

s32 hwmsgbox_super_standby_exit(void)
{
	hwmsgbox_init();

	return OK;
}


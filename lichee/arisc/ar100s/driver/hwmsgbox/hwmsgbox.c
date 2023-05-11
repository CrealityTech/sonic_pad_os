/**
 * driver\hwmsgbox\hwmsgbox.c
 *
 * Descript: hardware message-box driver.
 * Copyright (C) 2012-2016 AllWinnertech Ltd.
 * Author: Sunny <Sunny@allwinnertech.com>
 *
 */

#include "hwmsgbox_i.h"

static u32 hwmsg_suspend;

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
#define hwmsgbox_clear_receiver_pending(queue, user)\
	writel((0x1 << (queue * 2)), MSGBOX_IRQ_STATUS_REG(user))

/* ar100 wait message method: wait private flag to set 1 */
#define hwmsgbox_message_feedback(pmessage)  do { (pmessage)->private = 1; } while (0)



/*
*********************************************************************************************************
*                                           SET RECEIVER
*
* Description:  set user as receiver of one message-queue.
*
* Arguments  :  queue   : the number of message-queue which we want to set.
*               user    : the user which we want to set.
*
* Returns    :  OK if set user succeeded, others if failed.
*********************************************************************************************************
*/
static s32 hwmsgbox_set_receiver(u32 queue, u32 user)
{
	volatile u32 value;

	value  =  readl(MSGBOX_CTRL_REG(queue));
	value &= ~(1    << (0 + ((queue & 0x3)<<3)));
	value |=  (user << (0 + ((queue & 0x3)<<3)));
	writel(value, MSGBOX_CTRL_REG(queue));

	return OK;
}

/*
*********************************************************************************************************
*                                           SET TRANSMITTER
*
* Description:  set user as transmitter of one message-queue.
*
* Arguments  :  queue   : the number of message-queue which we want to set.
*               user    : the user which we want to set.
*
* Returns    :  OK if set user succeeded, others if failed.
*********************************************************************************************************
*/
static s32 hwmsgbox_set_transmitter(u32 queue, u32 user)
{
	volatile u32 value;

	value  =  readl(MSGBOX_CTRL_REG(queue));
	value &= ~(1    << (4 + ((queue & 0x3)<<3)));
	value |=  (user << (4 + ((queue & 0x3)<<3)));
	writel(value, MSGBOX_CTRL_REG(queue));

	return OK;
}

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

	/*
	 * ar100 asyn tx channel configure
	 * ar100 set as transmitter,
	 * ac327 set as receiver.
	 */
	hwmsgbox_set_transmitter(HWMSGBOX_AR100_ASYN_TX_CH, HWMSG_QUEUE_USER_ARISC);
	hwmsgbox_set_receiver   (HWMSGBOX_AR100_ASYN_TX_CH, HWMSG_QUEUE_USER_AC327);

	/*
	 * ar100 asyn rx channel configure
	 * ar100 set as receiver,
	 * ac327 set as transmitter.
	 */
	hwmsgbox_set_transmitter(HWMSGBOX_AR100_ASYN_RX_CH, HWMSG_QUEUE_USER_AC327);
	hwmsgbox_set_receiver   (HWMSGBOX_AR100_ASYN_RX_CH, HWMSG_QUEUE_USER_ARISC);

	/*
	 * ar100 syn tx channel configure
	 * ar100 set as transmitter,
	 * ac327 set as receiver.
	 */
	hwmsgbox_set_transmitter(HWMSGBOX_AR100_SYN_TX_CH, HWMSG_QUEUE_USER_ARISC);
	hwmsgbox_set_receiver   (HWMSGBOX_AR100_SYN_TX_CH, HWMSG_QUEUE_USER_AC327);

	/*
	 * ar100 syn rx channel configure
	 * ar100 set as receiver,
	 * ac327 set as transmitter.
	 */
	hwmsgbox_set_transmitter(HWMSGBOX_AR100_SYN_RX_CH, HWMSG_QUEUE_USER_AC327);
	hwmsgbox_set_receiver   (HWMSGBOX_AR100_SYN_RX_CH, HWMSG_QUEUE_USER_ARISC);

	/*
	 * ac327 syn tx channel configure
	 * ar100 set as receiver,
	 * ac327 set as transmitter.
	 */
	hwmsgbox_set_transmitter(HWMSGBOX_AC327_SYN_TX_CH, HWMSG_QUEUE_USER_AC327);
	hwmsgbox_set_receiver   (HWMSGBOX_AC327_SYN_TX_CH, HWMSG_QUEUE_USER_ARISC);

	/*
	 * ac327 syn rx channel configure
	 * ar100 set as transmitter,
	 * ac327 set as receiver.
	 */
	hwmsgbox_set_transmitter(HWMSGBOX_AC327_SYN_RX_CH, HWMSG_QUEUE_USER_ARISC);
	hwmsgbox_set_receiver   (HWMSGBOX_AC327_SYN_RX_CH, HWMSG_QUEUE_USER_AC327);

	/* register message-box interrupt handler.*/
	if (!hwmsg_suspend) {
		/*
		 * install_isr(INTC_M_BOX_IRQ, hwmsgbox_int_handler, NULL);
		 * interrupt_enable(INTC_M_BOX_IRQ);
		 */
	}

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
	while (readl(MSGBOX_FIFO_STATUS_REG(queue)) == 1) {
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
	while (readl(MSGBOX_MSG_STATUS_REG(queue)) == 0) {
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
		ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_AR100_SYN_TX_CH, timeout);
		if (ret != OK)
			return ret;
		value = pmessage->state | (pmessage->attr << 8) |
			(pmessage->type << 16) | (pmessage->result << 24);
		writel(value, MSGBOX_MSG_REG(HWMSGBOX_AR100_SYN_TX_CH));

		ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_AR100_SYN_TX_CH, timeout);
		if (ret != OK)
			return ret;
		value = pmessage->count;
		writel(value, MSGBOX_MSG_REG(HWMSGBOX_AR100_SYN_TX_CH));

		/* then send message paras */
		for (i = 0; i < pmessage->count; i++) {
			ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_AR100_SYN_TX_CH, timeout);
			if (ret != OK)
				return ret;
			writel(pmessage->paras[i], MSGBOX_MSG_REG(HWMSGBOX_AR100_SYN_TX_CH));
		}

		/* after send, wait feedback, hwsyn messsage must feedback use syn rx channel */

		/* first receive message header and misc */
		ret = hwmsgbox_wait_queue_not_empty(HWMSGBOX_AR100_SYN_RX_CH, timeout);
		if (ret != OK)
			return ret;
		value = readl(MSGBOX_MSG_REG(HWMSGBOX_AR100_SYN_RX_CH));
		pmessage->state = value & 0xff;
		pmessage->attr = (value >> 8) & 0xff;
		pmessage->type = (value >> 16) & 0xff;
		pmessage->result = (value >> 24) & 0xff;

		ret = hwmsgbox_wait_queue_not_empty(HWMSGBOX_AR100_SYN_RX_CH, timeout);
		if (ret != OK)
			return ret;
		value = readl(MSGBOX_MSG_REG(HWMSGBOX_AR100_SYN_RX_CH));
		pmessage->count = value & 0xff;

		/* then receive message paras */
		for (i = 0; i < pmessage->count; i++) {
			ret = hwmsgbox_wait_queue_not_empty(HWMSGBOX_AR100_SYN_RX_CH, timeout);
			if (ret != OK)
				return ret;
			pmessage->paras[i] = readl(MSGBOX_MSG_REG(HWMSGBOX_AR100_SYN_RX_CH));
		}
		return OK;
	} else {
		/* asyn message use asyn tx channel */
		INF("send asyn message\n");

		/* first send message header and misc */
		ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_AR100_ASYN_TX_CH, timeout);
		if (ret != OK)
			return ret;
		value = pmessage->state | (pmessage->attr << 8) |
			(pmessage->type << 16) | (pmessage->result << 24);
		writel(value, MSGBOX_MSG_REG(HWMSGBOX_AR100_ASYN_TX_CH));

		ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_AR100_ASYN_TX_CH, timeout);
		if (ret != OK)
			return ret;
		value = pmessage->count;
		writel(value, MSGBOX_MSG_REG(HWMSGBOX_AR100_ASYN_TX_CH));

		/* then send message paras */
		for (i = 0; i < pmessage->count; i++) {
			ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_AR100_ASYN_TX_CH, timeout);
			if (ret != OK)
				return ret;
			writel(pmessage->paras[i], MSGBOX_MSG_REG(HWMSGBOX_AR100_ASYN_TX_CH));
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
		ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_AC327_SYN_RX_CH, timeout);
		if (ret != OK)
			return ret;
		value = pmessage->state | (pmessage->attr << 8) |
			(pmessage->type << 16) | (pmessage->result << 24);
		writel(value, MSGBOX_MSG_REG(HWMSGBOX_AC327_SYN_RX_CH));

		ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_AC327_SYN_RX_CH, timeout);
		if (ret != OK)
			return ret;
		value = pmessage->count;
		writel(value, MSGBOX_MSG_REG(HWMSGBOX_AC327_SYN_RX_CH));

		/* then send message paras */
		for (i = 0; i < pmessage->count; i++) {
			ret = hwmsgbox_wait_queue_not_full(HWMSGBOX_AC327_SYN_RX_CH, timeout);
			if (ret != OK)
				return ret;
			writel(pmessage->paras[i], MSGBOX_MSG_REG(HWMSGBOX_AC327_SYN_RX_CH));
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
	if (!!readl(MSGBOX_MSG_STATUS_REG(HWMSGBOX_AR100_ASYN_RX_CH))) {
		LOG("query asyn msg\n");
		/* first receive message header and misc */
		while (!readl(MSGBOX_MSG_STATUS_REG(HWMSGBOX_AR100_ASYN_RX_CH)))
			;
		value = readl(MSGBOX_MSG_REG(HWMSGBOX_AR100_ASYN_RX_CH));
		pmessage->state = value & 0xff;
		pmessage->attr = (value >> 8) & 0xff;
		pmessage->type = (value >> 16) & 0xff;
		pmessage->result = (value >> 24) & 0xff;

		while (!readl(MSGBOX_MSG_STATUS_REG(HWMSGBOX_AR100_ASYN_RX_CH)))
			;
		value = readl(MSGBOX_MSG_REG(HWMSGBOX_AR100_ASYN_RX_CH));
		pmessage->count = value & 0xff;

		/* then receive message paras */
		for (i = 0; i < pmessage->count; i++) {
			while (!readl(MSGBOX_MSG_STATUS_REG(HWMSGBOX_AR100_ASYN_RX_CH)))
				;
			pmessage->paras[i] = readl(MSGBOX_MSG_REG(HWMSGBOX_AR100_ASYN_RX_CH));
		}

		/* clear pending */
		hwmsgbox_clear_receiver_pending(HWMSGBOX_AR100_ASYN_RX_CH, HWMSG_QUEUE_USER_ARISC);
		return OK;
	}

	/* query ar100 syn received channel */
	if (!!readl(MSGBOX_MSG_STATUS_REG(HWMSGBOX_AC327_SYN_TX_CH))) {
		LOG("query syn msg\n");
		/* first receive message header and misc */
		while (!readl(MSGBOX_MSG_STATUS_REG(HWMSGBOX_AC327_SYN_TX_CH)))
			;
		value = readl(MSGBOX_MSG_REG(HWMSGBOX_AC327_SYN_TX_CH));
		pmessage->state = value & 0xff;
		pmessage->attr = (value >> 8) & 0xff;
		pmessage->type = (value >> 16) & 0xff;
		pmessage->result = (value >> 24) & 0xff;

		while (!readl(MSGBOX_MSG_STATUS_REG(HWMSGBOX_AC327_SYN_TX_CH)))
			;
		value = readl(MSGBOX_MSG_REG(HWMSGBOX_AC327_SYN_TX_CH));
		pmessage->count = value & 0xff;

		/* then receive message paras */
		for (i = 0; i < pmessage->count; i++) {
			while (!readl(MSGBOX_MSG_STATUS_REG(HWMSGBOX_AC327_SYN_TX_CH)))
				;
			pmessage->paras[i] = readl(MSGBOX_MSG_REG(HWMSGBOX_AC327_SYN_TX_CH));
		}

		/* clear pending */
		hwmsgbox_clear_receiver_pending(HWMSGBOX_AC327_SYN_TX_CH, HWMSG_QUEUE_USER_ARISC);
		return OK;
	}

	/* no valid message */
	return FAIL;
}


s32 hwmsgbox_super_standby_init(void)
{
	hwmsg_suspend = 1;

	hwmsgbox_exit();

	return OK;
}

s32 hwmsgbox_super_standby_exit(void)
{
	hwmsgbox_init();

	hwmsg_suspend = 0;

	return OK;
}


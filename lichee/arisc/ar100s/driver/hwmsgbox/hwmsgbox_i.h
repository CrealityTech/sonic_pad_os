/**
 * driver\hwmsgbox\hwmsgbox_i.h
 *
 * Descript: hardware message-box internal header.
 * Copyright (C) 2012-2016 AllWinnertech Ltd.
 * Author: Sunny <Sunny@allwinnertech.com>
 *
 */

#ifndef __HWMSGBOX_I_H__
#define __HWMSGBOX_I_H__

#include "include.h"
/********************************************
 * for hwmsgbox.c
 ********************************************/
/*the user of hardware message queue.*/
typedef enum hwmsg_queue_user {
	HWMSG_QUEUE_USER_ARISC, /*arisc */
	HWMSG_QUEUE_USER_AC327, /*ac327 */
	HWMSG_QUEUE_USER_AC335, /*ac335/a15 */
} hwmsg_queue_user_e;

/* hardware message-box register list */
#define MSGBOX_REG_BASE             (HWMSGBOX_REG_BASE)
#define MSGBOX_CTRL_REG(m)          (MSGBOX_REG_BASE + 0x0000 + (0x4 * (m >> 2)))
#define MSGBOX_IRQ_EN_REG(u)        (MSGBOX_REG_BASE + 0x0040 + (0x20 * u))
#define MSGBOX_IRQ_STATUS_REG(u)    (MSGBOX_REG_BASE + 0x0050 + (0x20 * u))
#define MSGBOX_FIFO_STATUS_REG(m)   (MSGBOX_REG_BASE + 0x0100 + (0x4 * m))
#define MSGBOX_MSG_STATUS_REG(m)    (MSGBOX_REG_BASE + 0x0140 + (0x4 * m))
#define MSGBOX_MSG_REG(m)           (MSGBOX_REG_BASE + 0x0180 + (0x4 * m))
#define MSGBOX_DEBUG_REG            (MSGBOX_REG_BASE + 0x01c0)

/* the channel of hardware massage queue*/
#define HWMSGBOX_AR100_ASYN_TX_CH       (0)	/* hwmsgbox channels configure */
#define HWMSGBOX_AR100_ASYN_RX_CH       (1)
#define HWMSGBOX_AR100_SYN_TX_CH        (2)
#define HWMSGBOX_AR100_SYN_RX_CH        (3)
#define HWMSGBOX_AC327_SYN_TX_CH        (4)
#define HWMSGBOX_AC327_SYN_RX_CH        (5)

#endif /*__HWMSGBOX_I_H__*/

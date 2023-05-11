/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                   messages
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : messages.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-4-26
* Descript: the messages for asynchronous cpus communication.
* Update  : date                auther      ver     notes
*           2012-4-26 10:18:39  Sunny       1.0     Create this file.
*********************************************************************************************************
*/


#ifndef __MESSAGES_H__
#define __MESSAGES_H__

/* message attributes(only use 8bit) */
#define MESSAGE_ATTR_SOFTSYN        (1<<0)	/* need use soft syn with another cpu */
#define MESSAGE_ATTR_HARDSYN        (1<<1)	/* need use hard syn with another cpu */

/* message send timeout set*/
#define SEND_MSG_TIMEOUT                (100000)	/* send message max timeout, base on ms */

typedef struct message {
	unsigned char state;	/* identify the used status of message frame */
	unsigned char attr;	/* message attribute : SYN OR ASYN */
	unsigned char type;	/* message type : DVFS_REQ */
	unsigned char result;	/* message process result */
	unsigned char count;	/* message paras count unit of 4 byte */
	unsigned char reserved1[3];	/* reserved */
	unsigned int reserved2[5];	/* reserved for 8byte align */
	unsigned int *paras;	/* the point of message parameters */
} message_block_t;


/* the base of messages */
#define MESSAGE_BASE            (0x10)

/* standby commands */
#define SSTANDBY_ENTER_REQ          (MESSAGE_BASE + 0x00)	/* request to enter(ac327 to ar100)                 */
#define SSTANDBY_RESTORE_NOTIFY     (MESSAGE_BASE + 0x01)	/* restore finished(ac327 to ar100)                 */
#define ESSTANDBY_ENTER_REQ         (MESSAGE_BASE + 0x06)	/* request to enter       (ac327 to ar100)          */
#define FAKE_POWER_OFF_REQ          (MESSAGE_BASE + 0x09)	/* fake power off request(ac327 to ar100)           */
#define CPUIDLE_ENTER_REQ           (MESSAGE_BASE + 0x0A)	/* request to enter cpuidle (ac327 to ar200)        */
#define CPUIDLE_CFG_REQ             (MESSAGE_BASE + 0x11)	/* request to config      (ac327 to arisc)          */
#define CPU_OP_REQ                  (MESSAGE_BASE + 0x12)	/* cpu operations         (ac327 to arisc)          */
#define SYS_OP_REQ                  (MESSAGE_BASE + 0x14)	/* cpu operations         (ac327 to arisc)          */
#define CLEAR_WAKEUP_SRC_REQ        (MESSAGE_BASE + 0x15)
#define SET_WAKEUP_SRC_REQ          (MESSAGE_BASE + 0x16)

//set debug level commands
#define SET_DEBUG_LEVEL_REQ         (MESSAGE_BASE + 0x50)	/* set debug level   (ac327 to ar100)               */
#define MESSAGE_LOOPBACK            (MESSAGE_BASE + 0x51)	/* loopback message  (ac327 to ar100)               */
#define SET_UART_BAUDRATE           (MESSAGE_BASE + 0x52)	/* set uart baudrate (ac327 to ar100)               */
#define SET_DRAM_PARAS              (MESSAGE_BASE + 0x53)	/* config dram parameter (ac327 to ar100)           */
#define SET_DRAM_CRC_PARAS          (MESSAGE_BASE + 0x54)	/* config dram crc parameters (ac327 to ar100)      */
#define SET_IR_PARAS                (MESSAGE_BASE + 0x55)	/* config ir parameter (ac327 to ar100)             */
#define SET_PARAS                   (MESSAGE_BASE + 0x57)	/* set paras (arisc to ac327)                       */

#define CRASHDUMP_RESET             (MESSAGE_BASE + 0x85)	/* crashdump (arisc to ac327)                       */

/* ar100 initialize state notify commands */
#define AR100_STARTUP_NOTIFY        (MESSAGE_BASE + 0x80)	/* ar100 init state notify(ar100 to ac327)          */

#endif	/*__MESSAGES_H__*/

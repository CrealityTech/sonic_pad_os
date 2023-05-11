/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                timer  module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : timer.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-4-27
* Descript: timer controller public interfaces.
* Update  : date                auther      ver     notes
*           2012-4-27 17:03:52  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __TIMER_EXTENDED_H__
#define __TIMER_EXTENDED_H__

#include "include.h"

/* the base address of timer register */
#define TIMER_REG_BASE (R_TMR01_REG_BASE)
#define PRCM_REG_BASE  (R_PRCM_REG_BASE)

#define EXT_TIMER_IRQ_REG	(TIMER_REG_BASE + 0x00)
#define EXT_TIMER_STA_REG	(TIMER_REG_BASE + 0x04)
#define EXT_TIMER_CTRL_REG(n)	(TIMER_REG_BASE + (0x20) * (n + 1))
#define EXT_TIMER_IVL_REG(n)	(TIMER_REG_BASE + (0x20) * (n + 1) + 0x04)
#define EXT_TIMER_IVH_REG(n)	(TIMER_REG_BASE + (0x20) * (n + 1) + 0x0c)
#define EXT_TIMER_CVL_REG(n)	(TIMER_REG_BASE + (0x20) * (n + 1) + 0x08)
#define EXT_TIMER_CVH_REG(n)	(TIMER_REG_BASE + (0x20) * (n + 1) + 0x10)

#define EXT_TIMER_CLK_REG(n)	(R_PRCM_REG_BASE + 0x110 + n * 0x4)

/* the status of timer, been used or free */
typedef enum timer_status {
	TIMER_FREE = 0,
	TIMER_USED
} timer_status_e;

/*  timer structure */
typedef struct timer {
	u32 timer_no;
	u32 status;
	u32 irq_no;
	u32 ms_ticks;
	__pCBK_t phandler;
	void *parg;
} timer_t;

/* timer isr */
int timer_isr(void *parg);
s32 timer_hosc_onoff_cb(u32 message, u32 aux);

/* external vars */
extern struct timerc_regs *ptimerc_regs;
extern struct timer *delay_timer;

#endif	/*__TIMER_EXTENDED_H__ */

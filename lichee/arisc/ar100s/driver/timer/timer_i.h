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

#ifndef __TIMER_I_H__
#define __TIMER_I_H__

#include "include.h"

/* the base address of timer register */
#define TIMER_REG_BASE (R_TMR01_REG_BASE)

/* hardware timer registers */
typedef struct timer_regs {
	volatile u32 control;
	volatile u32 interval_value;
	volatile u32 count;
	volatile u32 reserved0[1];
} timer_regs_t;

/* timer controller registers */
typedef struct timerc_regs {
	/* irq control */
	volatile u32 irq_enbale;
	volatile u32 irq_status;
	volatile u32 reserved0[2];

	/* timer registers */
	struct timer_regs timer[TIMERC_TIMERS_NUMBER];
} timerc_regs_t;

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
	timer_regs_t *pregs;
} timer_t;

/* timer isr */
int timer_isr(void *parg);
s32 timer_hosc_onoff_cb(u32 message, u32 aux);

/* external vars */
extern struct timerc_regs *ptimerc_regs;
extern struct timer *delay_timer;

#endif	/*__TIMER_I_H__ */

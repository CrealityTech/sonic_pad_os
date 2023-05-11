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
* Descript: timer controller public header.
* Update  : date                auther      ver     notes
*           2012-4-27 17:03:52  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __TIMER_H__
#define __TIMER_H__

#include "ibase.h"
#include "../driver.h"

#define REQUSET_TIMER	0x001
#define RELEASE_TIMER	0x002
#define START_TIMER	0x003
#define STOP_TIMER	0x004
#define	TIMER_CLEARING_PENDING 0x005

typedef struct {
	arisc_device dev;
	u32 timer_number;
	TIM_InitConfig *config;
	const struct timer_ops *ops;
	TIM_TypedDef *TIMERx;
	struct list_head tmr_list;
	s32 (*clkchangecb)(u32 command, void *parg);
} timer_device;


timer_device *request_timer(u32 timer_number,__pISR_t phdle);
s32 timer_exit(void);
timer_device *query_timer(u32 timer_number);
HANDLE timer_request(__pISR_t phdle);
s32 timer_release(HANDLE htimer);
s32 timer_start(HANDLE htimer, u32 period, u32 irq_enable);
s32 timer_stop(HANDLE htimer);
s32 timer_isr(void *parg,u32 intno);
s32 timer_init(void);
#endif  //__TIMER_H__

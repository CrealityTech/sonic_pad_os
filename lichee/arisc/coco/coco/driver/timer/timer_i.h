/*
 *********************************************************************************************************
 *                                                AR100 SYSTEM
 *                                     AR100 Software System Develop Kits
 *
 *
 *                                    (c) Copyright 2018-2024, Luojie China
 *)                                             All Rights Reserved
 *
 * File    : timer.h
 * By      : LuoJie
 * Version : v1.0
 * Date    : 2018-04-12 15:43
 * Descript: .
 * Update  :auther      ver     notes
 *          Luojie      1.0     Create this file.
 *********************************************************************************************************
 */

#ifndef __TIMER_I_H__
#define __TIMER_I_H__

#include "ibase.h"
#include "driver.h"
#include "list.h"

#define TIMER_REG_BASE (R_TMR01_REG_BASE)

//timer isr
//int timer_isr(void *parg,u32 intno);
s32 timer_hosc_onoff_cb(u32 message, void *parg);



struct timer_ops {
	HANDLE (*request)(__pISR_t phdle);
	s32 (*release)(HANDLE htimer);
	s32 (*start)(HANDLE htimer, u32 period, u32 irq_enable);
	s32 (*stop)(HANDLE htimer);
};



#endif  //__TIMER_I_H__

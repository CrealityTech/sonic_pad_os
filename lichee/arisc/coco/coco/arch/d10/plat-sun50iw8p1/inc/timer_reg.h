/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*)                                             All Rights Reserved
*
* File    : timer_reg.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-04-11 17:13
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __TIMER_REG_H__
#define __TIMER_REG_H__

#include "lib_inc.h"
//the total number of hardware timer
#define TIMERC_TIMERS_NUMBER (4)

#define TIMER_IRQ_EN_REG	0x00
#define TIMER_IRQ_STA_REG	0x04
#define TIMER0_CTRL_REG		0x10
#define TIMER0_INTV_REG		0x14
#define TIMER0_CURV_REG		0x18
#define TIMER1_CTRL_REG		0x20
#define TIMER1_INTV_REG		0x24
#define TIMER1_CURV_REG		0x28
#define TIMER2_CTRL_REG		0x30
#define TIMER2_INTV_REG		0x34
#define TIMER2_CURV_REG		0x38
#define TIMER_VERSION		0x90

#define TIM_Mode_Cycle		0x0
#define TIM_Mode_Single		0x1




//hardware timer registers
typedef struct timer_regs
{
	volatile u32  control;
	volatile u32  interval_value;
	volatile u32  count;
	volatile u32  reserved0[1];
} TIM_TypedDef;

//timer controller registers
typedef struct timerc_regs
{
	//irq control
	volatile u32       irq_enbale;
	volatile u32       irq_status;
	volatile u32       reserved0[2];

	//timer registers
	struct timer_regs  timer[TIMERC_TIMERS_NUMBER];
} timer_reg;





#endif


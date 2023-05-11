/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : time.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:34
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __TIME_H__
#define __TIME_H__

#include "ibase.h"

#define SYSTICK_TMRNUM	0
#define DELAY_TMRNUM	1


u32 current_time_tick(void);

u32 mecs_to_ticks(u32 ms);

s32 ticktimer_init(void);




s32 delaytimer_init(void);

void time_mdelay(u32 ms);

void time_udelay(u32 us);

void time_cdelay(u32 cycles);


#define udelay(us) time_udelay(us)
#define mdelay(ms) time_mdelay(ms)

#endif



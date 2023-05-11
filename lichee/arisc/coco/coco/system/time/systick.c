/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : systick.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:31
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "ibase.h"
#include "driver.h"
#include "task.h"
#include "notifier.h"

struct tick_timer
{
	timer_device *timer;
	u64 ticks;
};

struct tick_timer ticktimer = {
	.ticks = 0,
};

extern task_struct daemon_task;

extern struct notifier *softtimer_notify;

static s32 ticktimer_handler(void *parg, u32 intno)
{
	parg = parg;
	intno = intno;

	ticktimer.ticks ++;
	notifier_notify(&softtimer_notify,0);

	Timer_ClearPending(ticktimer.timer->TIMERx);
	return OK;
}

u32 current_time_tick(void)
{
	return  ticktimer.ticks;
}

u32 mecs_to_ticks(u32 ms)
{
	return (TICK_PER_SEC * ms) / 1000;
}

s32 ticktimer_init(void)
{
	ticktimer.timer = request_timer(SYSTICK_TMRNUM,ticktimer_handler);

	if (ticktimer.timer)
	{
		u32 period;

		//start sysem time tick. period base on ms.
#ifdef CFG_FPGA_FLATFORM
		period = 240;
#else
		period = 1000 / TICK_PER_SEC * Timer_GetMsTick(ticktimer.timer->TIMERx);
#endif
		timer_start(ticktimer.timer, period, 1);
		interrupt_enable(ticktimer.timer->dev.irq_no);
		return OK;
	}
	//request timer failed
	return -EFAIL;
}






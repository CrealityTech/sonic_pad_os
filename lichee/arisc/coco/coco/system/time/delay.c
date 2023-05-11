/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : delay.c
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
#include "cpucfg.h"


timer_device *delaytimer;

void time_mdelay(u32 ms)
{
	u32 period;
	u32 cpsr;

	cpsr = cpu_disable_int();
	period = Timer_GetMsTick(delaytimer->TIMERx) * ms;

	timer_start((void *)delaytimer,period,1);

	while(Timer_GetIRQStatus(delaytimer->TIMERx) != 1);

	timer_stop((void *)delaytimer);

	Timer_ClearPending(delaytimer->TIMERx);
	cpu_enable_int(cpsr);
}

s32 delaytimer_init(void)
{
	delaytimer = request_timer(DELAY_TMRNUM,NULL);

	Timer_SetMode(delaytimer->TIMERx,TIM_Mode_Single);

	return OK;
}

/*
static void cnt64_udelay(u32 us)
{
	u64 expire = 0;

	if (us == 0)
		return;

	//calc expire time
	expire = (us * 24) + timestamp_read();
	while (expire > timestamp_read())
				;
}
*/
/*
static void cnt_delay(volatile u32 count)
{
	while(count--);
}
*/

void time_udelay(u32 us)
{
	u32 cycles;
	u32 cpus_freq;
	ccu_device * prcm;

	prcm = get_prcm_device();

	if (us == 0)
		return;

#ifdef CFG_EVB_FLATFORM
	cpus_freq = prcm->freq;
#elif defined CFG_FPGA_FLATFORM
	cpus_freq = 24000000;
#endif

	cycles = (cpus_freq * us + (1000000 - 1)) / 1000000;
	time_cdelay(cycles);
//	cnt_delay(cycles/3);
//	cnt64_udelay(cycles);

}







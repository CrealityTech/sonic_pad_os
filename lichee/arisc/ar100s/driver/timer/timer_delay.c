/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                timer  module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : timer_delay.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-3
* Descript: timer delay service.
* Update  : date                auther      ver     notes
*           2012-5-3 14:12:17   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "timer_i.h"

extern volatile u32 timer_lock;

void time_mdelay(u32 ms)
{
	if (timer_lock)
		return;

	/*
	 * check delay time too long
	 * ...
	 */
	if ((ms == 0) || (delay_timer->ms_ticks * ms >= 4294967296)) {
		/* no delay */
		return;
	}

	/* config timer internal value */
	delay_timer->pregs->interval_value = delay_timer->ms_ticks * ms;
	delay_timer->pregs->control |= (1 << 1);	/* reload interval value to current value */
	while ((delay_timer->pregs->control) & (1 << 1))
		;

	/* clear timer pending */
	ptimerc_regs->irq_status = (1 << delay_timer->timer_no);

	/* start timer */
	delay_timer->pregs->control |= 0x1;

	/* check timer pending valid or not */
	while ((ptimerc_regs->irq_status & (1 << delay_timer->timer_no)) == 0) {
		;
	}

	/* stop timer */
	delay_timer->pregs->control &= ~(0x1);

	/* clear timer pending */
	ptimerc_regs->irq_status = (1 << delay_timer->timer_no);
}

void cnt64_udelay(u32 us)
{
	u64 expire = 0;

	if (us == 0) {
		/* no delay */
		return;
	}

	/* calc expire time */
	expire = (us * 24) + cpucfg_counter_read();
	while (expire > cpucfg_counter_read()) {
		/* wait busy */
		;
	}
}

void time_udelay(u32 us)
{
	u32 cycles;
	u32 cpus_freq;

	if (us == 0) {
		/* no delay */
		return;
	}

	if (is_hosc_lock() || is_suspend_lock()) {
		cpus_freq = ccu_get_sclk_freq(CCU_SYS_CLK_CPUS);
		cycles = (cpus_freq * us + (1000000 - 1)) / 1000000;
		time_cdelay(cycles);
	} else {
		cnt64_udelay(us);
	}

}

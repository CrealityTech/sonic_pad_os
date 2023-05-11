/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                timer  module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : timer_delay-extend.c
* By      : AWA1442
* Version : v1.0
* Date    : 2020-11-16
* Descript: timer delay service.
* Update  : date                auther      ver     notes
*           2020-11-16 14:12:17   AWA1442       1.0     Create this file.
*********************************************************************************************************
*/

#include "timer-extended.h"

extern volatile u32 timer_lock;

void time_mdelay(u32 ms)
{
	u32 value;

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
	writel(delay_timer->ms_ticks * ms, EXT_TIMER_IVL_REG(delay_timer->timer_no));

	/*  reload interval value to current value */
	value = readl(EXT_TIMER_CTRL_REG(delay_timer->timer_no));
	value |= (1 << 1);
	writel(value, EXT_TIMER_CTRL_REG(delay_timer->timer_no));

	while (readl(EXT_TIMER_CTRL_REG(delay_timer->timer_no)) & (1 << 1))
		;

	/* clear timer pending */
	writel((1 << delay_timer->timer_no), EXT_TIMER_STA_REG);

	/* start timer */
	value = readl(EXT_TIMER_CTRL_REG(delay_timer->timer_no));
	value |= 0x1;
	writel(value, EXT_TIMER_CTRL_REG(delay_timer->timer_no));

	/* check timer pending valid or not */
	while ((readl(EXT_TIMER_STA_REG) & (1 << delay_timer->timer_no)) == 0)
		;

	/* stop timer */
	value = readl(EXT_TIMER_CTRL_REG(delay_timer->timer_no));
	value &= ~(0x1);
	writel(value, EXT_TIMER_CTRL_REG(delay_timer->timer_no));

	/* clear timer pending */
	writel((1 << delay_timer->timer_no), EXT_TIMER_STA_REG);
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

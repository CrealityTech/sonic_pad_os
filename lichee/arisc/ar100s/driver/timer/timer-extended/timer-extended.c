/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                timer  module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : timer-extend.c
* By      : AWA1442
* Version : v1.0
* Date    : 2020-11-16
* Descript: timer controller for new timer IP.
* Update  : date                auther      ver     notes
*           2020-11-16 14:12:17   AWA1442       1.0     Create this file.
*********************************************************************************************************
*/

#include "timer-extended.h"

volatile u32 timer_lock = 1;

/* delay timer handler */
struct timer *delay_timer;

/* the table of timers */
static struct timer timers[TIMERC_TIMERS_NUMBER] = {
	{ 0, TIMER_FREE, INTC_R_TIMER0_IRQ, 0, NULL, NULL },
	{ 1, TIMER_FREE, INTC_R_TIMER1_IRQ, 0, NULL, NULL },
};


/*
*********************************************************************************************************
*                                       INIT TIMER
*
* Description:  initialize timer.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize timer succeeded, others if failed.
*********************************************************************************************************
*/
s32 timer_init(void)
{
	u32 index;
	u32 value;

	/* enbale timer clock gating. */
	ccu_set_mclk_onoff(CCU_MOD_CLK_R_TIMER0_1, CCU_CLK_ON);

	/* set reset as de-assert state. */
	ccu_set_mclk_reset(CCU_MOD_CLK_R_TIMER0_1, CCU_CLK_NRESET);

	/* initialize timers */
	for (index = 0; index < TIMERC_TIMERS_NUMBER; index++) {
		writel(0x0, EXT_TIMER_CTRL_REG(index));

		/*
		 * timer tick time base on ms,
		 * soucre clock = 24M, pre-scale = 1, timer source clock = 24M.
		 */
		/* source clock = 24M */
		value = readl(EXT_TIMER_CLK_REG(index));
		value &= ~(0x3 << 4);
		value |= (0x0 << 4);
		writel(value, EXT_TIMER_CLK_REG(index));

		/* pre-scale = 1 */
		value = readl(EXT_TIMER_CLK_REG(index));
		value &= ~(0x7 << 1);
		value |= (0x0 << 1);
		writel(value, EXT_TIMER_CLK_REG(index));
#ifdef CFG_FPGA_PLATFORM
		timers[index].ms_ticks = 32;	/* fix to 32k */
#else
		timers[index].ms_ticks = 24 * 1000;	/* 24M */
#endif
	}

	/*
	 * use timer[0] for system accurate delay service, tick base ms.
	 * single shot mode.
	 * by sunny at 2012-11-21 17:39:25.
	 */
	timers[0].status = TIMER_USED;
	delay_timer = &(timers[0]);

	/* set timer0 mode to single mode */
	value = readl(EXT_TIMER_CTRL_REG(0));
	value |= (0x1 << 7);
	writel(value, EXT_TIMER_CTRL_REG(0));

	/* register 24mhosc notifier call-back */
	ccu_24mhosc_reg_cb(timer_hosc_onoff_cb);
	timer_lock = 0;

	return OK;
}

/*
*********************************************************************************************************
*                                       EXIT TIMER
*
* Description:  exit timer.
*
* Arguments  :  none.
*
* Returns    :  OK if exit timer succeeded, others if failed.
*********************************************************************************************************
*/
s32 timer_exit(void)
{
	/* set reset as de-assert state. */
	ccu_set_mclk_reset(CCU_MOD_CLK_R_TIMER0_1, CCU_CLK_RESET);

	/* enbale timer clock gating. */
	ccu_set_mclk_onoff(CCU_MOD_CLK_R_TIMER0_1, CCU_CLK_OFF);

	timer_lock = 1;

	return OK;
}

/*
*********************************************************************************************************
*                                       REQUEST TIMER
*
* Description:  request a hardware timer.
*
* Arguments  :  phdle   : the callback when the requested timer tick reached.
*               parg    : the argument for the callback.
*
* Returns    :  the handler if request hardware timer succeeded, NULL if failed.
*
* Note       :  the callback execute entironment : CPU disable interrupt response.
*********************************************************************************************************
*/
HANDLE timer_request(__pCBK_t phdle, void *parg)
{
	u32 cpsr;
	u32 index;
	struct timer *ptimer = NULL;

	if (timer_lock)
		return NULL;

	cpsr = cpu_disable_int();
	for (index = 0; index < TIMERC_TIMERS_NUMBER; index++) {
		if (timers[index].status == TIMER_FREE) {
			/* allocate this timer */
			ptimer = &(timers[index]);
			ptimer->status = TIMER_USED;
			ptimer->phandler = phdle;
			ptimer->parg = parg;
			break;
		}
	}
	cpu_enable_int(cpsr);

	if (ptimer == NULL) {
		/* no freed timer now */
		WRN("no free timer now\n");
		return NULL;
	}

	/* install timer isr */
	install_isr(ptimer->irq_no, timer_isr, (void *)ptimer);

	return (HANDLE) ptimer;
}

/*
*********************************************************************************************************
*                                       RELEASE TIMER
*
* Description:  release a hardware timer.
*
* Arguments  :  htimer  : the handler of the released timer.
*
* Returns    :  OK if release hardware timer succeeded, others if failed.
*********************************************************************************************************
*/
s32 timer_release(HANDLE htimer)
{
	struct timer *ptimer = (struct timer *)htimer;

	ASSERT(ptimer != NULL);

	/* set timer status as free */
	ptimer->status = TIMER_FREE;
	ptimer->phandler = NULL;
	ptimer->parg = NULL;

	return OK;
}

/*
*********************************************************************************************************
*                                       START TIMER
*
* Description:  start a hardware timer.
*
* Arguments  :  htimer  : the timer handler which we want to start.
*               period  : the period of the timer trigger, base on ms.
*               mode    : the mode the timer trigger, details please
*                         refer to timer trigger mode.
*
* Returns    :  OK if start hardware timer succeeded, others if failed.
*********************************************************************************************************
*/
s32 timer_start(HANDLE htimer, u32 period, u32 mode)
{
	u32 value;

	struct timer *ptimer = (struct timer *)htimer;

	ASSERT(ptimer != NULL);

	if (timer_lock)
		return -EACCES;

	/* set timer period */
	writel((ptimer->ms_ticks * period), EXT_TIMER_IVL_REG(ptimer->timer_no));

	/* reload interval value to current value */
	value = readl(EXT_TIMER_CTRL_REG(ptimer->timer_no));
	value |= (1 << 1);
	writel(value, EXT_TIMER_CTRL_REG(ptimer->timer_no));

	while (readl(EXT_TIMER_CTRL_REG(ptimer->timer_no)) & (1 << 1))
		;

	/* set timer mode */
	value = readl(EXT_TIMER_CTRL_REG(ptimer->timer_no));
	value &= ~(1 << 7);
	value |= (mode << 7);
	writel(value, EXT_TIMER_CTRL_REG(ptimer->timer_no));

	/* clear timer pending */
	writel((1 << ptimer->timer_no), EXT_TIMER_STA_REG);

	/* enable timer interrupt */
	value = readl(EXT_TIMER_IRQ_REG);
	value |= (1 << ptimer->timer_no);
	writel(value, EXT_TIMER_IRQ_REG);
	interrupt_enable(ptimer->irq_no);

	/* enable timer */
	value = readl(EXT_TIMER_CTRL_REG(ptimer->timer_no));
	value |= 0x1;
	writel(value, EXT_TIMER_CTRL_REG(ptimer->timer_no));

	return OK;
}

/*
*********************************************************************************************************
*                                       STOP TIMER
*
* Description:  stop a hardware timer.
*
* Arguments  :  htimer  : the timer handler which we want to stop.
*
* Returns    :  OK if stop hardware timer succeeded, others if failed.
*********************************************************************************************************
*/
s32 timer_stop(HANDLE htimer)
{
	struct timer *ptimer = (struct timer *)htimer;

	u32 value;

	ASSERT(ptimer != NULL);

	if (timer_lock)
		return -EACCES;

	/* disable timer */
	value = readl(EXT_TIMER_CTRL_REG(ptimer->timer_no));
	value &= ~(0x1);
	writel(value, EXT_TIMER_CTRL_REG(ptimer->timer_no));

	/* disable timer irq */
	value = readl(EXT_TIMER_IRQ_REG);
	value &= ~(1 << ptimer->timer_no);
	writel(value, EXT_TIMER_IRQ_REG);
	interrupt_disable(ptimer->irq_no);

	/* clear timer pending */
	writel((1 << ptimer->timer_no), EXT_TIMER_STA_REG);

	return OK;
}

/*
*********************************************************************************************************
*                                       TIMER ISR
*
* Description:  the isr for timer interrupt.
*
* Arguments  :  parg    : the argument for timer isr.
*
* Returns    :  TRUE if process timer interrupt succeeded, others if failed.
*********************************************************************************************************
*/
s32 timer_isr(void *parg)
{
	struct timer *ptimer = (struct timer *)parg;

	if (timer_lock)
		return -EACCES;

	/* check pending status valid or not */
	if (readl(EXT_TIMER_STA_REG) & (1 << ptimer->timer_no)) {
		/* process the timer handler */
		if (ptimer->phandler == NULL) {
			WRN("timer irq handler not install\n");
			return FALSE;
		}

		/* handler timer irq */
		ptimer->phandler(ptimer->parg);

		/* clear interrupt pending */
		writel((1 << ptimer->timer_no), EXT_TIMER_STA_REG);

		return TRUE;
	}
	return FALSE;
}

s32 timer_hosc_onoff_cb(u32 message, u32 aux)
{
#ifdef CFG_FPGA_PLATFORM
	return OK;
#else

	u32 index;
	u32 value;

	if (timer_lock)
		return -EACCES;

	switch (message) {
	case CCU_HOSC_WILL_OFF_NOTIFY:
		{
			/*
			 * 24m hosc will power-off,
			 * timer switch clock source to 32k losc.
			 */
			INF("24m hosc will power-off notify\n");
			for (index = 0; index < TIMERC_TIMERS_NUMBER; index++) {
				/* set source clock to 32k */
				value = readl(EXT_TIMER_CLK_REG(index));
				value &= ~(0x3 << 4);
				value |= (0x1 << 4);
				writel(value, EXT_TIMER_CLK_REG(index));

				timers[index].ms_ticks = 32;	/* 32 */
				value = readl(EXT_TIMER_IVL_REG(index));
				value = value / (24000 / 32);
				writel(value, EXT_TIMER_IVL_REG(index));
			}
			return OK;
		}
	case CCU_HOSC_ON_READY_NOTIFY:
		{
			/*
			 * 24m hosc power-on already,
			 * timer swith source clock to 24m hosc.
			 */
			INF("24m hosc power-on ready notify\n");
			for (index = 0; index < TIMERC_TIMERS_NUMBER; index++) {
				value = readl(EXT_TIMER_IVL_REG(index));
				value = value * (24000 / 32);
				writel(value, EXT_TIMER_IVL_REG(index));

				/* set source clock to 24M */
				value = readl(EXT_TIMER_CLK_REG(index));
				value &= ~(0x3 << 4);
				value |= (0x0 << 4);
				writel(value, EXT_TIMER_CLK_REG(index));
				timers[index].ms_ticks = 24 * 1000;	/* 24k */
			}
			return OK;
		}
	default:
		{
			break;
		}
	}
	return -ESRCH;
#endif
}

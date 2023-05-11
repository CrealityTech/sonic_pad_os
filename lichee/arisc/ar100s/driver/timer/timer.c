/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                timer  module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : timer.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-3
* Descript: timer controller.
* Update  : date                auther      ver     notes
*           2012-5-3 14:12:17   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "timer_i.h"

volatile u32 timer_lock = 1;

/* poniter of timer controller registers */
struct timerc_regs *ptimerc_regs;

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

	/* initialize the pointer of timer controller registers */
	ptimerc_regs = (struct timerc_regs *)(TIMER_REG_BASE);

	/* enbale timer clock gating. */
	ccu_set_mclk_onoff(CCU_MOD_CLK_R_TIMER0_1, CCU_CLK_ON);

	/* set reset as de-assert state. */
	ccu_set_mclk_reset(CCU_MOD_CLK_R_TIMER0_1, CCU_CLK_NRESET);

	/* initialize timers */
	for (index = 0; index < TIMERC_TIMERS_NUMBER; index++) {
		timers[index].pregs = &(ptimerc_regs->timer[index]);
		timers[index].pregs->control = 0;

		/*
		 * timer tick time base on ms,
		 * soucre clock = 24M, pre-scale = 1, timer source clock = 24M.
		 */
		timers[index].pregs->control &= ~(0x3 << 2);
		timers[index].pregs->control |= (0x1 << 2);	/* source clock = 24M */
		timers[index].pregs->control &= ~(0x7 << 4);
		timers[index].pregs->control |= (0x0 << 4);	/* pre-scale = 1 */
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
	delay_timer->pregs->control |= (0x1 << 7);

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

	ptimerc_regs = NULL;

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
	struct timer *ptimer = (struct timer *)htimer;

	ASSERT(ptimer != NULL);

	if (timer_lock)
		return -EACCES;

	/* set timer period */
	ptimer->pregs->interval_value = ptimer->ms_ticks * period;
	ptimer->pregs->control |= (1 << 1);	/* reload interval value to current value */
	while ((ptimer->pregs->control) & (1 << 1))
		;

	/* set timer mode */
	ptimer->pregs->control &= ~(1 << 7);
	ptimer->pregs->control |= (mode << 7);

	/* clear timer pending */
	ptimerc_regs->irq_status = (1 << ptimer->timer_no);

	/* enable timer interrupt */
	ptimerc_regs->irq_enbale |= (1 << ptimer->timer_no);
	interrupt_enable(ptimer->irq_no);

	/* enable timer */
	ptimer->pregs->control |= 0x1;

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

	ASSERT(ptimer != NULL);

	if (timer_lock)
		return -EACCES;

	/* disable timer */
	ptimer->pregs->control &= ~(0x1);

	/* disable timer irq */
	ptimerc_regs->irq_enbale &= ~(1 << ptimer->timer_no);
	interrupt_disable(ptimer->irq_no);

	/* clear timer pending */
	ptimerc_regs->irq_status = (1 << ptimer->timer_no);

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
	if (ptimerc_regs->irq_status & (1 << ptimer->timer_no)) {
		/* process the timer handler */
		if (ptimer->phandler == NULL) {
			WRN("timer irq handler not install\n");
			return FALSE;
		}

		/* handler timer irq */
		ptimer->phandler(ptimer->parg);

		/* clear interrupt pending */
		ptimerc_regs->irq_status = (1 << ptimer->timer_no);

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
				timers[index].pregs->control &= ~(0x3 << 2);
				timers[index].pregs->control |= (0x0 << 2);	/* source clock = 32k */
				timers[index].ms_ticks = 32;	/* 32 */
				timers[index].pregs->interval_value = \
					timers[index].pregs->interval_value / (24000 / 32);
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
				timers[index].pregs->interval_value = \
					timers[index].pregs->interval_value * (24000 / 32);
				timers[index].pregs->control &= ~(0x3 << 2);
				timers[index].pregs->control |= (0x1 << 2);	/* source clock = 24M */
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

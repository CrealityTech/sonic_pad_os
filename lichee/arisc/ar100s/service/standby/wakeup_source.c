#include "include.h"
#include "wakeup_source.h"
#include "irq_table.h"

u32 volatile wakeup_source;

static struct softtimer wakeup_timer;

static s32 wakeup_timer_handler(void *parg)
{
	wakeup_source = WAKESOURE_TIMER;

	return OK;
}

s32 wakeup_timer_init(void)
{
	wakeup_timer.cycle = 0;
	wakeup_timer.expires = 0;
	wakeup_timer.cb = wakeup_timer_handler;
	wakeup_timer.arg = NULL;
	wakeup_timer.start = SOFTTIMER_OFF;
	add_softtimer(&wakeup_timer);

	return OK;
}

void wakeup_timer_start(void)
{
	if (wakeup_timer.cycle != 0)
		start_softtimer(&wakeup_timer);
}

void wakeup_timer_stop(void)
{
	stop_softtimer(&wakeup_timer);
}

s32 default_wakeup_handler(void *parg)
{
	u32 intno = interrupt_get_current_intno();

	/*
	 * when wakeup event happen, we should mask the r_intc interrupt here first,
	 * otherwise cpus can't go out of the interrupt handler and the resume process can't go ahead.
	 */
	interrupt_set_mask(intno, TRUE);

	wakeup_source = CPUS_IRQ_MAPTO_CPUX(intno);

	return OK;
}

static int group_irq_init(void *parg)
{
	u32 *p = (u32 *)parg;
	u32 wakeup_root_irq = p[0];

	s32 group_irq_num = CPUX_IRQ_MAPTO_CPUS_GRP(wakeup_root_irq);

	/* this is a group irq, we need to set group config, otherwise do nothing. */
	if (group_irq_num != FAIL)
		interrupt_set_group_config(group_irq_num, TRUE);

	return OK;
}

static int group_irq_exit(void *parg)
{
	u32 *p = (u32 *)parg;
	u32 wakeup_root_irq = p[0];

	s32 group_irq_num = CPUX_IRQ_MAPTO_CPUS_GRP(wakeup_root_irq);

	/* this is a group irq, we need to set group config, otherwise do nothing. */
	if (group_irq_num != FAIL)
		interrupt_set_group_config(group_irq_num, FALSE);

	return OK;
}

static int irq_wakesource_init(s32 irq_num, __pCBK_t init, void *init_parg, __pISR_t handler, void *parg)
{
	if (init != NULL)
		(*init)(init_parg);

	if (handler != NULL)
		install_isr(irq_num, handler, parg);

	interrupt_enable(irq_num);
	interrupt_set_mask(irq_num, FALSE);

	return OK;
}

static int irq_wakesource_exit(s32 irq_num, __pCBK_t exit, void *exit_parg, __pISR_t handler, void *parg)
{
	if (exit != NULL)
		(*exit)(exit_parg);

	if (handler != NULL)
		install_isr(irq_num, handler, parg);
	interrupt_set_mask(irq_num, TRUE);

	return OK;
}

s32 set_wakeup_src(struct message *pmessage)
{
	u32 wakeup_src_type;
	u32 wakeup_root_irq;
	u32 wakeup_secondary_irq;
	u32 wakeup_thrid_irq;
	u32 time_to_wakeup;

	s32 irq_no;
	u32 para[3];

	wakeup_src_type = (pmessage->paras[0] >> 30) & 0x3;
	wakeup_root_irq = pmessage->paras[0] & 0x3FF;
	wakeup_secondary_irq = (pmessage->paras[0] >> 10) & 0x3FF;
	wakeup_thrid_irq = (pmessage->paras[0] >> 20) & 0x3FF;

	LOG("wakeup_root_irq: %d\n", wakeup_root_irq);

	if (wakeup_src_type == 0x3) {
		time_to_wakeup = pmessage->paras[0] & 0x3fffffff;
		wakeup_timer.cycle = msec_to_ticks(time_to_wakeup);
	} else {
		irq_no = CPUX_IRQ_MAPTO_CPUS(wakeup_root_irq);
		if (irq_no == -1) {
			ERR("%s(%d) irq_no error, root_irq %x\n",
				__func__, __LINE__, wakeup_root_irq);
			return -1;
		}

		/**
		 * 1.we should clear wakeup_source here to avoid
		 * missing or remain of wakeup event.
		 * 2.FIXME: modify wakeup_source only in process context
		 */
		if (wakeup_source == wakeup_root_irq) {
			wakeup_source = NO_WAKESOURCE;
		}

		switch (irq_no) {
		default:
			{
				para[0] = wakeup_root_irq;
				para[1] = wakeup_secondary_irq;
				para[2] = wakeup_thrid_irq;
				irq_wakesource_init(irq_no, group_irq_init, &para[0],
						default_wakeup_handler, NULL);
				break;
			}
		}
	}

	return OK;
}


s32 clear_wakeup_src(struct message *pmessage)
{
	u32 wakeup_src_type;
	u32 wakeup_root_irq;
	u32 wakeup_secondary_irq;
	u32 wakeup_thrid_irq;
	u32 time_to_wakeup;

	s32 irq_no;
	u32 para[3];

	wakeup_src_type = (pmessage->paras[0] >> 30) & 0x3;
	wakeup_root_irq = pmessage->paras[0] & 0x3FF;
	wakeup_secondary_irq = (pmessage->paras[0] >> 10) & 0x3FF;
	wakeup_thrid_irq = (pmessage->paras[0] >> 20) & 0x3FF;

	if (wakeup_src_type == 0x3) {
		time_to_wakeup = pmessage->paras[0] & 0x3fffffff;
		wakeup_timer.cycle = msec_to_ticks(time_to_wakeup);
	} else {
		irq_no = CPUX_IRQ_MAPTO_CPUS(wakeup_root_irq);
		if (irq_no == -1) {
			ERR("%s(%d) irq_no error, root_irq %x\n",
				__func__, __LINE__, wakeup_root_irq);
			return -1;
		}
		switch (irq_no) {
		default:
			{
				para[0] = wakeup_root_irq;
				para[1] = wakeup_secondary_irq;
				para[2] = wakeup_thrid_irq;
				irq_wakesource_exit(irq_no, group_irq_exit, &para[0],
						NULL, NULL);
				break;
			}
		}
	}

	return OK;
}


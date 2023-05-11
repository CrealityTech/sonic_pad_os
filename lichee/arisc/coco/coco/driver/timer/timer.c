/*
 *********************************************************************************************************
 *                                                AR100 SYSTEM
 *                                     AR100 Software System Develop Kits
 *
 *
 *                                    (c) Copyright 2018-2024, Luojie China
 *                                             All Rights Reserved
 *
 * File    : timer.c
 * By      : LuoJie
 * Version : v1.0
 * Date    : 2018-04-12 10:58
 * Descript: .
 * Update  :auther      ver     notes
 *          Luojie      1.0     Create this file.
 *********************************************************************************************************
 */
#include "timer_i.h"

//the table of timers
static timer_device timers[TIMERC_TIMERS_NUMBER];
LIST_HEAD(timer_list);
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
HANDLE timer_request(__pISR_t phdle)
{
	u32             cpsr;
	timer_device   *ptimer = NULL;
	u32 timer_find = 0;

	cpsr = cpu_disable_int();

	list_for_each_entry(ptimer,&timer_list,tmr_list){
		if(ptimer->dev.dev_lock == 1) {
			ptimer->dev.dev_lock = 0;
			ptimer->dev.handler = phdle;
			timer_find = 1;
			break;
		}
	}

	if(timer_find == 0 )
		ptimer = NULL;

	if (ptimer == NULL)
	{
		//no freed timer now
		printk("no free timer now\n");
		cpu_enable_int(cpsr);
		return NULL;
	}

	install_isr(ptimer->dev.irq_no, ptimer->dev.handler, (void *)ptimer);

	cpu_enable_int(cpsr);

	return (HANDLE)ptimer;

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
	timer_device *ptimer = (timer_device *)htimer;

	ASSERT(ptimer != NULL);

	//set timer status as free
	ptimer->dev.dev_lock  = 1;
	ptimer->dev.handler= NULL;

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
s32 timer_start(HANDLE htimer, u32 period, u32 irq_enable)
{
	timer_device *ptimer = (timer_device *)htimer;

	ASSERT(ptimer != NULL);

	if (ptimer->dev.dev_lock)
		return -EACCES;

	//set timer period
	Timer_Set_InValue(ptimer->TIMERx,period);

	//clear timer pending
	Timer_ClearPending(ptimer->TIMERx);
	//enable timer interrupt
	if(irq_enable)
		Timer_IRQEnable(ptimer->TIMERx);
	else
		Timer_IRQDisable(ptimer->TIMERx);

	Timer_Start(ptimer->TIMERx);

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
	timer_device *ptimer = (timer_device *)htimer;

	ASSERT(ptimer != NULL);

	if (ptimer->dev.dev_lock)
		return -EACCES;

	//disable timer
	Timer_Stop(ptimer->TIMERx);

	//disable timer irq
	Timer_IRQDisable(ptimer->TIMERx);

	//clear timer pending
	Timer_ClearPending(ptimer->TIMERx);

	return OK;
}


s32 timer_hosc_onoff_cb(u32 message, void * parg)
{
	timer_device   *ptimer = NULL;

	parg = parg;
#ifdef  CFG_FPGA_PLATFORM
	return OK;
#else

	switch(message)
	{
		case CCU_HOSC_WILL_OFF_NOTIFY:
			{
				list_for_each_entry(ptimer,&timer_list,tmr_list){
					if(ptimer->dev.dev_lock != 1) {
						Timer_SetSrc(ptimer->TIMERx,0);
						Timer_Set_InValue(ptimer->TIMERx,Timer_Get_InValue(ptimer->TIMERx)/(24000/32));
					}
				}
				return OK;
			}
		case CCU_HOSC_ON_READY_NOTIFY:
			{
				//24m hosc power-on already,
				//timer swith source clock to 24m hosc.
				INF("24m hosc power-on ready notify\n");
				list_for_each_entry(ptimer,&timer_list,tmr_list){
					if(ptimer->dev.dev_lock != 1) {
						Timer_SetSrc(ptimer->TIMERx,1);
						Timer_Set_InValue(ptimer->TIMERx,Timer_Get_InValue(ptimer->TIMERx)*(24000/32));
					}
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

	return OK;
}

timer_device *query_timer(u32 timer_number)
{
	timer_device *ptimer;

	list_for_each_entry(ptimer,&timer_list,tmr_list){
		if(ptimer->timer_number == timer_number) {
			break;
		}
		else
			return NULL;
	}

	return ptimer;
}

timer_device *request_timer(u32 timer_number,__pISR_t phdle)
{
	timer_device *ptimer;

	ptimer = (timer_device *)timer_request(phdle);

	if(ptimer != NULL) {
		if(ptimer->timer_number == 99) {
			ptimer->timer_number = timer_number;
		}
		else {
			ptimer = NULL;
		}
	}
	return ptimer;
}


static s32 timer_suspend(arisc_device *dev)
{
	dev = dev;

	return OK;
}

static s32 timer_resume(arisc_device *dev)
{
	dev = dev;

	return OK;
}

static int timer_ioctl(u32 request,va_list args)
{
	__pISR_t phdle;
	u32 timer_id;
	HANDLE ptimer;
	u32 period = 0;
	u32 irq_enable = 0;

//	va_start(args, request);
	switch (request) {
		case REQUSET_TIMER:
			timer_id = (u32)va_arg(args, unsigned int);
			phdle = (__pISR_t)va_arg(args, unsigned int);
			return (u32)request_timer(timer_id,phdle);
		case RELEASE_TIMER:
			ptimer = (HANDLE)va_arg(args, unsigned int);
			timer_release(ptimer);
			break;
		case START_TIMER:
			ptimer = (HANDLE)va_arg(args, unsigned int);
			period = va_arg(args, unsigned int);
			irq_enable = va_arg(args, unsigned int);
			timer_start(ptimer,period,irq_enable);
			interrupt_enable(((timer_device*)(ptimer))->dev.irq_no);
			break;
		case STOP_TIMER:
			ptimer = (HANDLE)va_arg(args, unsigned int);
			timer_stop(ptimer);
			break;
		case TIMER_CLEARING_PENDING:
			ptimer = (HANDLE)va_arg(args, unsigned int);
			Timer_ClearPending(((timer_device*)(ptimer))->TIMERx);
			break;
		default:
			break;
	}
	return OK;
}


arisc_driver timer_dri = {
	.suspend = timer_suspend,
	.resume  = timer_resume,
	.iorequset = timer_ioctl,
};

const struct timer_ops arisc_timer_ops = {
	.request = timer_request,
	.release = timer_release,
	.start = timer_start,
	.stop = timer_stop,
};

static s32 timer_config(timer_device *tmr,u32 reg_base, u32 irq_no)
{
	TIM_InitConfig timers_config;

	tmr->config = &timers_config;
	tmr->dev.reg_base = reg_base;
	tmr->dev.dev_lock = 1;
	tmr->dev.irq_no = irq_no;
	tmr->dev.dri = &timer_dri;

	tmr->ops = &arisc_timer_ops;

	tmr->clkchangecb = timer_hosc_onoff_cb;

	Timer_RegInit(&tmr->TIMERx, tmr->dev.reg_base);

//	if(tmr->config == NULL)
	Timer_DefInit(tmr->config);

	Timer_Config(tmr->TIMERx, tmr->config);

//	tmr->timer_number = Timer_GetId((&tmr->TIMERx));
	//这里需要插入通知链。

	list_add_tail(&(tmr->dev.list),&(dev_list));
	list_add_tail(&(tmr->tmr_list),&(timer_list));
	tmr->dev.dev_lock = 1;

	return OK;
}


s32 timer_init(void)
{
	u32 i = 0;

	prcm_reset_clk(get_prcm_device(),TIMER_BUS_GATING_REG);

	INIT_LIST_HEAD(&timer_list);
	for(i = 0; i < TIMERC_TIMERS_NUMBER; i++) {
		timers[i].dev.hw_ver = 0x10000;
		timers[i].dev.dri = &timer_dri;
		timers[i].dev.dev_id = TIMER0_DEVICE_ID + i;
//		timers[i].config = &timers_config;
		timers[i].timer_number = 99;

		if (dev_match(&(timers[i]).dev,R_TMR_REG_BASE + TIMER_VERSION)) {
			timer_config(&(timers[i]),R_TMR0_REG_BASE + i * 0x10,INTC_R_TIMER0_IRQ + i);
		}
		else {
			timer_exit();
			return FAIL;
		}
	}

	ccu_24mhosc_reg_cb(timer_hosc_onoff_cb,NULL);

	return OK;
}




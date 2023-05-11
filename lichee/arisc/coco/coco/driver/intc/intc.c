/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                               interrupt manager
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : interrupt_manager.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-3
* Descript: the manager of interrupt.
* Update  : date                auther      ver     notes
*           2012-5-3 10:45:15   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "intc_i.h"
#include "task.h"

struct int_isr_node isr_table[IRQ_SOUCE_MAX];

intc_device sintc;
/*
*********************************************************************************************************
*                                       INIT INTERRUPT MANAGER
*
* Description:  initialize interrupt manager.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize interrupt manager succeeded, others if failed.
*********************************************************************************************************
*/
struct notifier *irq_status = NULL;
static u32 systick_irq;
extern s32 irq_monitor(u32 message, void *aux);

static int intc_ioctl(u32 request,va_list args)
{
	u32 value0 = 0;
	u32 value1 = 0;
	u32 value2 = 0;

	switch (request) {
		case INTC_INSTALL_ISR:
			value0 = va_arg(args, unsigned int);
			value1 = va_arg(args, unsigned int);
			value2 = va_arg(args, unsigned int);
			install_isr(value0,(__pISR_t)value1,(void *)value2);
			break;
		case INTC_ENABLE_IRQ:
			value0 = va_arg(args, unsigned int);
			interrupt_enable(value0);
			break;
		case INTC_DISBALE_IRQ:
			value0 = va_arg(args, unsigned int);
			interrupt_disable(value0);
			break;
		case INTC_SET_MASK:
			value0 = va_arg(args, unsigned int);
			value1 = va_arg(args, unsigned int);
			interrupt_set_mask(value0,value1);
			break;
		case INTC_SET_GRP_CONFIG:
			value0 = va_arg(args, unsigned int);
			value1 = va_arg(args, unsigned int);
			interrupt_set_group_config(value0, value1);
			break;
		default:
			break;
	}

	return OK;
}



arisc_driver intc_dri = {
	.iorequset = intc_ioctl,
};


s32 interrupt_init(void)
{
	s32 index;
	//initialize interrupt controller
	Intc_Reset();

	sintc.dev.reg_base = R_INTC_REG_BASE;
	sintc.dev.dev_lock = 1;
	sintc.dev.dev_id = INTC_DEVICE_ID;

	sintc.dev.dri = &intc_dri;
	//initialize ISR table
	for (index = 0; index < IRQ_SOUCE_MAX; index++)
	{
		isr_table[index].pisr = isr_default;
		isr_table[index].parg = NULL;
	}

	list_add_tail(&sintc.dev.list,&(dev_list));
	sintc.dev.dev_lock = 0;
	//interrupt manager initialize succeeded
	return OK;
}


s32 interrupt_notify_init(void)
{
	timer_device *systick;

	systick = query_timer(SYSTICK_TMRNUM);
	if(systick != NULL) {
		systick_irq = systick->dev.irq_no;
		notifier_insert(&irq_status,irq_monitor,NULL);
		return OK;
	}
	else
		return FAIL;
}
/*
*********************************************************************************************************
*                                       EXIT INTERRUPT MANAGER
*
* Description:  exit interrupt manager.
*
* Arguments  :  none.
*
* Returns    :  OK if exit interrupt manager succeeded, others if failed.
*********************************************************************************************************
*/
s32 interrupt_exit(void)
{
	Intc_Exit();

	return OK;
}

/*
*********************************************************************************************************
*                                           ENABLE INTERRUPT
*
* Description:  enable a specific interrupt.
*
* Arguments  :  intno : the number of interrupt which we want to enable.
*
* Returns    :  OK if enable interrupt succeeded, others if failed.
*********************************************************************************************************
*/
s32 interrupt_enable(u32 intno)
{
	return Intc_Enable_Interrupt(intno);
}

/*
*********************************************************************************************************
*                                           DISABLE INTERRUPT
*
* Description:  disable a specific interrupt.
*
* Arguments  :  intno : the number of interrupt which we want to disable.
*
* Returns    :  OK if disable interrupt succeeded, others if failed.
*********************************************************************************************************
*/
s32 interrupt_disable(u32 intno)
{
	return Intc_Disable_Interrupt(intno);
}

/*
*********************************************************************************************************
*                                           SET NMI TRIGGER
*
* Description:  set nmi trigger.
*
* Arguments  :  type : the trigger type.
*
* Returns    :  OK if set trigger type succeeded, others if failed.
*********************************************************************************************************
*/
void interrupt_set_nmi_trigger(u32 type)
{
	Intc_Set_NMI(type);
}

s32 interrupt_set_mask(u32 intno,u32 mask)
{
	return Intc_Set_Mask(intno,mask);
}

s32 interrupt_set_group_config(u32 grp_irq_num, u32 mask)
{
	return Intc_Set_Group_Config(grp_irq_num, mask);
}

s32 interrupt_set_priority(u32 intno, u32 pri)
{
	return Intc_Set_Priority(intno,pri);
}

s32 interrupt_set_forcing(u32 intno,u32 forcing)
{
	return Intc_Set_Forcing(intno,forcing);
}
/*
*********************************************************************************************************
*                                           INSTALL ISR
*
* Description:  install ISR for a specific interrupt.
*
* Arguments  :  intno   : the number of interrupt which we want to install ISR.
*       pisr    : the ISR which to been install.
*       parg    : the argument for the ISR.
*
* Returns    :  OK if install ISR succeeded, others if failed.
*
* Note       :  the ISR execute entironment : CPU disable interrupt response.
*********************************************************************************************************
*/
s32 install_isr(u32 intno, __pISR_t pisr, void *parg)
{
	//intno can't beyond then IRQ_SOURCE_MAX
	ASSERT(intno < IRQ_R_SOUCE_MAX);

	//default isr, install directly
	isr_table[intno].pisr = pisr;
	isr_table[intno].parg = parg;

	return OK;
}

/*
*********************************************************************************************************
*                                           UNINSTALL ISR
*
* Description:  uninstall ISR for a specific interrupt.
*
* Arguments  :  intno   : the number of interrupt which we want to uninstall ISR.
*               pisr    : the ISR which to been uninstall.
*
* Returns    :  OK if uninstall ISR succeeded, others if failed.
*********************************************************************************************************
*/
s32 uninstall_isr(u32 intno, __pISR_t pisr)
{
	//intno can't beyond then IRQ_SOURCE_MAX
	ASSERT(intno < IRQ_SOUCE_MAX);

	if (isr_table[intno].pisr == pisr)
	{
		//uninstall isr
		isr_table[intno].pisr = isr_default;
		isr_table[intno].parg = NULL;
	}
	else
	{
		//don't support shared interrupt now,
		//by sunny at 2012-5-3 11:20:28.
		ERR("ISR not installed!\n");
		return -EFAIL;
	}

	return OK;
}

/*
*********************************************************************************************************
*                                           INTERRUPT ENTRY
*
* Description:  the entry of CPU IRQ, mainly for CPU IRQ exception.
*
* Arguments  :  none.
*
* Returns    :  OK if process CPU IRQ succeeded, others if failed.
*********************************************************************************************************
*/
extern task_struct daemon_task;
s32 interrupt_entry(void)
{
	u32 intno = Intc_Get_Current_Interrupt();

	//intno can't beyond then IRQ_SOURCE_MAX
	ASSERT(intno < IRQ_R_SOUCE_MAX);
	//process interrupt by call isr,
	//not support shared intterrupt.
	(isr_table[intno].pisr)(isr_table[intno].parg,intno);

	if(intno != systick_irq) {
		//INF("irq_status %x\n",irq_status);
		//resume_task(&daemon_task);
		notifier_notify(&irq_status,0);
	}
	return OK;
}

s32 interrupt_query_pending(u32 intno)
{
	return Intc_Query_Single_Pending(intno);
}

void interrupt_clear_pending(u32 intno)
{
	Intc_Clear_Single_Pending(intno);
}

s32 isr_default(void *arg __attribute__ ((__unused__)),
	u32 intno __attribute__ ((__unused__)))
{
	ERR("irq [%x] enable before ISR install\n",
			Intc_Get_Current_Interrupt());

	return TRUE;
}

//the backup of enable and mask register
u32 intc_enable[3];
u32 intc_mask[3];

s32 interrupt_suspend(void)
{
	//backup registers
	Intc_Save_Enable(intc_enable);
	Intc_Save_Mask(intc_mask);

	Intc_Clear_Enable();
	Intc_Clear_Mask();

	return OK;
}

s32 interrupt_resume(void)
{
	//clear standby pendings
	Intc_Clear_Pending();

	//restore registers
	Intc_Restore_Enable(intc_enable);
	Intc_Restore_Mask(intc_mask);

	return OK;
}

/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                              interrupt  module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : intc.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-4-27
* Descript: interrupt controller public header.
* Update  : date                auther      ver     notes
*           2012-4-27 10:52:56  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __INTC_H__
#define __INTC_H__


#include "ibase.h"
#include "../driver.h"

typedef struct {
	arisc_device dev;
	arisc_driver *dri;
}intc_device;

#define		INTC_INSTALL_ISR	0x1
#define		INTC_ENABLE_IRQ		0x2
#define		INTC_DISBALE_IRQ	0x3
#define		INTC_SET_MASK		0x4
#define		INTC_SET_GRP_CONFIG	0x5





//------------------------------------------------------------------------------
// interrupt source
//------------------------------------------------------------------------------
#define	INTC_NMI_IRQ    	INTC_R_NMI_IRQ
#define	INTC_TIMER0_IRQ 	INTC_R_TIMER0_IRQ
#define	INTC_TIMER1_IRQ 	INTC_R_TIMER1_IRQ
#define	INTC_TIMER2_IRQ 	INTC_R_TIMER2_IRQ
#define	INTC_TIMER3_IRQ 	INTC_R_TIMER3_IRQ
#define	INTC_ALM0_IRQ   	INTC_R_ALM0_IRQ
#define	INTC_ALM1_IRQ   	INTC_R_ALM1_IRQ
#define	INTC_WDOG_IRQ   	INTC_R_WDOG_IRQ
#define	INTC_TWD_IRQ    	INTC_R_TWD_IRQ
#define	INTC_PL_EINT_IRQ	INTC_R_PL_EINT_IRQ
#define	INTC_UART_IRQ   	INTC_R_UART_IRQ
#define	INTC_TWI_IRQ    	INTC_R_TWI_IRQ
#define	INTC_RSB_IRQ    	INTC_R_RSB_IRQ
#define	INTC_CIR_IRQ    	INTC_R_CIR_IRQ
#define	INTC_OWC_IRQ    	INTC_R_OWC_IRQ
#define	INTC_PM_EINT_IRQ	INTC_R_PM_EINT_IRQ
#define	INTC_CPUIDLE	  	INTC_R_CPUIDLE
#define	INTC_GIC_OUT_IRQ  	INTC_R_GIC_OUT_IRQ
#define	INTC_SPINLOCK_IRQ 	INTC_R_SPINLOCK_IRQ
#define	INTC_M_BOX_IRQ    	INTC_R_M_BOX_IRQ
#define	INTC_DMA_IRQ      	INTC_R_DMA_IRQ
#define	INTC_GINT0_IRQ    	INTC_R_GINT0_IRQ
#define	INTC_GINT1_IRQ    	INTC_R_GINT1_IRQ
#define	INTC_GINT2_IRQ    	INTC_R_GINT2_IRQ
#define	INTC_GINT3_IRQ    	INTC_R_GINT3_IRQ
#define	INTC_GINT4_IRQ    	INTC_R_GINT4_IRQ
#define	INTC_GINT5_IRQ    	INTC_R_GINT5_IRQ
#define	INTC_GINT6_IRQ    	INTC_R_GINT6_IRQ
#define	INTC_GINT7_IRQ    	INTC_R_GINT7_IRQ
#define	INTC_GINT8_IRQ    	INTC_R_GINT8_IRQ
#define	INTC_GINT9_IRQ    	INTC_R_GINT9_IRQ
#define	INTC_GINT10_IRQ   	INTC_R_GINT10_IRQ
#define	INTC_GINT11_IRQ   	INTC_R_GINT11_IRQ
#define	INTC_GINT12_IRQ   	INTC_R_GINT12_IRQ
#define	INTC_GINT13_IRQ   	INTC_R_GINT13_IRQ
#define	INTC_GINT14_IRQ   	INTC_R_GINT14_IRQ
//------------------------------------------------------------------------------
//the max interrupt source number
//------------------------------------------------------------------------------
#define IRQ_SOUCE_MAX           IRQ_R_SOUCE_MAX


s32 interrupt_init(void);
s32 interrupt_notify_init(void);
s32 interrupt_exit(void);
s32 interrupt_enable(u32 intno);
s32 interrupt_disable(u32 intno);
void interrupt_set_nmi_trigger(u32 type);
s32 interrupt_set_mask(u32 intno,u32 mask);
s32 interrupt_set_group_config(u32 grp_irq_num, u32 mask);
s32 interrupt_set_priority(u32 intno, u32 pri);
s32 interrupt_set_forcing(u32 intno,u32 forcing);
s32 install_isr(u32 intno, __pISR_t pisr, void *parg);
s32 uninstall_isr(u32 intno, __pISR_t pisr);
s32 interrupt_entry(void);
s32 interrupt_query_pending(u32 intno);
void interrupt_clear_pending(u32 intno);
s32 isr_default(void *arg,u32 intno);
s32 interrupt_suspend(void);
s32 interrupt_resume(void);

#endif  //__INTC_H__


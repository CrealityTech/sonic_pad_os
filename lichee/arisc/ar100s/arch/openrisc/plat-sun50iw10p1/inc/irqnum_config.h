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

#ifndef __IRQNUM_CONFIG_H__
#define __IRQNUM_CONFIG_H__

/*
 * ------------------------------------------------------------------------------
 * r_intc interrupt source
 * ------------------------------------------------------------------------------
 */
#define INTC_R_NMI_IRQ          0
#define INTC_R_TIMER0_IRQ       1
#define INTC_R_TIMER1_IRQ       2
#define INTC_R_TIMER2_IRQ       3
#define INTC_R_TIMER3_IRQ       4
#define INTC_R_ALM0_IRQ         5
#define INTC_R_WDOG_IRQ         6
#define INTC_R_TWD_IRQ          7
#define INTC_R_PL_EINT_IRQ      9
#define INTC_R_UART_IRQ         10
#define INTC_R_TWI0_IRQ         11
#define INTC_R_TWI1_IRQ         12
#define INTC_R_PPU_IRQ          13
#define INTC_GIC_OUT_IRQ        14
#define INTC_USB0_IRQ           16
#define INTC_USB1_IRQ           17
#define INTC_SPINLOCK_IRQ       32
#define INTC_M_BOX_IRQ          33
#define INTC_DMA_IRQ            34
#define INTC_GINT0_IRQ          35
#define INTC_GINT1_IRQ          36
#define INTC_GINT2_IRQ          37
#define INTC_GINT3_IRQ          38
#define INTC_GINT4_IRQ          39
#define INTC_GINT5_IRQ          40
#define INTC_GINT6_IRQ          41
#define INTC_GINT7_IRQ          42
#define INTC_GINT8_IRQ          43
#define INTC_GINT9_IRQ          44
#define INTC_GINT10_IRQ         45
#define INTC_GINT11_IRQ         46
#define INTC_GINT12_IRQ         47
#define INTC_GINT13_IRQ         48
#define INTC_GINT14_IRQ         49

/*
 * ------------------------------------------------------------------------------
 * gic interrupt source
 * ------------------------------------------------------------------------------
 */
#define GIC_USB0_EHCI_IRQ  62
#define GIC_USB0_OHCI_IRQ  63
#define GIC_USB1_EHCI_IRQ  65
#define GIC_USB1_OHCI_IRQ  66
#define GIC_R_EXTERNAL_NMI_IRQ  135
#define GIC_R_ALARM0_IRQ        140
#define GIC_R_GPIOL_IRQ         143

/*
 *------------------------------------------------------------------------------
 * the max interrupt source number
 *------------------------------------------------------------------------------
 */
#define IRQ_SOUCE_MAX           37

#endif	/*__IRQNUM_CONFIG_H__*/

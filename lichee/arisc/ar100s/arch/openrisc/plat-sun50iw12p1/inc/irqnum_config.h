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
#define INTC_R_ALM0_IRQ         9
#define INTC_R_WDT0_IRQ         11
#define INTC_R_WDT1_IRQ         12
#define INTC_R_PPU_IRQ          13
#define INTC_R_GPIOL_NS_IRQ     14
#define INTC_R_GPIOL_S_IRQ      15
#define INTC_R_GPIOM_NS_IRQ     16
#define INTC_R_GPIOM_S_IRQ      17
#define INTC_R_USB_IRQ          18
#define INTC_UART0_IRQ          21
#define INTC_TWI0_IRQ           26
#define INTC_TWI1_IRQ           27
#define INTC_R_IRRX_IRQ         32
#define INTC_R_PWM_IRQ          36
#define INTC_R_TZMA_IRQ         37
#define INTC_GIC_C0_IRQ         38
#define INTC_AHBS_HREADY_IRQ    39
#define INTC_SPINLOCK_IRQ       64
#define INTC_MBOX_R_IRQ         65
#define INTC_MBOX_W0_IRQ        66
#define INTC_MBOX_W1_IRQ        67
#define INTC_DMAC_IRQ           68
#define INTC_GINT0_IRQ          69
#define INTC_GINT1_IRQ          70
#define INTC_GINT2_IRQ          71
#define INTC_GINT3_IRQ          72
#define INTC_GINT4_IRQ          73
#define INTC_GINT5_IRQ          74
#define INTC_GINT6_IRQ          75
#define INTC_GINT7_IRQ          76
#define INTC_GINT8_IRQ          77
#define INTC_GINT9_IRQ          78
#define INTC_GINT10_IRQ         79
#define INTC_GINt11_IRQ         80
#define INTC_GINt12_IRQ         81
#define INTC_GINT13_IRQ         82
#define INTC_GINT14_IRQ         83

/*
 * ------------------------------------------------------------------------------
 * gic interrupt source
 * ------------------------------------------------------------------------------
 */
#define GIC_USB0_EHCI_IRQ  62
#define GIC_USB0_OHCI_IRQ  63
#define GIC_USB1_EHCI_IRQ  65
#define GIC_USB1_OHCI_IRQ  66
#define GIC_R_EXTERNAL_NMI_IRQ  172
#define GIC_R_ALARM0_IRQ        179

/*
 *------------------------------------------------------------------------------
 * the max interrupt source number
 *------------------------------------------------------------------------------
 */
#define IRQ_SOUCE_MAX           44

#endif	/*__IRQNUM_CONFIG_H__*/

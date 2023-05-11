/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                              interrupt  module
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : intc_lib.h
* By      : luojie
* Version : v1.0
* Date    : 2018-4-10
* Descript: interrupt controller module.
* Update  : date                auther      ver     notes
*           2018-4-10           Luojie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __INTC_LIB_H__
#define __INTC_LIB_H__

#include "lib_inc.h"

//------------------------------------------------------------------------------
// interrupt source
//------------------------------------------------------------------------------
#define INTC_R_NMI_IRQ          0
#define INTC_R_TIMER0_IRQ       1
#define INTC_R_TIMER1_IRQ       2
#define INTC_R_TIMER2_IRQ       3
#define INTC_R_TIMER3_IRQ       4
#define INTC_R_ALM0_IRQ         5
#define INTC_R_ALM1_IRQ         6
#define INTC_R_WDOG_IRQ         7
#define INTC_R_TWD_IRQ          8
#define INTC_R_PL_EINT_IRQ      9
#define INTC_R_UART_IRQ         10
#define INTC_R_TWI_IRQ          11
#define INTC_R_RSB_IRQ          12
#define INTC_R_CIR_IRQ          13
#define INTC_R_PWC_IRQ          14
#define INTC_R_GIC_OUT_IRQ      15
#define INTC_R_CPUIDLE		16
#define INTC_R_VOLT_CTRL_IRQ	17
#define INTC_R_PWM_IRQ		18
#define INTC_R_SPINLOCK_IRQ	32
#define INTC_R_M_BOX_IRQ	33
#define INTC_R_DMA_IRQ		34
#define INTC_MAD_WAKE_IRQ	35
#define INTC_MAD_DATA_REQ_IRQ	36
#define INTC_USB2_0_DRD_DEV	37
#define INTC_USB2_0_DRD_EHCI	38
#define INTC_USB2_0_DRD_OHCI	39
#define INTC_USB2_0_HOST1_EHCI	40
#define INTC_USB2_0_HOST1_OHCI	41
#define INTC_GPIOB          42
#define INTC_GPIOE          43
#define INTC_GPIOF          44
#define INTC_GPIOG          45
#define INTC_GPIOH          46
#define INTC_GPADC          47
#define INTC_THERMAL		48
#define INTC_LRADC		49
//------------------------------------------------------------------------------
//the max interrupt source number
//------------------------------------------------------------------------------
#define IRQ_R_SOUCE_MAX           37
#define IRQ_ALL_SOUCE_MAX           18 + 120

#define ENABLE_REG_NUM 3
#define PENDING_REG_NUM 3
#define ENABLE_REG_NUM 3

// interrput controller registers Offset
typedef struct intc_regs
{
	//offset 0x00
	volatile u32 vector;
	volatile u32 base_addr;
	volatile u32 reserved0;
	volatile u32 control;

	//offset 0x10
	volatile u32 pending;
	volatile u32 pending1;
	volatile u32 pending2;
	volatile u32 reserved1[9];

	//offset 0x40
	volatile u32 enable;
	volatile u32 enable1;
	volatile u32 enable2;
	volatile u32 reserved2[1];

	//offset 0x50
	volatile u32 mask;
	volatile u32 mask1;
	volatile u32 mask2;
	volatile u32 reserved3[5];

	//offset 0x70
	volatile u32 fast_forcing;
	volatile u32 fast_forcing1;
	volatile u32 fast_forcing2;
	volatile u32 reserved4;

	//offset 0x80
	volatile u32 priority0;
	volatile u32 priority1;
	volatile u32 priority2;
	volatile u32 priority3;
	volatile u32 priority4;
	volatile u32 priority5;
	volatile u32 reserved5[10];

	//offset 0xc0
	volatile u32 group_config0;
	volatile u32 group_config1;
	volatile u32 group_config2;
	volatile u32 group_config3;
} intc_regs_t;


//local functions
s32 Intc_Reset(void);
void Intc_Clear_Enable(void);
void Intc_Clear_Mask(void);
void Intc_Mask_Mask(void);
void Intc_Clear_Pending(void);
u32 Intc_Query_Single_Pending(u32 intno);
void Intc_Clear_Single_Pending(u32 intno);
void Intc_Save_Enable(u32 *buf);
void Intc_Save_Mask(u32 *buf);
void Intc_Save_Pending(u32 *buf);

void Intc_Restore_Enable(u32 *buf);
void Intc_Restore_Mask(u32 *buf);
void Intc_Restore_Pending(u32 *buf);

s32 Intc_Exit(void);
s32 Intc_Set_Fiq_Triggermode(u32 triggermode);
s32 Intc_Enable_Interrupt(u32 intno);
s32 Intc_Disable_Interrupt(u32 intno);
u32 Intc_Get_Current_Interrupt(void);

u32 Intc_Set_Forcing(u32 intno, u32 forcing);
u32 Intc_Set_Priority(u32 intno, u32 pri);
u32 Intc_Set_Mask(u32 intno, u32 mask);
u32 Intc_Set_Group_Config(u32 grp_irq_num, u32 mask);
void Intc_Set_NMI(u32 type);

#endif  //__INTC_I_H__

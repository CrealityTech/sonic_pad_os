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

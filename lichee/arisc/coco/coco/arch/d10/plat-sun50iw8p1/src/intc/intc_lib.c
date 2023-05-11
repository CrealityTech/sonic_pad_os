/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                              interrupt  module
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : intc_lib.c
* By      : luojie
* Version : v1.0
* Date    : 2018-4-10
* Descript: interrupt controller module.
* Update  : date                auther      ver     notes
*           2018-4-10           Luojie      1.0     Create this file.
*********************************************************************************************************
*/

#include "lib_inc.h"

struct intc_regs *pintc_regs;

/*
*********************************************************************************************************
*                                           INTERRUPT INIT
*
* Description:  initialize interrupt.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
s32 Intc_Reset(void)
{
	pintc_regs = (struct intc_regs *)(R_INTC_REG_BASE);

	//initialize interrupt controller
	pintc_regs->enable  = 0x0;
	pintc_regs->mask    = 0x0;
	pintc_regs->pending = 0xffffffff;

	pintc_regs->enable1  = 0x0;
	pintc_regs->mask1    = 0x0;
	pintc_regs->pending1 = 0xffffffff;

	pintc_regs->enable2  = 0x0;
	pintc_regs->mask2    = 0x0;
	pintc_regs->pending2 = 0xffffffff;
	return OK;
}

void Intc_Clear_Enable(void)
{
	//disable all interrupt
	pintc_regs->enable  = 0;
	pintc_regs->enable1 = 0;
	pintc_regs->enable2 = 0;
}

void Intc_Clear_Mask(void)
{
	pintc_regs->mask    = 0;
	pintc_regs->mask1   = 0;
	pintc_regs->mask2   = 0;
}

void Intc_Mask_Mask(void)
{
	pintc_regs->mask    = 0xffffffff;
	pintc_regs->mask1   = 0xffffffff;
	pintc_regs->mask2   = 0xffffffff;
}

void Intc_Clear_Pending(void)
{
	pintc_regs->pending  = 0xffffffff;
	pintc_regs->pending1   = 0xffffffff;
	pintc_regs->pending2   = 0xffffffff;
}

u32 Intc_Query_Single_Pending(u32 intno)
{
	volatile u32 pending;

	if(intno <=31)
		pending = pintc_regs->pending & (1 << intno);
	else if(intno >31 && intno <= 63)
		pending = pintc_regs->pending1 & (1 << (intno - 32));
	else
		pending = pintc_regs->pending2 & (1 << (intno - 64));

	return pending;

}

void Intc_Clear_Single_Pending(u32 intno)
{
	if(intno <= 31)
		pintc_regs->pending = (1 << intno);
	else if(intno > 31 && intno <= 63)
		pintc_regs->pending1 = (1 << (intno - 32));
	else
		pintc_regs->pending2 = (1 << (intno - 64));
}

void Intc_Save_Enable(u32 *buf)
{
	u32 *p = buf;

	*p++ = pintc_regs->enable;
	*p++ = pintc_regs->enable1;
	*p = pintc_regs->enable2;
}

void Intc_Save_Mask(u32 *buf)
{
	u32 *p = buf;

	*p++ = pintc_regs->mask;
	*p++ = pintc_regs->mask1;
	*p = pintc_regs->mask2;
}

void Intc_Save_Pending(u32 *buf)
{
	u32 *p = buf;

	pintc_regs->pending = *p++;
	pintc_regs->pending1 = *p++;
	pintc_regs->pending2 = *p;
}


void Intc_Restore_Enable(u32 *buf)
{
	u32 *p = buf;

	pintc_regs->enable = *p++;
	pintc_regs->enable1 = *p++;
	pintc_regs->enable2 = *p;
}

void Intc_Restore_Mask(u32 *buf)
{
	u32 *p = buf;

	pintc_regs->mask = *p++;
	pintc_regs->mask1 = *p++;
	pintc_regs->mask2 = *p;
}

void Intc_Restore_Pending(u32 *buf)
{
	u32 *p = buf;

	*p++ = pintc_regs->pending;
	*p++ = pintc_regs->pending1;
	*p = pintc_regs->pending2;
}
/*
*********************************************************************************************************
*                                         INTERRUPT EXIT
*
* Description:  exit interrupt.
*
* Arguments  :  none.
*
* Returns    :  OK if exit succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
s32 Intc_Exit(void)
{
	pintc_regs = NULL;

	return OK;
}

/*
*********************************************************************************************************
*                                           ENABLE INTERRUPT
*
* Description:  enable a specific interrupt.
*
* Arguments  :  intno   : the source number of interrupt to which we want to enable.
*
* Returns    :  OK if enable interrupt succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
s32 Intc_Enable_Interrupt(u32 intno)
{
	//intno can't beyond then IRQ_SOURCE_MAX
	ASSERT(intno < IRQ_SOUCE_MAX);

	//NMI interrupt should clear before enable.
	//by sunny at 2012-6-12 19:30:22.
	if (intno == INTC_R_NMI_IRQ)
	{
		INF("clean NMI interrupt pending\n");
		pintc_regs->pending = (1 << intno);
	}

	//enable interrupt which number is intno
	if(intno <= 31)
		pintc_regs->enable |= (1 << intno);
	else if(intno > 31 && intno <=63)
		pintc_regs->enable1 |= (1 << (intno-32));
	else
		pintc_regs->enable2 |= (1 << (intno-64));

	INF("intno:%d interrupt enable\n", intno);

	return OK;
}

/*
*********************************************************************************************************
*                                           DISABLE INTERRUPT
*
* Description:  disable a specific interrupt.
*
* Arguments  :  intno  : the source number of interrupt which we want to disable.
*
* Returns    :  OK if disable interrupt succeeded, others if failed.
*
* Note       :
*********************************************************************************************************
*/
s32 Intc_Disable_Interrupt(u32 intno)
{
	//intno can't beyond then IRQ_SOURCE_MAX
	ASSERT(intno < IRQ_SOUCE_MAX);

	//enable interrupt which number is intno
	if(intno <= 31)
		pintc_regs->enable &= ~(1 << intno);
	else if(intno > 31 && intno <=63)
		pintc_regs->enable1 &= ~(1 << (intno-32));
	else
		pintc_regs->enable2 &= ~(1 << (intno-64));

	return OK;
}

/*
*********************************************************************************************************
*                                   GET CURRENT INTERRUPT
*
* Description: get the source number of current interrupt.
*
* Arguments  : none.
*
* Returns    : the source number of current interrupt.
*
* Note       :
*********************************************************************************************************
*/
u32 Intc_Get_Current_Interrupt(void)
{
	volatile u32 interrupt;

	interrupt = (u32)((pintc_regs->vector) >> 2);

	return interrupt;
}

u32 Intc_Set_Mask(u32 intno,u32 mask)
{
	//intno can't beyond then IRQ_SOURCE_MAX
	ASSERT(intno < IRQ_SOUCE_MAX);
	//enable interrupt which number is intno
	if(intno <= 31) {
		pintc_regs->mask &= ~(1 << intno);
		pintc_regs->mask |= (mask << intno);
	}
	else if(intno > 31 && intno <=63) {
		pintc_regs->mask1 &= ~(1 << (intno-32));
		pintc_regs->mask1 |= (mask << intno);
	}
	else {
		pintc_regs->mask2 &= ~(1 << (intno-64));
		pintc_regs->mask2 |= (mask << intno);
	}

	return OK;
}

u32 Intc_Set_Forcing(u32 intno,u32 forcing)
{
	//intno can't beyond then IRQ_SOURCE_MAX
	ASSERT(intno < IRQ_R_SOUCE_MAX);
	//enable interrupt which number is intno
	if(intno <= 31) {
		pintc_regs->fast_forcing &= ~(1 << intno);
		pintc_regs->fast_forcing |= ~(forcing << intno);
	}
	else if(intno > 31 && intno <=63) {
		pintc_regs->fast_forcing1 &= ~(1 << (intno-32));
		pintc_regs->fast_forcing1 |= ~(forcing << intno);
	}
	else {
		pintc_regs->fast_forcing2 &= ~(1 << (intno-64));
		pintc_regs->fast_forcing2 |= ~(forcing << intno);
	}

	return OK;
}

u32 Intc_Set_Priority(u32 intno,u32 pri)
{
	u32 i,j;
	u32 *base;
	u32 *reg;
	//intno can't beyond then IRQ_SOURCE_MAX
	ASSERT(intno < IRQ_R_SOUCE_MAX);

	base = (u32 *)(&(pintc_regs->priority0));
	i = intno / 16;
	j = intno % 16;

	reg = (u32 *)((u32)base + i);
	*reg &= ~(0x3 << j);
	*reg |= (pri << j);

	return OK;
}

void Intc_Set_NMI(u32 type)
{
	pintc_regs->control = type;

	Intc_Set_Mask(0,1);
}





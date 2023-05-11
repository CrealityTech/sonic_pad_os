/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                         clock control unit module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : mclk.c
* By      : Superm
* Version : v1.0
* Date    : 2016-7-13
* Descript: module clock management
* Update  : date                auther      ver     notes
*           2016-7-13 8:43:10   Superm     1.0     Create this file.
*********************************************************************************************************
*/

#include "ccu_i.h"

/*
*********************************************************************************************************
*                                    SET ON-OFF STATUS OF MODULE CLOCK
*
* Description:  set the on-off status of a specific module clock.
*
* Arguments  :  mclk    : the module clock ID which we want to set on-off status.
*               onoff   : the on-off status which we want to set, the detail please
*                         refer to the clock status of on-off.
*
* Returns    :  OK if set module clock on-off status succeeded, others if failed.
*********************************************************************************************************
*/
s32 ccu_set_mclk_onoff(u32 mclk, s32 onoff)
{
	u32 value;

	switch (mclk) {
	case CCU_MOD_CLK_R_TWI:
		{
			value = readl(R_TWI_BUS_GATE_RST_REG);
			value &= (~(1 << 0));
			value |= (onoff << 0);
			writel(value, R_TWI_BUS_GATE_RST_REG);
			return OK;
		}
	case CCU_MOD_CLK_R_UART:
		{
			value = readl(R_UART_BUS_GATE_RST_REG);
			value &= (~(1 << 0));
			value |= (onoff << 0);
			writel(value, R_UART_BUS_GATE_RST_REG);
			return OK;
		}
	case CCU_MOD_CLK_R_TIMER0_1:
		{
			value = readl(R_TIMER_BUS_GATE_RST_REG);
			value &= (~(1 << 0));
			value |= (onoff << 0);
			writel(value, R_TIMER_BUS_GATE_RST_REG);
			return OK;
		}
	case CCU_MOD_CLK_R_RTC:
		{
			value = readl(RTC_BUS_GATE_RST_REG);
			value &= (~(1 << 0));
			value |= (onoff << 0);
			writel(value, RTC_BUS_GATE_RST_REG);
			return OK;
		}
	case CCU_MOD_CLK_MSGBOX:
		{
			writel(((readl(CCU_MSGBOX_BGR_REG) & (~(0x1 << 0)))
				| (onoff << 0)), CCU_MSGBOX_BGR_REG);
			return OK;
		}
	default:
		{
			WRN("invaid module clock id (%d) when set divider\n", mclk);
			return -EINVAL;
		}
	}
	/* un-reached */
}


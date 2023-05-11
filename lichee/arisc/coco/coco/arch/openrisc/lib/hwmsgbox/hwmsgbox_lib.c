/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : hwmsgbox_lib.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:35
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "lib_inc.h"

void MSGBOX_SetTransmitUser(u32 queue, u32 user)
{
	u32 value;

	if(queue <= 3) {
		value = readl(HWMSGBOX_REG_BASE + MSGBOX_CTRL_REG0);
		value &= ~(0xf << (queue * 8 + 4));
		value |= (user << (queue * 8 + 4));
		writel(value,HWMSGBOX_REG_BASE + MSGBOX_CTRL_REG0);
	}
	else {
		value = readl(HWMSGBOX_REG_BASE + MSGBOX_CTRL_REG1);
		value &= ~(0xf << ((queue - 4) * 8 + 4));
		value |= (user << ((queue - 4) * 8 + 4));
		writel(value,HWMSGBOX_REG_BASE + MSGBOX_CTRL_REG1);
	}
}

void MSGBOX_SetReceptionUser(u32 queue, u32 user)
{
	u32 value;

	if(queue <= 3) {
		value = readl(HWMSGBOX_REG_BASE + MSGBOX_CTRL_REG0);
		value &= ~(0xf << (queue * 8));
		value |= (user << (queue * 8));
		writel(value,HWMSGBOX_REG_BASE + MSGBOX_CTRL_REG0);
	}
	else {
		value = readl(HWMSGBOX_REG_BASE + MSGBOX_CTRL_REG1);
		value &= ~(0xf << ((queue - 4) * 8));
		value |= (user << ((queue - 4) * 8));
		writel(value,HWMSGBOX_REG_BASE + MSGBOX_CTRL_REG1);
	}
}


s32 MSGBOX_SetTransmitIRQ(u32 queue,u32 status)
{
	u32 value;

	value = readl(HWMSGBOX_REG_BASE + MSGBOXU_IRQ_EN_REG);
	value &= ~(1 << (queue * 2 + 1));
	value |= (status << (queue * 2 + 1));
	writel(value,HWMSGBOX_REG_BASE + MSGBOXU_IRQ_EN_REG);

	return OK;
}

s32 MSGBOX_SetReceptionIRQ(u32 queue,u32 status)
{
	u32 value;

	value = readl(HWMSGBOX_REG_BASE + MSGBOXU_IRQ_EN_REG);
	value &= ~(1 << queue * 2);
	value |= (status << queue * 2);
	writel(value,HWMSGBOX_REG_BASE + MSGBOXU_IRQ_EN_REG);

	return OK;
}

u32 MSGBOX_GetIRQStatus(u32 queue, u32 dir)
{
	u32 value;

	value = readl(HWMSGBOX_REG_BASE + MSGBOXU_IRQ_STATUS_REG);

	return (value >> (queue * 2 + dir)) & 0x1;
}

void MSGBOX_ClearPending(u32 queue)
{
	u32 value;

	value = readl(HWMSGBOX_REG_BASE + MSGBOXU_IRQ_STATUS_REG);
	value &= ~(1 << queue * 2);
	value |= (1 << queue * 2);
	writel(value,HWMSGBOX_REG_BASE + MSGBOXU_IRQ_STATUS_REG);
}

void MSGBOX_ClearTranPending(u32 queue)
{
	u32 value;

	value = readl(HWMSGBOX_REG_BASE + MSGBOXU_IRQ_STATUS_REG);
	value &= ~(1 << (queue * 2 + 1));
	value |= (1 << (queue * 2 + 1));
	writel(value,HWMSGBOX_REG_BASE + MSGBOXU_IRQ_STATUS_REG);
}

u32 MSGBOX_GetFIFOStatus(u32 queue)
{
	return (readl(HWMSGBOX_REG_BASE + MSGBOXM_FIFO_STATUS_REG + queue * 4));
}


s32 MSGBOX_GetMsgStatus(u32 queue)
{
	return (readl(HWMSGBOX_REG_BASE + MSGBOXM_MSG_STATUS_REG + queue * 4));
}


void MSGBOX_WriteMsg(u32 queue, u32 data)
{
	writel(data,HWMSGBOX_REG_BASE + MSGBOXM_MSG_REG + queue * 4);
}

u32 MSGBOX_ReadMsg(u32 queue)
{
	return readl(HWMSGBOX_REG_BASE + MSGBOXM_MSG_REG + queue * 4);
}

void MSGBOX_Debug(u32 state)
{
	u32 value;

	value = readl(HWMSGBOX_REG_BASE + MSGBOX_DEBUG_REG);
	value |= (state << 0);
	value |= (state << (8 + 4));
	writel(value,HWMSGBOX_REG_BASE + MSGBOX_DEBUG_REG);
}





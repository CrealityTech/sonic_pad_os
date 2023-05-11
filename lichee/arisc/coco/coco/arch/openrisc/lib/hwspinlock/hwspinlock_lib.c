/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : hwspinlock_lib.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:35
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "lib_inc.h"

u32 Spinlock_LockReg(u32 lock_id)
{
	return HWSPINLOCK_REG_BASE + SPINLOCK_LOCK_BASE_REG + lock_id * 0x4;
}

u32 Spinlock_ReadLock(u32 reg)
{
	return readl(reg);
}

void Spinlock_WriteLock(u32 reg,u32 value)
{
	writel(value,reg);
}

u32 Spinlock_ReadLockNum(void)
{
	u32 num;

	num = (readl(HWSPINLOCK_REG_BASE + SPINLOCK_SYSTATUS_REG) >> 28) & 0x3;
	if(num == 0x0)
		num =256;
	else if(num == 1)
		num =32;
	else if(num == 2)
		num =64;
	else if(num == 3)
		num =128;

	return num;
}

u32 Spinlock_ReadStatus(void)
{
	return	(readl(HWSPINLOCK_REG_BASE + SPINLOCK_SYSTATUS_REG) >> 8) & 0x1;
}

void Spinlock_SetIrqEnable(u32 lock_id)
{
	u32 value ;

	value = readl(HWSPINLOCK_REG_BASE + SPINLOCK_IRQENABLE_REG);

	value |= (1 << lock_id);

	writel(value,HWSPINLOCK_REG_BASE + SPINLOCK_IRQENABLE_REG);
}

void Spinlock_SetIrqDisable(u32 lock_id)
{
	u32 value ;

	value = readl(HWSPINLOCK_REG_BASE + SPINLOCK_IRQENABLE_REG);

	value &= ~(1 << lock_id);

	writel(value,HWSPINLOCK_REG_BASE + SPINLOCK_IRQENABLE_REG);

}

u32 Spinlock_GetIrqEnable(u32 lock_id)
{
	u32 value ;

	value = readl(HWSPINLOCK_REG_BASE + SPINLOCK_IRQENABLE_REG);

	return (value >> lock_id) & 0x1;

}

u32 Spinlock_GetPending(u32 lock_id)
{
	u32 value ;

	value = readl(HWSPINLOCK_REG_BASE + SPINLOCK_IRQSTATUS_REG);

	return (value >> lock_id) & 0x1;
}

void Spinlock_ClearPending(u32 lock_id)
{
	u32 value ;

	value = readl(HWSPINLOCK_REG_BASE + SPINLOCK_IRQSTATUS_REG);

	value |= (1 << lock_id);

	writel(value,HWSPINLOCK_REG_BASE + SPINLOCK_IRQSTATUS_REG);
}



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
	return	(readl(HWSPINLOCK_REG_BASE + SPINLOCK_SYSTATUS_REG) >> 28) & 0x3;
}

u32 Spinlock_ReadStatus(void)
{
	return	(readl(HWSPINLOCK_REG_BASE + SPINLOCK_SYSTATUS_REG) >> 8) & 0x1;
}























/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : spinlock.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-07-16 09:43
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "includes.h"
#include "spinlock.h"
#include "hwspinlock.h"

u32 get_spinlock(u32 id)
{
	return ioctl(HWSPINLOCK_DEVICE_ID,REQUEST_SPINLOCK,id);
}

u32 spinlock_trylock_timeout(u32 lock,u32 time)
{
	return ioctl(HWSPINLOCK_DEVICE_ID,REQUEST_SPINLOCK,lock,time);
}

u32 spinlock_unlock(u32 lock)
{
	return ioctl(HWSPINLOCK_DEVICE_ID,REQUEST_SPINLOCK,lock);
}


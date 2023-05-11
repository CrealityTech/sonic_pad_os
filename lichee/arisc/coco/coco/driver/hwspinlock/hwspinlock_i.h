/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : hwspinlock_i.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:32
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __HWSPINLOKC_I_H__
#define __HWSPINLOCK_I_H__

#include "driver.h"
#include "ibase.h"


struct hwspinlock_device {
	arisc_device dev;
	u32 hwspinlock_num;
	u32 hwspinlock_count;
	u32 suspended;
	u32 ref[MAX_SPINLOCK_NUM];
	s32 (*handler)(void *parg, u32 intno);
	struct hwspinlock locklock[32];
};





#endif



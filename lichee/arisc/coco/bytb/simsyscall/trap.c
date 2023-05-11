/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : trap.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-07-16 09:44
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "includes.h"
#include "ioctl.h"
#include "bytbmsg.h"
#include "task_handle.h"
#include "hwmsgbox.h"
#include "msgmanager.h"
#include "syscall.h"

static volatile u32 simsyscall_parg[4];

s32 printf__(const char *format, ...)
{
	va_list  args;
	bvprintk fn = vprintk;
	s32 ret = 0;

	va_start(args, format);

	fn =(bvprintk)((u32)fn & 0x00ffffff);
	ret = (*fn)(0xff,format,args);

	va_end(args);

	return ret;
}

s32 bytd_debugger_printf(u32 level, const char *format, ...)
{
	va_list  args;
	bvprintk fn = vprintk;
	s32 ret = 0;

	va_start(args, format);

	fn = (bvprintk)((u32)fn & 0x00ffffff);
	ret = (*fn)(level, format, args);

	va_end(args);

	return ret;
}

s32 ioctl(u32 devid, u32 request, ...)
{
	va_list args;
	ioctl__ fn = __ioctl;
	s32 ret = 0;

	va_start(args, request);

	fn = (ioctl__)((u32)fn & 0x00ffffff);
	ret = (*fn)(devid,request, args);

	va_end(args);

	return ret;
}

s32 syscall(u32 type, u32 d0, u32 d1, u32 d2)
{
	struct hwspinlock *lock;
	u32 spinlock;

	simsyscall_parg[0] = type;
	simsyscall_parg[1] = d0;
	simsyscall_parg[2] = d1;
	simsyscall_parg[3] = d2;

	spinlock = ioctl(HWSPINLOCK_DEVICE_ID,REQUEST_SPINLOCK,(u32)SIMSYSCLL_SPINLOCK);
	lock = (struct hwspinlock *)spinlock;
	lock->data = (void *)&simsyscall_parg[0];
	ioctl(HWSPINLOCK_DEVICE_ID,SPINLOCK_TRYLOCK_TIMEOUT,(u32)spinlock,1000);
	ioctl(HWSPINLOCK_DEVICE_ID,SPINLOCK_UNLOCK,(u32)spinlock);
	ioctl(HWSPINLOCK_DEVICE_ID,RELEASE_SPINLOCK,(u32)SIMSYSCLL_SPINLOCK);

	return simsyscall_parg[1];
}

s32 create_task(u32 task_struct, u32 task_handle)
{
	u32 ret;

	ret = syscall(TASK_CREATE,task_struct,task_handle,0);

	return ret;
}

s32 wakeup_task(u32 task_struct)
{
	u32 ret;

	ret = syscall(TASK_WAKEUP,task_struct,0,0);

	return ret;
}


s32 sleep_task(u32 task_struct)
{
	u32 ret;

	ret = syscall(TASK_SLEEP,task_struct,0,0);

	return ret;
}

s32 inh_msg(u32 src, u32 dest)
{
	u32 ret;

	ret = syscall(TASK_SLEEP,src,dest,0);

	return ret;
}

s32 time_mdelay_irqdisable(u32 ms)
{
	u32 ret;

	ret = syscall(SYSTEM_MDELAY,ms,0,0);

	return ret;

}

u32 ms_to_ticks(u32 ms)
{
	u32 ret;

	ret = syscall(SYSTEM_MS2TICKS,ms,0,0);

	return ret;
}



/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : main.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-07-16 09:42
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "includes.h"
#include "device.h"
#include "ioctl.h"
#include "timer.h"
#include "task.h"
#include "bytbmsg.h"
#include "loopback.h"
#include "psci.h"
#include "pmu_service.h"


#ifdef CFG_BYTB_DEBUG
s32 bytb_test_timer(void *parg, u32 intno);
__TASK_INITIAL(bytb_test_task,DEFAULT_TASK_PRIORITY,0,DEFAULT_TASK_HANDLE);

s32 bytb_test_timer(void *parg, u32 intno)
{
	HANDLE timer = parg;
	intno = intno;

	printf("timer from dram\n");
	wakeup_task((u32)(&bytb_test_task));

	ioctl(TIMER2_DEVICE_ID,TIMER_CLEARING_PENDING,(u32)timer);
	return OK;
}

static s32 bytb_test_task_handle(task_struct *task)
{
	task = task;

	printf("%s(%d)\n",__func__,__LINE__);

	return OK;
}
#endif

void bytb_init(void)
{
#ifdef CFG_BYTB_DEBUG
	HANDLE timer;

	timer = (HANDLE)ioctl(TIMER2_DEVICE_ID,REQUSET_TIMER,2,(u32)bytb_test_timer);
	ioctl(TIMER2_DEVICE_ID,START_TIMER,(u32)timer,240000 * 300,1);
	create_task((u32)(&bytb_test_task),(u32)bytb_test_task_handle);
#endif

	loop_back_init();
	psci_platform_init();
	pmu_platform_init();

	return;
}


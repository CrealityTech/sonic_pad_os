/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : device.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:33
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "list.h"
#include "ibase.h"

extern struct list_head dev_list;

typedef struct device  arisc_device;

typedef struct {
	u32 hw_ver;
	int (*suspend)(struct device *dev);
	int (*resume)(struct device *dev);
	void *data;
	int (*iorequset)(u32 request,va_list argp);
//	struct device *dev;
}arisc_driver;



struct device {
	u32 hw_ver;
	u32 dev_id;
	u32 irq_no;
	u32 reg_base;
	u32 dev_lock;
	u32 pin_lock;
	u32 suspend_status;
	s32 (*handler)(void *parg, u32 intno);
	struct list_head list;
	arisc_driver *dri;
	void *data;
};



static inline s32 dev_match(arisc_device *dev, u32 reg)
{
	if(dev->hw_ver != readl(reg))
		return 0;
	else
		return 1;
}

#define CCMU_DEVICE_ID		1
#define PRCM_DEVICE_ID		2
#define INTC_DEVICE_ID		3

#define PINM_DEVICE_ID		11
#define PINL_DEVICE_ID		12

#define TIMER0_DEVICE_ID	21
#define TIMER1_DEVICE_ID	22
#define TIMER2_DEVICE_ID	23
#define TIMER3_DEVICE_ID	24

#define UART_DEVICE_ID		31
#define RSB_DEVICE_ID		32
#define PMU_DEVICE_ID		33


#define HWMSGBOX_DEVICE_ID		41
#define HWSPINLOCK_DEVICE_ID		42

#endif


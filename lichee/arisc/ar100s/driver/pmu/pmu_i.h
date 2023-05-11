/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                pmu module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : pmu_i.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-22
* Descript: power management unit module internal header.
* Update  : date                auther      ver     notes
*           2012-5-22 13:27:09  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __PMU_I_H__
#define __PMU_I_H__

#include "include.h"
#include "notifier.h"
#include "wakeup_source.h"

#define RTC_ALARM_INT_EN_REG (0xd0)
#define RTC_ALARM_INT_ST_REG (0xd1)

#define RSB_INVALID_RTSADDR (0)

/**
 * keep the struct word align
 * by superm at 2014-2-13 15:34:09
 */
typedef struct voltage_info {
	u16 devaddr;
	u16 regaddr;
	u16 min1_mV;
	u16 max1_mV;
	u16 step1_mV;
	u16 step1_num;
	u16 min2_mV;
	u16 max2_mV;
	u16 step2_mV;
	u16 step2_num;
	u32 mask;
} voltage_info_t;

#endif  /* __PMU_I_H__ */

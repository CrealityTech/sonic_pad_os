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

//#include "include.h"
//#include "../../prcm/ccu_i-sun8iw15.h"
#include "ibase.h"
#include "driver.h"
#include "wakeup_source.h"

//AW1736 registers list
#define AW1736_PWR_ON_SRC_STA   (0x00)
#define AW1736_PWR_OFF_SRC_STA  (0x01)
#define AW1736_IC_NO_REG        (0x03)
#define AW1736_DATA_BUFF1       (0x04)
#define AW1736_DATA_BUFF2       (0x05)
#define AW1736_DATA_BUFF3       (0x06)
#define AW1736_DATA_BUFF4       (0x07)
#define AW1736_OUTPUT_PWR_CTRL0 (0x10)
#define AW1736_OUTPUT_PWR_CTRL1 (0x11)
#define AW1736_OUTPUT_PWR_CTRL2 (0x12)
#define AW1736_DCDC1_VOL_CTRL   (0x13)
#define AW1736_DCDC2_VOL_CTRL   (0x14)
#define AW1736_DCDC3_VOL_CTRL   (0x15)
#define AW1736_DCDC4_VOL_CTRL   (0x16)
#define AW1736_DCDC5_VOL_CTRL   (0x17)
#define AW1736_DCDC6_VOL_CTRL   (0x18)
#define AW1736_ALDO1_VOL_CTRL   (0x19)
#define AW1736_DCDC_CTRL1       (0x1A)
#define AW1736_DCDC_CTRL2       (0x1B)
#define AW1736_DCDC_CTRL3       (0x1C)
#define AW1736_DCDC_FREQ_SET    (0x1D)
#define AW1736_OUT_MONITOR_CTL  (0x1E)
#define AW1736_IRQ_PWROK_VOFF   (0x1F)
#define AW1736_ALDO2_VOL_CTRL   (0x20)
#define AW1736_ALDO3_VOL_CTRL   (0x21)
#define AW1736_ALDO4_VOL_CTRL   (0x22)
#define AW1736_ALDO5_VOL_CTRL   (0x23)
#define AW1736_BLDO1_VOL_CTRL   (0x24)
#define AW1736_BLDO2_VOL_CTRL   (0x25)
#define AW1736_BLDO3_VOL_CTRL   (0x26)
#define AW1736_BLDO4_VOL_CTRL   (0x27)
#define AW1736_BLDO5_VOL_CTRL   (0x28)
#define AW1736_CLDO1_VOL_CTRL   (0x29)
#define AW1736_CLDO2_VOL_CTRL   (0x2A)
#define AW1736_CLDO3_VOL_CTRL   (0x2B)
#define AW1736_CLDO4_GPIO2_CTRL (0x2C)
#define AW1736_CLDO4_VOL_CTRL   (0x2D)
#define AW1736_CPUSLDO_VOL_CTRL (0x2E)
#define AW1736_WAKEUP_CTL_OCIRQ (0x31)
#define AW1736_PWR_DISABLE_DOWN (0x32)
#define AW1736_POK_SET          (0x36)
#define AW1736_INT_MODE_SELECT  (0x3E)
#define AW1736_IRQ_ENABLE1      (0x40)
#define AW1736_IRQ_ENABLE2      (0x41)
#define AW1736_IRQ_STATUS1      (0x48)
#define AW1736_IRQ_STATUS2      (0x49)
#define AW1736_INVALID_ADDR     (0x100)

#define RTC_ALARM_INT_EN_REG    (0xd0)
#define RTC_ALARM_INT_ST_REG    (0xd1)

//the AW1760 bmu registers list
#define AW1760_PWR_SRC_STA      (0x00)
#define AW1760_VBUS_TYPE        (0x01)
#define AW1760_BAT_STA          (0x02)
#define AW1760_IC_NO_REG        (0x03)
#define AW1760_BOOST_STATUS     (0x04)
#define AW1760_INPUT_LIMIT 	    (0x10)
#define AW1760_BOOST_CTL	    (0x12)
#define AW1760_BOOST_LIMIT	    (0x13)
#define AW1760_IRQ_ENABLE1      (0x40)
#define AW1760_IRQ_ENABLE2      (0x41)
#define AW1760_IRQ_ENABLE3      (0x42)
#define AW1760_IRQ_ENABLE4      (0x43)
#define AW1760_IRQ_ENABLE5      (0x44)
#define AW1760_IRQ_ENABLE6      (0x45)
#define AW1760_IRQ_STATUS1      (0x48)
#define AW1760_IRQ_STATUS2      (0x49)
#define AW1760_IRQ_STATUS3      (0x4a)
#define AW1760_IRQ_STATUS4      (0x4b)
#define AW1760_IRQ_STATUS5      (0x4c)
#define AW1760_IRQ_STATUS6      (0x4d)
#define AW1760_CHG_CUR_LIMIT    (0x8b)
#define AW1760_CHG_VOL_LIMIT    (0x8c)
#define AW1760_INVALID_ADDR     (0x100)


//keep the struct word align
//by superm at 2014-2-13 15:34:09
typedef struct voltage_info
{
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

//local functions
//s32 nmi_int_handler(void *parg);
//s32 pmu_clear_pendings(void);

#endif  //__PMU_I_H__

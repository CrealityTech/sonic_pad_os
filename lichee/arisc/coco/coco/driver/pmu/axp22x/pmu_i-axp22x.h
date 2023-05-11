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
#include "ibase.h"
#include "driver.h"
#include "notifier.h"
#include "head_para.h"
#include "wakeup_source.h"


#define RTC_ALARM_INT_EN_REG (0xd0)
#define RTC_ALARM_INT_ST_REG (0xd1)

//AW1669 registers list
#define AW1669_PWR_SRC_STA      (0x00)
#define AW1669_PWRM_CHGR_STA    (0x01)
#define AW1669_IC_NO_REG        (0x03)
#define AW1669_DATA_BUFF0       (0x04)
#define AW1669_OUTPUT_PWR_CTRL0 (0x10)
#define AW1669_OUTPUT_PWR_CTRL1 (0x12)
#define AW1669_ALDO123_OP_MODE  (0x13)
#define AW1669_DLDO1_VOL_CTRL   (0x15)
#define AW1669_DLDO2_VOL_CTRL   (0x16)
#define AW1669_DLDO3_VOL_CTRL   (0x17)
#define AW1669_DLDO4_VOL_CTRL   (0x18)
#define AW1669_ELDO1_VOL_CTRL   (0x19)
#define AW1669_ELDO2_VOL_CTRL   (0x1A)
#define AW1669_ELDO3_VOL_CTRL   (0x1B)
#define AW1669_DC5LDO_VOL_CTRL  (0x1C)
#define AW1669_DCDC1_VOL_CTRL   (0x21)
#define AW1669_DCDC2_VOL_CTRL   (0x22)
#define AW1669_DCDC3_VOL_CTRL   (0x23)
#define AW1669_DCDC4_VOL_CTRL   (0x24)
#define AW1669_DCDC5_VOL_CTRL   (0x25)
#define AW1669_DCDC23_DVM_CTRL  (0x27)
#define AW1669_ALDO1_VOL_CTRL   (0x28)
#define AW1669_ALDO2_VOL_CTRL   (0x29)
#define AW1669_ALDO3_VOL_CTRL   (0x2a)
#define AW1669_VBUS_CTRL        (0x30)
#define AW1669_PWR_WAKEUP_CTRL  (0x31)
#define AW1669_PWR_OFF_CTRL     (0x32)
#define AW1669_CHAR_CURR_CTRL   (0x33)
#define AW1669_POK_SET		(0x36)
#define AW1669_IRQ_ENABLE1      (0x40)
#define AW1669_IRQ_ENABLE2      (0x41)
#define AW1669_IRQ_ENABLE3      (0x42)
#define AW1669_IRQ_ENABLE4      (0x43)
#define AW1669_IRQ_ENABLE5      (0x44)
#define AW1669_IRQ_STATUS1      (0x48)
#define AW1669_IRQ_STATUS2      (0x49)
#define AW1669_IRQ_STATUS3      (0x4a)
#define AW1669_IRQ_STATUS4      (0x4b)
#define AW1669_IRQ_STATUS5      (0x4c)
#define AW1669_IC_TEMP_HIG      (0x56)
#define AW1669_IC_TEMP_LOW      (0x57)
#define AW1669_ADC_DATA_HIGH    (0x58)
#define AW1669_ADC_DATA_LOW     (0x59)
#define AW1669_CHAR_CURR_STA1   (0x7a)
#define AW1669_CHAR_CURR_STA2   (0x7b)
#define AW1669_BAT_QUANTITY     (0xb9)
#define AW1669_INVALID_ADDR     (0x100)


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

struct pmu_ops {
	//s32 (*read_block)(rsb_device *dev, rsb_paras_t *paras);
	//s32 (*write_block)(rsb_device *dev, rsb_paras_t *paras);
	//s32 (*set_rtsa)(rsb_device *dev, u32 saddr ,u32 rtsaddr);
	s32 (*set_voltage)(u32 type, u32 voltage);
	s32 (*get_voltage)(u32 type);
	s32 (*set_voltage_state)(u32 type, u32 state);
	s32 (*get_voltage_state)(u32 type);
	s32 (*reg_write)(u8 *devaddr, u8 *regaddr, u8 *data, u32 len);
	s32 (*reg_read)(u8 *devaddr, u8 *regaddr, u8 *data, u32 len);
	void (*reset)(void);
	void (*shutdown)(void);
};

typedef struct {
	arisc_device dev;
	arisc_driver *dri;
	const struct pmu_ops *ops;
	s32 pmu_lock;
	s32 (*clkchangecb)(u32 command, u32 freq);
}pmu_device;

//local functions
//s32 nmi_int_handler(void *parg);
//s32 pmu_clear_pendings(void);

#endif  //__PMU_I_H__

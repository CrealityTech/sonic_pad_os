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

#ifndef __PMU_I_AXP2101_H__
#define __PMU_I_AXP2101_H__

#include "ibase.h"
#include "driver.h"
#include "notifier.h"
#include "head_para.h"
#include "wakeup_source.h"


#define RTC_ALARM_INT_EN_REG (0xd0)
#define RTC_ALARM_INT_ST_REG (0xd1)

/* registers list */
#define AXP2101_COMM_STAT0      (0x00)
#define AXP2101_COMM_STAT1      (0x01)
#define AXP2101_CHIP_ID         (0x03)
#define AXP2101_DATA_BUFFER0    (0x04)
#define AXP2101_DATA_BUFFER1    (0x05)
#define AXP2101_DATA_BUFFER2    (0x06)
#define AXP2101_DATA_BUFFER3    (0x07)
#define AXP2101_COMM_FAULT      (0x08)
#define AXP2101_COMM_CFG        (0X10)
#define AXP2101_BATFET_CTRL     (0X12)
#define AXP2101_DIE_TEMP_CFG    (0X13)
#define AXP2101_VSYS_MIN        (0x14)
#define AXP2101_VINDPM_CFG      (0x15)
#define AXP2101_IIN_LIM         (0x16)
#define AXP2101_RESET_CFG       (0x17)
#define AXP2101_MODULE_EN       (0x18)
#define AXP2101_WATCHDOG_CFG    (0x19)
#define AXP2101_GAUGE_THLD      (0x1A)
#define AXP2101_GPIO12_CTRL     (0x1B)
#define AXP2101_GPIO34_CTRL     (0x1C)
#define AXP2101_BUS_MODE_SEL    (0x1D)
#define AXP2101_PWRON_STAT      (0x20)
#define AXP2101_PWROFF_STAT     (0x21)
#define AXP2101_PWROFF_EN       (0x22)
#define AXP2101_DCDC_PWROFF_EN  (0x23)
#define AXP2101_VOFF_THLD       (0x24)
#define AXP2101_PWR_TIME_CTRL   (0x25)
#define AXP2101_SLEEP_CFG       (0x26)
#define AXP2101_PONLEVEL        (0x27)
#define AXP2101_FAST_PWRON_CFG0 (0x28)
#define AXP2101_FAST_PWRON_CFG1 (0x29)
#define AXP2101_FAST_PWRON_CFG2 (0x2A)
#define AXP2101_FAST_PWRON_CFG3 (0x2B)
#define AXP2101_ADC_CH_EN0      (0x30)
#define AXP2101_ADC_CH_EN1      (0x31)
#define AXP2101_ADC_CH_EN2      (0x32)
#define AXP2101_ADC_CH_EN3      (0x33)
#define AXP2101_VBAT_H          (0x34)
#define AXP2101_VBAT_L          (0x35)
#define AXP2101_TS_H            (0x36)
#define AXP2101_TS_L            (0x37)
#define AXP2101_VBUS_H          (0x38)
#define AXP2101_VBUS_L          (0x39)
#define AXP2101_VSYS_H          (0x3A)
#define AXP2101_VSYS_L          (0x3B)
#define AXP2101_TDIE_H          (0x3C)
#define AXP2101_TDIE_L          (0x3D)
#define AXP2101_GPADC_H         (0x3E)
#define AXP2101_GPADC_L         (0x3F)
#define AXP2101_INTEN1          (0x40)
#define AXP2101_INTEN2          (0x41)
#define AXP2101_INTEN3          (0x42)
#define AXP2101_INTSTS1         (0x48)
#define AXP2101_INTSTS2         (0x49)
#define AXP2101_INTSTS3         (0x4A)
#define AXP2101_TS_CFG          (0x50)

#define AXP2101_TS_HYSHL2H      (0x52)
#define AXP2101_TS_HYSH21       (0x53)
#define AXP2101_VLTF_CHG        (0x54)
#define AXP2101_VHTF_CHG        (0x55)
#define AXP2101_VLTF_WORK       (0x56)
#define AXP2101_VHTF_WORK       (0x57)
#define AXP2101_JEITA_CFG       (0x58)
#define AXP2101_JEITA_CV_CFG    (0x59)
#define AXP2101_JEITA_COOL      (0x5A)
#define AXP2101_JEITA_WARM      (0x5B)
#define AXP2101_TS_CFG_DATA_H   (0x5C)
#define AXP2101_TS_CFG_DATA_L   (0x5D)
#define AXP2101_CHG_CFG         (0x60)
#define AXP2101_IPRECHG_CFG     (0x61)
#define AXP2101_ICC_CFG         (0x62)
#define AXP2101_ITERM_CFG       (0x63)
#define AXP2101_CHG_V_CFG       (0x64)
#define AXP2101_TREGU_THLD      (0x65)
#define AXP2101_CHG_FREQ        (0x66)
#define AXP2101_CHG_TMR_CFG     (0x67)
#define AXP2101_BAT_DET         (0x68)
#define AXP2101_CHGLED_CFG      (0x69)
#define AXP2101_BTN_CHG_CFG     (0x6A)
#define AXP2101_SW_TEST_CFG     (0x7B)
#define AXP2101_DCDC_CFG0       (0x80)
#define AXP2101_DCDC_CFG1       (0x81)
#define AXP2101_DCDC1_CFG       (0x82)
#define AXP2101_DCDC2_CFG       (0x83)
#define AXP2101_DCDC3_CFG       (0x84)
#define AXP2101_DCDC4_CFG       (0x85)
#define AXP2101_DCDC5_CFG       (0x86)
#define AXP2101_DCDC_OC_CFG     (0x87)
#define AXP2101_LDO_EN_CFG0     (0x90)
#define AXP2101_LDO_EN_CFG1     (0x91)
#define AXP2101_ALDO1_CFG       (0x92)
#define AXP2101_ALDO2_CFG       (0x93)
#define AXP2101_ALDO3_CFG       (0x94)
#define AXP2101_ALDO4_CFG       (0x95)
#define AXP2101_BLDO1_CFG       (0x96)
#define AXP2101_BLDO2_CFG       (0x97)
#define AXP2101_CPUSLD_CFG      (0x98)
#define AXP2101_DLDO1_CFG       (0x99)
#define AXP2101_DLDO2_CFG       (0x9A)
#define AXP2101_IP_VER          (0xA0)
#define AXP2101_BROM            (0xA1)
#define AXP2101_CONFIG          (0xA2)
#define AXP2101_TEMPERATURE     (0xA3)
#define AXP2101_SOC             (0xA4)
#define AXP2101_TIME2EMPTY_H    (0xA6)
#define AXP2101_TIME2EMPTY_L    (0xA7)
#define AXP2101_TIME2FULL_H     (0xA8)
#define AXP2101_TIME2FULL_L     (0xA9)
#define AXP2101_FW_VERSION      (0xAB)
#define AXP2101_INT0_FLAG       (0xAC)
#define AXP2101_COUTER_PERIOD   (0xAD)
#define AXP2101_BG_TRIM         (0xAE)
#define AXP2101_OSC_TRIM        (0xAF)
#define AXP2101_FG_ADDR         (0xB0)
#define AXP2101_FG_DATA_H       (0xB2)
#define AXP2101_FG_DATA_L       (0xB3)
#define AXP2101_RAM_MBIST       (0xB4)
#define AXP2101_ROM_TEST        (0xB5)
#define AXP2101_ROM_TEST_RT0    (0xB6)
#define AXP2101_ROM_TEST_RT1    (0xB7)
#define AXP2101_ROM_TEST_RT2    (0xB8)
#define AXP2101_ROM_TEST_RT3    (0xB9)
#define AXP2101_WD_CLR_DIS      (0xBA)
#define AXP2101_INVALID_ADDR    (0x100)


/**
 *keep the struct word align
 *by superm at 2014-2-13 15:34:09
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

struct pmu_ops {
	/* s32 (*read_block)(rsb_device *dev, rsb_paras_t *paras); */
	/* s32 (*write_block)(rsb_device *dev, rsb_paras_t *paras); */
	/* s32 (*set_rtsa)(rsb_device *dev, u32 saddr ,u32 rtsaddr); */
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
} pmu_device;

#endif  /* __PMU_I_AXP2101_H__ */

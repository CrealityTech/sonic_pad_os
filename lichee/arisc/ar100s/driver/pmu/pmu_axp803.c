/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                pmu module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : pmu.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-22
* Descript: power management unit.
* Update  : date                auther      ver     notes
*           2012-5-22 13:33:03  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "pmu_i.h"

#define SUNXI_CHARGING_FLAG (0x61)

/**
 * aw1660 voltages info table,
 * the index of table is voltage type.
 */
pmu_onoff_reg_bitmap_t aw1660_onoff_reg_bitmap[] = {
	//dev_addr               //reg_addr             //offset //state //dvm_en
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL0,    0,    1,    0},//AW1660_POWER_DCDC1
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL0,    1,    1,    0},//AW1660_POWER_DCDC2
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL0,    2,    1,    0},//AW1660_POWER_DCDC3
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL0,    3,    1,    0},//AW1660_POWER_DCDC4
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL0,    4,    1,    0},//AW1660_POWER_DCDC5
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL0,    5,    1,    0},//AW1660_POWER_DCDC6
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL0,    6,    1,    0},//AW1660_POWER_DCDC7
	{RSB_RTSADDR_AW1660,    AW1660_INVALID_ADDR,        6,    1,    0},//AW1660_POWER_RTCLDO
	{RSB_RTSADDR_AW1660,    AW1660_ALDO123_OP_MODE,     5,    0,    0},//AW1660_POWER_ALDO1
	{RSB_RTSADDR_AW1660,    AW1660_ALDO123_OP_MODE,     6,    0,    0},//AW1660_POWER_ALDO2
	{RSB_RTSADDR_AW1660,    AW1660_ALDO123_OP_MODE,     7,    0,    0},//AW1660_POWER_ALDO3
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL1,    3,    1,    0},//AW1660_POWER_DLDO1
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL1,    4,    0,    0},//AW1660_POWER_DLDO2
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL1,    5,    1,    0},//AW1660_POWER_DLDO3
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL1,    6,    1,    0},//AW1660_POWER_DLDO4
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL1,    0,    0,    0},//AW1660_POWER_ELDO1
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL1,    1,    0,    0},//AW1660_POWER_ELDO2
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL1,    2,    0,    0},//AW1660_POWER_ELDO3
	{RSB_RTSADDR_AW1660,    AW1660_ALDO123_OP_MODE,     2,    0,    0},//AW1660_POWER_FLDO1
	{RSB_RTSADDR_AW1660,    AW1660_ALDO123_OP_MODE,     3,    0,    0},//AW1660_POWER_FLDO2
	{RSB_RTSADDR_AW1660,    AW1660_ALDO123_OP_MODE,     4,    0,    0},//AW1660_POWER_FLDO3
	{RSB_RTSADDR_AW1660,    AW1660_INVALID_ADDR,        7,    0,    0},//AW1660_POWER_LDOIO0
	{RSB_RTSADDR_AW1660,    AW1660_INVALID_ADDR,        7,    0,    0},//AW1660_POWER_LDOIO1
	{RSB_RTSADDR_AW1660,    AW1660_OUTPUT_PWR_CTRL1,    7,    0,    0},//AW1660_POWER_DC1SW
	{RSB_INVALID_RTSADDR,   AW1660_INVALID_ADDR,        0,    0,    0},//POWER_ONOFF_MAX
};

/**
 * aw1660 specific function,
 * only called by pmu common function.
 */
static void aw1660_pmu_shutdown(void)
{
	u8 devaddr = RSB_RTSADDR_AW1660;
	u8 regaddr = AW1660_PWR_OFF_CTRL;
	u8 data;

	save_state_flag(REC_SHUTDOWN | 0x101);

	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	data |= 1 << 7;
	pmu_reg_write(&devaddr, &regaddr, &data, 1);

	LOG("poweroff system\n");
	while (1)
		;
}

static void aw1660_pmu_reset(void)
{
	u8 devaddr = RSB_RTSADDR_AW1660;
	u8 regaddr = AW1660_PWR_WAKEUP_CTRL;
	u8 data;

	save_state_flag(REC_SHUTDOWN | 0x201);

	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	data |= 1 << 6;
	pmu_reg_write(&devaddr, &regaddr, &data, 1);

	LOG("reset system\n");
	while (1)
		;
}

static void aw1660_pmu_charging_reset(void)
{
	u8 devaddr = RSB_RTSADDR_AW1660;
	u8 regaddr;
	u8 val;

	regaddr = AW1660_PWR_SRC_STA;
	pmu_reg_read(&devaddr, &regaddr, &val, 1);
	/* vbus presence */
	if ((val & 0x20) == 0x20) {
		regaddr = AW1660_PWRM_CHGR_STA;
		pmu_reg_read(&devaddr, &regaddr, &val, 1);
		/* only when battery presence we do reset instead of shutdown */
		if ((val & 0x20) == 0x20) {
			regaddr = AW1660_DATA_BUFF1;
			val = SUNXI_CHARGING_FLAG;
			pmu_reg_write(&devaddr, &regaddr, &val, 1);
			aw1660_pmu_reset();
		}
	}
}

static s32 aw1660_pmu_set_voltage_state(u32 type, u32 state)
{
	u8 devaddr;
	u8 regaddr;
	u8 data;
	u32 offset;

	devaddr = aw1660_onoff_reg_bitmap[type].devaddr;
	regaddr = aw1660_onoff_reg_bitmap[type].regaddr;
	offset  = aw1660_onoff_reg_bitmap[type].offset;
	aw1660_onoff_reg_bitmap[type].state = state;

	//read-modify-write
	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	data &= (~(1 << offset));
	data |= (state << offset);
	pmu_reg_write(&devaddr, &regaddr, &data, 1);

	if (state == POWER_VOL_ON) {
		//delay 1ms for open PMU output
		time_mdelay(1);
	}

	return OK;
}

pmu_ops_t pmu_axp803_ops = {
	.pmu_shutdown = aw1660_pmu_shutdown,
	.pmu_reset = aw1660_pmu_reset,
	.pmu_charging_reset = aw1660_pmu_charging_reset,
	.pmu_set_voltage_state = aw1660_pmu_set_voltage_state,
};


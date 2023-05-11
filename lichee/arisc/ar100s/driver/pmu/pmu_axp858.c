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

/**
 * aw1736 voltages info table,
 * the index of table is voltage type.
 */
pmu_onoff_reg_bitmap_t aw1736_onoff_reg_bitmap[] = {
	//dev_addr               //reg_addr             //offset //state //dvm_en
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL0,    0,    1,    0},//AW1736_POWER_DCDC1
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL0,    1,    1,    0},//AW1736_POWER_DCDC2
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL0,    2,    1,    0},//AW1736_POWER_DCDC3
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL0,    3,    1,    0},//AW1736_POWER_DCDC4
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL0,    4,    1,    0},//AW1736_POWER_DCDC5
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL0,    5,    1,    0},//AW1736_POWER_DCDC6
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL1,    0,    0,    0},//AW1736_POWER_ALDO1
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL1,    1,    0,    0},//AW1736_POWER_ALDO2
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL1,    2,    0,    0},//AW1736_POWER_ALDO3
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL1,    3,    0,    0},//AW1736_POWER_ALDO4
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL1,    4,    0,    0},//AW1736_POWER_ALDO5
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL1,    5,    0,    0},//AW1736_POWER_BLDO1
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL1,    6,    0,    0},//AW1736_POWER_BLDO2
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL1,    7,    0,    0},//AW1736_POWER_BLDO3
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL2,    0,    0,    0},//AW1736_POWER_BLDO4
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL2,    1,    0,    0},//AW1736_POWER_BLDO5
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL2,    2,    0,    0},//AW1736_POWER_CLDO1
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL2,    3,    0,    0},//AW1736_POWER_CLDO2
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL2,    4,    0,    0},//AW1736_POWER_CLDO3
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL2,    5,    0,    0},//AW1736_POWER_CLDO4
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL2,    6,    1,    0},//AW1736_POWER_CPUSLDO
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL2,    7,    0,    0},//AW1736_POWER_DC1SW
	{RSB_RTSADDR_AW1736,    AW1736_OUTPUT_PWR_CTRL0,    6,    1,    0},//AW1736_POWER_RTC
	{RSB_INVALID_RTSADDR,   AW1736_INVALID_ADDR,        0,    0,    0},//POWER_ONOFF_MAX
};

/**
 * aw1736 specific function,
 * only called by pmu common function.
 */
static void aw1736_pmu_shutdown(void)
{
	u8 devaddr = RSB_RTSADDR_AW1736;
	u8 regaddr = AW1736_PWR_DISABLE_DOWN;
	u8 data = 1 << 7;

	save_state_flag(REC_SHUTDOWN | 0x101);

	/* power off system, disable DCDC & LDO */
	pmu_reg_write(&devaddr, &regaddr, &data, 1);
	pmu_reg_read(&devaddr, &regaddr, &data, 1);

	LOG("poweroff system\n");
	while (1)
		;
}

static void aw1736_pmu_reset(void)
{
	u8 devaddr = RSB_RTSADDR_AW1736;
	u8 regaddr = AW1736_PWR_DISABLE_DOWN;
	u8 data;

	save_state_flag(REC_SHUTDOWN | 0x201);

	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	data |= 1 << 6;
	pmu_reg_write(&devaddr, &regaddr, &data, 1);

	LOG("reset system\n");
	while (1)
		;
}

static s32 aw1736_pmu_set_voltage_state(u32 type, u32 state)
{
	u8 devaddr;
	u8 regaddr;
	u8 data;
	u32 offset;

	devaddr = aw1736_onoff_reg_bitmap[type].devaddr;
	regaddr = aw1736_onoff_reg_bitmap[type].regaddr;
	offset  = aw1736_onoff_reg_bitmap[type].offset;
	aw1736_onoff_reg_bitmap[type].state = state;

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

pmu_ops_t pmu_axp858_ops = {
	.pmu_shutdown = aw1736_pmu_shutdown,
	.pmu_reset = aw1736_pmu_reset,
	.pmu_set_voltage_state = aw1736_pmu_set_voltage_state,
};


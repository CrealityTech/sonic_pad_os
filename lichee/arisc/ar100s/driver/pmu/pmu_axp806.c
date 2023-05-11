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
 * aw1657 voltages info table,
 * the index of table is voltage type.
 */
pmu_onoff_reg_bitmap_t aw1657_onoff_reg_bitmap[] = {
	//dev_addr               //reg_addr             //offset //state //dvm_en
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL1,    0,    1,    0},//AW1657_POWER_DCDCA
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL1,    1,    1,    0},//AW1657_POWER_DCDCB
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL1,    2,    1,    0},//AW1657_POWER_DCDCC
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL1,    3,    1,    0},//AW1657_POWER_DCDCD
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL1,    4,    1,    0},//AW1657_POWER_DCDCE
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL1,    5,    1,    0},//AW1657_POWER_ALDO1
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL1,    6,    1,    0},//AW1657_POWER_ALDO2
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL1,    7,    1,    0},//AW1657_POWER_ALDO3
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL2,    0,    1,    0},//AW1657_POWER_BLDO1
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL2,    1,    1,    0},//AW1657_POWER_BLDO2
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL2,    2,    1,    0},//AW1657_POWER_BLDO3
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL2,    3,    1,    0},//AW1657_POWER_BLDO4
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL2,    4,    1,    0},//AW1657_POWER_CLDO1
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL2,    5,    1,    0},//AW1657_POWER_CLDO2
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL2,    6,    1,    0},//AW1657_POWER_CLDO3
	{RSB_RTSADDR_AW1657,    AW1657_OUTPUT_PWR_CTRL2,    7,    1,    0},//AW1657_POWER_DCESW
	{RSB_INVALID_RTSADDR,   AW1657_INVALID_ADDR,        0,    0,    0},//POWER_ONOFF_MAX
};

/**
 * aw1657 specific function,
 * only called by pmu common function.
 */
static void aw1657_pmu_shutdown(void)
{
	u8 devaddr = RSB_RTSADDR_AW1657;
	u8 regaddr = AW1657_PWR_DOWN_SEQ;
	u8 data;

	save_state_flag(REC_SHUTDOWN | 0x101);

	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	data |= 1 << 7;
	pmu_reg_write(&devaddr, &regaddr, &data, 1);

	LOG("poweroff system\n");
	while (1)
		;
}

static void aw1657_pmu_reset(void)
{
	u8 devaddr = RSB_RTSADDR_AW1657;
	u8 regaddr = AW1657_PWR_DOWN_SEQ;
	u8 data;

	save_state_flag(REC_SHUTDOWN | 0x201);

	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	data |= 1 << 6;
	pmu_reg_write(&devaddr, &regaddr, &data, 1);

	LOG("reset system\n");
	while (1)
		;
}

static s32 aw1657_pmu_set_voltage_state(u32 type, u32 state)
{
	u8 devaddr;
	u8 regaddr;
	u8 data;
	u32 offset;

	devaddr = aw1657_onoff_reg_bitmap[type].devaddr;
	regaddr = aw1657_onoff_reg_bitmap[type].regaddr;
	offset  = aw1657_onoff_reg_bitmap[type].offset;
	aw1657_onoff_reg_bitmap[type].state = state;

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

pmu_ops_t pmu_axp806_ops = {
	.pmu_shutdown = aw1657_pmu_shutdown,
	.pmu_reset = aw1657_pmu_reset,
	.pmu_set_voltage_state = aw1657_pmu_set_voltage_state,
};


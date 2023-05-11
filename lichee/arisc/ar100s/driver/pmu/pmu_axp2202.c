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

#define SUNXI_CHARGING_FLAG_AXP2202 (0x08)

/**
 * axp2202 voltages info table,
 * the index of table is voltage type.
 */
pmu_onoff_reg_bitmap_t axp2202_onoff_reg_bitmap[] = {
	//dev_addr               //reg_addr             //offset //state //dvm_en
	{RSB_RTSADDR_AXP2202,    AXP2202_DCDC_CFG0,   	 0,    1,    0},//AWP2202_POWER__DCDC1
	{RSB_RTSADDR_AXP2202,    AXP2202_DCDC_CFG0,   	 1,    1,    0},//AXP2202_POWER_DCDC2
	{RSB_RTSADDR_AXP2202,    AXP2202_DCDC_CFG0,  	 2,    1,    0},//AXP2202_POWER_DCDC3
	{RSB_RTSADDR_AXP2202,    AXP2202_DCDC_CFG0,    	 3,    1,    0},//AXP2202_POWER_DCDC4
	{RSB_RTSADDR_AXP2202,    AXP2202_LDO_EN_CFG0,    0,    0,    0},//AXP2202_POWER_ALDO1
	{RSB_RTSADDR_AXP2202,    AXP2202_LDO_EN_CFG0,    1,    0,    0},//AXP2202_POWER_ALDO2
	{RSB_RTSADDR_AXP2202,    AXP2202_LDO_EN_CFG0,    2,    0,    0},//AXP2202_POWER_ALDO3
	{RSB_RTSADDR_AXP2202,    AXP2202_LDO_EN_CFG0,    3,    0,    0},//AXP2202_POWER_ALDO4
	{RSB_RTSADDR_AXP2202,    AXP2202_LDO_EN_CFG0,    4,    0,    0},//AXP2202_POWER_BLDO1
	{RSB_RTSADDR_AXP2202,    AXP2202_LDO_EN_CFG0,    5,    0,    0},//AXP2202_POWER_BLDO2
	{RSB_RTSADDR_AXP2202,    AXP2202_LDO_EN_CFG0,    6,    0,    0},//AXP2202_POWER_BLDO3
	{RSB_RTSADDR_AXP2202,    AXP2202_LDO_EN_CFG0,    7,    0,    0},//AXP2202_POWER_BLDO4
	{RSB_RTSADDR_AXP2202,    AXP2202_LDO_EN_CFG1,    0,    0,    0},//AXP2202_POWER_CLDO1
	{RSB_RTSADDR_AXP2202,    AXP2202_LDO_EN_CFG1,    1,    0,    0},//AXP2202_POWER_CLDO2
	{RSB_RTSADDR_AXP2202,    AXP2202_LDO_EN_CFG1,    2,    0,    0},//AXP2202_POWER_CLDO3
	{RSB_RTSADDR_AXP2202,    AXP2202_LDO_EN_CFG1,    3,    0,    0},//AXP2202_POWER_CLDO4
	{RSB_RTSADDR_AXP2202,    AXP2202_LDO_EN_CFG1,    4,    0,    0},//AXP2202_POWER_CPUSLDO
	{RSB_INVALID_RTSADDR,    AXP2202_INVALID_ADDR,   0,    0,    0},//POWER_ONOFF_MAX
};


/**
 * axp2202 specific function,
 * only called by pmu common function.
 */
static void axp2202_pmu_shutdown(void)
{
	u8 devaddr = RSB_RTSADDR_AXP2202;
	u8 regaddr = AXP2202_SOFT_PWROFF;
	u8 data;

	save_state_flag(REC_SHUTDOWN | 0x101);

	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	data |= 1 << 0;
	pmu_reg_write(&devaddr, &regaddr, &data, 1);

	LOG("poweroff system\n");
	while (1)
		;
}

static void axp2202_pmu_reset(void)
{
	u8 devaddr = RSB_RTSADDR_AXP2202;
	u8 regaddr = AXP2202_SOFT_PWROFF;
	u8 data;

	save_state_flag(REC_SHUTDOWN | 0x201);

	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	data |= 1 << 1;
	pmu_reg_write(&devaddr, &regaddr, &data, 1);

	LOG("reset system\n");
	while (1)
		;
}

static void axp2202_pmu_charging_reset(void)
{
	u8 devaddr = RSB_RTSADDR_AXP2202;
	u8 regaddr;
	u8 val;

	regaddr = AXP2202_COMM_STAT0;
	pmu_reg_read(&devaddr, &regaddr, &val, 1);
	/* vbus presence */
	if ((val & 0x20) == 0x20) {
		regaddr = AXP2202_COMM_STAT0;
		pmu_reg_read(&devaddr, &regaddr, &val, 1);
		/* only when battery presence we do reset instead of shutdown */
		if ((val & 0x08) == 0x08) {
			regaddr = AXP2202_BUFFER0;
			val = SUNXI_CHARGING_FLAG_AXP2202;
			pmu_reg_write(&devaddr, &regaddr, &val, 1);
			axp2202_pmu_reset();
		}
	}
}

static s32 axp2202_pmu_set_voltage_state(u32 type, u32 state)
{
	u8 devaddr;
	u8 regaddr;
	u8 data;
	u32 offset;

	devaddr = axp2202_onoff_reg_bitmap[type].devaddr;
	regaddr = axp2202_onoff_reg_bitmap[type].regaddr;
	offset  = axp2202_onoff_reg_bitmap[type].offset;
//	axp2202_onoff_reg_bitmap[type].state = state;

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

pmu_ops_t pmu_axp2202_ops = {
	.pmu_shutdown = axp2202_pmu_shutdown,
	.pmu_reset = axp2202_pmu_reset,
	.pmu_charging_reset = axp2202_pmu_charging_reset,
	.pmu_set_voltage_state = axp2202_pmu_set_voltage_state,
};


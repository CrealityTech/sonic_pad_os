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

#include "pmu_i-aw1736.h"

#ifdef CFG_PMU_USED
#ifdef CFG_PMU_AW1736_USED
u32 regulator_start, regulator_end;

extern struct ccu_reg_list *ccu_reg_addr;

#ifdef CFG_PMU_CHRCUR_CRTL_USED
static u32 pmu_discharge_ltf = 0; //pmu discharge ltf
static u32 pmu_discharge_htf = 0; //pmu discharge htf
static u32 bat_charge_temp = 0; /* the max temperature of PMU IC */
static u32 pmu_max_chg_cur = 0; /* the max charge current of battery */
#endif

#ifdef CFG_PMU_AW1736_USED
extern u32 rsb_audio_used;
#endif

//voltages info table,
//the index of table is voltage type.
voltage_info_t voltages_table[] =
{
	//dev_addr               //reg_addr             //min1_mV //max1_mV //step1_mV //step1_num //min2_mV //max2_mV  //step2_mV //step2_num //mask
	{RSB_RTSADDR_AW1736 ,    AW1736_DCDC1_VOL_CTRL  ,    1500,    3400,    100 ,    20  ,    3400,    3400,    0   ,    0 ,    0x1f},//AW1736_POWER_DCDC1
	{RSB_RTSADDR_AW1736 ,    AW1736_DCDC2_VOL_CTRL  ,    500 ,    1200,    10  ,    71  ,    1220,    1540,    20  ,    17,    0x7f},//AW1736_POWER_DCDC2
	{RSB_RTSADDR_AW1736 ,    AW1736_DCDC3_VOL_CTRL  ,    500 ,    1200,    10  ,    71  ,    1220,    1540,    20  ,    17,    0x7f},//AW1736_POWER_DCDC3
	{RSB_RTSADDR_AW1736 ,    AW1736_DCDC4_VOL_CTRL  ,    500 ,    1200,    10  ,    71  ,    1220,    1540,    20  ,    17,    0x7f},//AW1736_POWER_DCDC4
	{RSB_RTSADDR_AW1736 ,    AW1736_DCDC5_VOL_CTRL  ,    800 ,    1120,    10  ,    33  ,    1140,    1184,    20  ,    37,    0x7f},//AW1736_POWER_DCDC5
	{RSB_RTSADDR_AW1736 ,    AW1736_DCDC6_VOL_CTRL  ,    500 ,    3400,    100 ,    30  ,    3400,    3400,    0   ,    0 ,    0x1f},//AW1736_POWER_DCDC6
	{RSB_RTSADDR_AW1736 ,    AW1736_ALDO1_VOL_CTRL  ,    700 ,    3300,    100 ,    27  ,    3300,    3300,    0   ,    0 ,    0x1f},//AW1736_POWER_ALDO1
	{RSB_RTSADDR_AW1736 ,    AW1736_ALDO2_VOL_CTRL  ,    700 ,    3300,    100 ,    27  ,    3300,    3300,    0   ,    0 ,    0x1f},//AW1736_POWER_ALDO2
	{RSB_RTSADDR_AW1736 ,    AW1736_ALDO3_VOL_CTRL  ,    700 ,    3300,    100 ,    27  ,    3300,    3300,    0   ,    0 ,    0x1f},//AW1736_POWER_ALDO3
	{RSB_RTSADDR_AW1736 ,    AW1736_ALDO4_VOL_CTRL  ,    700 ,    3300,    100 ,    27  ,    3300,    3300,    0   ,    0 ,    0x1f},//AW1736_POWER_ALDO4
	{RSB_RTSADDR_AW1736 ,    AW1736_ALDO5_VOL_CTRL  ,    700 ,    3300,    100 ,    27  ,    3300,    3300,    0   ,    0 ,    0x1f},//AW1736_POWER_ALDO5
	{RSB_RTSADDR_AW1736 ,    AW1736_BLDO1_VOL_CTRL  ,    700 ,    3300,    100 ,    27  ,    3300,    3300,    0   ,    0 ,    0x1f},//AW1736_POWER_BLDO1
	{RSB_RTSADDR_AW1736 ,    AW1736_BLDO2_VOL_CTRL  ,    700 ,    3300,    100 ,    27  ,    3300,    3300,    0   ,    0 ,    0x1f},//AW1736_POWER_BLDO2
	{RSB_RTSADDR_AW1736 ,    AW1736_BLDO3_VOL_CTRL  ,    700 ,    3300,    100 ,    27  ,    3300,    3300,    0   ,    0 ,    0x1f},//AW1736_POWER_BLDO3
	{RSB_RTSADDR_AW1736 ,    AW1736_BLDO4_VOL_CTRL  ,    700 ,    3300,    100 ,    27  ,    3300,    3300,    0   ,    0 ,    0x1f},//AW1736_POWER_BLDO4
	{RSB_RTSADDR_AW1736 ,    AW1736_BLDO5_VOL_CTRL  ,    700 ,    3300,    100 ,    27  ,    3300,    3300,    0   ,    0 ,    0x1f},//AW1736_POWER_BLDO5
	{RSB_RTSADDR_AW1736 ,    AW1736_CLDO1_VOL_CTRL  ,    700 ,    3300,    100 ,    27  ,    3300,    3300,    0   ,    0 ,    0x1f},//AW1736_POWER_CLDO1
	{RSB_RTSADDR_AW1736 ,    AW1736_CLDO2_VOL_CTRL  ,    700 ,    3300,    100 ,    27  ,    3300,    3300,    0   ,    0 ,    0x1f},//AW1736_POWER_CLDO2
	{RSB_RTSADDR_AW1736 ,    AW1736_CLDO3_VOL_CTRL  ,    700 ,    3300,    100 ,    27  ,    3300,    3300,    0   ,    0 ,    0x1f},//AW1736_POWER_CLDO3
	{RSB_RTSADDR_AW1736 ,    AW1736_CLDO4_VOL_CTRL  ,    700 ,    4200,    100 ,    36  ,    4200,    4200,    0   ,    0 ,    0x3f},//AW1736_POWER_CLDO4
	{RSB_RTSADDR_AW1736 ,    AW1736_CPUSLDO_VOL_CTRL,    700 ,    1400,    50  ,    15  ,    1400,    1400,    0   ,    0 ,    0x0f},//AW1736_POWER_CPUSLDO
	{RSB_INVALID_RTSADDR,    AW1736_INVALID_ADDR    ,    0   ,    0   ,    0   ,    27  ,    0   ,    0   ,    0   ,    0 ,    0x00},//AW1736_POWER_MAX
};

pmu_onoff_reg_bitmap_t pmu_onoff_reg_bitmap[] =
{
	//dev_addr               //reg_addr             //offset //state //dvm_en
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL0,    0,    1,    0},//AW1736_POWER_DCDC1
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL0,    1,    1,    0},//AW1736_POWER_DCDC2
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL0,    2,    1,    0},//AW1736_POWER_DCDC3
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL0,    3,    1,    0},//AW1736_POWER_DCDC4
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL0,    4,    1,    0},//AW1736_POWER_DCDC5
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL0,    5,    1,    0},//AW1736_POWER_DCDC6
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL0,    6,    1,    0},//AW1736_POWER_RTC
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL1,    0,    0,    0},//AW1736_POWER_ALDO1
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL1,    1,    0,    0},//AW1736_POWER_ALDO2
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL1,    2,    0,    0},//AW1736_POWER_ALDO3
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL1,    3,    0,    0},//AW1736_POWER_ALDO4
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL1,    4,    0,    0},//AW1736_POWER_ALDO5
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL1,    5,    0,    0},//AW1736_POWER_BLDO1
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL1,    6,    0,    0},//AW1736_POWER_BLDO2
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL1,    7,    0,    0},//AW1736_POWER_BLDO3
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL2,    0,    0,    0},//AW1736_POWER_BLDO4
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL2,    1,    0,    0},//AW1736_POWER_BLDO5
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL2,    2,    0,    0},//AW1736_POWER_CLDO1
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL2,    3,    0,    0},//AW1736_POWER_CLDO2
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL2,    4,    0,    0},//AW1736_POWER_CLDO3
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL2,    5,    0,    0},//AW1736_POWER_CLDO4
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL2,    6,    1,    0},//AW1736_POWER_CPUSLDO
	{RSB_RTSADDR_AW1736 ,    AW1736_OUTPUT_PWR_CTRL2,    7,    0,    0},//AW1736_POWER_DC1SW
	{RSB_INVALID_RTSADDR,    AW1736_INVALID_ADDR    ,    0,    0,    0},//POWER_ONOFF_MAX
};

s32 pmu_init(void)
{
	u8 devaddr;
	u8 regaddr;
	u8 data;

	rsb_device *rsb_dev;
	rsb_dev = query_rsb();

	rsb_set_run_time_addr(rsb_dev, RSB_SADDR_AW1736, RSB_RTSADDR_AW1736);

	//check IC version number match or not
	devaddr = RSB_RTSADDR_AW1736;
	regaddr = AW1736_IC_NO_REG;
	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	printk("PMU AW1736 IC_NO_REG value = %x\n", data);

	//enable CPUA(DCDC2) and CPUB(DCDC3) DVM
	devaddr = RSB_RTSADDR_AW1736;
	regaddr = AW1736_DCDC_CTRL1;
	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	data |= 1 << 0;
	data |= 1 << 1;
	pmu_reg_write(&devaddr, &regaddr, &data, 1);
	pmu_onoff_reg_bitmap[AW1736_POWER_DCDC2].dvm_st = 1;
	pmu_onoff_reg_bitmap[AW1736_POWER_DCDC3].dvm_st = 1;

	regulator_start = AW1736_POWER_DCDC1;
	regulator_end = AW1736_POWER_MAX - 1;

	//FIXME:remove when linux ready, just test gpio exti
	//power on vcc-sensor, switch pmu ALDO5
	//pmu_set_voltage_state(AW1736_POWER_ALDO5, POWER_VOL_ON);

	//FIXME: NMI response by linux pmu driver
	//clear pendings
	//pmu_clear_pendings();

	//register pmu interrupt handler.
	//install_isr(INTC_R_NMI_IRQ, nmi_int_handler, NULL);

	//interrupt_clear_pending(INTC_R_NMI_IRQ);
	//pmu irq should be enable after linux pmu driver ready.
	//by superm at 2013-1-10 8:58:04.
	//interrupt_enable(INTC_R_NMI_IRQ);

	return OK;
}

s32 pmu_exit(void)
{
	return OK;
}

s32 bmu_init(void)
{
	u8 devaddr;
	u8 regaddr;
	u8 data;

	rsb_device *rsb_dev;
	rsb_dev = query_rsb();

	rsb_set_run_time_addr(rsb_dev, RSB_SADDR_BMU, RSB_RTSADDR_BMU);

	//check IC version number match or not
	devaddr = RSB_RTSADDR_AW1760;
	regaddr = AW1760_IC_NO_REG;
	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	printk("BMU AW1760 IC_NO_REG value = %x\n", data);

	return OK;
}

s32 bmu_exit(void)
{
	return OK;
}

void pmu_shutdown(void)
{
	u8 devaddr = RSB_RTSADDR_AW1736;
	u8 regaddr = AW1736_PWR_DISABLE_DOWN;
	u8 data = 1 << 7;

	//power off system
	//disable DCDC & LDO
	pmu_reg_write(&devaddr, &regaddr, &data, 1);
	pmu_reg_read(&devaddr, &regaddr, &data, 1);
}

void pmu_reset(void)
{
	writel(0x0, CPUX_TIMER_REG_BASE + 0xA0); //disable watchdog int
	writel(0x1, CPUX_TIMER_REG_BASE + 0xB4); //reset whole system
	writel((0x3 << 4), CPUX_TIMER_REG_BASE + 0xB8); //set reset after 3s
	writel((readl(CPUX_TIMER_REG_BASE + 0xB8) | 0x1), CPUX_TIMER_REG_BASE + 0xB8); //enable watchdog
	while (1);
}

u32 pmu_output_is_stable(u32 type)
{
	u8 devaddr = voltages_table[type].devaddr;
	u8 regaddr = voltages_table[type].regaddr;
	u8 data = 0;

	pmu_reg_read(&devaddr, &regaddr, &data, 1);

	if (data & (0x1 << 7))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
s32 pmu_set_boost(u32 state)
{
	u8 devaddr = RSB_RTSADDR_AW1760;
	u8 regaddr = AW1760_BOOST_CTL;
	u8 data = 0;
	u8 ret = OK;
	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	if(state) {
		data |= 0x80;
	} else {
		data &= 0x7f;
	}
	ret = pmu_reg_write(&devaddr, &regaddr, &data, 1);
	return ret;
}
//the voltage base on mV
s32 pmu_set_voltage(u32 type, u32 voltage)
{
	u8 devaddr;
	u8 regaddr;
	u8 step;
	u8 ret = OK;
	voltage_info_t *pvoltage_info;

	if (type >= AW1736_POWER_MAX)
	{
		//invalid power voltage type
		WRN("invalid power voltage type[%u]\n", type);
		return -EINVAL;
	}

	//get voltage info by type
	pvoltage_info = &(voltages_table[type]);

	//check voltage region valid or not
	//min1_mV < max1_mV <= min2_mV <= max2_mV
	if ((voltage < pvoltage_info->min1_mV) ||
		(voltage > pvoltage_info->max2_mV))
	{
		WRN("invalid voltage value[%u] to set, type[%u], the reasonable range[%u-%u]\n",
			voltage, type, pvoltage_info->min1_mV, pvoltage_info->max2_mV);
		return -EINVAL;
	}

	if (voltage <= pvoltage_info->max1_mV)
	{
		//calc step value
		step = ((voltage - pvoltage_info->min1_mV + pvoltage_info->step1_mV - 1) / (pvoltage_info->step1_mV)) & 0x7f;
	}
	else if ((voltage > pvoltage_info->max1_mV) && (voltage <= pvoltage_info->min2_mV))
	{
		step = pvoltage_info->step1_num & 0x7f;
	}
	else
	{
		//calc step value
		step = ((voltage - pvoltage_info->min2_mV + pvoltage_info->step2_mV - 1) / (pvoltage_info->step2_mV) + pvoltage_info->step1_num) & 0x7f;
	}

	//write voltage control register
	devaddr = pvoltage_info->devaddr;
	regaddr = pvoltage_info->regaddr;
	step &= pvoltage_info->mask;
	ret = pmu_reg_write(&devaddr, &regaddr, &step, 1);

#if 0  //AW1736 have no stable bit
	if (pmu_onoff_reg_bitmap[type].dvm_st)
	{
		//delay 100us for pmu hardware update
		time_udelay(100);
		while(pmu_output_is_stable(type) == FALSE);
	}
#endif

	return ret;
}

s32 pmu_get_voltage(u32 type)
{
	u8 devaddr;
	u8 regaddr;
	u8 step = 0;
	voltage_info_t *pvoltage_info;

	if (type >= AW1736_POWER_MAX)
	{
		//invalid power voltage type
		WRN("invalid power voltage type[%u]\n", type);
		return -EINVAL;
	}

	//get voltage info by type
	pvoltage_info = &(voltages_table[type]);

	//read voltage control register
	devaddr = pvoltage_info->devaddr;
	regaddr = pvoltage_info->regaddr;
	pmu_reg_read(&devaddr, &regaddr, &step, 1);

	step &= pvoltage_info->mask;

	if (step <= (pvoltage_info->step1_num - 1))
	{
		return ((step * (pvoltage_info->step1_mV)) + pvoltage_info->min1_mV);
	}
	else if (step == pvoltage_info->step1_num)
	{
		return pvoltage_info->min2_mV;
	}
	else
	{
		return (((step - pvoltage_info->step1_num) * (pvoltage_info->step2_mV)) + pvoltage_info->min2_mV);
	}
}

s32 pmu_set_voltage_state(u32 type, u32 state)
{
	u8 devaddr;
	u8 regaddr;
	u8 data;
	u32 offset;

	devaddr = pmu_onoff_reg_bitmap[type].devaddr;
	regaddr = pmu_onoff_reg_bitmap[type].regaddr;
	offset  = pmu_onoff_reg_bitmap[type].offset;
	pmu_onoff_reg_bitmap[type].state = state;

	//read-modify-write
	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	data &= ( ~(1 << offset));
	data |= (state << offset);
	pmu_reg_write(&devaddr, &regaddr, &data, 1);

	if (state == POWER_VOL_ON)
	{
		//delay 1ms for open PMU output
		time_mdelay(1);
	}

	return OK;
}

s32 pmu_get_voltage_state(u32 type)
{
	u8 devaddr;
	u8 regaddr;
	u8 data;
	u32 offset;

	devaddr = pmu_onoff_reg_bitmap[type].devaddr;
	regaddr = pmu_onoff_reg_bitmap[type].regaddr;
	offset  = pmu_onoff_reg_bitmap[type].offset;

	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	if (data & (1 << offset))
	{
		pmu_onoff_reg_bitmap[type].state = POWER_VOL_ON;
		return POWER_VOL_ON;
	}
	else
	{
		pmu_onoff_reg_bitmap[type].state = POWER_VOL_OFF;
		return POWER_VOL_OFF;
	}
}

s32 pmu_reg_write(u8 *devaddr, u8 *regaddr, u8 *data, u32 len)
{
	//ASSERT(len <= AXP_TRANS_BYTE_MAX);

#if CFG_RSB_USED
	u8 i;
	s32 result = OK;
	u32 data_temp = 0;

	rsb_device *rsb_dev;
	rsb_paras_t rsb_block_paras;
	rsb_dev = query_rsb();

	for (i = 0; i < len; i++)
	{
		data_temp = *(data + i);
		//result |= rsb_write(devaddr[i], regaddr[i], &data_temp, RSB_DATA_TYPE_BYTE);

		rsb_block_paras.len      = 1;
		rsb_block_paras.datatype = RSB_DATA_TYPE_BYTE;
		rsb_block_paras.devaddr  = devaddr[i];
		rsb_block_paras.regaddr  = &regaddr[i];
		rsb_block_paras.data     = &data_temp;

		result |= rsb_write_block(rsb_dev, &rsb_block_paras);
	}
	return result;
#else
	return twi_write(regaddr, data, 1);
#endif  //CFG_RSB_USED
}

s32 pmu_reg_read(u8 *devaddr, u8 *regaddr, u8 *data, u32 len)
{
	//ASSERT(len <= AXP_TRANS_BYTE_MAX);

#if CFG_RSB_USED
	//read register value by rsb
	u8 i;
	s32 result = OK;
	u32 data_temp = 0;

	rsb_device *rsb_dev;
	rsb_paras_t rsb_block_paras;
	rsb_dev = query_rsb();

	for (i = 0; i < len; i++)
	{
		//result |= rsb_read(devaddr[i], regaddr[i], &data_temp, RSB_DATA_TYPE_BYTE);
		rsb_block_paras.len      = 1;
		rsb_block_paras.datatype = RSB_DATA_TYPE_BYTE;
		rsb_block_paras.devaddr  = devaddr[i];
		rsb_block_paras.regaddr  = &regaddr[i];
		rsb_block_paras.data     = &data_temp;

		result |= rsb_read_block(rsb_dev, &rsb_block_paras);

		*(data + i) = (u8)(data_temp & 0xff);
	}
	return result;
#else
	return twi_read(regaddr, data, len);
#endif  //CFG_RSB_USED
}

int get_nmi_int_type(void)
{
	u32 devaddr;
	u8 regaddr[6];
	u32 enable[6];
	u32 status[6];
	u32 nmi_int_type = 0;
	u32 len = 6;
	u8 i;

	rsb_device *rsb_dev;
	rsb_paras_t rsb_block_paras;
	rsb_dev = query_rsb();

	//==================================check pmu interrupt===============================
	devaddr = RSB_RTSADDR_AW1760;
	//read enable regs
	regaddr[0] = AW1760_IRQ_ENABLE1;
	regaddr[1] = AW1760_IRQ_ENABLE2;
	regaddr[2] = AW1760_IRQ_ENABLE3;
	regaddr[3] = AW1760_IRQ_ENABLE4;
	regaddr[4] = AW1760_IRQ_ENABLE5;
	regaddr[5] = AW1760_IRQ_ENABLE6;

	for (i = 0; i < len; i++)
	{
		//rsb_read(devaddr, regaddr[i], &enable[i], RSB_DATA_TYPE_BYTE);

		rsb_block_paras.len      = 1;
		rsb_block_paras.datatype = RSB_DATA_TYPE_BYTE;
		rsb_block_paras.devaddr  = devaddr;
		rsb_block_paras.regaddr  = &regaddr[i];
		rsb_block_paras.data     = &enable[i];

		rsb_read_block(rsb_dev, &rsb_block_paras);

		//LOG("%x:%x\n", regaddr[i], enable[i]);
	}

	//read status regs
	regaddr[0] = AW1760_IRQ_STATUS1;
	regaddr[1] = AW1760_IRQ_STATUS2;
	regaddr[2] = AW1760_IRQ_STATUS3;
	regaddr[3] = AW1760_IRQ_STATUS4;
	regaddr[4] = AW1760_IRQ_STATUS5;
	regaddr[5] = AW1760_IRQ_STATUS6;
	for (i = 0; i < len; i++)
	{
		//rsb_read(devaddr, regaddr[i], &status[i], RSB_DATA_TYPE_BYTE);

		rsb_block_paras.len      = 1;
		rsb_block_paras.datatype = RSB_DATA_TYPE_BYTE;
		rsb_block_paras.devaddr  = devaddr;
		rsb_block_paras.regaddr  = &regaddr[i];
		rsb_block_paras.data     = &status[i];

		rsb_read_block(rsb_dev, &rsb_block_paras);

		//LOG("%x:%x\n", regaddr[i], status[i]);
	}

	i = 0;
	while ((i < len) && (!(enable[i] & status[i])))
	{
		i++;
	}

	if (i != len)
	{
		nmi_int_type |= NMI_INT_TYPE_PMU_OFFSET;
	}

	return nmi_int_type;
}


int nmi_int_handler(void *parg)
{
#if (defined KERNEL_USED)
	struct message *pmessage;
	u32 nmi_int_type = 0;
#else
	//u32 event;
#endif

	parg = parg;

	//pmu interrupt coming,
	//notify ac327 pmu driver to handler
	LOG("NMI irq coming...\n");

#if (defined KERNEL_USED)
	nmi_int_type = get_nmi_int_type();

	if (nmi_int_type == 0)
	{
		WRN("invalid nmi int type\n");
		//clear interrupt flag first
		interrupt_clear_pending(INTC_R_NMI_IRQ);

		return FALSE;
	}

	pmessage = message_allocate();
	if (pmessage == NULL)
	{
		WRN("allocate message for nmi int notify failed\n");
		return FALSE;
	}

	//initialize message
	pmessage->type     = AXP_INT_COMING_NOTIFY;
	pmessage->private  = 0;
	pmessage->attr     = 0;
	pmessage->state    = MESSAGE_INITIALIZED;
	pmessage->paras[0] = nmi_int_type;
	hwmsgbox_send_message(pmessage, SEND_MSG_TIMEOUT);

	//disable axp interrupt
	interrupt_disable(INTC_R_NMI_IRQ);
#else /* !(defined KERNEL_USED)*/
	/* NOTE: if pmu interrupt enabled,
	 * means allow power key to power on system
	 */

	/*
	pmu_query_event(&event);
	if (event & (CPUS_WAKEUP_DESCEND | CPUS_WAKEUP_ASCEND | \
	    CPUS_WAKEUP_SHORT_KEY | CPUS_WAKEUP_LONG_KEY)) {
		//LOG("power key:%x\n", event);
		write_rtc_domain_reg(START_OS_REG, 0x0f);
		//LOG("reset system now\n");
		pmu_reset_system();
	}
	*/
	pmu_clear_pendings();
#endif

	//clear interrupt flag first
	interrupt_clear_pending(INTC_R_NMI_IRQ);

	return TRUE;
}

#if 0
//backup pmu interrupt enable registers
static u8 int_enable_regs[6];

s32 pmu_standby_init(u32 event, u32 gpio_bitmap)
{
	u8  devaddr[6];
	u8  regaddr[6];
	u8  enable[6];
	u32 len = 6;

	devaddr[0] = RSB_RTSADDR_AW1736;
	devaddr[1] = RSB_RTSADDR_AW1736;

	//read pmu irq enable registers
	regaddr[0] = AW1736_IRQ_ENABLE1;
	regaddr[1] = AW1736_IRQ_ENABLE2;

	pmu_reg_read(devaddr, regaddr, enable, len);

	//backup enable registers
	memcpy(int_enable_regs, enable, len);

	//clear enable array firstly
	memset(enable, 0, len);

	//initialize pmu wakeup events
	if (event & CPUS_WAKEUP_BAT_TEMP)
	{
		//AW1736_IRQ_ENABLE1,
		//BIT0 : battery temperature over the warning level1 IRQ.
		//BIT1 : battery temperature over the warning level2 IRQ.
		enable[0] |= (0x1 << 0);
		enable[0] |= (0x1 << 1);
	}
	if (gpio_bitmap & CPUS_GPIO_AXP(1))
	{
		//AW1736_IRQ_ENABLE2,
		//BIT2 : GPIO1 input edge.
		enable[1] |= (0x1 << 2);
	}
	if (gpio_bitmap & CPUS_GPIO_AXP(2))
	{
		//AW1736_IRQ_ENABLE1,
		//BIT5 : GPIO2 input edge.
		enable[1] |= (0x1 << 5);
	}
	if (event & CPUS_WAKEUP_LONG_KEY)
	{
		//AW1736_IRQ_ENABLE1,
		//BIT0 : long key.
		enable[1] |= (0x1 << 0);
	}
	if (event & CPUS_WAKEUP_SHORT_KEY)
	{
		//AW1736_IRQ_ENABLE1,
		//BIT1 : short key.
		enable[1] |= (0x1 << 1);
	}
	if (event & CPUS_WAKEUP_DESCEND)
	{
		//AW1736_IRQ_ENABLE1,
		//BIT3 : POK negative.
		enable[1] |= (0x1 << 3);
	}
	if (event & CPUS_WAKEUP_ASCEND)
	{
		//AW1736_IRQ_ENABLE1,
		//BIT4 : POK postive.
		enable[1] |= (0x1 << 4);
	}

	//write pmu enable registers
	pmu_reg_write(devaddr, regaddr, enable, len);

	return OK;
}

s32 pmu_standby_exit(void)
{
	u8 devaddr[6];
	u8 regaddr[6];
	u32 len = 6;

	//restore pmu irq enable registers
	devaddr[0] = RSB_RTSADDR_AW1736;
	devaddr[1] = RSB_RTSADDR_AW1736;
	regaddr[0] = AW1736_IRQ_ENABLE1;
	regaddr[1] = AW1736_IRQ_ENABLE2;
	pmu_reg_write(devaddr, regaddr, int_enable_regs, len);

	return OK;
}
#endif
s32 pmu_reg_write_para(pmu_paras_t *para)
{
	return pmu_reg_write(para->devaddr, para->regaddr, para->data, para->len);
}

s32 pmu_reg_read_para(pmu_paras_t *para)
{
	return pmu_reg_read(para->devaddr, para->regaddr, para->data, para->len);
}

s32 pmu_query_event(u32 *event)
{
	event = event;
#if 0
	u8 devaddr[6];
	u8 regaddr[6];
	u8 status[6];
	u32 len = 6;

	//read pmu irq enable registers
	devaddr[0] = RSB_RTSADDR_AW1760;
	devaddr[1] = RSB_RTSADDR_AW1760;
	devaddr[2] = RSB_RTSADDR_AW1760;
	devaddr[3] = RSB_RTSADDR_AW1736;
	devaddr[4] = RSB_RTSADDR_AW1760;
	devaddr[5] = RSB_RTSADDR_AW1760;
	regaddr[0] = AW1760_IRQ_STATUS1;
	regaddr[1] = AW1760_IRQ_STATUS2;
	regaddr[2] = AW1760_IRQ_STATUS3;
	regaddr[3] = AW1736_IRQ_STATUS2;
	regaddr[4] = AW1760_IRQ_STATUS5;
	regaddr[5] = AW1760_IRQ_STATUS6;
	pmu_reg_read(devaddr, regaddr, status, len);
	//query pmu wakeup events
	INF("query pmu wakeup event\n");
	if ((status[0] & (0x1 << 7)) || (status[0] & (0x1 << 6)))
	{
		//AW1760_IRQ_STATUS1,
		//BIT7 : battery capacity drop to waring level2
		//BIT6 : battery capacity drop to waring level1.
		INF("low battery\n");
		*event |= CPUS_WAKEUP_LOWBATT;
	}
	if (status[1])
	{
		//AW1760_IRQ_STATUS2,
		//BIT7 : battery over temperature in charge mode.
		//BIT6 : quit battery over temperatur in charge mode.
		//BIT5 : battery under temperature in charge mode.
		//BIT4 : quit battery under temperature in charge mode.
		//BIT3 : battery over temperature in work mode.
		//BIT2 : quit battery over temperature in work mode.
		//BIT1 : battery under temperature in work mode.
		//BIT0 : quit battery under temperature in work mode.
		INF("bat temp\n");
		*event |= CPUS_WAKEUP_BAT_TEMP;
	}
	if (status[2] & (0x1 << 7) || status[2] & (0x1 << 6))
	{
		//AW1760_IRQ_STATUS3,
		//BIT7 : vbus insert.
		//BIT6 : vbus remove.
		INF("vbus\n");
		*event |= CPUS_WAKEUP_USB;
	}
	if (status[3] & (0x1 << 0))
	{
		//AW1736_IRQ_STATUS1,
		//BIT0 : long key.
		INF("long key\n");
		*event |= CPUS_WAKEUP_LONG_KEY;
	}
	if (status[3] & (0x1 << 1))
	{
		//AW1736_IRQ_STATUS1,
		//BIT1 : short key.
		INF("short key\n");
		*event |= CPUS_WAKEUP_SHORT_KEY;
	}
	if (status[3] & (0x1 << 3))
	{
		//AW1736_IRQ_STATUS1,
		//BIT5 : POK negative.
		INF("POK negative\n");
		*event |= CPUS_WAKEUP_DESCEND;
	}
	if (status[3] & (0x1 << 4))
	{
		//AW1736_IRQ_STATUS1,
		//BIT4 : POK postive.
		INF("POK postive\n");
		*event |= CPUS_WAKEUP_ASCEND;
	}
	if (status[4] & (0x1 << 6))
	{
		//AW1760_IRQ_STATUS5,
		//BIT4 : battery is full/charge done.
		INF("battery full\n");
		*event |= CPUS_WAKEUP_FULLBATT;
	}

	INF("event:%x\n", *event);

#endif

	return OK;
}

s32 pmu_clear_pendings(void)
{
	u32 i;
	u8  devaddr[5];
	u8  regaddr[5];
	u8  data[5];
	u32 len = 5;
	//clear AW1760 irq
	for (i = 0; i < len; i++)
	{
		devaddr[i] = RSB_RTSADDR_AW1760;
		regaddr[i] = 0x48 + i;
		data[i] = 0xff;
	}
	pmu_reg_write(devaddr, regaddr, data, len);
	//clear AW1760 irq
	len = 2;
	for (i = 0; i < len; i++)
	{
		devaddr[i] = RSB_RTSADDR_AW1736;
		regaddr[i] = 0x48 + i;
		data[i] = 0xff;
	}
	return pmu_reg_write(devaddr, regaddr, data, len);

}


//s32 pmu_get_chip_id(struct message *pmessage)
s32 pmu_get_chip_id(u32 *chip_id)
{
		u32 i = 0;
		u8 regaddr[23];
		u8 data_u8[23];
		u32 data_u32[23];
		u64 data_u64[2];
		u8 data[16];
		u32 singledata;
		u8 singleaddr;
		s32 result = OK;

		rsb_device *rsb_dev;
		rsb_paras_t rsb_block_paras;
		rsb_dev = query_rsb();

		//set pmu reg 0xff to 1
		singledata = 1;
		singleaddr = 0xff;
		//rsb_write(RSB_RTSADDR_AW1736, 0xff, &singledata, RSB_DATA_TYPE_BYTE);

		rsb_block_paras.len      = 1;
		rsb_block_paras.datatype = RSB_DATA_TYPE_BYTE;
		rsb_block_paras.devaddr  = RSB_RTSADDR_AW1736;
		rsb_block_paras.regaddr  = &singleaddr;
		rsb_block_paras.data     = &singledata;

		result |= rsb_write_block(rsb_dev, &rsb_block_paras);

		//init pmu regaddr
		for (i = 0; i < 23; i++)
		{
			regaddr[i] = 0x20 + i;
		}

		//read data one by one
		for (i = 0; i < 23; i++)
		{
			//result |= rsb_read(RSB_RTSADDR_AW1736, regaddr[i], &(data_u32[i]), RSB_DATA_TYPE_BYTE);

			rsb_block_paras.len      = 1;
			rsb_block_paras.datatype = RSB_DATA_TYPE_BYTE;
			rsb_block_paras.devaddr  = RSB_RTSADDR_AW1736;
			rsb_block_paras.regaddr  = &regaddr[i];
			rsb_block_paras.data     = &data_u32[i];

			result |= rsb_read_block(rsb_dev, &rsb_block_paras);

			data_u8[i] = (u8)data_u32[i];
		}

		/* convert 256bit to 128bit */
		data[15] = data_u8[0];
		data[14] = data_u8[1];
		data[13] = data_u8[2];
		data[12] = data_u8[3];
		data[11] = data_u8[4];

		/* |6|6|6|2|8|6|6|6|6|6|6| */
		data_u64[0] = (u64)(data_u8[5] & 0x3f);
		data_u64[0] |= (u64)(data_u8[6] & 0x3f) << 6;
		data_u64[0] |= (u64)(data_u8[7] & 0x3f) << 12;
		data_u64[0] |= (u64)(data_u8[8] & 0x3f) << 18;
		data_u64[0] |= (u64)(data_u8[9] & 0x3f) << 24;
		data_u64[0] |= (u64)(data_u8[10] & 0x3f) << 30;
		data_u64[0] |= (u64)(data_u8[11] & 0xff) << 36;
		data_u64[0] |= (u64)(data_u8[12] & 0x3) << 44;
		data_u64[0] |= (u64)(data_u8[16] & 0x3f) << 46;
		data_u64[0] |= (u64)(data_u8[17] & 0x3f) << 52;
		data_u64[0] |= (u64)(data_u8[18] & 0x3f) << 58;

		/* |6|6|6|6| */
		data_u64[1] = (u64)(data_u8[19] & 0x3f);
		data_u64[1] |= (u64)(data_u8[20] & 0x3f) << 6;
		data_u64[1] |= (u64)(data_u8[21] & 0x3f) << 12;
		data_u64[1] |= (u64)(data_u8[22] & 0x3f) << 18;

		data[10] = data_u64[0] & 0xff;
		data[9] = (data_u64[0] >> 8) & 0xff;
		data[8] = (data_u64[0] >> 16) & 0xff;
		data[7] = (data_u64[0] >> 24) & 0xff;
		data[6] = (data_u64[0] >> 32) & 0xff;
		data[5] = (data_u64[0] >> 40) & 0xff;
		data[4] = (data_u64[0] >> 48) & 0xff;
		data[3] = (data_u64[0] >> 56) & 0xff;

		data[2] = data_u64[1] & 0xff;
		data[1] = (data_u64[1] >> 8) & 0xff;
		data[0] = (data_u64[1] >> 16) & 0xff;

		//copy readout data to pmessage->paras
		for (i = 0; i < 4; i++)
		{
			/* |paras[0]    |paras[1]    |paras[2]     |paras[3]      |
			 * |chip_id[0~3]|chip_id[4~7]|chip_id[8~11]|chip_id[12~15]|
			 */
			//pmessage->paras[0] |= (data[i] << (i * 8));
			//pmessage->paras[1] |= (data[4 + i] << (i * 8));
			//pmessage->paras[2] |= (data[8 + i] << (i * 8));
			//pmessage->paras[3] |= (data[12 + i] << (i * 8));

			chip_id[0] |= (data[i] << (i * 8));
			chip_id[1] |= (data[4 + i] << (i * 8));
			chip_id[2] |= (data[8 + i] << (i * 8));
			chip_id[3] |= (data[12 + i] << (i * 8));

		}

		//set pmu reg 0xff to 0
		singledata = 0;
		singleaddr = 0xff;
		//rsb_write(RSB_RTSADDR_AW1736, 0xff, &singledata, RSB_DATA_TYPE_BYTE);

		rsb_block_paras.len      = 1;
		rsb_block_paras.datatype = RSB_DATA_TYPE_BYTE;
		rsb_block_paras.devaddr  = RSB_RTSADDR_AW1736;
		rsb_block_paras.regaddr  = &singleaddr;
		rsb_block_paras.data     = &singledata;

		rsb_write_block(rsb_dev, &rsb_block_paras);

		return result;
}

//s32 pmu_set_paras(struct message *pmessage)
s32 pmu_set_paras(u32 *paras)
{
	paras = paras;

#ifdef CFG_PMU_CHRCUR_CRTL_USED
	//pmu_discharge_ltf = pmessage->paras[0] * 10;
	//pmu_discharge_htf = pmessage->paras[1] * 10;

	pmu_discharge_ltf = paras[0] * 10;
	pmu_discharge_htf = paras[1] * 10;

	//LOG("cfg pmu temp safe range:0x%x ~ 0x%x\n", pmu_discharge_ltf, pmu_discharge_htf);
#endif

	return OK;
}

//s32 set_pwr_tree(struct message *pmessage)
s32 set_pwr_tree(u32 *paras)
{
	//memcpy((void *)arisc_para.power_regu_tree, (const void *)pmessage->paras, sizeof(arisc_para.power_regu_tree));

//	hexdump("tree", (char *)arisc_para.power_regu_tree, sizeof(arisc_para.power_regu_tree));

	paras = paras;

	return OK;
}

#ifdef CFG_PMU_CHRCUR_CRTL_USED
void pmu_contrl_batchrcur(void)
{

}
#endif

#ifdef SUSPEND_POWER_CHECK
s32 pmu_get_batconsum(void)
{

}

u32 pmu_get_powerstate(u32 power_reg)
{

}
#endif

void pmu_sysconfig_cfg(void)
{
	//LOG("pwkey_used:%d\n", arisc_para.start_os.pmukey_used);
	//if (arisc_para.start_os.pmukey_used) { /* physical key used */
	//	interrupt_enable(INTC_R_NMI_IRQ);
	//}
}

void pmu_reset_system(void)
{
	u8 devaddr;
	u8 regaddr;
	u8 data;

	devaddr = RSB_RTSADDR_AW1736;
	regaddr = AW1736_PWR_DISABLE_DOWN;

	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	//LOG("pmu reset reg:%x\n", data);
	data |= 1U << 6;
	pmu_reg_write(&devaddr, &regaddr, &data, 1);
	LOG("reset system\n");
	while(1);
}

void pmu_poweroff_system(void)
{
	u8 devaddr;
	u8 regaddr;
	u8 data;

	devaddr = RSB_RTSADDR_AW1736;
	regaddr = AW1736_PWR_DISABLE_DOWN;

	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	//LOG("pmu pwroff reg:%x\n", data);
	data |= 1U << 7;
	pmu_reg_write(&devaddr, &regaddr, &data, 1);
	LOG("poweroff system\n");
	while(1);
}

/* set power key time, support 0~3
 * 0:128mS, 1:1S, 2:2S, 3:3S
 */
s32 pmu_set_pok_time(int time)
{
	u8 devaddr;
	u8 regaddr;
	u8 data;

	devaddr = RSB_RTSADDR_AW1736;
	regaddr = AW1736_POK_SET;

	if (time > 4)
		return -EINVAL;

	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	//LOG("pmu pwrkey reg:%x\n", data);
	data &= ~(3U << 6);
	data |= time << 6;
	pmu_reg_write(&devaddr, &regaddr, &data, 1);

	return time;
}

void pmu_set_lowpcons(void)
{

	u8 devaddr;
	u8 regaddr;
	u8 data;

	//LOG("pmu enter low pow cons\n");
	//hwspinlock_super_standby_init();

	/* set pll_audio&pll_periph&other plls power off gating valid */
	//ccu_set_poweroff_gating_state(PWRCTL_VDD_CPUS, CCU_POWEROFF_GATING_VALID);
	//ccu_set_poweroff_gating_state(PWRCTL_VDD_AVCC_A, CCU_POWEROFF_GATING_VALID);

	// need to be supplied
	time_mdelay(1);

	/* set VDD_SYS reset as assert state */
	//ccu_set_mclk_reset(CCU_MOD_CLK_VDD_SYS, CCU_CLK_RESET);
	prcm_reset_clk_mask(get_prcm_device(),VDD_SYS_PWOER_RESET_REG);

	/* bit define, see standby.h */
	devaddr = RSB_RTSADDR_AW1736;
	regaddr = AW1736_OUTPUT_PWR_CTRL2;
	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	//LOG("%x ", data);
	data = 0xc0; //DC1SW and cpus not power down
	pmu_reg_write(&devaddr, &regaddr, &data, 1);
	//LOG("%x reg12 poweroff\n", data);

	devaddr = RSB_RTSADDR_AW1736;
	regaddr = AW1736_OUTPUT_PWR_CTRL1;
	pmu_reg_read(&devaddr, &regaddr, &data, 1);
	//LOG("%x ", data);
	data = 0x0;
	pmu_reg_write(&devaddr, &regaddr, &data, 1);
	//LOG("%x reg12 poweroff\n", data);

	devaddr = RSB_RTSADDR_AW1736;
	regaddr = AW1736_OUTPUT_PWR_CTRL0;
	data = 0x40; //rtc not power down
	pmu_reg_write(&devaddr, &regaddr, &data, 1);
	//LOG("%x reg10 poweroff\n", data);
}

int pmu_set_gpio(unsigned int id, unsigned int state)
{

	u8 devaddr;
	u8 regaddr;
	u8 data;

	if ((id > 2) || (state > 1))
		return -EINVAL;

	devaddr = RSB_RTSADDR_AW1736;
	if (id == 1) {
		regaddr = AW1736_CLDO3_VOL_CTRL;
		data = 0;
		if (state) {
			data |= 0b10 << 5;
			data |= 0x1f;
		} else {
			data |= 0b01 << 5;
			data |= 0x0;
		}
		pmu_reg_write(&devaddr, &regaddr, &data, 1);
	} else if (id == 2) {
		if (state) {
			regaddr = AW1736_CLDO4_GPIO2_CTRL;
			pmu_reg_read(&devaddr, &regaddr, &data, 1);
			data &= ~(0x7);
			data |= 0b10;
			pmu_reg_write(&devaddr, &regaddr, &data, 1);
			regaddr = AW1736_CLDO4_VOL_CTRL;
			data = 0x3f;
			pmu_reg_write(&devaddr, &regaddr, &data, 1);
		} else {
			regaddr = AW1736_CLDO4_GPIO2_CTRL;
			pmu_reg_read(&devaddr, &regaddr, &data, 1);
			data &= ~(0x7);
			data |= 0b01;
			pmu_reg_write(&devaddr, &regaddr, &data, 1);
			regaddr = AW1736_CLDO4_VOL_CTRL;
			data = 0x0;
			pmu_reg_write(&devaddr, &regaddr, &data, 1);
		}

	}

	return OK;
}

#endif /* CFG_PMU_AW1736_USED */
#endif /* CFG_PMU_USED */
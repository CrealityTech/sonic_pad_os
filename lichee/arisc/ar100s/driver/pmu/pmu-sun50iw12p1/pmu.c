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

#define RSB_INVALID_RTSADDR (0)

#define WATCHDOG_KEYFIELD (0x16aa << 16)

#define SUNXI_CHARGING_FLAG (0x61)

extern struct arisc_para arisc_para;
extern struct notifier *wakeup_notify;

void watchdog_reset(void);

u32 axp_power_max;

static u32 pmu_exist = FALSE;


void pmu_shutdown(void)
{

}

void pmu_reset(void)
{
}

void pmu_charging_reset(void)
{
}

s32 pmu_set_voltage(u32 type, u32 voltage)
{
	s32 ret = OK;
	return ret;
}

s32 pmu_get_voltage(u32 type)
{
	s32 ret = OK;

	return ret;
}

s32 pmu_set_voltage_state(u32 type, u32 state)
{
	s32 ret = OK;

	return ret;
}

s32 pmu_get_voltage_state(u32 type)
{
	s32 ret = OK;

	return ret;
}

s32 pmu_query_event(u32 *event)
{
	s32 ret = OK;
	return ret;
}

s32 pmu_clear_pendings(void)
{
	s32 ret = OK;

	return ret;
}

void pmu_chip_init(void)
{
}

s32 pmu_reg_write(u8 *devaddr, u8 *regaddr, u8 *data, u32 len)
{
	ASSERT(len <= AXP_TRANS_BYTE_MAX);

#ifdef CFG_RSB_USED
	u8 i;
	s32 result = OK;
	u32 data_temp = 0;

	for (i = 0; i < len; i++) {
		data_temp = *(data + i);
		result |= rsb_write(devaddr[i], regaddr[i], &data_temp, RSB_DATA_TYPE_BYTE);
	}
	return result;
#else
	return twi_write(devaddr[0], regaddr, data, len);
#endif
}

s32 pmu_reg_read(u8 *devaddr, u8 *regaddr, u8 *data, u32 len)
{
	ASSERT(len <= AXP_TRANS_BYTE_MAX);

#ifdef CFG_RSB_USED
	u8 i;
	s32 result = OK;
	u32 data_temp = 0;

	for (i = 0; i < len; i++) {
		result |= rsb_read(devaddr[i], regaddr[i], &data_temp, RSB_DATA_TYPE_BYTE);
		*(data + i) = (u8)(data_temp & 0xff);
	}
	return result;
#else
	return twi_read(devaddr[0], regaddr, data, len);
#endif
}

s32 pmu_reg_write_para(pmu_paras_t *para)
{
	return pmu_reg_write(para->devaddr, para->regaddr, para->data, para->len);
}

s32 pmu_reg_read_para(pmu_paras_t *para)
{
	return pmu_reg_read(para->devaddr, para->regaddr, para->data, para->len);
}

void watchdog_reset(void)
{
}

int nmi_int_handler(void *parg __attribute__ ((__unused__)), u32 intno)
{
	return TRUE;
}

s32 pmu_init(void)
{
	return OK;
}

s32 pmu_exit(void)
{
	return OK;
}

u32 is_pmu_exist(void)
{
	return pmu_exist;
}

s32 pmu_standby_init(void)
{
	return OK;
}

s32 pmu_standby_exit(void)
{
	return OK;
}


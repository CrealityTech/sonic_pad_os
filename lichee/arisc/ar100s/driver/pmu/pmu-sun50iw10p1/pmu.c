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
#include "include.h"
#include "notifier.h"
#include "wakeup_source.h"

#define WATCHDOG_KEYFIELD (0x16aa << 16)

#if defined CFG_AXP803_USED
extern pmu_ops_t pmu_axp803_ops;
static pmu_ops_t *pmu_ops_p = &pmu_axp803_ops;
u32 axp_power_max = AXP1660_POWER_MAX;
#elif defined CFG_AXP2202_USED
extern pmu_ops_t pmu_axp2202_ops;
static pmu_ops_t *pmu_ops_p = &pmu_axp2202_ops;
u32 axp_power_max = AXP2202_POWER_MAX;
#elif defined CFG_AXP806_USED
extern pmu_ops_t pmu_axp806_ops;
static pmu_ops_t *pmu_ops_p = &pmu_axp806_ops;
u32 axp_power_max = AW1657_POWER_MAX;
#elif defined CFG_AXP858_USED
extern pmu_ops_t pmu_axp858_ops;
static pmu_ops_t *pmu_ops_p = &pmu_axp858_ops;
u32 axp_power_max = AW1736_POWER_MAX;
#else
#error "sun50iw10p1 not support no pmu"
#endif

extern struct arisc_para arisc_para;
extern struct notifier *wakeup_notify;

static u32 pmu_exist = FALSE;

void watchdog_reset(void)
{
	LOG("watchdog reset\n");

	/* disable watchdog int */
	writel(0x0, R_WDOG_REG_BASE + 0x0);

	/* reset whole system */
	writel((0x1 | (0x1 << 8) | WATCHDOG_KEYFIELD), R_WDOG_REG_BASE + 0x14);

	/* set reset after 0.5s */
	writel(((0 << 4) | WATCHDOG_KEYFIELD), R_WDOG_REG_BASE + 0x18);
	mdelay(1);

	/* enable watchdog */
	writel((readl(R_WDOG_REG_BASE + 0x18) | 0x1 | WATCHDOG_KEYFIELD), R_WDOG_REG_BASE + 0x18);
	while (1)
		;
}

void pmu_shutdown(void)
{
	if (is_pmu_exist() == FALSE)
		return;

	while (!!twi_get_status()) {
		LOG("wait twi bus idle loop\n");
		time_mdelay(1000 * 2);
		if (!twi_send_clk_9pulse())
			break;
	}

	if (pmu_ops_p->pmu_shutdown)
		pmu_ops_p->pmu_shutdown();
}

void pmu_reset(void)
{
	if (is_pmu_exist() == FALSE) {
		watchdog_reset();
	}

	while (!!twi_get_status()) {
		LOG("wait twi bus idle loop\n");
		time_mdelay(1000 * 2);
		if (!twi_send_clk_9pulse())
			break;
	}

#if defined CFG_AXP806_USED
	watchdog_reset();
#else
	if (pmu_ops_p->pmu_reset)
		pmu_ops_p->pmu_reset();
#endif
}

void pmu_charging_reset(void)
{
	if (is_pmu_exist() == FALSE)
		return;

	while (!!twi_get_status()) {
		time_mdelay(1000 * 2);
		LOG("wait twi bus idle loop\n");
	}

	if (pmu_ops_p->pmu_charging_reset)
		pmu_ops_p->pmu_charging_reset();
}

s32 pmu_set_voltage(u32 type, u32 voltage)
{
	s32 ret = OK;

	if (is_pmu_exist() == FALSE)
		return -ENODEV;

	return ret;
}

s32 pmu_get_voltage(u32 type)
{
	s32 ret = OK;

	if (is_pmu_exist() == FALSE)
		return -ENODEV;

	return ret;
}

s32 pmu_set_voltage_state(u32 type, u32 state)
{
	s32 ret = OK;

	if (is_pmu_exist() == FALSE)
		return -ENODEV;

	if (pmu_ops_p->pmu_set_voltage_state)
		ret = pmu_ops_p->pmu_set_voltage_state(type, state);

	return ret;
}

s32 pmu_get_voltage_state(u32 type)
{
	s32 ret = OK;

	if (is_pmu_exist() == FALSE)
		return -ENODEV;

	return ret;
}

s32 pmu_query_event(u32 *event)
{
	s32 ret = OK;

	if (is_pmu_exist() == FALSE)
		return OK;

	return ret;
}

s32 pmu_clear_pendings(void)
{
	s32 ret = OK;

	if (is_pmu_exist() == FALSE)
		return OK;

	return ret;
}

void pmu_chip_init(void)
{
	if (is_pmu_exist() == FALSE)
		return;
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

int nmi_int_handler(void *parg __attribute__ ((__unused__)), u32 intno)
{
	u32 event = 0;

	/**
	 * NOTE: if pmu interrupt enabled,
	 * means allow power key to power on system
	 */
	pmu_query_event(&event);
	LOG("pmu event: 0x%x\n", event);

	//notifier_notify(&wakeup_notify, CPUS_IRQ_MAPTO_CPUX(intno));

	pmu_clear_pendings();

	/* clear interrupt flag first */
	interrupt_clear_pending(INTC_R_NMI_IRQ);

	return TRUE;
}

s32 pmu_init(void)
{
	u32 power_mode = 0;

	/* power_mode may parse from dts */
	if (power_mode == POWER_MODE_AXP) {
		pmu_exist = TRUE;
		LOG("pmu is exist\n");
	} else {
		pmu_exist = FALSE;
		LOG("pmu is not exist\n");
		return OK;
	}

	interrupt_clear_pending(INTC_R_NMI_IRQ);

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
	struct message message_ws;
	u32 paras = GIC_R_EXTERNAL_NMI_IRQ - 32;

	message_ws.paras = &paras;
	set_wakeup_src(&message_ws);

	return OK;
}

s32 pmu_standby_exit(void)
{
	struct message message_ws;
	u32 paras = GIC_R_EXTERNAL_NMI_IRQ - 32;

	message_ws.paras = &paras;
	clear_wakeup_src(&message_ws);

	return OK;
}


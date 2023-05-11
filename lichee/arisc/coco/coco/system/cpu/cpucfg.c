/*
 *********************************************************************************************************
 *                                                AR100 SYSTEM
 *                                     AR100 Software System Develop Kits
 *
 *
 *                                    (c) Copyright 2018-2024, Luojie China
 *                                             All Rights Reserved
 *
 * File    : cpucfg.c
 * By      : LuoJie
 * Version : v1.0
 * Date    : 2018-06-05 16:31
 * Descript: .
 * Update  :auther      ver     notes
 *          Luojie      1.0     Create this file.
 *********************************************************************************************************
 */
#include "ibase.h"
#include "cpucfg.h"

static u32 little_endian_used = 0;



s32 cpucfg_set_little_endian_address(void *start, void *end)
{
	volatile u32 value;

	if (little_endian_used >= CPUCFG_LE_SPACE_NUMBER)
	{
		WRN("no space for little-endian config\n");
		return -ENOSPC;
	}

	//config space0 as little-endian
	value  = readl(CPUS_SPACE_LE_CTRL_REG);
	value |= (0x1 << little_endian_used);
	writel(value, CPUS_SPACE_LE_CTRL_REG);

	//config start address and end address
	writel((volatile u32)start, CPUS_SPACE_START_REG(little_endian_used));
	writel((volatile u32)end, CPUS_SPACE_END_REG(little_endian_used));

	//the number of used little-endian space increase
	little_endian_used++;

	//set little-endian succeeded
	return OK;
}



s32 cpucfg_remove_little_endian_address(void *start, void *end)
{
	int i;
	volatile u32 value;

	end = end;
	if (little_endian_used <= 0) {
		WRN("no space for little-endian remove\n");
		return -ENODEV;
	}

	for (i = little_endian_used; i; i--)
		if ((int)(readl(CPUS_SPACE_START_REG(i))>>9) == (int)start>>9)
			break;
	INF("remove:%d\n", i);
	//config space0 as little-endian
	value  = readl(CPUS_SPACE_LE_CTRL_REG);
	value &= ~(0x1 << i);
	writel(value, CPUS_SPACE_LE_CTRL_REG);

	//config start address and end address
	writel((volatile u32)0, CPUS_SPACE_START_REG(i));
	writel((volatile u32)0, CPUS_SPACE_END_REG(i));

	//the number of used little-endian space increase
	little_endian_used--;

	//set little-endian succeeded
	return OK;
}


static inline u32 read_timestamp_low(void)
{
	return readl(CNT_LOW_REG);
}

static inline u32 read_timestamp_high(void)
{
	return readl(CNT_HIGH_REG);
}



u64 timestamp_read(void)
{
	volatile u32 high;
	volatile u32 low;
	volatile u64 counter;

	do {
		low  = read_timestamp_low();
		high = read_timestamp_high();
	} while ((high != read_timestamp_high()) || (low > read_timestamp_low()));

	counter = high;
	counter = (counter << 32) + low;

	return counter;
}


static int cpu_power_switch_set(u32 cluster, u32 cpu, bool enable)
{
	if (enable) {
		if (0x00 == readl(SUNXI_CPU_PWR_CLAMP(cluster, cpu))) {
			//WRN("cpu%d power switch enable already\n", cpu);
			return OK;
		}
		/* de-active cpu power clamp */
		writel(0xFE, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		time_udelay(20);

		writel(0xF8, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		time_udelay(10);

		writel(0xE0, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		time_udelay(10);

		writel(0xc0, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		time_udelay(10);

		writel(0x80, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		time_udelay(10);

		writel(0x40, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		time_udelay(10);

		writel(0x00, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		time_udelay(10);

		while(0x00 != readl(SUNXI_CPU_PWR_CLAMP(cluster, cpu)))
			time_udelay(10);
		while(0x00 != readl(SUNXI_CPU_PWR_CLAMP(cluster, cpu)))
			;
	} else {
		if (0xFF == readl(SUNXI_CPU_PWR_CLAMP(cluster, cpu))) {
			WRN("cpu%d power switch disable already\n", cpu);
			return OK;
		}
		writel(0xFF, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
			time_udelay(10);
		while(0xFF != readl(SUNXI_CPU_PWR_CLAMP(cluster, cpu)))
			;
	}
	return OK;
}


void cpucfg_cpu_suspend_late(void)
{
	u32 value;

	//set cpu0 H_reset as assert state
	writel(0, SUNXI_CLUSTER_PWRON_RESET(0));

	//set system reset to assert
	writel(0, SUNXI_CPU_SYS_RESET);
	time_mdelay(1);

      //set clustr0+c0_cpu0 power off gating to valid
	value = readl(C0_CPUX_POWEROFF_GATING_REG);
	value &= (~(0x1 << 0));
	value |= (1 & 0x1) << 0;
	writel(value, C0_CPUX_POWEROFF_GATING_REG);
	save_state_flag(REC_HOTPULG | 0x4);

	//power off cluster0 cpu0
	cpu_power_switch_set(0, 0, 0);
	save_state_flag(REC_HOTPULG | 0x5);
}

s32 cpucfg_cpu_resume_early(void)
{
	u32 value;
	//power on cluster0 cpu0
	cpu_power_switch_set(0, 0, 1);
	save_state_flag(REC_HOTPULG | 0x6);

	//set clustr0+cpu0 power off gating to invalid
	value = readl(C0_CPUX_POWEROFF_GATING_REG);
	value &= (~(0x1 << 0));
	value |= (0 & 0x1) << 0;
	writel(value, C0_CPUX_POWEROFF_GATING_REG);

	time_mdelay(1);
	save_state_flag(REC_HOTPULG | 0x7);
      //set system reset to de-assert state
	writel(1, SUNXI_CPU_SYS_RESET);
	//set cpu0 H_reset as de-assert state
	writel(1 << 16, SUNXI_CLUSTER_PWRON_RESET(0));
	save_state_flag(REC_HOTPULG | 0x8);

	return OK;
}




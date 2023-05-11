#include "includes.h"
#include "task.h"
#include "task_handle.h"
#include "msgmanager.h"
#include "cpucfg.h"
#include "psci.h"


void set_secondary_entry(u32 entry, u32 cpu)
{
	writel(entry, SUNXI_CPU_RVBA_L(cpu));
	writel(0, SUNXI_CPU_RVBA_H(cpu));
}

s32 cpucfg_set_cpux_jump_addr(u32 addr)
{
	writel(0x16AA1234, SUNXI_BOOT_CPU_STANDBY);

	writel(0xAA16efe8, SUNXI_BOOT_CPU_STANDBY);

	writel(addr, SUNXI_STANDBY_SOFT_ENTRY);

	return OK;
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
		__time_udelay(20);

		writel(0xF8, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		__time_udelay(10);

		writel(0xE0, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		__time_udelay(10);

		writel(0xc0, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		__time_udelay(10);

		writel(0x80, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		__time_udelay(10);

		writel(0x40, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		__time_udelay(10);

		writel(0x00, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		__time_udelay(10);

		while(0x00 != readl(SUNXI_CPU_PWR_CLAMP(cluster, cpu)))
			__time_udelay(20);
		while(0x00 != readl(SUNXI_CPU_PWR_CLAMP(cluster, cpu)))
			;
	} else {
		if (0xFF == readl(SUNXI_CPU_PWR_CLAMP(cluster, cpu))) {
			printf("cpu%d power switch disable already\n", cpu);
			return OK;
		}
		writel(0xFF, SUNXI_CPU_PWR_CLAMP(cluster, cpu));
		__time_udelay(30);
		while(0xFF != readl(SUNXI_CPU_PWR_CLAMP(cluster, cpu)))
			;
	}
	return OK;
}

void cpu_power_up(u32 cluster, u32 cpu)
{
	u32 value;

	/* Assert nCPUPORESET LOW */
	value	= readl(SUNXI_CPU_RST_CTRL(cluster));
	value &= (~(1<<cpu));
	writel(value, SUNXI_CPU_RST_CTRL(cluster));

	/* Assert cpu power-on reset */
	value	= readl(SUNXI_CLUSTER_PWRON_RESET(cluster));
	value &= (~(1<<cpu));
	writel(value, SUNXI_CLUSTER_PWRON_RESET(cluster));

	/* hold the core output clamps */
	value = readl(SUNXI_CLUSTER_PWROFF_GATING(cluster));
	value |= (0x1<<cpu);
	writel(value, SUNXI_CLUSTER_PWROFF_GATING(cluster));

	/* Apply power to the PDCPU power domain. */
	cpu_power_switch_set(cluster, cpu, 1);

	/* Release the core output clamps */
	value = readl(SUNXI_CLUSTER_PWROFF_GATING(cluster));
	value &= (~(0x1<<cpu));
	writel(value, SUNXI_CLUSTER_PWROFF_GATING(cluster));
	//__asm volatile ("l.csync");
	__time_udelay(1);

	/* Deassert cpu power-on reset */
	value	= readl(SUNXI_CLUSTER_PWRON_RESET(cluster));
	value |= ((1<<cpu));
	writel(value, SUNXI_CLUSTER_PWRON_RESET(cluster));

	/* Deassert core reset */
	value	= readl(SUNXI_CPU_RST_CTRL(cluster));
	value |= (1<<cpu);
	writel(value, SUNXI_CPU_RST_CTRL(cluster));

	/* Assert DBGPWRDUP HIGH */
	value = readl(SUNXI_DBG_REG0);
	value |= (1<<cpu);
	writel(value, SUNXI_DBG_REG0);

	value = readl(SUNXI_IRQ_FIQ_MASK);
	value |= (1<<cpu);
	writel(value, SUNXI_IRQ_FIQ_MASK);
}

void cpu_power_down(u32 cluster, u32 cpu)
{
	u32 value;

#ifdef FPGA_PLATFORM
	__time_cdelay(1000000);
#else
	while (!CPU_IS_WFI_MODE(cluster, cpu));
#endif
	value = readl(SUNXI_IRQ_FIQ_MASK);
	value &= (~(1<<cpu));
	writel(value, SUNXI_IRQ_FIQ_MASK);

	/* step7: Deassert DBGPWRDUP LOW */
	value = readl(SUNXI_DBG_REG0);
	value &= (~(1<<cpu));
	writel(value, SUNXI_DBG_REG0);

	/* step8: Activate the core output clamps */
	value = readl(SUNXI_CLUSTER_PWROFF_GATING(cluster));
	value |= (1 << cpu);
	writel(value, SUNXI_CLUSTER_PWROFF_GATING(cluster));
#ifdef CFG_OPENRISC
	__asm volatile ("l.csync");
#elif defined CFG_D10
	__asm volatile ("isb");
	__asm volatile ("dsb");
#endif

	__time_udelay(1);

	/* step9: Assert nCPUPORESET LOW */
	value	= readl(SUNXI_CPU_RST_CTRL(cluster));
	value &= (~(1<<cpu));
	writel(value, SUNXI_CPU_RST_CTRL(cluster));

	/* step10: Assert cpu power-on reset */
	value  = readl(SUNXI_CLUSTER_PWRON_RESET(cluster));
	value &= (~(1<<cpu));
	writel(value, SUNXI_CLUSTER_PWRON_RESET(cluster));

	/* step11: Remove power from th e PDCPU power domain */
	cpu_power_switch_set(cluster, cpu, 0);

}


static s32 cpucfg_set_cpu_reset_state(u32 cpu_num, s32 state)
{
	volatile u32 value;

	ASSERT(cpu_num < CPUCFG_CPU_NUMBER);

	//set cluster0 cpux state
	value  = readl(C0_RST_CTRL_REG);
	value &= ~(0x1 << cpu_num);
	value |= ((state & 0x1) << cpu_num);
	writel(value, C0_RST_CTRL_REG);

	return OK;
}

static void cpucfg_l1l2_reset_by_hardware(u32 status)
{
	volatile u32 value;

	value  = readl(C0_CTRL_REG0);
	value &= ~0x1f;
	value |= (0x1f & status);
	writel(value, C0_CTRL_REG0);
}

static void cpucfg_acinactm_process(u32 status)
{
	volatile u32 value;

	//Assert ACINACTM to LOW
	value  = readl(C0_CTRL_REG1);
	value &= ~0x1;
	value |= (0x1 & status);
	writel(value, C0_CTRL_REG1);
}
static void cpucfg_cluster0_process(u32 status)
{

	switch(status)
	{
		case CLUSTER_RESET_ASSERT:
			{
				writel(0, SUNXI_CPU_RST_CTRL(0));
				break;
			}
		case CLUSTER_RESET_DEASSERT:
			{
				writel(0x13ff0100, SUNXI_CPU_RST_CTRL(0));
				writel(0xf, SUNXI_DBG_REG0);
				break;
			}
		default:
			{
				printf("invalid cluster reset status\n");
			}
	}
}

static void cpucfg_wait_l2_enter_wfi(void)
{
#ifndef FPGA_PLATFORM
	while ((readl(C0_CPU_STATUS_REG) & (1 << 0)) != 1)
		;
#endif
}

void cpucfg_cpu_suspend(void)
{
	volatile u32 value;

	/* Assert ACINACTM to HIGH */
	cpucfg_acinactm_process(ACINACTM_HIGH);
	save_state_flag(REC_HOTPULG | 0x0);

	/* wait L2 enter WFI */
	cpucfg_wait_l2_enter_wfi();
	save_state_flag(REC_HOTPULG | 0x1);

	value = readl(SUNXI_DBG_REG0);
	value &= (~(1<<0));
	writel(value, SUNXI_DBG_REG0);

	cpucfg_set_cpu_reset_state(CPUCFG_C0_CPU0, CPU_RESET_ASSERT);

	//set cpu0 poweron reset as assert state
	value  = readl(SUNXI_CLUSTER_PWRON_RESET(0));
	value &= ~(0x1 << 0);
	writel(value, SUNXI_CLUSTER_PWRON_RESET(0));
	//set cpu0 core_reset as assert state.
	cpucfg_set_cpu_reset_state(CPUCFG_C0_CPU0, CPU_RESET_ASSERT);
	save_state_flag(REC_HOTPULG | 0x2);

	cpucfg_cluster0_process(CLUSTER_RESET_ASSERT);
	save_state_flag(REC_HOTPULG | 0x3);
}


s32 cpucfg_cpu_resume(u32 entrypoint)
{
	u32 value;

	cpucfg_set_cpu_reset_state(CPUCFG_C0_CPU0, CPU_RESET_ASSERT);
	save_state_flag(REC_HOTPULG | 0x8);
	//enable l1&l2 reset by hardware
	cpucfg_l1l2_reset_by_hardware(L1_EN_L2_EN);
	save_state_flag(REC_HOTPULG | 0x9);

	//Assert ACINACTM to LOW
	cpucfg_acinactm_process(ACINACTM_LOW);
	save_state_flag(REC_HOTPULG | 0xa);

	cpucfg_cluster0_process(CLUSTER_RESET_DEASSERT);

	save_state_flag(REC_HOTPULG | 0xc);

	cpucfg_set_cpux_jump_addr(entrypoint);
	//set cpu0 power-on reset as deassert state
	value  = readl(SUNXI_CLUSTER_PWRON_RESET(0));
	value |= (0x1 << 0);
	writel(value, SUNXI_CLUSTER_PWRON_RESET(0));
	//set cpu0 core_reset as de-assert state.
	cpucfg_set_cpu_reset_state(CPUCFG_C0_CPU0, CPU_RESET_DEASSERT);
	save_state_flag(REC_HOTPULG | 0xd);

	return OK;
}

#ifdef CFG_HWCPUIDLE_USED
static void cpux_hotplug_control(bool enable)
{
	if (enable)
		writel(0xfa50392f, SUNXI_BOOT_CPU_HOTPLUG);
	else
		writel(0x00000000, SUNXI_BOOT_CPU_HOTPLUG);
}

s32 cpuidle_suspend(void)
{
	cpux_hotplug_control(0);

	return 0;
}

s32 cpuidle_resume(void)
{
	cpux_hotplug_control(1);

	return 0;
}
#endif

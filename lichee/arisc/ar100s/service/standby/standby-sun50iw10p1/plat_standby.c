/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                               standby module
*
*                                    (c) Copyright 2012-2016, superm China
*                                             All Rights Reserved
*
* File    : extended_super_standby.c
* By      : superm
* Version : v1.0
* Date    : 2013-1-16
* Descript: extended super-standby module public header.
* Update  : date                auther      ver     notes
*           2013-1-16 17:04:28  superm       1.0     Create this file.
*********************************************************************************************************
*/
#include <libfdt.h>
#include "../standby_i.h"
#include "wakeup_source.h"
#include "cpucfg_regs.h"

extern u32 dram_crc_enable;
extern u32 dram_crc_src;
extern u32 dram_crc_len;
extern u32 volatile wakeup_source;
extern u32 axp_power_max;

static s32 result;
static u32 cpus_src;
static u32 pll_periph0, mbus;
static u32 suspend_lock;

static u32 standby_type;

static u32 volatile pll_cpux_ctrl_reg_bak;
static u32 volatile c0_cpux_axi_cfg_reg_bak;

static uint32_t standby_vdd_cpu;
static uint32_t standby_vdd_sys;
static uint32_t standby_vcc_pll;
static uint32_t standby_osc24m_on = 1;
static s32 standby_dts_parse(void)
{
	static u32 dts_has_parsed = 0;
	void *fdt;
	int32_t param_node;

	if (!!dts_has_parsed)
		return 0;

	fdt = (void *)(ARISC_DTS_BASE);

	/* parse power tree */
	param_node = fdt_path_offset(fdt, "standby_param");
	if (param_node < 0) {
		WRN("no standby_param: %x\n", param_node);

		/* try "standby-param" more to keep compatibility with linux-4.9 and linux-5.4 */
		param_node = fdt_path_offset(fdt, "standby-param");
		if (param_node < 0) {
			ERR("no standby-param: %x\n", param_node);
			return -1;
		}
	}

	fdt_getprop_u32(fdt, param_node, "vdd-cpu", &standby_vdd_cpu);
	fdt_getprop_u32(fdt, param_node, "vdd-sys", &standby_vdd_sys);
	fdt_getprop_u32(fdt, param_node, "vcc-pll", &standby_vcc_pll);
	fdt_getprop_u32(fdt, param_node, "osc24m-on", &standby_osc24m_on);
	LOG("standby power %x, %x, %x, %x\n",
		standby_vdd_cpu, standby_vdd_sys, standby_vcc_pll, standby_osc24m_on);

	dts_has_parsed = 1;

	return 0;
}

static void wait_wakeup(void)
{
	wakeup_timer_start();
	while (1) {
		/*
		 * maybe add user defined task process here
		 */


		if (wakeup_source != NO_WAKESOURCE) {
			LOG("wakeup: %d\n", wakeup_source);
			break;
		}

		writel(readl(LP_CTRL_REG) | ((1 << 24) | (1 << 25) | (1 << 26) | (1 << 27)), LP_CTRL_REG);
		cpu_enter_doze();
		writel(readl(LP_CTRL_REG) & (~((1 << 24) | (1 << 25) | (1 << 26) | (1 << 27))), LP_CTRL_REG);
	}
	wakeup_timer_stop();
}

static void wait_cpu0_resume(void)
{
	s32 ret;
	struct message message;

	/* no paras for resume notify message */
	message.paras = NULL;

	printk("wait ac327 resume...\n");

	/* wait cpu0 restore finished. */
	while (1) {
		ret = hwmsgbox_query_message(&message, 0);
		if (ret != OK)
			continue; /* no message, query again */

		/* query valid message */
		if (message.type == SSTANDBY_RESTORE_NOTIFY) {
			/* cpu0 restore, feedback wakeup event. */
			LOG("cpu0 restore finished\n");
			/* init feedback message */
			message.count = 1;
			message.paras = (u32 *)&wakeup_source;
			hwmsgbox_feedback_message(&message, SEND_MSG_TIMEOUT); /* synchronous message, need feedback. */
			break;
		} else {
			/* invalid message detected, ignore it, by sunny at 2012-6-28 11:33:13. */
			ERR("standby ignore message [%x]\n", message.type);
		}
	}

	wakeup_source = NO_WAKESOURCE;
}

static void dm_suspend(void)
{
	u32 type;

	/* vdd-cpu powerdown */
	for (type = 0; type < axp_power_max; type++) {
		if ((standby_vdd_cpu >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_OFF);
	}

	/* vdd-sys powerdown */
	for (type = 0; type < axp_power_max; type++) {
		if ((standby_vdd_sys >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_OFF);
	}

	/* vcc-pll powerdown */
	for (type = 0; type < axp_power_max; type++) {
		if ((standby_vcc_pll >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_OFF);
	}
}

static void dm_resume(void)
{
	u32 type;

	/* vcc-pll powerup */
	for (type = 0; type < axp_power_max; type++) {
		if ((standby_vcc_pll >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_ON);
	}

	/* vdd-sys powerup */
	for (type = 0; type < axp_power_max; type++) {
		if ((standby_vdd_sys >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_ON);
	}

	/* vdd-cpu powerup */
	for (type = 0; type < axp_power_max; type++) {
		if ((standby_vdd_cpu >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_ON);
	}
}

static void dram_suspend(void)
{
#ifndef CFG_FPGA_PLATFORM
	/* calc dram checksum */
	if (standby_dram_crc_enable()) {
		before_crc = standby_dram_crc();
		LOG("before_crc: 0x%x\n", before_crc);
	}
#endif
	pll_periph0 = readl(CCU_PLL_PERIPH0_REG);
	mbus = readl(CCU_MBUS_CLK_REG);
	dram_power_save_process(&arisc_para.dram_para);
}

static void dram_resume(void)
{
	/* restore dram controller and transing area. */
	LOG("power-up dram\n");

	/* disable pll_periph0 firstly */
	writel(readl(CCU_PLL_PERIPH0_REG) & (~(1 << 31)), CCU_PLL_PERIPH0_REG);

	/* set pll factor but not enable pll */
	writel((pll_periph0&(~(0x1 << 31))), CCU_PLL_PERIPH0_REG);

	/* delay 100us for factor to be effective */
	time_udelay(100);

	/* enable pll */
	writel((readl(CCU_PLL_PERIPH0_REG) | (0x1 << 31)), CCU_PLL_PERIPH0_REG);

	time_udelay(20);

	/*
	 * mbus default clk src is 24MHz, switch to pll_periph0(x2),
	 * so before increase mbus freq, should set div firstly.
	 * by Superm Wu at 2015-09-18
	 */
#ifndef CFG_FPGA_PLATFORM
	writel(mbus&0x7, CCU_MBUS_CLK_REG);
	time_udelay(200);
	writel(mbus&((0x3 << 24) | 0x7), CCU_MBUS_CLK_REG);
	time_udelay(20);
	writel((readl(CCU_MBUS_CLK_REG) | (0x1 << 31)), CCU_MBUS_CLK_REG);
	time_udelay(10000);
	dram_power_up_process(&arisc_para.dram_para);

	/* calc dram checksum */
	if (standby_dram_crc_enable()) {
//		dram_master_enable(16, 1);
		after_crc = standby_dram_crc();
		if (after_crc != before_crc) {
			save_state_flag(REC_SSTANDBY | REC_DRAM_DBG | 0xf);
			ERR("dram crc error...\n");
			ERR("---->>>>LOOP<<<<----\n");
			while (1)
				;
		}
	}
#else
	/* on fpga platform, do nothing */
#endif
}

struct SMC_REGION_T {
	u32 region_low;
	u32 region_high;
	u32 region_attr;
} static smc_region_save[SMC_REGION_COUNT];
static void smc_standby_init(void)
{
	int read_idx;
	for (read_idx = 0; read_idx < SMC_REGION_COUNT; read_idx++) {
		if (readl(SMC_REGIN_ATTRIBUTE_REG(read_idx)) == 0)
			break;
		smc_region_save[read_idx].region_low =
			readl(SMC_REGIN_SETUP_LOW_REG(read_idx));
		smc_region_save[read_idx].region_high =
			readl(SMC_REGIN_SETUP_HIGH_REG(read_idx));
		smc_region_save[read_idx].region_attr =
			readl(SMC_REGIN_ATTRIBUTE_REG(read_idx));
	}
}
static void smc_standby_exit(void)
{
	int read_idx, resume_idx;
	if ((readl(SID_SEC_MODE_STA) & SID_SEC_MODE_MASK) == 0) {
		/*chip non-secure, do not need smc config*/
		return;
	}

	/*enable smc control*/
	writel(0x0, SMC_ACTION_REG);
	writel(0, SMC_MST0_BYP_REG);
	writel(0, SMC_MST1_BYP_REG);
	writel(0, SMC_MST2_BYP_REG);
	writel(0xffffffff, SMC_MST0_SEC_REG);
	writel(0xffffffff, SMC_MST1_SEC_REG);
	writel(0xffffffff, SMC_MST2_SEC_REG);

	/*resume region settings*/
	resume_idx = 0;
	for (read_idx = 0; read_idx < SMC_REGION_COUNT; read_idx++) {
		if (smc_region_save[read_idx].region_attr == 0)
			break;
		writel(smc_region_save[resume_idx].region_low,
		       SMC_REGIN_SETUP_LOW_REG(resume_idx));
		writel(smc_region_save[resume_idx].region_high,
		       SMC_REGIN_SETUP_HIGH_REG(resume_idx));
		writel(smc_region_save[resume_idx].region_attr,
		       SMC_REGIN_ATTRIBUTE_REG(resume_idx));
		resume_idx++;
	}
}

static void usb_suspend(void)
{
	u32 val;

	if (!(standby_type & CPUS_WAKEUP_USB)) {
		val = readl(VDD_SYS_PWROFF_GATING_REG);
		val |= VDD_USB2CPUS_GATING(1);
		writel(val, VDD_SYS_PWROFF_GATING_REG);
	} else {
		/* for usb standby suspend , usb0*/
		val = readl(CCU_USB0_CLOCK_REG);
		val &= ~SCLK_GATING_USBPHY0_MASK;
		val &= ~SCLK_GATING_OHCI0_MASK;
		writel(val, CCU_USB0_CLOCK_REG);

		val = readl(CCU_USB_BUS_GATING_RST_REG);
		val &= ~USBOHCI0_GATING_MASK;
		val &= ~USBEHCI0_GATING_MASK;
		writel(val, CCU_USB_BUS_GATING_RST_REG);

		/* usb1*/
		val = readl(CCU_USB1_CLOCK_REG);
		val &= ~SCLK_GATING_USBPHY1_MASK;
		val &= ~SCLK_GATING_OHCI1_MASK;
		writel(val, CCU_USB1_CLOCK_REG);

		val = readl(CCU_USB_BUS_GATING_RST_REG);
		val &= ~USBEHCI1_GATING_MASK;
		val &= ~USBOHCI1_GATING_MASK;
		writel(val, CCU_USB_BUS_GATING_RST_REG);

		val = readl(VDD_SYS_PWROFF_GATING_REG);
		val |= VDD_SYS2USB_GATING(1);
		writel(val, VDD_SYS_PWROFF_GATING_REG);
	}
}

static void usb_resume(void)
{
	u32 val;

	if (!(standby_type & CPUS_WAKEUP_USB)) {
		val = readl(VDD_SYS_PWROFF_GATING_REG);
		val &= ~VDD_USB2CPUS_GATING(1);
		writel(val, VDD_SYS_PWROFF_GATING_REG);
	} else {
		/* for usb standby resume */
		val = readl(CCU_USB_BUS_GATING_RST_REG);
		val |= USBEHCI1_GATING_MASK;
		val |= USBOHCI1_GATING_MASK;
		writel(val, CCU_USB_BUS_GATING_RST_REG);

		val = readl(CCU_USB_BUS_GATING_RST_REG);
		val |= USBEHCI1_RST_MASK;
		val |= USBOHCI1_RST_MASK;
		writel(val, CCU_USB_BUS_GATING_RST_REG);

		val = readl(CCU_USB1_CLOCK_REG);
		val |= SCLK_GATING_USBPHY1_MASK;
		val |= SCLK_GATING_OHCI1_MASK;
		val |= USBPHY1_RST_MASK;
		writel(val, CCU_USB1_CLOCK_REG);

		/* for usb standby resume */
		val = readl(CCU_USB_BUS_GATING_RST_REG);
		val |= USBOHCI0_GATING_MASK;
		val |= USBEHCI0_GATING_MASK;
		writel(val, CCU_USB_BUS_GATING_RST_REG);

		val = readl(CCU_USB_BUS_GATING_RST_REG);
		val |= USBOHCI0_RST_MASK;
		val |= USBEHCI0_RST_MASK;
		writel(val, CCU_USB_BUS_GATING_RST_REG);

		val = readl(CCU_USB0_CLOCK_REG);
		val |= SCLK_GATING_USBPHY0_MASK;
		val |= SCLK_GATING_OHCI0_MASK;
		val |= USBPHY0_RST_MASK;
		writel(val, CCU_USB0_CLOCK_REG);

		val = readl(VDD_SYS_PWROFF_GATING_REG);
		val &= ~VDD_SYS2USB_GATING(1);
		writel(val, VDD_SYS_PWROFF_GATING_REG);

	}

}

static void device_suspend(void)
{
	smc_standby_init();
	pmu_standby_init();
	twi_standby_init();
	hwmsgbox_super_standby_init();
	usb_suspend();
}

static void device_resume(void)
{
	usb_resume();
	hwmsgbox_super_standby_exit();
	twi_standby_exit();
	pmu_standby_exit();
	smc_standby_exit();
}

static void clk_save(void)
{
	pll_cpux_ctrl_reg_bak = readl(CCU_PLL_C0_REG);
	c0_cpux_axi_cfg_reg_bak = readl(CCU_CPU_AXI_CFG_REG);
}

static void clk_restore(void)
{
	/* recovery CCU_PLL_C0_REG */
	writel(pll_cpux_ctrl_reg_bak, CCU_PLL_C0_REG);

	writel((readl(CCU_PLL_C0_REG) & (~C0_PLL_LOCK_ENABLE_MASK)) | (C0_PLL_LOCK_ENABLE(0)),
			CCU_PLL_C0_REG);
	writel((readl(CCU_PLL_C0_REG) & (~C0_PLL_LOCK_ENABLE_MASK)) | (C0_PLL_LOCK_ENABLE(1)),
			CCU_PLL_C0_REG);

	while (!(readl(CCU_PLL_C0_REG) & C0_PLL_LOCK_STATUS_MASK))
		;

	/* recovery CCU_CPU_AXI_CFG_REG */
	writel((readl(CCU_CPU_AXI_CFG_REG) & (~CPUX_AXI_FACTOR_M_MASK)) | (c0_cpux_axi_cfg_reg_bak & CPUX_AXI_FACTOR_M_MASK),
			CCU_CPU_AXI_CFG_REG);
	writel((readl(CCU_CPU_AXI_CFG_REG) & (~CPUX_APB_FACTOR_N_MASK)) | (c0_cpux_axi_cfg_reg_bak & CPUX_APB_FACTOR_N_MASK),
			CCU_CPU_AXI_CFG_REG);
	writel((readl(CCU_CPU_AXI_CFG_REG) & (~CPUX_CLK_SRC_SEL_MASK)) | (c0_cpux_axi_cfg_reg_bak & CPUX_CLK_SRC_SEL_MASK),
			CCU_CPU_AXI_CFG_REG);
	time_mdelay(1);
}

static void clk_suspend(void)
{
	u32 val;

	clk_save();

	/* set cpus clk to RC16M */
	writel((readl(CPUS_CFG_REG) & (~CPUS_CLK_SRC_SEL_MASK)) | CPUS_CLK_SRC_SEL(2), CPUS_CFG_REG);

	/*
	 * set apbs2 clk to RC16M,
	 * then change the baudrate of uart and twi...
	 */
	twi_clkchangecb(CCU_CLK_CLKCHG_REQ, iosc_freq);
	uart_clkchangecb(CCU_CLK_CLKCHG_REQ, iosc_freq);
	writel((readl(APBS2_CFG_REG) & (~APBS2_CLK_SRC_SEL_MASK)) | APBS2_CLK_SRC_SEL(2), APBS2_CFG_REG);
	uart_clkchangecb(CCU_CLK_CLKCHG_DONE, iosc_freq);
	twi_clkchangecb(CCU_CLK_CLKCHG_DONE, iosc_freq);
	time_mdelay(10);

	/* set axi, psi, ahb3, apb1, apb2 clk to RTC32K */
	writel((readl(CCU_PSI_AHB1_AHB2_CFG_REG) & (~PSI_CLK_SRC_SEL_MASK)) | PSI_CLK_SRC_SEL(1), CCU_PSI_AHB1_AHB2_CFG_REG);
	writel((readl(CCU_AHB3_CFG_REG) & (~AHB3_CLK_SRC_SEL_MASK)) | AHB3_CLK_SRC_SEL(1), CCU_AHB3_CFG_REG);
	writel((readl(CCU_APB1_CFG_REG) & (~APB1_CLK_SRC_SEL_MASK)) | APB1_CLK_SRC_SEL(1), CCU_APB1_CFG_REG);
	writel((readl(CCU_APB2_CFG_REG) & (~APB2_CLK_SRC_SEL_MASK)) | APB2_CLK_SRC_SEL(1), CCU_APB2_CFG_REG);
	writel((readl(CCU_CPU_AXI_CFG_REG) & (~CPUX_CLK_SRC_SEL_MASK)) | CPUX_CLK_SRC_SEL(1), CCU_CPU_AXI_CFG_REG);

	/* disable PLLs */
	writel((readl(CCU_PLL_C0_REG) & (~CPUX_PLL_ENABLE_MASK)) | CPUX_PLL_ENABLE(0), CCU_PLL_C0_REG);
	writel((readl(CCU_PLL_DDR0_REG) & (~DDR0_PLL_ENABLE_MASK)) | DDR0_PLL_ENABLE(0), CCU_PLL_DDR0_REG);
	writel((readl(CCU_PLL_PERIPH0_REG) & (~PERIPH0_PLL_ENABLE_MASK)) | PERIPH0_PLL_ENABLE(0), CCU_PLL_PERIPH0_REG);
	writel((readl(CCU_PLL_PERIPH1_REG) & (~PERIPH1_PLL_ENABLE_MASK)) | PERIPH1_PLL_ENABLE(0), CCU_PLL_PERIPH1_REG);

	val = readl(RTC_XO_CTRL_REG) & (1 << 31);
	if ((!standby_osc24m_on) || (!!val)) {
		ccu_24mhosc_disable();
	}
}

static void clk_resume_early(void)
{
	u32 val;

	val = readl(RTC_XO_CTRL_REG) & (1 << 31);
	if ((!standby_osc24m_on) || (!!val)) {
		ccu_24mhosc_enable();
		time_mdelay(1);
	}
}

static void clk_resume(void)
{
	/* do not enable pll here */

	/*
	 * set apbs2 clk to OSC24M,
	 * then change the baudrate of uart and twi...
	 */
	twi_clkchangecb(CCU_CLK_CLKCHG_REQ, CCU_HOSC_FREQ);
	uart_clkchangecb(CCU_CLK_CLKCHG_REQ, CCU_HOSC_FREQ);
	writel((readl(APBS2_CFG_REG) & (~APBS2_CLK_SRC_SEL_MASK)) | APBS2_CLK_SRC_SEL(0), APBS2_CFG_REG);
	uart_clkchangecb(CCU_CLK_CLKCHG_DONE, CCU_HOSC_FREQ);
	twi_clkchangecb(CCU_CLK_CLKCHG_DONE, CCU_HOSC_FREQ);

	/* set cpus clk to OSC24M */
	writel((readl(CPUS_CFG_REG) & (~CPUS_CLK_SRC_SEL_MASK)) | CPUS_CLK_SRC_SEL(0), CPUS_CFG_REG);

	/* enable PLLs */
	writel((readl(CCU_PLL_C0_REG) & (~CPUX_PLL_ENABLE_MASK)) | CPUX_PLL_ENABLE(1), CCU_PLL_C0_REG);
	writel((readl(CCU_PLL_DDR0_REG) & (~DDR0_PLL_ENABLE_MASK)) | DDR0_PLL_ENABLE(1), CCU_PLL_DDR0_REG);
	writel((readl(CCU_PLL_PERIPH0_REG) & (~PERIPH0_PLL_ENABLE_MASK)) | PERIPH0_PLL_ENABLE(1), CCU_PLL_PERIPH0_REG);
	writel((readl(CCU_PLL_PERIPH1_REG) & (~PERIPH1_PLL_ENABLE_MASK)) | PERIPH1_PLL_ENABLE(1), CCU_PLL_PERIPH1_REG);

	/* set axi, psi, ahb3, apb1, apb2 clk to OSC24M */
	writel((readl(CCU_PSI_AHB1_AHB2_CFG_REG) & (~PSI_CLK_SRC_SEL_MASK)) | PSI_CLK_SRC_SEL(0), CCU_PSI_AHB1_AHB2_CFG_REG);
	writel((readl(CCU_AHB3_CFG_REG) & (~AHB3_CLK_SRC_SEL_MASK)) | AHB3_CLK_SRC_SEL(0), CCU_AHB3_CFG_REG);
	writel((readl(CCU_APB1_CFG_REG) & (~APB1_CLK_SRC_SEL_MASK)) | APB1_CLK_SRC_SEL(0), CCU_APB1_CFG_REG);
	writel((readl(CCU_APB2_CFG_REG) & (~APB2_CLK_SRC_SEL_MASK)) | APB2_CLK_SRC_SEL(0), CCU_APB2_CFG_REG);
	writel((readl(CCU_CPU_AXI_CFG_REG) & (~CPUX_CLK_SRC_SEL_MASK)) | CPUX_CLK_SRC_SEL(0), CCU_CPU_AXI_CFG_REG);

	clk_restore();
}

static void system_suspend(void)
{
	writel((readl(VDD_SYS_PWR_RST_REG) & (~VDD_SYS_MODULE_RST_MASK)) | VDD_SYS_MODULE_RST(0), VDD_SYS_PWR_RST_REG);

	writel((readl(ANA_PWR_RST_REG) & (~RES_VDD_ON_CTRL_MASK)) | RES_VDD_ON_CTRL(0), ANA_PWR_RST_REG);
	writel((readl(ANA_PWR_RST_REG) & (~AVCC_A_GATING_MASK)) | AVCC_A_GATING(1), ANA_PWR_RST_REG);

	writel((readl(VDD_SYS_PWROFF_GATING_REG) & (~VDD_CPUS_GATING_MASK)) | VDD_CPUS_GATING(1), VDD_SYS_PWROFF_GATING_REG);
}

static void system_resume(void)
{
	writel((readl(VDD_SYS_PWROFF_GATING_REG) & (~VDD_CPUS_GATING_MASK)) | VDD_CPUS_GATING(0), VDD_SYS_PWROFF_GATING_REG);

	writel((readl(ANA_PWR_RST_REG) & (~RES_VDD_ON_CTRL_MASK)) | RES_VDD_ON_CTRL(1), ANA_PWR_RST_REG);
	writel((readl(ANA_PWR_RST_REG) & (~AVCC_A_GATING_MASK)) | AVCC_A_GATING(0), ANA_PWR_RST_REG);

	writel((readl(VDD_SYS_PWR_RST_REG) & (~VDD_SYS_MODULE_RST_MASK)) | VDD_SYS_MODULE_RST(1), VDD_SYS_PWR_RST_REG);
}

static u32 platform_standby_type(void)
{
	u32 type = 0;

	/* usb standby */
	if (interrupt_get_enabled(INTC_USB0_IRQ) ||
			interrupt_get_enabled(INTC_USB1_IRQ)) {
		type |= CPUS_WAKEUP_USB;
	}

	return type;
}

static s32 standby_process_init(struct message *pmessage)
{
	suspend_lock = 1;

	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x1);

	cpucfg_cpu_suspend();
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x2);

	device_suspend();
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x3);

	dram_suspend();
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x4);

	cpucfg_cpu_suspend_late();
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x5);

	clk_suspend();
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x6);

	system_suspend();
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x7);

	dm_suspend();
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x8);

	return OK;
}

static s32 standby_process_exit(struct message *pmessage)
{
	u32 resume_entry = pmessage->paras[1];

	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x1);

	dm_resume();
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x2);

	clk_resume_early();

	system_resume();
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x3);

	clk_resume();
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x4);

	cpucfg_cpu_resume_early(resume_entry);
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x5);

	dram_resume();
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x6);

	device_resume();
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x7);

	cpucfg_cpu_resume(resume_entry);
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x8);

	wait_cpu0_resume();
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x9);

	suspend_lock = 0;

	return OK;
}

/*
*********************************************************************************************************
*                                       ENTEY OF TALK-STANDBY
*
* Description:  the entry of extended super-standby.
*
* Arguments  :  request:request command message.
*
* Returns    :  OK if enter extended super-standby succeeded, others if failed.
*********************************************************************************************************
*/
static s32 standby_entry(struct message *pmessage)
{
	save_state_flag(REC_ESTANDBY | REC_ENTER);

	standby_dts_parse();

	save_state_flag(REC_ESTANDBY | REC_ENTER | 0x1);

	/* backup cpus source clock */
	iosc_freq_init();
	cpus_src = readl(CPUS_CFG_REG) & CPUS_CLK_SRC_SEL_MASK;

	/* parse standby type from enabled interrupt */
	standby_type = platform_standby_type();

	/*
	 * --------------------------------------------------------------------------
	 *
	 * initialize enter super-standby porcess
	 *
	 * --------------------------------------------------------------------------
	 */
	save_state_flag(REC_ESTANDBY | REC_BEFORE_INIT);
	result = standby_process_init(pmessage);
	save_state_flag(REC_ESTANDBY | REC_AFTER_INIT);

	/*
	 * --------------------------------------------------------------------------
	 *
	 * wait valid wakeup source porcess
	 *
	 * --------------------------------------------------------------------------
	 */
	LOG("wait wakeup\n");
	save_state_flag(REC_ESTANDBY | REC_WAIT_WAKEUP);

	wait_wakeup();

	/*
	 * --------------------------------------------------------------------------
	 *
	 * exit super-standby wakeup porcess
	 *
	 * --------------------------------------------------------------------------
	 */
	save_state_flag(REC_ESTANDBY | REC_BEFORE_EXIT);
	standby_process_exit(pmessage);
	save_state_flag(REC_ESTANDBY | REC_AFTER_EXIT);

	/* restore cpus source clock */
	writel((readl(CPUS_CFG_REG) & (~CPUS_CLK_SRC_SEL_MASK)) | cpus_src, CPUS_CFG_REG);

	return OK;
}

u32 is_suspend_lock(void)
{
	return suspend_lock;
}

int cpu_op(struct message *pmessage)
{
	u32 mpidr = pmessage->paras[0];
	u32 entrypoint = pmessage->paras[1];
	u32 cpu_state = pmessage->paras[2];
	u32 cluster_state = pmessage->paras[3]; /* unused variable */
	u32 system_state = pmessage->paras[4];

	LOG("mpidr:%x, entrypoint:%x; cpu_state:%x, cluster_state:%x, system_state:%x\n", mpidr, entrypoint, cpu_state, cluster_state, system_state);
	if (cpu_state == arisc_power_on) {
		set_secondary_entry(entrypoint, mpidr);
		cpu_power_up(0, mpidr);
	} else if (cpu_state == arisc_power_off) {
		if (entrypoint) {
			if (system_state == arisc_power_off) {
				standby_entry(pmessage);
			} else if (cluster_state == arisc_power_off) {

			} else {

			}
		} else {
			cpu_power_down(0, mpidr);
		}
	}

	return 0;
}

static void system_shutdown(void)
{
	pmu_charging_reset();

	iosc_freq_init();

	/* set cpus clk to RC16M */
	writel((readl(CPUS_CFG_REG) & (~CPUS_CLK_SRC_SEL_MASK)) | CPUS_CLK_SRC_SEL(2), CPUS_CFG_REG);

	/*
	 * set apbs2 clk to RC16M,
	 * then change the baudrate of uart and twi...
	 */
	twi_clkchangecb(CCU_CLK_CLKCHG_REQ, iosc_freq);
	uart_clkchangecb(CCU_CLK_CLKCHG_REQ, iosc_freq);
	writel((readl(APBS2_CFG_REG) & (~APBS2_CLK_SRC_SEL_MASK)) | APBS2_CLK_SRC_SEL(2), APBS2_CFG_REG);
	uart_clkchangecb(CCU_CLK_CLKCHG_DONE, iosc_freq);
	twi_clkchangecb(CCU_CLK_CLKCHG_DONE, iosc_freq);
	time_mdelay(10);

	ccu_24mhosc_disable();

	pmu_shutdown();
}

static void system_reset(void)
{
	pmu_reset();
}

int sys_op(struct message *pmessage)
{
	u32 state = pmessage->paras[0];

	LOG("state:%x\n", state);

	switch (state) {
	case arisc_system_shutdown:
		{
			save_state_flag(REC_SHUTDOWN | 0x101);
			system_shutdown();
			break;
		}
	case arisc_system_reset:
	case arisc_system_reboot:
		{
			save_state_flag(REC_SHUTDOWN | 0x102);
			system_reset();
			break;
		}
	default:
		{
			WRN("invaid system power state (%d)\n", state);
			return -EINVAL;
		}
	}

	return 0;
}


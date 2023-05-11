/*
 *********************************************************************************************************
 *                                                AR100 SYSTEM
 *                                     AR100 Software System Develop Kits
 *
 *
 *                                    (c) Copyright 2018-2024, Luojie China
 *                                             All Rights Reserved
 *
 * File    : cpucfg_regs.h
 * By      : LuoJie
 * Version : v1.0
 * Date    : 2018-06-05 16:31
 * Descript: .
 * Update  :auther      ver     notes
 *          Luojie      1.0     Create this file.
 *********************************************************************************************************
 */

#ifndef __CPUCFG_REGS_H__
#define __CPUCFG_REGS_H__

#include "lib_inc.h"
/* CPUSCFG REGS */
#define CPUS_RST_CTRL_REG       (R_CPUCFG_REG_BASE + 0x0000)
#define CPUS_SPACE_LE_CTRL_REG  (R_CPUCFG_REG_BASE + 0x000C)
#define CPUS_SPACE_START_REG(n) (R_CPUCFG_REG_BASE + 0x0010 + n * 8)
#define CPUS_SPACE_END_REG(n)   (R_CPUCFG_REG_BASE + 0x0014 + n * 8)

#define C0_CPUX_RESET_REG            (R_CPUCFG_REG_BASE + 0x0040)
#define C0_CPUX_POWEROFF_GATING_REG  (R_CPUCFG_REG_BASE + 0x0044)
#define CPU0_HOTPLUG_FLAG_REG        (R_CPUCFG_REG_BASE + 0x01C0)

#define IOSC_CLK_AUTO_CALI	(RTC_REG_BASE + 0x000C)
#define XO_CTRL			(RTC_REG_BASE + 0x0160)
#define SUPER_STANDBY_FLAG_REG  (RTC_REG_BASE + 0x01F8)
#define PRIVATE_REG0            (RTC_REG_BASE + 0x01BC)
#define PRIVATE_REG1            (RTC_REG_BASE + 0x01FC)
#define BOOT_CPU_HOTPLUG_REG    (RTC_REG_BASE + 0x01B8)

#define CNT_LOW_REG             (TS_STAT_REG_BASE + 0x0000)
#define CNT_HIGH_REG            (TS_STAT_REG_BASE + 0x0004)
#define CNT_CTRL_REG            (TS_CTRL_REG_BASE + 0x0000)
#define CNT_LOW_REG_SET         (TS_CTRL_REG_BASE + 0x0008)
#define CNT_HIGH_REG_SET        (TS_CTRL_REG_BASE + 0x000C)
#define CNT_FREQID_REG          (TS_CTRL_REG_BASE + 0x0020)

/* CPUXCFG REGS */
#define C0_RST_CTRL_REG         (CPUCFG_REG_BASE + 0x0000)
#define C0_CTRL_REG0            (CPUCFG_REG_BASE + 0x0010)
#define C0_CTRL_REG1            (CPUCFG_REG_BASE + 0x0014)
#define C0_CTRL_REG2            (CPUCFG_REG_BASE + 0x0018)
#define CACHE_CFG_REG           (CPUCFG_REG_BASE + 0x0024)
#define SUNXI_CPU_RVBA_L(cpu)   (CPUCFG_REG_BASE + 0x40 + (cpu)*0x8)
#define SUNXI_CPU_RVBA_H(cpu)   (CPUCFG_REG_BASE + 0x44 + (cpu)*0x8)
#define C0_CPU_STATUS_REG       (CPUCFG_REG_BASE + 0x0080)
#define L2_STATUS_REG           (CPUCFG_REG_BASE + 0x0084)

#define SUNXI_DBG_REG0          (CPUCFG_REG_BASE + 0xC0)
#define SUNXI_DBG_REG1          (CPUCFG_REG_BASE + 0xC4)

#define GENER_CTRL_REG0         (CPUSUBSYS_REG_BASE + 0x0000)
#define GENER_CTRL_REG1         (CPUSUBSYS_REG_BASE + 0x0004)
#define GIC_JTAG_RST_CTRL_REG   (CPUSUBSYS_REG_BASE + 0x000C)
#define C0_INT_EN_REG           (CPUSUBSYS_REG_BASE + 0x0010)
#define IRQ_FIQ_STATUS_REG      (CPUSUBSYS_REG_BASE + 0x0014)
#define C0_DBG_STATE            (CPUSUBSYS_REG_BASE + 0x001C)

#define SUNXI_CLUSTER_PWROFF_GATING(cluster)      (R_CPUCFG_REG_BASE + 0x44 + (cluster<<2))
#define SUNXI_CPU_PWR_CLAMP(cluster, cpu)         (R_CPUCFG_REG_BASE + 0x50 + (cluster<<4) + (cpu<<2))

#define SUNXI_SOFT_ENTRY			  (RTC_REG_BASE + 0x1bc)
#define SUNXI_BOOT_CPU_HOTPLUG			  (CPU0_HOTPLUG_FLAG_REG)
#define SUNXI_F1F2_FLAG				  (R_CPUCFG_REG_BASE + 0x104)
#define SUNXI_IDLE_STATE			  (R_CPUCFG_REG_BASE + 0x110)
#define SUNXI_F1F2_CONFIG_DELAY			  (R_CPUCFG_REG_BASE + 0x144)
#define SUNXI_PWR_SW_DELAY			  (R_CPUCFG_REG_BASE + 0x140)
#define SUNXI_CPUIDLE				  (R_CPUCFG_REG_BASE + 0x100)
#define SUNXI_IRQ_FIQ_MASK			  (R_CPUCFG_REG_BASE + 0x10c)
#define SUNXI_IDLE_IRQ_FIQ_STATE		  (R_CPUCFG_REG_BASE + 0x108)

#define SUNXI_CLUSTER_CTRL0(cluster)              (CPUCFG_REG_BASE + 0x10 + (cluster<<4))
#define SUNXI_CLUSTER_CTRL1(cluster)              (CPUCFG_REG_BASE + 0x14 + (cluster<<4))
#define SUNXI_CLUSTER_CTRL2(cluster)              (CPUCFG_REG_BASE + 0x18 + (cluster<<4))
#define SUNXI_CLUSTER_CPU_STATUS(cluster)         (CPUCFG_REG_BASE + 0x80 + (cluster<<2))
#define SUNXI_CPU_RST_CTRL(cluster)               (CPUCFG_REG_BASE + 0x00 + (cluster<<2))

#define SUNXI_CLUSTER_PWRON_RESET(cluster)        (R_CPUCFG_REG_BASE + 0x40  + (cluster<<2))
#define SUNXI_CPU_SYS_RESET                       (R_CPUCFG_REG_BASE + 0xA0)

#define CLUSTER0    0
#define CLUSTER1    1

#define DCXO_CALI_INTEGER_OFFSET 20
#define DCXO_CALI_DECIMAL_OFFSET 4


typedef enum cpucfg_cpu_reset_ctrl
{
	CPUCFG_C0_CPU0    = 0x0,
	CPUCFG_C0_CPU1    = 0x1,
	CPUCFG_C0_CPU2    = 0x2,
	CPUCFG_C0_CPU3    = 0x3,
	CPUCFG_CPU_NUMBER = 0x4,
} cpucfg_cpu_reset_ctrl_e;
#define IS_CPU0_WFI() (readl(0x09010000 + 0x80) & (0x1 << 16))

#ifdef FPGA_PLATFORM
#define L2CACHE_IS_WFI_MODE(cluster)  (1)
#define CPU_IS_WFI_MODE(cluster, cpu) (1)
#else
#define L2CACHE_IS_WFI_MODE(cluster)  (readl(SUNXI_CLUSTER_CPU_STATUS(cluster)) & (1 << 0))
#define CPU_IS_WFI_MODE(cluster, cpu) (readl(SUNXI_CLUSTER_CPU_STATUS(cluster)) & (1 << (16 + cpu)))
#endif

#define CPUCFG_SUPER_STANDBY_FLAG   (0xefe8)

//cpus little-endian space number
#define CPUCFG_LE_SPACE_NUMBER      (4)

//the reset status of cpu
typedef enum cpu_reset_status
{
	CPU_RESET_ASSERT    = 0x0,
	CPU_RESET_DEASSERT  = 0x3,
} cpu_reset_status_e;

//the status of cluster0
typedef enum cluster_reset_status
{
	CLUSTER_RESET_ASSERT    = 0x0,
	CLUSTER_RESET_DEASSERT  = 0x1,
} cluster_reset_status_e;

typedef enum acinactm_status
{
	ACINACTM_LOW    = 0x0,
	ACINACTM_HIGH   = 0x1,
} acinactm_status_e;

//the hardware power status of module
typedef enum power_status
{
	POWER_STATUS_MUST_POWER_ON      = 0x0,
	POWER_STATUS_CAN_ENTER_DORMANT  = 0x2,
	POWER_STATUS_CAN_ENTER_POWEROFF = 0x3,
	POWER_STATUS_INVALID                 ,
} power_status_e;

//L1 & L2 reset by hardware status
typedef enum l1l2_reset_by_hardware_status
{
	L1_EN_L2_EN     = 0x0,
	L1_DIS_L2_EN    = 0xf,
	L1_EN_L2_DIS    = 0x10,
	L1_DIS_L2_DIS   = 0x1f
} l1l2_reset_by_hardware_status_e;


#endif  /* __CPUCFG_SUN50IW6_REGS_H__ */

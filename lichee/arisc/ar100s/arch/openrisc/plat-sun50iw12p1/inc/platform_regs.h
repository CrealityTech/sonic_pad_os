/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                 io module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : io.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-18
* Descript: io module public header.
* Update  : date                auther      ver     notes
*           2012-5-18 10:04:21  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __REG_BASE_H__
#define __REG_BASE_H__

#define CCU_REG_BASE            (0x02001000)
#define HWMSGBOX_REG_BASE       (0x03003000)
#define RTC_REG_BASE            (0x07090000)
#define R_CPUCFG_REG_BASE       (0x07000400)
#define R_PRCM_REG_BASE         (0x07010000)
#define R_TMR01_REG_BASE        (0x07090400)
#define R_WDOG_REG_BASE         (0x07020400)
#define R_INTC_REG_BASE         (0x07021000)
#define R_PIO_REG_BASE          (0x07022000)
#define R_UART_REG_BASE         (0x07080000)
#define R_TWI_REG_BASE          (0x07081400)
#define CPUSUBSYS_REG_BASE      (0x08100000)
#define TS_STAT_REG_BASE        (0x08110000)
#define TS_CTRL_REG_BASE        (0x08120000)
#define CPUCFG_REG_BASE         (0x09010000)

//rtc domain record reg
#define RTC_RECORD_REG   (RTC_REG_BASE + 0x10c)

/*
 *  device physical addresses  \\not use now
 */
#define SUNXI_SID_PBASE                  0x03006000
#define SUNXI_SMC_PBASE                  0x04800000

//prcm regs
#define CPUS_CFG_REG                (R_PRCM_REG_BASE + 0x000)
#define APBS2_CFG_REG               (R_PRCM_REG_BASE + 0x010)
#define R_TIMER_BUS_GATE_RST_REG    (R_PRCM_REG_BASE + 0x11c)
#define R_PWM_BUS_GATE_RST_REG      (R_PRCM_REG_BASE + 0x13c)
#define R_UART_BUS_GATE_RST_REG     (R_PRCM_REG_BASE + 0x18c)
#define R_TWI_BUS_GATE_RST_REG      (R_PRCM_REG_BASE + 0x19c)
#define R_IR_RX_BUS_GATE_RST_REG    (R_PRCM_REG_BASE + 0x1cc)
#define RTC_BUS_GATE_RST_REG        (R_PRCM_REG_BASE + 0x20c)
#define CCU_PLL_CTRL1               (R_PRCM_REG_BASE + 0x244)
#define VDD_SYS_PWROFF_GATING_REG   (R_PRCM_REG_BASE + 0x250)
#define ANA_PWR_RST_REG             (R_PRCM_REG_BASE + 0x254)
#define VDD_SYS_PWR_RST_REG         (R_PRCM_REG_BASE + 0x260)
#define LP_CTRL_REG                 (R_PRCM_REG_BASE + 0x33c)

/* APBS1_CFG_REG */
#define APBS2_CLK_SRC_SEL_SHIFT (24)
#define APBS2_CLK_DIV_RATIO_N_SHIFT (8)
#define APBS2_FACTOR_M_SHIFT (0)

#define APBS2_CLK_SRC_SEL_MASK (0x7 << APBS2_CLK_SRC_SEL_SHIFT)
#define APBS2_CLK_DIV_RATIO_N_MASK (0x3 << APBS2_CLK_DIV_RATIO_N_SHIFT)
#define APBS2_FACTOR_M_MASK (0x1f << APBS2_FACTOR_M_SHIFT)

#define APBS2_CLK_SRC_SEL(n) ((n) << APBS2_CLK_SRC_SEL_SHIFT)
#define APBS2_CLK_DIV_RATIO_N(n) ((n) << APBS2_CLK_DIV_RATIO_N_SHIFT)
#define APBS2_FACTOR_M(n) ((n) << APBS2_FACTOR_M_SHIFT)

/* CPUS_CFG_REG */
#define CPUS_CLK_SRC_SEL_SHIFT (24)
#define CPUS_CLK_DIV_N_SHIFT (8)
#define CPUS_FACTOR_M_SHIFT (0)

#define CPUS_CLK_SRC_SEL_MASK (0x7 << CPUS_CLK_SRC_SEL_SHIFT)
#define CPUS_CLK_DIV_N_MASK (0x3 << CPUS_CLK_DIV_N_SHIFT)
#define CPUS_FACTOR_M_MASK (0x1f << CPUS_FACTOR_M_SHIFT)

#define CPUS_CLK_SRC_SEL(n) ((n) << CPUS_CLK_SRC_SEL_SHIFT)
#define CPUS_CLK_DIV_N(n) ((n) << CPUS_CLK_DIV_N_SHIFT)
#define CPUS_FACTOR_M(n) ((n) << CPUS_FACTOR_M_SHIFT)

//name by pll order
#define CCU_PLLx_REG(n)             (CCU_REG_BASE + (0x8 * (n - 1)))

//name by pll function
#define CCU_PLL_C0_REG              (CCU_REG_BASE + 0x000)
#define CCU_PLL_DDR0_REG            (CCU_REG_BASE + 0X010)
#define CCU_PLL_PERIPH0_REG         (CCU_REG_BASE + 0x020)
#define CCU_PLL_PERIPH1_REG         (CCU_REG_BASE + 0x028)
#define CCU_CPU_AXI_CFG_REG         (CCU_REG_BASE + 0x500)
#define CCU_MSGBOX_BGR_REG          (CCU_REG_BASE + 0x71c)
#define CCU_SPINLOCK_BGR_REG        (CCU_REG_BASE + 0x72c)
#define CCU_USB0_CLOCK_REG          (CCU_REG_BASE + 0xA70)
#define CCU_USB_BUS_GATING_RST_REG  (CCU_REG_BASE + 0xA8C)


/*smc config info*/
#define SMC_ACTION_REG                (SUNXI_SMC_PBASE + 0x0004)
#define SMC_MST0_BYP_REG              (SUNXI_SMC_PBASE + 0x0070)
#define SMC_MST1_BYP_REG              (SUNXI_SMC_PBASE + 0x0074)

#define SMC_MST0_SEC_REG              (SUNXI_SMC_PBASE + 0x0080)
#define SMC_MST1_SEC_REG              (SUNXI_SMC_PBASE + 0x0084)

#define SMC_REGION_COUNT (8) /*total 16,not all used, save some space*/
#define SMC_REGIN_SETUP_LOW_REG(x)    (SUNXI_SMC_PBASE + 0x100 + 0x10*(x))
#define SMC_REGIN_SETUP_HIGH_REG(x)   (SUNXI_SMC_PBASE + 0x104 + 0x10*(x))
#define SMC_REGIN_ATTRIBUTE_REG(x)    (SUNXI_SMC_PBASE + 0x108 + 0x10*(x))

/*SID*/
#define SID_SEC_MODE_STA              (SUNXI_SID_PBASE + 0xA0)
#define SID_SEC_MODE_MASK             (0x1)

/* CCU_USB_BUS_GATING_RST_REG */
#define PLL_NUM     (14)
#define BUS_NUM     (10)
#define IO_NUM      (2)

#define CCU_HOSC_FREQ               (24000000)	//24M
#define CCU_LOSC_FREQ               (31250)	//31250
#define CCU_CPUS_PLL0_FREQ          (200000000)
#define CCU_APBS2_PLL0_FREQ         (200000000)
#define CCU_IOSC_FREQ               (16000000)	//16M
#define CCU_CPUS_POST_DIV           (100000000)	//cpus post div source clock freq
#define CCU_PERIPH0_FREQ            (600000000)	//600M

#endif

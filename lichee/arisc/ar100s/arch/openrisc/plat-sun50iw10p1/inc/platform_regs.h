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

#define CCU_REG_BASE            (0x03001000)
#define HWMSGBOX_REG_BASE       (0x03003000)
#define HWSPINLOCK_REG_BASE     (0x03004000)
#define CPUX_TIMER_REG_BASE     (0x03009000)
#define CPUX_PIO_REG_BASE       (0x0300B000)
#define RTC_REG_BASE            (0x07000000)
#define R_CPUCFG_REG_BASE       (0x07000400)
#define R_PRCM_REG_BASE         (0x07010000)
#define R_TMR01_REG_BASE        (0x07020000)
#define R_WDOG_REG_BASE         (0x07020400)
#define R_TWD_REG_BASE          (0x07020800)
#define R_PWM_REG_BASE          (0x07020C00)
#define R_INTC_REG_BASE         (0x07021000)
#define R_PIO_REG_BASE          (0x07022000)
#define R_CIR_REG_BASE          (0x07040000)
#define R_OWC_REG_BASE          (0x07040400)
#define R_UART_REG_BASE         (0x07080000)
#define R_TWI_REG_BASE          (0x07081400)
#define R_RSB_REG_BASE          (0x07083000)
#define CPUSUBSYS_REG_BASE      (0x08100000)
#define TS_STAT_REG_BASE        (0x08110000)
#define TS_CTRL_REG_BASE        (0x08120000)
#define CPUCFG_REG_BASE         (0x09010000)

/*
 * memory definitions
 */
#define SUNXI_IO_PBASE                   0x01000000
#define SUNXI_IO_SIZE                    0x01000000
#define SUNXI_BROM0_N_PBASE              0xffff0000
#define SUNXI_BROM0_N_SIZE               0x00008000
#define SUNXI_BROM1_S_PBASE              0xffff0000
#define SUNXI_BROM1_S_SIZE               0x00010000
#define SUNXI_SRAM_A1_PBASE              0x00000000
#define SUNXI_SRAM_A1_SIZE               0x00008000
#define SUNXI_SRAM_A2_PBASE              0x00040000
#define SUNXI_SRAM_A2_SIZE               0x00014000
#define SUNXI_SRAM_B_PBASE               0x00020000
#define SUNXI_SRAM_B_SIZE                0x00018000
#define SUNXI_SDRAM_PBASE                0x40000000

/*
 *  device physical addresses
 */

#define SUNXI_CPUXCFG_SIZE               0x00000400
#define SUNXI_TIMESTAMP_STA_PBASE        0x08110000
#define SUNXI_TIMESTAMP_STA_SIZE         0x00001000
#define SUNXI_TIMESTAMP_CTRL_PBASE       0x08120000
#define SUNXI_TIMESTAMP_CTRL_SIZE        0x00001000
#define SUNXI_CCI400_PBASE               0x01790000

#define SUNXI_SRAMCTRL_PBASE             0x03000000
#define SUNXI_DMA_PBASE                  0x01c02000
#define SUNXI_NFC0_PBASE                 0x01c03000
#define SUNXI_KEYMEM_PBASE               0x01c0b000
#define SUNXI_LCD0_PBASE                 0x01c0c000
#define SUNXI_LCD1_PBASE                 0x01c0d000
#define SUNXI_VE_PBASE                   0x01c0e000
#define SUNXI_SDMMC0_PBASE               0x01c0f000
#define SUNXI_SDMMC1_PBASE               0x01c10000
#define SUNXI_SDMMC2_PBASE               0x01c11000
#define SUNXI_SID_PBASE                  0x03006000
#define SUNXI_SS_PBASE                   0x01c15000
#define SUNXI_MSGBOX_PBASE               0x01c17000
#define SUNXI_SPINLOCK_PBASE             0x01c18000
#define SUNXI_SMC_PBASE                  0x04800000
#define SUNXI_CCM_PBASE                  0x03001000
#define SUNXI_CCM_PEND                   0x03001ff0
#define SUNXI_PIO_PBASE                  0x0300b000
#define SUNXI_TIMER_PBASE                0x01c20c00
#define SUNXI_OWA_PBASE                  0x01c21000
#define SUNXI_PWM_PBASE                  0x01c21400
#define SUNXI_DAUDIO0_PBASE              0x01c22000
#define SUNXI_DAUDIO1_PBASE              0x01c22400
#define SUNXI_DAUDIO2_PBASE              0x01c22800
#define SUNXI_TDM_PBASE                  0x01c23000
#define SUNXI_SMTA_PBASE                 0x03008000
#define SUNXI_MIPI_DSI0_PBASE            0x01c26000
#define SUNXI_UART0_PBASE                0x01c28000
#define SUNXI_UART1_PBASE                0x01c28400
#define SUNXI_UART2_PBASE                0x01c28800
#define SUNXI_UART3_PBASE                0x01c28c00
#define SUNXI_UART4_PBASE                0x01c29000
#define SUNXI_TWI0_PBASE                 0x01c2ac00
#define SUNXI_TWI1_PBASE                 0x01c2b000
#define SUNXI_TWI2_PBASE                 0x01c2b400
#define SUNXI_GMAC_PBASE                 0x01c30000
#define SUNXI_GPU_PBASE                  0x01c40000
#define SUNXI_HSTMR_PBASE                0x01c60000
#define SUNXI_DRAMCOM_PBASE              0x01c62000
#define SUNXI_DRAMCTL0_PBASE             0x01c63000
#define SUNXI_DRAMPHY0_PBASE             0x01c65000
#define SUNXI_SPI0_PBASE                 0x01c68000
#define SUNXI_SPI1_PBASE                 0x01c69000
#define SUNXI_GIC_PBASE                  0x01c80000
#define SUNXI_GIC_DIST_PBASE             0x01c81000
#define SUNXI_GIC_CPU_PBASE              0x01c82000
#define SUNXI_CSI_PBASE                  0x01cb0000
#define SUNXI_VEMEM_PBASE                0x01d00000
#define SUNXI_HDMI_PBASE                 0x01ee0000
#define SUNXI_R_TIMER_PBASE              0x01f00800
#define SUNXI_R_INTC_PBASE               0x07022000
#define SUNXI_R_WDOG_PBASE               0x01f01000
#define SUNXI_R_PRCM_PBASE               0x01f01400
#define SUNXI_R_TWD_PBASE                0x01f01800
#define SUNXI_R_CPUCFG_PBASE             0x01f01c00
#define SUNXI_CIR_TX_PBASE               0x01f02000
#define SUNXI_R_TWI_PBASE                0x01f02400
#define SUNXI_R_UART_PBASE               0x01f02800
#define SUNXI_R_PIO_PBASE                0x01f02c00
#define SUNXI_R_RSB_PBASE                0x01f03400
#define SUNXI_R_PWM_PBASE                0x01f03800
#define SUNXI_LRADC_PBASE                0x01f03c00
#define SUNXI_R_TH_PBASE                 0x01f04000

//rtc domian regs
#define RTC_DM_REG0 (RTC_REG_BASE + 0x100 + (0x4 * 0))
#define RTC_DM_REG1 (RTC_REG_BASE + 0x100 + (0x4 * 1))
#define RTC_DM_REG2 (RTC_REG_BASE + 0x100 + (0x4 * 2))
#define RTC_DM_REG3 (RTC_REG_BASE + 0x100 + (0x4 * 3))
#define RTC_DM_REG4 (RTC_REG_BASE + 0x100 + (0x4 * 4))
#define RTC_DM_REG5 (RTC_REG_BASE + 0x100 + (0x4 * 5))
#define RTC_DM_REG6 (RTC_REG_BASE + 0x100 + (0x4 * 6))

//rtc domain record reg
#define RTC_RECORD_REG   (RTC_REG_BASE + 0x10c)
#define START_OS_REG     (RTC_DM_REG2)

#define RTC_XO_CTRL_REG			(RTC_REG_BASE + 0x160)

//prcm regs
#define CPUS_CFG_REG                (R_PRCM_REG_BASE + 0x000)
#define APBS2_CFG_REG               (R_PRCM_REG_BASE + 0x010)
#define CCU_PLL_CTRL1               (R_PRCM_REG_BASE + 0x244)
#define VDD_SYS_PWROFF_GATING_REG   (R_PRCM_REG_BASE + 0x250)
#define ANA_PWR_RST_REG             (R_PRCM_REG_BASE + 0x254)
#define VDD_SYS_PWR_RST_REG         (R_PRCM_REG_BASE + 0x260)
#define LP_CTRL_REG                 (R_PRCM_REG_BASE + 0x33c)

/* VERSION REG */
#define CPU_REG_LAYOUT_VERSION_REG  (SUNXI_SRAMCTRL_PBASE + 0x0024)

/* APBS2_CFG_REG */
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

/* VDD_SYS_PWROFF_GATING_REG */
#define VDD_USB2CPUS_GATING_SHIFT (8)
#define VDD_SYS2USB_GATING_SHIFT (3)
#define VDD_CPUS_GATING_SHIFT (2)

#define VDD_USB2CPUS_GATING_MASK (0x1 << VDD_USB2CPUS_GATING_SHIFT)
#define VDD_SYS2USB_GATING_MASK (0x1 << VDD_SYS2USB_GATING_SHIFT)
#define VDD_CPUS_GATING_MASK (0x1 << VDD_CPUS_GATING_SHIFT)

#define VDD_USB2CPUS_GATING(n) ((n) << VDD_USB2CPUS_GATING_SHIFT)
#define VDD_SYS2USB_GATING(n) ((n) << VDD_SYS2USB_GATING_SHIFT)
#define VDD_CPUS_GATING(n) ((n) << VDD_CPUS_GATING_SHIFT)

/* ANA_PWR_RST_REG */
#define AVCC_A_GATING_SHIFT (0)
#define RES_VDD_ON_CTRL_SHIFT (1)

#define AVCC_A_GATING_MASK (0x1 << AVCC_A_GATING_SHIFT)
#define RES_VDD_ON_CTRL_MASK (0x1 << RES_VDD_ON_CTRL_SHIFT)

#define AVCC_A_GATING(n) ((n) << AVCC_A_GATING_SHIFT)
#define RES_VDD_ON_CTRL(n) ((n) << RES_VDD_ON_CTRL_SHIFT)

/* VDD_SYS_PWR_RST_REG */
#define VDD_SYS_MODULE_RST_SHIFT (0)

#define VDD_SYS_MODULE_RST_MASK (0x1 << VDD_SYS_MODULE_RST_SHIFT)

#define VDD_SYS_MODULE_RST(n) ((n) << VDD_SYS_MODULE_RST_SHIFT)

//name by pll order
#define CCU_PLLx_REG(n)             (CCU_REG_BASE + (0x8 * (n - 1)))

//name by pll function
#define CCU_PLL_C0_REG              (CCU_REG_BASE + 0x000)
#define CCU_PLL_DDR0_REG            (CCU_REG_BASE + 0x010)
#define CCU_PLL_DDR1_REG            (CCU_PLLx_REG(4))
#define CCU_PLL_PERIPH0_REG         (CCU_REG_BASE + 0x020)
#define CCU_PLL_PERIPH1_REG         (CCU_REG_BASE + 0x028)
#define CCU_PLL_GPU0_REG            (CCU_PLLx_REG(7))
#define CCU_PLL_VIDEO0_REG          (CCU_PLLx_REG(9))
#define CCU_PLL_VIDEO1_REG          (CCU_PLLx_REG(10))
#define CCU_PLL_VE_REG              (CCU_PLLx_REG(12))
#define CCU_PLL_DE_REG              (CCU_PLLx_REG(13))
#define CCU_PLL_HSIC_REG            (CCU_PLLx_REG(15))
#define CCU_PLL_AUDIO_REG           (CCU_PLLx_REG(16))
#define CCU_CPU_AXI_CFG_REG         (CCU_REG_BASE + 0x500)
#define CCU_PSI_AHB1_AHB2_CFG_REG   (CCU_REG_BASE + 0x510)
#define CCU_AHB3_CFG_REG            (CCU_REG_BASE + 0x51c)
#define CCU_APB1_CFG_REG            (CCU_REG_BASE + 0x520)
#define CCU_APB2_CFG_REG            (CCU_REG_BASE + 0x524)
#define CCU_CCI_CFG_REG             (CCU_REG_BASE + 0x530)
#define CCU_MBUS_CLK_REG            (CCU_REG_BASE + 0x540)
#define CCU_MSGBOX_BGR_REG          (CCU_REG_BASE + 0x71c)
#define CCU_SPINLOCK_BGR_REG        (CCU_REG_BASE + 0x72c)
#define CCU_USB0_CLOCK_REG          (CCU_REG_BASE + 0xA70)
#define CCU_USB1_CLOCK_REG          (CCU_REG_BASE + 0xA74)
#define CCU_USB_BUS_GATING_RST_REG  (CCU_REG_BASE + 0xA8C)

#define USB0_BASE	0x05101000
#define USB_PHY		(USB0_BASE + 0x800)

#define USB1_BASE	0x05200000
#define USB_CTRL1	(USB1_BASE + 0x800)
#define USB_CTRL2	(USB1_BASE + 0x808)
#define PHY_CTRL	(USB1_BASE + 0x810)
/* CCU_PLL_C0_REG */
#define CPUX_PLL_ENABLE_SHIFT (31)

#define CPUX_PLL_ENABLE_MASK (0x1 << CPUX_PLL_ENABLE_SHIFT)

#define CPUX_PLL_ENABLE(n) ((n) << CPUX_PLL_ENABLE_SHIFT)

/* CCU_PLL_DDR0_REG */
#define DDR0_PLL_ENABLE_SHIFT (31)

#define DDR0_PLL_ENABLE_MASK (0x1 << DDR0_PLL_ENABLE_SHIFT)

#define DDR0_PLL_ENABLE(n) ((n) << DDR0_PLL_ENABLE_SHIFT)

/* CCU_PLL_PERIPH0_REG */
#define PERIPH0_PLL_ENABLE_SHIFT (31)

#define PERIPH0_PLL_ENABLE_MASK (0x1 << PERIPH0_PLL_ENABLE_SHIFT)

#define PERIPH0_PLL_ENABLE(n) ((n) << PERIPH0_PLL_ENABLE_SHIFT)

/* CCU_PLL_PERIPH1_REG */
#define PERIPH1_PLL_ENABLE_SHIFT (31)

#define PERIPH1_PLL_ENABLE_MASK (0x1 << PERIPH1_PLL_ENABLE_SHIFT)

#define PERIPH1_PLL_ENABLE(n) ((n) << PERIPH1_PLL_ENABLE_SHIFT)

/* CCU_CPU_AXI_CFG_REG */
#define CPUX_CLK_SRC_SEL_SHIFT (24)
#define CPUX_APB_FACTOR_N_SHIFT (8)
#define CPUX_AXI_FACTOR_M_SHIFT (0)

#define CPUX_CLK_SRC_SEL_MASK (0x7 << CPUX_CLK_SRC_SEL_SHIFT)
#define CPUX_APB_FACTOR_N_MASK (0x3 << CPUX_APB_FACTOR_N_SHIFT)
#define CPUX_AXI_FACTOR_M_MASK (0x3 << CPUX_AXI_FACTOR_M_SHIFT)

#define CPUX_CLK_SRC_SEL(n) ((n) << CPUX_CLK_SRC_SEL_SHIFT)
#define CPUX_APB_FACTOR_N(n) ((n) << CPUX_APB_FACTOR_N_SHIFT)
#define CPUX_AXI_FACTOR_M(n) ((n) << CPUX_AXI_FACTOR_M_SHIFT)

/* CCU_PSI_AHB1_AHB2_CFG_REG */
#define PSI_CLK_SRC_SEL_SHIFT (24)
#define PSI_FACTOR_N_SHIFT (8)
#define PSI_FACTOR_M_SHIFT (0)

#define PSI_CLK_SRC_SEL_MASK (0x7 << PSI_CLK_SRC_SEL_SHIFT)
#define PSI_FACTOR_N_MASK (0x3 << PSI_FACTOR_N_SHIFT)
#define PSI_FACTOR_M_MASK (0x3 << PSI_FACTOR_M_SHIFT)

#define PSI_CLK_SRC_SEL(n) ((n) << PSI_CLK_SRC_SEL_SHIFT)
#define PSI_FACTOR_N(n) ((n) << PSI_FACTOR_N_SHIFT)
#define PSI_FACTOR_M(n) ((n) << PSI_FACTOR_M_SHIFT)

/* CCU_AHB3_CFG_REG */
#define AHB3_CLK_SRC_SEL_SHIFT (24)
#define AHB3_FACTOR_N_SHIFT (8)
#define AHB3_FACTOR_M_SHIFT (0)

#define AHB3_CLK_SRC_SEL_MASK (0x7 << AHB3_CLK_SRC_SEL_SHIFT)
#define AHB3_FACTOR_N_MASK (0x3 << AHB3_FACTOR_N_SHIFT)
#define AHB3_FACTOR_M_MASK (0x3 << AHB3_FACTOR_M_SHIFT)

#define AHB3_CLK_SRC_SEL(n) ((n) << AHB3_CLK_SRC_SEL_SHIFT)
#define AHB3_FACTOR_N(n) ((n) << AHB3_FACTOR_N_SHIFT)
#define AHB3_FACTOR_M(n) ((n) << AHB3_FACTOR_M_SHIFT)

/* CCU_APB1_CFG_REG */
#define APB1_CLK_SRC_SEL_SHIFT (24)
#define APB1_FACTOR_N_SHIFT (8)
#define APB1_FACTOR_M_SHIFT (0)

#define APB1_CLK_SRC_SEL_MASK (0x7 << APB1_CLK_SRC_SEL_SHIFT)
#define APB1_FACTOR_N_MASK (0x3 << APB1_FACTOR_N_SHIFT)
#define APB1_FACTOR_M_MASK (0x3 << APB1_FACTOR_M_SHIFT)

#define APB1_CLK_SRC_SEL(n) ((n) << APB1_CLK_SRC_SEL_SHIFT)
#define APB1_FACTOR_N(n) ((n) << APB1_FACTOR_N_SHIFT)
#define APB1_FACTOR_M(n) ((n) << APB1_FACTOR_M_SHIFT)

/* CCU_APB2_CFG_REG */
#define APB2_CLK_SRC_SEL_SHIFT (24)
#define APB2_FACTOR_N_SHIFT (8)
#define APB2_FACTOR_M_SHIFT (0)

#define APB2_CLK_SRC_SEL_MASK (0x7 << APB2_CLK_SRC_SEL_SHIFT)
#define APB2_FACTOR_N_MASK (0x3 << APB2_FACTOR_N_SHIFT)
#define APB2_FACTOR_M_MASK (0x3 << APB2_FACTOR_M_SHIFT)

#define APB2_CLK_SRC_SEL(n) ((n) << APB2_CLK_SRC_SEL_SHIFT)
#define APB2_FACTOR_N(n) ((n) << APB2_FACTOR_N_SHIFT)
#define APB2_FACTOR_M(n) ((n) << APB2_FACTOR_M_SHIFT)

/* CCU_PLL_C0_REG */
#define C0_PLL_FACTOR_M_SHIFT			(0)
#define C0_PLL_FACTOR_N_SHIFT			(8)
#define C0_PLL_OUT_EXT_DIVP_SHIFT		(16)
#define C0_PLL_OUTPUT_ENABLE_SHIFT		(27)
#define C0_PLL_LOCK_STATUS_SHIFT		(28)
#define C0_PLL_LOCK_ENABLE_SHIFT		(29)
#define C0_PLL_ENABLE_SHIFT			(31)

#define C0_PLL_FACTOR_M_MASK			(0x3 << C0_PLL_FACTOR_M_SHIFT)
#define C0_PLL_FACTOR_N_MASK			(0xff << C0_PLL_FACTOR_N_SHIFT)
#define C0_PLL_OUT_EXT_DIVP_MASK		(0x3 << C0_PLL_OUT_EXT_DIVP_SHIFT)
#define C0_PLL_OUTPUT_ENABLE_MASK		(0x1 << C0_PLL_OUTPUT_ENABLE_SHIFT)
#define C0_PLL_LOCK_STATUS_MASK			(0x1 << C0_PLL_LOCK_STATUS_SHIFT)
#define C0_PLL_LOCK_ENABLE_MASK			(0x1 << C0_PLL_LOCK_ENABLE_SHIFT)
#define C0_PLL_ENABLE_MASK			(0x1 << C0_PLL_ENABLE_SHIFT)

#define C0_PLL_FACTOR_M(n)			((n) << C0_PLL_FACTOR_M_SHIFT)
#define C0_PLL_FACTOR_N(n)			((n) << C0_PLL_FACTOR_N_SHIFT)
#define C0_PLL_OUT_EXT_DIVP(n)			((n) << C0_PLL_OUT_EXT_DIVP_SHIFT)
#define C0_PLL_OUTPUT_ENABLE(n)			((n) << C0_PLL_OUTPUT_ENABLE_SHIFT)
#define C0_PLL_LOCK_STATUS(n)			((n) << C0_PLL_LOCK_STATUS_SHIFT)
#define C0_PLL_LOCK_ENABLE(n)			((n) << C0_PLL_LOCK_ENABLE_SHIFT)
#define C0_PLL_ENABLE(n)			((n) << C0_PLL_ENABLE_SHIFT)

/* CCU_USB0_CLOCK_REG */
#define SCLK_GATING_OHCI0_MASK		(0x1 << 31)
#define USBPHY0_RST_MASK		(0x1 << 30)
#define SCLK_GATING_USBPHY0_MASK	(0x1 << 29)

/* CCU_USB0_CLOCK_REG */
#define SCLK_GATING_OHCI1_MASK		(0x1 << 31)
#define USBPHY1_RST_MASK		(0x1 << 30)
#define SCLK_GATING_USBPHY1_MASK	(0x1 << 29)

/* CCU_USB_BUS_GATING_RST_REG */
#define USBEHCI0_RST_MASK		(0x1 << 20)
#define USBOHCI0_RST_MASK		(0x1 << 16)
#define USBEHCI0_GATING_MASK		(0x1 << 4)
#define USBOHCI0_GATING_MASK		(0x1 << 0)

#define USBEHCI1_RST_MASK		(0x1 << 21)
#define USBOHCI1_RST_MASK		(0x1 << 17)
#define USBEHCI1_GATING_MASK		(0x1 << 5)
#define USBOHCI1_GATING_MASK		(0x1 << 1)

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

/*smc config info*/
#define SMC_ACTION_REG                (SUNXI_SMC_PBASE + 0x0004)
#define SMC_MST0_BYP_REG              (SUNXI_SMC_PBASE + 0x0070)
#define SMC_MST1_BYP_REG              (SUNXI_SMC_PBASE + 0x0074)
#define SMC_MST2_BYP_REG              (SUNXI_SMC_PBASE + 0x0078)

#define SMC_MST0_SEC_REG              (SUNXI_SMC_PBASE + 0x0080)
#define SMC_MST1_SEC_REG              (SUNXI_SMC_PBASE + 0x0084)
#define SMC_MST2_SEC_REG              (SUNXI_SMC_PBASE + 0x0088)

#define SMC_REGION_COUNT (8) /*total 16,not all used, save some space*/
#define SMC_REGIN_SETUP_LOW_REG(x)    (SUNXI_SMC_PBASE + 0x100 + 0x10*(x))
#define SMC_REGIN_SETUP_HIGH_REG(x)   (SUNXI_SMC_PBASE + 0x104 + 0x10*(x))
#define SMC_REGIN_ATTRIBUTE_REG(x)    (SUNXI_SMC_PBASE + 0x108 + 0x10*(x))

/*SID*/
#define SID_SEC_MODE_STA              (SUNXI_SID_PBASE + 0xA0)
#define SID_SEC_MODE_MASK             (0x1)

#endif

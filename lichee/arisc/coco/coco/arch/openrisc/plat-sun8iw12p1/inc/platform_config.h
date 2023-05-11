/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : lib_inc.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-04-13 15:16
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __PLATFORM_CONFIG_H__
#define __PLATFORM_CONFIG_H__

#include <stdarg.h>     //use gcc stardard header file
#include <stddef.h>     //use gcc stardard define keyword, like size_t, ptrdiff_t
#include "ibase.h"

#include "ccmu_reg.h"
#include "ccu_lib.h"
#include "prcm_reg.h"
#include "reg_base.h"
#include "cpucfg_regs.h"
#include "irqnum_config.h"
#include "rsb_address.h"


#define ARISC_USE_DRAM_CODE_PBASE       (0x48100000)
#define ARISC_USE_DRAM_CODE_SIZE        (0x00003000)
#define ARISC_USE_DRAM_DATA_PBASE       (ARISC_USE_DRAM_CODE_PBASE + ARISC_USE_DRAM_CODE_SIZE)
#define ARISC_USE_DRAM_DATA_SIZE        (0x00002000)


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
#define SUNXI_SID_PBASE                  0x01c14000
#define SUNXI_SS_PBASE                   0x01c15000
#define SUNXI_MSGBOX_PBASE               0x01c17000
#define SUNXI_SPINLOCK_PBASE             0x01c18000
#define SUNXI_SMC_PBASE                  0x03007000
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


#endif


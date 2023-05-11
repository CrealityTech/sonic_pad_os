/*
 *********************************************************************************************************
 *                                                AR100 SYSTEM
 *                                     AR100 Software System Develop Kits
 *
 *
 *                                    (c) Copyright 2018-2024, Luojie China
 *)                                             All Rights Reserved
 *
 * File    : prcm_reg.h
 * By      : LuoJie
 * Version : v1.0
 * Date    : 2018-04-13 11:21
 * Descript: .
 * Update  :auther      ver     notes
 *          Luojie      1.0     Create this file.
 *********************************************************************************************************
 */

#ifndef __PRCM_REGS_H__
#define __PRCM_REGS_H__

#include "lib_inc.h"


#define	CPUS_CONFIG_REG			0x0000
#define	APBS1_CONFIG_REG		0x000c
#define	APBS2_CONFIG_REG		0x0010
#define	TIMER_BUS_GATING_REG		0x011c
#define	WATCHDOG_BUG_GATING_REG		0x012c
#define PWM_BUS_GATING_REG		0x013c
#define UART_BUS_GATING_REG		0x018c
#define TWI_BUS_GATING_REG		0x019c
#define RSB_BUS_GATING_REG		0x01bc
#define IR_RX_CLK_REG			0x01c0
#define IR_RX_BUS_GATING_REG		0x01cc
#define OWC_CLK_REG			0x01e0
#define OWC_BUS_GATING_REG		0x01ec
#define RTC_BUS_GATING_REG		0x020c
#define PLL_CONTROL_REG0		0x0240
#define PLL_CONTROL_REG1		0x0244
#define VDD_SYS_POWEROFF_GATING_REG	0x0250
#define GPU_POWEROFF_GATING_REG		0x0254
#define VE_POWEROFF_GATING_REG		0
#define VDD_SYS_PWOER_RESET_REG		0x0260
#define RAM_CONFIG_REG			0x0270
#define RAM_TEST_CONTROL_REG		0x0274
#define PRCM_SECURITY_REG		0x0290
#define REG_CALIB_CONTROL_REG		0x0310
#define OHMS200_REG			0x0314
#define OHMS240_REG			0x0318
#define REG_CALIB_STATUS_REG		0x031c
#define PRCM_VERSION_REG		0x03f0

/******************************************************
 *
 *****************************************************/
#define PRCM_SRC_HOSC			0x00
#define PRCM_SRC_LOSC			0x01
#define PRCM_SRC_IOSC			0x02
#define PRCM_SRC_PLL			0x03

/******************************************************
 *
 *****************************************************/
#define PRCM_CLK_SRC_OFFSET		24
#define PRCM_CLK_SRC_MASK		0x3

#define PRCM_FACTOR_M_OFFSET		0
#define PRCM_FACTOR_M_MASK		0x1f
#define PRCM_APBS1_FACTOR_M_MASK	0x3
#define PRCM_FACTOR_N_OFFSET		8
#define PRCM_FACTOR_N_MASK		0x3

#define PRCM_BUS_RESET_OFFSET		16
#define PRCM_BUS_GATING_OFFSET		0

#define PRCM_SPECIAL_CLK_GATING_OFFSET	31
#define PRCM_SPECIAL_CLK_SRC_OFFSET	24
#define PRCM_SPECIAL_N_OFFSET		8
#define PRCM_SPECIAL_M_OFFSET		0

/******************************************************
 *
 *****************************************************/
#define GET_PRCM_FACTOR_M_MASK(x)	\
	(x == (R_PRCM_REG_BASE + APBS1_CONFIG_REG)) ? PRCM_APBS1_FACTOR_M_MASK : PRCM_FACTOR_M_MASK

#endif


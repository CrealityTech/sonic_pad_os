/*
 *********************************************************************************************************
 *                                                AR100 SYSTEM
 *                                     AR100 Software System Develop Kits
 *                                              interrupt  module
 *
 *                                    (c) Copyright 2018-2024, Luojie China
 *                                             All Rights Reserved
 *
 * File    : intc_i.h
 * By      : Luojie
 * Version : v1.0
 * Date    : 2018-4-10
 * Descript: io module public header.
 * Update  : date                auther      ver     notes
 *           2012-4-10 11:06:40  Luojie      1.0     Create this file.
 *********************************************************************************************************
 */

#ifndef __REG_BASE_H__
#define __REG_BASE_H__

#include "lib_inc.h"


#define RTC_REG_BASE            (0x07000000)
#define R_CPUCFG_REG_BASE       (0x07000400)
#define R_PRCM_REG_BASE         (0x07010000)
#define R_TMR_REG_BASE		(0x07020000)
#define R_TMR0_REG_BASE		(0x07020010)
#define R_TMR1_REG_BASE		(0x07020020)
#define R_TMR2_REG_BASE		(0x07020030)
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
#define CPUCFG_REG_BASE         (0x09010000)
#define CPUSUBSYS_REG_BASE      (0x08100000)
#define HWMSGBOX_REG_BASE	(0x03003000)
#define HWSPINLOCK_REG_BASE	(0x03004000)
#define CPUX_PIO_REG_BASE       (0x0300B000)
#define CPUX_TIMER_REG_BASE     (0x03009000)
#define CCMU_REG_BASE            (0x03001000)
#define TS_STAT_REG_BASE        (0x08110000)
#define TS_CTRL_REG_BASE        (0x08120000)

#endif  //__IO_H__

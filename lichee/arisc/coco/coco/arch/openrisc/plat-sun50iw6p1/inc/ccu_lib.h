/*
 *                       AR100 SYSTEM
 *          AR100 Software System Develop Kits
 *                ccu module register library
 *
 *            (c) Copyright 2018-2024, Luojie China
 *                      All Rights Reserved
 *
 * File    : intc_i.h
 * By      : Luojie
 * Version : v1.0
 * Date    : 2018-4-10
 * Descript: ccu module register library public header.
 * Update  : date                auther      ver     notes
 *           2018-4-10 11:06:40  Luojie      1.0     Create this file.
 */

#ifndef __CCU_LIB_H__
#define __CCU_LIB_H__

#include "lib_inc.h"

#define GET_CCU_FACTOR_M_OFFSET(x) \
	((x >= R_PRCM_REG_BASE) ? PRCM_FACTOR_M_OFFSET : CCMU_FACTOR_M_OFFSET)

#define GET_CCU_FACTOR_M_MASK(x)                                              \
	((x >= R_PRCM_REG_BASE) ? (unsigned int)(GET_PRCM_FACTOR_M_MASK(x)) : \
				  (unsigned int)CCMU_FACTOR_M_MASK)

#define GET_CCU_FACTOR_N_OFFSET(x) \
	((x >= R_PRCM_REG_BASE) ? PRCM_FACTOR_N_OFFSET : CCMU_FACTOR_N_OFFSET)

#define GET_CCU_FACTOR_N_MASK(x) \
	((x >= R_PRCM_REG_BASE) ? PRCM_FACTOR_N_MASK : CCMU_FACTOR_N_MASK)

#define CCU_SRC_OFFSET        24
#define CCU_SRC_MASK          0x3
#define PERIO_1X_PARA         4

#define CLOKC_SRC_SET_HOSC    0x00
#define CLOKC_SRC_SET_LOSC    0x01
#define CLOKC_SRC_SET_IOSC    0x02
#define CLOKC_SRC_SET_PSI     0x02
#define CLOKC_SRC_SET_PLL     0x03

#define CLOKC_SRC_HOSC        24000000
#define CLOKC_SRC_LOSC        32000
#define CLOKC_SRC_IOSC        16000000

#define CCU_CLK_ENABLE_OFFSET 31

#define BUS_TYPE_PLL          0x0
#define BUS_TYPE_AHB_PSI_APB  0x1

#define GET_BUS_TYPE(x) \
	(x = PLL_PERI0_CTRL_REG ? BUS_TYPE_PLL : BUS_TYPE_AHB_PSI_APB)

s32 CCU_Get_Clksrc(u32 reg);
void CCU_Set_Clkdiv(u32 reg, u32 n, u32 m, u32 p);
void CCU_Get_Clkdiv(u32 reg, u32 *n, u32 *m, u32 *p);
void CCU_Set_Bus_Gating_Reset(u32 reg, u32 status);
void CCU_Set_Special_Clk(u32 reg, u32 clk);
void CCU_Set_Special_Clkgating(u32 reg, u32 gating);
void CCU_Set_Clksrc(u32 reg, u32 clk);
u32 CCU_Get_Clkfreq(u32 reg);
void CCU_Disbale_Clk(u32 reg);
u32 Get_APBS2_Freq(void);
static inline u32 iosc_freq_init(void)
{
	return -ENODEV;
}

#endif


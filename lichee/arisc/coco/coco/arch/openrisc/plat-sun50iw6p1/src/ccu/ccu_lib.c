/*
 *			ARISC
 *		cpus reg library for 1728
 *
 *           (c) Copyright 2017-2020, luojie China
 *                  All Rights Reserved
 *
 * File    : 1728_prcm.c
 * By      : luojie
 * Version : v0.1
 * Date    : 208-4-9
 * Descript: reg clock management
 * Update  : date                auther      ver     notes
 *           2018-4-9            luojie      0.1     Create this file.
 */
#include "lib_inc.h"

void CCU_Set_Clksrc(u32 reg, u32 clk)
{
	u32 value;
	u32 srcoffset, srcmask;

	value = readl(reg);

	srcoffset = CCU_SRC_OFFSET;
	srcmask = CCU_SRC_MASK;

	value &= ~(srcmask << srcoffset);
	value |= (clk << srcoffset);

	writel(value, reg);
}

s32 CCU_Get_Clksrc(u32 reg)
{
	u32 value;
	u32 srcoffset, srcmask;

	value = readl(reg);

	srcoffset = CCU_SRC_OFFSET;
	srcmask = CCU_SRC_MASK;

	value = value >> srcoffset;
	value &= srcmask;

	return value;
}

void CCU_Set_Clkdiv(u32 reg, u32 n, u32 m, u32 p)
{
	u32 value;
	u32 noffset, moffset;

	value = readl(reg);

	noffset = GET_CCU_FACTOR_N_OFFSET(reg);
	moffset = GET_CCU_FACTOR_M_OFFSET(reg);

	value &= (~((GET_CCU_FACTOR_N_MASK(reg) << noffset)));
	value |= (n << noffset);

	value &= (~((GET_CCU_FACTOR_M_MASK(reg) << moffset)));
	value |= (m << moffset);

	writel(value, reg);
}

void CCU_Get_Clkdiv(u32 reg, u32 *n, u32 *m, u32 *p)
{
	u32 value;
	u32 noffset, moffset, nmask, mmask;

	*p = 0;

	noffset = GET_CCU_FACTOR_N_OFFSET(reg);
	moffset = GET_CCU_FACTOR_M_OFFSET(reg);
	nmask = GET_CCU_FACTOR_N_MASK(reg);
	mmask = GET_CCU_FACTOR_M_MASK(reg);

	value = readl(reg);
	*n = (value >> noffset) & nmask;

	value = readl(reg);
	*m = (value >> moffset) & mmask;
}

void CCU_Set_Bus_Gating_Reset(u32 reg, u32 status)
{
	u32 value;

	value = readl(reg);

	value &= ~(0x1 << PRCM_BUS_RESET_OFFSET);
	value |= (status << PRCM_BUS_RESET_OFFSET);

	value &= ~(0x1 << PRCM_BUS_GATING_OFFSET);
	value |= (status << PRCM_BUS_GATING_OFFSET);

	writel(value, reg);
}

void CCU_Set_Special_Clk(u32 reg, u32 clk)
{
	u32 value;

	value = readl(reg);

	value &= ~(1 << PRCM_SPECIAL_CLK_SRC_OFFSET);
	value |= (clk << PRCM_SPECIAL_CLK_SRC_OFFSET);

	writel(value, reg);
}

void CCU_Set_Special_Clkgating(u32 reg, u32 gating)
{
	u32 value;

	value = readl(reg);

	value &= ~(1 << PRCM_SPECIAL_CLK_GATING_OFFSET);
	value |= (gating << PRCM_SPECIAL_CLK_GATING_OFFSET);

	writel(value, reg);
}

u32 CCU_Get_Clkfreq(u32 reg)
{
	u32 value;

	value = readl(reg);
	value = value >> CCU_SRC_OFFSET;
	value &= CCU_SRC_MASK;

	return value;
}

void CCU_Disbale_Clk(u32 reg)
{
	u32 value;

	value = readl(reg);

	value &= ~(1 << CCU_CLK_ENABLE_OFFSET);

	writel(value, reg);
}

u32 Get_APBS2_Freq(void)
{
	u32 apbs_clk;
	u32 clk_sel;
	u32 n, m, p;

	clk_sel = CCU_Get_Clksrc(R_PRCM_REG_BASE + APBS2_CONFIG_REG);
	switch (clk_sel) {
	case CLOKC_SRC_SET_HOSC:
		apbs_clk = CLOKC_SRC_HOSC;
		break;
	case CLOKC_SRC_SET_LOSC:
		apbs_clk = CLOKC_SRC_LOSC;
		break;
	case CLOKC_SRC_SET_IOSC:
		apbs_clk = CLOKC_SRC_IOSC;
		break;
	case CLOKC_SRC_SET_PLL:
		apbs_clk = CCU_Get_Clksrc(CCMU_REG_BASE + PLL_PERI0_CTRL_REG);
		CCU_Get_Clkdiv(PLL_PERI0_CTRL_REG, &n, &m, &p);
		apbs_clk = apbs_clk / (n + 1) / (m + 1);
		break;
	default:
		break;
	}

	return apbs_clk;
}


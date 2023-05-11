/*
*********************************************************************************************************
*						ARISC
*					cpus reg library for 1728
*
*                                    (c) Copyright 2017-2020, luojie China
*                                             All Rights Reserved
*
* File    : 1728_prcm.c
* By      : luojie
* Version : v0.1
* Date    : 208-4-9
* Descript: reg clock management
* Update  : date                auther      ver     notes
*           2018-4-9            luojie      0.1     Create this file.
*********************************************************************************************************
*/
#include "lib_inc.h"
#include "cpucfg.h"

static u32 iosc_freq;

#define CCU_PLL_C0_REG (0x03001000)
#define CCU_HOSC_FREQ               (24000000)  //24M
#define CLOCK_SOURCE (24000000)//24MHZ
typedef struct ccu_pll_c0_cpux_reg0000
{
	volatile u32 enable:1;      //bit31, 0-disable, 1-enable, (24Mhz*N*K)/(M)
	volatile u32 reserved0:1;   //bit30, reserved
	volatile u32 lock_en:1;     //bit29, 0-disable lock, 1-enable lock
	volatile u32 lock_st:1;     //bit28, 0-unlocked, 1-locked(PLL has been stable)
	volatile u32 reserved1:1;   //bit27, reserved
	volatile u32 lock_time:3;   //bit24, lock time:freq scaling step
	volatile u32 reserved2:6;   //bit18, reserved
	volatile u32 factor_p:2;    //bit16, PLL1 Factor_P
	volatile u32 factor_n:8;    //bit8,  PLL1 Factor_N
	volatile u32 reserved3:6;   //bit2,  reserved
	volatile u32 factor_m:2;    //bit0,  PLL1 Factor_M
} ccu_pll_c0_cpux_reg0000_t;

typedef struct ccu_pll1_factor
{
    u8 factor_n;
    u8 factor_k;
    u8 factor_m;
    u8 factor_p;
    u32 freq;
} ccu_pll1_factor_t;


void CCU_Set_Clksrc(u32 reg ,u32 clk)
{
	u32 value;
	u32 srcoffset, srcmask;

	value = readl(reg);

	srcoffset = CCU_SRC_OFFSET;

	if ((reg - CCMU_REG_BASE) == CPU_AXI_CFG_REG)
		srcmask = CCU_CPU_SRC_MASK;
	else
		srcmask = CCU_SRC_MASK;

	value &= ~(srcmask << srcoffset);
	value |= (clk << PRCM_CLK_SRC_OFFSET);

	writel(value,reg);
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
/*
 * NOTE:
 * n: 0xffffffff will be ignored.
 * m: 0xffffffff will be ignored.
 **/
void CCU_Set_Clkdiv(u32 reg ,u32 n, u32 m , u32 p)
{
	u32 value;
	u32 noffset, moffset;

	p = p;
	value = readl(reg);

	noffset = GET_CCU_FACTOR_N_OFFSET(reg);
	moffset = GET_CCU_FACTOR_M_OFFSET(reg);

	if (n != 0xffffffff) {
		value &= (~((GET_CCU_FACTOR_N_MASK(reg) << noffset)));
		value |= (n << noffset);
	}

	if (m != 0xffffffff) {
		value &= (~((GET_CCU_FACTOR_M_MASK(reg) << moffset)));
		value |= (m << moffset);
	}

	writel(value,reg);
}

void CCU_Get_Clkdiv(u32 reg ,u32 *n, u32 *m , u32 *p)
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

void CCU_Set_Bus_Gating_Reset(u32 reg ,u32 status)
{
	u32 value;

	value = readl(reg);

	value &= ~(0x1 << PRCM_BUS_RESET_OFFSET);
	value |= (status << PRCM_BUS_RESET_OFFSET);

	value &= ~(0x1 << PRCM_BUS_GATING_OFFSET);
	value |= (status << PRCM_BUS_GATING_OFFSET);

	writel(value,reg);
}

void CCU_Set_Special_Clk(u32 reg ,u32 clk)
{
	u32 value;

	value = readl(reg);

	value &= ~(1 << PRCM_SPECIAL_CLK_SRC_OFFSET);
	value |= (clk << PRCM_SPECIAL_CLK_SRC_OFFSET);

	writel(value,reg);
}


void CCU_Set_Special_Clkgating(u32 reg ,u32 gating)
{
	u32 value;

	value = readl(reg);

	value &= ~(1 << PRCM_SPECIAL_CLK_GATING_OFFSET);
	value |= (gating << PRCM_SPECIAL_CLK_GATING_OFFSET);

	writel(value,reg);
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

	writel(value,reg);
}

u32 Get_APBS2_Freq(void)
{
	u32 apbs_clk;
	u32 clk_sel;
	u32 n,m,p;

	clk_sel = CCU_Get_Clksrc(R_PRCM_REG_BASE + APBS2_CONFIG_REG);
	switch (clk_sel){
		case CLOKC_SRC_SET_HOSC:
			apbs_clk = CLOKC_SRC_HOSC;
			break;
		case CLOKC_SRC_SET_LOSC:
			apbs_clk = CLOKC_SRC_LOSC;
			break;
		case CLOKC_SRC_SET_IOSC:
			apbs_clk = iosc_freq;
			break;
		case CLOKC_SRC_SET_PLL:
			apbs_clk = CCU_Get_Clksrc(CCMU_REG_BASE + PLL_PERI0_CTRL_REG);
			CCU_Get_Clkdiv(PLL_PERI0_CTRL_REG,&n ,&m, &p);
			apbs_clk = apbs_clk / (n+1) / (m+1);
			break;
		default:
			break;
	}

	return apbs_clk;
}

unsigned long unsigned_int_pow(unsigned long base, u8 power);
static void ccu_freq_get_factor_n(u8 *factor_n, u8 factor_p, u32 freq);
s32 ccu_calc_pll1_factor(struct ccu_pll1_factor *factor, u32 rate);
s32 ccu_get_sclk_freq(void);
s32 ccu_set_sclk_freq(u32 freq);
unsigned long unsigned_int_pow(unsigned long base, u8 power)
{
	u32 value = 1;

	if (power == 0)
		return value;

	while (power != 0) {
		value *= base;
		power--;
	}
	return value;
}

static void ccu_freq_get_factor_n(u8 *factor_n, u8 factor_p, u32 freq)
{
	*factor_n = (freq * unsigned_int_pow(2, factor_p)) / CLOCK_SOURCE - 1;
}

s32 ccu_calc_pll1_factor(struct ccu_pll1_factor *factor, u32 rate)
{
	if(rate > 2016000000)
	{
		rate = 2016000000;
	}
	if (rate > 600000000)
		factor->factor_p = 0;
	else if (rate > 150000000)
		factor->factor_p = 1;
	else
		factor->factor_p = 2;

	ccu_freq_get_factor_n(&factor->factor_n, factor->factor_p, rate);
	factor->factor_m = 0;

	return OK;
}


s32 ccu_set_sclk_freq(u32 freq)
{
	struct ccu_pll_c0_cpux_reg0000 *ccu_pll_c0_cpux_reg_addr = (struct ccu_pll_c0_cpux_reg0000 *)CCU_PLL_C0_REG;
	//maybe should delete
	ccu_pll1_factor_t       factor;
	ccu_pll_c0_cpux_reg0000_t  pll1;

	//calc pll1 factors by freq.
	ccu_calc_pll1_factor(&factor, freq);

	//set factor
	pll1 = *(ccu_pll_c0_cpux_reg_addr);

	//try to increase factor p first
	if (pll1.factor_p < factor.factor_p)
	{
		pll1.factor_p = factor.factor_p;
		*(ccu_pll_c0_cpux_reg_addr) = pll1;
		time_cdelay(2000);	//delay 10us, cpu clock 200m
	}

	//try to increase factor m first
	if (pll1.factor_m < factor.factor_m)
	{
		pll1.factor_m = factor.factor_m;
		*(ccu_pll_c0_cpux_reg_addr) = pll1;
		time_cdelay(2000);	//delay 10us, cpu clock 200m
	}

	//write factor n * k
	pll1.factor_n = factor.factor_n;
	*(ccu_pll_c0_cpux_reg_addr) = pll1;

	//wait for lock change first
	time_cdelay(20);

	//wait for PLL1 stable
	//maybe the fpga have not to wait PLLx stable
	//by superm at 2013-5-15 13:07:21
	time_mdelay(1);

	//decease factor m
	if (pll1.factor_m > factor.factor_m)
	{
		pll1.factor_m = factor.factor_m;
		*(ccu_pll_c0_cpux_reg_addr) = pll1;
		time_cdelay(2000);	//delay 10us, cpu clock 200m
	}

	//decease factor p
	if (pll1.factor_p > factor.factor_p)
	{
		pll1.factor_p = factor.factor_p;
		*(ccu_pll_c0_cpux_reg_addr) = pll1;
		time_cdelay(2000);	//delay 10us, cpu clock 200m
	}
	INF("PLL1 Freq %d N %d M %d P %d\n", freq, factor.factor_n, factor.factor_m, factor.factor_p);
	return OK;
}
/*
*********************************************************************************************************
*                                     GET SOURCE FREQUENCY
*
* Description: 	get the frequency of a specific source clock.
*
* Arguments  : 	sclk : the source clock ID which we want to get frequency.
*
* Returns    : 	frequency of the specific source clock.
*********************************************************************************************************
*/
s32 ccu_get_sclk_freq(void)
{
	struct ccu_pll_c0_cpux_reg0000 *ccu_pll_c0_cpux_reg_addr = (struct ccu_pll_c0_cpux_reg0000 *)CCU_PLL_C0_REG;
	ccu_pll_c0_cpux_reg0000_t pll_c0 = *(ccu_pll_c0_cpux_reg_addr);
	return (CCU_HOSC_FREQ * (pll_c0.factor_n + 1)) / ((pll_c0.factor_m + 1) * (1 << pll_c0.factor_p));
}

u32 iosc_freq_init(void)
{
	u64 cnt1, cnt2;
	u32 xt, ht, xf;
	u32 cpus_src;

	cpus_src = CCU_Get_Clksrc(R_PRCM_REG_BASE + CPUS_CONFIG_REG);
	CCU_Set_Clksrc(R_PRCM_REG_BASE + CPUS_CONFIG_REG ,PRCM_SRC_HOSC);
	time_cdelay(1600);
	//cpucfg_counter_clear();
	cnt1 = timestamp_read();
	time_cdelay(1000);
	cnt2 = timestamp_read() - cnt1;
	ht = (u32)(cnt2 & 0xffffffff);

	CCU_Set_Clksrc(R_PRCM_REG_BASE + CPUS_CONFIG_REG ,PRCM_SRC_IOSC);
	time_cdelay(1600);
	//cpucfg_counter_clear();
	cnt1 = timestamp_read();
	time_cdelay(1000);
	cnt2 = timestamp_read() - cnt1;
	xt = (u32)(cnt2 & 0xffffffff);

	xf = (24000 * ht)/xt;
	CCU_Set_Clksrc(R_PRCM_REG_BASE + CPUS_CONFIG_REG ,cpus_src);

	iosc_freq = xf * 1000;

	return iosc_freq;
}



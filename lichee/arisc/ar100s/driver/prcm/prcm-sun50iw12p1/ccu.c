/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                         clock control unit module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : ccu.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-7
* Descript: clock control unit module.
* Update  : date                auther      ver     notes
*           2012-5-7 8:43:10    Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "ccu_i.h"
#include "cpucfg_regs.h"


/* apb clock change notifier list */
u32 iosc_freq = 16000000;
u32 losc_freq = 32768;

volatile static u32 already_init_osc_freq;

void iosc_freq_init(void)
{
}

/*
*********************************************************************************************************
*                                       INITIALIZE CCU
*
* Description:  initialize clock control unit.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize ccu succeeded, others if failed.
*********************************************************************************************************
*/
s32 ccu_init(void)
{
#ifndef CFG_FPGA_PLATFORM
	/* setup cpus post div source to 200M */
	writel((readl(CPUS_CFG_REG) & (~CPUS_FACTOR_M_MASK)) | CPUS_FACTOR_M(2), CPUS_CFG_REG);
	writel((readl(CPUS_CFG_REG) & (~CPUS_CLK_DIV_N_MASK)) | CPUS_CLK_DIV_N(0), CPUS_CFG_REG);

	/* set cpus clock source to pll_peri0 */
	writel((readl(CPUS_CFG_REG) & (~CPUS_CLK_SRC_SEL_MASK)) | CPUS_CLK_SRC_SEL(3), CPUS_CFG_REG);
#endif

	/* ccu initialize succeeded */
	return OK;
}

/*
*********************************************************************************************************
*                                       EXIT CCU
*
* Description:  exit clock control unit.
*
* Arguments  :  none.
*
* Returns    :  OK if exit ccu succeeded, others if failed.
*********************************************************************************************************
*/
s32 ccu_exit(void)
{
	return OK;
}

void save_state_flag(u32 value)
{
	writel(value, RTC_RECORD_REG);
}

u32 read_state_flag(void)
{
	return readl(RTC_RECORD_REG);
}

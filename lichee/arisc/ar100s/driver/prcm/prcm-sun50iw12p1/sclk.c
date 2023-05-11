/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                	  	  clock control unit module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : sclk.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-7
* Descript: system clock management.
* Update  : date                auther      ver     notes
*           2012-5-7 8:43:10	Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "ccu_i.h"

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
s32 ccu_get_sclk_freq(u32 sclk)
{
	switch (sclk) {
	case CCU_SYS_CLK_CPUS:
		{
			switch (readl (CPUS_CFG_REG) >> 24) {
			case 0:
				{
					/* cpus clock source is losc */
					return CCU_HOSC_FREQ;
				}
			case 1:
				{
					/* cpus clock source is hosc */
					return losc_freq;
				}
			case 2:
				{
					/* cpus clock source is internal-osc */
					return iosc_freq;
				}
			case 3:
				{
					/* assume pll_peri0(1x) / factor_m must 200M */
					return CCU_CPUS_PLL0_FREQ;
				}
			default:
				{
					return 0;
				}
			}
		}
	case CCU_SYS_CLK_APBS2:
		{
			switch (readl (APBS2_CFG_REG) >> 24) {
			case 0:
				{
					/* cpus clock source is losc */
					return CCU_HOSC_FREQ;
				}
			case 1:
				{
					/* cpus clock source is hosc */
					return losc_freq;
				}
			case 2:
				{
					/* cpus clock source is internal-osc */
					return iosc_freq;
				}
			case 3:
				{
					/* cpus clock source is pll6 */
					return CCU_APBS2_PLL0_FREQ;
				}
			default:
				{
					return 0;
				}
			}
		}
	}
	WRN("invalid clock id for get source freq\n");
	return 0;
}

s32 ccu_set_sclk_onoff(u32 sclk, s32 onoff)
{
	switch (sclk) {
	case CCU_SYS_CLK_C0:
		{
			writel((readl(CCU_PLL_C0_REG) | (onoff << PLL_EN)), CCU_PLL_C0_REG);
			return OK;
		}
	case CCU_SYS_CLK_DDR0:
		{
			writel((readl(CCU_PLL_DDR0_REG) | (onoff << PLL_EN)), CCU_PLL_DDR0_REG);
			return OK;
		}
	case CCU_SYS_CLK_PERI0:
		{
			writel((readl(CCU_PLL_PERIPH0_REG) | (onoff << PLL_EN)), CCU_PLL_PERIPH0_REG);
			return OK;
		}
	case CCU_SYS_CLK_PERI1:
		{
			writel((readl(CCU_PLL_PERIPH1_REG) | (onoff << PLL_EN)), CCU_PLL_PERIPH1_REG);
			return OK;
		}
	}
	WRN("invalid clock id for get source freq\n");
	return 0;
}

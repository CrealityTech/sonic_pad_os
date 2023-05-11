/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                watchdog  module
*
*                                    (c) Copyright 2012-2016, Superm Wu China
*                                             All Rights Reserved
*
* File    : Watchdog.c
* By      : Superm Wu
* Version : v1.0
* Date    : 2012-9-18
* Descript: watchdog controller public interfaces.
* Update  : date                auther      ver     notes
*           2012-9-18 19:08:23  Superm Wu   1.0     Create this file.
*********************************************************************************************************
*/
#include "watchdog_i.h"

/* watchdog base registers */
struct watchdog_regs *pwatchdog_regs;

/*
*********************************************************************************************************
*                                       INIT WATCHDOG
*
* Description:  initialize watchdog.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize watchdog succeeded, others if failed.
*********************************************************************************************************
*/
s32 watchdog_init(void)
{
	/* initialize the pointer of watchdog registers */
	pwatchdog_regs = (struct watchdog_regs *)(WDOG0_REG_BASE);

	/*
	 * set watchdog work mode
	 * set the watchdog0 clock source:24M/750
	 */
	pwatchdog_regs->config &= ~(0x1 << 8);
	pwatchdog_regs->config |= WDOG0_CLK_SRC_32K;

	/* watchdog0 generates a reset signal to CPUS */
	pwatchdog_regs->config &= ~(0x3 << 0);
	pwatchdog_regs->config |= WDOG0_RST_SYS;

	/*
	 * if sclk=32k and interval value=16000,
	 * then interval time=0.5s
	 * the interval value maybe greater,
	 * then will decrease cpu performance loss
	 * set watchdog0 Interval Value:16000 cycles
	 */
	pwatchdog_regs->mode &= ~(0xf << 4);
	pwatchdog_regs->mode |= WDOG0_INTV_VALUE_04S;

	return OK;
}

/*
*********************************************************************************************************
*                                       EXIT WATCHDOG
*
* Description:  exit watchdog.
*
* Arguments  :  none.
*
* Returns    :  OK if exit watchdog succeeded, others if failed.
*********************************************************************************************************
*/
s32 watchdog_exit(void)
{
	/* release regs addrress */
	pwatchdog_regs = NULL;

	return OK;
}

/*
*********************************************************************************************************
*                                       ENABLE WATCHDOG
*
* Description:  enable watchdog.
*
* Arguments  :  none.
*
* Returns    :  OK if enable watchdog succeeded, others if failed.
*********************************************************************************************************
*/
s32 watchdog_enable(void)
{
	/* enable the watchdog0 */
	pwatchdog_regs->mode &= ~(0x1 << 0);
	pwatchdog_regs->mode |= WDOG0_EN;

	return OK;
}

/*
*********************************************************************************************************
*                                       DISABLE WATCHDOG
*
* Description:  disable watchdog.
*
* Arguments  :  none.
*
* Returns    :  OK if disable watchdog succeeded, others if failed.
*********************************************************************************************************
*/
s32 watchdog_disable(void)
{
	/* disable the watchdog0 */
	pwatchdog_regs->mode &= ~(0x1 << 0);
	pwatchdog_regs->mode |= WDOG0_DIS;

	return OK;
}

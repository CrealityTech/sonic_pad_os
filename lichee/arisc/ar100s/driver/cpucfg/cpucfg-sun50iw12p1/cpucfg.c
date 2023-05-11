/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                cpucfg module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : cpucfg.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-7
* Descript: cpu config module.
* Update  : date                auther      ver     notes
*           2012-5-7 17:24:32   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "../cpucfg_i.h"

static void cpucfg_counter_init(void);
volatile s32 little_endian_used;
/*
*********************************************************************************************************
*                                       INITIALIZE CPUCFG
*
* Description:  initialize cpu configure module.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize cpu configure succeeded, others if failed.
*********************************************************************************************************
*/
s32 cpucfg_init(void)
{
	cpucfg_counter_init();
	little_endian_used = 0;

	return OK;
}

/*
*********************************************************************************************************
*                                       EXIT CPUCFG
*
* Description:  exit cpu configure module.
*
* Arguments  :  none.
*
* Returns    :  OK if exit cpu configure succeeded, others if failed.
*********************************************************************************************************
*/
s32 cpucfg_exit(void)
{
	little_endian_used = 0;
	return OK;
}

/*
*********************************************************************************************************
*                                      SET LE ADDRESS SPACE
*
* Description:  set little-endian address space.
*
* Arguments  :  start   : the start address of little-endian space.
*               end     : the end address of little-endian space.
*
* Returns    :  OK if set little-endian succeeded, others if failed.
*********************************************************************************************************
*/
s32 cpucfg_set_little_endian_address(void *start, void *end)
{
	return OK;
}

s32 cpucfg_remove_little_endian_address(void *start, void *end)
{
	return OK;
}

/*
*********************************************************************************************************
*                                      SET CPU RESET STATE
*
* Description:  set the reset state of cpu.
*
* Arguments  :  cpu_num : the cpu id which we want to set reset status.
*               state   : the reset state which we want to set.
*
* Returns    :  OK if get power status succeeded, others if failed.
*********************************************************************************************************
*/
s32 cpucfg_set_cpu_reset_state(u32 cpu_num, s32 state)
{
	return OK;
}

/*
*********************************************************************************************************
*                                      CLEAR 64BIT COUNTER
*
* Description:  clear 64bit counter, after this operation,
*               the value of conuter will reset to zero.
*
* Arguments  :  none.
*
* Returns    :  OK if clear counter succeeded, others if failed.
*********************************************************************************************************
*/
s32 cpucfg_counter_clear(void)
{
	writel(0, CNT_LOW_REG_SET);
	writel(0, CNT_HIGH_REG_SET);

	return OK;
}

/*
*********************************************************************************************************
*                                      READ 64BIT COUNTER
*
* Description:  read 64bit counter, the counter value base on us.
*
* Arguments  :  none.
*
* Returns    :  the value of counter, base on us.
*********************************************************************************************************
*/
u64 cpucfg_counter_read(void)
{
	volatile u32 high;
	volatile u32 low;
	u64 counter;

	do {
		low = readl(CNT_LOW_REG);
		high = readl(CNT_HIGH_REG);
	} while ((high != readl(CNT_HIGH_REG))
		 || (low > readl(CNT_LOW_REG)));

	counter = high;
	counter = (counter << 32) + low;

	return counter;
}

void cpucfg_counter_ctrl(bool enable)
{
	writel(enable, CNT_CTRL_REG);
}

static void cpucfg_counter_init(void)
{
	writel(CCU_HOSC_FREQ, CNT_FREQID_REG);
}


void cpucfg_acinactm_process(u32 status)
{
}

void cpucfg_wait_l2_enter_wfi(void)
{
}

void cpucfg_l1l2_reset_by_hardware(u32 status)
{
}

void cpucfg_cluster0_process(u32 status)
{
}

static int cpu_power_switch_set(u32 cluster, u32 cpu, bool enable)
{
	return OK;
}

/*
 * set cpu die before pmu power off
 */
void cpucfg_cpu_suspend(void)
{
}

void cpucfg_cpu_suspend_late(void)
{
}

/*
 * set standby or hotplug flag before.
 */
s32 cpucfg_cpu_resume_early(u32 resume_addr)
{
	/* power on cluster0 cpu0 */
	cpu_power_switch_set(0, 0, 1);
	return OK;
}

s32 cpucfg_cpu_resume(u32 resume_addr)
{
	return OK;
}

void set_secondary_entry(u32 entry, u32 cpu)
{
}

void sun50i_set_AA32nAA64(u32 cluster, u32 cpu, bool is_aa64)
{
}

void cpu_power_up(u32 cluster, u32 cpu)
{
}

void cpu_power_down(u32 cluster, u32 cpu)
{
}

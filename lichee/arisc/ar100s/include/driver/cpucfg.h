/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                cpucfg module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : cpucfg.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-4-28
* Descript: cpu config module public header.
* Update  : date                auther      ver     notes
*           2012-4-28 14:48:38  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __CPUCFG_H__
#define __CPUCFG_H__

#ifdef FPGA_PLATFORM
#define L2CACHE_IS_WFI_MODE(cluster)  (1)
#define CPU_IS_WFI_MODE(cluster, cpu) (1)
#else
#define L2CACHE_IS_WFI_MODE(cluster)  (readl(SUNXI_CLUSTER_CPU_STATUS(cluster)) & (1 << 0))
#define CPU_IS_WFI_MODE(cluster, cpu) (readl(SUNXI_CLUSTER_CPU_STATUS(cluster)) & (1 << (16 + cpu)))
#endif

/* the reset status of cpu */
typedef enum cpu_reset_status {
	CPU_RESET_ASSERT    = 0x0,
	CPU_RESET_DEASSERT  = 0x3,
} cpu_reset_status_e;

/* the status of cluster0 */
typedef enum cluster_reset_status {
	CLUSTER_RESET_ASSERT    = 0x0,
	CLUSTER_RESET_DEASSERT  = 0x1,
} cluster_reset_status_e;

/* the status of acinactm */
typedef enum acinactm_status {
	ACINACTM_LOW    = 0x0,
	ACINACTM_HIGH   = 0x1,
} acinactm_status_e;

/* L1 & L2 reset by hardware status */
typedef enum l1l2_reset_by_hardware_status {
	L1_EN_L2_EN     = 0x0,
	L1_DIS_L2_EN    = 0xf,
	L1_EN_L2_DIS    = 0x10,
	L1_DIS_L2_DIS   = 0x1f
} l1l2_reset_by_hardware_status_e;

extern void cpucfg_cpu_suspend(void);
extern void cpucfg_cpu_suspend_late(void);
extern s32 cpucfg_cpu_resume_early(u32 resume_addr);
extern s32 cpucfg_cpu_resume(u32 resume_addr);

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
s32 cpucfg_init(void);

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
s32 cpucfg_exit(void);

/*
*********************************************************************************************************
*                                       SET SUPER-STANDBY FLAG
*
* Description:  set super-standby flag.
*
* Arguments  :  none.
*
* Returns    :  OK if set super-standby flag succeeded, others if failed.
*********************************************************************************************************
*/
s32 cpucfg_set_super_standby_flag(void);

/*
*********************************************************************************************************
*                                       CLEAR SUPER-STANDBY FLAG
*
* Description:  clear super-standby flag.
*
* Arguments  :  none.
*
* Returns    :  OK if clear super-standby flag succeeded, others if failed.
*********************************************************************************************************
*/
s32 cpucfg_clear_super_standby_flag(void);

/*
*********************************************************************************************************
*                                       GET CPU STATUS
*
* Description:  get the cpu status.
*
* Arguments  :  none.
*
* Returns    :  OK if get status succeeded, others if failed.
*********************************************************************************************************
*/
s32 cpucfg_get_cpu_state(u32 cpu_num);

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
s32 cpucfg_set_little_endian_address(void *start, void *end);
extern s32 cpucfg_remove_little_endian_address(void *start, void *end);

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
s32 cpucfg_counter_clear(void);

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
u64 cpucfg_counter_read(void);

s32 cpucfg_set_cpux_jump_addr(u32 addr);

s32 cpucfg_cpu_power_off_pre_process(void);
s32 cpucfg_cpu_power_on_post_process(void);

extern void cpucfg_mcpm_init(void);
void set_secondary_entry(u32 entry, u32 cpu);
void sun50i_set_AA32nAA64(u32 cluster, u32 cpu, bool is_aa64);
void cpu_power_up(u32 cluster, u32 cpu);
void cpu_power_down(u32 cluster, u32 cpu);
void cpucfg_counter_ctrl(bool enable);

#endif /* __CPUCFG_H__ */

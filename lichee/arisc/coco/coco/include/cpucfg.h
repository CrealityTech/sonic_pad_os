/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : cpucfg.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:32
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __CPUCFG_H__
#define __CPUCFG_H__




s32 cpucfg_set_little_endian_address(void *start, void *end);
s32 cpucfg_remove_little_endian_address(void *start, void *end);

u64 timestamp_read(void);

void set_secondary_entry(u32 entry, u32 cpu);
s32 cpucfg_set_cpux_jump_addr(u32 addr);
void sun50i_set_AA32nAA64(u32 cluster, u32 cpu, bool is_aa64);
void cpu_power_up(u32 cluster, u32 cpu);
void cpu_power_down(u32 cluster, u32 cpu);

void cpucfg_cpu_suspend(void);
void cpucfg_cpu_suspend_late(void);

s32 cpucfg_cpu_resume_early(void);
s32 cpucfg_cpu_resume(u32 entrypoint);

s32 cpuidle_suspend(void);
s32 cpuidle_resume(void);
#endif





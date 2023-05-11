/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : psci.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:34
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __PSCI_H__
#define __PSIC_H__

struct psci_paras {
	u32 mpidr;
	u32 entrypoint;
	u32 cpu_state;
	u32 cluster_state;
	u32 sys_state;
};


#define ARISC_POWER_ON		0
#define ARISC_POWER_OFF		3

typedef enum {
	arisc_system_shutdown = 0,
	arisc_system_reboot = 1,
	arisc_system_reset = 2
} arisc_system_state_t;


int psci_suspend_core_init(void);
int psci_suspend_prepare_init(void);
int psci_suspend_init(void);
int psci_platform_init(void);
int set_wakeup_src_init(void);



s32 default_wakeup_handler(void *parg, u32 intno);



#endif


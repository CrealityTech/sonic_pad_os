/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                         clock control unit module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : power.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-11-22
* Descript: module power manager.
* Update  : date                auther      ver     notes
*           2012-11-22 16:44:06 Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "ccu_i.h"
#include "cpucfg_regs.h"

static u32 hosc_lock;
/*
*********************************************************************************************************
*                                    SET POWER OFF STATUS OF HWMODULE
*
* Description:  set the power off gating status of a specific module.
*
* Arguments  :  module  : the module ID which we want to set power off gating status.
*               state   : the power off status which we want to set, the detail please
*                         refer to the status of power-off gating.
*
* Returns    :  OK if set module power off gating status succeeded, others if failed.
*********************************************************************************************************
*/
s32 ccu_set_poweroff_gating_state(s32 module, s32 state)
{
	return OK;
}

struct notifier *hosc_notifier_list = NULL;

s32 ccu_24mhosc_reg_cb(__pNotifier_t pcb)
{
	/* insert call-back to hosc_notifier_list. */
	return notifier_insert(&hosc_notifier_list, pcb);
}

s32 ccu_24mhosc_disable(void)
{
	return OK;
}

s32 ccu_24mhosc_enable(void)
{
	return OK;
}

s32 is_hosc_lock(void)
{
	return hosc_lock;
}

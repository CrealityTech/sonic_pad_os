/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                               standby module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : standby.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-14
* Descript: standby module public header.
* Update  : date                auther      ver     notes
*           2012-5-14 8:55:32   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "standby_i.h"
#include "wakeup_source.h"

u32 before_crc;
u32 after_crc;

/*
*********************************************************************************************************
*                                       INIT SUPER-STANDBY
*
* Description:  initialize super-standby module.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize succeeded, others if failed.
*********************************************************************************************************
*/
s32 standby_init(void)
{
	before_crc = 0;
	after_crc = 0;

	wakeup_timer_init();

	return OK;
}

/*
*********************************************************************************************************
*                                       EXIT SUPER-STANDBY
*
* Description:  exit super-standby module.
*
* Arguments  :  none.
*
* Returns    :  OK if exit succeeded, others if failed.
*********************************************************************************************************
*/
s32 standby_exit(void)
{
	return OK;
}

int long_jump(int (*fn)(void *arg), void *arg)
{
	INF("fn:%x,arg:%x\n", fn, arg);

	return (*fn)(arg);
}


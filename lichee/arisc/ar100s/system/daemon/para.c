/*
*********************************************************************************************************
*                                                AR200 SYSTEM
*                                     AR200 Software System Develop Kits
*                                                startup module
*
*                                    (c) Copyright 2012-2016, superm China
*                                             All Rights Reserved
*
* File    : para.c
* By      : superm
* Version : v1.0
* Date    : 2012-5-13
* Descript: startup module.
* Update  : date                auther      ver     notes
*           2013-5-23 9:32:20   superm      1.0     Create this file.
*********************************************************************************************************
*/

#include "daemon_i.h"
#include "para.h"

struct arisc_para arisc_para __attribute__ ((section("dts_paras"))) = {
	.para_info      = 3,
	.para_magic     = 0x73555043,
};

/*
*********************************************************************************************************
*                                       C STARTUP ENTRY
*
* Description:  the c entry of startup.
*
* Arguments  :  none.
*
* Returns    :  none.
*********************************************************************************************************
*/
void arisc_para_init(void)
{
	cpucfg_set_little_endian_address((void *)(ARISC_DTS_BASE), (void *)(ARISC_DTS_BASE + ARISC_DTS_SIZE));
}

void set_paras(void)
{
}


/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                dram crc module
*
*                                    (c) Copyright 2012-2016, Superm Wu China
*                                             All Rights Reserved
*
* File    : dram_crc.c
* By      : Superm Wu
* Version : v1.0
* Date    : 2012-9-18
* Descript: set dram crc paras.
* Update  : date                auther      ver     notes
*           2012-9-18 19:08:23  Superm Wu   1.0     Create this file.
*********************************************************************************************************
*/
#include "ibase.h"
#include "task.h"
#include "task_handle.h"
#include "msgmanager.h"

#include "standby_dram_crc.h"

u32 dram_crc_enable  = 1;
u32 dram_crc_src     = 0x40000000;
u32 dram_crc_len     = (1024 * 1024);

__TASK_INITIAL(dram_crc_task, DEFAULT_TASK_PRIORITY, TASK_GID_DEFAULT, SET_DRAM_CRC_PARAS);

s32 standby_set_dram_crc_paras(task_struct *task)
{
	u32 paras[3];
	u32 *paras_p = paras;

	get_paras_form_msg(task->msgparalist, (void **)&paras_p, sizeof(paras));

	dram_crc_enable = paras[0];
	dram_crc_src    = paras[1];
	dram_crc_len    = paras[2];

	LOG("dram_crc_enable:%x, dram_crc_src:%x; dram_crc_len:%x\n",
		dram_crc_enable, dram_crc_src, dram_crc_len);

	return OK;
}

s32 standby_dram_crc_enable(void)
{
	return dram_crc_enable;
}

u32 standby_dram_crc(void)
{
	u32 *pdata = (u32 *)(dram_crc_src);
	u32 crc = 0;

	LOG("crc begin src:%x len:%x\n", dram_crc_src, dram_crc_len);
	while (pdata < (u32 *)(dram_crc_src + dram_crc_len))
	{
		crc += *pdata;
		pdata++;
	}
	LOG("crc finish...\n");

	return crc;
}

s32 standby_dram_crc_init(void)
{
	add_task(&dram_crc_task,(__taskCBK_t)standby_set_dram_crc_paras);

	return OK;
}



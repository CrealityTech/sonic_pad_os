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
#include "include.h"

u32 dram_crc_enable  = 0;
u32 dram_crc_src     = 0x40000000;
u32 dram_crc_len     = (1024 * 1024);

s32 standby_set_dram_crc_paras(u32 enable, u32 src, u32 len)
{
	dram_crc_enable = enable;
	dram_crc_src    = src;
	dram_crc_len    = len;

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

	INF("crc begin src:%x len:%x\n", dram_crc_src, dram_crc_len);
	while (pdata < (u32 *)(dram_crc_src + dram_crc_len)) {
		crc += *pdata;
		pdata++;
	}
	INF("crc finish...\n");

	return crc;
}


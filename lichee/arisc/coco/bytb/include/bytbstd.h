/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : bytbstd.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-08-08 11:28
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __BYTBSTD_H__
#define __BYTBSTD_H__

#include "ioctl.h"
#include "ccu.h"

void __time_cdelay(u32 cycles);

static inline s32 div(s32 value,s32 di)
{
	s32 (*fn)(s32 ,s32);
	fn = __div;

	fn = (typeof(fn))(0x00ffffff & (u32)fn);

	return (*fn)(value,di);
}



static inline void __time_udelay(u32 us)
{
	volatile u32 cycles;
	u32 cpus_freq;
	ccu_device * prcm;

	prcm = (ccu_device *)ioctl(PRCM_DEVICE_ID,GET_PRCMDEVICE,(u32)0);

	if (us == 0)
		return;

#ifdef CFG_EVB_FLATFORM
	cpus_freq = prcm->freq;
#elif defined CFG_FPGA_FLATFORM
	cpus_freq = 24000000;
#endif

//	cycles = (cpus_freq * us + (1024 - 1)) >> 10;
	cycles = 10;
//	div(cycles,1000000);
	while(cycles --);
//	{
//		printf("cycle %d\n",cycles);
//	}
//	cnt_delay(cycles/3);
//	cnt64_udelay(cycles);

}







#endif  //__LIBRARY_H__

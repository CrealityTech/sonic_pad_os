/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : cpus_math.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:32
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __CPUS_MATH_H__


#define square(x) x*x

static inline u32 integer_log2(u32 m)
{
	u32 n = 0;

	while (m >= 2)
	{
		m = m >> 1;
		n++;
	}

	return n;
}


#endif


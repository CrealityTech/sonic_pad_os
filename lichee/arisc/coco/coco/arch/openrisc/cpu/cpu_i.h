/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                 cpu module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : cpu_i.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-3
* Descript: cpu module internal header.
* Update  : date                auther      ver     notes
*           2012-5-3 8:48:21    Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __CPU_I_H__
#define __CPU_I_H__

#include "ibase.h"
#include "spr_defines.h"
#include "driver.h"

//local functions
void cpu_exception_handler_entry_c(u32 exception, u32 epc, u32 *pstack);

#endif  //__CPU_I_H__

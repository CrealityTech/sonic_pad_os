/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : ibase.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:33
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __BASE_H__
#define __BASE_H__

//general header
#include <stdarg.h>     //use gcc stardard header file
#include <stddef.h>     //use gcc stardard define keyword, like size_t, ptrdiff_t
#include "./types.h"
#include "./error.h"

#include "lib_inc.h"
#include "platform_config.h"
#include "./device.h"
//debugger
#include "debugger.h"
#include "time.h"
#include "./dbgs.h"

//libary
#include "./library.h"
#include "conf.h"



//enable and disable interrupt
extern s32  cpu_disable_int(void);
extern void cpu_enable_int(int cpsr);

//cpu initialize
extern void cpu_init(void);

#define TICK_PER_SEC                    (100)
#endif //__INCLUDE_H__

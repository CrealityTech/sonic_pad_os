/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : includes.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-07-16 09:43
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __BINCLUDES_H__
#define __BINCLUDES_H__

#include <stdarg.h>     //use gcc stardard header file
#include <stddef.h>     //use gcc stardard define keyword, like size_t, ptrdiff_t
#include "types.h"
#include "./error.h"
#include "./device.h"
#include "conf.h"
#include "trap.h"
#include "bytbstd.h"


void bytb_init(void);

#endif

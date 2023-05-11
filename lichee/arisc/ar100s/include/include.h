/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                              include master
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : include.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-2
* Descript: the include mater header.
* Update  : date                auther      ver     notes
*           2012-5-2 14:48:33   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include "platform_regs.h"
#include "cpucfg_regs.h"
#include "irqnum_config.h"

/* general header */
#include <stdarg.h>		/* use gcc stardard header file */
#include <stddef.h>		/* use gcc stardard define keyword, like size_t, ptrdiff_t */
#include "autoconf.h"
#include "platform_config.h"
#include "./types.h"
#include "./error.h"

/* messages define */
#include "./messages.h"

/* system headers */
#include "./system/cpu.h"
#include "./system/daemon.h"
#include "./system/debugger.h"
#include "./system/notifier.h"
#include "./system/message_manager.h"
#include "./system/para.h"

/* driver headers */
#include "./driver/prcm.h"
#include "./driver/cpucfg.h"
#include "./driver/dram.h"
#include "./driver/hwmsgbox.h"
#include "./driver/intc.h"
#include "./driver/twi.h"
#include "./driver/pin.h"
#include "./driver/pmu.h"
#include "./driver/timer.h"
#include "./driver/uart.h"
#include "./driver/watchdog.h"

/* sevices */
#include "./service/standby.h"
#include "./service/mem_include.h"

/* debugger */
#include "./dbgs.h"

/* libary */
#include "./library.h"

#endif /* __INCLUDE_H__ */

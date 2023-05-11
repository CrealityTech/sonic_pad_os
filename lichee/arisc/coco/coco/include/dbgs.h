/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : dbgs.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:33
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __DBGS_H__
#define __DBGS_H__

#include "conf.h"
/*
 * debug level define, depend on variable debug_level
 * level 0 : dump debug information--error;
 * level 1 : dump debug information--error+warning;
 * level 2 : dump debug information--error+warning+log;
 * level 3 : dump debug information--error+warning+log+information;
 */

#define DEBUG_ON 1
#ifdef DEBUG_ON
/* debug levels */
#define DEBUG_LEVEL_INF    ((u32)1 << 0)
#define DEBUG_LEVEL_LOG    ((u32)1 << 1)
#define DEBUG_LEVEL_WRN    ((u32)1 << 2)
#define DEBUG_LEVEL_ERR    ((u32)1 << 3)

#ifdef CFG_DEBUG_INF
#define INF(...)    debugger_printf(DEBUG_LEVEL_INF, __VA_ARGS__)
#else
#define INF(...)
#endif

#ifdef CFG_DEBUG_WRN
#define WRN(...)    debugger_printf(DEBUG_LEVEL_WRN, "WRN:" __VA_ARGS__)
#else
#define WRN(...)
#endif

#ifdef CFG_DEBUG_ERR
#define ERR(...)    debugger_printf(DEBUG_LEVEL_ERR, "ERR:" __VA_ARGS__)
#else
#define ERR(...)
#endif

#ifdef CFG_DEBUG_LOG
#define LOG(...)    debugger_printf(DEBUG_LEVEL_LOG,__VA_ARGS__)
#else
#define LOG(...)
#endif

#else //DEBUG_ON
#define INF(...)
#define WRN(...)
#define ERR(...)
#define LOG(...)

#endif //DEBUG_ON

#define ASSERT(e)   (((void)0))
#define printk(...)    debugger_printf(0xf,__VA_ARGS__)


#define DAEMON_ONCE_TICKS 500
#define RTC_RECORD_REG   (RTC_REG_BASE + 0x10c)
#define save_state_flag(x) writel(x, RTC_RECORD_REG);
#endif  //__DBGS_H__

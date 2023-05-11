/*
*********************************************************************************************************
*                                                AR100 System
*                                     AR100 Software System Develop Kits
*                                               Debugger Module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : dbgs.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-4-25
* Descript: serial debugger public header.
* Update  : date                auther      ver     notes
*           2012-4-25 16:19:40  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __DBGS_H__
#define __DBGS_H__

/*
 * debug level define, depend on variable debug_level
 * level 0 : dump debug information--error;
 * level 1 : dump debug information--error+warning;
 * level 2 : dump debug information--error+warning+log;
 * level 3 : dump debug information--error+warning+log+information;
 */

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
#define LOG(...)    debugger_printf(DEBUG_LEVEL_LOG, __VA_ARGS__)
#else
#define LOG(...)
#endif

#define ASSERT(e)   (((void)0))
#define printk(...)    debugger_printf(0xf, __VA_ARGS__)

#endif /*__DBGS_H__*/

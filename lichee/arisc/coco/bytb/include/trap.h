/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : trap.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-07-16 09:43
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __TRAP_H__
#define __TRAP_H__

/*
 * debug level define, depend on variable debug_level
 * level 0 : dump debug information--error;
 * level 1 : dump debug information--error+warning;
 * level 2 : dump debug information--error+warning+log;
 * level 3 : dump debug information--error+warning+log+information;
 */

#define BYTD_DEBUG_ON 1
#ifdef BYTD_DEBUG_ON
/* debug levels */
#define BYTD_DEBUG_LEVEL_INF    ((u32)1 << 0)
#define BYTD_DEBUG_LEVEL_LOG    ((u32)1 << 1)
#define BYTD_DEBUG_LEVEL_WRN    ((u32)1 << 2)
#define BYTD_DEBUG_LEVEL_ERR    ((u32)1 << 3)

#ifdef CFG_DEBUG_INF
#define BYTD_INF(...)    bytd_debugger_printf(BYTD_DEBUG_LEVEL_INF, __VA_ARGS__)
#else
#define BYTD_INF(...)
#endif

#ifdef CFG_DEBUG_WRN
#define BYTD_WRN(...)    bytd_debugger_printf(BYTD_DEBUG_LEVEL_WRN, "WRN:" __VA_ARGS__)
#else
#define BYTD_WRN(...)
#endif

#ifdef CFG_DEBUG_ERR
#define BYTD_ERR(...)    bytd_debugger_printf(BYTD_DEBUG_LEVEL_ERR, "ERR:" __VA_ARGS__)
#else
#define BYTD_ERR(...)
#endif

#ifdef CFG_DEBUG_LOG
#define BYTD_LOG(...)    bytd_debugger_printf(BYTD_DEBUG_LEVEL_LOG, __VA_ARGS__)
#else
#define BYTD_LOG(...)
#endif

#else /* BYTD_DEBUG_ON */
#define BYTD_INF(...)
#define BYTD_WRN(...)
#define BYTD_ERR(...)
#define BYTD_LOG(...)

#endif /* BYTD_DEBUG_ON */

#define printf(...) printf__(__VA_ARGS__)

s32 printf__(const char *format, ...);
s32 bytd_debugger_printf(u32 level, const char *format, ...);
s32 ioctl(u32 devid, u32 requset, ...);
s32 create_task(u32 task_struct, u32 task_handle);
s32 wakeup_task(u32 task_struct);
s32 sleep_task(u32 task_struct);
s32 inh_msg(u32 src, u32 dest);
s32 syscall(u32 type, u32 d0, u32 d1, u32 d2);
s32 time_mdelay_irqdisable(u32 ms);

u32 ms_to_ticks(u32 ms);



#endif



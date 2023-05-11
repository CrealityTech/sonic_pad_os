/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                watchdog  module
*
*                                    (c) Copyright 2012-2016, Superm Wu China
*                                             All Rights Reserved
*
* File    : Watchdog.h
* By      : Superm Wu
* Version : v1.0
* Date    : 2012-9-18
* Descript: watchdog controller public interfaces.
* Update  : date                auther      ver     notes
*           2012-9-18 19:08:23  Superm Wu   1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __WATCHDOG_H__
#define __WATCHDOG_H_
#ifdef CFG_WATCHDOG_USED
/*
*********************************************************************************************************
*                                       INIT WATCHDOG
*
* Description:  initialize watchdog.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize watchdog succeeded, others if failed.
*********************************************************************************************************
*/
s32 watchdog_init(void);

/*
*********************************************************************************************************
*                                       EXIT WATCHDOG
*
* Description:  exit watchdog.
*
* Arguments  :  none.
*
* Returns    :  OK if exit watchdog succeeded, others if failed.
*********************************************************************************************************
*/
s32 watchdog_exit(void);

/*
*********************************************************************************************************
*                                       ENABLE WATCHDOG
*
* Description:  enable watchdog.
*
* Arguments  :  none.
*
* Returns    :  OK if enable watchdog succeeded, others if failed.
*********************************************************************************************************
*/
s32 watchdog_enable(void);

/*
*********************************************************************************************************
*                                       DISABLE WATCHDOG
*
* Description:  disable watchdog.
*
* Arguments  :  none.
*
* Returns    :  OK if disable watchdog succeeded, others if failed.
*********************************************************************************************************
*/
s32 watchdog_disable(void);

#else
static inline s32 watchdog_init(void) { return 0; }
static inline s32 watchdog_exit(void) { return 0; }
#endif /* CFG_WATCHDOG_USED */

#endif /* __WATCHDOG_H__ */

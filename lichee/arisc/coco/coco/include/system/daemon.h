/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : daemon.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:33
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __DAEMON_H__
#define __DAEMON_H__

#define DAEMON_RUN_NOTIFY   (0x100)

/*
*********************************************************************************************************
*                                       STARTUP ENTRY
*
* Description:  the entry of startup.
*
* Arguments  :  none.
*
* Returns    :  none.
*********************************************************************************************************
*/
void startup_entry(void);

#endif  //__DAEMON_H__

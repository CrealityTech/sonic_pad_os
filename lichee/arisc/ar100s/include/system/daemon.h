/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                daemon module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : daemon.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-13
* Descript: daemon module public header.
* Update  : date                auther      ver     notes
*           2012-5-13 15:06:10  Sunny       1.0     Create this file.
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

int daemon_register_service(__pNotifier_t pcb);

#endif /* __DAEMON_H__ */

/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                             Notifier Manager
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : notifier.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-11-23
* Descript: notifier manager.
* Update  : date                auther      ver     notes
*           2012-11-23 13:39:05 Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __NOTIFIER_H__
#define __NOTIFIER_H__

//the status of notifier
#define NOTIFIER_USED   1
#define NOTIFIER_FREE   0
#define NOTIFIER_MAX	8

//notifier structure
typedef struct notifier
{
	u32              flag;  //bit0 used to indicate free or use
	__pNotifier_t    pcb;   //cb function pointer
	struct notifier *next;  //next notifier object
	void 		*parg;
} notifier_t;

s32 notifier_init(void);
s32 notifier_exit(void);

s32 notifier_insert(struct notifier **head, __pNotifier_t pcb, void *parg);
s32 notifier_delete(struct notifier **head, __pNotifier_t pcb);
s32 notifier_notify(struct notifier **head, u32 message);

#endif  //__NOTIFIER_H__

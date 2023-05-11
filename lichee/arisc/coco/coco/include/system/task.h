/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : task.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:34
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __TASK_H__
#define __TASK_H__

#include "list.h"

typedef struct __task_struct{
	u8 handle;
	u8 status;
	u8 priority;
	u8 keep_run;
	u16 taskid;
	u16 gtaskid;
	u64 totle_time;
	u32 once_time;
	u32 ref_count;
	struct list_head list;
	struct list_head *msgparalist;
	struct list_head msglist;
	s32 (*callback)(void *para);
	s32 (*feedback)(void *para);
	void *para;
} task_struct;

#define TASK_SUSPEND		1
#define TASK_READ		2
#define TASK_RUN		3
#define TASK_DONE		4
#define TASK_DEAD		5


#define DEFAULT_TASK_HANDLE	0
#define DEFAULT_TASK_PRIORITY	10

#define ARISC_MESSAGE_BASE          (0x10)
#define ARISC_CPUX_DVFS_REQ		 (ARISC_MESSAGE_BASE + 0x20)
#define ARISC_CPUX_DVFS_CFG_VF_REQ       (ARISC_MESSAGE_BASE + 0x21)
#define ARISC_DRAM_DVFS_REQ              (ARISC_MESSAGE_BASE + 0x23)


/***********************************************************
 * system taskid <=100
 * user taskid >=100
 *
 * system group taskid <=50
 * user group taskid >=50
 **********************************************************/
#define __TASK_INITIAL(name, pri, gid, hdle) \
	struct __task_struct name = {	.taskid = 0,	\
					.gtaskid = gid,		\
					.priority = pri,	\
					.handle = hdle,		\
					.totle_time = 0,	\
					.once_time = 0,		\
					.para = NULL,		\
					.ref_count = 0,		\
					.msgparalist = NULL}

extern struct list_head task_list;
extern struct list_head free_list;

typedef s32  (* __taskCBK_t)(void *p_arg); //call-back

void task_init(void);
void active_task(task_struct *task);
void find_next_task(task_struct **cur_task);
void add_task(task_struct *task, __taskCBK_t cb);
void resume_task(task_struct *task);
void suspend_task(task_struct *task);
void inherit_msg(task_struct *src,task_struct *dest);
#endif


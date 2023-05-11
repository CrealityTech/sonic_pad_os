/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : msgmanager.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:30
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "ibase.h"
#include "driver.h"
#include "msgmanager.h"
#include "task.h"
#include "list.h"
#include "cpucfg.h"
#include "bytbmsg.h"
#include "spinlock.h"
#include "syscall.h"


struct msgblock_head *bytb_allock_msg(u32 block_num)
{
	struct msgblock_head *h;

	h =(struct msgblock_head *)syscall(MSG_ALLOCK,block_num,0,0);
	return h;
}


s32 bytb_free_msg(struct msgblock_head *head)
{
	u32 ret;

	ret = syscall(MSG_FREE,(u32)head,0,0);

	return ret;
}

s32 bytb_resolve_msg(u32 msgaddr)
{
	u32 ret;

	ret = syscall(MSG_RESOLVE,msgaddr,0,0);

	return ret;
}


s32 bytb_resolve_feedback(u32 msgaddr)
{
	u32 ret;

	ret = syscall(FEEDBACK_RESOLVE,msgaddr,0,0);

	return ret;
}

void bytb_get_paras_form_msg(struct list_head *head, void **para,u32 len)
{
	syscall(MSG_GETPARAS,(u32)head,(u32)para,len);
}

void bytb_set_paras_to_msg(struct list_head *head, void *para,u32 len)
{
	syscall(MSG_SETPARAS,(u32)head,(u32)para,len);
}


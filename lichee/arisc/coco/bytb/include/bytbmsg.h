/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : bytbmsg.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-07-16 09:43
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __BYTBMSG_H__
#define __BYTBMSG_H__

#include "includes.h"

int bytb_msgmanager_init(void);
struct msgblock_head *bytb_allock_msg(u32 block_num);
s32 bytb_free_msg(struct msgblock_head *head);
s32 bytb_resolve_msg(u32 msgaddr);
s32 bytb_resolve_feedback(u32 msgaddr);
void bytb_get_paras_form_msg(struct list_head *head, void **para,u32 len);
void bytb_set_paras_to_msg(struct list_head *head, void *para,u32 len);

#endif


/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : msgmanager.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:34
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __MSGMANAGER_H__
#define __MSGMANAGER_H__

#include "ibase.h"
#include "list.h"
#include "hwmsgbox.h"

#define MSGBLOCK_SIZE 11

#define MSGPOOL_START   0x419f8000
#define MSGPOOL_END     0x419f8900
#define MSGID_TABLE     0x419f8900
#define MSGID_END       0x419f9000

#define MSG_STATE_ALLOCKED	0x10
#define MSG_STATE_SENDING	0x11
#define MSG_STATE_RECEIVED	0x13
#define MSG_STATE_RESOLVE	0x14
#define MSG_STATE_RESOLVE_FINISH	0x15
#define MSG_STATE_FEEDBACK		0x16
#define MSG_STATE_STORAGE		0x17
#define MSG_STATE_FREE		0x18

#define MESSAGE_ATTR_ASYN        0
#define MESSAGE_ATTR_SYN        1
#define MESSAGE_ATTR_QUERY        2

#define MSGID_TABLE_COUNT	8
#define MSGID_SYN		99
#define MSGID_ASYN		0

#define RET_ATTR_ASYN		0x1000
#define RET_ATTR_SYN		0x1000
#define RET_ATTR_QUERY		0x2000

struct msgblock {
	struct list_head list;
	u32 paras[MSGBLOCK_SIZE];
};

typedef union {
	u32 word;
	struct {
		u8 result;
		u8 type;
		u8 attr;
		u8 state;
	}bytes;
}msg_status_t;

struct msgblock_head {
	struct list_head list;
	struct list_head wait_list;
	u32	state;
	u32	type;
	u32	attr;
	u32	result;
//	msg_status_t	status;
	u32	length;
	s32	msgid;
	s32	ref;
};

#define _ALIGN(addr,size)	(((addr)+((size)-1))/size)
#define BLOB_ALIGN(len)	_ALIGN(len, MSGBLOCK_SIZE * 4)

int msgmanager_init(void);
s32 get_free_msgid(void);
s32 free_msgid(s32 msgid);
struct msgblock_head *allock_msg(u32 block_num);
s32 free_msg(struct msgblock_head *head);
s32 resolve_msg(u32 msgaddr);
s32 resolve_feedback(u32 msgaddr);
void get_paras_form_msg(struct list_head *head, void **para,u32 len);
void set_paras_to_msg(struct list_head *head, void *para,u32 len);
u32 get_block_length_from_paras(u32 paras_size);

msgbox_device *request_msgbox(void);

#endif


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

//LIST_HEAD(msgfree_list);

struct hwspinlock *msg_lock;
struct hwspinlock *msgtable_lock;

struct msgblock *msgfree_head;
struct list_head *msgfree_list;
u32 *msgid_table;

static int msgpool_init(void)
{
	struct msgblock *p;
	u32 count;
	u32 msgblock_maxnum = 0;
	u32 msgpool_size = 0;

	msgpool_size = MSGPOOL_END - MSGPOOL_START;

	msgid_table = (u32 *)MSGID_TABLE;
	msgfree_head = (struct msgblock *)MSGPOOL_START;
	INIT_LIST_HEAD(&msgfree_head->list);

	msgfree_list = &msgfree_head->list;
	msgblock_maxnum = msgpool_size / sizeof(struct msgblock) - 1;
	for (p = (msgfree_head+1), count = 0; count < msgblock_maxnum; p++, count++)
	{
		list_add_tail(&(p->list), msgfree_list);
	}

	return 0;
}

int msgmanager_init(void)
{
	/* config message pool area as little endian */
	cpucfg_set_little_endian_address((void *)MSGPOOL_START, (void *)MSGPOOL_END);
	//clear message pool as initialize status
	memset((void *)MSGPOOL_START, 0, MSGID_END - MSGPOOL_START);
	msgpool_init();

	msg_lock = query_spinlock(MSG_SPINLOCK);
	msgtable_lock = query_spinlock(MSGTABLE_SPINLOCK);

	return 0;
}

s32 get_free_msgid(void)
{
	u8 i;
	u8 j;
	u32 ret = 0;

	ret = hwspin_lock_timeout(msgtable_lock, 1000);
	for(i = 0;i < MSGID_TABLE_COUNT; i++) {
		for(j = 0; j < MSGID_TABLE_COUNT; j++) {
			if(msgid_table[j + i * MSGID_TABLE_COUNT] == 0) {
				ret = (i + 1) + 10 * (j + 1);
				msgid_table[j + i * MSGID_TABLE_COUNT] = ret;
				hwspin_unlock(msgtable_lock);
				return ret;
			}
		}
	}

	hwspin_unlock(msgtable_lock);
	return -1;
}

s32 free_msgid(s32 msgid)
{
	u8 i;
	u8 j;
	u32 ret = 0;

	ret = hwspin_lock_timeout(msgtable_lock, 1000);
	i = msgid%10;
	j = msgid/10;

	i = i-1;
	j = j-1;
	msgid_table[j + i * MSGID_TABLE_COUNT] = 0;
	hwspin_unlock(msgtable_lock);

	return 0;
}

struct msgblock_head *allock_msg(u32 block_num)
{
	struct msgblock_head *h;
	struct list_head *p;
	u32 i =0;
	int ret;

	ret = hwspin_lock_timeout(msg_lock, 1000);
	//	if (ret < 0)
	//		return NULL;


	if(sizeof(struct msgblock_head) == sizeof(struct msgblock)) {
		p = msgfree_list->next;
		list_del(p);
		h = container_of(p,struct msgblock_head,list);
		INIT_LIST_HEAD(&(h->list));
	}
	else {
		hwspin_unlock(msg_lock);
		return NULL;
	}

	for(i = 0;i < block_num; i++) {
		if(msgfree_list->next != msgfree_list) {
			p = msgfree_list->next;
			list_del(p);
			list_add_tail(p,&(h->list));
		}
		else {
			hwspin_unlock(msg_lock);
			return NULL;
		}
	}

	h->ref = 1;
	hwspin_unlock(msg_lock);
	return h;
}

s32 free_msg(struct msgblock_head *head)
{
	struct list_head *p;
	s32 ret;

	if(head == NULL)
		return OK;

	ret = hwspin_lock_timeout(msg_lock, 1000);

	if(head->ref > 0)
		head->ref--;

	if(head->ref > 0) {
		hwspin_unlock(msg_lock);
		return OK;
	}


	while (1) {
		p = head->list.next;
		if(head->list.next != &head->list) {
			list_del(p);
			list_add_tail(p,msgfree_list);
		}
		else {
			list_del(p);
			list_add_tail(p,msgfree_list);
			break;
		}
	}

	hwspin_unlock(msg_lock);

	return OK;
}

s32 resolve_msg(u32 msgaddr)
{
	struct msgblock_head *head;
	u32 cpsr = 0;
	u32 ret = 0;

	task_struct *tp = NULL;
	task_struct *rtp = NULL;

	head = (struct msgblock_head *)msgaddr;
	if (head == NULL)
		return -EINVAL;

	cpsr = cpu_disable_int();

	//hexdump("head",(char *)head,0x150);
	list_for_each_entry(tp,&task_list,list) {
		if(tp->handle == head->type) {
			//list_add_tail(&head->wait_list,&(tp->msglist));
			tp->msglist.next = &(head->list);
			rtp = tp;
		}
	}

	if(rtp != NULL) {
		resume_task(rtp);
		cpu_enable_int(cpsr);
		return ret;
	}

	list_for_each_entry(tp,&free_list,list) {
		if(tp->handle == head->type) {
			tp->msgparalist = &(head->list);
			INIT_LIST_HEAD(&tp->msglist);
			rtp = tp;
		}
	}

	cpu_enable_int(cpsr);
	if(rtp != NULL)
		resume_task(rtp);
	return ret;
}

s32 resolve_feedback(u32 msgaddr)
{
	struct msgblock_head *head;
	task_struct *tp;
	u32 cpsr = 0;

	head = (struct msgblock_head *)msgaddr;

	cpsr = cpu_disable_int();
	list_for_each_entry(tp,&task_list,list) {
		if(tp->handle == head->type)
			tp->feedback(tp);
	}

	list_for_each_entry(tp,&free_list,list) {
		if(tp->handle == head->type)
			tp->feedback(tp);
	}

	cpu_enable_int(cpsr);
	return OK;
}

void get_paras_form_msg(struct list_head *head, void **para,u32 len)
{
	struct msgblock *bmsg;

	bmsg = container_of(head->next,struct msgblock,list);

	/*LOG("%s: bmsg = 0x%x, bmsg->paras[0] = %d, bmsg->paras[1]=%d\n",
		__func__, (unsigned int )bmsg, bmsg->paras[0], bmsg->paras[1]);*/
	while(len) {
		if(len <= (MSGBLOCK_SIZE * 4)) {
			memcpy(*para,(const void *)(&(bmsg->paras[0])),len);
			len = 0;
		}
		else {
			memcpy(*para,(const void *)(&(bmsg->paras[0])),MSGBLOCK_SIZE * 4);
			len = len - MSGBLOCK_SIZE * 4;
		}
		bmsg = container_of(bmsg->list.next,struct msgblock,list);
		*para = (void *)((u32 *)*para + MSGBLOCK_SIZE);
	}
}

void set_paras_to_msg(struct list_head *head, void *para,u32 len)
{
	struct msgblock *bmsg;

	bmsg = container_of(head->next,struct msgblock,list);

	while(len) {
		if(len <= (4 * MSGBLOCK_SIZE)) {
			memcpy((void *)(&(bmsg->paras[0])),para,len);
			len = 0;
		}
		else {
			memcpy((void *)(&(bmsg->paras[0])),para,MSGBLOCK_SIZE * 4);
			len = len - (4 * MSGBLOCK_SIZE);
		}
		bmsg = container_of(bmsg->list.next,struct msgblock,list);
		para = (void *)((u32 *)para + MSGBLOCK_SIZE);
	}
}

u32 get_block_length_from_paras(u32 paras_size)
{
	u32 length;

	if(!paras_size % (4 * MSGBLOCK_SIZE))
		length = paras_size / (4 * MSGBLOCK_SIZE);
	else
		length = paras_size / (4 * MSGBLOCK_SIZE) + 1;

	if(length == 0)
		length = 1;

	return length;
}


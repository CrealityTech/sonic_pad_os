/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : list.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:33
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __LIST_H__
#define __LIST_H__

#include "ibase.h"

#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({                      \
	const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
	(type *)( (char *)__mptr - offsetof(type,member) );})


struct list_head {
	struct list_head *next;
	u32	resverd0;
	struct list_head *prev;
	u32	resverd1;
};

#define LIST_HEAD_INIT(name) { &(name),0,&(name),0 }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
* list_for_each_entry>-->------iterate over list of given type
* @pos:>-------the type * to use as a loop cursor.
* @head:>------the head for your list.
* @member:>----the name of the list_struct within the struct.
*/
#define list_for_each_entry(pos, head, member) \
	for (pos = list_entry((head)->next, typeof(*pos), member);\
	     &pos->member != (head); \
	     pos = list_entry(pos->member.next, typeof(*pos), member))

static inline void __list_add(struct list_head *_new,
				struct list_head *prev,
				struct list_head *next)
{
	next->prev = _new;
	_new->next = next;
	_new->prev = prev;
	prev->next = _new;
}

static inline void list_add_tail(struct list_head *_new, struct list_head *head)
{
	__list_add(_new, head->prev, head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = (struct list_head*)NULL;
	entry->prev = (struct list_head*)NULL;
}


#endif


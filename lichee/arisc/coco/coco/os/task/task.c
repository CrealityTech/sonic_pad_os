/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : task.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:32
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "ibase.h"
#include "task.h"
#include "msgmanager.h"

LIST_HEAD(task_list);
LIST_HEAD(free_list);

void task_init(void)
{
	INIT_LIST_HEAD(&task_list);
	INIT_LIST_HEAD(&free_list);
}

void active_task(task_struct *task)
{
	struct msgblock_head *head;
	msgbox_device *mdev;
	u32 cpsr = 0;
	u32 ret = 0;

	task->status = TASK_RUN;
	ret = (task->callback)(task);
	task->status = TASK_DONE;

	if ((task->msgparalist != NULL) && (task->keep_run == 0)) {
		cpsr = cpu_disable_int();
		head = container_of(task->msgparalist, struct msgblock_head, list);
		task->msgparalist = NULL;
		if (head->attr == MESSAGE_ATTR_ASYN) {
			free_msg(head);
			free_msgid(head->msgid);
		} else if (head->attr != MESSAGE_ATTR_ASYN) {
			head->result = ret;
			mdev = request_msgbox();
			msgbox_send(&(mdev->coco_feedback[0]), (u32)head);
		}
		cpu_enable_int(cpsr);
	}

	suspend_task(task);
	if (task->keep_run == 1)
		resume_task(task);
}

void find_next_task(task_struct **cur_task)
{
	u32 cpsr = 0;

	cpsr = cpu_disable_int();

	INF("task_list.next %x\n", task_list.next);
	INF("task_list.prev %x\n", task_list.prev);
	INF("task_list %x\n", &task_list);
	if (task_list.next != &task_list) {
		*cur_task = container_of((task_list.next), task_struct, list);
	} else {
		*cur_task = NULL;
	}
	cpu_enable_int(cpsr);
}

void add_task(task_struct *task, __taskCBK_t cb)
{
	u32 cpsr = 0;

	task_struct *tp;

	cpsr = cpu_disable_int();
	task->callback = cb;
	list_add_tail(&task->list, &(free_list));
	task->status = TASK_SUSPEND;

	list_for_each_entry(tp, &free_list, list) {
		INF("add task : task id %d\n", tp->gtaskid);
	}
	cpu_enable_int(cpsr);
}

void resume_task(task_struct *task)
{
	u32 cpsr = 0;

	cpsr = cpu_disable_int();

	if (task->status == TASK_SUSPEND) {
		list_del(&task->list);
		list_add_tail(&task->list, &task_list);
		task->status = TASK_READ;
	} else {
		task->ref_count++;
	}

	cpu_enable_int(cpsr);

}

void suspend_task(task_struct *task)
{
	u32 cpsr = 0;

	struct msgblock_head *head;

	cpsr = cpu_disable_int();

	if (task->status != TASK_SUSPEND) {
		list_del(&task->list);
		list_add_tail(&task->list, &free_list);
	}
	task->status = TASK_SUSPEND;
	if ((task->ref_count != 0)) {
		if (task->msglist.next != &(task->msglist)) {
			head = container_of(task->msglist.next, struct msgblock_head, list);
			task->msgparalist = &(head->list);
			task->msglist.next = &(task->msglist);
			/* list_del(task->msglist.next); */
		}
		resume_task(task);
		task->ref_count--;
	}

	cpu_enable_int(cpsr);
}

void inherit_msg(task_struct *src, task_struct *dest)
{
	struct msgblock_head *head;
	u32 cpsr = 0;

	cpsr = cpu_disable_int();

	head = container_of(src->msgparalist, struct msgblock_head, list);

	head->ref++;
	dest->msgparalist = src->msgparalist;
	cpu_enable_int(cpsr);
}






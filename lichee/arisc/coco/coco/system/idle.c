/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : daemon.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-04-16 10:54
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "ibase.h"
#include "driver.h"
#include "task.h"
#include "notifier.h"
#include "cpucfg.h"
#include "softtimer.h"
#include "spr_defines.h"
#include "task_handle.h"

//the list of daemon notifier
__TASK_INITIAL(daemon_task,DEFAULT_TASK_PRIORITY,TASK_GID_DAEMON,DEFAULT_TASK_HANDLE);

task_struct idle_task_struct = {
	.taskid = 0,
	.gtaskid = 0,
	.totle_time = 0,
};

struct system_counter {
	u64 idle_count;
	u64 system_count;
};

struct system_counter syscount;

static void daemon(task_struct * task __attribute__ ((__unused__)))
{
	LOG("------------------------------\n");
	LOG("idle count :%d%d  system_count :%d%d \n",
			(u32)(syscount.idle_count >> 32),(u32)(syscount.idle_count & 0xffffffff),
			(u32)(syscount.system_count >> 32),(u32)(syscount.system_count & 0xffffffff));
}

static volatile u32 irq_comming;

s32 irq_monitor(u32 message, void *aux);
s32 irq_monitor(u32 message __attribute__ ((__unused__)),
	void *aux __attribute__ ((__unused__)))
{
	irq_comming = 1;

	return OK;
}

static void idle_task(task_struct * task __attribute__ ((__unused__)))
{
	INF("idle task\n");
	while(!irq_comming)
		;

	INF("break idle task\n");
	irq_comming = 0;
}

struct softtimer daemon_timer;
int idle_loop(void);
int idle_loop(void)
{
	u64 count_counter = 0;
	u64 active_count = 0;
	task_struct *current_task = NULL;

	add_task(&daemon_task,(__taskCBK_t)daemon);

	daemon_timer.ticks = mecs_to_ticks(5000);
	daemon_timer.expires = 0;
	daemon_timer.task = &daemon_task;
	daemon_timer.arg = NULL;

	add_softtimer(&daemon_timer);

	start_softtimer(&daemon_timer,SOFTTIMER_PERIOD);

	while (1) {
		count_counter = current_time_tick();

		find_next_task(&current_task);

		active_count = current_time_tick();
		if(current_task == NULL) {
			idle_task(&idle_task_struct);
			syscount.idle_count += (current_time_tick() - active_count);
		}
		else {
			active_task(current_task);
			current_task->totle_time += current_time_tick() - active_count;
		}
		syscount.system_count += (current_time_tick() - count_counter);

	}

	return 0;
}

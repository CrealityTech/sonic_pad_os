/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : softtimer.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:31
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "ibase.h"
#include "task.h"
#include "notifier.h"
#include "softtimer.h"

struct notifier *softtimer_notify = NULL;
extern struct notifier *irq_status;


void start_softtimer(softtimer_t *stimer, u32 mode)
{
	stimer->start = 1;
	stimer->period = mode;
	stimer->expires = stimer->ticks;
}

void stop_softtimer(softtimer_t *stimer)
{
	stimer->start = 0;
	stimer->expires = 0;
}

static s32 softtimer_notify_handler(u32 message, void *aux)
{
	struct softtimer *timer = (struct softtimer *)aux;

	message = message;
	if(timer->start == 0)
		return OK;

	timer->expires--;

	if(timer->expires == 0) {
		notifier_notify(&irq_status,0);
		resume_task(timer->task);
		timer->expires = timer->ticks;
		if(timer->period == SOFTTIMER_PERIOD)
			timer->start = 1;
		else
			timer->start = 0;
	}

	return OK;
}


s32 add_softtimer(softtimer_t *stimer)
{
	return notifier_insert(&softtimer_notify, softtimer_notify_handler, (void *)stimer);
}


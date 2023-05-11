#ifndef __SOFTTIMER_H__
#define __SOFTTIMER_H__

#include "ibase.h"
#include "task.h"


typedef struct softtimer
{
	s32             ticks;
	s32             expires;
	task_struct     *task;
	void            *arg;
	u32		start;
	u32		period;
} softtimer_t;


void start_softtimer(softtimer_t *stimer, u32 mode);
void stop_softtimer(softtimer_t *stimer);
s32 add_softtimer(softtimer_t *stimer);

#define SOFTTIMER_PERIOD 1
#define SOFTTIMER_SINGLE 0

#endif


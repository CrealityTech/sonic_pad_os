/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                cpu  module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : cpu.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-2
* Descript: cpu operations public header.
* Update  : date                auther      ver     notes
*           2012-5-2 13:29:32   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __CPU_H__
#define __CPU_H__

/* cache operation fucntion */
void cpu_flush_cache(void);

/* enable and disable interrupt */
s32  cpu_disable_int(void);
void cpu_enable_int(int cpsr);

/* cpu initialize */
void cpu_init(void);

/* system exit */
void exit(int i);

/* system time ticks */
s32 time_ticks_init(void);
u32 msec_to_ticks(u32 ms);
u32 current_time_tick(void);

/* build icache coherent */
void icache_coherent(void);

void cpu_enter_doze(void);


/* softtimer */
typedef enum softtimer_onoff {
	SOFTTIMER_OFF = 0x0,
	SOFTTIMER_ON  = 0x1,
} softtimer_onff_e;

typedef struct softtimer {
	s32             cycle;
	s32             expires;
	ar100_cb_t      cb;
	void            *arg;
	u32             start;
} softtimer_t;

s32 add_softtimer(struct softtimer *timer);
void start_softtimer(struct softtimer *timer);
void stop_softtimer(struct softtimer *timer);

#endif /* __CPU_H__ */

/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                timer  module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : timer.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-4-27
* Descript: timer controller public header.
* Update  : date                auther      ver     notes
*           2012-4-27 17:03:52  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __TIMER_H__
#define __TIMER_H__

/* the total number of hardware timer */
#define TIMERC_TIMERS_NUMBER (2)
#define SOFTTIMER_NUMBER     (2)

/* timer work mode */
typedef enum timer_mode {
	TIMER_MODE_PERIOD       = 0x0,  /* period trigger mode */
	TIMER_MODE_ONE_SHOOT    = 0x1,  /* one shoot trigger mode */
} timer_mode_e;

/*
*********************************************************************************************************
*                                       INIT TIMER
*
* Description:  initialize timer.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize timer succeeded, others if failed.
*********************************************************************************************************
*/
s32 timer_init(void);

/*
*********************************************************************************************************
*                                       EXIT TIMER
*
* Description:  exit timer.
*
* Arguments  :  none.
*
* Returns    :  OK if exit timer succeeded, others if failed.
*********************************************************************************************************
*/
s32 timer_exit(void);

/*
*********************************************************************************************************
*                                       REQUEST TIMER
*
* Description:  request a hardware timer.
*
* Arguments  :  phdle   : the callback when the requested timer tick reached.
*               parg    : the argument for the callback.
*
* Returns    :  the handler if request hardware timer succeeded, NULL if failed.
*
* Note       :  the callback execute entironment : CPU disable interrupt response.
*********************************************************************************************************
*/
HANDLE timer_request(__pCBK_t phdle, void *parg);

/*
*********************************************************************************************************
*                                       RELEASE TIMER
*
* Description:  release a hardware timer.
*
* Arguments  :  htimer  : the handler of the released timer.
*
* Returns    :  OK if release hardware timer succeeded, others if failed.
*********************************************************************************************************
*/
s32 timer_release(HANDLE htimer);

/*
*********************************************************************************************************
*                                       START TIMER
*
* Description:  start a hardware timer.
*
* Arguments  :  htimer  : the timer handler which we want to start.
*               period  : the period of the timer trigger, base on us.
*               mode    : the mode the timer trigger, details please
*                         refer to timer trigger mode.
*
* Returns    :  OK if start hardware timer succeeded, others if failed.
*********************************************************************************************************
*/
s32 timer_start(HANDLE htimer, u32 period, u32 mode);


/*
*********************************************************************************************************
*                                       STOP TIMER
*
* Description:  stop a hardware timer.
*
* Arguments  :  htimer  : the timer handler which we want ot stop.
*
* Returns    :  OK if stop hardware timer succeeded, others if failed.
*********************************************************************************************************
*/
s32 timer_stop(HANDLE htimer);

/* timer standby services */
s32  timer_standby_init(void);
s32  timer_standby_exit(void);
bool timer_standby_expired(void);

/* timer delay services */
void time_mdelay(u32 ms);     /* base on hardware timer */
void time_cdelay(u32 cycles); /* base on cpu cycle */
void time_udelay(u32 us);     /* base on hardware counter */
void cnt64_udelay(u32 us);

#define udelay(us) time_udelay(us)
#define mdelay(ms) time_mdelay(ms)


#endif  /*__TIMER_H__*/

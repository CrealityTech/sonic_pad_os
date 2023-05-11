/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                DVFS module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : dvfs.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-13
* Descript: DVFS module public header.
* Update  : date                auther      ver     notes
*           2012-5-13 15:23:44  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __DVFS_H__
#define __DVFS_H__
#include "ibase.h"

#ifdef CFG_DVFS_SERVICE

#ifdef CFG_DVFS_USED
#define DVFS_USED 1
#define AVS_USED 0
#else
#define AVS_USED 1
#define DVFS_USED 0
#endif

#define DVFS_VF_TABLE_MAX 16

#if AVS_USED
//the freq and voltage mapping value for AVS
typedef struct freq_voltage_avs
{
	u32 freq;       //cpu frequency
	u32 voltage;    //voltage for the frequency
	u32 axi_div;    //the divide ratio of AXI bus
	u32 pval;       //the p-sensor value
} freq_voltage_avs_t;

#define P_SENSOR_DELAY                  (1 << 6)
#define P_SENSOR_MAX_TIME               (7 << 1)
#define VOLT_STEP                       20
#define MIN_VOLTAGE                     820
#define P_STEP                          300
#define RADIUS_P                        200
#endif /* AVS_USED */

//the freq and voltage mapping value
typedef struct freq_voltage
{
	u32 freq;       //cpu frequency
	u32 voltage;    //voltage for the frequency
	u32 axi_div;    //the divide ratio of AXI bus
} freq_voltage_t;

s32 dramfreq_init(void);

/*
*********************************************************************************************************
*                                       INIT DVFS
*
* Description:  initialize dvfs module.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize succeeded, others if failed.
*********************************************************************************************************
*/
s32 dvfs_init(void);

/*
*********************************************************************************************************
*                                       EXIT DVFS
*
* Description:  exit dvfs module.
*
* Arguments  :  none.
*
* Returns    :  OK if exit succeeded, others if failed.
*********************************************************************************************************
*/
s32 dvfs_exit(void);

/*
*********************************************************************************************************
*                                       SET FREQ
*
* Description:  set the frequency of PLL1 module.
*
* Arguments  :  freq    : the frequency which we want to set.
*
* Returns    :  OK if set frequency succeeded, others if failed.
*********************************************************************************************************
*/
s32 dvfs_set_freq(u32 freq);
s32 dvfs_config_vf_table(u32 vf_num, u32 vf_addr);

#else /* CFG_DVFS_SERVICE */
static inline s32 dramfreq_init(void) { return 0; }
static inline s32 dvfs_init(void) { return 0; }
static inline s32 dvfs_exit(void) { return 0; }
#endif /* CFG_DVFS_SERVICE */
#endif  //__DVFS_H__

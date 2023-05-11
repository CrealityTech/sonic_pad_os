/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                dram module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : dram.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-4-28
* Descript: dram driver public header.
* Update  : date                auther      ver     notes
*           2012-4-28 10:28:51  Sunny       1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __DRAM_H__
#define __DRAM_H__

#include "autoconf.h"

#ifdef CFG_DDR3
#define DRAM_TYPE_DDR3
#endif

#ifdef CFG_DDR4
#define DRAM_TYPE_DDR4
#endif

#ifdef CFG_LPDDR3
#define DRAM_TYPE_LPDDR3
#endif

#ifdef CFG_LPDDR4
#define DRAM_TYPE_LPDDR4
#endif

#define __dram_para_t dram_para_t

typedef struct dram_para {
	/* normal configuration */
	unsigned int dram_clk;
	/* dram_type, DDR2: 2, DDR3: 3, LPDDR2: 6, LPDDR3: 7, DDR3L: 31 */
	unsigned int dram_type;
	unsigned int dram_dx_odt;
	unsigned int dram_dx_dri;
	unsigned int dram_ca_dri;
	unsigned int dram_para0;

	/* control configuration */
	unsigned int dram_para1;
	unsigned int dram_para2;

	/* timing configuration */
	unsigned int dram_mr0;
	unsigned int dram_mr1;
	unsigned int dram_mr2;
	unsigned int dram_mr3;
	unsigned int dram_mr4;
	unsigned int dram_mr5;
	unsigned int dram_mr6;
	unsigned int dram_mr11;
	unsigned int dram_mr12;
	unsigned int dram_mr13;
	unsigned int dram_mr14;
	unsigned int dram_mr16;
	unsigned int dram_mr17;
	unsigned int dram_mr22;

	unsigned int        dram_tpr0;  /*DRAMTMG0*/
	unsigned int        dram_tpr1;  /*DRAMTMG1*/
	unsigned int        dram_tpr2;  /*DRAMTMG2*/
	unsigned int        dram_tpr3;  /*DRAMTMG3*/
	unsigned int        dram_tpr6;  /*DRAMTMG8*/

	/* reserved for future use */
	unsigned int        dram_tpr10;
	unsigned int        dram_tpr11;
	unsigned int        dram_tpr12;
	unsigned int        dram_tpr13;
	unsigned int        dram_tpr14;
} __dram_para_t;

#define DELAY_FOR_DRAM

extern unsigned int dram_power_save_process(__dram_para_t *para);
extern unsigned int dram_power_up_process(__dram_para_t *para);
#endif /* __DRAM_H__ */

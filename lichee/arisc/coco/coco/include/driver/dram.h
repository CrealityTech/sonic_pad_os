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
typedef struct dram_para
{
	unsigned int	dram_clk;
	unsigned int	dram_type;
	unsigned int	dram_dx_odt;
	unsigned int	dram_dx_dri;
	unsigned int	dram_ca_dri;
	unsigned int	dram_odt_en;
	unsigned int	dram_para1;
	unsigned int	dram_para2;
	unsigned int	dram_mr0;
	unsigned int	dram_mr1;
	unsigned int	dram_mr2;
	unsigned int	dram_mr3;
	unsigned int    dram_mr4;
	unsigned int    dram_mr5;
	unsigned int    dram_mr6;
	unsigned int    dram_mr11;
	unsigned int    dram_mr12;
	unsigned int    dram_mr13;
	unsigned int    dram_mr14;
	unsigned int    dram_mr16;
	unsigned int    dram_mr17;
	unsigned int    dram_mr22;
	unsigned int	dram_tpr0;
	unsigned int	dram_tpr1;
	unsigned int	dram_tpr2;
	unsigned int	dram_tpr3;
	unsigned int    dram_tpr6;
	unsigned int	dram_tpr10;
	unsigned int	dram_tpr11;
	unsigned int	dram_tpr12;
	unsigned int	dram_tpr13;
	unsigned int	dram_reserved;
} dram_para_t;

typedef struct dram_data
{
	unsigned int        dram_data0;
	unsigned int        dram_data1;
	unsigned int        dram_data2;
	unsigned int        dram_data3;
	unsigned int        dram_data4;
	unsigned int        dram_data5;
	unsigned int        dram_data6;
	unsigned int        dram_data7;
	unsigned int        dram_data8;
	unsigned int        dram_data9;
	unsigned int        dram_data10;
	unsigned int        dram_data11;
	unsigned int        dram_data12;
	unsigned int        dram_data13;
	unsigned int        dram_data14;
	unsigned int        dram_data15;
	unsigned int 	    data_temp[48];		//for future use
}dram_data_t;

extern dram_para_t *dram_para;

#define DELAY_FOR_DRAM
extern unsigned int dram_power_save_process(void);
extern unsigned int dram_power_up_process(void);
#define dram_enter_suspend() dram_power_save_process()
#define dram_exit_suspend() dram_power_up_process()
extern unsigned int mdfs_dfs(__dram_para_t *para, unsigned int freq);
extern unsigned int ccm_set_pll_ddr1_clk_new_mode(void);
//extern int dram_config_paras(u32 index, u32 len, u32 *data);
#endif // w5, w6, w9

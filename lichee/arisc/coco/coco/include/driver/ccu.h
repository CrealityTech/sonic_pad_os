/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : ccu.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-04-13 14:54
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __CCU_H__
#define __CCU_H__

#include "ibase.h"
#include "driver.h"

typedef struct {
	arisc_device dev;
	u32 freq;
	const struct ccu_ops *ops;
}ccu_device;

#define CCU_HOSC_WILL_OFF_NOTIFY	0x1
#define CCU_HOSC_ON_READY_NOTIFY	0x2

#define CCU_APBS2_CLK_CLKCHG_REQ	0x11
#define CCU_ABPS2_CLK_CLKCHG_DONE	0x12

#define GET_PRCMDEVICE		0x1
#define CPUS_SET_FREQ		0x2


#define GET_CCMUDEVICE		0x1


s32 prcm_set_div(ccu_device *prcm, u32 reg, u32 div);
s32 prcm_set_src(ccu_device *prcm, u32 reg, u32 clk);
s32 prcm_get_freq(ccu_device *prcm, u32 reg);
s32 prcm_set_freq(ccu_device *prcm, u32 reg, u32 freq);
s32 prcm_reset_reg(ccu_device *prcm, u32 reg);
s32 prcm_reset_clk(ccu_device *prcm, u32 reg);
s32 prcm_reset_clk_mask(ccu_device *prcm, u32 reg);

s32 prcm_resume(arisc_device *prcm);
s32 prcm_suspend(arisc_device *prcm);
s32 prcm_suspend_early(ccu_device *dev);
s32 prcm_resume_early(ccu_device *dev);

s32 ccmu_init(void);
void ccmu_exit(void);
s32 prcm_init(void);
void prcm_exit(void);

ccu_device *get_prcm_device(void);
ccu_device *get_ccmu_device(void);

s32 ccmu_get_freq(ccu_device *ccmu, u32 reg);
s32 ccmu_set_freq(ccu_device *ccmu, u32 reg, u32 freq);
s32 ccmu_set_m_div(u32 reg, u32 mdiv);
s32 ccmu_get_m_div(u32 reg);
s32 ccmu_reset_module(ccu_device *ccmu, u32 reg);
s32 ccmu_disbale_clk(ccu_device *ccmu, u32 reg);
s32 ccmu_reset_clk(ccu_device *ccmu, u32 reg);
s32 ccmu_reset_clk_mask(ccu_device *ccmu, u32 reg);
s32 ccmu_suspend(arisc_device *ccmu);
s32 ccmu_resume(arisc_device *ccmu);
s32 ccmu_suspend_early(arisc_device *ccmu);
s32 ccmu_resume_early(arisc_device *ccmu);


s32 ccu_register(arisc_device *dev,void *parg);
s32 prcm_reg_mclk_cb(u32 mclk, __pNotifier_t pcb);
s32 prcm_unreg_mclk_cb(u32 mclk, __pNotifier_t pcb);


s32 ccu_24mhosc_reg_cb(__pNotifier_t pcb,void *parg);
s32 ccu_apbs2_reg_mclk_cb(__pNotifier_t pcb,void *parg);
s32 ccu_set_sclk_freq(u32 freq);
s32 ccu_get_sclk_freq(void);

#endif


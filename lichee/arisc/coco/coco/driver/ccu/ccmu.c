#include "ibase.h"
#include "driver.h"
#include "ccu_i.h"
#include "types.h"

static ccu_device _ccmu;


ccu_device *get_ccmu_device(void)
{
	return &_ccmu;
}

s32 ccmu_get_freq(ccu_device *ccmu, u32 reg)
{
	u32 value;
	u32 freq_sel;
	u32 freq;
	u32 n,m,p;

	CCU_Get_Clkdiv(ccmu->dev.reg_base + PLL_PERI0_CTRL_REG, &n,&m,&p);

	value = 24000000 * (n + 1) / ((m & 0x1) + 1) /(((m >> 1) & 0x1) + 1) / PERIO_1X_PARA;

	switch (GET_BUS_TYPE(reg)) {
		case BUS_TYPE_PLL:
			break;
		case BUS_TYPE_AHB_PSI_APB:
			CCU_Get_Clkdiv(ccmu->dev.reg_base + reg, &n,&m,&p);
			freq_sel = CCU_Get_Clkfreq(CCU_Get_Clksrc(ccmu->dev.reg_base + reg));
			if(freq_sel == CLOKC_SRC_SET_HOSC)
				freq = 24000000;
			else if(freq_sel == CLOKC_SRC_SET_LOSC)
				freq = 32000;
			else if(freq_sel == CLOKC_SRC_SET_IOSC || freq_sel == CLOKC_SRC_SET_PSI) {
				if(reg == PSI_AHB1_AHB2_CFG_REG)
					freq = 16000000;
				else
					freq = ccmu_get_freq(ccmu,PSI_AHB1_AHB2_CFG_REG);
			}
			else if(freq_sel == CLOKC_SRC_SET_PLL)
				freq = value;

			value =  freq / (n + 1) / (m + 1);
			break;
		default:
			break;
	}

	return value;
}

s32 ccmu_set_freq(ccu_device *ccmu, u32 reg, u32 freq)
{
	CCU_Set_Clksrc(ccmu->dev.reg_base + reg, freq);

	return OK;
}

s32 ccmu_set_m_div(u32 reg, u32 mdiv)
{
	CCU_Set_Clkdiv(_ccmu.dev.reg_base + reg, 0xffffffff, mdiv, 0xffffffff);

	return OK;
}

s32 ccmu_get_m_div(u32 reg)
{
	u32 mdiv = 0, ndiv = 0, pdiv = 0;
	CCU_Get_Clkdiv(_ccmu.dev.reg_base + reg, &ndiv, &mdiv, &pdiv);

	return mdiv;
}

s32 ccmu_reset_module(ccu_device *ccmu, u32 reg)
{
	ccmu = ccmu;
	reg = reg;

	return OK;
}

s32 ccmu_reset_clk(ccu_device *ccmu, u32 reg)
{
	CCU_Set_Bus_Gating_Reset(ccmu->dev.reg_base + reg, 1);

	return OK;
}
/*
s32 ccmu_set_div(ccu_device *ccmu, u32 reg,u32 div)
{
	ccmu = ccmu;
	reg = reg;

	CCU_Set_Bus_Gating_Reset(ccmu->dev.reg_base + reg, 1);

	return OK;
}
*/


s32 ccmu_disbale_clk(ccu_device *ccmu, u32 reg)
{
	ccmu = ccmu;
	reg = reg;

	CCU_Disbale_Clk(ccmu->dev.reg_base + reg);

	return OK;
}


s32 ccmu_reset_clk_mask(ccu_device *ccmu, u32 reg)
{
	CCU_Set_Bus_Gating_Reset(ccmu->dev.reg_base + reg, 0);

	return OK;
}


s32 ccmu_suspend(arisc_device *ccmu)
{
	ccmu = ccmu;

	return OK;
}

s32 ccmu_resume(arisc_device *ccmu)
{
	ccmu = ccmu;

	return OK;
}

s32 ccmu_suspend_early(arisc_device *ccmu)
{
	ccmu = ccmu;

	return OK;
}

s32 ccmu_resume_early(arisc_device *ccmu)
{
	ccmu = ccmu;

	return OK;
}


arisc_driver ccmu_dri = {
	.suspend = ccmu_suspend,
	.resume  = ccmu_resume,
};

struct ccu_ops arisc_ccmu_ops = {
//	.set_div = ccmu_set_div,
//	.set_src = ccmu_set_src,
	.get_freq = ccmu_get_freq,
	.set_freq = ccmu_set_freq,
	.reset_reg = ccmu_reset_module,
	.reset_clk = ccmu_reset_clk,
	.suspend_early = ccmu_suspend_early,
	.resume_early = ccmu_resume_early,
};

void ccmu_exit(void)
{
	;
}

s32 ccmu_init(void)
{

	_ccmu.dev.hw_ver = 0x20001;
	_ccmu.dev.reg_base = CCMU_REG_BASE;
	_ccmu.dev.dev_lock = 1;
	_ccmu.dev.dev_id = CCMU_DEVICE_ID;
	_ccmu.ops = &arisc_ccmu_ops;
	_ccmu.dev.dri = &ccmu_dri;

	if (dev_match(&(_ccmu.dev),CCMU_REG_BASE + CCMU_VERSION_REG)) {
		list_add_tail(&_ccmu.dev.list,&(dev_list));
		return OK;
	}
	else
		ccmu_exit();

	return FAIL;
}


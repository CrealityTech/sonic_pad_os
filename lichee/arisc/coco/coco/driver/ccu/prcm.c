#include "./ccu_i.h"

static ccu_device s_prcm;

ccu_device *get_prcm_device(void)
{
	return &s_prcm;
}

s32 prcm_set_div(ccu_device *prcm, u32 reg, u32 div)
{
	u32 n,m;

	calc_clk_ratio_nm(div, &n, &m);

	CCU_Set_Clkdiv(prcm->dev.reg_base + reg, n, m - 1, 0);

	return OK;
}

s32 prcm_set_src(ccu_device *prcm,u32 reg, u32 clk)
{
	CCU_Set_Clksrc(prcm->dev.reg_base + reg,clk);

	return OK;
}

s32 prcm_get_freq(ccu_device *prcm,u32 reg)
{
	u32 value;
	u32 n,m,p;
	ccu_device *ccmu;

	ccmu = get_ccmu_device();

	if(ccmu->ops->get_freq != NULL)
		value = ccmu->ops->get_freq(ccmu, PLL_PERI0_CTRL_REG);
	else
		return FAIL;

	CCU_Get_Clkdiv(prcm->dev.reg_base + reg,&n,&m,&p);

	return value / ((m + 1) * (n + 1));

}

s32 prcm_set_freq(ccu_device *prcm, u32 reg, u32 freq)
{
	u32 value;

	ccu_device *ccmu;

	ccmu = get_ccmu_device();
	if(ccmu->ops->get_freq != NULL)
		value = ccmu->ops->get_freq(ccmu, PLL_PERI0_CTRL_REG);
	else
		return FAIL;

	value = value / freq;

	if(value == 0)
		value = 1;

	prcm->ops->set_div(prcm,reg,value);
	prcm->ops->set_src(prcm,reg,PRCM_SRC_PLL);

	return OK;
}


s32 prcm_reset_clk(ccu_device *prcm, u32 reg)
{
	CCU_Set_Bus_Gating_Reset(prcm->dev.reg_base + reg,1);

	return 0;
}

s32 prcm_reset_clk_mask(ccu_device *prcm, u32 reg)
{
	CCU_Set_Bus_Gating_Reset(prcm->dev.reg_base + reg,0);

	return 0;
}

s32 prcm_suspend(arisc_device *prcm)
{
	prcm = prcm;

	return 0;
}

s32 prcm_resume(arisc_device *prcm)
{
	prcm = prcm;

	return 0;
}

s32 prcm_suspend_early(ccu_device *prcm)
{
	prcm = prcm;

	return 0;
}

s32 prcm_resume_early(ccu_device *prcm)
{
	prcm = prcm;

	return 0;
}

static int prcm_ioctl(u32 request,va_list args)
{
	u32 value;
	ccu_device *prcm;

	switch (request) {
		case GET_PRCMDEVICE:
			value = (u32)va_arg(args, unsigned int);
			value = value;
			return (u32)get_prcm_device();
		case CPUS_SET_FREQ:
			value = (u32)va_arg(args, unsigned int);
			prcm = get_prcm_device();
			return (u32)prcm_set_freq(prcm,CPUS_CONFIG_REG,value);
		default:
			break;
	}
	return OK;
}

arisc_driver prcm_dri = {
	.suspend = prcm_suspend,
	.resume  = prcm_resume,
	.iorequset = prcm_ioctl,
};

const struct ccu_ops arisc_prcm_ops = {
	.set_div = prcm_set_div,
	.set_src = prcm_set_src,
	.get_freq = prcm_get_freq,
	.set_freq = prcm_set_freq,
//	.reset_reg = prcm_reset_reg,
	.reset_clk = prcm_reset_clk,
};

void prcm_exit(void)
{
	;
}

struct notifier *hosc_notifier_list = NULL;
struct notifier *apbs2_notifier_head = NULL;

s32 ccu_24mhosc_reg_cb(__pNotifier_t pcb,void *parg)
{
	//insert call-back to hosc_notifier_list.
	return notifier_insert(&hosc_notifier_list, pcb, parg);
}

s32 ccu_apbs2_reg_mclk_cb(__pNotifier_t pcb,void *parg)
{
	return notifier_insert(&apbs2_notifier_head, pcb, parg);
}

s32 prcm_init(void)
{
	s_prcm.dev.hw_ver = 0x10002;
	s_prcm.dev.reg_base = R_PRCM_REG_BASE;
	s_prcm.dev.dev_lock = 1;
	s_prcm.dev.dev_id = PRCM_DEVICE_ID;
	s_prcm.ops = &arisc_prcm_ops;
	s_prcm.dev.dri = &prcm_dri;

//	if (dev_match(&s_prcm.dev,R_PRCM_REG_BASE + PRCM_VERSION_REG)) {
		s_prcm.ops->set_freq(&s_prcm,CPUS_CONFIG_REG,200000000);
		s_prcm.freq = 200000000;
		list_add_tail(&s_prcm.dev.list,&(dev_list));

		iosc_freq_init();

		return OK;
//	}
//	else
//		prcm_exit();

//	return FAIL;
}


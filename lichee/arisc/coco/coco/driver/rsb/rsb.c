
#include "rsb_i.h"

rsb_device pmu_rsb;

rsb_device *query_rsb(void)
{
	return &pmu_rsb;
}

s32 rsb_exit(rsb_device *rsb)
{
	rsb->dev.dev_lock = 1;

	return OK;
}

s32 rsb_set_run_time_addr(rsb_device *dev, u32 saddr ,u32 rtsaddr)
{
	return RSB_Set_Runtime_Addr(&dev->RSBx, saddr, rtsaddr);
}

void rsb_hw_init(rsb_device *dev, RSB_InitConfig *config)
{
	RSB_InitConfig rsb_config;

	RSB_Reg_Init(&dev->RSBx, dev->dev.reg_base);

	if(config == NULL) {
		RSB_DefInit(&rsb_config);
		RSB_Config(&dev->RSBx,&rsb_config);
	} else
		RSB_Config(&dev->RSBx,config);

}

s32 rsb_read_block(rsb_device *dev, rsb_paras_t *paras)
{
	u32 i = 0;
	s32 result = OK;

	u32 len;
	u32 datatype;
	u32 devaddr;
	u32 *data;
	u8 *regaddr;

	len	     = paras->len;
	datatype = paras->datatype;
	devaddr  = paras->devaddr;
	regaddr  = paras->regaddr;
	data     = paras->data;

	if (len > RSB_TRANS_BYTE_MAX) {
		WRN("rsb read reg paras error\n");
		return -EINVAL;
	}

	//read data one by one
	for (i = 0; i < len; i++)
	{
		switch (datatype)
		{
			case RSB_DATA_TYPE_BYTE:
				result |= RSB_Read_Byte(&dev->RSBx, devaddr,
							*regaddr++, data++);
				break;
			case RSB_DATA_TYPE_HWORD:
				result = 0;
				break;
			case RSB_DATA_TYPE_WORD:
				result = 0;
				break;
			default:
				break;
		}
	}

	return result;
}

s32 rsb_write_block(rsb_device *dev, rsb_paras_t *paras)
{
	u32 i = 0;
	s32 result = OK;

	u32 len;
	u32 datatype;
	u32 devaddr;
	u32 *data;
	u8 *regaddr;

	len	     = paras->len;
	datatype = paras->datatype;
	devaddr  = paras->devaddr;
	regaddr  = paras->regaddr;
	data     = paras->data;

	if (len > RSB_TRANS_BYTE_MAX) {
		WRN("rsb read reg paras error\n");
		return -EINVAL;
	}

	//write data one by one
	for (i = 0; i < len; i++)
	{
		switch (datatype)
		{
			case RSB_DATA_TYPE_BYTE:
				result |= RSB_Write_Byte(&dev->RSBx, devaddr,
							*regaddr++, data++);
				break;
			case RSB_DATA_TYPE_HWORD:
				result = 0;
				break;
			case RSB_DATA_TYPE_WORD:
				result = 0;
				break;
			default:
				break;
		}
	}

	return result;
}

s32 rsb_set_rate(rsb_device *dev, u32 rate)
{
	dev->rsb_rate = rate;

	return RSB_Set_Clk(&dev->RSBx, rate);
}

s32 rsb_set_lock(rsb_device *dev,u32 lock)
{
	//while (!IS_TX_FIFO_EMPTY(uart))
	//	continue;

	dev->dev.dev_lock = lock;

	return OK;
}

static s32 rsb_clkchangecb(u32 command, void *parg __attribute__ ((__unused__)))
{
	switch(command) {
		case CCU_APBS2_CLK_CLKCHG_REQ:
		{
			//check rsb is busy now
			//...

			//clock will be change, lock rsb interface
			LOG("rsb source clock change request\n");
			rsb_set_lock(&pmu_rsb, TRUE);
			return OK;
		}
		case CCU_ABPS2_CLK_CLKCHG_DONE:
		{
			//clock change finish, re-config rsb clock,
			//maybe re-config rsb clock should do somethings???
			rsb_set_rate(&pmu_rsb, RSB_SCK_FREQ);

			//unlock rsb interface
			rsb_set_lock(&pmu_rsb, FALSE);
			LOG("rsb rate change done\n");
			return OK;
		}
		default:
			break;
	}

	return OK;
}

static s32 rsb_suspend(arisc_device * dev __attribute__ ((__unused__)))
{
	return OK;
}

static s32 rsb_resume(arisc_device * dev __attribute__ ((__unused__)))
{
	return OK;
}

arisc_driver rsb_dri = {
	.suspend = rsb_suspend,
	.resume  = rsb_resume,
};

const struct rsb_ops arisc_rsb_ops = {
	.read_block = rsb_read_block,
	.write_block = rsb_write_block,
	.set_rtsa = rsb_set_run_time_addr,
};

void rsb_pin_init(rsb_device *dev)
{
	pin_config rsb_pin_config = {0};

	rsb_pin_config.pin_grp = GPIO_GP_PL;
	rsb_pin_config.pin_pull = GPIO_PULL_UP;
	rsb_pin_config.pin_fn = GPIO_FN_2;
	rsb_pin_config.pin_driving = GPIO_MULTI_DRIVE_1;

	rsb_pin_config.pin_num = GPIO_PIN_0;
	pin_set(&rsb_pin_config,&(dev->rsb_sck));

	rsb_pin_config.pin_num = GPIO_PIN_1;
	pin_set(&rsb_pin_config,&(dev->rsb_sda));

}

void rsb_init(void)
{
	RSB_InitConfig rsb_config;

	pmu_rsb.dev.hw_ver = 0x10;
	pmu_rsb.dev.reg_base = RSB_REG_BASE;
	pmu_rsb.dev.dev_lock = 1;
	pmu_rsb.dev.dev_id = RSB_DEVICE_ID;
	pmu_rsb.dev.dri = &rsb_dri;
	pmu_rsb.dev.irq_no = INTC_R_RSB_IRQ;
	pmu_rsb.ops = &arisc_rsb_ops;
	pmu_rsb.clkchangecb = rsb_clkchangecb;

	prcm_reset_clk_mask(get_prcm_device(),RSB_BUS_GATING_REG);
	time_mdelay(1);
	prcm_reset_clk(get_prcm_device(),RSB_BUS_GATING_REG);

	RSB_Soft_Reset(&pmu_rsb.RSBx);

	rsb_pin_init(&pmu_rsb);

	RSB_Reg_Init(&pmu_rsb.RSBx,pmu_rsb.dev.reg_base);
	RSB_DefInit(&rsb_config);
	RSB_Config(&pmu_rsb.RSBx,&rsb_config);

	//pmu initialize as rsb mode
	RSB_Set_PMU_Mode(&pmu_rsb.RSBx,0x00,0x3e,0x7c);

	list_add_tail(&pmu_rsb.dev.list,&(dev_list));
	pmu_rsb.dev.dev_lock = 0;

	ccu_apbs2_reg_mclk_cb(rsb_clkchangecb, NULL);

}





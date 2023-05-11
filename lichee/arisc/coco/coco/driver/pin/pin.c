#include "pin_i.h"

pin_device pinl;
pin_device pinm;

s32 pin_set(pin_config *config,pin_typedef **owner)
{
	pin_typedef *pin;
	pl_port *pl = (pl_port *)pinl.dev.data;
	pm_port *pm = (pm_port *)pinm.dev.data;
	u32 pin_grp = config->pin_grp;
	u32 pin_num = config->pin_num;

	if (config->pin_grp == GPIO_GP_PL)
		pin = &(pl->pin[pin_num]);
	else
		pin = &(pm->pin[pin_num]);

	memcpy((void *)(&(pin->init_config)),(const void *)config,sizeof(pin_config));

	if(pin->pin_used == 1) {
		ERR("pin request failed: pin_grp = %d, pin_num = %d\n",
			pin_grp, pin_num);
		return -EPERM;
	}

	if(owner != NULL)
		*owner = pin;

	Pin_Set_Multi_Sel(pin_grp,pin_num,config->pin_fn);
	Pin_Set_Pull(pin_grp,pin_num,config->pin_pull);
	Pin_Set_Drive(pin_grp,pin_num,config->pin_driving);

	if(config->pin_exti_ctrl) {
		Exti_Enable_Int(pin_grp,pin_num);
		Exti_Set_Int_Trigger_Mode(pin_grp,pin_num,config->pin_exti_config);
		Exti_Set_Int_Debounce(pin_grp,config->pin_exti_pre,config->pin_exti_clk);
	}

	pin->pin_used = 1;

	return OK;
}

void pin_disable(pin_typedef *pin)
{
	pin_config *config;
	u32 pin_grp = pin->init_config.pin_grp;
	u32 pin_num = pin->init_config.pin_num;

	config = &(pin->init_config);

	config->pin_fn = GPIO_FN_7;
	Pin_Set_Multi_Sel(pin_grp,pin_num,config->pin_fn);

	config->pin_exti_ctrl = GPIO_EXTI_DISABLE;
	Exti_Disable_Int(pin_grp, pin_num);

	pin->pin_used = 0;
}

void pin_exti_irq_request(pin_typedef *pin, __pISR_t handler)
{
	pin->exti_handler = handler;
}

void pin_data_write(pin_typedef *pin,u32 data)
{
	u32 pin_grp = pin->init_config.pin_grp;
	u32 pin_num = pin->init_config.pin_num;

	Pin_Write_Data(pin_grp,pin_num,data);
	pin->pin_data = (u8)data;
}

u32 pin_data_read(pin_typedef *pin)
{
	u32 data;

	u32 pin_grp = pin->init_config.pin_grp;
	u32 pin_num = pin->init_config.pin_num;

	data = (u8)Pin_Read_Data(pin_grp,pin_num);
	pin->pin_data = (u8)data;

	return data;
}

/* NOTE:gpio_int_en is from bit0, if used for PM, should right shift 12
 */
u32 is_wanted_gpio_int(u32 pin_grp, u32 gpio_int_en)
{
	volatile u32  value;

	value = Pin_Grp_Int_Status(pin_grp);

	return (value & gpio_int_en);
}

u32 is_cpux_gpio_pending(void)
{
	return 0;

}

u32 pin_query_pending(pin_typedef *pin)
{
	u32 pin_grp = pin->init_config.pin_grp;
	u32 pin_num = pin->init_config.pin_num;

	return Exti_Query_Pending(pin_grp,pin_num);
}

u32 pin_clear_pending(pin_typedef *pin)
{
	u32 pin_grp = pin->init_config.pin_grp;
	u32 pin_num = pin->init_config.pin_num;

	return Exti_Clear_Pending(pin_grp,pin_num);
}

s32 pin_enable_int(pin_typedef *pin)
{
	u32 pin_grp = pin->init_config.pin_grp;
	u32 pin_num = pin->init_config.pin_num;

	return Exti_Enable_Int(pin_grp,pin_num);
}

s32 pin_disable_int(pin_typedef *pin)
{
	u32 pin_grp = pin->init_config.pin_grp;
	u32 pin_num = pin->init_config.pin_num;

	return Exti_Disable_Int(pin_grp,pin_num);
}

#if 0
pin_typedef *demo_pin;

s32 demo_exti_handler(void *p_arg)
{
	pin_typedef *pin = (pin_typedef *)p_arg;

	printk("exti_hanler\n");

	pin_clear_pending(pin);
	return OK;
}

void demo_pin_exti(void)
{
	pin_config demo_pin_config;

	demo_pin_config.pin_grp = GPIO_GP_PM;
	demo_pin_config.pin_pull = GPIO_PULL_UP;
	demo_pin_config.pin_fn  = GPIO_FN_6;
	demo_pin_config.pin_driving = GPIO_MULTI_DRIVE_1;
	demo_pin_config.pin_num  = GPIO_PIN_6;

	demo_pin_config.pin_exti_ctrl = GPIO_EXTI_ENABLE;
	demo_pin_config.pin_exti_config = GPIO_EXTI_NEGATIVE_EDGE;
	demo_pin_config.pin_exti_pre = 0;
	demo_pin_config.pin_exti_clk = GPIO_CLK_HOSC;

	if(!pin_configuration(&demo_pin_config,&demo_pin)) {
		pin_clear_pending(demo_pin);
		pin_exti_irq_request(demo_pin,demo_exti_handler);
//		interrupt_enable(INTC_R_PM_EINT_IRQ);
	}
	else
		printk("pin set error\n");

}
#endif

s32 pl_irq_handler(void *p_arg,u32 intno)
{
	u32 exti_pending;
	u32 i;

	pl_port *pldev = (pl_port *)(((pin_device *)p_arg)->dev.data);

	exti_pending = Exti_Query_Grp_Pending(PIN_GRP_PL);
	for(i = 0 ;i < PL_NUM; i++) {
		if((exti_pending >> i) & 0x1) {
			if (pldev->pin[i].exti_handler != NULL) {
				(pldev->pin[i].exti_handler)((void *)(&(pldev->pin[i])), intno);
			}
		}
	}

	return 0;
}

s32 pm_irq_handler(void *p_arg,u32 intno)
{
	u32 exti_pending;
	u32 i;

	pm_port *pmdev = (pm_port *)(((pin_device *)p_arg)->dev.data);

	exti_pending = Exti_Query_Grp_Pending(PIN_GRP_PM);

	for(i = 0 ;i < PM_NUM; i++) {
		if((exti_pending >> i) & 0x1) {
			if (pmdev->pin[i].exti_handler != NULL) {
				(pmdev->pin[i].exti_handler)((void *)(&(pmdev->pin[i])), intno);
			}
		}
	}

	return 0;
}

s32 pin_suspend(arisc_device * dev __attribute__ ((__unused__)))
{
	return OK;
}

s32 pin_resume(arisc_device * dev __attribute__ ((__unused__)))
{
	return OK;
}



arisc_driver pin_dri = {
	.suspend = pin_suspend,
	.resume  = pin_resume,
};

void pin_exit(void)
{
	;
}

s32 pinl_init(void)
{
	pinl.dev.hw_ver = 0x10000;
	pinl.dev.reg_base = R_PIO_REG_BASE;
	pinl.dev.dev_lock = 1;
	pinl.dev.dev_id = PINL_DEVICE_ID;
	pinl.dri = &pin_dri;
	pinl.dev.handler = pl_irq_handler;
	pinl.dev.data = (void *)(&gpio_l_dev);
	pinl.dev.irq_no = INTC_R_PL_EINT_IRQ;

/***********************************************
 *pin clk always on, so we dont need to open it.
 ***********************************************/

	if (dev_match(&pinl.dev,R_PIO_REG_BASE + PIN_VERSION_REG)) {
		Pin_Clk_Switch(PIN_GRP_PL,1);
		install_isr(pinl.dev.irq_no, pinl.dev.handler, (void *)(&pinl));
		pinl.dev.dev_lock = 0;
		list_add_tail(&(pinl.dev.list),&(dev_list));

		return OK;
	}
	else
		pin_exit();

	return FAIL;

}

s32 default_pin_wakeup_init(void *parg)
{
	pin_typedef *pinx;
	pl_port *pl = (pl_port *)pinl.dev.data;
	pm_port *pm = (pm_port *)pinm.dev.data;

	u32 *p = (u32 *)parg;

	u32 pin_num = p[0];
	u32 pin_grp = p[1];
	u32 pin_exti_pre = 0;
	u32 pin_exti_clk = GPIO_CLK_LOSC;


	if (pin_grp == GPIO_GP_PL)
		pinx = &(pl->pin[pin_num]);
	else
		pinx = &(pm->pin[pin_num]);

	Exti_Set_Int_Debounce(pin_grp, pin_exti_pre, pin_exti_clk);
	Exti_Clear_Pending(pin_grp, pin_num);

	pin_exti_irq_request(pinx, (__pISR_t)p[2]);

	return OK;
}

s32 default_pin_wakeup_exit(void *parg)
{
	pin_typedef *pinx;
	pl_port *pl = (pl_port *)pinl.dev.data;
	pm_port *pm = (pm_port *)pinm.dev.data;

	u32 *p = (u32 *)parg;

	u32 pin_num = p[0];
	u32 pin_grp = p[1];
	u32 pin_exti_pre = 0;
	u32 pin_exti_clk = GPIO_CLK_HOSC;

	if (pin_grp == GPIO_GP_PL)
		pinx = &(pl->pin[pin_num]);
	else
		pinx = &(pm->pin[pin_num]);

	Exti_Set_Int_Debounce(pin_grp, pin_exti_pre, pin_exti_clk);
	Exti_Clear_Pending(pin_grp, pin_num);

	pin_exti_irq_request(pinx, (__pISR_t)p[2]);

	return OK;
}

s32 pin_wakeup_init(void *parg)
{
	u32 i = 0;
	u32 *p = (u32 *)parg;
	u32 irq_no = p[0];
	pin_config wakeup_pin_config = {0};
	pin_typedef *pinx;

	i = irq_no;

	wakeup_pin_config.pin_grp = p[1];
	wakeup_pin_config.pin_fn  = GPIO_FN_6;
	wakeup_pin_config.pin_driving = GPIO_MULTI_DRIVE_1;
	wakeup_pin_config.pin_pull = GPIO_PULL_UP;

	wakeup_pin_config.pin_num = i;

	wakeup_pin_config.pin_exti_ctrl = GPIO_EXTI_ENABLE;
	wakeup_pin_config.pin_exti_config = GPIO_EXTI_NEGATIVE_EDGE;
	wakeup_pin_config.pin_exti_pre = 0;
	wakeup_pin_config.pin_exti_clk = GPIO_CLK_LOSC;

	pin_set(&wakeup_pin_config, &pinx);
	pin_clear_pending(pinx);
	pin_exti_irq_request(pinx, (__pISR_t)p[2]);

	return OK;
}

#ifdef CFG_PIN_M_USED

s32 pinm_init(void)
{
	pinm.dev.hw_ver = 0x10000;
	pinm.dev.reg_base = R_PIO_REG_BASE;
	pinm.dev.dev_lock = 1;
	pinm.dev.dev_id = PINM_DEVICE_ID;
	pinm.dev.dri = &pin_dri;
	pinm.dev.handler = pm_irq_handler;
	pinm.dev.data = (void *)(&gpio_m_dev);
	pinm.dev.irq_no = INTC_R_PM_EINT_IRQ;

/***********************************************
 *pin clk always on, so we dont need to open it.
 ***********************************************/

	if (dev_match(&pinm.dev,R_PIO_REG_BASE + PIN_VERSION_REG)) {
		Pin_Clk_Switch(PIN_GRP_PM,1);
		install_isr(pinm.dev.irq_no, pinm.dev.handler, (void *)(&pinm));
		pinm.dev.dev_lock = 0;
		list_add_tail(&(pinm.dev.list),&(dev_list));

		return OK;
	}
	else
		pin_exit();

	return FAIL;
}

#endif

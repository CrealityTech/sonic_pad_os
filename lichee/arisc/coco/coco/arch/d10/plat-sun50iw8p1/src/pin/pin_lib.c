/*
 *********************************************************************************************************
 *                                                AR100 SYSTEM
 *                                     AR100 Software System Develop Kits
 *
 *
 *                                    (c) Copyright 2018-2024, Luojie China
 *                                             All Rights Reserved
 *
 * File    : pin_lib.c
 * By      : LuoJie
 * Version : v1.0
 * Date    : 2018-04-13 15:38
 * Descript: .
 * Update  :auther      ver     notes
 *          Luojie      1.0     Create this file.
 *********************************************************************************************************
 */

#include "lib_inc.h"

/*
 *********************************************************************************************************
 *                                       PIN INIT
 *
 * Description: pin controller initialize.
 *
 * Arguments  : none
 *
 * Returns    : OK if initialize succeeded, others if failed.
 *********************************************************************************************************
 */
s32 Pin_Clk_Switch(u32 pin_grp,u32 source)
{
	//switch cpus port sample clock to HOSC from LOSC
	writel(source, PIN_REG_INT_DEBOUNCE(pin_grp));

	return OK;
}

/*
 *********************************************************************************************************
 *                                       PIN EXIT
 *
 * Description: pin controller exit.
 *
 * Arguments  : none
 *
 * Returns    : OK if exit succeeded, others if failed.
 *********************************************************************************************************
s32 Pin_Exit(void)
{
	return OK;
}

 */
/*
 *********************************************************************************************************
 *                                      SET PIN MULTI_SEL
 *
 * Description:  set the multi-select of a specific pin.
 *
 * Arguments  :  pin_grp     : the group number of the specific pin.
 *               pin_num     : the pin number of the specific pin.
 *               multi_sel   : the multi-select which we want to set.
 *
 * Returns    :  OK if set pin multi-select succeeded, others if failed.
 *********************************************************************************************************
 */
s32 Pin_Set_Multi_Sel(u32 pin_grp, u32 pin_num, u32 multi_sel)
{
	volatile u32 *addr;
	volatile u32  value;

	//set multi-select
	addr   = PIN_REG_CFG(pin_grp, pin_num);
	value  = *addr;
	value &= ~(0x7       << ((pin_num & 0x7) * 4));
	value |=  (multi_sel << ((pin_num & 0x7) * 4));
	*addr = value;

	return OK;
}

/*
 *********************************************************************************************************
 *                                      SET PIN PULL
 *
 * Description:  set the pull of a specific pin.
 *
 * Arguments  :  pin_grp     : the group number of the specific pin.
 *               pin_num     : the pin number of the specific pin.
 *               pull        : the pull which we want to set.
 *
 * Returns    :  OK if set pin pull succeeded, others if failed.
 *********************************************************************************************************
 */
s32 Pin_Set_Pull(u32 pin_grp, u32 pin_num, u32 pull)
{
	volatile u32 *addr;
	volatile u32  value;

	//set pull status
	addr   = PIN_REG_PULL(pin_grp, pin_num);
	value  = *addr;
	value &= ~(0x3  << ((pin_num & 0xf) * 2));
	value |=  (pull << ((pin_num & 0xf) * 2));

	*addr = value;

	return OK;
}

/*
 *********************************************************************************************************
 *                                      SET PIN DRIVE
 *
 * Description:  set the drive of a specific pin.
 *
 * Arguments  :  pin_grp     : the group number of the specific pin.
 *               pin_num     : the pin number of the specific pin.
 *               drive       : the drive which we want to set.
 *
 * Returns    :  OK if set pin drive succeeded, others if failed.
 *********************************************************************************************************
 */
s32 Pin_Set_Drive(u32 pin_grp, u32 pin_num, u32 drive)
{
	volatile u32 *addr;
	volatile u32  value;

	//set drive level
	addr   = PIN_REG_DLEVEL(pin_grp, pin_num);
	value  = *addr;
	value &= ~(0x3   << ((pin_num & 0xf) * 2));
	value |=  (drive << ((pin_num & 0xf) * 2));

	*addr = value;

	return OK;
}

u32 Pin_Grp_Int_Status(u32 pin_grp)
{
	return readl(PIN_REG_INT_STAT(pin_grp));
}
/*
 *********************************************************************************************************
 *                                      WRITE PIN DATA
 *
 * Description:  write the data of a specific pin.
 *
 * Arguments  :  pin_grp     : the group number of the specific pin.
 *               pin_num     : the pin number of the specific pin.
 *               data        : the data which we want to write.
 *
 * Returns    :  OK if write pin data succeeded, others if failed.
 *********************************************************************************************************
 */
s32 Pin_Write_Data(u32 pin_grp, u32 pin_num, u32 data)
{
	volatile u32 *addr;
	volatile u32  value;

	//write data
	addr   = PIN_REG_DATA(pin_grp);
	value  = *addr;
	value &= ~(0x1  << pin_num);
	value |=  ((data & 0x1) << pin_num);

	*addr = value;

	return OK;
}

/*
 *********************************************************************************************************
 *                                      READ PIN DATA
 *
 * Description:  read the data of a specific pin.
 *
 * Arguments  :  pin_grp     : the group number of the specific pin.
 *               pin_num     : the pin number of the specific pin.
 *
 * Returns    :  the data of the specific pin.
 *********************************************************************************************************
 */
u32 Pin_Read_Data(u32 pin_grp, u32 pin_num)
{
	volatile u32  value;

	//read data
	value = PIN_REG_DATA_VALUE(pin_grp);

	return ((value >> pin_num) & 0x1);
}



/*
 *********************************************************************************************************
 *                                      SET PIN INT TRIGGER MODE
 *
 * Description:  set the trigger mode of external interrupt pin.
 *
 * Arguments  :  pin_grp : the group number of the specific pin.
 *               pin_num : the pin number of the specific pin.
 *               mode    : the trigger mode which we want to set.
 *
 * Returns    :  OK if set trigger mode succeeded, others if failed.
 *********************************************************************************************************
 */
s32 Exti_Set_Int_Trigger_Mode(u32 eint_grp, u32 eint_num, u32 mode)
{
	volatile u32  *addr;
	volatile u32   value;

	//set eint pin trigger mode
	addr = PIN_REG_INT_CFG(eint_grp, eint_num);
	value = *addr;
	value &= ~(0x7  << ((eint_num & 0x7) * 4));
	value |=  (mode << ((eint_num & 0x7) * 4));
	*addr = value;

	return OK;
}

/*
 *********************************************************************************************************
 *                                      ENABLE PIN INT
 *
 * Description:  enable the interrupt of external interrupt pin.
 *
 * Arguments  :  pin_grp : the group number of the specific pin.
 *               pin_num : the pin number of the specific pin.
 *
 * Returns    :  OK if enable pin interrupt succeeded, others if failed.
 *********************************************************************************************************
 */
s32 Exti_Enable_Int(u32 eint_grp, u32 eint_num)
{
	volatile u32  *addr;
	volatile u32   value;

	//enable pin interrupt
	addr   = PIN_REG_INT_CTL(eint_grp);
	value  = *addr;
	value |= (0x1 << (eint_num));
	*addr  = value;
	writel(value,0x07022230);

	return OK;
}

/*
 *********************************************************************************************************
 *                                      DISABLE PIN INT
 *
 * Description:  disable the interrupt of external interrupt pin.
 *
 * Arguments  :  pin_grp : the group number of the specific pin.
 *               pin_num : the pin number of the specific pin.
 *
 * Returns    :  OK if disable pin interrupt succeeded, others if failed.
 *********************************************************************************************************
 */
s32 Exti_Disable_Int(u32 eint_grp, u32 eint_num)
{
	volatile u32  *addr;
	volatile u32   value;

	//disable pin interrupt
	addr   = PIN_REG_INT_CTL(eint_grp);
	value  = *addr;
	value &= ~(0x1 << (eint_num));
	*addr  = value;

	return OK;
}

/*
 *********************************************************************************************************
 *                                      query pending
 *
 * description:  query the pending of interrupt of external interrupt pin.
 *
 * arguments  :  pin_grp : the group number of the specific pin.
 *               pin_num : the pin number of the specific pin.
 *
 * returns    :  the pending status of external interrupt.
 *********************************************************************************************************
 */
u32 Exti_Query_Pending(u32 eint_grp, u32 eint_num)
{
	volatile u32  *addr;
	volatile u32   value;

	eint_num = eint_num;
	//query pending
	addr   = PIN_REG_INT_CTL(eint_grp);
	value  = *addr;

	return value;
}


u32 Exti_Query_Grp_Pending(u32 eint_grp)
{
	volatile u32 *addr;
	volatile u32 value;

	addr = PIN_REG_INT_STAT(eint_grp);
	value = *addr;

	return value;
}

/*
 *********************************************************************************************************
 *                                      clear pending
 *
 * deription:  clear the pending of interrupt of external interrupt pin.
 *
 * arguments  :  pin_grp : the group number of the specific pin.
 *               pin_num : the pin number of the specific pin.
 *
 * returns    :  ok if clear pending succeeded, others if failed.
 *********************************************************************************************************
 */
s32 Exti_Clear_Pending(u32 eint_grp, u32 eint_num)
{
	volatile u32  *addr;
	volatile u32   value;

	//clear pending
	addr   = PIN_REG_INT_STAT(eint_grp);
	value  = (0x1 << (eint_num));
	*addr  = value;

	return OK;
}

s32 Exti_Set_Int_Debounce(u32 eint_grp, u32 eint_pre, u32 eint_clk)
{
	volatile u32  *addr;
	volatile u32   value;

	//clear pending
	addr   = PIN_REG_INT_DEBOUNCE(eint_grp);
	value  = ((eint_pre << 4) | eint_clk);
	*addr  = value;

	return OK;
}

#if 0
static u32 pl_int_en, pm_int_en;


s32 Exti_Standby_Init(u32 gpio_enable_bitmap)
{
	u32 i;

	pl_int_en = readl(PIN_REG_INT_CTL(PIN_GRP_PL));
	pm_int_en = readl(PIN_REG_INT_CTL(PIN_GRP_PM));

	writel(0x0, PIN_REG_INT_CTL(PIN_GRP_PL));
	writel(0x0, PIN_REG_INT_CTL(PIN_GRP_PM));

	writel(0xffffffff, PIN_REG_INT_STAT(PIN_GRP_PL));
	writel(0xffffffff, PIN_REG_INT_STAT(PIN_GRP_PM));

	for (i = 0; i < pl_num; i++)
	{
		if (gpio_enable_bitmap & cpus_gpio_pl(i))
		{
			Exti_Enable_Int(PIN_GRP_PL, i);
		}
	}
	for (i = 0; i < pm_num; i++)
	{
		if (gpio_enable_bitmap & cpus_gpio_pm(i))
		{
			Exti_Enable_Int(PIN_GRP_PM, i);
		}
	}

	return OK;
}

s32 Exti_Standby_Exit(void)
{
	writel(pl_int_en, PIN_REG_INT_CTL(PIN_GRP_PL));
	writel(pm_int_en, PIN_REG_INT_CTL(PIN_GRP_PM));

	return OK;
}

#endif

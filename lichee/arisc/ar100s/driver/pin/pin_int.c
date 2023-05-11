/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                 pin module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : pin_int.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-8
* Descript: interrupt pin managment module.
* Update  : date                auther      ver     notes
*           2012-5-8 14:25:55   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "pin_i.h"


static s32 pin_pin2eint(u32 pin_grp, u32 pin_num, u32 *eint_grp,
			u32 *eint_num)
{
	switch (pin_grp) {
		/*PL pin group */
	case 1:
		ASSERT(pin_num >= 0 && pin_num <= 12);
		*eint_grp = pin_grp;
		*eint_num = pin_num;
		return OK;
		/*PM pin group */
	case 2:
		ASSERT(pin_num >= 0 && pin_num <= 8);
		*eint_grp = pin_grp;
		*eint_num = pin_num;
		return OK;
	}
	/*invalid pin group number */
	*eint_grp = 0xff;
	*eint_num = 0xff;
	return -ESRCH;
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
s32 pin_set_int_trigger_mode(u32 pin_grp, u32 pin_num, u32 mode)
{
	u32 eint_grp = 0;
	u32 eint_num = 0;
	volatile u32 *addr;
	volatile u32 value;

	/*convert pin number to eint number */
	pin_pin2eint(pin_grp, pin_num, &eint_grp, &eint_num);

	/*set eint pin trigger mode */
	addr = PIN_REG_INT_CFG(eint_grp, eint_num);
	value = *addr;
	value &= ~(0x7 << ((eint_num & 0x7) * 4));
	value |= (mode << ((eint_num & 0x7) * 4));
	*addr = value;

	return OK;
}

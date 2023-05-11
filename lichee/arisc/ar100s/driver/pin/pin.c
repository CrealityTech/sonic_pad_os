/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                 pin module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : pin.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-8
* Descript: pin managment module.
* Update  : date                auther      ver     notes
*           2012-5-8 14:17:29   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "pin_i.h"

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
s32 pin_init(void)
{
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
*/
s32 pin_exit(void)
{
	return OK;
}

/*
*********************************************************************************************************
*                                      SET PIN STATUS
*
* Description:  set all status of a specific pin.
*
* Arguments  :  pin_grp : the group number of the specific pin.
*               pin_num : the pin number of the specific pin.
*               pstatus : the pin status structure which we want to set.
*
* Returns    :  OK if set pin status succeeded, others if failed.
*********************************************************************************************************
*/
s32 pin_set_status(u32 pin_grp, u32 pin_num, struct pin_status *pstatus)
{
	return OK;
}

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
s32 pin_set_multi_sel(u32 pin_grp, u32 pin_num, u32 multi_sel)
{
	volatile u32 *addr;
	volatile u32 value;

	/*set multi-select */
	addr = PIN_REG_CFG(pin_grp, pin_num);
	value = *addr;
	value &= ~(0x7 << ((pin_num & 0x7) * 4));
	value |= (multi_sel << ((pin_num & 0x7) * 4));
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
s32 pin_set_pull(u32 pin_grp, u32 pin_num, u32 pull)
{
	volatile u32 *addr;
	volatile u32 value;

	/*set pull status */
	addr = PIN_REG_PULL(pin_grp, pin_num);
	value = *addr;
	value &= ~(0x3 << ((pin_num & 0xf) * 2));
	value |= (pull << ((pin_num & 0xf) * 2));

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
s32 pin_set_drive(u32 pin_grp, u32 pin_num, u32 drive)
{
	volatile u32 *addr;
	volatile u32 value;

	/*set drive level */
	addr = PIN_REG_DLEVEL(pin_grp, pin_num);
	value = *addr;
	value &= ~(0x3 << ((pin_num & 0xf) * 2));
	value |= (drive << ((pin_num & 0xf) * 2));

	*addr = value;

	return OK;
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
s32 pin_write_data(u32 pin_grp, u32 pin_num, u32 data)
{
	volatile u32 *addr;
	volatile u32 value;

	/*write data */
	addr = PIN_REG_DATA(pin_grp);
	value = *addr;
	value &= ~(0x1 << pin_num);
	value |= ((data & 0x1) << pin_num);

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
u32 pin_read_data(u32 pin_grp, u32 pin_num)
{
	volatile u32 value;

	/*read data */
	value = PIN_REG_DATA_VALUE(pin_grp);

	return ((value >> pin_num) & 0x1);
}

/*
 * NOTE:gpio_int_en is from bit0, if used for PM, should right shift 12
 */
u32 is_wanted_gpio_int(u32 pin_grp, u32 gpio_int_en)
{
	volatile u32 value;

	value = readl(PIN_REG_INT_STAT(pin_grp));

	return (value & gpio_int_en);
}

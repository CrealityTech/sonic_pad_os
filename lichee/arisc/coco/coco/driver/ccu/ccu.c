#include "ccu_i.h"

s32 ccu_register(arisc_device *dev,void *parg)
{
	dev = dev;
	parg = parg;

	return OK;
}



/*
*********************************************************************************************************
*                                    SET SOURCE OF MODULE CLOCK
*
* Description:  set the source of a specific module clock.
*
* Arguments  :  mclk    : the module clock ID which we want to set source.
*               sclk    : the source clock ID whick we want to set as source.
*
* Returns    :  OK if set source succeeded, others if failed.
*********************************************************************************************************
*/
s32 calc_clk_ratio_nm(u32 div, u32 *n, u32 *m)
{
	if(div < 17)
	{
		*n = 0;
		*m = div;
	}
	else if(div < 33)
	{
		*n = 1;
		*m = (div + 1) / 2;
	}
	else if(div < 65)
	{
		*n = 2;
		*m = (div + 3) / 4;
	}
	else if(div < 129)
	{
		*n = 3;
		*m = (div + 7) / 8;
	}
	else
	{
		ERR("icd %x\n", div);
		return -EINVAL;
	}

	return OK;
}


/*
*********************************************************************************************************
*                                     REGISTER MODULE CB
*
* Description:  register call-back for module clock, when the source frequency
*               of the module clock changed, it will use this call-back to notify
*               module driver.
*
* Arguments  :  mclk    : the module clock ID which we want to register call-back.
*               pcb     : the call-back which we want to register.
*
* Returns    :  OK if register call-back succeeded, others if failed.
*********************************************************************************************************
*/
s32 prcm_reg_mclk_cb(u32 mclk, __pNotifier_t pcb)
{
	mclk = mclk;
	pcb = pcb;
	//we just support apb module clock notifier now.
	//by sunny at 2012-11-23 14:36:14.

	//insert call-back to apb_notifier_head.
//	return notifier_insert(&apbs2_notifier_head, pcb);
	return OK;
}

/*
*********************************************************************************************************
*                                     UNREGISTER MODULE CB
*
* Description:  unregister call-back for module clock.
*
* Arguments  :  mclk    : the module clock ID which we want to unregister call-back.
*               pcb     : the call-back which we want to unregister.
*
* Returns    :  OK if unregister call-back succeeded, others if failed.
*********************************************************************************************************
*/
s32 prcm_unreg_mclk_cb(u32 mclk, __pNotifier_t pcb)
{
	mclk = mclk;
	pcb = pcb;
	//we just support apb module clock notifier now.
	//by sunny at 2012-11-23 14:36:14.

	//delete call-back to apb_notifier_head.
//	return notifier_delete(&apbs2_notifier_head, pcb);
	return OK;
}

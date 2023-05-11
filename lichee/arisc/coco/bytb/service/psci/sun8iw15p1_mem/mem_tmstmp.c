#include "includes.h"
#include "mem.h"

static mem_tmstmpctrl_reg_t  *PTmstmpCtrlReg;
static mem_tmstmpst_reg_t  *PTmstmpStReg;

s32 mem_tmstmp_save(void)
{
	PTmstmpCtrlReg = (mem_tmstmpctrl_reg_t *)(SUNXI_TIMESTAMP_CTRL_PBASE);
	PTmstmpStReg = (mem_tmstmpst_reg_t *)(SUNXI_TIMESTAMP_STA_PBASE);

	system_back->tmstmp_back.low = PTmstmpStReg->low;
	system_back->tmstmp_back.high = PTmstmpStReg->high;
	system_back->tmstmp_back.freq = PTmstmpCtrlReg->freq;

	//mem_reg_debug(__func__, IO_ADDRESS(SUNXI_TIMESTAMP_CTRL_PBASE), TMSTMP_REG_LENGTH);

	return OK;
}

s32 mem_tmstmp_restore(void)
{
	PTmstmpCtrlReg->ctrl = 0;                             //disable timestamp firstly
	PTmstmpCtrlReg->freq = system_back->tmstmp_back.freq; //restore freq
	PTmstmpCtrlReg->low = system_back->tmstmp_back.low;   //restore low
	PTmstmpCtrlReg->high = system_back->tmstmp_back.high; //restore high
	PTmstmpCtrlReg->ctrl = 1;                             //enable timestamp firstly
	//mem_reg_debug(__func__, IO_ADDRESS(SUNXI_TIMESTAMP_CTRL_PBASE), TMSTMP_REG_LENGTH);

	return OK;
}


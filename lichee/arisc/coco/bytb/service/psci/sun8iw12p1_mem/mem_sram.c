#include "includes.h"
#include "mem.h"

int mem_sram_save(void)
{
	mem_reg_save(system_back->sram_back.sram_reg_back, \
	             (const unsigned int *)(SUNXI_SRAMCTRL_PBASE), SRAM_REG_LENGTH, 1);

	//mem_reg_debug(__func__, IO_ADDRESS(SUNXI_SRAMCTRL_PBASE), SRAM_REG_LENGTH);

	return OK;
}

int mem_sram_restore(void)
{
	mem_reg_restore((unsigned int *)(SUNXI_SRAMCTRL_PBASE), \
	                (const unsigned int *)system_back->sram_back.sram_reg_back, SRAM_REG_LENGTH, 1);

	//mem_reg_debug(__func__, IO_ADDRESS(SUNXI_SRAMCTRL_PBASE), SRAM_REG_LENGTH);

	return OK;
}


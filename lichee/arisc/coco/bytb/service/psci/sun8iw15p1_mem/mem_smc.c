#include "includes.h"
#include "mem.h"

int mem_smc_save(void)
{
	mem_reg_save(system_back->smc_back.smc_reg0_back, \
	             (const unsigned int *)(SUNXI_SMC_PBASE + SMC_REG0_START), \
	             SMC_REG0_LENGTH, 1);
	mem_reg_save(system_back->smc_back.smc_reg1_back, \
	             (const unsigned int *)(SUNXI_SMC_PBASE + SMC_REG1_START), \
	             SMC_REG1_LENGTH, 1);
	//mem_reg_debug(__func__, system_back->smc_back.smc_reg0_back, sizeof(system_back->smc_back.smc_reg0_back)/4);
	//mem_reg_debug(__func__, system_back->smc_back.smc_reg1_back, sizeof(system_back->smc_back.smc_reg1_back)/4);
	//mem_reg_debug(__func__, IO_ADDRESS(SUNXI_SMC_PBASE), SMC_REG0_LENGTH);
	//mem_reg_debug(__func__, IO_ADDRESS(SUNXI_SMC_PBASE + SMC_REG1_START), SMC_REG1_LENGTH);

	return OK;
}

int mem_smc_restore(void)
{
	mem_reg_restore((unsigned int *)(SUNXI_SMC_PBASE + SMC_REG0_START), \
	                (const unsigned int *)system_back->smc_back.smc_reg0_back, SMC_REG0_LENGTH, 1);
	mem_reg_restore((unsigned int *)(SUNXI_SMC_PBASE + SMC_REG1_START), \
	                (const unsigned int *)system_back->smc_back.smc_reg1_back, SMC_REG1_LENGTH, 1);

	//mem_reg_debug(__func__, IO_ADDRESS(SUNXI_SMC_PBASE), SMC_REG0_LENGTH);
	//mem_reg_debug(__func__, IO_ADDRESS(SUNXI_SMC_PBASE + SMC_REG1_START), SMC_REG1_LENGTH);

	return OK;
}


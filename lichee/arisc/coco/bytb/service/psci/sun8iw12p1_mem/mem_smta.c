#include "includes.h"
#include "mem.h"


int mem_smta_save(void)
{
	mem_spc_save(system_back->smta_back.smta_reg_back,\
			(unsigned int *)(SUNXI_SMTA_PBASE + SMTA_REG_START), SMTA_REG_LENGTH);

	return OK;
}

int mem_smta_restore(void)
{
	/*
	 * default is secure, so we only need restore no-secure
	 */
	mem_spc_restore((unsigned int *)(SUNXI_SMTA_PBASE + SMTA_REG_START), \
			system_back->smta_back.smta_reg_back, SMTA_REG_LENGTH);

	return OK;
}


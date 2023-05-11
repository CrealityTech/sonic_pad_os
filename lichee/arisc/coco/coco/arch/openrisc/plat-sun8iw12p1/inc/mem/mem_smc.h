#ifndef __MEM_SMC_H__
#define __MEM_SMC_H__

#define SMC_REG0_START       (0x0)
#define SMC_REG0_END         (0xb0 + 0x04)
#define SMC_REG0_LENGTH      ((SMC_REG0_END - SMC_REG0_START)>>2)
#define SMC_REG1_START       (0x100)
#define SMC_REG1_END         (0x1F8 + 0x04)
#define SMC_REG1_LENGTH      ((SMC_REG1_END - SMC_REG1_START)>>2)

struct smc_regs{
	unsigned int smc_reg0_back[SMC_REG0_LENGTH];
	unsigned int smc_reg1_back[SMC_REG1_LENGTH];
};


int mem_smc_save(void);
int mem_smc_restore(void);
#endif

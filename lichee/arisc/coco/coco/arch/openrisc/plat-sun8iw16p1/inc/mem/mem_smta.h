#ifndef __MEM_SMTA_H__
#define __MEM_SMTA_H__

#define SMTA_REG_START       (0x0)
#define SMTA_REG_END         (0xd8 + 0x04)
#define SMTA_SKIP            3
#define SMTA_REG_LENGTH      (((0xd8 + 0x04)>>2)/SMTA_SKIP)

struct smta_regs{
	unsigned int smta_reg_back[SMTA_REG_LENGTH];
};


int mem_smta_save(void);
int mem_smta_restore(void);

#endif /* __MEM_SMTA_H__ */

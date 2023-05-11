#ifndef __MEM_SRAM_H__
#define __MEM_SRAM_H__


#define SRAM_REG_LENGTH		((0xec+0x4)>>2)

struct sram_state{
	unsigned int sram_reg_back[SRAM_REG_LENGTH];
};

int mem_sram_save(void);
int mem_sram_restore(void);
#endif


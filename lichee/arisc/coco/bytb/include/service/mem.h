#ifndef __MEM_H__
#define __MEM_H__

#include "mem/mem_ccu.h"
#include "mem/mem_gpio.h"
#include "mem/mem_pmu.h"
#include "mem/mem_smc.h"
#include "mem/mem_smta.h"
#include "mem/mem_sram.h"
#include "mem/mem_tmstmp.h"

static inline void mem_hexdump(const char* name, char* base, u32 len)
{
	u32 i;
	printf("%s :\n", name);
	for (i = 0; i < len; i += 4) {
		if (!(i & 0xf))
			printf("\n0x%8x : ", base + i);
		printf("%8x ", readl(base + i));
	}
	printf("\n");
}

static inline void mem_reg_save(unsigned int *dest, const unsigned int *src, \
		unsigned int n, unsigned int skip)
{
	//ASSERT(dest != NULL);
	//ASSERT(src != NULL);

	while (n--) {
		*(dest + n) = *(src + n * skip);
		//MEM_LOG("ad:%x as:%x dd:%x ds:%x\n", dest + n, src + n * skip, *(dest + n), *(src + n * skip));
	}
}

static inline void mem_reg_restore(unsigned int *dest, const unsigned int *src, \
		unsigned int n, unsigned int skip)
{
	//ASSERT(dest != NULL);
	//ASSERT(src != NULL);

	while (n--) {
		*(dest + n * skip) = *(src + n);
		//MEM_LOG("ad:%x as:%x dd:%x ds:%x\n", dest + n * skip, src + n, *(dest + n * skip), *(src + n));
	}
}


static inline void mem_spc_save(unsigned int *dest, unsigned int *src, unsigned int length)
{
	unsigned int i = 0;
	unsigned int *pdest = dest;
	unsigned int *psrc = src;

	for(i = 0; i< length ;i++)
	{
		*(pdest++) = *psrc;
		psrc += 4;
	}
}

static inline void mem_spc_restore(unsigned int *dest, unsigned int *src, unsigned int length)
{
	unsigned int i = 0;
	unsigned int *psrc = src;
	unsigned int *pdest = dest;

	for(i = 0; i< length ;i++)
	{
		*(pdest + 1) = *psrc++;
		pdest += 4;
	}

}



typedef struct system_state {
	struct gpio_state gpio_back;
	u32 reserve_back1[2];
	struct smc_regs smc_back;
	u32 reserve_back2[2];
	struct smta_regs smta_back;
	u32 reserve_back3[2];
	struct sram_state sram_back;
	u32 reserve_back5[2];
	struct mem_tmstmpctrl_reg tmstmp_back;
	struct ccm_state ccm_back;
	u32 reserve_back6[2];
}system_state_t;


extern struct system_state *system_back;
#endif

#ifndef __ARISC_PARA_H__
#define __ARISC_PARA_H__

#include "../driver/dram.h"
#include "../driver/ir.h"
#include "../driver/pmu.h"
#include "../service/standby.h"


typedef struct arisc_para {
	u32 para_info;
	u32 para_magic;
	struct dram_para dram_para;
} arisc_para_t;

extern struct arisc_para arisc_para;

#define ARISC_PARA_SIZE (sizeof(struct arisc_para))

extern void arisc_para_init(void);
extern void set_paras(void);

#endif /* __ARISC_PARA_H__ */

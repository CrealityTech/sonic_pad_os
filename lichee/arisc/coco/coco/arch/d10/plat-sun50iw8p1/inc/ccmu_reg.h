#ifndef __CCMU_REG_H__
#define __CCMU_REG_H__

#include "lib_inc.h"

#define PLL_CPUX_CTRL_REG		0x0000
#define PLL_PERI0_CTRL_REG		0x0020
#define PLL_PERI1_CTRL_REG		0x0028
#define PSI_AHB1_AHB2_CFG_REG		0x0510
#define AHB3_CFG_REG			0x051c
#define APB1_CFG_REG			0x0520
#define APB2_CFG_REG			0x0524
#define MSGBOX_BUS_GATING_REG		0x071c
#define SPINLOCK_BUS_GATING_REG		0x072c
#define CCMU_VERSION_REG		0x0ff0


#define CCMU_FACTOR_M_OFFSET		0
#define CCMU_FACTOR_M_MASK		0x3
#define CCMU_FACTOR_N_OFFSET		8
#define CCMU_FACTOR_N_MASK		0xff



#endif



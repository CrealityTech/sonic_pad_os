#ifndef __HEAD_PARA_H__
#define __HEAD_PARA_H__

#include "dram.h"

#define IR_NUM_KEY_SUP                  (64)


typedef struct box_start_os_cfg
{
	u32 used;
	u32 start_type;
	u32 irkey_used;
	u32 pmukey_used;
	u32 pmukey_num;
	u32 led_power;
	u32 led_state;
} box_start_os_cfg_t;

typedef struct ir_code
{
	u32 key_code;
	u32 addr_code;
} ir_code_t;

typedef struct ir_key
{
	u32 num;
	ir_code_t ir_code_depot[IR_NUM_KEY_SUP];
} ir_key_t;


typedef enum power_dm
{
	DM_CPUA = 0, /* 0  */
	DM_CPUB,     /* 1  */
	DM_DRAM,     /* 2  */
	DM_GPU,      /* 3  */
	DM_SYS,      /* 4  */
	DM_VPU,      /* 5  */
	DM_CPUS,     /* 6  */
	DM_DRAMPLL,  /* 7  */
	DM_ADC,      /* 8  */
	DM_PL,       /* 9  */
	DM_PM,       /* 10 */
	DM_IO,       /* 11 */
	DM_CPVDD,    /* 12 */
	DM_LDOIN,    /* 13 */
	DM_PLL,      /* 14 */
	DM_LPDDR,    /* 15 */
	DM_TEST,     /* 16 */
	DM_RES1,     /* 17 */
	DM_RES2,     /* 18 */
	DM_RES3,     /* 19 */
	DM_MAX,      /* 20 */
} power_dm_e;

typedef struct arisc_para
{
	u32 para_info;
	u32 para_magic;
	struct dram_para dram_para;
	u32 message_pool_phys;
	u32 message_pool_size;
	u32 standby_base;
	u32 standby_size;
	u32 suart_status;
	u32 pmu_bat_shutdown_ltf;
	u32 pmu_bat_shutdown_htf;
	u32 pmu_pwroff_vol;
	u32 power_mode;
	u32 power_start;
	u32 powchk_used;
	u32 power_reg;
	u32 system_power;
	u32 vdd_cpua;
	u32 vdd_cpub;
	u32 vdd_sys;
	u32 vcc_pll;
	struct box_start_os_cfg start_os;
	struct ir_key ir_key;
	u32 resvert[65];
} arisc_para_t;


#endif /* __SUNXI_HARDWARE_H */

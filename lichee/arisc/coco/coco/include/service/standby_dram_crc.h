#ifndef __STANDBY_DRAM_CRC_H__
#define __STANDBY_DRAM_CRC_H__

s32 standby_set_dram_crc_paras(task_struct *task);
s32 standby_dram_crc_enable(void);
u32 standby_dram_crc(void);

s32 standby_dram_crc_init(void);

#endif


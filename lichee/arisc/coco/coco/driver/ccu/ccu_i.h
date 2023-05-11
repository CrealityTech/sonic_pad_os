#ifndef __CCU_I_H__
#define __CCU_I_H__

#include "ibase.h"
#include "driver.h"
#include "device.h"
#include "notifier.h"

struct ccu_ops {
	s32 (*set_div)(ccu_device *ccu, u32 reg, u32 div);
	s32 (*set_src)(ccu_device *ccu, u32 reg, u32 clk);
	s32 (*get_freq)(ccu_device *ccu, u32 reg);
	s32 (*set_freq)(ccu_device *ccu, u32 reg, u32 freq);
	s32 (*reset_reg)(ccu_device *ccu, u32 reg);
	s32 (*reset_clk)(ccu_device *ccu, u32 reg);
	s32 (*suspend_early)(arisc_device *dev);
	s32 (*resume_early)(arisc_device *dev);
};

s32 calc_clk_ratio_nm(u32 div, u32 *n, u32 *m);

#endif




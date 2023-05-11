/*
 * (C) Copyright 2013-2016
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * wangwei <wangwei@allwinnertech.com>
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __SID_H__
#define __SID_H__

#include <linux/types.h>
#include <asm/arch/cpu.h>

#define SID_PRCTL               (IOMEM_ADDR(SUNXI_SID_BASE) + 0x40)
#define SID_PRKEY               (IOMEM_ADDR(SUNXI_SID_BASE) + 0x50)
#define SID_RDKEY               (IOMEM_ADDR(SUNXI_SID_BASE) + 0x60)
#define SJTAG_AT0               (IOMEM_ADDR(SUNXI_SID_BASE) + 0x80)
#define SJTAG_AT1               (IOMEM_ADDR(SUNXI_SID_BASE) + 0x84)
#define SJTAG_S                 (IOMEM_ADDR(SUNXI_SID_BASE) + 0x88)
#define SID_EFUSE               (IOMEM_ADDR(SUNXI_SID_BASE) + 0x200)
#define SID_SECURE_MODE         (IOMEM_ADDR(SUNXI_SID_BASE) + 0xA0)
#define SID_OP_LOCK  (0xAC)

#define EFUSE_CHIPID            (0x0)
#define EFUSE_ROTPK             (0x38)
/* write protect */
#define EFUSE_WRITE_PROTECT     (0x28) /* 0x28-0x2b, 32bits */
/* read  protect */
#define EFUSE_READ_PROTECT      (0x2c) /* 0x2c-0x2f, 32bits */
/* jtag security */
#define EFUSE_LCJS              (0x30)

/*efuse power ctl*/
#define EFUSE_HV_SWITCH			(IOMEM_ADDR(SUNXI_RTC_BASE) + 0x204)
#endif    /*  #ifndef __SID_H__  */

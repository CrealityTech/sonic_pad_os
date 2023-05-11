/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : lib_inc.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-04-13 15:16
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __PLATFORM_CONFIG_H__
#define __PLATFORM_CONFIG_H__

#include <stdarg.h>     //use gcc stardard header file
#include <stddef.h>     //use gcc stardard define keyword, like size_t, ptrdiff_t
#include "ibase.h"

#include "ccmu_reg.h"
#include "ccu_lib.h"
#include "prcm_reg.h"
#include "reg_base.h"
#include "cpucfg_regs.h"
#include "irqnum_config.h"
#include "rsb_address.h"
#include "ibase.h"

#define ARISC_USE_DRAM_CODE_PBASE       (0x419fb000)
#define ARISC_USE_DRAM_CODE_SIZE        (0x00003000)
#define ARISC_USE_DRAM_DATA_PBASE       (ARISC_USE_DRAM_CODE_PBASE + ARISC_USE_DRAM_CODE_SIZE)
#define ARISC_USE_DRAM_DATA_SIZE        (0x00002000)

#define SUNXI_TIMESTAMP_STA_PBASE       (0x08110000)
#define SUNXI_TIMESTAMP_STA_SIZE        (0x00001000)
#define SUNXI_TIMESTAMP_CTRL_PBASE      (0x08120000)
#define SUNXI_TIMESTAMP_CTRL_SIZE       (0x00001000)

#define SUNXI_SRAMCTRL_PBASE            (0x03000000)
#define SUNXI_CCM_PBASE                 (0x03001000)
#define SUNXI_PIO_PBASE                 (0x0300b000)

#endif


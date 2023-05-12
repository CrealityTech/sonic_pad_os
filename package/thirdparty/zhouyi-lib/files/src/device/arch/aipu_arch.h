/**********************************************************************************
 * This file is CONFIDENTIAL and any use by you is subject to the terms of the
 * agreement between you and Arm China or the terms of the agreement between you
 * and the party authorised by Arm China to disclose this file to you.
 * The confidential and proprietary information contained in this file
 * may only be used by a person authorised under and to the extent permitted
 * by a subsisting licensing agreement from Arm China.
 *
 *        (C) Copyright 2020 Arm Technology (China) Co. Ltd.
 *                    All rights reserved.
 *
 * This entire notice must be reproduced on all copies of this file and copies of
 * this file may only be made by a person if such person is permitted to do so
 * under the terms of a subsisting license agreement from Arm China.
 *
 *********************************************************************************/

/**
 * @file  aipu_arch.h
 * @brief AIPU User Mode Driver (UMD) AIPU arch module header
 */

#ifndef _AIPU_ARCH_H_
#define _AIPU_ARCH_H_

#include <stdint.h>

typedef struct aipu_architecture {
    char     sys_info[20];
    uint32_t vreg_num;
    uint32_t lsram_size_kb;
    uint32_t lsram_bank;
    uint32_t gsram_size_kb;
    uint32_t gsram_bank;
    uint32_t dma_chan;
    char     has_iram[10];
    char     has_dcache[10];
    uint32_t icache_size;
    uint32_t hwa_mac;
    uint32_t tec_num;
} aipu_arch_t;

#define AIPU_CONFIG_0904 904   /* SC000 */
#define AIPU_CONFIG_1002 1002  /* SC002 */
#define AIPU_CONFIG_0701 701   /* SC003 */

#define AIPU_HWA_FEATURE_0904 0x533
#define AIPU_HWA_FEATURE_1002 0x544
#define AIPU_HWA_FEATURE_0701 0x10111

#define AIPU_ARCH_CONFIG_MAX 3

#endif /* _AIPU_ARCH_H_ */

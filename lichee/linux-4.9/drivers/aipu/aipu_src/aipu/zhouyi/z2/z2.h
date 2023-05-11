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
 * @file z2.h
 * Header of the zhouyi v2 AIPU hardware control info
 */

#ifndef _Z2_H_
#define _Z2_H_

#include "zhouyi.h"

/**
 * Zhouyi v2 AIPU Specific Interrupts
 */
#define ZHOUYI_IRQ_FAULT  0x8

#define ZHOUYIV2_IRQ  (ZHOUYI_IRQ | ZHOUYI_IRQ_FAULT)
#define ZHOUYIV2_IRQ_ENABLE_FLAG  (ZHOUYIV2_IRQ)
#define ZHOUYIV2_IRQ_DISABLE_FLAG (ZHOUYI_IRQ_NONE)

#define ZHOUYI_V2_MAX_SCHED_JOB_NUM  1

#define ZHOUYI_V2_ASE_READ_ENABLE           (1<<31)
#define ZHOUYI_V2_ASE_WRITE_ENABLE          (1<<30)
#define ZHOUYI_V2_ASE_RW_ENABLE             (3<<30)
#define ZHOUYI_V2_ASE_CTRL_SIZE(value) \
        (ilog2((max((u64)4096, (u64)roundup_pow_of_two(value))>>12)) + 1)

/**
 * Zhouyi v2 AIPU Specific Host Control Register Map
 */
#define AIPU_DATA_ADDR_2_REG_OFFSET         0x1C
#define AIPU_DATA_ADDR_3_REG_OFFSET         0x20
#define AIPU_SECURE_CONFIG_REG_OFFSET       0x30
#define AIPU_POWER_CTRL_REG_OFFSET          0x38
#define AIPU_ADDR_EXT0_CTRL_REG_OFFSET      0xC0
#define AIPU_ADDR_EXT0_HIGH_BASE_REG_OFFSET 0xC4
#define AIPU_ADDR_EXT0_LOW_BASE_REG_OFFSET  0xC8
#define AIPU_ADDR_EXT1_CTRL_REG_OFFSET      0xCC
#define AIPU_ADDR_EXT1_HIGH_BASE_REG_OFFSET 0xD0
#define AIPU_ADDR_EXT1_LOW_BASE_REG_OFFSET  0xD4
#define AIPU_ADDR_EXT2_CTRL_REG_OFFSET      0xD8
#define AIPU_ADDR_EXT2_HIGH_BASE_REG_OFFSET 0xDC
#define AIPU_ADDR_EXT2_LOW_BASE_REG_OFFSET  0xE0
#define AIPU_ADDR_EXT3_CTRL_REG_OFFSET      0xE4
#define AIPU_ADDR_EXT3_HIGH_BASE_REG_OFFSET 0xE8
#define AIPU_ADDR_EXT3_LOW_BASE_REG_OFFSET  0xEC

#endif /* _Z2_H_ */
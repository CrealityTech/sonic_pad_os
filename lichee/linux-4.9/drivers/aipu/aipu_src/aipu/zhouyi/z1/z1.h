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
 * @file z1.h
 * Header of the zhouyi v1 AIPU hardware control info
 */

#ifndef _Z1_H_
#define _Z1_H_

#include "zhouyi.h"

/**
 * Zhouyi V1 AIPU Interrupts
 */
#define ZHOUYIV1_IRQ              (ZHOUYI_IRQ)
#define ZHOUYIV1_IRQ_ENABLE_FLAG  (ZHOUYIV1_IRQ)
#define ZHOUYIV1_IRQ_DISABLE_FLAG (ZHOUYI_IRQ_NONE)

#define ZHOUYI_V1_MAX_SCHED_JOB_NUM  1

/**
 * Zhouyi V1 AIPU Specific Host Control Register Map
 */
#define ZHOUYI_INTR_CAUSE_REG_OFFSET          0x20
#define ZHOUYI_INTR_STAT_REG_OFFSET           0x24
#define ZHOUYI_INTR_BACKUP_STAT_REG_OFFSET    0x28
#define ZHOUYI_INTR_BACKUP_PC_REG_OFFSET      0x2C
#define ZHOUYI_DBG_ERR_CAUSE_REG_OFFSET       0x30
#define ZHOUYI_DBG_DATA_REG_0_OFFSET          0x34
#define ZHOUYI_DBG_DATA_REG_1_OFFSET          0x38
#define ZHOUYI_L2_CACHE_FEATURE_REG_OFFSET    0x6C

#endif /* _Z1_H_ */
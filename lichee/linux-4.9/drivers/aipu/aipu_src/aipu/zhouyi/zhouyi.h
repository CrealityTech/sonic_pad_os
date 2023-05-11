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
 * @file zhouyi.h
 * Header of the zhouyi AIPU hardware control and interrupt handle operations
 */

#ifndef _AIPU_ZHOUYI_H_
#define _AIPU_ZHOUYI_H_

#include <linux/device.h>
#include "uk_interface/aipu_capability.h"
#include "uk_interface/aipu_io_req.h"
#include "aipu_io.h"

/**
 * Zhouyi AIPU Common Interrupts
 */
#define ZHOUYI_IRQ_NONE   0x0
#define ZHOUYI_IRQ_QEMPTY 0x1
#define ZHOUYI_IRQ_DONE   0x2
#define ZHOUYI_IRQ_EXCEP  0x4

#define ZHOUYI_IRQ  (ZHOUYI_IRQ_QEMPTY | ZHOUYI_IRQ_DONE | ZHOUYI_IRQ_EXCEP)

#define ZHOUYI_AIPU_IDLE_STATUS   0x70000

/**
 * Zhouyi AIPU Common Host Control Register Map
 */
#define ZHOUYI_CTRL_REG_OFFSET                0x0
#define ZHOUYI_STAT_REG_OFFSET                0x4
#define ZHOUYI_START_PC_REG_OFFSET            0x8
#define ZHOUYI_INTR_PC_REG_OFFSET             0xC
#define ZHOUYI_IPI_CTRL_REG_OFFSET            0x10
#define ZHOUYI_DATA_ADDR_0_REG_OFFSET         0x14
#define ZHOUYI_DATA_ADDR_1_REG_OFFSET         0x18
#define ZHOUYI_CLK_CTRL_REG_OFFSET            0x3C
#define ZHOUYI_ISA_VERSION_REG_OFFSET         0x40
#define ZHOUYI_TPC_FEATURE_REG_OFFSET         0x44
#define ZHOUYI_SPU_FEATURE_REG_OFFSET         0x48
#define ZHOUYI_HWA_FEATURE_REG_OFFSET         0x4C
#define ZHOUYI_REVISION_ID_REG_OFFSET         0x50
#define ZHOUYI_MEM_FEATURE_REG_OFFSET         0x54
#define ZHOUYI_INST_RAM_FEATURE_REG_OFFSET    0x58
#define ZHOUYI_LOCAL_SRAM_FEATURE_REG_OFFSET  0x5C
#define ZHOUYI_GLOBAL_SRAM_FEATURE_REG_OFFSET 0x60
#define ZHOUYI_INST_CACHE_FEATURE_REG_OFFSET  0x64
#define ZHOUYI_DATA_CACHE_FEATURE_REG_OFFSET  0x68

int zhouyi_read_status_reg(struct io_region* io);
void zhouyi_clear_qempty_interrupt(struct io_region* io);
void zhouyi_clear_done_interrupt(struct io_region* io);
void zhouyi_clear_excep_interrupt(struct io_region* io);
int zhouyi_query_cap(struct io_region* io, struct aipu_cap* cap);
void zhouyi_io_rw(struct io_region* io, struct aipu_io_req* io_req);

#endif /* _AIPU_ZHOUYI_H_ */
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
 * @file z2.c
 * Implementation of the zhouyi v1 AIPU hardware cntrol interfaces
 */

#include "aipu.h"
#include "z2.h"
#include "aipu_io.h"
#include "uk_interface/aipu_errcode.h"

static void zhouyi_v2_enable_interrupt(struct aipu_core *core)
{
        if (core)
                aipu_write32(core->base0, ZHOUYI_CTRL_REG_OFFSET,
                        ZHOUYIV2_IRQ_ENABLE_FLAG);
}

static void zhouyi_v2_disable_interrupt(struct aipu_core *core)
{
        if (core)
                aipu_write32(core->base0, ZHOUYI_CTRL_REG_OFFSET,
                        ZHOUYIV2_IRQ_DISABLE_FLAG);
}

static void zhouyi_v2_clear_qempty_interrupt(struct aipu_core *core)
{
        if (core)
                zhouyi_clear_qempty_interrupt(core->base0);
}

static void zhouyi_v2_clear_done_interrupt(struct aipu_core *core)
{
        if (core)
                zhouyi_clear_done_interrupt(core->base0);
}

static void zhouyi_v2_clear_excep_interrupt(struct aipu_core *core)
{
        if (core)
                zhouyi_clear_excep_interrupt(core->base0);
}

static void zhouyi_v2_clear_fault_interrupt(struct aipu_core *core)
{
        if (core)
                aipu_write32(core->base0, ZHOUYI_STAT_REG_OFFSET, ZHOUYI_IRQ_FAULT);
}

static int zhouyi_v2_trigger(struct aipu_core *core, struct user_job_desc *udesc, int tid)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        u32 start_pc = 0;
        u32 intr_pc = 0;
        u32 data_0_pa = 0;
        u32 data_1_pa = 0;
        u64 ase0_start = min3(udesc->start_pc_addr, udesc->data_0_addr,
                udesc->data_1_addr);
        u64 ase0_end = max3(udesc->start_pc_addr + udesc->code_size,
                udesc->data_0_addr + udesc->rodata_size,
                udesc->data_1_addr + udesc->stack_size);

        /* Used when ASID is disabled */
        u32 ase0_base_high = udesc->start_pc_addr >> 32;

        if (!core) {
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
                goto finish;
        }

        if (udesc->enable_asid) {
                start_pc = (u32)(udesc->start_pc_addr - ase0_start);
                intr_pc = (u32)(udesc->intr_handler_addr - ase0_start);
                data_0_pa = (u32)(udesc->data_0_addr - ase0_start);
                data_1_pa = (u32)(udesc->data_1_addr - ase0_start);
        } else {
                start_pc = (u32)udesc->start_pc_addr;
                intr_pc = (u32)udesc->intr_handler_addr;
                data_0_pa = (u32)udesc->data_0_addr;
                data_1_pa = (u32)udesc->data_1_addr;
        }

        /* Load data addr 0 register */
        aipu_write32(core->base0, ZHOUYI_DATA_ADDR_0_REG_OFFSET, data_0_pa);

        /* Load data addr 1 register */
        aipu_write32(core->base0, ZHOUYI_DATA_ADDR_1_REG_OFFSET, data_1_pa);

        /* Load interrupt PC */
        aipu_write32(core->base0, ZHOUYI_INTR_PC_REG_OFFSET, intr_pc);

        /* Load ASE registers */
        if (udesc->enable_asid) {
                /* ASE 0 */
                aipu_write32(core->base0, AIPU_ADDR_EXT0_CTRL_REG_OFFSET,
                        ZHOUYI_V2_ASE_RW_ENABLE | ZHOUYI_V2_ASE_CTRL_SIZE(ase0_end - ase0_start));
                aipu_write32(core->base0, AIPU_ADDR_EXT0_HIGH_BASE_REG_OFFSET, ase0_start >> 32);
                aipu_write32(core->base0, AIPU_ADDR_EXT0_LOW_BASE_REG_OFFSET, 0);
                dev_dbg(core->dev, "ASE 0 Ctrl 0x%x, ASE 0 PA 0x%llx",
                        aipu_read32(core->base0, AIPU_ADDR_EXT0_CTRL_REG_OFFSET),
                        ((u64)aipu_read32(core->base0, AIPU_ADDR_EXT0_HIGH_BASE_REG_OFFSET) << 32) +
                        aipu_read32(core->base0, AIPU_ADDR_EXT0_LOW_BASE_REG_OFFSET));
                /* ASE 1 */
                aipu_write32(core->base0, AIPU_ADDR_EXT1_CTRL_REG_OFFSET,
                        ZHOUYI_V2_ASE_READ_ENABLE | ZHOUYI_V2_ASE_CTRL_SIZE(udesc->static_size));
                aipu_write32(core->base0, AIPU_ADDR_EXT1_HIGH_BASE_REG_OFFSET, udesc->static_addr >> 32);
                aipu_write32(core->base0, AIPU_ADDR_EXT1_LOW_BASE_REG_OFFSET, (u32)udesc->static_addr);
                dev_dbg(core->dev, "ASE 1 Ctrl 0x%x, ASE 1 PA 0x%llx",
                        aipu_read32(core->base0, AIPU_ADDR_EXT1_CTRL_REG_OFFSET),
                        ((u64)aipu_read32(core->base0, AIPU_ADDR_EXT1_HIGH_BASE_REG_OFFSET) << 32) +
                        aipu_read32(core->base0, AIPU_ADDR_EXT1_LOW_BASE_REG_OFFSET));
                /* ASE 2 */
                aipu_write32(core->base0, AIPU_ADDR_EXT2_CTRL_REG_OFFSET,
                        ZHOUYI_V2_ASE_RW_ENABLE | ZHOUYI_V2_ASE_CTRL_SIZE(udesc->reuse_size));
                aipu_write32(core->base0, AIPU_ADDR_EXT2_HIGH_BASE_REG_OFFSET, udesc->reuse_addr >> 32);
                aipu_write32(core->base0, AIPU_ADDR_EXT2_LOW_BASE_REG_OFFSET, (u32)udesc->reuse_addr);
                dev_dbg(core->dev, "ASE 2 Ctrl 0x%x, ASE 2 PA 0x%llx",
                        aipu_read32(core->base0, AIPU_ADDR_EXT2_CTRL_REG_OFFSET),
                        ((u64)aipu_read32(core->base0, AIPU_ADDR_EXT2_HIGH_BASE_REG_OFFSET) << 32) +
                        aipu_read32(core->base0, AIPU_ADDR_EXT2_LOW_BASE_REG_OFFSET));
        } else {
                /* default: ASE 0 */
                aipu_write32(core->base0, AIPU_ADDR_EXT0_CTRL_REG_OFFSET,
                        ZHOUYI_V2_ASE_RW_ENABLE);
                aipu_write32(core->base0, AIPU_ADDR_EXT0_HIGH_BASE_REG_OFFSET, ase0_base_high);
                aipu_write32(core->base0, AIPU_ADDR_EXT0_LOW_BASE_REG_OFFSET, 0);
                dev_dbg(core->dev, "ASE 0 Ctrl 0x%x, ASE 0 PA 0x%llx",
                        aipu_read32(core->base0, AIPU_ADDR_EXT0_CTRL_REG_OFFSET),
                        ((u64)aipu_read32(core->base0, AIPU_ADDR_EXT0_HIGH_BASE_REG_OFFSET) << 32) +
                        aipu_read32(core->base0, AIPU_ADDR_EXT0_LOW_BASE_REG_OFFSET));
        }

        /* Load start PC register */
        start_pc |= 0xD;
        aipu_write32(core->base0, ZHOUYI_START_PC_REG_OFFSET, start_pc);

        if (tid)
                dev_info(core->dev, "[%u] trigger Job 0x%x done: start pc = 0x%x, dreg0 = 0x%x, dreg1 = 0x%x",
                        tid, udesc->job_id, start_pc, data_0_pa, data_1_pa);
        else
                dev_info(core->dev, "[IRQ] trigger Job 0x%x done: start pc = 0x%x, dreg0 = 0x%x, dreg1 = 0x%x",
                        udesc->job_id, start_pc, data_0_pa, data_1_pa);

finish:
        return ret;
}

static bool zhouyi_v2_is_idle(struct aipu_core *core)
{
        if (!core) {
                pr_err("invalid input args core to be NULL!");
                return 0;
        }
        return AIPU_BIT(aipu_read32(core->base0, ZHOUYI_STAT_REG_OFFSET), 16) &&
                AIPU_BIT(aipu_read32(core->base0, ZHOUYI_STAT_REG_OFFSET), 17) &&
                AIPU_BIT(aipu_read32(core->base0, ZHOUYI_STAT_REG_OFFSET), 18);
}

static int zhouyi_v2_read_status_reg(struct aipu_core *core)
{
        if (!core)
                return 0;
        return zhouyi_read_status_reg(core->base0);
}

static void zhouyi_v2_print_hw_id_info(struct aipu_core *core)
{
        int ret = 0;

        if (!core) {
                pr_err("invalid input args core to be NULL!");
                return;
        }

        ret = aipu_read32(core->base0, ZHOUYI_STAT_REG_OFFSET);
        dev_info(core->dev, "AIPU Initial Status: 0x%x.", ret);

        dev_info(core->dev, "###### ZHOUYI V2 HARDWARE INFORMATION #######");
        dev_info(core->dev, "# ISA Version Register: 0x%x", aipu_read32(core->base0, ZHOUYI_ISA_VERSION_REG_OFFSET));
        dev_info(core->dev, "# TPC Feature Register: 0x%x", aipu_read32(core->base0, ZHOUYI_TPC_FEATURE_REG_OFFSET));
        dev_info(core->dev, "# SPU Feature Register: 0x%x", aipu_read32(core->base0, ZHOUYI_SPU_FEATURE_REG_OFFSET));
        dev_info(core->dev, "# HWA Feature Register: 0x%x", aipu_read32(core->base0, ZHOUYI_HWA_FEATURE_REG_OFFSET));
        dev_info(core->dev, "# Revision ID Register: 0x%x", aipu_read32(core->base0, ZHOUYI_REVISION_ID_REG_OFFSET));
        dev_info(core->dev, "# Memory Hierarchy Feature Register: 0x%x", aipu_read32(core->base0, ZHOUYI_MEM_FEATURE_REG_OFFSET));
        dev_info(core->dev, "# Instruction RAM Feature Register:  0x%x", aipu_read32(core->base0, ZHOUYI_INST_RAM_FEATURE_REG_OFFSET));
        dev_info(core->dev, "# TEC Local SRAM Feature Register:   0x%x", aipu_read32(core->base0, ZHOUYI_LOCAL_SRAM_FEATURE_REG_OFFSET));
        dev_info(core->dev, "# Global SRAM Feature Register:      0x%x", aipu_read32(core->base0, ZHOUYI_GLOBAL_SRAM_FEATURE_REG_OFFSET));
        dev_info(core->dev, "# Instruction Cache Feature Register:0x%x", aipu_read32(core->base0, ZHOUYI_INST_CACHE_FEATURE_REG_OFFSET));
        dev_info(core->dev, "# Data Cache Feature Register:       0x%x", aipu_read32(core->base0, ZHOUYI_DATA_CACHE_FEATURE_REG_OFFSET));
        dev_info(core->dev, "#############################################");
}

static int zhouyi_v2_query_cap(struct aipu_core *core, struct aipu_cap *cap)
{
        if (!core)
                return 0;
        return zhouyi_query_cap(core->base0, cap);
}

static void zhouyi_v2_io_rw(struct aipu_core *core, struct aipu_io_req *io_req)
{
        if (core)
                zhouyi_io_rw(core->base0, io_req);
}

static int zhouyi_v2_upper_half(void *data)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        struct aipu_priv* aipu = (struct aipu_priv*)data;
        struct aipu_core* core = aipu->core0;

        if (!core) {
                ret = AIPU_ERRCODE_INTERNAL_NULLPTR;
                goto finish;
        }

        zhouyi_v2_disable_interrupt(core);
        ret = zhouyi_v2_read_status_reg(core);
        if (ret & ZHOUYI_IRQ_QEMPTY) {
                zhouyi_v2_clear_qempty_interrupt(core);
        }

        if (ret & ZHOUYI_IRQ_DONE) {
                zhouyi_v2_clear_done_interrupt(core);
                aipu_job_manager_update_job_state_irq(aipu, 0);
                aipu_irq_schedulework(core->irq_obj);
        }

        if (ret & ZHOUYI_IRQ_EXCEP) {
                zhouyi_v2_clear_excep_interrupt(core);
                aipu_job_manager_update_job_state_irq(aipu, 1);
                aipu_irq_schedulework(core->irq_obj);
        }

        if (ret & ZHOUYI_IRQ_FAULT)
                zhouyi_v2_clear_fault_interrupt(core);
        zhouyi_v2_enable_interrupt(core);

        /* success */
        ret = AIPU_ERRCODE_NO_ERROR;

finish:
        return ret;
}

static void zhouyi_v2_bottom_half(void *data)
{
        struct aipu_priv *aipu = (struct aipu_priv*)data;
        aipu_job_manager_update_job_queue_done_irq(&aipu->job_manager);
}

struct aipu_io_operation zhouyi_v2_ops = {
        .enable_interrupt = zhouyi_v2_enable_interrupt,
        .disable_interrupt = zhouyi_v2_disable_interrupt,
        .trigger = zhouyi_v2_trigger,
        .is_idle = zhouyi_v2_is_idle,
        .read_status_reg = zhouyi_v2_read_status_reg,
        .print_hw_id_info = zhouyi_v2_print_hw_id_info,
        .query_capability = zhouyi_v2_query_cap,
        .io_rw = zhouyi_v2_io_rw,
        .upper_half = zhouyi_v2_upper_half,
        .bottom_half = zhouyi_v2_bottom_half,
};
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
 * @file aipu.c
 * Implementation of the aipu device struct creation & destroy
 */

#include <linux/of.h>
#include "aipu.h"
#include "aipu_fops.h"
#include "aipu_sysfs.h"
#include "config.h"

#if ((defined BUILD_ZHOUYI_V1) || (defined BUILD_ZHOUYI_COMPATIBLE))
extern struct aipu_io_operation zhouyi_v1_ops;
#endif
#if ((defined BUILD_ZHOUYI_V2) || (defined BUILD_ZHOUYI_COMPATIBLE))
extern struct aipu_io_operation zhouyi_v2_ops;
#endif
#if ((defined BUILD_PLATFORM_JUNO) && (BUILD_PLATFORM_JUNO == 1))
extern struct soc_io_operation  junor2_soc_ops;
#elif ((defined BUILD_PLATFORM_6CG) && (BUILD_PLATFORM_6CG == 1))
extern struct soc_io_operation  x6cg_soc_ops;
#elif ((defined BUILD_PLATFORM_DEFAULT) && (BUILD_PLATFORM_DEFAULT == 1))
extern struct soc_io_operation  default_soc_ops;
#endif

#if (((defined BUILD_ZHOUYI_V1) || (defined BUILD_ZHOUYI_COMPATIBLE)) && \
     (defined BUILD_PLATFORM_DEFAULT))
struct aipu_priv z1_platform_priv = {
        .board = AIPU_BOARD_BRAND_DEFAULT,
        .version = AIPU_VERSION_ZHOUYI_V1,
        .core_ctrl = &zhouyi_v1_ops,
        .soc_ctrl = &default_soc_ops,
};
#elif (((defined BUILD_ZHOUYI_V1) || (defined BUILD_ZHOUYI_COMPATIBLE)) && \
     (defined BUILD_PLATFORM_JUNO))
struct aipu_priv z1_platform_priv = {
        .board = AIPU_BOARD_BRAND_JUNOR2,
        .version = AIPU_VERSION_ZHOUYI_V1,
        .core_ctrl = &zhouyi_v1_ops,
        .soc_ctrl = &junor2_soc_ops,
};
#elif (((defined BUILD_ZHOUYI_V1) || (defined BUILD_ZHOUYI_COMPATIBLE)) && \
     (defined BUILD_PLATFORM_6CG))
struct aipu_priv z1_platform_priv = {
        .board = AIPU_BOARD_BRAND_JUNOR2,
        .version = AIPU_VERSION_ZHOUYI_V1,
        .core_ctrl = &zhouyi_v1_ops,
        .soc_ctrl = &x6cg_soc_ops,
};
#endif

#if (((defined BUILD_ZHOUYI_V2) || (defined BUILD_ZHOUYI_COMPATIBLE)) && \
     (defined BUILD_PLATFORM_DEFAULT))
struct aipu_priv z2_platform_priv = {
        .board = AIPU_BOARD_BRAND_DEFAULT,
        .version = AIPU_VERSION_ZHOUYI_V2,
        .core_ctrl = &zhouyi_v2_ops,
        .soc_ctrl = &default_soc_ops,
};
#elif (((defined BUILD_ZHOUYI_V2) || (defined BUILD_ZHOUYI_COMPATIBLE)) && \
     (defined BUILD_PLATFORM_JUNO))
struct aipu_priv z2_platform_priv = {
        .board = AIPU_BOARD_BRAND_JUNOR2,
        .version = AIPU_VERSION_ZHOUYI_V2,
        .core_ctrl = &zhouyi_v2_ops,
        .soc_ctrl = &junor2_soc_ops,
};
#elif (((defined BUILD_ZHOUYI_V2) || (defined BUILD_ZHOUYI_COMPATIBLE)) && \
     (defined BUILD_PLATFORM_6CG))
struct aipu_priv z2_platform_priv = {
        .board = AIPU_BOARD_BRAND_ZYNQ6CG,
        .version = AIPU_VERSION_ZHOUYI_V2,
        .core_ctrl = &zhouyi_v2_ops,
        .soc_ctrl = &x6cg_soc_ops,
};
#endif

static int init_misc_dev(struct aipu_priv *aipu)
{
        int ret = 0;

        if (!aipu)
                return -EINVAL;

        aipu_fops_register(&aipu->aipu_fops);

        aipu->misc = devm_kzalloc(aipu->dev, sizeof(struct miscdevice), GFP_KERNEL);
        if (!aipu->misc) {
                dev_err(aipu->dev, "no memory in allocating misc struct\n");
                return -ENOMEM;
        }

        /* misc init */
        aipu->misc->minor = MISC_DYNAMIC_MINOR;
        aipu->misc->name = "aipu";
        aipu->misc->fops = &aipu->aipu_fops;
        aipu->misc->mode = 0666;
        ret = misc_register(aipu->misc);
        if (ret)
                dev_err(aipu->dev, "AIPU misc register failed");

        return ret;
}

static void deinit_misc_dev(struct aipu_priv *aipu)
{
        if (aipu)
                misc_deregister(aipu->misc);
}

int init_aipu_priv(struct aipu_priv *aipu, struct device *dev)
{
        int ret = 0;

        if ((!dev) || (!aipu)) {
                dev_err(dev, "invalid input args dts/aipu to be NULL\n");
                return -EINVAL;
        }

        aipu->dev = dev;
        mutex_init(&aipu->lock);
        aipu->core0 = NULL;
        aipu->soc = NULL;
        aipu->misc = NULL;
        aipu->is_suspend = 0;

        /* init memory manager */
        ret = aipu_init_mm(&aipu->mm, dev, aipu->version);
        if (ret)
                goto err_handle;

        /* init misc device and fops */
        ret = init_misc_dev(aipu);
        if (ret)
                goto err_handle;

#ifdef AIPU_ENABLE_SYSFS
        ret = aipu_create_sysfs(aipu);
        if (ret)
                goto err_handle;
#endif

        goto finish;

err_handle:
        deinit_aipu_priv(aipu);

finish:
        return ret;
}

int aipu_priv_init_core(struct aipu_priv *aipu, int irqnum, u64 base, u64 size)
{
        int ret = 0;

        if (!aipu)
                return -EINVAL;

        ret = create_aipu_core(aipu->version, irqnum, base,
                size, 0, aipu->core_ctrl->upper_half,
               aipu->core_ctrl->bottom_half, aipu, aipu->dev, &aipu->core0);
        if (ret)
                return ret;

        ret = aipu_init_job_manager(&aipu->job_manager, aipu->dev, aipu->core0->max_sched_num);
        if (ret)
                return ret;

        return ret;
}

int aipu_priv_init_soc(struct aipu_priv *aipu, u64 base, u64 size)
{
        if ((!aipu) || (!aipu->dev))
                return -EINVAL;

        if (of_property_read_u64(aipu->dev->of_node, "host-aipu-offset",
            &aipu->host_to_aipu_map))
                aipu->host_to_aipu_map = 0;

        /* size == 0 means that no SoC IO exists */
        if (size)
                return create_aipu_soc(aipu->board, base, size, aipu->dev, &aipu->soc);
        else
                return 0;
}

int aipu_priv_add_mem_region(struct aipu_priv *aipu, u64 base, u64 size,
        enum aipu_mem_type type)
{
        if (!aipu)
                return -EINVAL;

        return aipu_mm_add_region(&aipu->mm, base, size, type);
}

int aipu_priv_get_version(struct aipu_priv *aipu)
{
        if (aipu)
                return aipu->core0->version;
        return 0;
}

void aipu_priv_enable_interrupt(struct aipu_priv *aipu)
{
        if (aipu)
               aipu->core_ctrl->enable_interrupt(aipu->core0);
}

void aipu_priv_disable_interrupt(struct aipu_priv *aipu)
{
        if (aipu)
               aipu->core_ctrl->disable_interrupt(aipu->core0);
}

int aipu_priv_trigger(struct aipu_priv *aipu, struct user_job_desc *udesc, int tid)
{
        if (aipu)
                return aipu->core_ctrl->trigger(aipu->core0, udesc, tid);
        return -EINVAL;
}

bool aipu_priv_is_idle(struct aipu_priv *aipu)
{
        if (aipu)
                return aipu->core_ctrl->is_idle(aipu->core0);
        return 0;
}

int aipu_priv_query_capability(struct aipu_priv *aipu, struct aipu_cap *cap)
{
        if (aipu)
                return aipu->core_ctrl->query_capability(aipu->core0, cap);
        return -EINVAL;
}

void aipu_priv_io_rw(struct aipu_priv *aipu, struct aipu_io_req *io_req)
{
        if (aipu)
                aipu->core_ctrl->io_rw(aipu->core0, io_req);
}

void aipu_priv_print_hw_id_info(struct aipu_priv *aipu)
{
        if (aipu)
               aipu->core_ctrl->print_hw_id_info(aipu->core0);
}

void aipu_priv_start_bw_profiling(struct aipu_priv *aipu)
{
        if (aipu)
                aipu->soc_ctrl->start_bw_profiling(aipu->soc);
}

void aipu_priv_stop_bw_profiling(struct aipu_priv *aipu)
{
        if (aipu)
                aipu->soc_ctrl->stop_bw_profiling(aipu->soc);
}

void aipu_priv_read_profiling_reg(struct aipu_priv *aipu, struct profiling_data *pdata)
{
        if (aipu)
                aipu->soc_ctrl->read_profiling_reg(aipu->soc, pdata);
}

bool aipu_priv_has_logic_reset(struct aipu_priv *aipu)
{
        if (aipu)
                return aipu->soc_ctrl->has_logic_reset(aipu->soc);
        return 0;
}

void aipu_priv_logic_reset(struct aipu_priv *aipu)
{
        if (aipu)
                aipu->soc_ctrl->logic_reset(aipu->soc);
}

void aipu_priv_logic_release(struct aipu_priv *aipu)
{
        if (aipu)
                aipu->soc_ctrl->logic_release(aipu->soc);
}

void aipu_priv_logic_reset_release(struct aipu_priv *aipu)
{
        if (aipu) {
                aipu->soc_ctrl->logic_reset(aipu->soc);
                aipu->soc_ctrl->logic_release(aipu->soc);
        }
}

void aipu_priv_enable_clk_gating(struct aipu_priv *aipu)
{
        if (aipu)
                aipu->soc_ctrl->enable_clk_gating(aipu->soc);
}

void aipu_priv_disable_clk_gating(struct aipu_priv *aipu)
{
        if (aipu)
                aipu->soc_ctrl->disable_clk_gating(aipu->soc);
}

int deinit_aipu_priv(struct aipu_priv *aipu)
{
        if (!aipu)
                return 0;

#ifdef AIPU_ENABLE_SYSFS
        aipu_destroy_sysfs(aipu);
#endif

        aipu_deinit_mm(&aipu->mm);
        if (aipu->misc)
                deinit_misc_dev(aipu);
        if (aipu->soc)
                destroy_aipu_soc(aipu->soc);
        if (aipu->core0) {
                destroy_aipu_core(aipu->core0);
                aipu_deinit_job_manager(&aipu->job_manager);
        }

        return 0;
}
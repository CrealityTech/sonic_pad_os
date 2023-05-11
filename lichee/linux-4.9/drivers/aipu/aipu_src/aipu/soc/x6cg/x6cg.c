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
 * @file x6cg.c
 * Implementation of the xilinx 6CG-440 SoC/board function
 */

#include "aipu_soc.h"
#include "x6cg_reset.h"
#include "config.h"

static void x6cg_start_bw_profiling(struct aipu_soc *soc)
{
}

static void x6cg_stop_bw_profiling(struct aipu_soc *soc)
{
}

static void x6cg_read_profiling_reg(struct aipu_soc *soc, struct profiling_data *pdata)
{
        if ((soc) && (pdata)) {
                pdata->rdata_tot_msb = 0;
                pdata->rdata_tot_lsb = 0;
                pdata->wdata_tot_msb = 0;
                pdata->wdata_tot_lsb = 0;
                pdata->tot_cycle_msb = 0;
                pdata->tot_cycle_lsb = 0;
        }
}

static void x6cg_logic_reset(struct aipu_soc *soc)
{
        if (soc)
                X6CG_LOGIC_RESET_APIU(soc->base0);
}

static void x6cg_logic_release(struct aipu_soc *soc)
{
        if (soc)
                X6CG_RELEASE_APIU(soc->base0);
}

static void x6cg_enable_clk_gating(struct aipu_soc *soc)
{
}

static void x6cg_disable_clk_gating(struct aipu_soc *soc)
{
}

static bool x6cg_has_logic_reset(struct aipu_soc *soc)
{
        if (soc && soc->base0)
                return AIPU_ENABLE_RESET_HW_NONE_IDLE;
        return false;
}

struct soc_io_operation x6cg_soc_ops = {
        .start_bw_profiling = x6cg_start_bw_profiling,
        .stop_bw_profiling = x6cg_stop_bw_profiling,
        .read_profiling_reg = x6cg_read_profiling_reg,
        .logic_reset = x6cg_logic_reset,
        .logic_release = x6cg_logic_release,
        .enable_clk_gating = x6cg_enable_clk_gating,
        .disable_clk_gating = x6cg_disable_clk_gating,
        .has_logic_reset = x6cg_has_logic_reset,
};
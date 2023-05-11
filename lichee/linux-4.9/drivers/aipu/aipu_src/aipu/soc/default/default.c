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
 * @file default.c
 * Implementation of the default SoC/board function
 */

#include "aipu_soc.h"

static void default_start_bw_profiling(struct aipu_soc *soc)
{
}

static void default_stop_bw_profiling(struct aipu_soc *soc)
{
}

static void default_read_profiling_reg(struct aipu_soc *soc, struct profiling_data *pdata)
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

static void default_logic_reset(struct aipu_soc *soc)
{
}

static void default_logic_release(struct aipu_soc *soc)
{
}

static void default_enable_clk_gating(struct aipu_soc *soc)
{
}

static void default_disable_clk_gating(struct aipu_soc *soc)
{
}

static bool default_has_logic_reset(struct aipu_soc *soc)
{
        return false;
}

struct soc_io_operation default_soc_ops = {
        .start_bw_profiling = default_start_bw_profiling,
        .stop_bw_profiling = default_stop_bw_profiling,
        .read_profiling_reg = default_read_profiling_reg,
        .logic_reset = default_logic_reset,
        .logic_release = default_logic_release,
        .enable_clk_gating = default_enable_clk_gating,
        .disable_clk_gating = default_disable_clk_gating,
        .has_logic_reset = default_has_logic_reset,
};
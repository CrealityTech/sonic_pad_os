/**********************************************************************************
 * This file is CONFIDENTIAL and any use by you is subject to the terms of the
 * agreement between you and Arm China or the termscore->base0 of the agreement between you
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
 * @file junor2.c
 * Implementation of the juno r2 related functions
 */

#include "aipu_soc.h"
#include "aipu_io.h"
#include "junor2_bw_stat.h"
#include "junor2_clock_gating.h"
#include "junor2_reset.h"
#include "config.h"

static void junor2_start_bw_profiling(struct aipu_soc *soc)
{
        if (soc)
                JUNOR2_START_APIU_BW_STAT(soc->base0);
}

static void junor2_stop_bw_profiling(struct aipu_soc *soc)
{
        if (soc)
                JUNOR2_STOP_APIU_BW_STAT(soc->base0);
}

static void junor2_read_profiling_reg(struct aipu_soc *soc, struct profiling_data *pdata)
{
        if ((soc) && (pdata)) {
                pdata->rdata_tot_msb = aipu_read32(soc->base0, JUNOR2_ALL_RDATA_TOT_MSB);
                pdata->rdata_tot_lsb = aipu_read32(soc->base0, JUNOR2_ALL_RDATA_TOT_LSB);
                pdata->wdata_tot_msb = aipu_read32(soc->base0, JUNOR2_ALL_WDATA_TOT_MSB);
                pdata->wdata_tot_lsb = aipu_read32(soc->base0, JUNOR2_ALL_WDATA_TOT_LSB);
                pdata->tot_cycle_msb = aipu_read32(soc->base0, JUNOR2_TOT_CYCLE_MSB);
                pdata->tot_cycle_lsb = aipu_read32(soc->base0, JUNOR2_TOT_CYCLE_LSB);
        }
}

static void junor2_logic_reset(struct aipu_soc *soc)
{
        if (soc)
                JUNOR2_LOGIC_RESET_APIU(soc->base0);
}

static void junor2_logic_release(struct aipu_soc *soc)
{
        if (soc)
                JUNOR2_RELEASE_APIU(soc->base0);
}

static void junor2_enable_clk_gating(struct aipu_soc *soc)
{
        if (soc)
                JUNOR2_ENABLE_AIPU_CLK_GATING(soc->base0);
}

static void junor2_disable_clk_gating(struct aipu_soc *soc)
{
        if (soc)
                JUNOR2_DISABLE_AIPU_CLK_GATING(soc->base0);
}

static bool junor2_has_logic_reset(struct aipu_soc *soc)
{
        if (soc && soc->base0)
                return AIPU_ENABLE_RESET_HW_NONE_IDLE;
        return false;
}

struct soc_io_operation junor2_soc_ops = {
        .start_bw_profiling = junor2_start_bw_profiling,
        .stop_bw_profiling = junor2_stop_bw_profiling,
        .read_profiling_reg = junor2_read_profiling_reg,
        .logic_reset = junor2_logic_reset,
        .logic_release = junor2_logic_release,
        .enable_clk_gating = junor2_enable_clk_gating,
        .disable_clk_gating = junor2_disable_clk_gating,
        .has_logic_reset = junor2_has_logic_reset,
};
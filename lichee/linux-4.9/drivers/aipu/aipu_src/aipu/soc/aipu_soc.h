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
 * @file aipu_soc.h
 * Header of the aipu_soc struct
 */

#ifndef _AIPU_SOC_H_
#define _AIPU_SOC_H_

#include <linux/device.h>
#include "aipu_io.h"
#include "uk_interface/aipu_profiling.h"

enum aipu_board_soc_brand {
        AIPU_BOARD_BRAND_DEFAULT = 0,
        AIPU_BOARD_BRAND_JUNOR2,
        AIPU_BOARD_BRAND_HYBRID,
        AIPU_BOARD_BRAND_ZYNQ440,
        AIPU_BOARD_BRAND_ZYNQ6CG,
};

/**
 * struct aipu_soc - a general struct describe the AIPU soc chip or FPGA board function
 *
 * @brand: SoC chip/board brand
 * @base0: IO region of this AIPU core
 */
struct aipu_soc {
        int brand;
        struct io_region *base0;
};

/**
 * struct aipu_io_operation - a struct contains AIPU hardware operation methods
 *
 * @start_bw_profiling: Start bandwidth profiling
 * @stop_bw_profiling: Stop bandwidth profiling
 * @read_profiling_reg: read profiling register values
 * @logic_reset: trigger AIPU to run a job
 * @logic_release: Is AIPU hardware idle or not
 * @enable_clk_gating: Read status register value
 * @disable_clk_gating: Print AIPU version ID registers information
 * @has_logic_reset: Has logic reset function or not
 */
struct soc_io_operation {
        void (*start_bw_profiling)(struct aipu_soc *soc);
        void (*stop_bw_profiling)(struct aipu_soc *soc);
        void (*read_profiling_reg)(struct aipu_soc *soc, struct profiling_data *pdata);
        void (*logic_reset)(struct aipu_soc *soc);
        void (*logic_release)(struct aipu_soc *soc);
        void (*enable_clk_gating)(struct aipu_soc *soc);
        void (*disable_clk_gating)(struct aipu_soc *soc);
        bool (*has_logic_reset)(struct aipu_soc *soc);
};

/**
 * @brief create an AIPU soc struct, intialize the IO region, etc.
 *
 * @param brand: SoC chip/board brand
 * @param soc_base0: AIPU SoC IO base 0 address
 * @param base0_size: AIPU SoC IO base 0 size
 *
 * @return aipu_soc struct pointer if successful; NULL if failed;
 */
int create_aipu_soc(int brand, u64 soc_base0, u64 base0_size, struct device *dev,
        struct aipu_soc **p_soc);
/**
 * @brief destroy a created aipu_soc struct
 *
 * @param core: AIPU soc struct pointer created by create_aipu_soc
 */
void destroy_aipu_soc(struct aipu_soc *soc);

#endif /* _AIPU_SOC_H_ */
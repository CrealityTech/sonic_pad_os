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
 * @file aipu_clock_gating.h
 * Header of the AIPU clock gating feature
 */

#ifndef _JUNOR2_CLOCK_GATING_H_
#define _JUNOR2_CLOCK_GATING_H_

#include "aipu_io.h"

#define JUNOR2_CLK_GATING_REG_OFFSET         0x104
#define JUNOR2_CLK_GATING_ENABLE_FLAG        0x31
#define JUNOR2_CLK_GATING_DISABLE_FLAG       0x1

#define JUNOR2_ENABLE_AIPU_CLK_GATING(io) do { \
        aipu_write32(io, JUNOR2_CLK_GATING_REG_OFFSET, JUNOR2_CLK_GATING_ENABLE_FLAG); \
    } while (0)

#define JUNOR2_DISABLE_AIPU_CLK_GATING(io) do { \
        aipu_write32(io, JUNOR2_CLK_GATING_REG_OFFSET, JUNOR2_CLK_GATING_DISABLE_FLAG); \
    } while (0)

#endif /* _JUNOR2_RESET_H_ */
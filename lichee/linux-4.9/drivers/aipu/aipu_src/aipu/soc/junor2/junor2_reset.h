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
  * @file junor2_reset.h
  * Header of the AIPU reset feature - Junor2 board
  */

#ifndef _JUNOR2_RESET_H_
#define _JUNOR2_RESET_H_

#include "aipu_io.h"

#define JUNOR2_RESET_REG_OFFSET   0x104
#define JUNOR2_RESET_AIPU_FLAG    0x0
#define JUNOR2_RELEASE_AIPU_FLAG  0x1

#define JUNOR2_LOGIC_RESET_APIU(io) do { \
        aipu_write32(io, JUNOR2_RESET_REG_OFFSET, JUNOR2_RESET_AIPU_FLAG); \
    } while (0)

#define JUNOR2_RELEASE_APIU(io) do { \
        aipu_write32(io, JUNOR2_RESET_REG_OFFSET, JUNOR2_RELEASE_AIPU_FLAG); \
} while (0)

#define JUNOR2_RESET_APIU(io) do { \
        JUNOR2_LOGIC_RESET_APIU(io); \
        JUNOR2_RELEASE_APIU(io); \
    } while (0)

#endif /* _JUNOR2_RESET_H_ */
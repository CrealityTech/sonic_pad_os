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
 * @file config.h
 * Config options file
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define KMD_VERSION  "1.2.28"

#define AIPU_ENABLE_RESET_HW_NONE_IDLE 0

#if ((defined BUILD_PLATFORM_JUNO) && (BUILD_PLATFORM_JUNO == 1))
#define PLATFORM_HAS_CLOCK_GATING 1
#define PLATFORM_HAS_RESET        1
#elif ((defined BUILD_PLATFORM_6CG) && (BUILD_PLATFORM_6CG == 1))
#define PLATFORM_HAS_CLOCK_GATING 0
#define PLATFORM_HAS_RESET        1
#else
#define PLATFORM_HAS_CLOCK_GATING 0
#define PLATFORM_HAS_RESET        0
#endif

#if ((defined BUILD_DEBUG_VERSION) && (BUILD_DEBUG_VERSION == 1))
#define KMD_BUILD_DEBUG_FLAG "debug"
#define AIPU_SYS_DEBUG       1
#define AIPU_ENABLE_SYSFS    1
#else
#define KMD_BUILD_DEBUG_FLAG "release"
#endif /* BUILD_DEBUG_VERSION */

#define AIPU_CONFIG_TEXT_ASID      AIPU_ASE_ID_0
#define AIPU_CONFIG_RO_STACK_ASID  AIPU_ASE_ID_0
#define AIPU_CONFIG_STATIC_ASID    AIPU_ASE_ID_1
#define AIPU_CONFIG_REUSE_ASID     AIPU_ASE_ID_2

#define AIPU_CONFIG_ENABLE_SRAM             0
#define AIPU_CONFIG_ENABLE_FALL_BACK_TO_DDR 1
#define AIPU_CONFIG_SRAM_DATA_ASID          AIPU_CONFIG_REUSE_ASID
#define AIPU_CONFIG_MM_ALLOC_FLAG           AIPU_ALLOC_FLAG_COMPACT

#endif /* _CONFIG_H_ */

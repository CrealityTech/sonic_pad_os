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
 * @file  graph_v0_0_0_3.h
 * @brief AIPU loadable graph def header
 * @version v0.0.0.3
 */

#ifndef _GRAPH_V3_H_
#define _GRAPH_V3_H_

#include <stdint.h>

#define AIPU_LOADABLE_GRAPH_V0003     3

namespace AIRT
{
/**
 * @brief struct BSS header v3
 *        stack is used by AIPU inference layers;
 *        static sections store static data like weights or bias;
 *        reuse sections store input/output/intermediate reused data;
 *        memory should be allocated for all stack & static & reuse sections;
 */
typedef struct bss_hdr_v3 {
    uint32_t stack_size;              /**< stack size */
    uint32_t stack_align_bytes;       /**< static base address align requirement (in bytes) */
    uint32_t static_section_desc_cnt; /**< number of static section descriptors */
    uint32_t reuse_section_desc_cnt;  /**< number of reuse section descriptors */
} bss_hdr_v3_t;

/**
 * @brief struct sub-section data descriptor v3
 *        sub-section data need no additional memory allocation
 */
typedef struct sub_section_desc_v3 {
    uint32_t offset_in_section_exec; /**< offset in a section */
    uint32_t size;                   /**< tensor size */
    uint32_t type;                   /**< tensor type: 0: input; 1: output; 2: saved/dump; 3: intermediate */
    uint32_t id;                     /**< tensor ID, used for identifying different inputs/outputs */
    uint32_t dtype;                  /**< tensor layout type: 0: None; 1: bool; 2: uint8; 3: int8; 4: uint16; 5: int16; */
    uint32_t layout;                 /**< tensor layout type: 0: None; 1: NHWC; 2: NCHW; */
    uint32_t shape[4];               /**< tensor shape type: N, H, W, C */
    uint32_t addr_mask;              /**< sub-section addr mask, ro_new = (addr & addr_mask) | (ro_old & ~addr_mask) */
    uint32_t offset_in_ro_cnt;       /**< number of offset(s) in rodata section where sub-section base address should be loaded */
} sub_section_desc_v3_t;

/**
 * @brief struct static data section descriptor v3
 *        a static section will contain one or more sub-sections inside
 */
typedef struct static_section_desc_v3 {
    uint32_t offset_in_file;  /**< static data offset in loadable graph file */
    uint32_t size;            /**< section size */
    uint32_t align_bytes;     /**< section base address align requirement (in bytes) */
    uint32_t sub_section_cnt; /**< number of sub-sections in this section */
} static_section_desc_v3_t;

/**
 * @brief struct reuse data section descriptor v3 (default)
 *        a section will contain one or more sub-sections inside
 */
typedef struct section_desc_v3 {
    uint32_t size;            /**< section size */
    uint32_t align_bytes;     /**< section base address align requirement (in bytes) */
    uint32_t sub_section_cnt; /**< number of sub-sections in this section */
} section_desc_default_v3_t;

/**
 * @brief struct reuse data section descriptor v3
 */
typedef section_desc_default_v3_t reuse_section_desc_v3_t;
}

#endif /* _GRAPH_V3_H_ */
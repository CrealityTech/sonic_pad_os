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
 * @file  graph_info.h
 * @brief graph information struct header
 */

#ifndef _GRAPH_INFO_H_
#define _GRAPH_INFO_H_

namespace AIRT
{
typedef enum
{
    PARAM_MAP_LOAD_TYPE_REUSE,
    PARAM_MAP_LOAD_TYPE_STATIC,
} param_map_load_type_t;

/**
 * @brief struct sub section descriptor
 *        subsection is one part of a bigger section
 */
typedef struct sub_section_desc {
    uint32_t offset_in_section;   /**< offset in a section where this subsection based in */
} sub_section_desc_t;

/**
 * @brief struct section descriptor
 */
typedef struct section_desc {
    void* load_src;               /**< section data load source (if applicable) */
    uint32_t size;                /**< section data size */
    uint32_t align_in_page;       /**< section assress alignment requirement (in page) */
    std::vector<sub_section_desc_t> sub_sections; /**< sub-section(s) in this section */
    void init()                   /**< section initializer */
    {
        load_src = nullptr;
        size = 0;
        align_in_page = 1;
        sub_sections.clear();
    }
} section_desc_t;

typedef struct io_tensor_desc {
    uint32_t id;
    uint32_t ref_section_iter;
    uint32_t offset_in_section;
    uint32_t size;
    aipu_tensor_fmt_t fmt;
} io_tensor_desc_t;

/**
 * @brief struct parameter map load descriptor
 *        This struct is used to update the rodata section of a job to calculate the
 *        base address of buffers as super parameter and save their offset in rodata section.
 */
typedef struct param_map_load_desc {
    uint32_t offset_in_map;          /**< parameter load offset in rodata parameter map */
    uint32_t load_type;              /**< data type */
    uint32_t ref_section_iter;       /**< referenced section iterator */
    uint32_t ref_sub_section_iter;
    uint32_t sub_section_offset;     /**< subsection offset in its section */
    uint32_t addr_mask;
    void init(uint32_t offset, uint32_t sec_type, uint32_t sec_iter,
        uint32_t sub_sec_iter, uint32_t sub_sec_offset, uint32_t mask)
    {
        offset_in_map = offset;
        load_type = sec_type;
        ref_section_iter = sec_iter;
        ref_sub_section_iter = sub_sec_iter;
        sub_section_offset = sub_sec_offset;
        addr_mask = mask;
    }
} param_map_load_desc_t;

typedef struct pbuf_alloc_templ {
    void*    inst_src;
    uint32_t inst_size;
    void*    data_src;
    uint32_t data_size;
    std::vector<section_desc_t> static_sections;
} pbuf_alloc_templ_t;

typedef struct tbuf_alloc_templ {
    void*    rodata_src;
    uint32_t rodata_size;
    uint32_t stack_size;
    uint32_t stack_align_bytes;
    std::vector<section_desc_t> reuse_sections;
} tbuf_alloc_templ_t;

typedef struct graph_info {
    void*    gbin;
    uint32_t gbin_size;
    uint32_t device;
    uint32_t version;
    uint32_t build_version;
    uint32_t entry;
    uint32_t arch;
    void*    bss_src;
    pbuf_alloc_templ_t pbuf_templ;
    tbuf_alloc_templ_t tbuf_templ;
    std::vector<io_tensor_desc_t> inputs;
    std::vector<io_tensor_desc_t> outputs;
    std::vector<io_tensor_desc_t> dumps;
    std::vector<param_map_load_desc_t> param_map;
} graph_info_t;
}

#endif /* _GRAPH_INFO_H_ */

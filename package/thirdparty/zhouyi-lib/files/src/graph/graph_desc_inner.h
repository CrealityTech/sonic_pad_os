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
 * @file  graph_desc_inner.h
 * @brief graph descriptor struct header (UMD internally used)
 */

#ifndef _GRAPH_DESC_INNER_H_
#define _GRAPH_DESC_INNER_H_

#include "standard_api_v2.0.0.h"

typedef struct tensor_info_inner {
    uint32_t number;
    aipu_tensor_desc_t* desc;
} aipu_tensor_info_inner_t;

typedef struct graph_desc_inner {
    uint32_t id;
    uint32_t graph_version;
    uint8_t  building_tool_major;
    uint8_t  building_tool_minor;
    uint16_t build_version;
    aipu_tensor_info_inner_t inputs;
    aipu_tensor_info_inner_t outputs;
    aipu_tensor_info_inner_t dumps;
} aipu_graph_desc_inner_t;

#endif /* _GRAPH_DESC_INNER_H_ */
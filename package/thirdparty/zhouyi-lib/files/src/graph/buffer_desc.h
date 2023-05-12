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
 * @file  buffer_desc.h
 * @brief AIPU User Mode Driver (UMD) buffer descriptor header
 */

#ifndef _BUFFER_DESC_H_
#define _BUFFER_DESC_H_

typedef uint32_t DEV_PA;
typedef uint32_t HOST_PA;

typedef struct buffer_desc {
    volatile void* va;
    HOST_PA  pa;        /**< host physical base address */
    uint32_t size;      /**< buffer size */
    uint32_t real_size; /**< real data size (<= buffer size) */
} buffer_desc_t;

typedef struct thread_buffer_info {
    uint32_t handle;
    bool is_free;
    buffer_desc_t stack;
    buffer_desc_t rodata;
    std::vector<buffer_desc_t> reuse_buf;
} tbuf_info_t;

typedef struct process_buffer_alloc_info {
    buffer_desc_t inst;
    std::vector<buffer_desc_t> static_buf;
} pbuf_info_t;

typedef struct tensor_buffer_inner {
    uint32_t number;
    aipu_buffer_t* tensors;
    HOST_PA* pa;
} aipu_tensor_buffer_inner_t;

typedef struct {
    aipu_tensor_buffer_inner_t inputs;
    aipu_tensor_buffer_inner_t outputs;
    aipu_tensor_buffer_inner_t dumps;
} iobuf_info_t;

#endif /* _BUFFER_DESC_H_ */
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
 * @file aipu_mm.h
 * Header of the AIPU memory management supports Address Space Extension (ASE)
 */

#ifndef _AIPU_MM_H_
#define _AIPU_MM_H_

#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include "aipu_session.h"
#include "uk_interface/aipu_buf_req.h"
#include "aipu_buffer.h"

struct aipu_mem_region;
struct aipu_memory_manager;
typedef int (*alloc_in_region_t)(struct aipu_memory_manager *mm, struct aipu_mem_region *region,
        struct buf_request *buf_req, struct aipu_buffer *buf);

enum aipu_blk_state {
        AIPU_BLOCK_STATE_FREE,
        AIPU_BLOCK_STATE_ALLOCATED,
};

enum aipu_asid {
        AIPU_ASE_ID_NONE = 0x0,
        AIPU_ASE_ID_0 = 0x1,
        AIPU_ASE_ID_1 = 0x2,
        AIPU_ASE_ID_2 = 0x4,
        AIPU_ASE_ID_3 = 0x8,
        AIPU_ASE_ID_ALL = 0xF,
};

enum aipu_mem_type {
        AIPU_MEM_TYPE_SRAM,
        AIPU_MEM_TYPE_CMA,
        AIPU_MEM_TYPE_RESERVED,
};

struct aipu_block {
        u64 pa;
        u64 bytes;
        int tid;
        enum aipu_mm_data_type type;
        enum aipu_blk_state state;
        struct list_head list;
};

struct aipu_mem_region {
        int id;
        struct aipu_block *blk_head;
        struct mutex lock;
        u64 pa;
        void *va;
        u64 tot_bytes;
        u64 tot_free_bytes;
        int flag;
        enum aipu_mem_type type;
        alloc_in_region_t alloc_in_region;
        struct list_head list;
};

struct aipu_memory_manager {
        struct aipu_mem_region *sram_head;
        int sram_cnt;
        struct aipu_mem_region *ddr_head;
        int ddr_cnt;
        enum aipu_asid sram_global;
        struct device *dev;
        int version;
};

/*
 * @brief initialize mm module during driver probe phase
 *
 * @param mm: memory manager struct allocated by user
 * @param dev: device struct pointer
 * @param aipu_version: AIPU version
 *
 * @return AIPU_ERRCODE_NO_ERROR if successful; others if failed.
 */
int aipu_init_mm(struct aipu_memory_manager *mm, struct device *dev, int version);
/*
 * @brief initialize mm module during driver probe phase
 *
 * @param mm: memory manager struct allocated by user
 * @param base: base physical address of this region
 * @param bytes: size of this region (in bytes)
 * @param type: AIPU memory type
 *
 * @return AIPU_ERRCODE_NO_ERROR if successful; others if failed.
 */
int aipu_mm_add_region(struct aipu_memory_manager *mm, u64 base, u64 bytes,
        enum aipu_mem_type type);
/*
 * @brief alloc memory buffer for user request
 *
 * @param mm: memory manager struct allocated by user
 * @param buf_req:  buffer request struct from userland
 * @param buf: successfully allocated buffer descriptor
 *
 * @return AIPU_ERRCODE_NO_ERROR if successful; others if failed.
 */
int aipu_mm_alloc(struct aipu_memory_manager *mm, struct buf_request *buf_req,
        struct aipu_buffer *buf);
/*
 * @brief free buffer allocated by aipu_mm_alloc
 *
 * @param mm: memory manager struct allocated by user
 * @param buf: buffer descriptor to be released
 *
 * @return AIPU_ERRCODE_NO_ERROR if successful; others if failed.
 */
int aipu_mm_free(struct aipu_memory_manager *mm, struct buf_desc *buf);
/*
 * @brief free all the allocated buffers of a session
 *
 * @param mm: mm struct pointer
 * @param session: session struct pointer
 *
 * @return AIPU_ERRCODE_NO_ERROR if successful; others if failed.
 */
int aipu_mm_free_session_buffers(struct aipu_memory_manager *mm,
        struct aipu_session *session);
/*
 * @brief de-initialize mm module while kernel module unloading
 *
 * @param mm: memory manager struct allocated by user
 *
 * @return void
 */
void aipu_deinit_mm(struct aipu_memory_manager *mm);

#endif /* _AIPU_MM_H_ */
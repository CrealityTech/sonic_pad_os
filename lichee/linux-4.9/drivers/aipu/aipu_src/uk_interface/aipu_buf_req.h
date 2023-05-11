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
 * @file aipu_buf_req.h
 * UMD & KMD interface header of buffer request
 */

#ifndef _AIPU_BUF_REQ_H_
#define _AIPU_BUF_REQ_H_

#include <linux/types.h>

enum aipu_mm_data_type {
        AIPU_MM_DATA_TYPE_NONE,
        AIPU_MM_DATA_TYPE_TEXT,
        AIPU_MM_DATA_TYPE_RO_STACK,
        AIPU_MM_DATA_TYPE_STATIC,
        AIPU_MM_DATA_TYPE_REUSE,
};

enum aipu_alloc_flag {
        AIPU_ALLOC_FLAG_DEFAULT = 0x0,
        AIPU_ALLOC_FLAG_STRICT = 0x1,
        AIPU_ALLOC_FLAG_COMPACT = 0x2,
};

struct buf_desc {
        __u64 pa;
        __u64 dev_offset; /* user space access this area via mapping this offset from the dev file start */
        __u64 bytes;
        __u32 region_id;
};

struct buf_request {
        __u64 bytes;          /* bytes requested to allocate */
        __u32 align_in_page;  /* alignment requirements (in 4KB) */
        __u32 data_type;      /* type of data in the buffer to allocate */
        __u32 region_id;      /* region ID specified (if applicable) */
        __u32 alloc_flag;     /* Allocation flag: default, strict or compact */
        struct buf_desc desc; /* info of buffer successfully allocated */
        __u32 errcode;
};

#endif /* _AIPU_BUF_REQ_H_ */
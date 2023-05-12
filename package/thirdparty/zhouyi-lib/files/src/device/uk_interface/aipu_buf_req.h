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

typedef struct buf_desc {
    __u32 phys_addr;
    __u32 dev_offset; /* user space access this area via mapping this offset from the dev file start */
    __u32 align_size;
} buf_desc_t;

typedef struct buf_request {
    __u32      size;
    __u32      align_in_page;
    buf_desc_t desc;
    __u32      errcode;
} buf_request_t;

#endif //_AIPU_BUF_REQ_H_
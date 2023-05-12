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
  * @file aipu_io_req.h
  * UMD & KMD interface header of read/write IO
  */

#ifndef _AIPU_RW_IO_H_
#define _AIPU_RW_IO_H_

#include <linux/types.h>

typedef enum {
    AIPU_IO_READ,
    AIPU_IO_WRITE
} aipu_rw_attr;

typedef struct aipu_io_req {
    __u32 offset;
    aipu_rw_attr rw;
    __u32 value;
    __u32 errcode;
} aipu_io_req_t;

#endif //_AIPU_RW_IO_H_
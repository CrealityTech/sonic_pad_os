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
 * @file aipu_job_desc.h
 * UMD & KMD interface header of AIPU job descriptor
 */

#ifndef _AIPU_JOB_DESC_H_
#define _AIPU_JOB_DESC_H_

#include <linux/types.h>

#ifdef __KERNEL__

#define AIPU_JOB_STATE_IDLE      0
#define AIPU_JOB_STATE_PENDING   1
#define AIPU_JOB_STATE_SCHED     2
#define AIPU_JOB_STATE_END       3

#define AIPU_JOB_FLAG_INVALID    0
#define AIPU_JOB_FLAG_VALID      1
#endif

struct user_job_desc {
        __u64 start_pc_addr;
        __u64 intr_handler_addr;
        __u64 data_0_addr;
        __u64 data_1_addr;
        __u64 static_addr;
        __u64 reuse_addr;
        __u32 job_id;
        __u32 code_size;
        __u32 rodata_size;
        __u32 stack_size;
        __u32 static_size;
        __u32 reuse_size;
        __u32 enable_prof;
        __u32 enable_asid;
};

struct user_job {
        struct user_job_desc desc;
        __u32 errcode;
};

#endif /* _AIPU_JOB_DESC_H_ */
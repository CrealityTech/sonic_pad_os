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
#include "../aipu_private.h"

#define AIPU_JOB_STATE_IDLE      0
#define AIPU_JOB_STATE_PENDING   1
#define AIPU_JOB_STATE_SCHED     2
#define AIPU_JOB_STATE_END       3

#define AIPU_JOB_FLAG_INVALID    0
#define AIPU_JOB_FLAG_VALID      1
#else
#include "standard_api_v2.0.0.h"
#endif

typedef struct user_job_desc {
    __u32 start_pc_addr;
    __u32 intr_handler_addr;
    __u32 data_0_addr;
    __u32 data_1_addr;
    __u32 job_id;
    __u32 enable_prof;
} user_job_desc_t;

typedef struct user_job {
    user_job_desc_t desc;
    __u32           errcode;
} user_job_t;

#ifdef __KERNEL__
/*
 * @brief init a job descriptor
 *
 * @param job_desc: job_desc to be initialized
 * @param desc: desc used as initial value
 *
 * @return AIPU_ERRCODE_NO_ERROR if successful; others if failed;
 */
int init_job_desc(user_job_desc_t *job_desc, _aipu_const_ user_job_desc_t *desc);
/*
 * @brief return if a job is specified to be profiled or not
 *
 * @param job_desc: job_desc fro userland
 *
 * @return true/false
 */
int is_job_profiling_enable(user_job_desc_t *job_desc);
#endif

#endif //_AIPU_JOB_DESC_H_

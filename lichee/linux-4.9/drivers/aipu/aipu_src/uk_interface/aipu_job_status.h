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
 * @file aipu_job_status.h
 * UMD & KMD interface header of job status querying
 */

#ifndef _AIPU_JOB_STATUS_H_
#define _AIPU_JOB_STATUS_H_

#include <linux/types.h>
#include "aipu_profiling.h"

#define AIPU_JOB_STATE_DONE      0x1
#define AIPU_JOB_STATE_EXCEPTION 0x2

struct job_status_desc {
        __u32 job_id;
        __u32 thread_id;
        __u32 state;
        struct profiling_data pdata;
};

struct job_status_query {
        __u32 max_cnt;
        __u32 get_single_job;
        __u32 job_id;
        struct job_status_desc *status;
        __u32 poll_cnt;
        __u32 errcode;
};

#endif /* _AIPU_JOB_STATUS_H_ */
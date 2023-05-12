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
 * @file  job_desc.h
 * @brief AIPU User Mode Driver (UMD) job descriptor module header
 */

#ifndef _JOB_DESC_H_
#define _JOB_DESC_H_

#include <string>
#include <sys/time.h>
#include "standard_api_v2.0.0.h"
#include "buffer_desc.h"

typedef enum {
    JOB_STATE_NO_STATE = 0,
    JOB_STATE_DONE,
    JOB_STATE_EXCEPTION,
    JOB_STATE_BUILT,
    JOB_STATE_SCHED,
} job_state_t;

typedef struct dev_config {
    uint32_t arch;
    uint32_t hw_version;
    uint32_t hw_config;
    aipu_debug_info_t code;
    DEV_PA rodata_base;
    DEV_PA stack_base;
} dev_config_t;

typedef struct job_desc {
    uint32_t id;
    uint32_t buf_handle;
    job_state_t state;
    dev_config_t config;
    uint32_t dump_options;
    std::string dump_dir;
    struct timeval sched_time;
} job_desc_t;

#endif /* _JOB_DESC_H_ */
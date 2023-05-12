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
 * @file aipu_exception.h
 * UMD & KMD interface header of detailed job exception info.
 */

#ifndef _AIPU_EXCEPTION_H_
#define _AIPU_EXCEPTION_H_

#include <linux/types.h>

typedef enum aipu_exception_type {
    AIPU_EXCEP_NO_EXCEPTION=0,
    AIPU_EXCEP_ILLEGAL_INST=1,
    AIPU_EXCEP_FETCH_EXCEPTION=2,
    AIPU_EXCEP_IPI=3,
    AIPU_EXCEP_CORE_FAULT=32,
    AIPU_EXCEP_TPC_FAULT=33,
    AIPU_EXCEP_HWA_INT=34,
    AIPU_EXCEP_DMA_INT=35
} aipu_exception_type_t;

typedef struct aipu_exception {
    __u32 job_id;
    aipu_exception_type_t type;
    __u32 errcode;
} aipu_exception_t;

#endif //_AIPU_EXCEPTION_H_

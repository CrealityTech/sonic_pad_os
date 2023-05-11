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
 * @file aipu_errcode_map.c
 * KMD errcode mapping before return to userland
 */

#include <linux/errno.h>
#include "uk_interface/aipu_errcode.h"

int map_errcode(int err)
{
        switch (err) {
        case AIPU_ERRCODE_NO_ERROR:
                return 0;
        case AIPU_ERRCODE_INVALID_INT_STAT:
        case AIPU_ERRCODE_IOREMAP_FAIL:
        case AIPU_ERRCODE_JOB_PROF_DISABLED:
        case AIPU_ERRCODE_CREATE_KOBJ_ERR:
        case AIPU_ERRCODE_PLAT_GET_RES_FAIL:
                return -EFAULT;
        case AIPU_ERRCODE_INVALID_OPS:
        case AIPU_ERRCODE_JOB_NOT_END:
                return -ENOTTY;
        case AIPU_ERRCODE_INTERNAL_NULLPTR:
        case AIPU_ERRCODE_INVALID_ARGS:
                return -EINVAL;
        case AIPU_ERRCODE_NO_MEMORY:
        case AIPU_ERRCODE_NO_ALIGN_MEM:
                return -ENOMEM;
        case AIPU_ERRCODE_JOB_ID_NOT_FOUND:
        case AIPU_ERRCODE_AIPU_NOT_FOUND:
        case AIPU_ERRCODE_ITEM_NOT_FOUND:
        case AIPU_ERRCODE_JOB_DESC_UNMATCHED:
                return -ENOENT;
        default:
                return -EFAULT;
        }
}
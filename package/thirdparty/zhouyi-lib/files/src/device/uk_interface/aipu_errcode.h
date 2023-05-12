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
 * @file aipu_errcode.h
 * UMD & KMD interface header of error code for UMD to check
 */

#ifndef _AIPU_ERR_CODE_H_
#define _AIPU_ERR_CODE_H_

#define AIPU_ERRCODE_NO_ERROR            0
#define AIPU_ERRCODE_JOB_ID_NOT_FOUND    1
#define AIPU_ERRCODE_JOB_NOT_END         2
#define AIPU_ERRCODE_JOB_PROF_DISABLED   3
#define AIPU_ERRCODE_AIPU_NOT_FOUND      4
#define AIPU_ERRCODE_JOB_DESC_UNMATCHED  5
#define AIPU_ERRCODE_NO_MEMORY           6
#define AIPU_ERRCODE_NO_ALIGN_MEM        7
#define AIPU_ERRCODE_INTERNAL_NULLPTR    8
#define AIPU_ERRCODE_PLAT_GET_RES_FAIL   9
#define AIPU_ERRCODE_CREATE_KOBJ_ERR     10
#define AIPU_ERRCODE_INVALID_OPS         11
#define AIPU_ERRCODE_INVALID_ARGS        12
#define AIPU_ERRCODE_IOREMAP_FAIL        13
#define AIPU_ERRCODE_INVALID_INT_STAT    14
#define AIPU_ERRCODE_ITEM_NOT_FOUND      15

#ifdef __KERNEL__
/*
 * @brief map KMD internal errcode into standard kernel errcode
 *
 * @param errcode: KMD internal errcode
 *
 * @return kernel standard error code
 */
int map_errcode(int errcode);
#endif //__KERNEL__

#endif
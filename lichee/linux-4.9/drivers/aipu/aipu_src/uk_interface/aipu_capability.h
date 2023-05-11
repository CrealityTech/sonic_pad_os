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
 * @file aipu_capability.h
 * UMD & KMD interface header of AIPU hardware capability
 */

#ifndef _AIPU_CAPABILITY_H_
#define _AIPU_CAPABILITY_H_

#include <linux/types.h>

struct aipu_cap {
        __u32 isa_version;
        __u32 tpc_feature;
        __u32 aiff_feature;
        __u32 errcode;
};

#endif /* _AIPU_CAPABILITY_H_ */
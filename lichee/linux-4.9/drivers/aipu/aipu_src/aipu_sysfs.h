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
 * @file aipu_sysfs.h
 * sysfs interface header file
 */

#ifndef _AIPU_SYSFS_H_
#define _AIPU_SYSFS_H_

#include <linux/sysfs.h>
#include "aipu_job_manager.h"
#include "uk_interface/aipu_io_req.h"
#include "config.h"

#define MAX_CHAR_SYSFS 4096

/*
 * @brief initialize sysfs debug interfaces in probe
 *
 * @param aipu_priv: aipu_priv struct pointer
 *
 * @return 0 if successful; others if failed.
 */
int aipu_create_sysfs(void *aipu_priv);
/*
 * @brief de-initialize sysfs debug interfaces in remove
 *
 * @param aipu_priv: aipu_priv struct pointer
 */
void aipu_destroy_sysfs(void *aipu_priv);

#endif /* _AIPU_SYSFS_H_ */
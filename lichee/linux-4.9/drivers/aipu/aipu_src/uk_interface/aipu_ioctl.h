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
 * @file aipu_ioctl.h
 * UMD & KMD interface header of ioctl macro
 */

#ifndef _AIPU_IOCTL_H_
#define _AIPU_IOCTL_H_

#include <linux/ioctl.h>
#include <linux/types.h>

#define IPUIOC_MAGIC 'A'

#define IPUIOC_QUERYCAP          _IOR(IPUIOC_MAGIC,  0, struct aipu_cap)
#define IPUIOC_REQBUF            _IOWR(IPUIOC_MAGIC, 1, struct buf_request)
#define IPUIOC_RUNJOB            _IOWR(IPUIOC_MAGIC, 2, struct user_job)
#define IPUIOC_FREEBUF           _IOW(IPUIOC_MAGIC,  3, struct buf_desc)
#define IPUIOC_REQSHMMAP         _IOR(IPUIOC_MAGIC,  4, __u64)
#define IPUIOC_REQIO             _IOWR(IPUIOC_MAGIC, 5, struct aipu_io_req)
#define IPUIOC_QUERYSTATUS       _IOWR(IPUIOC_MAGIC, 6, struct job_status_query)
#define IPUIOC_KILL_TIMEOUT_JOB  _IOW(IPUIOC_MAGIC,  7, __u32)

#endif /* _AIPU_IOCTL_H_ */
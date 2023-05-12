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

#define IPUIOC_MAGIC 'A'

#define IPUIOC_CLEAR             _IO(IPUIOC_MAGIC,   0)
#define IPUIOC_QUERYCAP          _IOR(IPUIOC_MAGIC,  1, aipu_cap_t)
#define IPUIOC_REQBUF            _IOWR(IPUIOC_MAGIC, 2, buf_request_t)
#define IPUIOC_RUNJOB            _IOWR(IPUIOC_MAGIC, 3, user_job_t)
#define IPUIOC_QUERYERR          _IOWR(IPUIOC_MAGIC, 4, aipu_exception_t)
#define IPUIOC_FREEBUF           _IOW(IPUIOC_MAGIC,  5, buf_desc_t)
#define IPUIOC_QUERYPDATA        _IOWR(IPUIOC_MAGIC, 6, req_pdata_t)
#define IPUIOC_REQSHMMAP         _IOR(IPUIOC_MAGIC,  7, int)
#define IPUIOC_REQIO             _IOWR(IPUIOC_MAGIC, 8, aipu_io_req_t)

#endif //_AIPU_IOCTL_H_
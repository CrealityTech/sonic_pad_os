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
#ifndef _AIPU_THREAD_STATUS_H_
#define _AIPU_THREAD_STATUS_H_

typedef enum
{
    AIPU_THREAD_STATUS_NO_STATUS=0,
    AIPU_THREAD_STATUS_DONE=1,
    AIPU_THREAD_STATUS_EXCEPTION=2,
    AIPU_THREAD_STATUS_BUILT=3,
    AIPU_THREAD_STATUS_SCHEDULED=4,
} thread_status_t;

#endif //_AIPU_THREAD_STATUS_H_

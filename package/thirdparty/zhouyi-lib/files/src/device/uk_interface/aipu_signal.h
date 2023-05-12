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
 * @file aipu_signal.h
 * UMD & KMD interface header of signal info.
 */

#ifndef _AIPU_SIGNAL_H_
#define _AIPU_SIGNAL_H_

#ifdef __KERNEL__
#define AIPU_SIG_DONE      (SIGRTMIN+3+2)
#define AIPU_SIG_EXCEPTION (SIGRTMIN+4+2)
#else
#define AIPU_SIG_DONE      (SIGRTMIN+3)
#define AIPU_SIG_EXCEPTION (SIGRTMIN+4)
#endif //__KERNEL__

#endif //_AIPU_SIGNAL_H_
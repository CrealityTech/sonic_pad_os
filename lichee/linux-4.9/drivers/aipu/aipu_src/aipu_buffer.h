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
 * @file aipu_buffer.h
 * Header file of AIPU memory buffer module
 */

#ifndef _AIPU_BUFFER_H_
#define _AIPU_BUFFER_H_

/**
 * struct aipu_buffer: buffer info descriptor struct
 * @phys_addr: physical address start point of this buffer
 * @kern_addr: kernel virtual address start point
 * @size: buffer size
 */
struct aipu_buffer {
        u64 pa;
        void *va;
        u64 bytes;
        u32 region_id;
        u32 type;
};

#endif /* _AIPU_BUFFER_H_ */
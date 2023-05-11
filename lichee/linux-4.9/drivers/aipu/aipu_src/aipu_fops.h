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
 * @file aipu_fops.h
 * KMD file operation API register func header
 */

#ifndef _AIPU_FOPS_H_
#define _AIPU_FOPS_H_

#include <linux/fs.h>

/*
 * @brief register AIPU fops operations into fops struct
 *
 * @param fops: file_operations struct pointer
 *
 * @return AIPU_ERRCODE_NO_ERROR if successful; others if failed.
 */
int aipu_fops_register(struct file_operations *fops);

#endif /* _AIPU_FOPS_H_ */

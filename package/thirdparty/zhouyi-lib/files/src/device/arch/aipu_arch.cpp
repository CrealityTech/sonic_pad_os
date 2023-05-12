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

#include "aipu_arch.h"

aipu_arch_t arch[AIPU_ARCH_CONFIG_MAX] = {
    {"ZHOUYI_Z1_SC000", 32, 32, 2, 128, 2, 9, "True", "True", 32, 256, 4}, /* ZHOUYI_Z1_SC000 */
    {"ZHOUYI_Z1_SC002", 32, 32, 2, 128, 2, 9, "True", "True", 32, 512, 2}, /* ZHOUYI_Z1_SC002 */
    {"ZHOUYI_Z1_SC003", 16, 32, 2, 1, 1, 3, "False", "False", 16, 64, 1},  /* ZHOUYI_Z1_SC003 */
};
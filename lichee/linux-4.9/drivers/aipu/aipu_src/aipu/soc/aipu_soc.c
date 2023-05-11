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
 * @file aipu_soc.c
 * Implementation of the aipu_soc create & destroy
 */

#include <linux/slab.h>
#include "aipu_soc.h"

int create_aipu_soc(int brand, u64 soc_base0, u64 base0_size, struct device *dev,
        struct aipu_soc **p_soc)
{
        int ret = 0;
        struct aipu_soc *soc = NULL;

        if (!p_soc)
                return -EINVAL;

        *p_soc = NULL;

        soc = devm_kzalloc(dev, sizeof(struct aipu_soc), GFP_KERNEL);
        if (!soc)
                return -ENOMEM;

        if (base0_size) {
                soc->base0 = aipu_create_ioregion(dev, soc_base0, base0_size);
                if (!soc->base0)
                        return -EFAULT;
        }
        else
                soc->base0 = NULL;

        soc->brand = brand;

        /* success */
        *p_soc = soc;
        return ret;
}

void destroy_aipu_soc(struct aipu_soc *soc)
{
        if (soc) {
                if (soc->base0)
                        aipu_destroy_ioregion(soc->base0);
        }
}
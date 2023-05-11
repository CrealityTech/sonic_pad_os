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
 * @file aipu_core.c
 * Implementation of the aipu_core create & destroy
 */

#include <linux/slab.h>
#include "aipu_core.h"

int create_aipu_core(int version, int irqnum, u64 aipu_base0, u64 base0_size,
        u32 freq, aipu_irq_uhandler_t uhandler, aipu_irq_bhandler_t bhandler,
        void *aipu_priv, struct device *dev, struct aipu_core **p_core)
{
        int ret = 0;
        struct aipu_core *core = NULL;

        if ((!base0_size) || (!aipu_priv) || (!dev) || (!p_core)) {
                if (dev)
                        dev_err(dev, "invalid input args base0_size/aipu_priv/p_core to be NULL\n");
                return -EINVAL;
        }

        core = devm_kzalloc(dev, sizeof(struct aipu_core), GFP_KERNEL);
        if (!core) {
                dev_err(dev, "no memory in allocating aipu_core struct\n");
                return -ENOMEM;
        }

        core->base0 = NULL;
        core->irq_obj = NULL;

        /* init core */
        if (version == AIPU_VERSION_ZHOUYI_V1)
                core->max_sched_num = ZHOUYI_V1_MAX_SCHED_JOB_NUM;
        else if (version == AIPU_VERSION_ZHOUYI_V2)
                core->max_sched_num = ZHOUYI_V2_MAX_SCHED_JOB_NUM;
        else {
                dev_err(dev, "invalid hardware version %d KMD cannot recognized!\n", version);
                return -EINVAL;
        }
        core->version = version;

        core->base0 = aipu_create_ioregion(dev, aipu_base0, base0_size);
        if (!core->base0) {
                dev_err(dev, "create IO region for core0 failed: base 0x%llx, size 0x%llx\n",
                        aipu_base0, base0_size);
                return -EFAULT;
        }

        core->irq_obj = aipu_create_irq_object(irqnum, uhandler, bhandler,
                aipu_priv, dev, "aipu");
        if (!core->irq_obj) {
                dev_err(dev, "create IRQ object for core0 failed: IRQ 0x%x\n", irqnum);
                return -EFAULT;
        }

        core->freq_in_MHz = freq;
        core->dev = dev;

        /* success */
        *p_core = core;
        return ret;
}

void destroy_aipu_core(struct aipu_core *core)
{
        if (core) {
                if (core->base0)
                        aipu_destroy_ioregion(core->base0);
                if (core->irq_obj)
                        aipu_destroy_irq_object(core->irq_obj);
        }
}
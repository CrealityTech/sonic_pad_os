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
 * @file aipu_irq.c
 * Implementation of the interrupt request and handlers' abstraction
 */

#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include "uk_interface/aipu_errcode.h"
#include "aipu_irq.h"
#include "aipu.h"

struct workqueue_struct *aipu_wq = NULL;

static irqreturn_t aipu_irq_handler_upper_half(int irq, void *dev_id)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        struct aipu_irq_object *irq_obj = NULL;
        struct aipu_priv *aipu = NULL;

        if (!dev_id) {
                goto error;
        }
        else {
                aipu = (struct aipu_priv *)(((struct device *)dev_id)->driver_data);
                irq_obj = aipu->core0->irq_obj;
                ret = irq_obj->uhandler(aipu);
                if (AIPU_ERRCODE_NO_ERROR != ret) {
                        goto error;
                }
                else {
                        /* success */
                        goto finish;
                }
        }

error:
        return IRQ_NONE;

finish:
        return IRQ_HANDLED;
}

static void aipu_irq_handler_bottom_half(struct work_struct *work)
{
        struct aipu_irq_object *irq_obj = NULL;

        if (work) {
                irq_obj = container_of(work, struct aipu_irq_object, work);
                irq_obj->bhandler(irq_obj->aipu_priv);
        }
}

struct aipu_irq_object *aipu_create_irq_object(u32 irqnum, aipu_irq_uhandler_t uhandler, aipu_irq_bhandler_t bhandler,
    void *aipu_priv, struct device *dev, char *description)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        struct aipu_irq_object *irq_obj = NULL;

        if ((!aipu_priv) || (!dev) || (!description)) {
                ret = AIPU_ERRCODE_INTERNAL_NULLPTR;
                goto finish;
        }

        irq_obj = kzalloc(sizeof(struct aipu_irq_object), GFP_KERNEL);
        if (!irq_obj)
                goto finish;

        irq_obj->aipu_wq = NULL;
        irq_obj->irqnum = 0;
        irq_obj->dev = dev;

        irq_obj->aipu_wq = create_singlethread_workqueue("aipu");
        if (!irq_obj->aipu_wq)
                goto err_handle;

        INIT_WORK(&irq_obj->work, aipu_irq_handler_bottom_half);

        ret = request_irq(irqnum, aipu_irq_handler_upper_half, IRQF_SHARED | IRQF_TRIGGER_RISING, description, dev);
        if (ret) {
                dev_err(dev, "request IRQ (num %u) failed! (errno = %d)", irqnum, ret);
                goto err_handle;
        }

        irq_obj->irqnum = irqnum;
        irq_obj->uhandler = uhandler;
        irq_obj->bhandler = bhandler;
        irq_obj->aipu_priv = aipu_priv;

        /* success */
        goto finish;

err_handle:
        aipu_destroy_irq_object(irq_obj);
        irq_obj = NULL;

finish:
        return irq_obj;
}

void aipu_irq_schedulework(struct aipu_irq_object *irq_obj)
{
        if (irq_obj)
                queue_work(irq_obj->aipu_wq, &irq_obj->work);
}

void aipu_irq_flush_workqueue(struct aipu_irq_object *irq_obj)
{
        /* only one workqueue currently */
        flush_workqueue(irq_obj->aipu_wq);
}

void aipu_destroy_irq_object(struct aipu_irq_object *irq_obj)
{
        if (irq_obj) {
                if (irq_obj->aipu_wq) {
                        flush_workqueue(irq_obj->aipu_wq);
                        destroy_workqueue(irq_obj->aipu_wq);
                        irq_obj->aipu_wq = NULL;
                }
                if (irq_obj->irqnum)
                        free_irq(irq_obj->irqnum, irq_obj->dev);
                kfree(irq_obj);
                flush_scheduled_work();
        }
}
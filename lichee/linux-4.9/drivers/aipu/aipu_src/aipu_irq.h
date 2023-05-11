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
 * @file aipu_irq.h
 * Header of the interrupt request and handlers' abstraction
 */

#ifndef _AIPU_IRQ_H_
#define _AIPU_IRQ_H_

#include <linux/device.h>
#include <linux/workqueue.h>

typedef int  (*aipu_irq_uhandler_t) (void *arg);
typedef void (*aipu_irq_bhandler_t) (void *arg);
typedef void (*aipu_irq_trigger_t) (void *arg);
typedef void (*aipu_irq_ack_t) (void *arg);

/**
 * struct aipu_irq_object - IRQ instance for each hw module in AIPU with interrupt function
 *
 * @irqnum: interrupt number used to request IRQ
 * @aipu_priv: aipu_priv struct pointer
 * @uhandler: real upper-half handler
 * @bhandler: real bottom-half handler
 * @work: work struct
 * @dev: device pointer
 * @aipu_wq: workqueue struct pointer
 */
struct aipu_irq_object {
        u32 irqnum;
        void *aipu_priv;
        aipu_irq_uhandler_t uhandler;
        aipu_irq_bhandler_t bhandler;
        struct work_struct  work;
        struct device *dev;
        struct workqueue_struct *aipu_wq;
};

/**
 * @brief initialize an AIPU IRQ object for a HW module with interrupt function
 *
 * @param irqnum: interrupt number
 * @param uhandler: upper-half handler
 * @param bhandler: bottom-half handler
 * @aipu_priv: aipu_priv struct pointer
 * @param description: irq object description string
 *
 * @return irq_object pointer if successful; NULL if failed;
 */
struct aipu_irq_object *aipu_create_irq_object(u32 irqnum, aipu_irq_uhandler_t uhandler,
        aipu_irq_bhandler_t bhandler, void *aipu_priv, struct device *dev, char *description);
/**
 * @brief workqueue schedule API
 *
 * @param irq_obj: interrupt object
 *
 * @return void
 */
void aipu_irq_schedulework(struct aipu_irq_object *irq_obj);
/**
 * @brief workqueue flush API
 *
 * @param irq_obj: interrupt object
 *
 * @return void
 */
void aipu_irq_flush_workqueue(struct aipu_irq_object *irq_obj);
/**
 * @brief workqueue terminate API
 *
 * @param irq_obj: interrupt object
 *
 * @return void
 */
void aipu_destroy_irq_object(struct aipu_irq_object *irq_obj);

#endif //_AIPU_IRQ_H_
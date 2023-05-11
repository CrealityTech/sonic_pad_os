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
 * @file aipu_core.h
 * Header of the aipu_core struct
 */

#ifndef _AIPU_CORE_H_
#define _AIPU_CORE_H_

#include <linux/device.h>
#include "uk_interface/aipu_capability.h"
#include "uk_interface/aipu_job_desc.h"
#include "uk_interface/aipu_io_req.h"
#include "aipu_irq.h"
#include "aipu_io.h"
#include "zhouyi/z1/z1.h"
#include "zhouyi/z2/z2.h"

enum aipu_version {
        AIPU_VERSION_ZHOUYI_V1 = 1,
        AIPU_VERSION_ZHOUYI_V2
};

/**
 * struct aipu_core - a general struct describe a hardware AIPU core
 *
 * @version: AIPU hardware version
 * @freq_in_MHz: AIPU core working frequency
 * @max_sched_num: maximum number of jobs can be scheduled in pipeline
 * @base0: IO region of this AIPU core
 * @irq_obj: interrupt object of this core
 */
struct aipu_core {
        int version;
        int freq_in_MHz;
        int max_sched_num;
        struct io_region *base0;
        struct aipu_irq_object *irq_obj;
        struct device *dev;
};

/**
 * struct aipu_io_operation - a struct contains AIPU hardware operation methods
 *
 * @enable_interrupt: Enable all AIPU interrupts
 * @disable_interrupt: Disable all AIPU interrupts
 * @trigger: trigger AIPU to run a job
 * @is_idle: Is AIPU hardware idle or not
 * @read_status_reg: Read status register value
 * @print_hw_id_info: Print AIPU version ID registers information
 * @query_capability: Query AIPU hardware capability information
 * @io_rw: Direct IO read/write operations
 */
struct aipu_io_operation {
        void (*enable_interrupt)(struct aipu_core* core);
        void (*disable_interrupt)(struct aipu_core* core);
        int  (*trigger)(struct aipu_core* core, struct user_job_desc* udesc, int tid);
        bool (*is_idle)(struct aipu_core* core);
        int  (*read_status_reg)(struct aipu_core* core);
        void (*print_hw_id_info)(struct aipu_core* core);
        int  (*query_capability)(struct aipu_core* core, struct aipu_cap* cap);
        void (*io_rw)(struct aipu_core* core, struct aipu_io_req* io_req);
        int  (*upper_half)(void* data);
        void (*bottom_half)(void* data);
};

/**
 * @brief create an AIPU core struct in driver probe phase
 *
 * @param version: AIPU hardware version
 * @param irqnum: interrupt number
 * @param aipu_base0: AIPU core IO base 0 address
 * @param base0_size: AIPU core IO base 0 size
 * @param freq: AIPU core working frequency
 * @param dev: device struct
 *
 * @return 0 if successful; others if failed;
 */
int create_aipu_core(int version, int irqnum, u64 aipu_base0, u64 base0_size,
        u32 freq, aipu_irq_uhandler_t uhandler, aipu_irq_bhandler_t bhandler,
        void *aipu_priv, struct device *dev, struct aipu_core **p_core);
/**
 * @brief destroy a created aipu_core struct
 *
 * @param core: AIPU hardware core created by create_aipu_core
 */
void destroy_aipu_core(struct aipu_core *core);

#endif /* _AIPU_CORE_H_ */
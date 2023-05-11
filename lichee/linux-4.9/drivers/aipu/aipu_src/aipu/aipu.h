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
 * @file aipu.h
 * Header of the aipu device struct
 */

#ifndef _AIPU_H_
#define _AIPU_H_

#include <linux/device.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/mutex.h>
#include <linux/types.h>
#include "aipu_irq.h"
#include "aipu_io.h"
#include "aipu_core.h"
#include "soc/aipu_soc.h"
#include "aipu_job_manager.h"
#include "aipu_mm.h"
#include "aipu_sysfs.h"
#include "aipu_sunxi_platform.h"

struct aipu_priv {
        int board;
        int version;
        struct aipu_core *core0;
        struct aipu_soc *soc;
        struct aipu_io_operation* core_ctrl;
        struct soc_io_operation*  soc_ctrl;
        int   open_num;
        u64   host_to_aipu_map;
        struct device *dev;
        struct file_operations aipu_fops;
        struct miscdevice *misc;
        struct mutex lock;
        struct aipu_job_manager job_manager;
        struct aipu_memory_manager mm;
        struct kobject *sys_kobj;
        int is_suspend;
        int is_reset;
};

/**
 * @brief initialize an input AIPU private data struct
 *
 * @param aipu: pointer to AIPU private data struct
 * @param dev: device struct pointer
 *
 * @return 0 if successful; others if failed;
 */
int init_aipu_priv(struct aipu_priv *aipu, struct device *dev);
/**
 * @brief initialize AIPU core info in the AIPU private data struct
 *
 * @param aipu: pointer to AIPU private data struct
 * @param irqnum: AIPU interrupt number
 * @param base: AIPU external registers phsical base address
 * @param size: AIPU external registers address remap size
 *
 * @return 0 if successful; others if failed;
 */
int aipu_priv_init_core(struct aipu_priv *aipu, int irqnum, u64 base, u64 size);
/**
 * @brief initialize the SoC info in the AIPU private data struct
 *
 * @param aipu: pointer to AIPU private data struct
 * @param base: SoC registers phsical base address
 * @param size: SoC external registers address remap size
 *
 * @return 0 if successful; others if failed;
 */
int aipu_priv_init_soc(struct aipu_priv *aipu, u64 base, u64 size);
/**
 * @brief add a reserved memory region into the AIPU private data struct
 *
 * @param aipu: pointer to AIPU private data struct
 * @param base: memory region start physical address
 * @param size: memory region length size
 * @param type: AIPU memory type
 *
 * @return 0 if successful; others if failed;
 */
int aipu_priv_add_mem_region(struct aipu_priv *aipu, u64 base, u64 size,
        enum aipu_mem_type type);
/**
 * @brief get AIPU hardware version number wrapper
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return version
 */
int aipu_priv_get_version(struct aipu_priv *aipu);
/**
 * @brief enable interrupt wrapper
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return void
 */
void aipu_priv_enable_interrupt(struct aipu_priv *aipu);
/**
 * @brief disable interrupt wrapper
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return void
 */
void aipu_priv_disable_interrupt(struct aipu_priv *aipu);
/**
 * @brief disable interrupt wrapper
 *
 * @param aipu:  pointer to AIPU private data struct
 * @param udesc: descriptor of a job to be triggered on AIPU
 * @param tid:   user thread ID
 *
 * @return 0 if successful; others if failed;
 */
int aipu_priv_trigger(struct aipu_priv *aipu, struct user_job_desc *udesc, int tid);
/**
 * @brief check if AIPU is idle wrapper
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return 1 if AIPU is in IDLE state
 */
bool aipu_priv_is_idle(struct aipu_priv *aipu);
/**
 * @brief query AIPU capability wrapper
 *
 * @param aipu: pointer to AIPU private data struct
 * @param cap:  pointer to the capability struct
 *
 * @return 0 if successful; others if failed;
 */
int aipu_priv_query_capability(struct aipu_priv *aipu, struct aipu_cap *cap);
/**
 * @brief AIPU external register read/write wrapper
 *
 * @param aipu: pointer to AIPU private data struct
 * @param io_req:  pointer to the io_req struct
 *
 * @return void
 */
void aipu_priv_io_rw(struct aipu_priv *aipu, struct aipu_io_req *io_req);
/**
 * @brief print AIPU hardware ID information wrapper
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return void
 */
void aipu_priv_print_hw_id_info(struct aipu_priv *aipu);
/**
 * @brief start AIPU bandwidth profiling wrapper
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return void
 */
void aipu_priv_start_bw_profiling(struct aipu_priv *aipu);
/**
 * @brief stop AIPU bandwidth profiling wrapper
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return void
 */
void aipu_priv_stop_bw_profiling(struct aipu_priv *aipu);
/**
 * @brief reading AIPU SoC profiling data wrapper
 *
 * @param aipu:  pointer to AIPU private data struct
 * @param pdata: pointer to prof data structure
 *
 * @return void
 */
void aipu_priv_read_profiling_reg(struct aipu_priv *aipu, struct profiling_data *pdata);
/**
 * @brief check if AIPU has logic reset function wrapper;
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return true if corresponding platform has reset function
 */
bool aipu_priv_has_logic_reset(struct aipu_priv *aipu);
/**
 * @brief print AIPU logic reset wrapper;
 *        after this call returns AIPU is in reset state and cannot be accessed
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return void
 */
void aipu_priv_logic_reset(struct aipu_priv *aipu);
/**
 * @brief print AIPU logic release wrapper;
 *        after this call returns AIPU can work normally
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return void
 */
void aipu_priv_logic_release(struct aipu_priv *aipu);
/**
 * @brief print AIPU logic reset wrapper;
 *        after this call returns the AIPU is reset to idle state
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return void
 */
void aipu_priv_logic_reset_release(struct aipu_priv *aipu);
/**
 * @brief enable AIPU SoC clock gating wrapper;
 *        after this call returns the AIPU is clock-gated and cannot be accessed;
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return void
 */
void aipu_priv_enable_clk_gating(struct aipu_priv *aipu);
/**
 * @brief disable AIPU SoC clock gating wrapper;
 *        after this call returns the AIPU the clock-gating operation is removed;
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return void
 */
void aipu_priv_disable_clk_gating(struct aipu_priv *aipu);
/**
 * @brief deinit an AIPU private data struct
 *
 * @param aipu: pointer to AIPU private data struct
 *
 * @return 0 if successful; others if failed;
 */
int deinit_aipu_priv(struct aipu_priv *aipu);

#endif /* _AIPU_H_ */
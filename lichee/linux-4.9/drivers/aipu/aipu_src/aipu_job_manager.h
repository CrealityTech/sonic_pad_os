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
 * @file aipu_job_manager.h
 * Job manager module header file
 */

#ifndef _AIPU_JOB_MANAGER_H_
#define _AIPU_JOB_MANAGER_H_

#include <linux/list.h>
#include <linux/spinlock.h>
#include "uk_interface/aipu_job_desc.h"
#include "aipu_session.h"
#include "aipu_thread_waitqueue.h"

#define AIPU_EXCEP_NO_EXCEPTION   0

/**
 * struct aipu_job - job element struct describing a job under scheduling in job manager
 *        Job status will be tracked as soon as interrupt or user evenets come in.
 *
 * @uthread_id: ID of user thread scheduled this job
 * @desc: job desctiptor from userland
 * @session: session pointer refernece of this job
 * @session_job: corresponding job object in session
 * @state: job state
 * @exception_flag: exception flag
 * @valid_flag: valid flag, indicating this job canceled by user or not
 * @node: list head struct
 */
 struct aipu_job {
        int uthread_id;
        struct user_job_desc desc;
        struct aipu_session *session;
        struct session_job *session_job;
        int state;
        int exception_flag;
        int valid_flag;
        struct list_head node;
};

/**
 * struct aipu_job_manager - job manager
 *        Maintain all jobs and update their status
 *
 * @scheduled_queue_head: scheduled job queue head
 * @pending_queue_head: pending job queue head
 * @sched_num: number of jobs have been scheduled
 * @max_sched_num: maximum allowed scheduled job number
 * @lock: spinlock
 * @dev: device struct pointer
 */
struct aipu_job_manager {
        struct aipu_job *scheduled_queue_head;
        struct aipu_job *pending_queue_head;
        int sched_num;
        int max_sched_num;
        int init_done;
        spinlock_t lock;
        struct device *dev;
};

/**
 * @brief initialize an existing job manager struct during driver probe phase
 *
 * @param job_manager: job_manager struct pointer allocated from user;
 * @param p_dev: aipu device struct pointer
 * @param max_sched_num: maximum allowed scheduled job number;
 *
 * @return 0 if successful; others if failed;
 */
int aipu_init_job_manager(struct aipu_job_manager *job_manager, struct device *p_dev, int max_sched_num);
/**
 * @brief de-init job manager
 *
 * @param job_manager: job_manager struct pointer allocated from user;
 *
 * @return void
 */
void aipu_deinit_job_manager(struct aipu_job_manager *job_manager);
/**
 * @brief schedule new job flushed from userland
 *
 * @param job_manager: job_manager struct pointer;
 * @param user_job: user_job struct;
 * @param kern_job: session job;
 * @param session: session pointer refernece of this job;
 *
 * @return AIPU_ERRCODE_NO_ERROR if successful; others if failed;
 */
int aipu_job_manager_schedule_new_job(struct aipu_job_manager *job_manager, struct user_job *user_job,
    struct session_job *kern_job, struct aipu_session *session);
/**
 * @brief update job state and indicating if exception happens
 *
 * @param aipu_priv: aipu private struct
 * @param exception_flag: exception flag
 *
 * @return void
 */
void aipu_job_manager_update_job_state_irq(void *aipu_priv, int exception_flag);
/**
 * @brief done interrupt handler for job manager
 *
 * @param job_manager: job_manager struct pointer;
 *
 * @return void
 */
void aipu_job_manager_update_job_queue_done_irq(struct aipu_job_manager *job_manager);
/**
 * @brief cancel all jobs flushed by a user thread
 *
 * @param job_manager: job_manager struct pointer allocated from user;
 * @param session: session serviced for that user thread
 *
 * @return AIPU_ERRCODE_NO_ERROR if successful; others if failed;
 */
int aipu_job_manager_cancel_session_jobs(struct aipu_job_manager *job_manager,
        struct aipu_session *session);
/**
 * @brief invalidate/kill a timeout job
 *
 * @param job_manager: job_manager struct pointer allocated from user;
 * @param job_id: job ID
 *
 * @return AIPU_ERRCODE_NO_ERROR if successful; others if failed;
 */
int aipu_invalidate_timeout_job(struct aipu_job_manager *job_manager, int job_id);
/**
 * @brief show KMD job info via sysfs
 *
 * @param job_manager: job_manager struct pointer allocated from user;
 * @param buf: userspace buffer for KMD to fill the job info
 *
 * @return buf written bytes number;
 */
int aipu_job_manager_sysfs_job_show(struct aipu_job_manager *job_manager, char *buf);

#endif /* _AIPU_JOB_MANAGER_H_ */
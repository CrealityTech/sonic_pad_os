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
 * @file aipu_session.h
 * session module header file
 */

#ifndef _AIPU_SESSION_H_
#define _AIPU_SESSION_H_

#include <linux/list.h>
#include <linux/mm_types.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/device.h>
#include "uk_interface/aipu_buf_req.h"
#include "uk_interface/aipu_job_desc.h"
#include "uk_interface/aipu_profiling.h"
#include "uk_interface/aipu_job_status.h"
#include "aipu_buffer.h"
#include "aipu_thread_waitqueue.h"

/**
 * struct session_buf: session private buffer list
 * @desc: buffer descriptor struct
 * @dev_offset: offset of this buffer in device file
 * @type: buffer type: CMA/SRAM/RESERVED
 * @map_num: memory mmapped number
 * @head: list head struct
 */
struct session_buf {
        struct aipu_buffer desc;
        u64 dev_offset;
        u32 type;
        int map_num;
        struct list_head head;
};

/**
 * struct session_job: session private job list
 * @uthread_id: ID of user thread scheduled this job
 * @desc: job descriptor struct
 * @state: job state
 * @exception_type: type of exception if any
 * @pdata: profiling data struct
 * @head: list head struct
 */
struct session_job {
        int uthread_id;
        struct user_job_desc desc;
        int state;
        int exception_type;
        struct profiling_data pdata;
        struct list_head head;
};

/**
 * struct aipu_session: private data struct for every file open operation
 * @user_pid: ID of the user thread doing the open operation
 * @sbuf_list: successfully allocated shared buffer of this session
 * @sbuf_lock: mutex lock for sbuf list
 * @job_list: job list of this session
 * @job_lock: spinlock for job list
 * @aipu_priv: aipu_priv struct pointer
 * @wait_queue_head: thread waitqueue list head of this session
 * @com_wait: session common waitqueue head
 * @single_thread_poll: flag to indicate the polling method, thread vs. fd
 */
struct aipu_session {
        int user_pid;
        struct session_buf sbuf_list;
        struct mutex sbuf_lock;
        struct session_job job_list;
        spinlock_t job_lock;
        void *aipu_priv;
        struct aipu_thread_wait_queue *wait_queue_head;
        wait_queue_head_t com_wait;
        int single_thread_poll;
};

/*
 * @brief create unique session DS for an open request
 *
 * @param pid: user mode thread pid
 * @param aipu: aipu_priv struct pointer
 * @param p_session: session struct pointer
 *
 * @return 0 if successful; others if failed.
 */
int aipu_create_session(int pid, void *aipu_priv,
        struct aipu_session **p_session);
/*
 * @brief destroy an existing session
 *
 * @param session: session pointer
 *
 * @return AIPU_KMD_ERR_OK if successful; others if failed.
 */
int aipu_destroy_session(struct aipu_session *session);
/*
 * @brief get pid of this session
 *
 * @param session: session pointer
 *
 * @return id if successful; 0 if failed.
 */
int aipu_get_session_pid(struct aipu_session *session);
/*
 * @brief add an allocated buffer of this session
 *
 * @param session: session pointer
 * @param buf_req: request buffer struct pointer
 * @param buf: buffer allocated
 *
 * @return AIPU_KMD_ERR_OK if successful; others if failed.
 */
int aipu_session_add_buf(struct aipu_session *session, struct buf_request *buf_req,
        struct aipu_buffer *buf);
/*
 * @brief remove an allocated buffer of this session
 *
 * @param session: session pointer
 * @param buf: buffer to be removed
 *
 * @return AIPU_KMD_ERR_OK if successful; others if failed.
 */
int aipu_session_detach_buf(struct aipu_session *session, struct buf_desc *buf);
/*
 * @brief mmap an allocated buffer of this session
 *
 * @param session: session pointer
 * @param vma: vm_area_struct
 * @param dev: device struct
 *
 * @return AIPU_KMD_ERR_OK if successful; others if failed.
 */
int aipu_session_mmap_buf(struct aipu_session *session, struct vm_area_struct *vma, struct device *dev);
/*
 * @brief get first valid buffer descriptor of this session
 *
 * @param session: session pointer
 *
 * @return buffer if successful; NULL if failed.
 */
struct aipu_buffer* aipu_get_session_sbuf_head(struct aipu_session *session);
/*
 * @brief add a job descriptor of this session
 *
 * @param session: session pointer
 * @param user_job: userspace job descriptor pointer
 *
 * @return non-NULL kernel job ptr if successful; NULL if failed.
 */
struct session_job* aipu_session_add_job(struct aipu_session *session, struct user_job *user_job);
/*
 * @brief delete all jobs of a session
 *
 * @param session: session pointer
 *
 * @return AIPU_KMD_ERR_OK if successful; others if failed.
 */
int aipu_session_delete_jobs(struct aipu_session *session);
/*
 * @brief job done interrupt bottom half handler
 *
 * @param session: session pointer
 * @param job: session job pointer
 * @param excep_flag: exception flag
 *
 */
void aipu_session_job_done(struct aipu_session *session, struct session_job *job,
        int excep_flag);
/*
 * @brief update bandwidth profiling data after job done
 *
 * @param session: session pointer
 * @param job: session job pointer
 *
 */
void aipu_session_job_update_pdata(struct aipu_session *session, struct session_job *job);
/*
 * @brief check if any scheduled job of the specified thread is done/exception
 *
 * @param session: session pointer
 * @param uthread_id: user thread ID
 *
 * @return 1 if has don job(s); 0 if no.
 */
int aipu_session_thread_has_end_job(struct aipu_session *session, int uthread_id);
/*
 * @brief get one or multiple end jobs' status
 *
 * @param session: session pointer
 * @param job_status: job status query struct
 *
 * @return 1 if has don job(s); 0 if no.
 */
int aipu_session_get_job_status(struct aipu_session *session, struct job_status_query *job_status);
/*
 * @brief add waitqueue into session thread waitqueue list
 *
 * @param session: session pointer
 * @param filp: file struct from file operation API
 * @param wait: wait struct from poll file operation API
 * @param uthread_id: user thread ID
 *
 */
void aipu_session_add_poll_wait_queue(struct aipu_session *session,
    struct file *filp, struct poll_table_struct *wait, int uthread_id);
/*
 * @brief mark the scheduled time of a job
 *
 * @param job: session job pointer
 */
void session_job_mark_sched(struct session_job *job);
/*
 * @brief mark the done time of a job
 *
 * @param job: session job pointer
 */
void session_job_mark_done(struct session_job *job);
/*
 * @brief check if a job is enabled to do profiling
 *
 * @param job: session job pointer
 */
int is_session_job_prof_enabled(struct session_job *job);

#endif //_AIPU_SESSION_H_
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
 * @file aipu_session.c
 * Implementation of session module
 */

#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/string.h>
#include "uk_interface/aipu_errcode.h"
#include "aipu_session.h"
#include "aipu_mm.h"
#include "aipu.h"
#include "log.h"

static void init_session_buf(struct session_buf *buf,
        struct aipu_buffer *desc, u64 dev_offset)
{
        if (buf) {
                if (!desc)
                        memset(&buf->desc, 0, sizeof(struct aipu_buffer));
                else {
                        buf->desc = *desc;
                        buf->type = desc->type;
                }
                buf->dev_offset = dev_offset;
                buf->map_num = 0;
                INIT_LIST_HEAD(&buf->head);
        }
}

static struct session_buf *create_session_buf(struct aipu_buffer *desc,
        u64 dev_offset)
{
        struct session_buf *sbuf = NULL;

        if (!desc) {
                LOG(LOG_ERR, "descriptor is needed while creating new session buf!");
                goto finish;
        }

        sbuf = kzalloc(sizeof(struct session_buf), GFP_KERNEL);
        init_session_buf(sbuf, desc, dev_offset);

finish:
        return sbuf;
}

static int destroy_session_buffer(struct session_buf *buf)
{
        int ret = AIPU_ERRCODE_NO_ERROR;

        if (buf)
                kfree(buf);
        else {
                LOG(LOG_ERR, "invalid null buf args or list not empty!");
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
        }

        return ret;
}

static void init_session_job(struct session_job *job, struct user_job_desc *desc)
{
    if (job) {
            /* init job->desc to be all 0 if desc == NULL */
            job->uthread_id = task_pid_nr(current);
            if (!desc)
                    memset(&job->desc, 0, sizeof(struct user_job_desc));
            else
                    job->desc = *desc;
            job->state = 0;
            job->exception_type = AIPU_EXCEP_NO_EXCEPTION;
            INIT_LIST_HEAD(&job->head);
    }
}

static struct session_job *create_session_job(struct user_job_desc *desc)
{
        struct session_job *new_job = NULL;

        if (!desc) {
                LOG(LOG_ERR, "descriptor is needed while creating new session job!");
                goto finish;
        }

        new_job = kzalloc(sizeof(struct session_job), GFP_KERNEL);
        init_session_job(new_job, desc);

finish:
        return new_job;
}

static int destroy_session_job(struct session_job *job)
{
        int ret = AIPU_ERRCODE_NO_ERROR;

        if (job)
                kfree(job);
        else {
                LOG(LOG_ERR, "invalid null job args or list not empty!");
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
        }

        return ret;
}

static int is_session_all_jobs_end(struct aipu_session *session)
{
        return (!session) ? 1: list_empty(&session->job_list.head);
}

static int is_session_all_buffers_freed(struct aipu_session *session)
{
        return (!session) ? 1: list_empty(&session->sbuf_list.head);
}

static struct session_buf *find_buffer_bydesc_no_lock(struct aipu_session *session,
        struct buf_desc *buf_desc)
{
        struct session_buf *target_buf  = NULL;
        struct session_buf *session_buf = NULL;
        struct list_head *node = NULL;

        if ((!session) || (!buf_desc)) {
                LOG(LOG_ERR, "invalid input session or buf_desc args to be null!");
                goto finish;
        }

        list_for_each(node, &session->sbuf_list.head) {
                session_buf = list_entry(node, struct session_buf, head);

                if (session_buf &&
                    (session_buf->desc.pa == buf_desc->pa) &&
                    (session_buf->desc.bytes == buf_desc->bytes)) {
                        target_buf = session_buf;
                        LOG(LOG_CLOSE, "found matching buffer to be deleted.");
                        goto finish;
                }
        }

finish:
        return target_buf;
}

static struct session_buf *find_buffer_byoffset_no_lock(struct aipu_session *session,
    u64 offset, int len)
{
        struct session_buf *target_buf = NULL;
        struct session_buf *session_buf = NULL;
        struct list_head *node = NULL;

        if (!session) {
                LOG(LOG_ERR, "invalid input session args to be null!");
                goto finish;
        }

        list_for_each(node, &session->sbuf_list.head) {
                session_buf = list_entry(node, struct session_buf, head);
                if (session_buf &&
                    (session_buf->dev_offset == offset) &&
                    (len <= session_buf->desc.bytes)) {
                        target_buf = session_buf;
                        goto finish;
                }
        }

finish:
        return target_buf;
}

/********************************************************************************
 *  The following APIs are called in thread context for session obj management  *
 *  and member query service                                                    *
 *  -- aipu_create_session                                                      *
 *  -- aipu_destroy_session                                                     *
 *  -- aipu_get_session_pid                                                     *
 ********************************************************************************/
int aipu_create_session(int pid, void *aipu_priv,
        struct aipu_session **p_session)
{
        int ret = 0;
        struct aipu_session *session = NULL;
        struct device *dev = NULL;

        if ((!aipu_priv) || (!p_session)) {
                LOG(LOG_ERR, "invalid input session or common args to be null!");
                goto finish;
        }

        dev = ((struct aipu_priv*)aipu_priv)->dev;

        session = kzalloc(sizeof(struct aipu_session), GFP_KERNEL);
        if (!session)
                return -ENOMEM;

        session->user_pid = pid;
        init_session_buf(&session->sbuf_list, NULL, 0);
        mutex_init(&session->sbuf_lock);
        init_session_job(&session->job_list, NULL);
        spin_lock_init(&session->job_lock);
        session->aipu_priv = aipu_priv;
        session->wait_queue_head = create_thread_wait_queue_no_lock(NULL, 0);
        init_waitqueue_head(&session->com_wait);
        session->single_thread_poll = 0;

        *p_session = session;
        dev_dbg(dev, "[%d] new session created\n", pid);

finish:
        return ret;
}

int aipu_destroy_session(struct aipu_session *session)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        struct device *dev = NULL;
        int pid = 0;

        if (session &&
            is_session_all_jobs_end(session) &&
            is_session_all_buffers_freed(session)) {
                dev = ((struct aipu_priv*)session->aipu_priv)->dev;
                pid = session->user_pid;
                delete_wait_queue(session->wait_queue_head);
                kfree(session->wait_queue_head);
                kfree(session);
                dev_dbg(dev, "[%d] session destroyed\n", pid);
        } else {
                LOG(LOG_WARN, "invalid input session args to be null or invalid operation!");
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
        }

        return ret;
}

int aipu_get_session_pid(struct aipu_session *session)
{
        if (session)
                return session->user_pid;
        else {
                LOG(LOG_WARN, "invalid input session args to be null!");
                return map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
        }
}

/********************************************************************************
 *  The following APIs are called in thread context for servicing user space    *
 *  request in resource allocation/free and job scheduling via fops             *
 *  -- aipu_session_add_buf                                                     *
 *  -- aipu_session_detach_buf                                                  *
 *  -- aipu_get_session_sbuf_head                                               *
 *  -- aipu_session_mmap_buf                                                    *
 *  -- aipu_session_add_job                                                     *
 *  -- aipu_session_delete_jobs                                                 *
 ********************************************************************************/
int aipu_session_add_buf(struct aipu_session *session,
        struct buf_request *buf_req, struct aipu_buffer *buf)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        struct session_buf *new_sbuf = NULL;

        if ((!session) || (!buf_req) || (!buf)) {
                LOG(LOG_ERR, "invalid input session or buf_req or buf args to be null!");
                if (buf_req)
                        buf_req->errcode = AIPU_ERRCODE_INTERNAL_NULLPTR;
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
                goto finish;
        }

        new_sbuf = create_session_buf(buf, buf->pa);
        if (!new_sbuf) {
                LOG(LOG_ERR, "create session buf failed!");
                buf_req->errcode = AIPU_ERRCODE_CREATE_KOBJ_ERR;
                ret = map_errcode(AIPU_ERRCODE_CREATE_KOBJ_ERR);
        } else {
                mutex_lock(&session->sbuf_lock);
                list_add(&new_sbuf->head, &session->sbuf_list.head);

                /* success */
                /* copy buffer descriptor to userland */
                buf_req->desc.pa = buf->pa;
                buf_req->desc.dev_offset = buf->pa;
                buf_req->desc.bytes = buf->bytes;
                buf_req->desc.region_id = buf->region_id;
                buf_req->errcode = AIPU_ERRCODE_NO_ERROR;
                mutex_unlock(&session->sbuf_lock);
        }

finish:
        return ret;
}

int aipu_session_detach_buf(struct aipu_session *session, struct buf_desc *buf_desc)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        struct session_buf *target_buf = NULL;

        if ((!session) || (!buf_desc)) {
                LOG(LOG_ERR, "invalid input session or buf args to be null!");
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
                goto finish;
        }

        /* LOCK */
        mutex_lock(&session->sbuf_lock);
        target_buf = find_buffer_bydesc_no_lock(session, buf_desc);
        if (!target_buf) {
                LOG(LOG_ERR, "no corresponding buffer found in this session!");
                ret = map_errcode(AIPU_ERRCODE_ITEM_NOT_FOUND);
        } else {
                list_del(&target_buf->head);
                ret = destroy_session_buffer(target_buf);
                if (AIPU_ERRCODE_NO_ERROR != ret)
                        LOG(LOG_ERR, "destroy session failed!");
                else {
                        /* success */
                        target_buf = NULL;
                }
        }
        mutex_unlock(&session->sbuf_lock);
        /* UNLOCK */

finish:
        return ret;
}

int aipu_session_mmap_buf(struct aipu_session *session, struct vm_area_struct *vma, struct device *dev)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        u64 offset = 0;
        int len = 0;
        unsigned long vm_pgoff = 0;
        struct session_buf *buf = NULL;

        if ((!session) || (!vma)) {
                LOG(LOG_ERR, "invalid input session or vma args to be null!");
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
                goto finish;
        }

        offset = vma->vm_pgoff * PAGE_SIZE;
        len = vma->vm_end - vma->vm_start;

        /* LOCK */
        mutex_lock(&session->sbuf_lock);
        /* to find an allocated buffer with matching dev offset and length */
        buf = find_buffer_byoffset_no_lock(session, offset, len);
        if (!buf) {
                LOG(LOG_ERR, "invalid operation or args: no corresponding buffer found in this session!");
                ret = map_errcode(AIPU_ERRCODE_ITEM_NOT_FOUND);
        } else {
                if (buf->map_num) {
                        LOG(LOG_ERR, "duplicated mmap operations on identical buffer!");
                        ret = map_errcode(AIPU_ERRCODE_INVALID_OPS);
                } else {
                        vm_pgoff = vma->vm_pgoff;
                        vma->vm_pgoff = 0;
                        vma->vm_flags |= VM_IO;
                        vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

                        if (buf->type == AIPU_MEM_TYPE_CMA) {
                                ret = dma_mmap_coherent(dev, vma, buf->desc.va,
                                        (dma_addr_t)buf->desc.pa, buf->desc.bytes);
                                if (ret)
                                        LOG(LOG_ERR, "CMA mmap to userspace failed!");
                        } else if ((buf->type == AIPU_MEM_TYPE_SRAM) ||
                                   (buf->type == AIPU_MEM_TYPE_RESERVED)) {
                                ret = remap_pfn_range(vma, vma->vm_start, buf->desc.pa >> PAGE_SHIFT,
                                        vma->vm_end - vma->vm_start, vma->vm_page_prot);
                                if (ret)
                                        LOG(LOG_ERR, "SRAM mmap to userspace failed!");
                        }

                        vma->vm_pgoff = vm_pgoff;
                        if(!ret)
                                buf->map_num++;
                }
        }
        mutex_unlock(&session->sbuf_lock);
        /* UNLOCK */

finish:
    return ret;
}

struct aipu_buffer *aipu_get_session_sbuf_head(struct aipu_session *session)
{
        struct session_buf *session_buf = NULL;
        struct aipu_buffer *buf_desc = NULL;
        struct list_head *node = NULL;

        if (!session) {
                LOG(LOG_ERR, "invalid input session or buf_req or buf args to be null!");
                goto finish;
        }

        list_for_each(node, &session->sbuf_list.head) {
                if (!list_empty(node)) {
                        session_buf = list_entry(node, struct session_buf, head);
                        buf_desc = &session_buf->desc;
                        goto finish;
                }
        }

finish:
        return buf_desc;
}

struct session_job *aipu_session_add_job(struct aipu_session *session, struct user_job *user_job)
{
        struct session_job *kern_job = NULL;

        if ((!session) || (!user_job)) {
                LOG(LOG_ERR, "invalid input session or user_job args to be null!");
                if (NULL != user_job)
                        user_job->errcode = AIPU_ERRCODE_INTERNAL_NULLPTR;
                goto finish;
        }

        kern_job = create_session_job(&user_job->desc);
        if (!kern_job) {
                LOG(LOG_ERR, "invalid input session or job args to be null!");
                user_job->errcode = AIPU_ERRCODE_CREATE_KOBJ_ERR;
        } else {
                /* THREAD LOCK */
                spin_lock_bh(&session->job_lock);
                list_add(&kern_job->head, &session->job_list.head);
                create_thread_wait_queue_no_lock(session->wait_queue_head, kern_job->uthread_id);
                spin_unlock_bh(&session->job_lock);
                /* THREAD UNLOCK */

                /* success */
                user_job->errcode = AIPU_ERRCODE_NO_ERROR;
        }

finish:
        return kern_job;
}

int aipu_session_delete_jobs(struct aipu_session *session)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        struct session_job *cursor = NULL;
        struct session_job *next = NULL;

        if (!session) {
                LOG(LOG_ERR, "invalid input session to be null!");
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
                goto finish;
        }

        /* THREAD LOCK */
        spin_lock_bh(&session->job_lock);
        list_for_each_entry_safe(cursor, next, &session->job_list.head, head) {
                list_del(&cursor->head);
                destroy_session_job(cursor);
        }
        spin_unlock_bh(&session->job_lock);
        /* THREAD UNLOCK */

finish:
        return ret;
}

/********************************************************************************
 *  The following APIs are called in interrupt context to update end job status *
 *  They will be called by IRQ handlers in job manager module                   *
 *  Note that param session and session_job passed by job manager is assumed    *
 *  to be valid and active (not cancelled by userland)                          *
 *  -- aipu_session_job_done                                                    *
 *  -- aipu_session_job_excep                                                   *
 *  -- aipu_session_job_update_pdata                                            *
 ********************************************************************************/
void aipu_session_job_done(struct aipu_session *session, struct session_job *job,
        int excep_flag)
{
        struct aipu_thread_wait_queue *queue = NULL;
        wait_queue_head_t *thread_queue = NULL;

        if ((!session) || (!job)) {
                LOG(LOG_ERR, "invalid input session or job args to be null!");
                return;
        }

        if (AIPU_EXCEP_NO_EXCEPTION == excep_flag)
                LOG(LOG_DEBUG, "Done interrupt received...");
        else
                LOG(LOG_DEBUG, "Exception interrupt received...");

        /* IRQ LOCK */
        spin_lock(&session->job_lock);
        job->state = AIPU_JOB_STATE_END;
        job->exception_type = AIPU_EXCEP_NO_EXCEPTION;

        if (session->single_thread_poll) {
                queue = get_thread_wait_queue_no_lock(session->wait_queue_head,
                        job->uthread_id);
                if (queue)
                        thread_queue = &queue->p_wait;
                else {
                        LOG(LOG_ERR, "job waitqueue not found!");
                        spin_unlock(&session->job_lock);
                        return;
                }
        }
        else
                thread_queue = &session->com_wait;

        if (thread_queue)
                wake_up_interruptible(thread_queue);
        else
                LOG(LOG_ERR, "[%d] thread wait queue not found!", job->uthread_id);

        spin_unlock(&session->job_lock);
        /* IRQ UNLOCK */
}

void aipu_session_job_update_pdata(struct aipu_session *session, struct session_job *job)
{
        struct aipu_priv *aipu = NULL;
        if ((!session) || (!job)) {
                LOG(LOG_ERR, "invalid input session or desc or scc args to be null!");
                return;
        }

        aipu = (struct aipu_priv *)session->aipu_priv;

        if (aipu && job->desc.enable_prof)
                aipu_priv_read_profiling_reg(aipu, &job->pdata);

        LOG(LOG_CLOSE, "TOT WDATA LSB: 0x%x\n", job->pdata.wdata_tot_lsb);
        LOG(LOG_CLOSE, "TOT WDATA MSB: 0x%x\n", job->pdata.wdata_tot_msb);
        LOG(LOG_CLOSE, "TOT RDATA LSB: 0x%x\n", job->pdata.rdata_tot_lsb);
        LOG(LOG_CLOSE, "TOT RDATA MSB: 0x%x\n", job->pdata.rdata_tot_msb);
        LOG(LOG_CLOSE, "TOT CYCLE LSB: 0x%x\n", job->pdata.tot_cycle_lsb);
        LOG(LOG_CLOSE, "TOT CYCLE MSB: 0x%x\n", job->pdata.tot_cycle_msb);
}

/********************************************************************************
 *  The following APIs are called in thread context for user query service      *
 *  after job end                                                               *
 *  -- aipu_session_query_pdata                                                 *
 *  -- aipu_session_thread_has_end_job                                          *
 *  -- aipu_session_get_job_status                                              *
 ********************************************************************************/
int aipu_session_thread_has_end_job(struct aipu_session *session, int uthread_id)
{
        int ret = 0;
        struct session_job *session_job = NULL;
        struct list_head *node = NULL;
        int wake_up_single = 0;

        if (!session) {
                LOG(LOG_ERR, "invalid input session or excep args to be null!");
                goto finish;
        }

        /**
         * If uthread_id found in job_list, then the condition returns is specific to
         * the status of jobs of this thread (thread-specific); otherwise, the condition
         * is specific to the status of jobs of this session (fd-specific).
         */
        spin_lock(&session->job_lock);
        list_for_each(node, &session->job_list.head) {
                session_job = list_entry(node, struct session_job, head);
                if (session_job && (session_job->uthread_id == uthread_id)) {
                        wake_up_single = 1;
                        break;
                }
        }

        list_for_each(node, &session->job_list.head) {
                session_job = list_entry(node, struct session_job, head);
                if (session_job && (session_job->state == AIPU_JOB_STATE_END)) {
                        if (wake_up_single) {
                                if (session_job->uthread_id == uthread_id) {
                                        ret = 1;
                                        break;
                                }
                        } else {
                                ret = 1;
                                break;
                        }
                }
        }
        spin_unlock(&session->job_lock);

finish:
        return ret;
}

int aipu_session_get_job_status(struct aipu_session *session, struct job_status_query *job_status)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        int query_cnt;
        struct job_status_desc *status = NULL;
        struct session_job *cursor = NULL;
        struct session_job *next = NULL;
        int poll_iter = 0;

        if ((!session) || (!job_status)) {
                LOG(LOG_ERR, "invalid input session or excep args to be null!");
                goto finish;
        }

        if (job_status->max_cnt < 1) {
                job_status->errcode = AIPU_ERRCODE_INVALID_ARGS;
                ret = map_errcode(AIPU_ERRCODE_INVALID_ARGS);
                goto finish;
        }

        if (job_status->get_single_job)
                query_cnt = 1;
        else
                query_cnt = job_status->max_cnt;

        status = kzalloc(query_cnt * sizeof(struct job_status_desc), GFP_KERNEL);
        if (!status) {
                job_status->errcode = AIPU_ERRCODE_NO_MEMORY;
                ret = map_errcode(AIPU_ERRCODE_NO_MEMORY);
                goto finish;
        }

        job_status->poll_cnt = 0;
        spin_lock(&session->job_lock);
        list_for_each_entry_safe(cursor, next, &session->job_list.head, head) {
                if (job_status->poll_cnt == job_status->max_cnt)
                        break;

                if ((((cursor->desc.job_id == job_status->job_id) && (job_status->get_single_job)) ||
                    (!job_status->get_single_job)) &&
                    (cursor->state == AIPU_JOB_STATE_END)) {
                        status[poll_iter].job_id = cursor->desc.job_id;
                        status[poll_iter].thread_id = session->user_pid;
                        status[0].state = (cursor->exception_type == AIPU_EXCEP_NO_EXCEPTION) ?
                                AIPU_JOB_STATE_DONE : AIPU_JOB_STATE_EXCEPTION;
                        status[poll_iter].pdata = cursor->pdata;
                        job_status->poll_cnt++;
                        list_del(&cursor->head);
                        destroy_session_job(cursor);
                        cursor = NULL;
                        if (job_status->get_single_job)
                                break;
                }
        }
        spin_unlock(&session->job_lock);

        if (!job_status->poll_cnt) {
                job_status->errcode = AIPU_ERRCODE_ITEM_NOT_FOUND;
                ret = map_errcode(AIPU_ERRCODE_ITEM_NOT_FOUND);
                goto clean;
        }

        ret = copy_to_user((struct job_status_desc __user*)job_status->status, status,
            job_status->poll_cnt * sizeof(struct job_status_desc));
        if (AIPU_ERRCODE_NO_ERROR == ret)
                job_status->errcode = AIPU_ERRCODE_NO_ERROR;

clean:
        kfree(status);

finish:
        return ret;
}

wait_queue_head_t *aipu_session_get_wait_queue(struct aipu_session *session, int uthread_id)
{
    struct aipu_thread_wait_queue *queue = NULL;

        if (!session) {
                LOG(LOG_ERR, "invalid input session to be null!");
                return NULL;
        }

        /* LOCK */
        spin_lock(&session->job_lock);
        queue = get_thread_wait_queue_no_lock(session->wait_queue_head, uthread_id);
        spin_unlock(&session->job_lock);
        /* UNLOCK */

        if (queue)
                return &queue->p_wait;

        return NULL;
}

void aipu_session_add_poll_wait_queue(struct aipu_session *session,
    struct file *filp, struct poll_table_struct *wait, int uthread_id)
{
        struct aipu_thread_wait_queue *wait_queue = NULL;
        struct session_job *curr = NULL;

        if ((!session) || (!filp) || (!wait)) {
                LOG(LOG_ERR, "invalid input session to be null!");
                return;
        }

        spin_lock_bh(&session->job_lock);
        list_for_each_entry(curr, &session->job_list.head, head) {
                if (curr->uthread_id == uthread_id) {
                        wait_queue = get_thread_wait_queue_no_lock(session->wait_queue_head,
                                uthread_id);
                        if (wait_queue) {
                                poll_wait(filp, &wait_queue->p_wait, wait);
                                session->single_thread_poll = 1;
                        }
                        else
                                LOG(LOG_ERR, "thread wait_queue not found!");
                        break;
                }
        }

        if (!session->single_thread_poll)
                poll_wait(filp, &session->com_wait, wait);
        spin_unlock_bh(&session->job_lock);
}

void session_job_mark_sched(struct session_job *job)
{
        if (job)
                do_gettimeofday(&job->pdata.sched_tv);
}

void session_job_mark_done(struct session_job *job)
{
        if (job)
                do_gettimeofday(&job->pdata.done_tv);
}

int is_session_job_prof_enabled(struct session_job *job)
{
        int ret = 0;
        if (job)
                ret = job->desc.enable_prof;
        return ret;
}

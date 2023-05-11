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
 * @file aipu_job_manager.c
 * Job manager module implementation file
 */

#include <linux/slab.h>
#include <linux/string.h>
#include <linux/time.h>
#include "uk_interface/aipu_job_desc.h"
#include "uk_interface/aipu_errcode.h"
#include "aipu_job_manager.h"
#include "aipu.h"

static int init_aipu_job(struct aipu_job *aipu_job, struct user_job_desc *desc,
        struct session_job *kern_job, struct aipu_session *session)
{
        int ret = AIPU_ERRCODE_NO_ERROR;

        if (!aipu_job) {
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
                goto finish;
        }

        if (kern_job)
                aipu_job->uthread_id = kern_job->uthread_id;

        if (!desc)
                memset(&aipu_job->desc, 0, sizeof(struct user_job_desc));
        else
                aipu_job->desc = *desc;
        aipu_job->session = (struct aipu_session *)session;
        aipu_job->session_job = (struct session_job *)kern_job;
        aipu_job->state = AIPU_JOB_STATE_IDLE;
        aipu_job->exception_flag = AIPU_EXCEP_NO_EXCEPTION;
        aipu_job->valid_flag = AIPU_JOB_FLAG_VALID;
        INIT_LIST_HEAD(&aipu_job->node);

finish:
        return ret;
}

static void destroy_aipu_job(struct aipu_job *job)
{
        if (job)
                kfree(job);
}

static void remove_aipu_job(struct aipu_job *job)
{
        if (job) {
                list_del(&job->node);
                destroy_aipu_job(job);
        }

}

static struct aipu_job *create_aipu_job(struct user_job_desc *desc,
        struct session_job *kern_job, struct aipu_session *session)
{
        struct aipu_job *new_aipu_job = NULL;

        new_aipu_job = kzalloc(sizeof(struct aipu_job), GFP_KERNEL);
        if (init_aipu_job(new_aipu_job, desc, kern_job, session) != AIPU_ERRCODE_NO_ERROR) {
                destroy_aipu_job(new_aipu_job);
                new_aipu_job = NULL;
        }

        return new_aipu_job;
}

static void aipu_job_manager_trigger_job_sched(struct aipu_priv *aipu, struct aipu_job *aipu_job)
{
        if (aipu && aipu_job) {
                aipu_priv_trigger(aipu, &aipu_job->desc, aipu_job->uthread_id);
                if (is_session_job_prof_enabled(aipu_job->session_job)) {
                        session_job_mark_sched(aipu_job->session_job);
                        aipu_priv_start_bw_profiling(aipu);
                }
        }
}

int aipu_init_job_manager(struct aipu_job_manager *job_manager, struct device *p_dev, int max_sched_num)
{
        int ret = 0;

        if ((!job_manager) || (!p_dev))
                return -EINVAL;

        if (job_manager->init_done)
                return 0;

        job_manager->scheduled_queue_head = create_aipu_job(NULL, NULL, NULL);
        job_manager->pending_queue_head = create_aipu_job(NULL, NULL, NULL);
        if ((!job_manager->pending_queue_head) || (!job_manager->scheduled_queue_head))
                return -ENOMEM;

        job_manager->sched_num = 0;
        job_manager->max_sched_num = max_sched_num;
        spin_lock_init(&job_manager->lock);
        job_manager->dev = p_dev;
        job_manager->init_done = 1;

        return ret;
}

static void delete_queue(struct aipu_job *head)
{
        struct aipu_job *cursor = head;
        struct aipu_job *next = NULL;

        if (head) {
                list_for_each_entry_safe(cursor, next, &head->node, node) {
                        remove_aipu_job(cursor);
                }
        }
}

void aipu_deinit_job_manager(struct aipu_job_manager *job_manager)
{
        if (job_manager) {
                delete_queue(job_manager->scheduled_queue_head);
                delete_queue(job_manager->pending_queue_head);
                job_manager->sched_num = 0;
        }
}

static void aipu_schedule_pending_job_no_lock(struct aipu_job_manager *job_manager)
{
        struct aipu_job *curr = NULL;
        struct aipu_priv *aipu = container_of(job_manager, struct aipu_priv, job_manager);

        if (!job_manager) {
                dev_err(job_manager->dev, "invalid input args user_job or kern_job or session to be NULL!");
                return;
        }

        /* 1st pending job should be scheduled if any */
        if ((!list_empty(&job_manager->pending_queue_head->node)) &&
            (job_manager->sched_num < job_manager ->max_sched_num) &&
            (aipu_priv_is_idle(aipu))) {
                /*
                  detach head of pending queue and add it to the tail of scheduled job queue

                                      |--->>------->>---|
                                      |(real head)      |(tail)
                  --------------------------------    ----------------------------------
                  | j <=> j <=> j <=> j <=> head |    | [empty to fill] <=> j <=> head |
                  --------------------------------    ----------------------------------
                          pending job queue                   scheduled job queue
                */
                curr = list_next_entry(job_manager->pending_queue_head, node);

                aipu_job_manager_trigger_job_sched(aipu, curr);
                curr->state = AIPU_JOB_STATE_SCHED;
                list_move_tail(&curr->node, &job_manager->scheduled_queue_head->node);
                job_manager->sched_num++;
        } else {
                /**
                 * do nothing because no pending job needs to be scheduled
                 * or AIPU is not available to accept more jobs
                 */
                if (list_empty(&job_manager->pending_queue_head->node)) {
                        if (!task_pid_nr(current))
                                dev_dbg(job_manager->dev, "[IRQ] no pending job to trigger");
                        else
                                dev_dbg(job_manager->dev, "[%u] no pending job to trigger", task_pid_nr(current));
                }

                if (job_manager->sched_num >= job_manager->max_sched_num) {
                        if (!task_pid_nr(current))
                                dev_dbg(job_manager->dev, "[IRQ] AIPU busy and do not trigger");
                        else
                                dev_dbg(job_manager->dev, "[%u] AIPU busy and do not trigger", task_pid_nr(current));
                }

        }
}

int aipu_job_manager_schedule_new_job(struct aipu_job_manager *job_manager, struct user_job *user_job,
        struct session_job *session_job, struct aipu_session *session)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        struct aipu_job *aipu_job = NULL;

        if ((!job_manager) || (!user_job) || (!session_job) || (!session)) {
                if (user_job)
                        user_job->errcode = AIPU_ERRCODE_INTERNAL_NULLPTR;
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
                goto finish;
        }

        aipu_job = create_aipu_job(&user_job->desc, session_job, session);
        if (!aipu_job) {
                user_job->errcode = AIPU_ERRCODE_CREATE_KOBJ_ERR;
                ret = map_errcode(AIPU_ERRCODE_CREATE_KOBJ_ERR);
                goto finish;
        }

        /* LOCK */
        spin_lock_irq(&job_manager->lock);

        /* pending the flushed job from userland and try to schedule it */
        aipu_job->state = AIPU_JOB_STATE_PENDING;
        list_add_tail(&aipu_job->node, &job_manager->pending_queue_head->node);
        aipu_schedule_pending_job_no_lock(job_manager);

        spin_unlock_irq(&job_manager->lock);
        /* UNLOCK */

        /* success */
        user_job->errcode = AIPU_ERRCODE_NO_ERROR;

finish:
        return ret;
}

static int aipu_invalidate_job_no_lock(struct aipu_job_manager *job_manager,
        struct aipu_job *job)
{
        struct aipu_priv *aipu = container_of(job_manager, struct aipu_priv, job_manager);

        if ((!job_manager) || (!job))
                return -EINVAL;

        if (job->state == AIPU_JOB_STATE_SCHED) {
                if (aipu_priv_has_logic_reset(aipu)) {
                        /* do AIPU reset */
                        aipu_priv_logic_reset_release(aipu);
                        job_manager->sched_num--;
                        /* re-trigger */
                        aipu_schedule_pending_job_no_lock(job_manager);
                } else
                        job->valid_flag = 0;
        } else if (job->state == AIPU_JOB_STATE_PENDING) {
                remove_aipu_job(job);
        } else
                return -EINVAL;

        return 0;
}

static void aipu_invalidate_canceled_jobs_no_lock(struct aipu_job_manager *job_manager,
        struct aipu_job *head, struct aipu_session *session)
{
        struct aipu_job *cursor = NULL;
        struct aipu_job *next = NULL;

        if ((!job_manager) || (!head) || (!session))
                return;

        list_for_each_entry_safe(cursor, next, &head->node, node) {
                if (aipu_get_session_pid(cursor->session) == aipu_get_session_pid(session))
                        aipu_invalidate_job_no_lock(job_manager, cursor);
        }
}

int aipu_job_manager_cancel_session_jobs(struct aipu_job_manager *job_manager,
        struct aipu_session *session)
{
        int ret = AIPU_ERRCODE_NO_ERROR;

        if (!session) {
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
                goto finish;
        }

        /* LOCK */
        spin_lock_irq(&job_manager->lock);

        /**
         * invalidate all active jobs of this session in job manager
         */
        aipu_invalidate_canceled_jobs_no_lock(job_manager, job_manager->pending_queue_head, session);
        aipu_invalidate_canceled_jobs_no_lock(job_manager, job_manager->scheduled_queue_head, session);

        spin_unlock_irq(&job_manager->lock);
        /* UNLOCK */

        /* delete all session_job */
        aipu_session_delete_jobs(session);

finish:
        return ret;
}

static int aipu_invalidate_timeout_job_no_lock(struct aipu_job_manager *job_manager,
        struct aipu_job *head, int job_id)
{
        int ret = -EINVAL;
        struct aipu_job *cursor = NULL;
        struct aipu_job *next = NULL;

        if ((!job_manager) || (!head))
                return -EINVAL;

        list_for_each_entry_safe(cursor, next, &head->node, node) {
                if ((cursor->uthread_id == task_pid_nr(current)) &&
                        (cursor->desc.job_id == job_id)) {
                        ret = aipu_invalidate_job_no_lock(job_manager, cursor);
                        break;
                }
        }

        return ret;
}

int aipu_invalidate_timeout_job(struct aipu_job_manager *job_manager, int job_id)
{
        int ret = 0;

        if (!job_manager)
                return -EINVAL;

        /* LOCK */
        spin_lock_irq(&job_manager->lock);
        ret = aipu_invalidate_timeout_job_no_lock(job_manager, job_manager->pending_queue_head, job_id);
        if (ret) {
                ret = aipu_invalidate_timeout_job_no_lock(job_manager, job_manager->scheduled_queue_head, job_id);
                pr_debug("Timeout job invalidated from sched queue.");
        }
        else
                pr_debug("Timeout job invalidated from pending queue.");

        spin_unlock_irq(&job_manager->lock);
        /* UNLOCK */

        return ret;
}

static void aipu_job_manager_update_job_profiling_data(struct aipu_priv *aipu, struct aipu_job *job)
{
        if (job && is_session_job_prof_enabled(job->session_job) &&
            (job->state == AIPU_JOB_STATE_END) &&
            (job->exception_flag == AIPU_EXCEP_NO_EXCEPTION)) {
                aipu_priv_stop_bw_profiling(aipu);
                aipu_session_job_update_pdata(job->session, job->session_job);
        }
}

void aipu_job_manager_update_job_state_irq(void *aipu_priv, int exception_flag)
{
        struct aipu_job *curr = NULL;
        struct aipu_priv *aipu = (struct aipu_priv*)aipu_priv;
        struct aipu_job_manager *job_manager = &aipu->job_manager;

        /* LOCK */
        spin_lock(&job_manager->lock);
        list_for_each_entry(curr, &job_manager->scheduled_queue_head->node, node) {
                if (curr->state == AIPU_JOB_STATE_SCHED) {
                        curr->state = AIPU_JOB_STATE_END;
                        curr->exception_flag = exception_flag;

                        if (curr->exception_flag)
                                pr_debug("[IRQ] job 0x%x of thread %u EXCEPTION",
                                        curr->desc.job_id, curr->uthread_id);
                        else
                                pr_debug("[IRQ] job 0x%x of thread %u DONE",
                                        curr->desc.job_id, curr->uthread_id);


                        if (is_session_job_prof_enabled(curr->session_job))
                                session_job_mark_done(curr->session_job);

                        dev_dbg(job_manager->dev, "start: %ld s, %ld us", curr->session_job->pdata.sched_tv.tv_sec,
                                curr->session_job->pdata.sched_tv.tv_usec);
                        dev_dbg(job_manager->dev, "end: %ld s, %ld us", curr->session_job->pdata.done_tv.tv_sec,
                                curr->session_job->pdata.done_tv.tv_usec);

                        if (curr->valid_flag == AIPU_JOB_FLAG_VALID)
                                aipu_job_manager_update_job_profiling_data(aipu, curr);

                        if (job_manager->sched_num)
                                job_manager->sched_num--;
                        break;
                }
        }

        /* schedule a new pending job */
        aipu_schedule_pending_job_no_lock(job_manager);
        spin_unlock(&job_manager->lock);
        /* UNLOCK */
}

void aipu_job_manager_update_job_queue_done_irq(struct aipu_job_manager *job_manager)
{
        struct aipu_job *curr = NULL;
        struct aipu_job *next = NULL;

        /* LOCK */
        spin_lock(&job_manager->lock);
        list_for_each_entry_safe(curr, next, &job_manager->scheduled_queue_head->node, node) {
                if (AIPU_JOB_STATE_END != curr->state)
                        continue;

                /*
                   DO NOT call session API for invalid job because
                   session struct probably not exist on this occasion
                */
                if (AIPU_JOB_FLAG_VALID == curr->valid_flag) {
                        pr_debug("[BH] handling job 0x%x of thread %u...",
                                curr->desc.job_id, curr->uthread_id);
                        aipu_session_job_done(curr->session, curr->session_job,
                            curr->exception_flag);
                }
                else
                        pr_debug("[BH] this done job has been cancelled by user.");

                list_del(&curr->node);
                destroy_aipu_job(curr);
                curr = NULL;
                /* DO NOT minus sched_num here because upper half has done that */
        }
        spin_unlock(&job_manager->lock);
        /* UNLOCK */
}

static int print_job_info(char* buf, int buf_size, struct aipu_job* job)
{
        int ret = 0;
        char state_str[20];
        char excep_str[10];

        if ((!buf) || (!job))
                return ret;

        if (job->state == AIPU_JOB_STATE_PENDING)
                snprintf(state_str, 20, "Pending");
        else if (job->state == AIPU_JOB_STATE_SCHED)
                snprintf(state_str, 20, "Executing");
        else if (job->state == AIPU_JOB_STATE_END)
                snprintf(state_str, 20, "Done");

        if (job->exception_flag)
                snprintf(excep_str, 10, "Y");
        else
                snprintf(excep_str, 10, "N");

        return snprintf(buf, buf_size, "%-*d0x%-*x%-*s%-*s\n", 12, job->uthread_id, 10,
                job->desc.job_id, 10, state_str, 5, excep_str);
}

int aipu_job_manager_sysfs_job_show(struct aipu_job_manager *job_manager, char* buf)
{
        int ret = 0;
        int tmp_size = 1024;
        char tmp[tmp_size];
        struct aipu_job* curr = NULL;
        int number = 0;

        if (!buf)
                return ret;

        ret += snprintf(tmp, 1024, "-------------------------------------------\n");
        strcat(buf, tmp);
        ret += snprintf(tmp, 1024, "%-*s%-*s%-*s%-*s\n", 12, "Thread ID", 12, "Job ID",
                10, "State", 5, "Exception");
        strcat(buf, tmp);
        ret += snprintf(tmp, 1024, "-------------------------------------------\n");
        strcat(buf, tmp);

        /* LOCK */
        spin_lock_irq(&job_manager->lock);
        list_for_each_entry(curr, &job_manager->pending_queue_head->node, node) {
                ret += print_job_info(tmp, tmp_size, curr);
                strcat(buf, tmp);
                number++;
        }
        curr = NULL;
        list_for_each_entry(curr, &job_manager->scheduled_queue_head->node, node) {
                ret += print_job_info(tmp, tmp_size, curr);
                strcat(buf, tmp);
                number++;
        }
        spin_unlock_irq(&job_manager->lock);
        /* UNLOCK */

        if (!number) {
                ret += snprintf(tmp, tmp_size, "No job.\n");
                strcat(buf, tmp);
        }

        ret += snprintf(tmp, 1024, "-------------------------------------------\n");
        strcat(buf, tmp);

        return ret;
}
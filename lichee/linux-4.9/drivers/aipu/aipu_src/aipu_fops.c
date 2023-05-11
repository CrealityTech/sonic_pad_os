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
 * @file aipu_fops.c
 * Implementations of KMD file operation API
 */

#include <linux/fs.h>
#include <linux/mm_types.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/poll.h>
#include "uk_interface/aipu_ioctl.h"
#include "uk_interface/aipu_capability.h"
#include "uk_interface/aipu_buf_req.h"
#include "uk_interface/aipu_job_desc.h"
#include "uk_interface/aipu_profiling.h"
#include "uk_interface/aipu_io_req.h"
#include "uk_interface/aipu_errcode.h"
#include "uk_interface/aipu_job_status.h"
#include "aipu_mm.h"
#include "aipu_job_manager.h"
#include "aipu_session.h"
#include "aipu.h"

static int aipu_open(struct inode *inode, struct file *filp)
{
        int ret = 0;
        struct aipu_priv *aipu = NULL;
        struct aipu_session *session  = NULL;
        int pid = task_pid_nr(current);

        aipu = container_of(filp->f_op, struct aipu_priv, aipu_fops);

        ret = aipu_create_session(pid, aipu, &session);
        if (ret) {
                return ret;
        } else {
                filp->private_data = session;
                filp->f_pos = 0;
        }

        /* success */
        return ret;
}

static int aipu_release(struct inode *inode, struct file *filp)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        struct aipu_priv *aipu = NULL;
        struct aipu_session *session = filp->private_data;

        if (!session) {
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
                goto finish;
        }

        aipu = container_of(filp->f_op, struct aipu_priv, aipu_fops);

        /* jobs should be cleared prior to buffer free */
        ret = aipu_job_manager_cancel_session_jobs(&aipu->job_manager, session);
        if (AIPU_ERRCODE_NO_ERROR != ret)
                goto err_handle;

        ret = aipu_mm_free_session_buffers(&aipu->mm, session);
        if (AIPU_ERRCODE_NO_ERROR != ret)
                goto err_handle;

        ret = aipu_destroy_session(session);
        if (AIPU_ERRCODE_NO_ERROR != ret)
                goto err_handle;

        /* success */
        goto finish;

err_handle:
        /* no handler */
finish:
        return ret;
}

static long aipu_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        int cp_ret = AIPU_ERRCODE_NO_ERROR;
        struct aipu_session *session = filp->private_data;
        struct aipu_priv *aipu = NULL;

        struct aipu_cap cap;
        struct buf_request buf_req;
        struct aipu_buffer buf;
        struct user_job user_job;
        struct session_job *kern_job = NULL;
        struct buf_desc desc;
        struct aipu_io_req io_req;
        struct job_status_query job;
        u32 job_id;

        if (!session) {
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
                goto finish;
        }

        aipu = container_of(filp->f_op, struct aipu_priv, aipu_fops);

        switch (cmd)
        {
        case IPUIOC_QUERYCAP:
                ret = copy_from_user(&cap, (struct aipu_cap __user*)arg, sizeof(struct aipu_cap));
                if (AIPU_ERRCODE_NO_ERROR != ret)
                        dev_err(aipu->dev, "KMD ioctl: QUERYCAP copy from user failed!");
                else {
                        aipu_priv_query_capability(aipu, &cap);
                        /* copy cap info/errcode to user for reference */
                        cp_ret = copy_to_user((struct aipu_cap __user*)arg, &cap, sizeof(struct aipu_cap));
                        if ((AIPU_ERRCODE_NO_ERROR == ret) && (AIPU_ERRCODE_NO_ERROR != cp_ret))
                                ret = cp_ret;
                }
                break;
        case IPUIOC_REQBUF:
                ret = copy_from_user(&buf_req, (struct buf_request __user*)arg, sizeof(struct buf_request));
                if (AIPU_ERRCODE_NO_ERROR != ret)
                        dev_err(aipu->dev, "KMD ioctl: REQBUF copy from user failed!");
                else {
                        ret = aipu_mm_alloc(&aipu->mm, &buf_req, &buf);
                        if (AIPU_ERRCODE_NO_ERROR == ret) {
                                ret = aipu_session_add_buf(session, &buf_req, &buf);
                                if (AIPU_ERRCODE_NO_ERROR != ret)
                                        dev_err(aipu->dev, "KMD ioctl: add buf failed!");
                        }

                        /* copy buf info/errcode to user for reference */
                        cp_ret = copy_to_user((struct buf_request __user*)arg, &buf_req, sizeof(struct buf_request));
                        if ((AIPU_ERRCODE_NO_ERROR == ret) && (AIPU_ERRCODE_NO_ERROR != cp_ret))
                                ret = cp_ret;
                }
                break;
        case IPUIOC_RUNJOB:
                ret = copy_from_user(&user_job, (struct user_job __user*)arg, sizeof(struct user_job));
                if (AIPU_ERRCODE_NO_ERROR != ret)
                        dev_err(aipu->dev, "KMD ioctl: RUNJOB copy from user failed!");
                else {
                        kern_job = aipu_session_add_job(session, &user_job);
                        if (NULL == kern_job)
                                dev_err(aipu->dev, "KMD ioctl: RUNJOB add failed!");
                        else {
                                ret = aipu_job_manager_schedule_new_job(&aipu->job_manager, &user_job, kern_job,
                                        session);
                                if (AIPU_ERRCODE_NO_ERROR != ret)
                                        dev_err(aipu->dev, "KMD ioctl: RUNJOB run failed!");
                        }

                        /* copy job errcode to user for reference */
                        cp_ret = copy_to_user((struct user_job __user*)arg, &user_job, sizeof(struct user_job));
                        if ((AIPU_ERRCODE_NO_ERROR == ret) && (AIPU_ERRCODE_NO_ERROR != cp_ret))
                                ret = cp_ret;
                }
                break;
        case IPUIOC_KILL_TIMEOUT_JOB:
                ret = copy_from_user(&job_id, (u32 __user*)arg, sizeof(__u32));
                if (AIPU_ERRCODE_NO_ERROR != ret)
                        dev_err(aipu->dev, "KMD ioctl: KILL_TIMEOUT_JOB copy from user failed!");
                else
                        ret = aipu_invalidate_timeout_job(&aipu->job_manager, job_id);
                break;
        case IPUIOC_FREEBUF:
                ret = copy_from_user(&desc, (struct buf_desc __user*)arg, sizeof(struct buf_desc));
                if (AIPU_ERRCODE_NO_ERROR != ret)
                        dev_err(aipu->dev, "KMD ioctl: FREEBUF copy from user failed!");
                else {
                        /* detach first to validate the free buf request */
                        ret = aipu_session_detach_buf(session, &desc);
                        if (AIPU_ERRCODE_NO_ERROR != ret)
                                dev_err(aipu->dev, "KMD ioctl: detach session buffer failed!");
                        else {
                                /* do free operation */
                                ret = aipu_mm_free(&aipu->mm, &desc);
                                if (AIPU_ERRCODE_NO_ERROR != ret)
                                        dev_err(aipu->dev, "KMD ioctl: free buf failed!");
                        }
                }
                break;
        case IPUIOC_REQSHMMAP:
                ret = copy_to_user((unsigned long __user*)arg, &aipu->host_to_aipu_map, sizeof(u64));
                break;
        case IPUIOC_REQIO:
                ret = copy_from_user(&io_req, (struct aipu_io_req __user*)arg, sizeof(struct aipu_io_req));
                if (AIPU_ERRCODE_NO_ERROR != ret)
                        dev_err(aipu->dev, "KMD ioctl: REQIO copy from user failed!");
                else {
                        aipu_priv_io_rw(aipu, &io_req);
                        ret = copy_to_user((struct aipu_io_req __user*)arg, &io_req, sizeof(struct aipu_io_req));
                }
                break;
        case IPUIOC_QUERYSTATUS:
                ret = copy_from_user(&job, (struct job_status_query __user*)arg,
                    sizeof(struct job_status_query));
                if (AIPU_ERRCODE_NO_ERROR != ret)
                        dev_err(aipu->dev, "KMD ioctl: QUERYSTATUS copy from user failed!");
                else {
                        ret = aipu_session_get_job_status(session, &job);
                        if (AIPU_ERRCODE_NO_ERROR == ret)
                                ret = copy_to_user((struct job_status_query __user*)arg, &job,
                                    sizeof(struct job_status_query));
                }
                break;
        default:
                dev_err(aipu->dev, "no matching ioctl call (cmd = 0x%lx)!", (unsigned long)cmd);
                ret = -ENOTTY;
                break;
        }

finish:
        return ret;
}

static int aipu_mmap(struct file *filp, struct vm_area_struct *vma)
{
        int ret = AIPU_ERRCODE_NO_ERROR;
        struct aipu_priv *aipu = NULL;
        struct aipu_session *session = filp->private_data;
        if (!session) {
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
                goto finish;
        }

        aipu = container_of(filp->f_op, struct aipu_priv, aipu_fops);
        ret = aipu_session_mmap_buf(session, vma, aipu->dev);
        if (AIPU_ERRCODE_NO_ERROR != ret)
                dev_err(aipu->dev, "mmap to userspace failed!");

finish:
        return ret;
}

static unsigned int aipu_poll(struct file *filp, struct poll_table_struct *wait)
{
        unsigned int mask = 0;
        struct aipu_session *session = filp->private_data;
        int tid = task_pid_nr(current);

        if (!session)
                goto finish;

        aipu_session_add_poll_wait_queue(session, filp, wait, tid);

        if (aipu_session_thread_has_end_job(session, tid))
        {
                mask |= POLLIN | POLLRDNORM;
        }

finish:
        return mask;
}

int aipu_fops_register(struct file_operations *fops)
{
        int ret = AIPU_ERRCODE_NO_ERROR;

        if (!fops) {
                ret = map_errcode(AIPU_ERRCODE_INTERNAL_NULLPTR);
                goto err_handle;
        }

        fops->owner = THIS_MODULE;
        fops->open = aipu_open;
        fops->poll = aipu_poll;
        fops->unlocked_ioctl = aipu_ioctl;
#ifdef CONFIG_COMPAT
        fops->compat_ioctl = aipu_ioctl;
#endif
        fops->mmap = aipu_mmap;
        fops->release = aipu_release;

        /* success */
        goto finish;

err_handle:
        /* no handler */
finish:
        return ret;
}

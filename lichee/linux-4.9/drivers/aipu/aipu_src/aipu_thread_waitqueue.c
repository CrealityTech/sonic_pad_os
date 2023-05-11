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
 * @file aipu_thread_waitqueue.c
 * Thread waitqueue module implementation file
 */

#include <linux/slab.h>
#include "aipu_thread_waitqueue.h"

static struct aipu_thread_wait_queue *do_create_thread_wait_queue(int uthread_id)
{
        struct aipu_thread_wait_queue *new_wait_queue =
                kzalloc(sizeof(struct aipu_thread_wait_queue), GFP_KERNEL);
        new_wait_queue->ref_cnt = 0;
        new_wait_queue->uthread_id = uthread_id;
        init_waitqueue_head(&new_wait_queue->p_wait);
        INIT_LIST_HEAD(&new_wait_queue->node);
        return new_wait_queue;
}

struct aipu_thread_wait_queue* get_thread_wait_queue_no_lock(struct aipu_thread_wait_queue *head,
        int uthread_id)
{
        struct aipu_thread_wait_queue *curr = NULL;

        if (!head)
                return NULL;

        list_for_each_entry(curr, &head->node, node) {
                if (curr->uthread_id == uthread_id)
                        return curr;
        }
        return NULL;
}

struct aipu_thread_wait_queue *create_thread_wait_queue_no_lock(struct aipu_thread_wait_queue *head,
        int uthread_id)
{
        struct aipu_thread_wait_queue *ret = NULL;
        struct aipu_thread_wait_queue *queue = get_thread_wait_queue_no_lock(head, uthread_id);

        if (!queue) {
                queue = do_create_thread_wait_queue(uthread_id);
                if (queue && head) {
                        list_add_tail(&queue->node, &head->node);
                }
                ret = queue;
        }

        queue->ref_cnt++;
        return ret;
}

void ck_delete_thread_wait_queue_no_lock(struct aipu_thread_wait_queue *wait_queue)
{
        if (wait_queue && (!wait_queue->ref_cnt)) {
                list_del(&wait_queue->node);
                kfree(wait_queue);
        }
}

void delete_wait_queue(struct aipu_thread_wait_queue *wait_queue_head)
{
        struct aipu_thread_wait_queue *cursor = NULL;
        struct aipu_thread_wait_queue *next = NULL;

        if (wait_queue_head) {
                list_for_each_entry_safe(cursor, next, &wait_queue_head->node, node) {
                        list_del(&cursor->node);
                        kfree(cursor);
                }
        }
}
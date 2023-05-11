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
 * @file aipu_thread_waitqueue.h
 * Thread waitqueue module header file
 */

#ifndef _AIPU_THREAD_WAITQUEUE_H_
#define _AIPU_THREAD_WAITQUEUE_H_

#include <linux/list.h>
#include <linux/wait.h>

/**
 * struct waitqueue: maintain the waitqueue for a user thread
 *
 * @uthread_id: user thread owns this waitqueue
 * @ref_cnt: strucr reference count
 * @p_wait: wait queue head for polling
 * @node: list head struct
 */
struct aipu_thread_wait_queue {
        int uthread_id;
        int ref_cnt;
        wait_queue_head_t p_wait;
        struct list_head node;
};

/*
 * @brief get requested waitqueue for a user thread
 *
 * @param head: wait queue head
 * @uthread_id: user thread ID
 *
 * @return waitqueue pointer; NULL if not found;
 */
struct aipu_thread_wait_queue *get_thread_wait_queue_no_lock(struct aipu_thread_wait_queue *head,
    int uthread_id);
/*
 * @brief create a new waitqueue for a user thread if there is no existing one
 *
 * @param head: wait queue head
 * @uthread_id: user thread ID
 *
 * @return waitqueue pointer if not existing one; NULL if there has been an existing one;
 */
struct aipu_thread_wait_queue *create_thread_wait_queue_no_lock(struct aipu_thread_wait_queue *head,
    int uthread_id);
/*
 * @brief check if a waitqueue can be deleted and delete it if it is OK
 *
 * @param head: wait queue head
 *
 */
void ck_delete_thread_wait_queue_no_lock(struct aipu_thread_wait_queue *wait_queue);
/*
 * @brief delete a waitqueue list
 *
 * @param head: wait queue head
 *
 */
void delete_wait_queue(struct aipu_thread_wait_queue *wait_queue_head);

#endif /* _AIPU_THREAD_WAITQUEUE_H_ */
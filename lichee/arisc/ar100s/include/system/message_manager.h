/**
 * include\system\message_manager.h
 *
 * Descript: message manager public header.
 * Copyright (C) 2012-2016 AllWinnertech Ltd.
 * Author: Sunny <Sunny@allwinnertech.com>
 *
 */

#ifndef __MESSAGE_MANAGER_H__
#define __MESSAGE_MANAGER_H__

/*
*********************************************************************************************************
*                                       INIT MESSAGE MANAGER
*
* Description:  initialize message manager.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize succeeded, others if failed.
*********************************************************************************************************
*/
s32 message_manager_init(void);

/*
*********************************************************************************************************
*                                       EXIT MESSAGE MANAGER
*
* Description:  exit message manager.
*
* Arguments  :  none.
*
* Returns    :  OK if exit succeeded, others if failed.
*********************************************************************************************************
*/
s32 message_manager_exit(void);

/*
*********************************************************************************************************
*                                       ALLOCATE MESSAGE FRAME
*
* Description:  allocate one message frame. mainly use for send message by message-box,
*               the message frame allocate form messages pool shared memory area.
*
* Arguments  :  none.
*
* Returns    :  the pointer of allocated message frame, NULL if failed;
*********************************************************************************************************
*/
struct message *message_allocate(void);

/*
*********************************************************************************************************
*                                       FREE MESSAGE FRAME
*
* Description:  free one message frame. mainly use for process message finished,
*               free it to messages pool or add to free message queue.
*
* Arguments  :  pmessage : the pointer of free message frame.
*
* Returns    :  none.
*********************************************************************************************************
*/
void message_free(struct message *pmessage);

/*
*********************************************************************************************************
*                                       QUERY MESSAGE QUEUE
*
* Description:  query the message number of work queue.
*
* Arguments  :  none.
*
* Returns    :  the message number of work queue;
*********************************************************************************************************
*/
s32 message_work_queue_query(void);

/*
*********************************************************************************************************
*                                       PROCESS MESSAGE QUEUE
*
* Description:  process current messages work queue.
*
* Arguments  :  none.
*
* Returns    :  OK if process messages succeed, others if failed.
*********************************************************************************************************
*/
s32 message_work_queue_process(void);

/*
*********************************************************************************************************
*                                       NOTIFY MESSAGE COMING
*
* Description:  notify system that one message coming.
*
* Arguments  :  pmessage : the pointer of coming message frame.
*
* Returns    :  OK if notify succeeded, other if failed.
*********************************************************************************************************
*/
s32 message_coming_notify(struct message *pmessage);

int message_valid(struct message *pmessage);

#endif /* __MESSAGE_MANAGER_H__ */

/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                             Notifier Manager
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : notifier.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-11-23
* Descript: notifier manager.
* Update  : date                auther      ver     notes
*           2012-11-23 14:06:58 Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "ibase.h"
#include "driver.h"
#include "notifier.h"

//global notifiers table
static struct notifier notifiers[NOTIFIER_MAX];

s32 notifier_init(void)
{
	u32 i;

	//initialize notifiers as free status
	for (i = 0; i < NOTIFIER_MAX; i++)
	{
		notifiers[i].flag = NOTIFIER_FREE;
	}
	return OK;
}

s32 notifier_exit(void)
{
	return OK;
}

static struct notifier *notifier_alloc(void)
{
	u32              i;
	struct notifier *notifier = NULL;

	//allocate notifier from global table
	s32 cpsr = cpu_disable_int();
	for (i = 0; i < NOTIFIER_MAX; i++)
	{
		if (notifiers[i].flag == NOTIFIER_FREE)
		{
			notifiers[i].flag = NOTIFIER_USED;
			notifier = &(notifiers[i]);
			break;
		}
	}
	cpu_enable_int(cpsr);
	return notifier;
}

static void notifier_free(struct notifier *notifier)
{
	if (notifier)
	{
		notifier->flag = NOTIFIER_FREE;
		notifier->next = NULL;
	}
}

s32 notifier_insert(struct notifier **head, __pNotifier_t pcb, void *parg)
{
	struct notifier *notifier;

	ASSERT(pcb != NULL);

	//allocate a notifier
	notifier = notifier_alloc();
	if (notifier == NULL)
	{
		WRN("allocate notifier failed\n");
		return -ENOSPC;
	}

	//add notifier to head of notifier list
	notifier->pcb  = pcb;
	notifier->next = *head;
	notifier->parg = parg;
	*head = notifier;

	return OK;
}

s32 notifier_delete(struct notifier **head, __pNotifier_t pcb)
{
	struct notifier *target = NULL;
	struct notifier *pnext;
	struct notifier *pprev;

	ASSERT(pcb != NULL);

	if (*head == NULL)
	{
		//notifier list is empty, you can't delete anything
		return -ENODEV;
	}
	//search the target notifier
	pprev = *head;
	if (pprev->next == NULL)
	{
		//only one notifier in list
		if (pprev->pcb == pcb)
		{
			//find target, free this notifier and clear notifier list.
			*head = NULL;
			target = pprev;
		}
	}
	else
	{
		pnext = pprev->next;
		while (pnext)
		{
			if (pnext->pcb == pcb)
			{
				//find target notifier.
				pprev->next = pnext->next;
				target = pnext;
				break;
			}
			pprev = pnext;
			pnext = pnext->next;
		}
	}
	if (target == NULL)
	{
		return -EFAIL;
	}
	//free target notifier
	notifier_free(target);

	return OK;
}

s32 notifier_notify(struct notifier **head, u32 command)
{
	struct notifier *notifier;

	//broadcast message
	notifier = *head;
	while (notifier)
	{
		notifier->pcb(command, notifier->parg);

		//next notifier
		notifier = notifier->next;
	}
	return OK;
}

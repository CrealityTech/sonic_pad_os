#include "includes.h"
#include "device.h"
#include "ioctl.h"
#include "timer.h"
#include "task.h"
#include "bytbmsg.h"
#include "loopback.h"
#include "msgmanager.h"
#include "task_handle.h"


__TASK_INITIAL(loop_back_task,DEFAULT_TASK_PRIORITY,0,CPUS_LOOP_BACK);
char test[100];

static s32 loop_back_task_handle(task_struct *task)
{
	char *p;
	struct msgblock_head *head;

	p = &test[0];

	head = container_of(task->msgparalist,struct msgblock_head,list);

	bytb_get_paras_form_msg(task->msgparalist,(void **)&p,head->length);

	printf("%s\n",test);

	return OK;
}



void loop_back_init(void)
{
	create_task((u32)(&loop_back_task),(u32)loop_back_task_handle);
}



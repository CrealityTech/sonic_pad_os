#include "ibase.h"
#include "task.h"
#include "task_handle.h"
#include "softtimer.h"
#include "msgmanager.h"

__TASK_INITIAL(sysdebug_task,DEFAULT_TASK_PRIORITY,TASK_GID_SYSBEBUG,DEFAULT_TASK_HANDLE);
struct softtimer sysdebug_timer;
int sysdebug_init(void);

static const char *debug_string = "this is msg for system debug\n";

static s32 sysdebug(task_struct *task)
{
	struct msgblock_head *msgh;
	u32 paras_length;

	printk("task ref count %d\n",task->ref_count);

	paras_length = strlen(debug_string);
	LOG("system debug paras_length %d\n",paras_length);
	msgh = allock_msg(get_block_length_from_paras(paras_length));

	msgh->type = CPUS_SYSTEM_DEBUG;
	msgh->attr = MESSAGE_ATTR_SYN;
	msgh->state = MSG_STATE_ALLOCKED;
	msgh->length = paras_length;

	set_paras_to_msg(&msgh->list,(void *)SUB_VER,paras_length);

	LOG("msgh head addr %x\n",msgh);
	hexdump("msgpool",(char *)(msgh),0x100);

	return OK;
}

int sysdebug_init(void)
{

	add_task(&sysdebug_task,(__taskCBK_t)sysdebug);

	sysdebug_timer.ticks = mecs_to_ticks(2000);
	sysdebug_timer.expires = 0;
	sysdebug_timer.task = &sysdebug_task;
	sysdebug_timer.arg = NULL;

	add_softtimer(&sysdebug_timer);
	start_softtimer(&sysdebug_timer,SOFTTIMER_SINGLE);

	return OK;
}




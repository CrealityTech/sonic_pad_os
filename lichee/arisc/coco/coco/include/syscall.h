#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#define MSG_OPS			0x01000000
#define MSG_ALLOCK		0x01000001
#define MSG_FREE		0x01000002
#define MSG_SETPARAS		0x01000003
#define MSG_GETPARAS		0x01000004
#define MSG_RESOLVE		0x01000005
#define FEEDBACK_RESOLVE	0x01000006


#define TASK_OPS	0x02000000
#define TASK_CREATE	0x02000001
#define TASK_SLEEP	0x02000002
#define TASK_WAKEUP	0x02000003
#define TASK_INMSG	0x02000004

#define SYSTEM_OPS	0x03000000
#define SYSTEM_MDELAY	0x03000001

#define SYSTEM_MS2TICKS	0x03000002

void simsyscall_init(void);
void simsyscall_suspend(void);
void simsyscall_resume(void);


#endif


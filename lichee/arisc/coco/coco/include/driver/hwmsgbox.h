#ifndef __HWMSGBOX_H__
#define	__HWMSGBOX_H__

#include "ibase.h"
#include "driver.h"

#define CPUX_TRANSMIT		1
#define COCO_RECEPTION		0
#define CPUX_RECEPTION		1
#define COCO_TRANSMIT		0


#define COCO_SEND_NUMBER	1
#define COCO_FB_NUMBER		1
#define CPUX_SEND_NUMBER	1
#define CPUX_FB_NUMBER		1
#define BYTBCALL_NUMBER		1

#define HWMSG_RESOLVE		0x1
#define HWMSG_FEEDBACK		0x2
#define HWMSG_SEND		0x3

struct msg_queue {
	u32 channel_id;
	u32 channel_addr;
	u32 channel_dir;
	u32 irq_status;
	u32 fifo_status;
};

typedef struct {
	arisc_device dev;
	arisc_driver *dri;
	const struct msgbox_ops *ops;
	struct msg_queue coco_send[COCO_SEND_NUMBER];
	struct msg_queue coco_feedback[COCO_FB_NUMBER];
	struct msg_queue cpux_send[CPUX_SEND_NUMBER];
	struct msg_queue cpux_feedback[CPUX_FB_NUMBER];
	struct msg_queue cpus_simsyscall[BYTBCALL_NUMBER];
} msgbox_device;

void msgbox_set_direction(struct msg_queue *queue, u32 qdir);
s32 msgbox_feedback(struct msg_queue *queue);
s32 msgbox_send(struct msg_queue *queue,u32 addr);
s32 msgbox_query(struct msg_queue *queue);
int msgbox_init(void);




#endif


/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : hwmsgbox.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:31
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "hwmsgbox_i.h"
#include "hwmsgbox.h"

msgbox_device msgbox_dev;

static s32 msgbox_handler(void *parg, u32 intno)
{
	s32 msg_num;
	s32 ret;
	u32 msg_offset;
	msgbox_device *mdev = (msgbox_device *)parg;

	intno = intno;

	msg_num = MSGBOX_GetMsgStatus(mdev->cpux_send[0].channel_id);
	if(msg_num > 0)
		while(msg_num--) {
			msg_offset = MSGBOX_ReadMsg(mdev->cpux_send[0].channel_id);
			ret = resolve_msg(MSGPOOL_START + msg_offset);
		}

	MSGBOX_ClearPending(mdev->cpux_send[0].channel_id);

	msg_num = MSGBOX_GetMsgStatus(mdev->cpux_feedback[0].channel_id);
	if(msg_num > 0)
		while(msg_num--)
			resolve_feedback(MSGPOOL_START + MSGBOX_ReadMsg(mdev->cpux_feedback[0].channel_id));

	MSGBOX_ClearPending(mdev->cpux_feedback[0].channel_id);

	return 0;
}

msgbox_device *request_msgbox(void)
{
	return &msgbox_dev;
}

void msgbox_set_direction(struct msg_queue *queue, u32 qdir)
{
	u32 transmit = (qdir > 1) & 0x1;
	u32 receive = qdir & 0x1;

	MSGBOX_SetTransmitUser(queue->channel_id,transmit);
	MSGBOX_SetReceptionUser(queue->channel_id,receive);
}

s32 msgbox_feedback(struct msg_queue *queue)
{
	queue = queue;

	return 0;
}

s32 msgbox_send(struct msg_queue *queue,u32 addr)
{

	if(MSGBOX_GetFIFOStatus(queue->channel_id))
		return FAIL;

	MSGBOX_WriteMsg(queue->channel_id,(u32)(addr - MSGPOOL_START));

	return 0;
}

s32 msgbox_query(struct msg_queue *queue)
{
	while(!MSGBOX_GetMsgStatus(queue->channel_id))
		;

	MSGBOX_ClearPending(queue->channel_id);
	return ((s32)MSGBOX_ReadMsg(queue->channel_id) + MSGPOOL_START);
}

static s32 msgbox_suspend(arisc_device *dev)
{
	dev = dev;

	ccmu_reset_clk_mask(get_ccmu_device(),MSGBOX_BUS_GATING_REG);

	return OK;
}

static s32 msgbox_resume(arisc_device *dev)
{
	u32 i = 0;
	u32 __channel_id = 0;

	dev = dev;

	ccmu_reset_clk(get_ccmu_device(),MSGBOX_BUS_GATING_REG);

	for(i = 0; i < COCO_SEND_NUMBER; i++) {
		msgbox_dev.coco_send[i].channel_id = __channel_id++;
		msgbox_set_direction(&(msgbox_dev.coco_send[i]),CPUs2CPUx);
	}

	for(i = 0; i < COCO_FB_NUMBER; i++) {
		msgbox_dev.coco_feedback[i].channel_id = __channel_id++;
		msgbox_set_direction(&(msgbox_dev.coco_feedback[i]),CPUs2CPUx);
	}

	for(i = 0; i < CPUX_SEND_NUMBER; i++) {
		msgbox_dev.cpux_send[i].channel_id = __channel_id++;
		msgbox_set_direction(&(msgbox_dev.cpux_send[i]),CPUx2CPUs);
		MSGBOX_SetReceptionIRQ(msgbox_dev.cpux_send[i].channel_id,1);
	}

	for(i = 0; i < CPUX_FB_NUMBER; i++) {
		msgbox_dev.cpux_feedback[i].channel_id = __channel_id++;
		msgbox_set_direction(&(msgbox_dev.cpux_feedback[i]),CPUx2CPUs);
		MSGBOX_SetReceptionIRQ(msgbox_dev.cpux_feedback[i].channel_id,1);
	}

	return OK;
}

static int msgbox_ioctl(u32 request,va_list args)
{
	u32 value = 0;
	u32 queue;

	switch (request) {
		case HWMSG_RESOLVE:
			value = va_arg(args, unsigned int);
			resolve_msg(value);
			break;
		case HWMSG_FEEDBACK:
			value = va_arg(args, unsigned int);
			msgbox_feedback((struct msg_queue *)value);
			break;
		case HWMSG_SEND:
			queue = va_arg(args, unsigned int);
			value = va_arg(args, unsigned int);
			msgbox_send((struct msg_queue *)queue,value);
			break;
		default:
			break;
	}

	return OK;
}

arisc_driver msgbox_dri = {
	.suspend = msgbox_suspend,
	.resume  = msgbox_resume,
	.iorequset = msgbox_ioctl,
};

int msgbox_init(void)
{
	u32 i = 0;
	u32 __channel_id = 0;

	ccmu_reset_clk(get_ccmu_device(),MSGBOX_BUS_GATING_REG);

	msgbox_dev.dev.hw_ver = 0x10000;
	msgbox_dev.dev.reg_base = HWMSGBOX_REG_BASE;
	msgbox_dev.dev.dev_lock = 1;
	msgbox_dev.dev.dev_id = HWMSGBOX_DEVICE_ID;
	msgbox_dev.dev.irq_no = INTC_R_M_BOX_IRQ;

	msgbox_dev.dev.dri = &msgbox_dri;

	for(i = 0; i < COCO_SEND_NUMBER; i++) {
		msgbox_dev.coco_send[i].channel_id = __channel_id++;
		msgbox_set_direction(&(msgbox_dev.coco_send[i]),CPUs2CPUx);
	}

	for(i = 0; i < COCO_FB_NUMBER; i++) {
		msgbox_dev.coco_feedback[i].channel_id = __channel_id++;
		msgbox_set_direction(&(msgbox_dev.coco_feedback[i]),CPUs2CPUx);
	}

	for(i = 0; i < CPUX_SEND_NUMBER; i++) {
		msgbox_dev.cpux_send[i].channel_id = __channel_id++;
		msgbox_set_direction(&(msgbox_dev.cpux_send[i]),CPUx2CPUs);
		MSGBOX_SetReceptionIRQ(msgbox_dev.cpux_send[i].channel_id,1);
	}

	for(i = 0; i < CPUX_FB_NUMBER; i++) {
		msgbox_dev.cpux_feedback[i].channel_id = __channel_id++;
		msgbox_set_direction(&(msgbox_dev.cpux_feedback[i]),CPUx2CPUs);
		MSGBOX_SetReceptionIRQ(msgbox_dev.cpux_feedback[i].channel_id,1);
	}


	install_isr(msgbox_dev.dev.irq_no, (__pISR_t)msgbox_handler, (void *)(&msgbox_dev));

	list_add_tail(&msgbox_dev.dev.list,&(dev_list));

	return 0;
}


#ifndef __HWMSGBOX_LIB_H__
#define __HWMSGBOX_LIB_H__


#define DEBUG_MODE	1
#define NORMAL_MODE	0

#define TRANSMIT	1
#define RECEPTION	0

#define MSGBOX_CTRL_REG0		0x0000
#define MSGBOX_CTRL_REG1		0x0004
#define MSGBOX_VER_REG			0x0010
#define MSGBOXU_IRQ_EN_REG		0x0040
#define MSGBOXU_IRQ_STATUS_REG		0x0050
#define MSGBOXM_FIFO_STATUS_REG		0x0100
#define MSGBOXM_MSG_STATUS_REG		0x0140
#define MSGBOXM_MSG_REG			0x0180
#define MSGBOX_DEBUG_REG		0x01c0


void MSGBOX_SetTransmitUser(u32 queue, u32 user);
void MSGBOX_SetReceptionUser(u32 queue, u32 user);
s32 MSGBOX_SetTransmitIRQ(u32 queue,u32 status);
s32 MSGBOX_SetReceptionIRQ(u32 queue,u32 status);
u32 MSGBOX_GetIRQStatus(u32 queue, u32 dir);
void MSGBOX_ClearPending(u32 queue);
void MSGBOX_ClearTranPending(u32 queue);
u32 MSGBOX_GetFIFOStatus(u32 queue);
s32 MSGBOX_GetMsgStatus(u32 queue);
void MSGBOX_WriteMsg(u32 queue, u32 data);
u32 MSGBOX_ReadMsg(u32 queue);
void MSGBOX_Debug(u32 state);










#endif




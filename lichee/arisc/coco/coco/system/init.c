/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*)                                             All Rights Reserved
*
* File    : init.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-04-10 14:53
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "ibase.h"
#include "driver.h"
#include "task.h"
#include "notifier.h"
#include "msgmanager.h"
#include "psci.h"
#include "task_handle.h"
#include "sysdebug.h"
#include "ioctl.h"
#include "syscall.h"
#include "head_para.h"
#include "cpucfg.h"
#include "dvfs_service.h"
#include "rsb_service.h"

extern int idle_loop(void);
void startup_entry(void);
void cpux_notify(void);
extern char paras_start_addr, paras_end_addr;

#define PARAS_START_ADDR  (char  *)(&paras_start_addr)
#define PARAS_END_ADDR  (char  *)(&paras_end_addr)

struct arisc_para arisc_para __attribute__ ((section("dts_paras"))) = {
	.para_info	= 4,
	.para_magic	= 0x73555043,
};



void cpux_notify(void)
{
	struct msgblock_head *msgh;
	msgbox_device *dev;
	msgbox_device *msgdev;
	u32 paras_length;
	s32 ret;

	paras_length = sizeof(SUB_VER);
	msgh = allock_msg(get_block_length_from_paras(paras_length));

	msgh->type = CPUS_STARTUP_NOTIFY;
	msgh->attr = MESSAGE_ATTR_SYN;
	msgh->state = MSG_STATE_ALLOCKED;
	msgh->length = paras_length;

	set_paras_to_msg(&msgh->list,(void *)SUB_VER,paras_length);
	dev = request_msgbox();
	ret = msgbox_send(&(dev->coco_send[0]),(u32)msgh);

	if(ret == OK)
		LOG("send notify succeed\n");
	else
		LOG("send notify failed\n");

	if((u32)msgh !=  (u32)msgbox_query(&(dev->cpux_feedback[0]))) {
		ERR("CPUx feedback failed , system die\n");
		while(1);
	} else
		LOG("CPUx notify feedback succeed\n");

	free_msg(msgh);

	msgdev = request_msgbox();
	interrupt_enable(msgdev->dev.irq_no);
}



static void arisc_para_init(void)
{
	cpucfg_set_little_endian_address((void *)PARAS_START_ADDR, (void *)PARAS_END_ADDR);
	cpucfg_set_little_endian_address((void *)arisc_para.standby_base, (void *)arisc_para.standby_base+arisc_para.standby_size);
	//copy arisc initialize parameters from cpux system
}

int arisc_para_get_uart_pin_info(void)
{
	return arisc_para.suart_status;
}


#define MAX_SPRS_PER_GRP_BITS   (11)
#define SPRGROUP_SYS            (0<< MAX_SPRS_PER_GRP_BITS)
#define SPR_SR                  (SPRGROUP_SYS + 17)


static void service_init(void)
{
	psci_suspend_core_init();
	rsb_service_init();
}

/*
*********************************************************************************************************
*                                       STARTUP ENTRY
*
* Description:  the entry of startup.
*
* Arguments  :  none.
*
* Returns    :  none.
*********************************************************************************************************
*/

void startup_entry(void)
{
	//initialize notifier manager
	save_state_flag(REC_HOTPULG | 0);
	notifier_init();
	save_state_flag(REC_HOTPULG | 1);

	ccmu_init();
	save_state_flag(REC_HOTPULG | 2);

	prcm_init();
	save_state_flag(REC_HOTPULG | 3);

	interrupt_init();
	save_state_flag(REC_HOTPULG | 4);

	cpu_init();
	save_state_flag(REC_HOTPULG | 5);

	arisc_para_init();

	pinl_init();
	save_state_flag(REC_HOTPULG | 6);

#ifdef CFG_PIN_M_USED
	pinm_init();
#endif
	save_state_flag(REC_HOTPULG | 7);

	task_init();
	save_state_flag(REC_HOTPULG | 8);

	uart_init();
	save_state_flag(REC_HOTPULG | 9);

	debugger_init();
	save_state_flag(REC_HOTPULG | 0xa);
	LOG("debugger system ok\n");

	timer_init();
	save_state_flag(REC_HOTPULG | 0xb);

	ticktimer_init();
	save_state_flag(REC_HOTPULG | 0xc);

	delaytimer_init();
	save_state_flag(REC_HOTPULG | 0xd);

	interrupt_notify_init();
	save_state_flag(REC_HOTPULG | 0xe);

	msgbox_init();
	save_state_flag(REC_HOTPULG | 0xf);

	hwspinlock_init();
	save_state_flag(REC_HOTPULG | 0x11);

	msgmanager_init();
	save_state_flag(REC_HOTPULG | 0x12);

#ifdef CFG_RSB_USED
	rsb_init();
	save_state_flag(REC_HOTPULG | 0x13);
#endif

#ifdef CFG_PMU_USED
	pmu_init();
	save_state_flag(REC_HOTPULG | 0x14);
#endif

	dvfs_init();
	save_state_flag(REC_HOTPULG | 0x15);

	dramfreq_init();
	save_state_flag(REC_HOTPULG | 0x16);

	simsyscall_init();
//	taskcall_init();
	service_init();
	cpux_notify();
	//enter daemon process main.

	sysdebug_init();

	load_bytb();
	idle_loop();

	//to avoid daemon main return.
	LOG("system daemon exit\n");
	while (1)
		;
}


#include "includes.h"
//#include "ibase.h"
#include "task.h"
#include "task_handle.h"
#include "msgmanager.h"
#include "cpucfg.h"
#include "pmu_service.h"
#include "bytbmsg.h"

__TASK_INITIAL(pmu_op_task, DEFAULT_TASK_PRIORITY, TASK_GID_PMU, PMU_OPS_HDLE);

static s32 pmu_irq_disable_op(task_struct * task __attribute__ ((__unused__)))
{
	s32 result = 0;

	result = ioctl(PMU_DEVICE_ID, IOCTL_PMU_DISABLE_IRQ);
	return result;
}

static s32 pmu_irq_enable_op(task_struct * task __attribute__ ((__unused__)))
{
	s32 result = 0;

	result = ioctl(PMU_DEVICE_ID, IOCTL_PMU_ENABLE_IRQ);
	return result;
}

static s32 pmu_clr_irq_sta_op(task_struct * task __attribute__ ((__unused__)))
{
	s32 result = 0;

	result = ioctl(PMU_DEVICE_ID, IOCTL_PMU_CLR_IRQ_STATUS);
	return result;
}

static s32 pmu_set_irq_trigger_op(task_struct *task)
{
	s32 result = 0;

	u32 paras[8];
	u32 trigger_type;
	u32 *paras_p = paras;

	bytb_get_paras_form_msg(task->msgparalist, (void **)&paras_p, sizeof(paras));

	trigger_type = paras[0];

	result = ioctl(PMU_DEVICE_ID, IOCTL_PMU_SET_IRQ_TRIGGER, trigger_type);
	return result;
}

static s32 pmu_op_task_handler(task_struct *task)
{
	s32 result = 0;

	u32 paras[8];
	u32 *paras_p = paras;

	u32 pmu_cmd;

	bytb_get_paras_form_msg(task->msgparalist, (void **)&paras_p, sizeof(paras));

	/*
	 * package address and data to message->paras,
	 * message->paras data layout:
	 * |para[0]       |para[1]|para[2]   |para[3]|para[4]|para[5]|para[6]|       para[7]|
	 * |(len|datatype)|devaddr|regaddr0~3|data0  |data1  |data2  |data3  | rsb_cmd
	 */
	pmu_cmd = paras[7];

	switch (pmu_cmd) {
	case AXP_DISABLE_IRQ:
		result = pmu_irq_disable_op(task);
		break;
	case AXP_ENABLE_IRQ:
		result = pmu_irq_enable_op(task);
		break;
	case CLR_NMI_STATUS:
		result = pmu_clr_irq_sta_op(task);
		break;
	case SET_NMI_TRIGGER:
		result = pmu_set_irq_trigger_op(task);
		break;
	default:
		break;
	}

	return result;
}

static s32 pmu_task_feedback(task_struct * task __attribute__ ((__unused__)))
{
	return OK;
}

int pmu_platform_init(void)
{
	create_task((u32)(&pmu_op_task),(u32)pmu_op_task_handler);

	pmu_op_task.feedback = (__taskCBK_t)pmu_task_feedback;

	return OK;
}






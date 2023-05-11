#include "includes.h"
#include "task.h"
#include "task_handle.h"
#include "msgmanager.h"
#include "driver.h"
#include "rsb_service.h"

__TASK_INITIAL(rsb_op_task, DEFAULT_TASK_PRIORITY, TASK_GID_RSB, RSB_OPS_HDLE);

static s32 rsb_read_op(task_struct *task)
{
	u32 i;
	s32 result = 0;

	u32 paras[8];
	u32 *paras_p = paras;

	u8 regaddr[RSB_TRANS_BYTE_MAX];
	u32 data[RSB_TRANS_BYTE_MAX];

	struct rsb_paras rsb_block_paras;
	struct msgblock_head *head;

	rsb_device *rsb_dev;

	rsb_dev = query_rsb();

	get_paras_form_msg(task->msgparalist, (void **)&paras_p, sizeof(paras));
#ifdef RSB_SERVICE_DEBUG
	LOG("paras[0-3]:0x%x, 0x%x, 0x%x, 0x%x\n",
		paras[0], paras[1], paras[2], paras[3]);
	LOG("paras[4-7]:0x%x, 0x%x, 0x%x, 0x%x\n",
		paras[4], paras[5], paras[6], paras[7]);
#endif

	/*
	 * package address and data to message->paras,
	 * message->paras data layout:
	 * |para[0]       |para[1]|para[2]   |para[3]|para[4]|para[5]|para[6]|
	 * |(len|datatype)|devaddr|regaddr0~3|data0  |data1  |data2  |data3  |
	 */
	rsb_block_paras.len      = paras[0] & 0xffff;
	rsb_block_paras.datatype = (paras[0] >> 16) & 0xffff;
	rsb_block_paras.devaddr  = paras[1];
	rsb_block_paras.regaddr  = regaddr;
	rsb_block_paras.data     = data;
	for (i = 0; i < rsb_block_paras.len; i++) {
		/* unpack 8bit regaddr0~regaddr3 from 32bit paras[2] */
		rsb_block_paras.regaddr[i] = (paras[2] >> (i * 8)) & 0xff;
	}

	head = container_of(task->msgparalist, struct msgblock_head, list);
#ifdef RSB_SERVICE_DEBUG
	LOG("head type:%x, len:%x, attr:%x\n",
		head->type, head->length, head->attr);
#endif

	result = rsb_read_block(rsb_dev, &rsb_block_paras);

	for (i = 0; i < rsb_block_paras.len; i++) {
		paras[3 + i] = rsb_block_paras.data[i];
	}
	set_paras_to_msg(task->msgparalist, (void *)paras, sizeof(paras));
#ifdef RSB_SERVICE_DEBUG
	LOG("paras[0-3]:0x%x, 0x%x, 0x%x, 0x%x\n",
		paras[0], paras[1], paras[2], paras[3]);
	LOG("paras[4-7]:0x%x, 0x%x, 0x%x, 0x%x\n",
		paras[4], paras[5], paras[6], paras[7]);
#endif

	return result;
}

static s32 rsb_write_op(task_struct *task)
{
	u32 i;
	s32 result = 0;

	u32 paras[8];
	u32 *paras_p = paras;

	u8 regaddr[RSB_TRANS_BYTE_MAX];
	u32 data[RSB_TRANS_BYTE_MAX];

	struct rsb_paras rsb_block_paras;
	struct msgblock_head *head;

	rsb_device *rsb_dev;

	rsb_dev = query_rsb();

	get_paras_form_msg(task->msgparalist, (void **)&paras_p, sizeof(paras));
#ifdef RSB_SERVICE_DEBUG
	LOG("paras[0-3]:0x%x, 0x%x, 0x%x, 0x%x\n",
		paras[0], paras[1], paras[2], paras[3]);
	LOG("paras[4-7]:0x%x, 0x%x, 0x%x, 0x%x\n",
		paras[4], paras[5], paras[6], paras[7]);
#endif

	/*
	 * package address and data to message->paras,
	 * message->paras data layout:
	 * |para[0]       |para[1]|para[2]   |para[3]|para[4]|para[5]|para[6]|
	 * |(len|datatype)|devaddr|regaddr0~3|data0  |data1  |data2  |data3  |
	 */
	rsb_block_paras.len      = paras[0] & 0xffff;
	rsb_block_paras.datatype = (paras[0] >> 16) & 0xffff;
	rsb_block_paras.devaddr  = paras[1];
	rsb_block_paras.regaddr  = regaddr;
	rsb_block_paras.data     = data;
	for (i = 0; i < rsb_block_paras.len; i++) {
		/* unpack 8bit regaddr0~regaddr3 from 32bit paras[2] */
		rsb_block_paras.regaddr[i] = (paras[2] >> (i * 8)) & 0xff;
	}

	head = container_of(task->msgparalist, struct msgblock_head, list);
#ifdef RSB_SERVICE_DEBUG
	LOG("head type:%x, len:%x; devaddr:%x, attr:%x\n",
		head->type, head->length, head->attr);
#endif

	for (i = 0; i < rsb_block_paras.len; i++) {
		rsb_block_paras.data[i] = paras[3 + i];
	}
	result = rsb_write_block(rsb_dev, &rsb_block_paras);

	return OK;
}

static s32 rsb_op_task_handler(task_struct *task)
{
	s32 result = 0;

	u32 paras[8];
	u32 *paras_p = paras;

	u32 rsb_cmd;

	get_paras_form_msg(task->msgparalist, (void **)&paras_p, sizeof(paras));

	/*
	 * package address and data to message->paras,
	 * message->paras data layout:
	 * |para[0]       |para[1]|para[2]   |para[3]|para[4]|para[5]|para[6]|       para[7]|
	 * |(len|datatype)|devaddr|regaddr0~3|data0  |data1  |data2  |data3  | rsb_cmd
	 */
	rsb_cmd = paras[7];

	switch (rsb_cmd) {
	case RSB_READ_BLOCK_DATA:
		result = rsb_read_op(task);
		break;
	case RSB_WRITE_BLOCK_DATA:
		result = rsb_write_op(task);
		break;
	default:
		break;
	}

	return result;
}

int rsb_service_init(void)
{
	add_task(&rsb_op_task, (__taskCBK_t)rsb_op_task_handler);

	return OK;
}






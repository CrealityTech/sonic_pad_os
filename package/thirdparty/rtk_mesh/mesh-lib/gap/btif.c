/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 */

#include <string.h>
#include <stdarg.h>
#include <btif.h>
#include <trace.h>

#include <gap_msg.h>
#include "gap_internal.h"
#include "gap_adv.h"
#include "bt_types.h"
#include "hci_imp.h"

#include "mesh_beacon.h"

#define HCI_OP_LE_SET_ADV_PARA 0x2006
#define HCI_OP_LE_SET_ADV_ENABLE 0x200A
#define HCI_OP_LE_SET_ADV_DATA 0x2008
#define HCI_OP_LE_SET_SCAN_RSP 0x2009
#define HCI_OP_LE_SET_SCAN_ENABLE 0x200C
#define HCI_OP_LE_SET_SCAN_PARA 0x200B

#define HCI_OP_LE_SET_ONE_SHOT_ADV_ENABLE 0xFC87
#define HCI_OP_LE_SET_AGGRESSIVE_ENABLE 0xFCA6

#define F_BT_LE_SUPPORT				1
#define F_BT_LE_GAP_PERIPHERAL_SUPPORT		1
#define F_BT_LE_GAP_CENTRAL_SUPPORT		1

struct hci_cmd_le_set_adv_para {
	uint16_t adv_interval_min;
	uint16_t adv_interval_max;
	uint8_t adv_type;
	uint8_t own_address_type;
	uint8_t peer_address_type;
	uint8_t peer_address[6];
	uint8_t adv_channel_map;
	uint8_t adv_filter_policy;
} __attribute__((packed));

struct hci_cmd_le_set_adv_dat {
	uint8_t adv_dat_len;
	uint8_t adv_dat[31];
} __attribute__((packed));

struct hci_cmd_le_set_scan_rsp {
	uint8_t scan_rsp_len;
	uint8_t scan_rsp[31];
} __attribute__((packed));

struct hci_cmd_le_set_scan_enable{
	uint8_t scan_mode;
	uint8_t filter_duplicates;
} __attribute__((packed));

struct hci_cmd_le_set_scan_para{
	uint8_t scan_type;
	uint16_t scan_interval;
	uint16_t scan_window;
	uint8_t own_address_type;
	uint8_t scan_filter_policy;
} __attribute__((packed));

static uint16_t le_adv_opcode = 0;
static uint8_t le_adv_req_count = 0;
static uint8_t le_adv_param_set_count = 0;
static uint8_t le_adv_data_set_count = 0;
static uint8_t le_scan_data_set_count = 0;
static uint8_t le_scan_enable_set_count = 0;
static uint8_t le_scan_param_set_count = 0;
static uint8_t le_scan_param_set_retry_count = 0;
static T_BTIF_LE_SCAN_TYPE le_scan_type_record = 0;
static uint16_t le_scan_interval_record = 0;
static uint16_t le_scan_window_record = 0;
static T_BTIF_LE_SCAN_FILTER_POLICY le_scan_filter_policy_record = 0;
static T_BTIF_LOCAL_ADDR_TYPE le_scan_local_addr_type_record = 0;

#if F_BT_LE_SUPPORT

#if F_BT_LE_GAP_PERIPHERAL_SUPPORT
void btif_param_reset(void)
{
	le_adv_opcode = 0;
	le_adv_req_count = 0;
	le_adv_param_set_count = 0;
	le_adv_data_set_count = 0;
	le_scan_data_set_count = 0;
	le_scan_enable_set_count = 0;
	le_scan_param_set_count = 0;
	le_scan_param_set_retry_count = 0;
}
#ifdef MESH_ANDROID
struct le_vendor_cmd_0xfca6_return_param
{
	uint8_t status;
	uint8_t cmd_subtype;
};
#endif

#ifdef MESH_ANDROID
struct hci_cmd_le_vendor_set_scan_para{
	uint8_t cmd_subtype;
	uint8_t scan_type;
	uint16_t scan_interval;
	uint16_t scan_window;
	uint8_t own_address_type;
	uint8_t scan_filter_policy;
} __attribute__((packed));

struct hci_cmd_le_vendor_set_scan_enable{
	uint8_t cmd_subtype;
	uint8_t scan_mode;
	uint8_t filter_duplicates;
} __attribute__((packed));

#endif

static void hci_le_set_scan_enable_rsp(const void *data, uint8_t size,
				       void *user_data);
static void hci_le_set_scan_param_rsp(const void *data, uint8_t size,
				      void *user_data);

static void hci_le_set_aggressive_enable_rsp(const void *data, uint8_t size,
				       void *user_data)
{
#ifdef MESH_ANDROID
	struct le_vendor_cmd_0xfca6_return_param *return_param;
	return_param = (struct le_vendor_cmd_0xfca6_return_param *)data;

	if(return_param->cmd_subtype == 0x02)
	{
		if (return_param->status != 0) {
			BTIF_PRINT_ERROR2("vendor cmd 0xFCA6 sub_cmd 0x%x return err code 0x%02x",
							  return_param->cmd_subtype, return_param->status);
		}
		else
		{
			//for driving the gap scheduler to send set scan param hci cmd immediately, send one beacon
			beacon_send();
		}
	}
	else
	{
		if (return_param->status != 0) {
			BTIF_PRINT_ERROR2("vendor cmd 0xFCA6 sub_cmd 0x%x return error code 0x%02x",
						  return_param->cmd_subtype, return_param->status);
		}
	}
	return;
#else
	uint8_t status = *(uint8_t *)data;
	if (status != 0) {
				BTIF_PRINT_ERROR1("Set aggressive mode enable failed, error code 0x%02x",
						  status);
	}
	//for driving the gap scheduler to send set scan param hci cmd immediately, send one beacon
	beacon_send();
	return;
#endif

}
static void hci_le_set_one_shot_adv_enable_rsp(const void *data, uint8_t size,
				       void *user_data)
{
	T_BTIF_UP_MSG pmsg;
	T_BTIF_VENDOR_CMD_RSP *p_rsp;
	uint8_t status = *(uint8_t *)data;
       le_adv_opcode = 0;

	p_rsp = &pmsg.p.vendor_cmd_rsp;

    	pmsg.command = BTIF_MSG_VENDOR_CMD_RSP;

	p_rsp->command     = HCI_LE_VENDOR_EXTENSION_FEATURE;
	p_rsp->cause       = status;
    	p_rsp->is_cmpl_evt = false;
    	p_rsp->param_len   = 1;
	p_rsp->param[0]  = HCI_EXT_SUB_ONE_SHOT_ADV;

	if (status == 0) {
		BTIF_PRINT_INFO0("Set one shot adv success");
	} else {
		BTIF_PRINT_ERROR1("Set one shot adv failed, error code 0x%02x",
				  status);
	}

	gap_handle_le_vendor_cmd_rsp(p_rsp);

	return;
}

bool btif_vendor_cmd_req(uint16_t op, uint8_t paralen, uint8_t *para)
{
	unsigned int id;
	uint16_t cause = 0;
	T_BTIF_UP_MSG pmsg;
	T_BTIF_VENDOR_CMD_RSP *p_rsp;

	switch(op)
	{
		case HCI_OP_LE_SET_ONE_SHOT_ADV_ENABLE:
			if (le_adv_opcode) {
				BTIF_PRINT_ERROR1("LE one shot adv request failed, op 0x%04x", le_adv_opcode);
				cause = GATT_ERR | GATT_ERR_INVALID_STATE;
				goto failed;
			} else {
				le_adv_opcode = HCI_OP_LE_SET_ONE_SHOT_ADV_ENABLE;
			}
			BTIF_PRINT_INFO1("LE one shot ADV enable, op %04x", le_adv_opcode);
			id = hci_cmd_send(HCI_OP_LE_SET_ONE_SHOT_ADV_ENABLE, para, paralen,
			   					hci_le_set_one_shot_adv_enable_rsp, NULL, NULL);
			if (!id) {
				BTIF_PRINT_ERROR0("Sending set one shot adv enable cmd failed");
				le_adv_opcode = 0;
				cause = HCI_ERR | HCI_ERR_NO_MEMORY;
				goto failed;
			}
			break;
		case HCI_OP_LE_SET_AGGRESSIVE_ENABLE:
			id = hci_cmd_send(HCI_OP_LE_SET_AGGRESSIVE_ENABLE, para, paralen,
								hci_le_set_aggressive_enable_rsp, NULL, NULL);
			if(!id) {
				BTIF_PRINT_ERROR0("Sending set aggressive mode enable cmd failed");
			}
			break;
		default:
			BTIF_PRINT_ERROR1("vendor op 0x%04x is not supported",op);
			break;
	}
	return true;

failed:

	p_rsp = &pmsg.p.vendor_cmd_rsp;

    	pmsg.command = BTIF_MSG_VENDOR_CMD_RSP;
    	switch(op)
	{
		case HCI_OP_LE_SET_ONE_SHOT_ADV_ENABLE:
			p_rsp->command     = HCI_LE_VENDOR_EXTENSION_FEATURE;
			p_rsp->cause       = cause;
    			p_rsp->is_cmpl_evt = false;
    			p_rsp->param_len   = 1;
			p_rsp->param[0]  = HCI_EXT_SUB_ONE_SHOT_ADV;
			break;
		default:
			break;
	}

	gap_handle_le_vendor_cmd_rsp(p_rsp);
	return false;

}

/* Invoke when cmd complete event of hci le set adv enable cmd is received */
static void hci_le_set_adv_enable_resp(const void *data, uint8_t size,
				       void *user_data)
{
	T_BTIF_UP_MSG pmsg;
	uint8_t status = *(uint8_t *)data;

	if(le_adv_req_count == 0)
	{
		BTIF_PRINT_ERROR0("unexpected adv enable response event received");
		return;
	}
	else
	{
		le_adv_req_count--;
	}

	pmsg.command = BTIF_MSG_LE_ADV_RSP;
	if (le_adv_opcode & (1 << 15)) {
		pmsg.p.le_adv_rsp.adv_mode = BTIF_LE_ADV_MODE_ENABLED;
	} else {
		pmsg.p.le_adv_rsp.adv_mode = BTIF_LE_ADV_MODE_DISABLED;
	}

	pmsg.p.le_adv_rsp.cause = (uint16_t)status;

	le_adv_opcode = 0;

	if (status == 0) {
		BTIF_PRINT_INFO0("Set adv parameter success");
	} else {
		BTIF_PRINT_ERROR1("Set adv parameter failed, error code 0x%02x",
				  status);
	}

	le_adv_handle_btif_msg(&pmsg);

	return;
}

/* Issue hci le set adv enable command */
bool btif_le_adv_req(T_BTIF_LE_ADV_MODE adv_mode)
{
	unsigned int id;
	uint8_t para;
	uint16_t cause = 0;
	T_BTIF_UP_MSG pmsg;

	para = adv_mode;

	le_adv_req_count++;

	if (le_adv_opcode) {
		BTIF_PRINT_ERROR1("LE adv request failed, op 0x%04x",
				  le_adv_opcode);
		cause = GATT_ERR | GATT_ERR_INVALID_STATE;
		goto failed;
	} else {
		le_adv_opcode = HCI_OP_LE_SET_ADV_ENABLE;
		if (adv_mode == BTIF_LE_ADV_MODE_ENABLED)
			le_adv_opcode |= (1 << 15);
	}

	BTIF_PRINT_INFO1("LE ADV enable, op %04x", le_adv_opcode);
	id = hci_cmd_send(HCI_OP_LE_SET_ADV_ENABLE, &para, 1,
			   hci_le_set_adv_enable_resp, NULL, NULL);
	if (!id) {
		BTIF_PRINT_ERROR0("Sending set adv enable cmd failed");
		le_adv_opcode = 0;
		cause = HCI_ERR | HCI_ERR_NO_MEMORY;
		goto failed;
	}

	return true;

failed:
	le_adv_req_count--;
	pmsg.command = BTIF_MSG_LE_ADV_RSP;
	pmsg.p.le_adv_rsp.adv_mode = adv_mode;
	pmsg.p.le_adv_rsp.cause = cause;

	le_adv_handle_btif_msg(&pmsg);

	return false;
}

/* Invoke when cmd complete event of hci le set adv para cmd is received */
static void hci_le_set_adv_param_rsp(const void *data, uint8_t size,
				     void *user_data)
{
	T_BTIF_UP_MSG pmsg;
	uint8_t status = *(uint8_t *)data;

	pmsg.command = BTIF_MSG_LE_ADV_PARAM_SET_RSP;
	pmsg.p.le_adv_param_set_rsp.cause = (uint16_t) status;

	le_adv_opcode = 0;
	if(le_adv_param_set_count == 0)
	{
		BTIF_PRINT_ERROR0("unexpected set adv param rsp received");
		return;
	}
	else
	{
		le_adv_param_set_count--;
	}

	if (status == 0)
	{
		BTIF_PRINT_INFO0("Set adv parameter success");
	}
	else
	{
		BTIF_PRINT_ERROR1("Set adv parameter failed, error code 0x%02x",
				  status);
	}
	le_adv_handle_btif_msg(&pmsg);

	return;
}

bool btif_le_adv_param_set_req(T_BTIF_LE_ADV_TYPE adv_type,
			       T_BTIF_LE_ADV_FILTER_POLICY filter_policy,
			       uint16_t min_interval, uint16_t max_interval,
			       T_BTIF_LOCAL_ADDR_TYPE local_addr_type,
			       uint8_t* bd_addr,
			       T_BTIF_REMOTE_ADDR_TYPE remote_addr_type,
			       uint8_t chann_map)
{
	unsigned int id;
	T_BTIF_UP_MSG pmsg;
	uint16_t cause = 0;
	struct hci_cmd_le_set_adv_para para;

	le_adv_param_set_count++;

	if (le_adv_opcode != 0) {
		BTIF_PRINT_ERROR1("le_adv_param_set_req failed, op 0x%04x",
				  le_adv_opcode);
		cause = GATT_ERR | GATT_ERR_INVALID_STATE;
		goto failed;
	} else {
		le_adv_opcode = HCI_OP_LE_SET_ADV_PARA;
	}

	BTIF_PRINT_INFO0("Set adv parameters");
	para.adv_interval_min = min_interval;
	para.adv_interval_max = max_interval;
	para.adv_type = adv_type;
	para.own_address_type = local_addr_type;
	para.peer_address_type = remote_addr_type;
	para.adv_channel_map = chann_map;
	para.adv_filter_policy = filter_policy;
	if (!bd_addr)
		memset(para.peer_address, 0, 6);
	else
		memcpy(para.peer_address, bd_addr, 6);

	id = hci_cmd_send(HCI_OP_LE_SET_ADV_PARA,
			   &para, sizeof(para),
			   hci_le_set_adv_param_rsp, NULL, NULL);
	if (!id) {
		BTIF_PRINT_ERROR0("Sending set adv param cmd failed");
		le_adv_opcode = 0;
		cause = HCI_ERR | HCI_ERR_NO_MEMORY;
		goto failed;
	}

	return true;

failed:
	le_adv_param_set_count--;
	pmsg.command = BTIF_MSG_LE_ADV_PARAM_SET_RSP;
	pmsg.p.le_adv_param_set_rsp.cause = cause;

	le_adv_handle_btif_msg(&pmsg);
	return false;
}

static void hci_le_set_adv_data_rsp(const void *data, uint8_t size,
				    void *user_data)
{
	uint8_t status = *(uint8_t *)data;
	T_BTIF_UP_MSG pmsg;
	if(le_adv_data_set_count == 0)
	{
		BTIF_PRINT_ERROR0("unexpected set adv data rsp received");
		return;
	}
	else
	{
		le_adv_data_set_count --;
	}

	pmsg.command = BTIF_MSG_LE_ADV_DATA_SET_RSP;
	pmsg.p.le_adv_data_set_rsp.data_type = BTIF_LE_DATA_TYPE_ADV;
	pmsg.p.le_adv_data_set_rsp.cause = (uint16_t)status;

	le_adv_opcode = 0;

	if (status == 0) {
		BTIF_PRINT_INFO0("Set adv data success");
	} else {
		BTIF_PRINT_ERROR1("Set adv data failed, error code 0x%02x",
				  status);
	}

	le_adv_handle_btif_msg(&pmsg);

	return;
}

static void hci_le_set_scan_data_rsp(const void *data, uint8_t len,
				     void *user_data)
{
	uint8_t status = *(uint8_t *)data;
	T_BTIF_UP_MSG pmsg;

	pmsg.command = BTIF_MSG_LE_ADV_DATA_SET_RSP;
	pmsg.p.le_adv_data_set_rsp.data_type = BTIF_LE_DATA_TYPE_SCAN_RSP;
	pmsg.p.le_adv_data_set_rsp.cause = (uint16_t)status;

	le_adv_opcode = 0;
	if(le_scan_data_set_count == 0)
	{
		BTIF_PRINT_ERROR0("unexpected set scan data rsp received");
		return;
	}
	else
	{
		le_scan_data_set_count--;
	}
	if (status == 0) {
		BTIF_PRINT_INFO0("Set scan rsp data success");
	} else {
		BTIF_PRINT_ERROR1("Set scan rsp data failed, error code 0x%02x",
				  status);
	}

	le_adv_handle_btif_msg(&pmsg);

	return;
}

bool btif_le_adv_data_set_req(T_BTIF_LE_ADV_DATA_TYPE data_type,
			      uint8_t data_len, uint8_t * p_data)
{
	uint16_t cause = 0;
	T_BTIF_UP_MSG pmsg;
	unsigned int id;

	if (le_adv_opcode != 0) {
		BTIF_PRINT_ERROR1("le_adv_data_set_req failed, op 0x%x",
				  le_adv_opcode);
		cause = GATT_ERR | GATT_ERR_INVALID_STATE;
		goto failed;
	} else {
		if (data_type == BTIF_LE_DATA_TYPE_ADV)
			le_adv_opcode = HCI_OP_LE_SET_ADV_DATA;
		else
			le_adv_opcode = HCI_OP_LE_SET_SCAN_RSP;
	}

	if (data_type == BTIF_LE_DATA_TYPE_ADV) {
		struct hci_cmd_le_set_adv_dat adv_para;
		le_adv_data_set_count ++;
		BTIF_PRINT_INFO0("Set adv data");
		adv_para.adv_dat_len = data_len;
		memcpy(adv_para.adv_dat, p_data, data_len);
		id = hci_cmd_send(HCI_OP_LE_SET_ADV_DATA,
				   &adv_para, (data_len + 1),
				   hci_le_set_adv_data_rsp, NULL,
				   NULL);
		if (!id) {
			BTIF_PRINT_ERROR0("Set adv data failed");
			le_adv_opcode = 0;
			le_adv_data_set_count --;
			cause = HCI_ERR | HCI_ERR_NO_MEMORY;
			goto failed;
		}
	} else {
		struct hci_cmd_le_set_scan_rsp scan_para;
		le_scan_data_set_count++;
		BTIF_PRINT_INFO0("Set scan resp data");
		scan_para.scan_rsp_len = data_len;
		memcpy(scan_para.scan_rsp, p_data, data_len);
		id = hci_cmd_send(HCI_OP_LE_SET_SCAN_RSP,
				   &scan_para, (data_len + 1),
				   hci_le_set_scan_data_rsp, NULL,
				   NULL);
		if (!id) {
			BTIF_PRINT_ERROR0("Set scan rsp failed");
			le_adv_opcode = 0;
			le_scan_data_set_count--;
			cause = HCI_ERR | HCI_ERR_NO_MEMORY;
			goto failed;
		}

	}
	return true;

failed:
	pmsg.command = BTIF_MSG_LE_ADV_DATA_SET_RSP;
	pmsg.p.le_adv_data_set_rsp.data_type = data_type;
	pmsg.p.le_adv_data_set_rsp.cause = cause;

	le_adv_handle_btif_msg(&pmsg);
	return false;
}
#endif /* end of F_BT_LE_GAP_PERIPHERAL_SUPPORT */

#if F_BT_LE_GAP_CENTRAL_SUPPORT
static void hci_le_set_scan_enable_rsp(const void *data, uint8_t size,
				       void *user_data)
{
	T_BTIF_UP_MSG pmsg;
	uint8_t status = *(uint8_t *)data;

	if(le_scan_enable_set_count == 0)
	{
		BTIF_PRINT_ERROR0("unexpected set scan enable rsp received");
		return;
	}
	else
	{
		le_scan_enable_set_count--;
	}
	pmsg.command = BTIF_MSG_LE_SCAN_RSP;
	pmsg.p.le_scan_rsp.cause = (uint16_t) status;
	if (status == 0) {
		BTIF_PRINT_INFO0("Set scan para success");
	} else {
		BTIF_PRINT_ERROR1("Set scan para failed, error code 0x%02x",
				  status);
	}
	le_scan_handle_btif_msg(&pmsg);

	return;
}

bool btif_le_scan_req(T_BTIF_LE_SCAN_MODE mode,
		      T_BTIF_LE_SCAN_FILTER_DUPLICATES filter_duplicates)
{
	uint16_t cause = 0;
	T_BTIF_UP_MSG pmsg;
	unsigned int id;
	struct hci_cmd_le_set_scan_enable para;
	le_scan_enable_set_count++;
	para.scan_mode = mode;
	para.filter_duplicates = filter_duplicates;

#ifdef MESH_ANDROID
	//BTIF_PRINT_ERROR0("MESH_ANDROID: send scan enable by 0xfca9");
	id = hci_cmd_send(0xFCA9,
			   &para, sizeof(para),
			   hci_le_set_scan_enable_rsp, NULL, NULL);
#else
	id = hci_cmd_send(HCI_OP_LE_SET_SCAN_ENABLE,
			   &para, sizeof(para),
			   hci_le_set_scan_enable_rsp, NULL, NULL);
#endif
	if (!id) {
		BTIF_PRINT_ERROR0("btif_le_scan_param_set_req failed");
		cause = HCI_ERR | HCI_ERR_NO_MEMORY;
		goto failed;
	}

	return true;

failed:
	le_scan_enable_set_count--;
	pmsg.command = BTIF_MSG_LE_SCAN_RSP;
	pmsg.p.le_scan_rsp.cause = cause;

	le_scan_handle_btif_msg(&pmsg);
	return false;
}

static void hci_le_set_scan_param_rsp(const void *data, uint8_t size,
				      void *user_data)
{
	T_BTIF_UP_MSG pmsg;
	uint8_t status = *(uint8_t *)data;

	if(le_scan_param_set_count == 0)
	{
		BTIF_PRINT_ERROR0("unexpected set scan param rsp received");
		return;
	}
	else
	{
		le_scan_param_set_count--;
	}
	pmsg.command = BTIF_MSG_LE_SCAN_PARAM_SET_RSP;
	pmsg.p.le_scan_param_set_rsp.cause = (uint16_t) status;

	if (status == 0) {
		BTIF_PRINT_INFO0("Set scan para success");
	} else {
		BTIF_PRINT_ERROR1("Set scan para failed, error code  0x%02x",
				  status);
		if(status == 0x0c) //cmd is disallowed
		{
			le_scan_param_set_retry_count++;
			if(le_scan_param_set_retry_count == 40)
			{
				le_scan_param_set_retry_count = 0;
			}
			else
			{
				usleep(30000);
				BTIF_PRINT_ERROR1("retry to send hci scan param cmd, %d",le_scan_param_set_retry_count);
				btif_le_scan_param_set_req(le_scan_type_record, le_scan_interval_record,
				le_scan_window_record,
				le_scan_filter_policy_record,
				le_scan_local_addr_type_record);
				return;
			}
		}
	}

	le_scan_handle_btif_msg(&pmsg);

	return;
}

bool btif_le_scan_param_set_req(T_BTIF_LE_SCAN_TYPE type, uint16_t interval,
				uint16_t window,
				T_BTIF_LE_SCAN_FILTER_POLICY filter_policy,
				T_BTIF_LOCAL_ADDR_TYPE local_addr_type)
{
	uint16_t cause = 0;
	T_BTIF_UP_MSG pmsg;
	unsigned int id;
	struct hci_cmd_le_set_scan_para para;
	le_scan_param_set_count++;
	para.scan_type = type;
	para.scan_interval = interval;
	para.scan_window = window;
	para.own_address_type = local_addr_type;
	para.scan_filter_policy = filter_policy;

	le_scan_type_record = type;
	le_scan_interval_record = interval;
	le_scan_window_record = window;
	le_scan_local_addr_type_record = local_addr_type;
	le_scan_filter_policy_record = filter_policy;

#ifdef MESH_ANDROID
	//BTIF_PRINT_ERROR0("MESH_ANDROID: send scan param by 0xfca8");
	id = hci_cmd_send(0xFCA8,
			   &para, sizeof(para),
			   hci_le_set_scan_param_rsp, NULL, NULL);
#else
	id = hci_cmd_send(HCI_OP_LE_SET_SCAN_PARA,
			   &para, sizeof(para),
			   hci_le_set_scan_param_rsp, NULL, NULL);
#endif

	if (!id) {
		BTIF_PRINT_ERROR0("btif_le_scan_param_set_req failed");
		cause = HCI_ERR | HCI_ERR_NO_MEMORY;
		goto failed;
	}
	return true;
failed:
	le_scan_param_set_count--;
	pmsg.command = BTIF_MSG_LE_SCAN_PARAM_SET_RSP;
	pmsg.p.le_scan_param_set_rsp.cause = cause;

	le_scan_handle_btif_msg(&pmsg);
	return false;
}
#endif /* end of F_BT_LE_GAP_CENTRAL_SUPPORT */

#endif /* end of F_BT_LE_SUPPORT */

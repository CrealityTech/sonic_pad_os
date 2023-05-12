#include "rtkmesh_interface.h"

int32_t rtkmesh_iv_init(uint32_t iv_index)
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void*)&iv_index;
	ret = mesh_bta_submit_command_wait(RTK_MESH_IV_INIT_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_seq_init(uint32_t seq)
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void*)&seq;
	ret = mesh_bta_submit_command_wait(RTK_MESH_SEQ_INIT_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_iv_seq_get(uint32_t *pseq, uint32_t *piv_index)
{
	int32_t ret = 0;
	void *argv[2];
	int argc;

	argc = 2;
	argv[0] = (void*)pseq;
	argv[1] = (void*)piv_index;
	ret = mesh_bta_submit_command_wait(RTK_MESH_IV_SEQ_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_node_num_get(uint32_t *pnode_num )
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void*)pnode_num;
	ret = mesh_bta_submit_command_wait(RTK_MESH_NODE_NUM_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_hb_monitor_set(int32_t count, int32_t timeout )
{
	int32_t ret = 0;
	void *argv[2];
	int argc;

	argc = 2;
	argv[0] = (void*)&count;
	argv[1] = (void*)&timeout;
	ret = mesh_bta_submit_command_wait(RTK_MESH_HB_MONITOR_SET_OP, argc, argv);

	return ret;
}
int32_t rtkmesh_stack_enable(uint8_t localUUID[16])
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void*)localUUID;
	ret = mesh_bta_submit_command_wait(RTK_MESH_ENABLE_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_set_local_netkey(int32_t netKeyIndex, uint8_t Netkey[16], bool bUpdate)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&netKeyIndex;
	argv[1] = (void* )Netkey;
	argv[2] = (void* )&bUpdate;

	ret = mesh_bta_submit_command_wait(RTK_MESH_SET_LOCAL_NET_KEY_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_set_local_appkey(int32_t appKeyIndex, int32_t netKeyIndex, uint8_t Appkey[16], bool bUpdate)
{
	int32_t ret = 0;
	void *argv[4];
	int argc;

	argc = 4;
	argv[0] = (void* )&appKeyIndex;
	argv[1] = (void* )&netKeyIndex;
	argv[2] = (void* )Appkey;
	argv[3] = (void* )&bUpdate;

	ret = mesh_bta_submit_command_wait(RTK_MESH_SET_LOCAL_APP_KEY_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_set_scan_param(uint16_t mesh_ble_scan_interval, uint16_t mesh_ble_scan_window)
{
	int32_t ret = 0;
	void *argv[2];
	int argc;

	argc = 2;
	argv[0] = (void* )&mesh_ble_scan_interval;
	argv[1] = (void* )&mesh_ble_scan_window;

	ret = mesh_bta_submit_command_wait(RTK_MESH_SET_BLE_SCAN_PARAM_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_stop_send_access_msg(uint32_t access_opcode)
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void* )&access_opcode;

	ret = mesh_bta_submit_command_wait(RTK_MESH_STOP_SEND_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_unprov_scan(bool start, int32_t duration)
{
	int32_t ret = 0;
	void *argv[2];
	int argc;

	argc = 2;
	argv[0] = (void* )&start;
	argv[1] = (void* )&duration;

	ret = mesh_bta_submit_command_wait(RTK_MESH_UNPROV_SCAN_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_prov(uint8_t deviceUUID[16], uint16_t assign_address, uint32_t attention_dur, bool enable)
{
	int32_t ret = 0;
	void *argv[4];
	int argc;

	argc = 4;
	argv[0] = (void* )deviceUUID;
	argv[1] = (void* )&assign_address;
	argv[2] = (void* )&attention_dur;
	argv[3] = (void* )&enable;

	ret = mesh_bta_submit_command_wait(RTK_MESH_PROV_INVITE_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_prov_method_choose(prov_start_public_key_t public_key, prov_auth_method_t auth_method, prov_auth_action_t auth_action, prov_auth_size_t auth_size)
{
	int32_t ret = 0;
	void *argv[4];
	int argc;

	argc = 4;
	argv[0] = (void* )&public_key;
	argv[1] = (void* )&auth_method;
	argv[2] = (void* )&auth_action;
	argv[3] = (void* )&auth_size;

	ret = mesh_bta_submit_command_wait(RTK_MESH_PROV_START_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_prov_device_public_key_set(uint8_t public_key[64])
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void* )public_key;

	ret = mesh_bta_submit_command_wait(RTK_MESH_PROV_DEVICE_PUBLIC_KEY_SET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_prov_auth_value_set(uint8_t* pvalue, uint8_t len)
{
	int32_t ret = 0;
	void *argv[2];
	int argc;

	argc = 2;
	argv[0] = (void* )pvalue;
	argv[1] = (void* )&len;

	ret = mesh_bta_submit_command_wait(RTK_MESH_PROV_AUTH_VALUE_SET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_set_mesh_mode(uint8_t enable)
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void* )&enable;

	ret = mesh_bta_submit_command_wait(RTK_MESH_ENABLE_AGGRESSIVE_SET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_set_scan_param_when_a2dp_active(uint16_t scan_interval, uint16_t scan_windows)
{
	int32_t ret = 0;
	void *argv[2];
	int argc;

	argc = 2;
	argv[0] = (void* )&scan_interval;
	argv[1] = (void* )&scan_windows;

	ret = mesh_bta_submit_command_wait(RTK_MESH_SET_SCAN_PARAM_WHEN_A2DP_ACTIVE_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_set_scan_high_prioirty(uint8_t enable, uint8_t highTime)
{
	int32_t ret = 0;
	void *argv[2];
	int argc;

	argc = 2;
	argv[0] = (void* )&enable;
	argv[1] = (void* )&highTime;

	ret = mesh_bta_submit_command_wait(RTK_MESH_SET_SCAN_HIGH_PRIOIRTY_OP, argc, argv );

	return ret;
}

int32_t rtkmesh_model_appkey_bind(uint8_t element_index, uint32_t model_id, uint32_t appKeyIndex_g, uint8_t bindAllAppKeyFlag)
{
	int32_t ret = 0;
	void *argv[4];
	int argc;

	argc = 4;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&model_id;
	argv[2] = (void* )&appKeyIndex_g;
	argv[3] = (void* )&bindAllAppKeyFlag;
	ret = mesh_bta_submit_command_wait(RTK_MESH_BIND_LOCAL_APP_KEY_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_local_element_addr_get(uint8_t element_index, uint16_t *elementAddr)
{
	int32_t ret = 0;
	void *argv[2];
	int argc;

	argc = 2;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )elementAddr;

	ret = mesh_bta_submit_command_wait(RTK_MESH_GET_LOCAL_ELEMENT_ADDR_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_local_ttl_get(uint8_t *currentTTL )
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void* )currentTTL;
	ret = mesh_bta_submit_command_wait(RTK_MESH_GET_LOCAL_DEF_TTL_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_local_node_reset(void )
{
	int32_t ret = 0;

	ret = mesh_bta_submit_command_wait(RTK_MESH_NET_INFO_ERASE_OP, 0, NULL);
	return ret;
}

int32_t rtkmesh_devkey_add(uint16_t addr, uint8_t element_num, uint8_t devkey[16])
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&addr;
	argv[1] = (void* )&element_num;
	argv[2] = (void* )devkey;

	ret = mesh_bta_submit_command_wait(RTK_MESH_ADD_DEV_KEY_OP, argc, argv);
	return ret;
}

int32_t rtkmesh_devkey_delete(uint16_t addr )
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void* )&addr;

	ret = mesh_bta_submit_command_wait(RTK_MESH_DEL_DEV_KEY_OP, argc, argv);
	return ret;
}

int32_t rtkmesh_model_data_set(MESH_MODEL_DATA_T *p)
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void* )p;

	ret = mesh_bta_submit_command_wait(RTK_MESH_SET_MODEL_DATA_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_bt_MAC_get(uint8_t* btMac )
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void* )btMac;
	ret = mesh_bta_submit_command_wait(RTK_MESH_GET_BT_MAC_ADDR_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_mesh_info_list(void )
{
	int32_t ret = 0;
	ret = mesh_bta_submit_command_wait(RTK_MESH_INFO_LIST, 0, NULL);
	return ret;
}

int32_t rtkmesh_cfg_appkey_add(uint16_t dst, uint16_t netKeyIndex_g, uint16_t appKeyIndex_g )
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&netKeyIndex_g;
	argv[2] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_APP_KEY_ADD_OP, argc, argv);
	return ret;
}

int32_t rtkmesh_cfg_model_appkey_bind(uint16_t dst, uint8_t element_index, uint32_t model_id, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	void *argv[4];
	int argc;

	argc = 4;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&element_index;
	argv[2] = (void* )&model_id;
	argv[3] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_APP_BIND_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_sub_add(uint16_t dst, uint8_t element_index, uint32_t model_id, uint16_t group_addr)
{
	int32_t ret = 0;
	void *argv[4];
	int argc;

	argc = 4;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&element_index;
	argv[2] = (void* )&model_id;
	argv[3] = (void* )&group_addr;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_SUB_ADD_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_vir_sub_add(uint16_t dst, uint8_t element_index, uint32_t model_id, uint8_t group_addr[16])
{
	int32_t ret = 0;
	void *argv[4];
	int argc;

	argc = 4;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&element_index;
	argv[2] = (void* )&model_id;
	argv[3] = (void* )group_addr;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_VIR_SUB_ADD_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_sub_delete(uint16_t dst, uint8_t element_index, uint32_t model_id, uint16_t group_addr)
{
	int32_t ret = 0;
	void *argv[4];
	int argc;

	argc = 4;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&element_index;
	argv[2] = (void* )&model_id;
	argv[3] = (void* )&group_addr;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_SUB_DELETE_OP, argc, argv);

	return ret;
}
int32_t rtkmesh_cfg_model_vir_sub_delete(uint16_t dst, uint8_t element_index, uint32_t model_id, uint8_t group_addr[16])
{
	int32_t ret = 0;
	void *argv[4];
	int argc;

	argc = 4;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&element_index;
	argv[2] = (void* )&model_id;
	argv[3] = (void* )group_addr;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_VIR_SUB_DELETE_OP, argc, argv);

	return ret;
}
int32_t rtkmesh_cfg_model_sub_overwrite(uint16_t dst, uint8_t element_index, uint32_t model_id, uint16_t sub_addr)
{
	int32_t ret = 0;
	void *argv[4];
	int argc;

	argc = 4;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&element_index;
	argv[2] = (void* )&model_id;
	argv[3] = (void* )&sub_addr;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_SUB_OVERWRITE_OP, argc, argv);

	return ret;
}
int32_t rtkmesh_cfg_model_vir_sub_overwrite(uint16_t dst, uint8_t element_index, uint32_t model_id, uint8_t sub_addr[16])
{
	int32_t ret = 0;
	void *argv[4];
	int argc;

	argc = 4;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&element_index;
	argv[2] = (void* )&model_id;
	argv[3] = (void* )sub_addr;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_VIR_SUB_OVERWRITE_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_sub_delete_all(uint16_t dst, uint8_t element_index, uint32_t model_id)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&element_index;
	argv[2] = (void* )&model_id;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_SUB_DELETE_ALL_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_hb_pub_set(uint16_t dst, uint16_t dst_pub, uint8_t count_log,
                                     uint8_t period_log,
                                     uint8_t ttl, uint16_t features, uint16_t net_key_index)
{
	int32_t ret = 0;
	void *argv[7];
	int argc;

	argc = 7;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&dst_pub;
	argv[2] = (void* )&count_log;
	argv[3] = (void* )&period_log;
	argv[4] = (void* )&ttl;
	argv[5] = (void* )&features;
	argv[6] = (void* )&net_key_index;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_HB_PUB_SET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_node_reset(uint16_t dst )
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void* )&dst;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_NODE_RESET_OP, argc, argv);

	return ret;
}


int32_t rtkmesh_generic_on_off_set(uint16_t element_index, uint16_t dst, uint16_t appKeyIndex_g, generic_on_off_t on_off,
									bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
	void *argv[8];
	int argc;

	argc = 8;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;
	argv[3] = (void* )&on_off;
	argv[4] = (void* )&optional;
	argv[5] = (void* )&trans_time;
	argv[6] = (void* )&delay;
	argv[7] = (void* )&ack;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_GOO_MODEL_ON_OFF_SET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_generic_on_off_get(uint16_t element_index, uint16_t dst, uint16_t appKeyIndex_g )
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_GOO_MODEL_ON_OFF_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_lightness_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_LLC_MODEL_LIGHTNESS_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_lightness_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t lightness,
								bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
	void *argv[8];
	int argc;

	argc = 8;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;
	argv[3] = (void* )&lightness;
	argv[4] = (void* )&optional;
	argv[5] = (void* )&trans_time;
	argv[6] = (void* )&delay;
	argv[7] = (void* )&ack;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_LLC_MODEL_LIGHTNESS_SET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_hsl_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_HSL_MODEL_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_hsl_set(uint8_t element_index,  uint16_t dst,
                                    uint16_t appKeyIndex_g, uint16_t lightness, uint16_t hue, int16_t saturation,
                                    bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
	void *argv[10];
	int argc;

	argc = 10;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;
	argv[3] = (void* )&lightness;
	argv[4] = (void* )&hue;
	argv[5] = (void* )&saturation;
	argv[6] = (void* )&optional;
	argv[7] = (void* )&trans_time;
	argv[8] = (void* )&delay;
	argv[9] = (void* )&ack;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_HSL_MODEL_SET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_hsl_target_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_HSL_MODEL_TARGET_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_hsl_hue_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_HSL_MODEL_HUE_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_hsl_hue_set(uint8_t element_index,  uint16_t dst,
                                    uint16_t appKeyIndex_g, uint16_t hue,bool optional,
                                    generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
	void *argv[8];
	int argc;

	argc = 8;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;
	argv[3] = (void* )&hue;
	argv[4] = (void* )&optional;
	argv[5] = (void* )&trans_time;
	argv[6] = (void* )&delay;
	argv[7] = (void* )&ack;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_HSL_MODEL_HUE_SET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_hsl_saturation_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_HSL_MODEL_SATURATION_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_hsl_saturation_set(uint8_t element_index,  uint16_t dst,
                                    uint16_t appKeyIndex_g, uint16_t saturation,bool optional,
                                    generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
	void *argv[8];
	int argc;

	argc = 8;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;
	argv[3] = (void* )&saturation;
	argv[4] = (void* )&optional;
	argv[5] = (void* )&trans_time;
	argv[6] = (void* )&delay;
	argv[7] = (void* )&ack;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_HSL_MODEL_SATURATION_SET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_hsl_default_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_HSL_MODEL_DEFAULT_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_hsl_default_set(uint8_t element_index,  uint16_t dst,
                                    uint16_t appKeyIndex_g, uint16_t lightness,uint16_t hue,
                                    uint16_t saturation, bool ack)
{
	int32_t ret = 0;
	void *argv[8];
	int argc;

	argc = 7;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;
	argv[3] = (void* )&lightness;
	argv[4] = (void* )&hue;
	argv[5] = (void* )&saturation;
	argv[6] = (void* )&ack;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_HSL_MODEL_DEFAULT_SET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_hsl_range_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_HSL_MODEL_RANGE_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_hsl_range_set(uint8_t element_index,  uint16_t dst,
                                    uint16_t appKeyIndex_g, uint16_t hue_range_min, uint16_t hue_range_max,
                                    uint16_t saturation_range_min, uint16_t saturation_range_max, bool ack)
{
	int32_t ret = 0;
	void *argv[8];
	int argc;

	argc = 8;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;
	argv[3] = (void* )&hue_range_min;
	argv[4] = (void* )&hue_range_max;
	argv[5] = (void* )&saturation_range_min;
	argv[6] = (void* )&saturation_range_max;
	argv[7] = (void* )&ack;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_HSL_MODEL_RANGE_SET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_ctl_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_LIGHT_CTL_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_ctl_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t lightness, uint16_t temperature,
								int16_t delta_uv, bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
	void *argv[10];
	int argc;

	argc = 10;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;
	argv[3] = (void* )&lightness;
	argv[4] = (void* )&temperature;
	argv[5] = (void* )&delta_uv;
	argv[6] = (void* )&optional;
	argv[7] = (void* )&trans_time;
	argv[8] = (void* )&delay;
	argv[9] = (void* )&ack;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_LIGHT_CTL_SET_OP, argc, argv);

	return ret;

}

int32_t rtkmesh_light_ctl_temperature_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_LIGHT_CTL_TEMPERATURE_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_ctl_temperature_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t temperature,
								int16_t delta_uv, bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
	void *argv[9];
	int argc;

	argc = 9;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;
	argv[3] = (void* )&temperature;
	argv[4] = (void* )&delta_uv;
	argv[5] = (void* )&optional;
	argv[6] = (void* )&trans_time;
	argv[7] = (void* )&delay;
	argv[8] = (void* )&ack;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_LIGHT_CTL_TEMPERATURE_SET_OP, argc, argv);

	return ret;

}

int32_t rtkmesh_light_ctl_temperature_range_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_LIGHT_CTL_TEMPERATURE_RANGE_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_ctl_temperature_range_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t range_min,
													uint16_t range_max, bool ack)
{
	int32_t ret = 0;
	void *argv[6];
	int argc;

	argc = 6;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;
	argv[3] = (void* )&range_min;
	argv[4] = (void* )&range_max;
	argv[5] = (void* )&ack;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_LIGHT_CTL_TEMPERATURE_RANGE_SET_OP, argc, argv);

	return ret;

}

int32_t rtkmesh_light_ctl_default_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_LIGHT_CTL_DEFAULT_GET_OP, argc, argv);

	return ret;
}

int32_t rtkmesh_light_ctl_default_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t lightness,
													uint16_t temperature, int16_t delta_uv, bool ack)
{
	int32_t ret = 0;
	void *argv[7];
	int argc;

	argc = 7;
	argv[0] = (void* )&element_index;
	argv[1] = (void* )&dst;
	argv[2] = (void* )&appKeyIndex_g;
	argv[3] = (void* )&lightness;
	argv[4] = (void* )&temperature;
	argv[5] = (void* )&delta_uv;
	argv[6] = (void* )&ack;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_LIGHT_CTL_DEFAULT_SET_OP, argc, argv);

	return ret;

}

int32_t rtkmesh_cfg_model_net_trans_set(uint16_t dst, uint8_t count, uint8_t steps)
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&count;
	argv[2] = (void* )&steps;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_NET_TRANS_SET_OP,argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_net_trans_get(uint16_t dst )
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void* )&dst;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_NET_TRANS_GET_OP,argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_beacon_get(uint16_t dst )
{
	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void* )&dst;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_BEACON_GET_OP,argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_beacon_set(uint16_t dst, uint8_t state)
{
	int32_t ret = 0;
	void *argv[2];
	int argc;

	argc = 2;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&state;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_BEACON_SET_OP,argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_compo_data_get(uint16_t dst, uint8_t page)
{
	int32_t ret = 0;
	void *argv[2];
	int argc;

	argc = 2;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&page;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_COMPO_DATA_GET_OP,argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_default_ttl_get(uint16_t dst )
{

	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void* )&dst;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_TTL_GET_OP,argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_default_ttl_set(uint16_t dst, uint8_t ttl )
{

	int32_t ret = 0;
	void *argv[2];
	int argc;

	argc = 2;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&ttl;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_TTL_SET_OP,argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_proxy_stat_get(uint16_t dst )
{

	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void* )&dst;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_PROXY_STAT_GET_OP,argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_proxy_stat_set(uint16_t dst, uint8_t proxy_state )
{

	int32_t ret = 0;
	void *argv[2];
	int argc;

	argc = 2;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&proxy_state;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_PROXY_STAT_SET_OP,argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_relay_get(uint16_t dst )
{

	int32_t ret = 0;
	void *argv[1];
	int argc;

	argc = 1;
	argv[0] = (void* )&dst;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_RELAY_GET_OP,argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_relay_set(uint16_t dst, uint8_t state, uint8_t count, uint8_t steps)
{

	int32_t ret = 0;
	void *argv[4];
	int argc;

	argc = 4;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&state;
	argv[2] = (void* )&count;
	argv[3] = (void* )&steps;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_RELAY_SET_OP,argc, argv);

	return ret;
}

int32_t rtkmesh_cfg_model_pub_get(uint16_t dst, uint16_t element_index, uint32_t model_id )
{
	int32_t ret = 0;
	void *argv[3];
	int argc;

	argc = 3;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&element_index;
	argv[2] = (void* )&model_id;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_PUB_GET_OP, argc, argv);
	return ret;
}

int32_t rtkmesh_cfg_model_pub_set(uint16_t dst, uint16_t element_index, uint16_t publish_addr, pub_key_info_t pub_key_info, uint8_t ttl,
							pub_period_t pub_period, pub_retrans_info_t pub_retrans_info, uint32_t model_id )
{
	int32_t ret = 0;
	void *argv[8];
	int argc;

	argc = 8;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&element_index;
	argv[2] = (void* )&publish_addr;
	argv[3] = (void* )&pub_key_info;
	argv[4] = (void* )&ttl;
	argv[5] = (void* )&pub_period;
	argv[6] = (void* )&pub_retrans_info;
	argv[7] = (void* )&model_id;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_PUB_SET_OP, argc, argv);
	return ret;
}

int32_t rtkmesh_cfg_model_vir_pub_set(uint16_t dst, uint16_t element_index, uint8_t publish_addr[16], pub_key_info_t pub_key_info, uint8_t ttl,
							pub_period_t pub_period, pub_retrans_info_t pub_retrans_info, uint32_t model_id )
{
	int32_t ret = 0;
	void *argv[8];
	int argc;

	argc = 8;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&element_index;
	argv[2] = (void* )publish_addr;
	argv[3] = (void* )&pub_key_info;
	argv[4] = (void* )&ttl;
	argv[5] = (void* )&pub_period;
	argv[6] = (void* )&pub_retrans_info;
	argv[7] = (void* )&model_id;

	ret = mesh_bta_submit_command_wait(RTK_MESH_MSG_CFG_MODEL_VIR_PUB_SET_OP, argc, argv);
	return ret;
}

int32_t rtkmesh_vendor_access_msg_send(int32_t dst, int32_t dstAddrType, int32_t src, int32_t ttl,
						int32_t netKeyIndex, int32_t appKeyIndex, uint8_t* data, int32_t data_len)
{
	int32_t ret = 0;
	void *argv[8];
	int argc;

	argc = 8;
	argv[0] = (void* )&dst;
	argv[1] = (void* )&dstAddrType;
	argv[2] = (void* )&src;
	argv[3] = (void* )&ttl;
	argv[4] = (void* )&netKeyIndex;
	argv[5] = (void* )&appKeyIndex;
	argv[6] = (void* )data;
	argv[7] = (void* )&data_len;

	ret = mesh_bta_submit_command_wait(RTK_MESH_SEND_PKT_OP, argc, argv);
	return ret;
}

int32_t rtkmesh_intensive_adv_set(uint32_t interval, uint32_t duration, uint32_t adjust_interval, uint32_t scan_window_per_interval)
{
	int32_t ret = 0;
	void *argv[4];
	int argc;

	argc = 4;
	argv[0] = (void*)&interval;
	argv[1] = (void*)&duration;
	argv[2] = (void*)&adjust_interval;
	argv[3] = (void*)&scan_window_per_interval;
	ret = mesh_bta_submit_command_wait(RTK_MESH_INTENSIVE_ADV_SET_OP, argc, argv);
	return ret;
}


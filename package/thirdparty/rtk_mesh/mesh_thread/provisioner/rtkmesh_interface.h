
#ifndef _RTK_MESH_INTERFACE_H_
#define _RTK_MESH_INTERFACE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "generic_on_off.h"
#include "light_ctl.h"
#include "generic_power_on_off.h"
#include "light_lightness.h"
#include "light_ctl.h"
#include "light_hsl.h"
#include "health.h"
#include "proxy_protocol.h"
#include "mesh_provisioner.h"
#include "generic_client_app.h"

#include "RTKDefine.h"
#include "RTKMeshTypes.h"
#ifdef __cplusplus
extern "C" {
#endif

int32_t rtkmesh_iv_init(uint32_t iv_index);
int32_t rtkmesh_seq_init(uint32_t seq);
int32_t rtkmesh_iv_seq_get(uint32_t *pseq, uint32_t *piv_index);
int32_t rtkmesh_node_num_get(uint32_t *pnode_num );
int32_t rtkmesh_hb_monitor_set(int32_t count, int32_t timeout );
int32_t rtkmesh_stack_enable(uint8_t localUUID[16]);
int32_t rtkmesh_set_local_netkey(int32_t netKeyIndex, uint8_t Netkey[16], bool bUpdate);
int32_t rtkmesh_set_local_appkey(int32_t appKeyIndex, int32_t netKeyIndex, uint8_t Appkey[16], bool bUpdate);

int32_t rtkmesh_unprov_scan(bool start, int32_t duration);
int32_t rtkmesh_prov(uint8_t deviceUUID[16], uint16_t assign_address, uint32_t attention_dur, bool enable);
int32_t rtkmesh_prov_method_choose(prov_start_public_key_t public_key, prov_auth_method_t auth_method, prov_auth_action_t auth_action, prov_auth_size_t auth_size);
int32_t rtkmesh_prov_device_public_key_set(uint8_t public_key[64]);
int32_t rtkmesh_prov_auth_value_set(uint8_t* pvalue, uint8_t len);

int32_t rtkmesh_set_mesh_mode(uint8_t enable);
int32_t rtkmesh_model_appkey_bind(uint8_t element_index, uint32_t model_id, uint32_t appKeyIndex_g, uint8_t bindAllAppKeyFlag);
int32_t rtkmesh_local_element_addr_get(uint8_t element_index, uint16_t *elementAddr);
int32_t rtkmesh_local_ttl_get(uint8_t *currentTTL );
int32_t rtkmesh_local_node_reset(void );
int32_t rtkmesh_devkey_add(uint16_t addr, uint8_t element_num, uint8_t devkey[16]);
int32_t rtkmesh_devkey_delete(uint16_t addr );
int32_t rtkmesh_model_data_set(MESH_MODEL_DATA_T *p);
int32_t rtkmesh_bt_MAC_get(uint8_t* btMac );
int32_t rtkmesh_mesh_info_list(void );
int32_t rtkmesh_cfg_appkey_add(uint16_t dst, uint16_t netKeyIndex_g, uint16_t appKeyIndex_g );
int32_t rtkmesh_cfg_model_appkey_bind(uint16_t dst, uint8_t element_index, uint32_t model_id, uint16_t appKeyIndex_g);


int32_t rtkmesh_cfg_model_beacon_get(uint16_t dst );
int32_t rtkmesh_cfg_model_beacon_set(uint16_t dst, uint8_t state);
int32_t rtkmesh_cfg_model_compo_data_get(uint16_t dst, uint8_t page);
int32_t rtkmesh_cfg_model_default_ttl_get(uint16_t dst );
int32_t rtkmesh_cfg_model_default_ttl_set(uint16_t dst, uint8_t ttl );
int32_t rtkmesh_cfg_model_proxy_stat_get(uint16_t dst );
int32_t rtkmesh_cfg_model_proxy_stat_set(uint16_t dst, uint8_t proxy_state );
int32_t rtkmesh_cfg_model_relay_get(uint16_t dst );
int32_t rtkmesh_cfg_model_relay_set(uint16_t dst, uint8_t state, uint8_t count, uint8_t steps);
int32_t rtkmesh_cfg_model_pub_get(uint16_t dst, uint16_t element_index, uint32_t model_id );
int32_t rtkmesh_cfg_model_pub_set(uint16_t dst, uint16_t element_index, uint16_t publish_addr, pub_key_info_t pub_key_info, uint8_t ttl,
							pub_period_t pub_period, pub_retrans_info_t pub_retrans_info, uint32_t model_id );
int32_t rtkmesh_cfg_model_vir_pub_set(uint16_t dst, uint16_t element_index, uint8_t publish_addr[16], pub_key_info_t pub_key_info, uint8_t ttl,
							pub_period_t pub_period, pub_retrans_info_t pub_retrans_info, uint32_t model_id );
int32_t rtkmesh_cfg_model_sub_add(uint16_t dst, uint8_t element_index, uint32_t model_id, uint16_t group_addr);
int32_t rtkmesh_cfg_model_vir_sub_add(uint16_t dst, uint8_t element_index, uint32_t model_id, uint8_t group_addr[16]);
int32_t rtkmesh_cfg_model_sub_delete(uint16_t dst, uint8_t element_index, uint32_t model_id, uint16_t group_addr);
int32_t rtkmesh_cfg_model_vir_sub_delete(uint16_t dst, uint8_t element_index, uint32_t model_id, uint8_t group_addr[16]);
int32_t rtkmesh_cfg_model_sub_overwrite(uint16_t dst, uint8_t element_index, uint32_t model_id, uint16_t sub_addr);
int32_t rtkmesh_cfg_model_vir_sub_overwrite(uint16_t dst, uint8_t element_index, uint32_t model_id, uint8_t sub_addr[16]);
int32_t rtkmesh_cfg_model_sub_delete_all(uint16_t dst, uint8_t element_index, uint32_t model_id);
int32_t rtkmesh_cfg_model_hb_pub_set(uint16_t dst, uint16_t dst_pub, uint8_t count_log,
                                     uint8_t period_log,
                                     uint8_t ttl, uint16_t features, uint16_t net_key_index);

int32_t rtkmesh_cfg_model_node_reset(uint16_t dst );

int32_t rtkmesh_cfg_model_net_trans_set(uint16_t dst, uint8_t count, uint8_t steps);
int32_t rtkmesh_cfg_model_net_trans_get(uint16_t dst );

int32_t rtkmesh_generic_on_off_get(uint16_t element_index, uint16_t dst, uint16_t appKeyIndex_g );
int32_t rtkmesh_generic_on_off_set(uint16_t element_index, uint16_t dst, uint16_t appKeyIndex_g, generic_on_off_t on_off,
									bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack);

int32_t rtkmesh_light_lightness_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g);
int32_t rtkmesh_light_lightness_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t lightness,
								bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack);

int32_t rtkmesh_light_hsl_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g);
int32_t rtkmesh_light_hsl_set(uint8_t element_index,  uint16_t dst,
                                    uint16_t appKeyIndex_g, uint16_t lightness, uint16_t hue, int16_t saturation,
                                    bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack);
int32_t rtkmesh_light_hsl_target_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g);
int32_t rtkmesh_light_hsl_hue_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g);
int32_t rtkmesh_light_hsl_hue_set(uint8_t element_index,  uint16_t dst,
                                    uint16_t appKeyIndex_g, uint16_t hue,bool optional,
                                    generic_transition_time_t trans_time, uint8_t delay, bool ack);
int32_t rtkmesh_light_hsl_saturation_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g);
int32_t rtkmesh_light_hsl_saturation_set(uint8_t element_index,  uint16_t dst,
                                    uint16_t appKeyIndex_g, uint16_t saturation,bool optional,
                                    generic_transition_time_t trans_time, uint8_t delay, bool ack);
int32_t rtkmesh_light_hsl_default_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g);
int32_t rtkmesh_light_hsl_default_set(uint8_t element_index,  uint16_t dst,
                                    uint16_t appKeyIndex_g, uint16_t lightness,uint16_t hue,
                                    uint16_t saturation, bool ack);
int32_t rtkmesh_light_hsl_range_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g);
int32_t rtkmesh_light_hsl_range_set(uint8_t element_index,  uint16_t dst,
                                    uint16_t appKeyIndex_g, uint16_t hue_range_min, uint16_t hue_range_max,
                                    uint16_t saturation_range_min, uint16_t saturation_range_max, bool ack);
int32_t rtkmesh_light_ctl_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g);
int32_t rtkmesh_light_ctl_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t lightness, uint16_t temperature,
								int16_t delta_uv, bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack);
int32_t rtkmesh_light_ctl_temperature_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g);
int32_t rtkmesh_light_ctl_temperature_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t temperature,
								int16_t delta_uv, bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack);
int32_t rtkmesh_light_ctl_temperature_range_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g);
int32_t rtkmesh_light_ctl_temperature_range_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t range_min,
													uint16_t range_max, bool ack);
int32_t rtkmesh_light_ctl_default_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g);
int32_t rtkmesh_light_ctl_default_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t lightness,
													uint16_t temperature, int16_t delta_uv, bool ack);

int32_t rtkmesh_vendor_access_msg_send(int32_t dst, int32_t dstAddrType, int32_t src, int32_t ttl,
						int32_t netKeyIndex, int32_t appKeyIndex, uint8_t* data, int32_t data_len);

int32_t rtkmesh_set_scan_param_when_a2dp_active(uint16_t scan_interval, uint16_t scan_windows);
int32_t rtkmesh_set_scan_param(uint16_t mesh_ble_scan_interval, uint16_t mesh_ble_scan_window);
int32_t rtkmesh_set_scan_high_prioirty(uint8_t enable, uint8_t highTime);
int32_t rtkmesh_stop_send_access_msg(uint32_t access_opcode);

int32_t rtkmesh_intensive_adv_set(uint32_t interval, uint32_t duration, uint32_t adjust_interval, uint32_t scan_window_per_interval);


#ifdef __cplusplus
}
#endif
#endif



#ifndef _RTK_MESH_TYPE_H_
#define _RTK_MESH_TYPE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <app_msg.h>
#include "os_msg.h"

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

#include "user_cmd_parse.h"
#include "RTKDefine.h"
#include "mesh_api.h"
#ifdef __cplusplus
extern "C" {
#endif

#define RTK_MESH_ENABLE_OP										0x0000  //for mesh_enable & Mesh_disable
#define RTK_MESH_SET_LOCAL_NET_KEY_OP 							0x0001  //for mesh_set_netKey
#define RTK_MESH_SET_LOCAL_APP_KEY_OP 							0x0002  //for mesh_set_appKey
#define RTK_MESH_SET_BLE_SCAN_PARAM_OP							0x0003
#define RTK_MESH_STOP_SEND_OP									0x0004
#define RTK_MESH_IV_INIT_OP										0x0005
#define RTK_MESH_SEQ_INIT_OP										0x0006
#define RTK_MESH_SEND_PKT_OP										0x0037
#define RTK_MESH_IV_SEQ_GET_OP									0x0008
#define RTK_MESH_NODE_NUM_GET_OP								0x0039
#define RTK_MESH_HB_MONITOR_SET_OP							0x0041

#define RTK_MESH_UNPROV_SCAN_OP 									0x8004	//for mesh_unprov_scan
#define RTK_MESH_PROV_INVITE_OP									0x8005  //for mesh_prov_invite
#define RTK_MESH_PROV_START_OP									0x8006
#define RTK_MESH_PROV_DEVICE_PUBLIC_KEY_SET_OP					0x8007
#define RTK_MESH_PROV_AUTH_VALUE_SET_OP						0x8008

#define RTK_MESH_BIND_LOCAL_APP_KEY_OP							0x0007  //for mesh_modelAPP_bind
#define RTK_MESH_GET_LOCAL_ELEMENT_ADDR_OP						0x0009  //for mesh_get_elementAddr
#define RTK_MESH_GET_LOCAL_DEF_TTL_OP 							0x000A 	//for mesh_get_defaultTTL

#define RTK_MESH_NET_INFO_ERASE_OP 								0x000C  //for mesh_data_reset

#define RTK_MESH_ADD_DEV_KEY_OP									0x000E	//for mesh_add_devKey
#define RTK_MESH_DEL_DEV_KEY_OP									0x000F   //for mesh_delete_devKey
#define RTK_MESH_SET_MODEL_DATA_OP 								0x0010	//for mesh_set_model_data
#define RTK_MESH_ENABLE_AGGRESSIVE_SET_OP   						0x0011   //for mesh_enable_aggressiveSetting
#define RTK_MESH_SET_SCAN_PARAM_WHEN_A2DP_ACTIVE_OP	 		0x0012
#define RTK_MESH_SET_SCAN_HIGH_PRIOIRTY_OP						0x0013

#define RTK_MESH_GET_BT_MAC_ADDR_OP								0x0014 	//for mesh_getBtMac
#define RTK_MESH_INFO_LIST											0x0015	//for list all mesh node info

#define RTK_MESH_MSG_CFG_MODEL_APP_KEY_ADD_OP					0x0016
#define RTK_MESH_MSG_CFG_MODEL_APP_BIND_OP						0x0017


#define RTK_MESH_MSG_CFG_MODEL_NET_TRANS_SET_OP				0x0021
#define RTK_MESH_MSG_CFG_MODEL_NET_TRANS_GET_OP 				0x0022

#define RTK_MESH_MSG_CFG_MODEL_BEACON_GET_OP 					0x0023
#define RTK_MESH_MSG_CFG_MODEL_BEACON_SET_OP					0x0024
#define RTK_MESH_MSG_CFG_MODEL_COMPO_DATA_GET_OP			0x0025
#define RTK_MESH_MSG_CFG_MODEL_TTL_GET_OP						0x0026
#define RTK_MESH_MSG_CFG_MODEL_TTL_SET_OP						0x0027
#define RTK_MESH_MSG_CFG_MODEL_PROXY_STAT_GET_OP				0x0028
#define RTK_MESH_MSG_CFG_MODEL_PROXY_STAT_SET_OP				0x0029
#define RTK_MESH_MSG_CFG_MODEL_RELAY_GET_OP					0x002a
#define RTK_MESH_MSG_CFG_MODEL_RELAY_SET_OP					0x002b
#define RTK_MESH_MSG_CFG_MODEL_PUB_GET_OP						0x002c
#define RTK_MESH_MSG_CFG_MODEL_PUB_SET_OP						0x002d
#define RTK_MESH_MSG_CFG_MODEL_VIR_PUB_SET_OP					0x002e
#define RTK_MESH_MSG_CFG_MODEL_SUB_ADD_OP						0x002f
#define RTK_MESH_MSG_CFG_MODEL_VIR_SUB_ADD_OP					0x0030
#define RTK_MESH_MSG_CFG_MODEL_SUB_DELETE_OP					0x0031
#define RTK_MESH_MSG_CFG_MODEL_VIR_SUB_DELETE_OP				0x0032
#define RTK_MESH_MSG_CFG_MODEL_SUB_OVERWRITE_OP				0x0033
#define RTK_MESH_MSG_CFG_MODEL_VIR_SUB_OVERWRITE_OP			0x0034
#define RTK_MESH_MSG_CFG_MODEL_SUB_DELETE_ALL_OP				0x0035
#define RTK_MESH_MSG_CFG_MODEL_HB_PUB_SET_OP				0x0036


#define RTK_MESH_MSG_CFG_MODEL_NODE_RESET_OP					0x0040


#define RTK_MESH_MSG_GOO_MODEL_ON_OFF_SET_OP					0x011b
#define RTK_MESH_MSG_GOO_MODEL_ON_OFF_GET_OP					0x011c

#define RTK_MESH_MSG_LLC_MODEL_LIGHTNESS_SET_OP				0x021d
#define RTK_MESH_MSG_LLC_MODEL_LIGHTNESS_GET_OP				0x021e

#define RTK_MESH_MSG_LIGHT_CTL_GET_OP							0x0300
#define RTK_MESH_MSG_LIGHT_CTL_SET_OP							0x0301
#define RTK_MESH_MSG_LIGHT_CTL_TEMPERATURE_GET_OP			0x0302
#define RTK_MESH_MSG_LIGHT_CTL_TEMPERATURE_SET_OP				0x0303
#define RTK_MESH_MSG_LIGHT_CTL_TEMPERATURE_RANGE_GET_OP		0x0304
#define RTK_MESH_MSG_LIGHT_CTL_TEMPERATURE_RANGE_SET_OP		0x0305
#define RTK_MESH_MSG_LIGHT_CTL_DEFAULT_GET_OP					0x0306
#define RTK_MESH_MSG_LIGHT_CTL_DEFAULT_SET_OP					0x0307

#define RTK_MESH_MSG_HSL_MODEL_GET_OP									0x0400
#define RTK_MESH_MSG_HSL_MODEL_SET_OP									0x0401

#define RTK_MESH_MSG_HSL_MODEL_TARGET_GET_OP					0x0402
#define RTK_MESH_MSG_HSL_MODEL_HUE_GET_OP							0x0403
#define RTK_MESH_MSG_HSL_MODEL_HUE_SET_OP							0x0404

#define RTK_MESH_MSG_HSL_MODEL_SATURATION_GET_OP			0x0405
#define RTK_MESH_MSG_HSL_MODEL_SATURATION_SET_OP			0x0406

#define RTK_MESH_MSG_HSL_MODEL_DEFAULT_GET_OP					0x0407
#define RTK_MESH_MSG_HSL_MODEL_DEFAULT_SET_OP					0x0408

#define RTK_MESH_MSG_HSL_MODEL_RANGE_GET_OP						0x0409
#define RTK_MESH_MSG_HSL_MODEL_RANGE_SET_OP						0x040a

#define RTK_MESH_INTENSIVE_ADV_SET_OP							0x0501

#define MESH_TRACE_LEVEL 	0x03
#define MESH_INFO_LEVEL	 	0x02
#define MESH_WARN_LEVEL	 	0x01
#define MESH_ERROR_LEVEL	0x00


#define DISC_TIMEOUT_MSG 123 //
#define PROV_START_BY_STEP_TIMEOUT_MSG 124

#define MESH_INIT_NETKEY_SET_DONE	0x01
#define MESH_INIT_APPKEY_SET_DONE	0x02
#define MESH_INIT_MODEL_REG_DONE	0x04

#define    MESH_FLASH_PARAMS_NODE_INFO_APP 	0
#define    MESH_FLASH_PARAMS_IV_INDEX_APP	1
#define   MESH_FLASH_PARAMS_IV_UPDATE_FLAG_APP	2
#define   MESH_FLASH_PARAMS_SEQ_APP		3
#define   MESH_FLASH_PARAMS_TTL_APP		4
#define   MESH_FLASH_PARAMS_FEATURES_APP	5
#define   MESH_FLASH_PARAMS_NWK_TX_PARAMS_APP	6
#define   MESH_FLASH_PARAMS_HB_PARAMS_APP	7
#define   MESH_FLASH_PARAMS_DEV_KEY_APP		8
#define   MESH_FLASH_PARAMS_NET_KEY_APP		9
#define    MESH_FLASH_PARAMS_APP_KEY_APP	10
#define    MESH_FLASH_PARAMS_VIR_ADDR_APP	11
#define    MESH_FLASH_PARAMS_RPL_APP		12
#define    MESH_FLASH_PARAMS_RPL_ENTRY_APP	13
#define    MESH_FLASH_PARAMS_RPL_SEQ_APP	14
#define   MESH_FLASH_PARAMS_MODEL_PUB_PARAMS_APP 	15
#define    MESH_FLASH_PARAMS_MODEL_APP_KEY_BINDING_APP	16
#define    MESH_FLASH_PARAMS_MODEL_SUBSCRIBE_ADDR_APP  	17

#define COMPANY_ID	0x005D
#define PRODUCT_ID	0x0000
#define VERSION_ID      ((0x0000 << 1) | (0 & 0x1))

/*!        @*/
#define BTA_WAKE_CODE	0x78

#define REQ_PENDING	0
#define REQ_DONE	1

#define APP_HANDLE_MESH_EVET 				0x00
#define APP_HANDLE_LLS_DATA					0x01
#define APP_HANDLE_GOOS_DATA				0x02
#define APP_HANDLE_CTL_TEMPRATUE 			0x03
#define APP_HANDLE_CFG_SERVICE_STATUS		0x04
#define APP_HANDLE_HSL_DATA							0x05

struct mesh_bta_command {
	unsigned int id;
	uint16_t opcode;
	uint16_t req_status;
	int req_result;
	int argc;
	void **argv;
	uint32_t rparams[2];
};

typedef uint8_t MeshLibLogLevel_t;
typedef uint8_t MeshAppLogLevel_t;

typedef enum {
    ADD_CONFIGURATION_SERVER_MODEL_OP,
    ADD_LIGHTNESS_CLIENT_MODEL_OP,
} MESH_ADD_MODEL_OPCODE_T;

typedef struct {
    MESH_ADD_MODEL_OPCODE_T                  opcode;
    uint8_t element_index;
    model_data_cb_pf callback;
} MESH_MODEL_DATA_T;

typedef struct {
    UINT8 number_of_elements;   /**< Number of elements supported by the device */
    UINT16 algorithms;          /**< Supported algorithms and other capabilities */
    UINT8 public_key_type;      /**< Supported public key types */
    UINT8 static_oob_type;      /**< Supported static OOB Types */
    UINT8 output_oob_size;      /**< Maximum size of Output OOB supported */
    UINT16 output_oob_action;   /**< Supported Output OOB Actions */
    UINT8 input_oob_size;       /**< Maximum size in octets of Input OOB supported */
    UINT16 input_oob_action;    /**< Supported Input OOB Actions */
} MESH_PROV_CAPABILITIES_T;

typedef enum {
    AG_MESH_PROV_SUCCESS = 0,         /**< Provisioning success */
    AG_MESH_PROV_FAILED_ERROR_CODE_INVALID_PDU = 1,        /**< The provisioning protocol PDU is not recognized by the device */
    AG_MESH_PROV_FAILED_ERROR_CODE_INVALID_FORMAT = 2,     /**< The arguments of the protocol PDUs are outside expected values or the length of the PDU is different than expected */
    AG_MESH_PROV_FAILED_ERROR_CODE_UNEXPECTED_PDU = 3,     /**< The PDU received was not expected at this moment of the procedure */
    AG_MESH_PROV_FAILED_ERROR_CODE_CONFIRMATION_FAILED = 4, /**< The computed confirmation value was not successfully verified */
    AG_MESH_PROV_FAILED_ERROR_CODE_OUT_OF_RESOURCES = 5,   /**< The provisioning protocol cannot be continued due to insufficient resources in the device */
    AG_MESH_PROV_FAILED_ERROR_CODE_DECRYPTION_FAILED = 6,  /**< The Data block was not successfully decrypted */
    AG_MESH_PROV_FAILED_ERROR_CODE_UNEXPECTED_ERROR = 7,   /**< An unexpected error occurred that may not be recoverable */
    AG_MESH_PROV_FAILED_ERROR_CODE_CANNOT_ASSIGN_ADDR = 8, /**< The device cannot assign consecutive unicast addresses to all elements */
    AG_MESH_PROV_FAILED_ERROR_CODE_TRANSACTION_TIMEOUT = 200, /**MTK private enum field, reserve 200 elements for original SDK */
    AG_MESH_PROV_FAILED_ERROR_CODE_PROVISION_TIMEOUT = 201,
    AG_MESH_PROV_FAILED_ERROR_CODE_AUTHENTICATION_FAILED = 202,
    AG_MESH_PROV_FAILED_ERROR_CODE_SDK_INTERNAL = 300,     /**< Alibaba: private error code for ERROR_CODE_INTERNAL */
    AG_MESH_PROV_FAILED_ERROR_CODE_INVALID_GW_INFO = 301,  /**< Alibaba: private error code for ERROR_CODE_INTERNAL */
    AG_MESH_PROV_FAILED_ERROR_CODE_GW_CONN_TOUT = 302,     /**< Alibaba: private error code for ERROR_CODE_GW_CONN_TOUT */
    AG_MESH_PROV_FAILED_ERROR_CODE_GW_CONFIRM = 303,       /**< Alibaba: private error code for ERROR_CODE_GW_CONFIRM */
    AG_MESH_PROV_FAILED_ERROR_CODE_GW_AUTH = 304,          /**< Alibaba: private error code for ERROR_CODE_GW_AUTH */
    AG_MESH_PROV_FAILED_ERROR_CC_TOUT = 305,               /**< Alibaba: private error code for ERROR_CODE_CC_TOUT */
} MESH_PROV_ERROR_T;

typedef struct {
    MESH_PROV_ERROR_T reason;       /**< Indicate the provisioning process success or failed reason. */
    UINT16 address;      /**< Indicate the target unicast address. */
    UINT8 device_key[16]; /**< Indicate the device key. */
    BOOL success;       /**< Indicate the provisioning process is successfull or not. */
    BOOL gatt_bearer;
} MESH_EVT_PROV_DONE_T;

typedef struct {
    MESH_PROV_CAPABILITIES_T cap;     /**< The capabilities detail value. */
} MESH_EVT_PROV_CAPABILITIES_T;

typedef struct {
    UINT8 uuid[16];   /**< The unprovisioned device UUID. */
    UINT16 oob_info;                  /**< The OOB information of unprovisioned device. */
} MESH_EVT_PROV_SCAN_UD_T;

typedef struct {
    UINT16 address;
    bool active;
}MESH_EVT_HEARTBEAT_T;

typedef enum {
    AG_MESH_PROV_FACTOR_CONFIRMATION_KEY,
    AG_MESH_PROV_FACTOR_RANDOM_PROVISIONER,
    AG_MESH_PROV_FACTOR_RANDOM_DEVICE,
    AG_MESH_PROV_FACTOR_CONFIRMATION_PROVISIONER,
    AG_MESH_PROV_FACTOR_CONFIRMATION_DEVICE,
    AG_MESH_PROV_FACTOR_PUB_KEY,
    AG_MESH_PROV_FACTOR_AUTHEN_VALUE,
    AG_MESH_PROV_FACTOR_AUTHEN_RESULT,
} MESH_PROV_FACTOR_TYPE_T;

typedef struct {
    union {
        MESH_EVT_PROV_CAPABILITIES_T      prov_cap;
        MESH_EVT_PROV_DONE_T              prov_done;
        MESH_EVT_PROV_SCAN_UD_T           prov_scan_ud;
				MESH_EVT_HEARTBEAT_T							heartbeat;
    } mesh;
} MESH_EVT_T;

typedef enum {
    MESH_EVT_INIT_DONE,
    MESH_EVT_PROV_CAPABILITIES,
    MESH_EVT_PROV_DONE,
    MESH_EVT_PROV_SCAN_UD_RESULT,
    MESH_EVT_PROV_PUBLIC_KEY,
    MESH_EVT_PROV_AUTH_DATA,
    MESH_EVT_HEARTBEAT,
    //The above enum value match with SDK
    MESH_EVT_UD_RESULT_COMPLETE = 200,
} MESH_EVENT_ID;

typedef struct {
    UINT32 evt_id; ///< Event ID
    union
    {
      MESH_EVT_T mesh_evt; ///< MESH event structure
    }evt;
} MESH_BT_EVENT_T;

typedef struct {
    uint32_t type;
    void *pargs;
} MESH_MODEL_LLS_RECV_DATA_T,MESH_MODEL_GOOS_RECV_DATA_T,MESH_MODEL_LC_RECV_DATA_T,MESH_MODEL_RECV_DATA_T;

typedef VOID (*AppMeshBtEventCbk)(MESH_BT_EVENT_T *event);

typedef struct _MeshLibLogMoudle_t
{
	uint32_t COMMON:1;
	uint32_t GAP_SCHED:1;
	uint32_t PB_ADV:1;
	uint32_t PB_GATT:1;
	uint32_t SERVICE:1;
	uint32_t PROV:1;
	uint32_t PROXY:1;
	uint32_t BEACON:1;
	uint32_t BEARER:1;
	uint32_t NETWORK:1;
	uint32_t TRANSPORT:1;
	uint32_t FRND:1;
	uint32_t HB:1;
	uint32_t TRANS_PING:1;
	uint32_t ACCESS:1;
	uint32_t RPL:1;
	uint32_t SECURITY:1;
	uint32_t FLASH:1;
	uint32_t MODEL:1;
	uint32_t APP:1;
}MeshLibLogMoudle_t;

typedef struct _MeshAppLogMoudle_t
{
	uint8_t APP:1;
	uint8_t DFU:1;
	uint8_t PROFILE:1;
	uint8_t GAP:1;
	uint8_t BTIF:1;
	uint8_t GATT:1;
}MeshAppLogMoudle_t;

typedef struct _MESH_NODE_SET_t{
//log set
	MeshLibLogLevel_t		MeshLibLogLevel;
	MeshLibLogMoudle_t		MeshLibLogMoudle;
	MeshAppLogLevel_t		MeshAppLogLevel;
	MeshAppLogMoudle_t		MeshAppLogMoudle;
//mesh node cfg
	mesh_node_features_t 	features;
	mesh_node_cfg_t 		node_cfg;
}Mesh_node_set_t;


typedef int (*bcmd_handler_t)(uint16_t opcode, int argc, void **argv);
/* mesh_blue_cmd.h */

//copy from mesh_rpl.h
typedef struct
{
    uint16_t used: 1;
    uint16_t rfu: 15;
    uint16_t src;
    uint32_t seq;
} rpl_entry_t, *rpl_entry_p;

typedef struct
{
    uint8_t rpl_loop; //!< Indicate which list is new
    uint16_t entry_num; //!< the number of replay protection list entries
    rpl_entry_t *rpl[2];
} mesh_rpl_t, *mesh_rpl_p;

extern struct queue *mesh_event_msgq;
extern mesh_model_info_t model_goo_client_info;
extern mesh_model_info_t model_light_ctl_client_info;
extern mesh_model_info_t model_llc_client_info;
extern mesh_model_info_t model_light_hsl_client_info;
extern bool dev_info_show_flag;
extern mesh_node_t mesh_node;
extern uint16_t assign_addr;
extern uint16_t assign_net_key_index;
extern mesh_model_info_t cfg_client;
extern T_GAP_DEV_STATE old_device_state;
extern T_GAP_DEV_STATE gap_device_state;
extern plt_timer_t mesh_service_id_adv_timer;
extern proxy_ctx_t *proxy_ctx;
extern mesh_rpl_t mesh_rpl;
extern uint8_t proxy_ctx_count;

extern uint8_t gap_adv_flag ;
extern bool gap_update_param ;
extern T_GAP_DEV_STATE gap_device_state;
extern T_GAP_DEV_STATE old_device_state;
extern uint16_t gap_init_flag;
extern uint8_t period_adv_set_flag;
extern uint8_t period_adv_flag;
extern uint32_t period_adv_time;
extern uint16_t period_adv_interval;

extern bool prov_manual;
extern uint32_t attn_dur;
extern int first_start_flag;

extern plt_timer_t discover_timer;
extern compo_data_page0_header_t compo_data_page0_header_exceptFeature;

extern AppMeshBtEventCbk mesh_event_cb;

extern uint64_t APPloglayer;
extern uint32_t MESHloglayer;
extern uint8_t loglevel;
extern uint16_t devnum;
extern uint8_t prov_start_flag;

void disc_handle_timeout(void );
void prov_start_by_step_handle_timeout(void );

bool mesh_element_create_forALi(uint16_t loc);

int32_t mesh_bta_cmd_process(uint16_t opcode, int argc, void **argv);
int32_t mesh_init_ali(AppMeshBtEventCbk cb,uint32_t logLayer1,uint32_t logLayer2,uint32_t logLevel,int32_t maxNodeSize,uint16_t local_unicast_addr);
int mesh_bta_submit_command_wait(uint16_t opcode, int argc, void **argv);
int32_t quit_mesh_bta_thread(void);

/*!        @*/
void set_mesh_bta_cmd_fd(int fd[2]);
void set_mesh_bta_mainloop_id(pthread_t tid);
void mesh_bta_register_handler(bcmd_handler_t handler);
void mesh_bta_run_command(void);
void mesh_bta_deinit(void);
void mesh_bta_clear_commands(void);
int32_t rtk_mesh_getBtMac(char* btMac);
void user_app_local_mesh_node_deinit(void);
void set_realtime_priority(void);

int32_t generic_on_off_client_data_recv(const mesh_model_info_p pmodel_info, uint32_t type,
                                    void *pargs);


/* mesh_blue_cmd.h */

void mesh_stack_init(uint16_t local_unicast_addr);

void mesh_cfg_client_recv_cb_register(model_data_cb_pf cb);
void mesh_goo_recv_cb_register(model_data_cb_pf cb);
void mesh_llc_recv_cb_register(model_data_cb_pf cb);
void mesh_light_ctl_recv_cb_register(model_data_cb_pf cb);
void mesh_light_hsl_recv_cb_register(model_data_cb_pf cb);

#ifdef __cplusplus
}
#endif
#endif


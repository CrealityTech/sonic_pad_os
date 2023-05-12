/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     dfu_server.h
* @brief
* @details
* @author   ken_mei
* @date     02-09-2016
* @version  v1.0.0
******************************************************************************
* @attention
* <h2><center>&copy; COPYRIGHT 2015 Realtek Semiconductor Corporation</center></h2>
******************************************************************************
*/

#ifndef _DFU_SERVICE_H_
#define _DFU_SERVICE_H_

#include "platform_misc.h"
#include "profile_server.h"
#include "flash_device.h"
#include "otp.h"
#include "aes_api.h"
#include "patch_header_check.h"

BEGIN_DECLS

/** @addtogroup DFU_Server
  * @{
  */

/** @defgroup DFU_Server_Exported_Macros Exported Macros
  * @{
  */

#define DFU_TEMP_BUFFER_SIZE            2048
#define DFU_SERVER_ADV_PERIOD           5000//!< ms
#define DFU_SERVER_TIMEOUT_MSG          110
#define DFU_WO_SCAN                     1

//00006287-3c17-d293-8e48-14fe2e4da212
#define GATT_UUID128_DFU_SERVICE        0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0x87, 0x62, 0x00, 0x00
#define GATT_UUID128_DFU_DATA           0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0x87, 0x63, 0x00, 0x00
#define GATT_UUID128_DFU_CONTROL_POINT  0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0x87, 0x64, 0x00, 0x00

//#define GATT_UUID_DFU_SERVICE              0x8762
//#define GATT_UUID_DFU_PACKET               0x8763
//#define GATT_UUID_DFU_CONTROL_POINT        0x8764


#define DFU_OPCODE_MIN                          0x00
#define DFU_OPCODE_START_DFU                    0x01
#define DFU_OPCODE_RECEIVE_FW_IMAGE_INFO        0x02
#define DFU_OPCODE_VALID_FW                     0x03
#define DFU_OPCODE_ACTIVE_IMAGE_RESET           0x04
#define DFU_OPCODE_SYSTEM_RESET                 0x05
#define DFU_OPCODE_REPORT_TARGET_INFO           0x06
#define DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ      0x07

#define DFU_OPCODE_PKT_RX_NOTIFICATION_VOICE    0x08
#define DFU_OPCODE_BUFFER_CHECK_EN              0x09 /*report current ota function version information*/
#define DFU_OPCODE_REPORT_BUFFER_CRC            0x0a /*report current buffer CRC*/

#define DFU_OPCODE_RECEIVE_IC_TYPE              0x0b
#define DFU_OPCODE_MAX                          0x0c


#define DFU_OPCODE_NOTIF                    0x10

/*length of each control point procedure*/
#define DFU_LENGTH_START_DFU                (1+16)
#define DFU_LENGTH_RECEIVE_FW_IMAGE_INFO    (1+2+4)
#define DFU_LENGTH_VALID_FW                 (1+2)
#define DFU_LENGTH_ACTIVE_IMAGE_RESET       0x01
#define DFU_LENGTH_SYSTEM_RESET             0x01
#define DFU_LENGTH_REPORT_TARGET_INFO       (1+2)
#define DFU_LENGTH_PKT_RX_NOTIF_REQ         (1+2)
#define DFU_LENGTH_CONN_PARA_TO_UPDATE_REQ  (1+2+2+2+2)

#define DFU_NOTIFY_LENGTH_ARV                   3
/**
 * error, shall be 3 + 4 + 4, but can't fix due to compatibility.
 * Beacause the breakpoint resume isn't supportted, it doesn't matter.
 */
#define DFU_NOTIFY_LENGTH_REPORT_TARGET_INFO    (3+2+4)
#define DFU_NOTIFY_LENGTH_PKT_RX_NOTIF          (3+2)


#define INDEX_DFU_PACKET_VALUE              0x02
#define INDEX_DFU_CONTROL_POINT_CHAR_VALUE  0x04

#define DFU_OPCODE_MIN                                      0x00
#define DFU_OPCODE_START_DFU                                0x01
#define DFU_OPCODE_RECEIVE_FW_IMAGE_INFO                    0x02
#define DFU_OPCODE_VALID_FW                                 0x03
#define DFU_OPCODE_ACTIVE_IMAGE_RESET                       0x04
#define DFU_OPCODE_SYSTEM_RESET                             0x05
#define DFU_OPCODE_REPORT_TARGET_INFO                       0x06
#define DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ                  0x07
#define DFU_OPCODE_PKT_RX_NOTIFICATION_VOICE                0x08
#define DFU_OPCODE_REPORT_FUNCTION_TYPE                     0x09 /*report current ota function version information*/
#define DFU_OPCODE_REPORT_BUFFER_SIZE                       0x0a /*report current buffer size information*/
//#define DFU_OPCODE_REPORT_BUFFER_CRC                        0x0b /*report current buffer CRC*/
#define DFU_OPCODE_INDICATE_BUFFER_IS_VALID                 0x0c /*indicate current buffer data if valid*/
//#define DFU_OPCODE_RECEIVE_IC_TYPE                          0x0d

//#define DFU_OPCODE_MAX                                      0x0e

#define INDEX_DFU_PACKET_VALUE                  0x02
#define INDEX_DFU_CONTROL_POINT_CHAR_VALUE      0x04
#define INDEX_DFU_CHAR_CCCD_INDEX               0x05

#define DFU_OPCODE_NOTIFICATION                          0x10
//------------------------DFU_LENGTH DEFINE------------------------------

//#define DFU_LENGTH_PKT_RX_NOTIFICATION_REQ      (1+8)

//#define DFU_NOTIFY_LENGTH_ARV                           3
//#define DFU_NOTIFY_LENGTH_REPORT_TARGET_INFO             (3+2+4)
//#define DFU_NOTIFY_LENGTH_PKT_RX_NOTIFICATION         (3+2)

#define DFU_ARV_SUCCESS                                             0x01
#define DFU_ARV_FAIL_INVALID_PARAMETER              0x02
#define DFU_ARV_FAIL_OPERATION                              0x03
#define DFU_ARV_FAIL_DATA_SIZE_EXCEEDS_LIMIT    0x04
#define DFU_ARV_FAIL_CRC_ERROR                              0x05
#define DFU_ARV_FAIL_LENGTH_ERROR               0x06
#define DFU_ARV_FAIL_PROG_ERROR                 0x07
#define DFU_ARV_FAIL_ERASE_ERROR                0x08

#define DFU_NOTIFY_LENGTH_FUNC_VERSION          (1+1+1+2)
#define DFU_NOTIFY_LENGTH_BUFFER_SIZE           (1+1+1+4)
#define DFU_NOTIFY_LENGTH_BUFFER_CRC            (1+1+1+2)
#define DFU_NOTIFY_LENGTH_BUFFER_IS_VALID       (1+1)/*opCode + valid flag*/
/** @} */

/** @defgroup DFU_Server_Exported_Types Exported Types
  * @{
  */
typedef struct _T_START_DFU_PARA
{
    uint8_t ic_type;
    uint8_t secure_version;
    union
    {
        uint16_t value;
        struct
        {
            uint16_t xip: 1; // payload is executed on flash
            uint16_t enc: 1; // all the payload is encrypted
            uint16_t load_when_boot: 1; // load image when boot
            uint16_t enc_load: 1; // encrypt load part or not
            uint16_t enc_key_select: 3; // referenced to ENC_KEY_SELECT
            uint16_t not_ready : 1; //for copy image in ota
            uint16_t not_obsolete : 1; //for copy image in ota
            uint16_t rsvd: 7;
        };
    } ctrl_flag;
    uint16_t signature;
    uint16_t crc16;
    uint32_t image_length;
} _PACKED_ T_START_DFU_PARA;

typedef struct _T_PKT_RX_NOTIF_REQ
{
    uint16_t packet_num;
} _PACKED_ T_PKT_RX_NOTIF_REQ;


typedef struct _T_DFU_CTRL_POINT
{
    uint8_t opcode;
    union
    {
        T_START_DFU_PARA start_dfu;
        T_PKT_RX_NOTIF_REQ pkt_rx_notify_req;
    } p;
} T_DFU_CTRL_POINT, * P_DFU_CTRL_POINT;

/*Notifications defined here*/

typedef struct _TNOTIFICATION_TARGET_IMAGE_INFO
{
    uint16_t nOrigFwVersion;
    uint32_t nImageUpdateOffset;
} TNOTIFICATION_TARGET_IMAGE_INFO;

typedef struct _TNOTIFICATION_REPORT_PKT_NUM
{
    uint16_t PacketNum;
} TNOTIFICATION_REPORT_PKT_NUM;

typedef struct _TNOTIFICATION_REPORT_OTA_FUNC
{
    uint16_t OtaFuncVersion;
    uint32_t invalid;
} TNOTIFICATION_REPORT_OTA_FUNC;


typedef enum _TNOTIFICATION_REPORT_FUNC_VERSION
{
    NORMAL_FUNCTION = 0x0000,  /*normal function*/
    IMAGE_CHECK_FUNCTION = 0x0001  /*image check function*/
} TNOTIFICATION_REPORT_FUNC_VERSION;

typedef enum _DFU_BUFFER_IS_VALID
{
    DFU_BUFFER_VALID = 0x00,
    DFU_BUFFER_INVALID = 0x01
} DFU_BUFFER_IS_VALID;

typedef struct _DFUNotification
{
    uint8_t opCode;
    uint8_t reqOpCode;
    uint8_t respValue;
    union
    {
        TNOTIFICATION_TARGET_IMAGE_INFO NotifyTargetImageInfo;
        TNOTIFICATION_REPORT_PKT_NUM NotifyPktNum;
    } p;
} TDFUNotification, * PDFUNotification;

typedef enum
{
    DFU_CB_NONE,
    DFU_CB_START,
    DFU_CB_END,
    DFU_CB_FAIL
} dfu_cb_type_t;

typedef union
{
    uint8_t dull;
} dfu_cb_data_t;

/** Dfu service data to inform application */
typedef struct
{
    uint8_t conn_id;
    dfu_cb_type_t type;
    dfu_cb_data_t data;
} dfu_cb_msg_t;

typedef struct
{
    uint32_t origin_image_version;
    uint32_t curr_offset;
    uint8_t ic_type;
    //uint8_t ota_flag;
    uint8_t secure_version;
    union
    {
        uint16_t value;
        struct
        {
            uint16_t xip: 1; // payload is executed on flash
            uint16_t enc: 1; // all the payload is encrypted
            uint16_t load_when_boot: 1; // load image when boot
            uint16_t enc_load: 1; // encrypt load part or not
            uint16_t enc_key_select: 3; // referenced to ENC_KEY_SELECT
            uint16_t not_ready : 1; //for copy image in ota
            uint16_t not_obsolete : 1; //for copy image in ota
            uint16_t rsvd: 7;
        };
    } ctrl_flag;
    uint16_t signature; //!< app then patch
    uint32_t app_version;
    uint32_t patch_version;
    uint16_t crc16;
    uint32_t image_length;
    bool ota_conn_para_upd_in_progress;
    uint16_t mtu_size;
    uint32_t product_id;
    dfu_cb_type_t fsm;
    dfu_cb_type_t fsm_client;
    bool bg_scan;
} dfu_ctx_t;

extern dfu_ctx_t dfu_ctx;
extern uint8_t dfu_server_id;
extern const uint8_t dfu_service_uuid[16];
/** @} */

/** @defgroup DFU_Server_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief add service to the stack database.
  * @param[in] pcb: pointer of app callback function called by profile.
  * @return service ID auto generated by profile layer.
  * @retval server_id
  */
uint8_t dfu_server_add(void *pcb);

/**
  * @brief send the dfu adv to allow the client to discovery it
  * @return none
  */
void dfu_server_adv_send(void);

/**
  * @brief handle the disconnection
  * @param[in] conn_id: the connection id
  * @return none
  */
void dfu_server_disconnect_cb(uint8_t conn_id);

/** @} */
/** @} */

END_DECLS

#endif

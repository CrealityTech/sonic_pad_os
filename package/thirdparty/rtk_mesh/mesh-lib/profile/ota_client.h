/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ota_client.h
  * @brief    Head file for using ota service Client.
  * @details  Data structs and external functions declaration.
  * @author   bill
  * @date     2017-8-28
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _OTA_CLIENT_H_
#define _OTA_CLIENT_H_

/* Add Includes here */
#include "profileAPI.h"
#include "profile_client.h"
#include "ota_service.h"

BEGIN_DECLS

/** @addtogroup OTA_Client
  * @{
  */

/** @defgroup OTA_Client_Exported_Types Exported Types
  * @{
  */

/** @brief Handle cache for intrested UUIDs */
typedef enum
{
    HDL_OTA_SRV_START,           // start handle of simple ble service
    HDL_OTA_SRV_END,             // end handle of simple ble service
    HDL_OTA_ENTER_OTA_MODE,
    HDL_OTA_BD_ADDR,
    HDL_OTA_PATCH_VERSION,
    HDL_OTA_APP_VERSION,
    HDL_OTA_PATCH_EXT_VERSION,
    HDL_OTA_BANK_NUM,
    HDL_OTA_UPDATE_IMAGE_INFO,
    HDL_OTA_CACHE_LEN
} TOtaClientHandleType;

/** @brief used to inform app the discovery procedure is done or pending */
typedef enum
{
    OTA_DISC_DONE,
    OTA_DISC_PENDING
} TOtaClientDiscState, *POtaClientDiscState;

/** @brief handle information found in discovery procedure */
typedef struct
{
    TOtaClientHandleType  handle_type;
    uint16_t        handle_value;
} TOtaClientDiscHdlInfo;

/** @brief read request type, used by app to send read request */
typedef enum
{
    OTA_READ_REQ_BD_ADDR,
    OTA_READ_REQ_PATCH_VERSION,
    OTA_READ_REQ_APP_VERSION,
    OTA_READ_REQ_PATCH_EXT_VERSION,
    OTA_READ_REQ_BANK_NUM
} TOtaClientReadType, *POtaClientReadType;

/** @brief read result type, used to inform app which read value responsed */
typedef enum
{
    OTA_READ_RES_BD_ADDR,
    OTA_READ_RES_PATCH_VERSION,
    OTA_READ_RES_APP_VERSION,
    OTA_READ_RES_PATCH_EXT_VERSION,
    OTA_READ_RES_BANK_NUM,
    OTA_READ_RES_FAIL
} TOtaClientReadResult;

/** @brief read content, used to inform app read response data content */
typedef union
{
    uint8_t bd_addr[6];
    uint16_t patch_version;
    uint16_t app_version;
    uint16_t patch_ext_version;
    uint8_t bank_num;
} TOtaClientReadContent, *POtaClientReadContent;

/** @brief read data, used to inform app read response data */
typedef struct
{
    TOtaClientReadResult read_result;
    TOtaClientReadContent read_content;
} TOtaClientReadData;

/** @brief write request result */
typedef enum
{
    OTA_WRITE_RESULT_SUCCESS,
    OTA_WRITE_RESULT_FAIL
} TOtaClientWriteResult;

/** @brief notif/ind receive type */
typedef enum
{
    OTA_NOTIF_IND_RECEIVE_FAIL
} TOtaClientNotifIndReceiveType;

/** @brief notif/ind receive content */
typedef union
{
    uint8_t data;
} TOtaClientNotifIndReceiveContent, *POtaClientNotifIndReceiveContent;

/** @brief The notif/ind data received by the client from the server */
typedef struct
{
    TOtaClientNotifIndReceiveType receive_type;
    TOtaClientNotifIndReceiveContent receive_content;
} TOtaClientNotifIndData, *POtaClientNotifIndData;

/** @brief Event type to inform app */
typedef enum
{
    OTA_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    OTA_CLIENT_CB_TYPE_DISC_HDL,            //!< Discovered handle information.
    OTA_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    OTA_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    OTA_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    OTA_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} TOtaClientCB_Type;

/** @brief Callback content sent to application from the client */
typedef union
{
    TOtaClientDiscState disc_state;
    TOtaClientDiscHdlInfo disc_hdl_info;
    TOtaClientReadData read_data;
    TOtaClientWriteResult write_result;
    TOtaClientNotifIndData notif_ind_data;
} TOtaClientCB_Content;

/** @brief Callback data sent to application from the client, include type and content */
typedef struct
{
    TOtaClientCB_Type     cb_type;
    TOtaClientCB_Content    cb_content;
} TOtaClientCB_Data;
/** @} */

/** @defgroup OTA_Server_Exported_Functions Exported Functions
  * @{
  */

///@cond
TClientID Ota_AddClient(pfnSpecificClientAppCB_t appCB);
bool OtaClient_SetHandle(TOtaClientHandleType handle_type, uint16_t handle_value);
uint16_t OtaClient_GetHandle(TOtaClientHandleType handle_type);
bool OtaClient_StartDiscovery(void);
bool OtaClient_ReadByHandle(TOtaClientReadType readCharType);
bool OtaClient_ReadByUUID(TOtaClientReadType readCharType);
bool OtaClient_OtaDataNotifyCmd(bool command);
bool OtaClient_OtaDataWrite(uint8_t *pdata, uint16_t length);
///@endcond

/** @} */
/** @} */

END_DECLS

#endif  /* _OTA_CLIENT_H_ */

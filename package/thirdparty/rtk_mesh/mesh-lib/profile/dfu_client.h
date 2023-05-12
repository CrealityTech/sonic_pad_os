/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     dfu_client.h
  * @brief    Head file for using dfu service Client.
  * @details  Data structs and external functions declaration.
  * @author   bill
  * @date     2018-4-17
  * @version  v2.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _DFU_CLIENT_H_
#define _DFU_CLIENT_H_

/* Add Includes here */
#include "profile_client.h"
#include "dfu_server.h"

BEGIN_DECLS

/** @defgroup DFU_Client
  * @{
  */

/** @defgroup DFU_Client_Exported_Types Exported Types
  * @{
  */

/** @brief Handle cache for intrested UUIDs */
typedef enum
{
    HDL_DFU_SRV_START,           // start handle of simple ble service
    HDL_DFU_SRV_END,             // end handle of simple ble service
    HDL_DFU_DATA,
    HDL_DFU_CP,
    HDL_DFU_CP_END,
    HDL_DFU_CP_CCCD,
    HDL_DFU_CACHE_LEN
} dfu_handle_type_t;

/** @brief used to inform app the discovery procedure is done or pending */
typedef enum
{
    DFU_DISC_DONE,
    DFU_DISC_FAIL,
    DFU_DISC_NOT_FOUND
} dfu_disc_state_t;

/** @brief handle information found in discovery procedure */
typedef struct
{
    dfu_handle_type_t handle_type;
    uint16_t handle_value;
} dfu_disc_hdl_info_t;

/** @brief read request type, used by app to send read request */
typedef enum
{
    DFU_READ_CP_CCCD
} dfu_read_type_t;

/** @brief read content, used to inform app read response data content */
typedef union
{
    bool dfu_cp_cccd;
} dfu_read_data_t;

/** @brief read data, used to inform app read response data */
typedef struct
{
    dfu_read_type_t type;
    dfu_read_data_t data;
    uint16_t cause;
} dfu_read_result_t;

/** @brief write request result */
typedef enum
{
    DFU_WRITE_CP,
    DFU_WRITE_CP_CCCD,
    DFU_WRITE_DATA
} dfu_write_type_t;

typedef struct
{
    dfu_write_type_t type;
    uint16_t cause;
} dfu_write_result_t;

/** @brief notif/ind receive type */
typedef enum
{
    DFU_NOTIF_RECEIVE_DFU_CP_NOTIFY,
    DFU_NOTIF_IND_RECEIVE_FAIL
} dfu_notify_indicate_type_t;

/** @brief notif/ind receive content */
typedef struct
{
    uint16_t len;
    uint8_t *pdata;
} dfu_notify_indicate_value_t;

/** @brief The notif/ind data received by the client from the server */
typedef struct
{
    dfu_notify_indicate_type_t type;
    dfu_notify_indicate_value_t value;
} dfu_notify_indicate_data_t;

/** @brief Event type to inform app */
typedef enum
{
    DFU_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    DFU_CLIENT_CB_TYPE_DISC_HDL,            //!< Discovered handle information.
    DFU_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    DFU_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    DFU_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    DFU_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} dfu_client_cb_type_t;

/** @brief Callback content sent to application from the client */
typedef union
{
    dfu_disc_state_t disc_state;
    dfu_disc_hdl_info_t disc_hdl_info;
    dfu_read_result_t read_result;
    dfu_write_result_t write_result;
    dfu_notify_indicate_data_t notif_ind_data;
} dfu_client_cb_content_t;

/** @brief Callback data sent to application from the client, include type and content */
typedef struct
{
    dfu_client_cb_type_t cb_type;
    dfu_client_cb_content_t cb_content;
} dfu_client_cb_data_t;

typedef enum
{
    DFU_FSM_WRITE_DO_NOT_CARE,
    DFU_FSM_WRITE_CCCD_ENABLE,
    DFU_FSM_WRITE_DFU_DATA,
    DFU_FSM_WRITE_WAIT_WRITE_RESP
} dfu_client_fsm_write_t;

typedef enum
{
    DFU_FSM_AUTO_CONN_SCAN, //!< adv
    DFU_FSM_AUTO_CONN_CONNECT, //!< connect but don't ota
    DFU_FSM_AUTO_CONN_OTA //!< ota
} dfu_fsm_auto_connect_t;

extern T_CLIENT_ID dfu_client_id;
/** @} */

/** @defgroup DFU_Client_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief register the client
  * @param[in] app_cb: the callback function
  * @return client id
  */
T_CLIENT_ID dfu_client_add(P_FUN_GENERAL_APP_CB app_cb);

/**
  * @brief start the service discovery
  *
  * contains the service/characteristic/cccd declaration
  * @param[in] conn_id: the connection id
  * @return the operatino result
  */
bool dfu_client_start_discovery(uint8_t conn_id);

/**
  * @brief handle the adv pkt
  * @param[in] p_le_scan_info: the adv pkt pointer
  * @return none
  */
void dfu_client_handle_adv_pkt(T_LE_SCAN_INFO *p_le_scan_info);

/**
  * @brief Get the conn id when actively connect.
  * @param[in] conn_id: the connection id
  * @return none
  */
void dfu_client_handle_connecting(uint8_t conn_id);

/**
  * @brief handle the establish of connection
  *
  * discovery the dfu server etc.
  * @param[in] conn_id: the connection id
  * @return none
  */
void dfu_client_handle_connect(uint8_t conn_id);

/**
  * @brief handle the disconnetion
  *
  * to release the resource and notify the dfu state
  * @param[in] conn_id: the connection id
  * @return the operatino result
  */
void dfu_client_disconnect_cb(uint8_t conn_id);

/** @} */
/** @} */

END_DECLS

#endif  /* _DFU_CLIENT_H_ */

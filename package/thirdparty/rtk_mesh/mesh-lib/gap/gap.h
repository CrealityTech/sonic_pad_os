/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap.h
  * @brief   This file contains all the constants and functions prototypes for GAP.
  * @details This file is used both bredr and le.
  * @author  jane
  * @date    2017-02-18
  * @version v1.0
  * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_H
#define GAP_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include <bt_types.h>


/** @addtogroup GAP_COMMON GAP Common
  * @brief GAP common module
  * @{
  */

/*============================================================================*
 *                         Micros
 *============================================================================*/
/** @defgroup GAP_Common_Exported_Macros GAP Common Exported Macros
  * @{
  */

/** @defgroup GAP_Common_Macros GAP Common Macros
  * @{
  */
#define GAP_BD_ADDR_LEN               6     //!< Default Bluetooth Device Address Length.
#define GAP_KEY_LEN                   16    //!< Default Long Term Key(LTK) length.
#define GAP_DEVICE_NAME_LEN           (39+1)//!< Max length of device name, if device name length exceeds it, it will be truncated.
#define GAP_PASSCODE_MAX              999999//!< Maximum Pairing Passcode/Passkey value.  Range of a passkey can be 0 - 999,999.
#define GAP_OOB_LEN                   16    //!< Length of out of band data.
/**
  * @}
  */

/** @defgroup BOND_PAIRING_MODE_DEFINES Pairing Modes
 * @{
 */
#define GAP_PAIRING_MODE_NO_PAIRING          0x00  //!< Pairing is not allowed.
#define GAP_PAIRING_MODE_PAIRABLE            0x01  //!< Pairable, Wait for a pairing request from master or security request from slave.
/**
  * @}
  */

/** @defgroup BOND_MITM_DEFINES Mitm And Bonding Flags
 * @{
 */
#define GAP_AUTHEN_BIT_NONE                  0      //!<  No authentication required.
#define GAP_AUTHEN_BIT_BONDING_FLAG          0x0001 //!<  Bonding is required
#define GAP_AUTHEN_BIT_MITM_FLAG             0x0004 //!<  Mitm is preferred
#define GAP_AUTHEN_BIT_SC_FLAG               0x0008 //!<  Secure connection is preferred
#define GAP_AUTHEN_BIT_KEYPRESS_FLAG         0x0010 //!<  Keypress notifaction is preferred
#define GAP_AUTHEN_BIT_FORCE_BONDING_FLAG    0x0100 //!<  Force bonding is required
#define GAP_AUTHEN_BIT_SC_ONLY_FLAG          0x0200 //!<  Secure connection only mode for BLE is required
/**
  * @}
  */

/** @defgroup GAP_COMMON_MSG_TYPE GAP Msg Type
 * @{
 */
#define GAP_MSG_WRITE_AIRPLAN_MODE      0x00 //!<gap_write_airplan_mode
#define GAP_MSG_READ_AIRPLAN_MODE       0x01 //!<gap_read_airplan_mode
/**
  * @}
  */

/** End of GAP_Common_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_Common_Exported_Types GAP Common Exported Types
  * @{
  */

/** @brief I/O Capabilities */
typedef enum
{
    GAP_IO_CAP_DISPLAY_ONLY,        //!<  Only a Display present, no Keyboard or Yes/No Keys.
    GAP_IO_CAP_DISPLAY_YES_NO,      //!<  Display and Yes/No Keys present.
    GAP_IO_CAP_KEYBOARD_ONLY,       //!<  Only a Keyboard present, no Display.
    GAP_IO_CAP_NO_INPUT_NO_OUTPUT,  //!<  No input/output capabilities.
    GAP_IO_CAP_KEYBOARD_DISPLAY,    //!<  Keyboard and Display present.
} T_GAP_IO_CAP;

/** @brief Key types */
typedef enum
{
    GAP_KEY_LE_LOCAL_LTK          = 0x11, /**< BLE Long Term Key Blob. */
    GAP_KEY_LE_REMOTE_LTK         = 0x12, /**< BLE Long Term Key Blob. */
    GAP_KEY_LE_LOCAL_IRK          = 0x13, /**< BLE Identity Resolving Key. */
    GAP_KEY_LE_REMOTE_IRK         = 0x14, /**< BLE Identity Resolving Key. */
    GAP_KEY_LE_LOCAL_CSRK         = 0x15, /**< BLE Local CSR Key. */
    GAP_KEY_LE_REMOTE_CSRK        = 0x16, /**< BLE Remote CSR Key. */
    GAP_KEY_DELETE                = 0xFF  /**< Link key is no longer valid and should be deleted. */
} T_GAP_KEY_TYPE;

/** @brief  Keypress notification type.*/
typedef enum
{
    GAP_PASSKEY_ENTRY_STARTED,     /**< The passkey key entry protocol procedure is started. */
    GAP_PASSKEY_DIGIT_ENTERED,     /**< A digit is entered by the remote user. */
    GAP_PASSKEY_DIGIT_ERASED,      /**< A digit is erased by the remote user. */
    GAP_PASSKEY_CLEARED,           /**< The display is cleared by the remote user. */
    GAP_PASSKEY_ENTRY_CMPL,        /**< The passkey key entry protocol procedure is completed. */
} T_GAP_KEYPRESS_NOTIF_TYPE;

/** @brief GAP Remote Address Type */
typedef enum
{
    GAP_REMOTE_ADDR_LE_PUBLIC          = 0x00, /**< LE Public device address type. */
    GAP_REMOTE_ADDR_LE_RANDOM          = 0x01, /**< LE Randon device address type. */
    GAP_REMOTE_ADDR_LE_PUBLIC_IDENTITY = 0x02, /**< LE Public identity address type. */
    GAP_REMOTE_ADDR_LE_RANDOM_IDENTITY = 0x03, /**< LE Randon identity address type. */
    GAP_REMOTE_ADDR_LE_ANONYMOUS       = 0xff, /**< LE anonymous device address type. */
} T_GAP_REMOTE_ADDR_TYPE;

/** @brief GAP Cause List */
typedef enum
{
    GAP_CAUSE_SUCCESS           = 0x00,//!< Operation success.
    GAP_CAUSE_ALREADY_IN_REQ    = 0x01,//!< Operation already in progress.
    GAP_CAUSE_INVALID_STATE     = 0x02,//!< Invalid state.
    GAP_CAUSE_INVALID_PARAM     = 0x03,//!< Invalid parameter.
    GAP_CAUSE_NON_CONN          = 0x04,//!< No connection establishment.
    GAP_CAUSE_NOT_FIND_IRK      = 0x05,//!< IRK not found.
    GAP_CAUSE_ERROR_CREDITS     = 0x06,//!< Credits error.
    GAP_CAUSE_SEND_REQ_FAILED   = 0x07,//!< Send Request failed.
    GAP_CAUSE_NO_RESOURCE       = 0x08,//!< No resource.
    GAP_CAUSE_INVALID_PDU_SIZE  = 0x09,//!< Invalid PDU size.
    GAP_CAUSE_NOT_FIND          = 0x0a,//!< Not Found.
    GAP_CAUSE_CONN_LIMIT        = 0x0b,//!< Connection reachs limited count.
    GAP_CAUSE_NO_BOND           = 0x0c,//!< No Bond.
    GAP_CAUSE_ERROR_UNKNOWN     = 0xFF,//!< Unknown error.
} T_GAP_CAUSE;

/** @brief  GAP Confirmation Cause List */
typedef enum
{
    GAP_CFM_CAUSE_ACCEPT = (GAP_ERR | GAP_ACCEPT),    /**< Accept for indication, only used when confirmation. */
    GAP_CFM_CAUSE_REJECT = (GAP_ERR | GAP_ERR_REJECT),/**< Reject for indication, only used when confirmation. */
} T_GAP_CFM_CAUSE;

/** @brief  APP Return Result List */
typedef enum
{
    APP_RESULT_SUCCESS                    = (APP_SUCCESS),
    APP_RESULT_PENDING                    = (APP_ERR | APP_ERR_PENDING),
    APP_RESULT_ACCEPT                     = (APP_ERR | APP_ERR_ACCEPT),
    APP_RESULT_REJECT                     = (APP_ERR | APP_ERR_REJECT),
    APP_RESULT_NOT_RELEASE                = (APP_ERR | APP_ERR_NOT_RELEASE),

    APP_RESULT_PREP_QUEUE_FULL            = (ATT_ERR | ATT_ERR_PREP_QUEUE_FULL),
    APP_RESULT_INVALID_OFFSET             = (ATT_ERR | ATT_ERR_INVALID_OFFSET),
    APP_RESULT_INVALID_VALUE_SIZE         = (ATT_ERR | ATT_ERR_INVALID_VALUE_SIZE),
    APP_RESULT_INVALID_PDU                = (ATT_ERR | ATT_ERR_INVALID_PDU),
    APP_RESULT_ATTR_NOT_FOUND             = (ATT_ERR | ATT_ERR_ATTR_NOT_FOUND),
    APP_RESULT_ATTR_NOT_LONG              = (ATT_ERR | ATT_ERR_ATTR_NOT_LONG),
    APP_RESULT_INSUFFICIENT_RESOURCES     = (ATT_ERR | ATT_ERR_INSUFFICIENT_RESOURCES),
    APP_RESULT_APP_ERR                    = (ATT_ERR | ATT_ERR_MIN_APPLIC_CODE),
    APP_RESULT_CCCD_IMPROPERLY_CONFIGURED = (ATT_ERR | ATT_ERR_CCCD_IMPROPERLY_CONFIGURED),
    APP_RESULT_PROC_ALREADY_IN_PROGRESS   = (ATT_ERR | ATT_ERR_PROC_ALREADY_IN_PROGRESS),
} T_APP_RESULT;

/** @brief GAP Parameter List */
typedef enum
{
    GAP_PARAM_BD_ADDR                        = 0x200,//!< Locol Device's Address. Read Only. Size is uint8[GAP_BD_ADDR_LEN]. This item is read from the controller.
    GAP_PARAM_BOND_PAIRING_MODE              = 0x202,//!< Pairing Mode. Read/Write. Size is uint8_t. Default is GAP_PAIRING_MODE_PAIRABLE (@ref BOND_PAIRING_MODE_DEFINES).
    GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS = 0x203,//!< Man-In-The-Middle (MITM) basically turns on Passkey protection in the pairing algorithm. Read/Write. Size is uint8_t. Default is GAP_AUTHEN_NO_MITM_YES_BOND (@ref BOND_MITM_DEFINES).
    GAP_PARAM_BOND_IO_CAPABILITIES           = 0x204,//!< I/O capabilities.  Read/Write. Size is uint8_t. Default is GAP_IO_CAP_NO_INPUT_NO_OUTPUT (@ref T_GAP_IO_CAP).
    GAP_PARAM_BOND_OOB_ENABLED               = 0x205,//!< OOB data available for pairing algorithm. Read/Write. Size is uint8_t. Default is 0(disabled).
} T_GAP_PARAM_TYPE;


/** @brief Reponse for write airplane mode */
typedef struct
{
    uint16_t        cause; //!< Write response result.
} T_GAP_WRITE_AIRPLAN_MODE_RSP;

/** @brief Reponse for read airplane mode */
typedef struct
{
    uint16_t        cause;  //!< Read response result.
    uint8_t         mode;   //!< Current airplane mode.
} T_GAP_READ_AIRPLAN_MODE_RSP;

/** @brief Reponse for airplane mode */
typedef union
{
    T_GAP_WRITE_AIRPLAN_MODE_RSP *p_gap_write_airplan_mode_rsp; //!< Write airplane mode response.
    T_GAP_READ_AIRPLAN_MODE_RSP  *p_gap_read_airplan_mode_rsp;//!< Read airplane mode response.
} T_GAP_CB_DATA;

/** End of GAP_Common_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * @defgroup GAP_COMMON_EXPORT_Functions GAP Common Exported Functions
 *
 * @{
 */

/**
  * @brief Callback for gap to notify app
  * @param[in] cb_type callback msy type @ref GAP_COMMON_MSG_TYPE.
  * @param[in] p_cb_data point to callback data @ref T_GAP_CB_DATA.
  * @retval void
  */
typedef void(*P_FUN_GAP_APP_CB)(uint8_t cb_type, void *p_cb_data);

/**
 * @brief  Make upper stack ready to use by sending the register request.
 * @param[in] evt_queue     Event queue handle which is created by APP.
 * @param[in] io_queue      IO message queue handle which is created by APP.
 * @param[in] msg_queue_elem_num  Number of GAP message queue which will be created by GAP module.
 * @return result
 * @retval true  Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_main_task(void *p_param)
    {
        uint8_t event;
        os_msg_queue_create(&io_queue_handle, MAX_NUMBER_OF_IO_MESSAGE, sizeof(T_IO_MSG));
        os_msg_queue_create(&evt_queue_handle, MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint8_t));

        gap_start_bt_stack(evt_queue_handle, io_queue_handle, MAX_NUMBER_OF_GAP_MESSAGE);

        while (true)
        {
            if (os_event_recv(evt_queue_handle, &event, 0xFFFFFFFF) == true)
            {
                if (event == EVENT_IO_TO_APP)
                {
                    T_IO_MSG io_msg;
                    if (os_msg_recv(io_queue_handle, &io_msg, 0) == true)
                    {
                        app_handle_io_msg(io_msg);
                    }
                }
                else
                {
                    gap_handle_msg(event);
                }
            }
        }
    }
 * \endcode
 */
//bool gap_start_bt_stack(void *evt_queue, void *io_queue, uint16_t msg_queue_elem_num);

/**
 * @brief  Initialize gap lib to use some extended functions.
 * @return void
 * <b>Example usage</b>
 * \code{.c}
    int main(void)
    {
        ...
        gap_lib_init();
        ...
    }
 * \endcode
 */
//void gap_lib_init(void);

/**
 * @brief  Register callback to gap, when messages in @ref GAP_COMMON_MSG_TYPE happens, it will callback to app.
 * @param[in] app_callback Callback function provided by the APP to handle gap common messages sent from the GAP.
 *              @arg NULL -> Not send gap common messages to APP.
 *              @arg Other -> Use application defined callback function.
 * @return void
 *
 * <b>Example usage</b>
 * \code{.c}
   void app_le_gap_init()
   {
       ...
       le_register_app_cb(app_gap_callback);
       gap_register_app_cb(app_gap_common_callback);
   }
   \endcode
 */
//void gap_register_app_cb(P_FUN_GAP_APP_CB app_callback);

/**
  * @brief  Set a GAP parameter.
  *
  *         NOTE: You can call this function with a GAP Common Parameter type and it will set the
  *         GAP Parameter.  GAP Peripheral Parameters are defined in @ref T_GAP_PARAM_TYPE.
  *         If the "len" field sets to the size of a "uint16_t" ,the
  *         "p_value" field must point to a data with type of "uint16_t".
  *
  * @param[in]  param GAP parameter ID: @ref T_GAP_PARAM_TYPE.
  * @param[in]  len Length of data to write.
  * @param[in]  p_value Pointer to data to write.  This is dependent on
  *                  the parameter type and WILL be cast to the appropriate
  *                  data type (For example: if data type param is uint16, p_value will be cast to
  *                  pointer of uint16_t).
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void app_le_gap_init(void)
    {
        ...
        //GAP Bond Manager parameters
        uint8_t  auth_pair_mode = GAP_PAIRING_MODE_PAIRABLE;
        uint16_t auth_flags = GAP_AUTHEN_BIT_BONDING_FLAG | GAP_AUTHEN_BIT_MITM_FLAG;
        uint8_t  auth_io_cap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
        uint8_t  auth_oob = false;

        // Setup the GAP Bond Manager
        gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
        gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(auth_flags), &auth_flags);
        gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(auth_io_cap), &auth_io_cap);
        gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(auth_oob), &auth_oob);
        ...
    }
  * \endcode
  */
//T_GAP_CAUSE gap_set_param(T_GAP_PARAM_TYPE param, uint8_t len, void *p_value);

/**
  * @brief  Get a GAP Common parameter.
  *
  *         NOTE: You can call this function with a GAP Common Parameter type and it will get a
  *         GAP Common Parameter.  GAP Common Parameters are defined in @ref T_GAP_PARAM_TYPE.
  *
  * @param[in]  param GAP parameter type: @ref T_GAP_PARAM_TYPE
  * @param[in,out]  p_value Pointer to location to get the value.  This is dependent on
  *                  the parameter ID and WILL be cast to the appropriate
  *                  data type (For example: if data type param is uint16, p_value will be cast to
  *                  pointer of uint16_t).
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        uint8_t bt_addr[6];
        gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
    }
  * \endcode
  */
//T_GAP_CAUSE gap_get_param(T_GAP_PARAM_TYPE param, void *p_value);

/**
  * @brief  Make the device in pairable mode.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  */
//T_GAP_CAUSE gap_set_pairable_mode(void);

/**
 * @brief   Write command to control device to enter airplane mode, the result will be notified by callback
            regisgtered by @ref gap_register_app_cb.
 * @param[in] mode the mode of the device
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_wairplane(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        T_GAP_CAUSE cause;
        uint8_t mode = p_parse_value->dw_param[0];
        cause = gap_write_airplan_mode(mode);
        return (T_USER_CMD_PARSE_RESULT)cause;
    }

    void app_gap_common_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_GAP_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_GAP_CB_DATA));
        APP_PRINT_INFO1("app_gap_common_callback: cb_type = %d", cb_type);
        switch (cb_type)
        {
        case GAP_MSG_WRITE_AIRPLAN_MODE:
            APP_PRINT_INFO1("GAP_MSG_WRITE_AIRPLAN_MODE: cause 0x%x",
                            cb_data.p_gap_write_airplan_mode_rsp->cause);
            break;
        case GAP_MSG_READ_AIRPLAN_MODE:
            APP_PRINT_INFO2("GAP_MSG_READ_AIRPLAN_MODE: cause 0x%x, mode %d",
                            cb_data.p_gap_read_airplan_mode_rsp->cause,
                            cb_data.p_gap_read_airplan_mode_rsp->mode);
            break;
        default:
            break;
        }
        return;
    }
 * \endcode
 */

//T_GAP_CAUSE gap_write_airplan_mode(uint8_t mode);

/**
 * @brief   Read the airplane mode of the device, the result will be notified by callback regisgtered
            by @ref gap_register_app_cb.
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_rairplane(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        T_GAP_CAUSE cause;
        cause = gap_read_airplan_mode();
        return (T_USER_CMD_PARSE_RESULT)cause;
    }

    void app_gap_common_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_GAP_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_GAP_CB_DATA));
        APP_PRINT_INFO1("app_gap_common_callback: cb_type = %d", cb_type);
        switch (cb_type)
        {
        case GAP_MSG_WRITE_AIRPLAN_MODE:
            APP_PRINT_INFO1("GAP_MSG_WRITE_AIRPLAN_MODE: cause 0x%x",
                            cb_data.p_gap_write_airplan_mode_rsp->cause);
            break;
        case GAP_MSG_READ_AIRPLAN_MODE:
            APP_PRINT_INFO2("GAP_MSG_READ_AIRPLAN_MODE: cause 0x%x, mode %d",
                            cb_data.p_gap_read_airplan_mode_rsp->cause,
                            cb_data.p_gap_read_airplan_mode_rsp->mode);
            break;
        default:
            break;
        }
        return;
    }
 * \endcode
 */
//T_GAP_CAUSE gap_read_airplan_mode(void);

/**
 * @brief  Handle GAP message received from BT stack or timeout event for GAP timer.
 * @param[in] event Event type which is received from APP event queue.
 * @return None.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_main_task(void *p_param)
    {
        uint8_t event;
        os_msg_queue_create(&io_queue_handle, MAX_NUMBER_OF_IO_MESSAGE, sizeof(T_IO_MSG));
        os_msg_queue_create(&evt_queue_handle, MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint8_t));

        gap_start_bt_stack(evt_queue_handle, io_queue_handle, MAX_NUMBER_OF_GAP_MESSAGE);

        while (true)
        {
            if (os_event_recv(evt_queue_handle, &event, 0xFFFFFFFF) == true)
            {
                if (event == EVENT_IO_TO_APP)
                {
                    T_IO_MSG io_msg;
                    if (os_msg_recv(io_queue_handle, &io_msg, 0) == true)
                    {
                        app_handle_io_msg(io_msg);
                    }
                }
                else
                {
                    gap_handle_msg(event);
                }
            }
        }
    }
 * \endcode
 */
//void gap_handle_msg(uint8_t event);

/**
 * @brief  Release the buffer of GAP supplied.
 * @param[in] p_buf Point the buffer that needs to release.
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    uint8_t *p_data_buf;
    uint16_t data_offset;
    T_APP_RESULT simp_ble_service_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id,
                                            uint16_t attrib_index, T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                            P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
    {
        ......
        server_get_write_cmd_data_buffer(conn_id, &p_data_buf, &data_offset);
        return APP_RESULT_NOT_RELEASE;
    }
    void release(void)
    {
        if(p_data_buf != NULL)
        {
            gap_buffer_free(p_data_buf);
            p_data_buf = NULL;
        }
    }
 * \endcode
 */
//T_GAP_CAUSE gap_buffer_free(void *p_buf);
/** @} */ /* End of group GAP_COMMON_EXPORT_Functions */

/** @} */ /* End of group GAP_COMMON */

/*-------------------------------------------------------------------
-------------------------------------------------------------------*/
void gap_set_param(T_GAP_PARAM_TYPE param, uint8_t len, void *p_value);
uint8_t gap_get_param(T_GAP_PARAM_TYPE param, void *p_value);

#ifdef __cplusplus
}
#endif

#endif /* GAP_H */

/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    dfu_client.c
  * @brief   dfu service client source file.
  * @details
  * @author  bill
  * @date    2017-8-28
  * @version v1.0
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2015 Realtek Semiconductor Corporation</center></h2>
  ******************************************************************************
  */

/** Add Includes here **/
#include "platform_diagnose.h"
#include "dfu_client.h"
#include "data_uart.h"
#include "mem_config.h"
#include "mesh_api.h"
#include "patch_header_check.h"
#include "gap_manufacture_adv.h"
#include "dfu_flash.h"

/**
  * @{ Used for CCCD handle discovering in discovery procedure
  */
typedef enum
{
    DFU_CCCD_DISC_START,
    DFU_CCCD_DISC_CP_NOTIFY,
    DFU_CCCD_DISC_END
} dfu_cccd_disc_state_t;

/**
  * @}
  */
#define BUFFER_CHECK_ENABLE          1

uint8_t CpMsg[20];
uint8_t CpMsgLen;

dfu_client_fsm_write_t dfu_fsm_write = DFU_FSM_WRITE_DO_NOT_CARE;
dfu_fsm_auto_connect_t dfu_fsm_auto_connect = DFU_FSM_AUTO_CONN_SCAN;
uint16_t buffer_check_pkt_num;
uint16_t buffer_check_pkt_count;
uint16_t buffer_check_crc;

#define DFU_CLIENT_INVALID_CONN_ID 0xff
struct
{
    uint8_t bt_addr[6];
    T_GAP_REMOTE_ADDR_TYPE addr_type;
    uint8_t conn_id;
} dfu_client_ctx;

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
const uint8_t dfu_data_uuid[16] = {GATT_UUID128_DFU_DATA};
const uint8_t dfu_cp_uuid[16] = {GATT_UUID128_DFU_CONTROL_POINT};

/**<  Dfu client ID. */
T_CLIENT_ID dfu_client_id = CLIENT_PROFILE_GENERAL_ID;
/**<  Dfu discovery end handle control. */
static dfu_handle_type_t dfu_end_hdl_idx = HDL_DFU_CACHE_LEN;
/**<  Callback used to send data to app from Dfu client layer. */
static P_FUN_GENERAL_APP_CB pf_dfu_client_app_cb = NULL;
static P_FUN_GENERAL_APP_CB pf_dfu_client_app_info_cb = NULL;
/**<  Handle Cache for simple BLE client to cache discovered handles. */
static uint16_t dfu_hdl_cache[HDL_DFU_CACHE_LEN] = {0};
/**<  Discovery State indicate which CCCD is in discovery. */
static dfu_cccd_disc_state_t dfu_cccd_disc_state = DFU_CCCD_DISC_START;

static void dfu_client_init(void);

/**
  * @brief  Used by application, to set the handles in Dfu handle cache.
  * @param  handle_type: handle types of this specific profile.
  * @param  handle_value: handle value to set.
  * @retval TRUE--set success.
  *         FALSE--set failed.
  */
bool dfu_client_handle_set(dfu_handle_type_t handle_type, uint16_t handle_value)
{
    if (handle_type < HDL_DFU_CACHE_LEN)
    {
        dfu_hdl_cache[handle_type] = handle_value;
        return TRUE;
    }
    return FALSE;
}

/**
  * @brief  Used by application, to get the handles in Dfu handle cache.
  * @param  handle_type: handle types of this specific profile.
  * @retval handle value.
  */
uint16_t dfu_client_handle_get(dfu_handle_type_t handle_type)
{
    return dfu_hdl_cache[handle_type];
}

/**
  * @brief  Used by application, to start the discovery procedure of Dfu server.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
bool dfu_client_start_discovery(uint8_t conn_id)
{
    /* the connection id shall also be set when not auto ota */
    dfu_client_ctx.conn_id = conn_id;
    printi("dfu_client_start_discovery");
    /* First clear handle cache. */
    memset(dfu_hdl_cache, 0, sizeof(dfu_hdl_cache));
    return (client_by_uuid128_srv_discovery(conn_id, dfu_client_id,
                                            (uint8_t *)dfu_service_uuid) == GAP_CAUSE_SUCCESS);
}

/**
  * @brief  Used internal, start the discovery of Dfu characteristics.
  *         NOTE--user can offer this interface for application if required.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
static bool dfu_client_start_char_discovery(uint8_t conn_id)
{
    printi("dfu_client_start_char_discovery");
    return (client_all_char_discovery(conn_id, dfu_client_id, dfu_hdl_cache[HDL_DFU_SRV_START],
                                      dfu_hdl_cache[HDL_DFU_SRV_END]) == GAP_CAUSE_SUCCESS);
}

/**
  * @brief  Used internal, start the discovery of Dfu characteristics descriptor.
  *         NOTE--user can offer this interface for application if required.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
static bool dfu_client_start_char_descriptor_discovery(uint8_t conn_id, uint16_t start_handle,
                                                       uint16_t end_handle)
{
    printi("dfu_client_start_char_descriptor_discovery");
    return (client_all_char_descriptor_discovery(conn_id, dfu_client_id, start_handle,
                                                 end_handle) == GAP_CAUSE_SUCCESS);
}

/**
  * @brief  Used by application, read data from server by using handles.
  * @param  readCharType: one of characteristic that has the readable property.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
bool dfu_client_read_by_handle(uint8_t conn_id, dfu_read_type_t type)
{
    uint16_t handle = 0;
    switch (type)
    {
    case DFU_READ_CP_CCCD:
        handle = dfu_hdl_cache[HDL_DFU_CP_CCCD];
        break;
    default:
        return FALSE;
    }

    if (handle)
    {
        if (client_attr_read(conn_id, dfu_client_id, handle) == GAP_CAUSE_SUCCESS)
        {
            return TRUE;
        }
    }

    printe("dfu_client_read_by_handle: Request fail! Please check!");
    return FALSE;
}

/**
  * @brief  Used by application, read data from server by using UUIDs.
  * @param  readCharType: one of characteristic that has the readable property.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
bool dfu_client_read_by_uuid(uint8_t conn_id, dfu_read_type_t type)
{
    uint16_t start_handle;
    uint16_t end_handle;
    uint16_t uuid16;
    switch (type)
    {
    case DFU_READ_CP_CCCD:
        start_handle = dfu_hdl_cache[HDL_DFU_CP] + 1;
        end_handle = dfu_hdl_cache[HDL_DFU_CP_END];
        uuid16 = GATT_UUID_CHAR_CLIENT_CONFIG;
        break;
    default:
        return FALSE;
    }

    if (client_attr_read_using_uuid(conn_id, dfu_client_id, start_handle, end_handle, uuid16,
                                    NULL) == GAP_CAUSE_SUCCESS)
    {
        return TRUE;
    }
    printe("dfu_client_read_by_uuid: Request fail! Please check!");
    return FALSE;
}

/**
  * @brief  Used by application, to enable or disable the notification of peer server's V3 Notify Characteristic.
  * @param  command: 0--disable the notification, 1--enable the notification.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
bool dfu_client_cp_cccd_set(uint8_t conn_id, bool command)
{
    uint16_t handle;
    uint16_t length;
    uint8_t *pdata;
    uint16_t cccd_value;
    handle = dfu_hdl_cache[HDL_DFU_CP_CCCD];
    if (handle)
    {
        cccd_value = command ? 1 : 0;
        length = sizeof(uint16_t);
        pdata = (uint8_t *)&cccd_value;
        if (client_attr_write(conn_id, dfu_client_id, GATT_WRITE_TYPE_REQ, handle, length,
                              pdata) == GAP_CAUSE_SUCCESS)
        {
            return TRUE;
        }
    }

    printe("dfu_client_cp_cccd_set: Request fail! Please check!");
    return FALSE;
}

/**
  * @brief  Used internal, to send write request to peer server's V5 Control Point Characteristic.
  * @param  ctl_pnt_ptr: pointer of control point data to write.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
bool dfu_client_cp_write(uint8_t conn_id, uint8_t *pdata, uint16_t length)
{
    uint16_t handle;
    printi("dfu_client_cp_write: opcode = %d lenth = %d.", pdata[0], length);
    dprintt(pdata, length);
    dfu_fsm_write = DFU_FSM_WRITE_WAIT_WRITE_RESP;
    handle = dfu_hdl_cache[HDL_DFU_CP];
    if (handle)
    {
        if (client_attr_write(conn_id, dfu_client_id, GATT_WRITE_TYPE_REQ, handle, length,
                              pdata) == GAP_CAUSE_SUCCESS)
        {
            return TRUE;
        }
    }
    printe("dfu_client_cp_write: Request fail! Please check!");
    return FALSE;
}

/**
  * @brief  Used internal, to send write request to peer server's V5 Control Point Characteristic.
  * @param  ctl_pnt_ptr: pointer of control point data to write.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
bool dfu_client_data_write(uint8_t conn_id, uint8_t *pdata, uint16_t length)
{
    uint16_t handle;
    dfu_fsm_write = DFU_FSM_WRITE_DFU_DATA;
    handle = dfu_hdl_cache[HDL_DFU_DATA];
    if (handle && client_attr_write(conn_id, dfu_client_id, GATT_WRITE_TYPE_CMD, handle, length,
                                    pdata) == GAP_CAUSE_SUCCESS)
    {
        return TRUE;
    }
    printe("dfu_client_data_write: Request fail! Please check!");
    return FALSE;
}

/**
  * @brief  Used internal, switch to the next CCCD handle to be discovered.
  *         NOTE--this function only used when peer service has more than one CCCD.
  * @param  pSwitchState: CCCD discovery state.
  * @retval none.
  */
static void dfu_client_switch_next_descriptor(dfu_cccd_disc_state_t *cccd_state)
{
    dfu_cccd_disc_state_t new_state;
    switch (*cccd_state)
    {
    case DFU_CCCD_DISC_START:
        if (dfu_hdl_cache[HDL_DFU_CP_END])
        {
            new_state = DFU_CCCD_DISC_CP_NOTIFY;
        }
        else
        {
            new_state = DFU_CCCD_DISC_END;
        }
        break;
    case DFU_CCCD_DISC_CP_NOTIFY:
        new_state = DFU_CCCD_DISC_END;
        break;
    default:
        new_state = DFU_CCCD_DISC_END;
        break;
    }

    *cccd_state = new_state;
}

/**
  * @brief  Called by profile client layer, when discover state of discovery procedure changed.
  * @param  discoveryState: current service discovery state.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static void dfu_client_disc_state_cb(uint8_t conn_id, T_DISCOVERY_STATE disc_state)
{
    bool cb_flag = FALSE;
    bool descriptor_disc_flag = FALSE;
    uint16_t start_handle;
    uint16_t end_handle;
    dfu_client_cb_data_t cb_data;
    cb_data.cb_type = DFU_CLIENT_CB_TYPE_DISC_STATE;
    printi("dfu_client_disc_state_cb: disc_state = %d", disc_state);
    switch (disc_state)
    {
    case DISC_STATE_SRV_DONE:
        /* Indicate that service handle found. Start discover characteristic. */
        if ((dfu_hdl_cache[HDL_DFU_SRV_START] != 0)
            || (dfu_hdl_cache[HDL_DFU_SRV_END] != 0))
        {
            if (dfu_client_start_char_discovery(conn_id) == FALSE)
            {
                cb_data.cb_content.disc_state = DFU_DISC_FAIL;
                cb_flag = TRUE;
            }
        }
        /* No Dfu service handle found. Discover procedure complete. */
        else
        {
            cb_data.cb_content.disc_state = DFU_DISC_NOT_FOUND;
            cb_flag = TRUE;
        }
        break;
    case DISC_STATE_CHAR_DONE:
        /* We should store the last char end handle if needed. */
        if (dfu_end_hdl_idx < HDL_DFU_CACHE_LEN)
        {
            dfu_hdl_cache[dfu_end_hdl_idx] = dfu_hdl_cache[HDL_DFU_SRV_END];
            dfu_end_hdl_idx = HDL_DFU_CACHE_LEN;
        }
        /* Find the first descriptor to be discovered. */
        dfu_cccd_disc_state = DFU_CCCD_DISC_START;
        dfu_client_switch_next_descriptor(&dfu_cccd_disc_state);
        break;
    case DISC_STATE_CHAR_DESCRIPTOR_DONE:
        /* Find the next descriptor to be discovered. */
        dfu_client_switch_next_descriptor(&dfu_cccd_disc_state);
        break;
    default:
        printw("dfu_client_disc_state_cb: Invalid Discovery State!");
        break;
    }

    /* Switch different char descriptor discovery, if has multi char descriptors. */
    switch (dfu_cccd_disc_state)
    {
    case DFU_CCCD_DISC_CP_NOTIFY:
        /* Need to discover v3 notify char descriptor. */
        start_handle = dfu_hdl_cache[HDL_DFU_CP] + 1;
        end_handle = dfu_hdl_cache[HDL_DFU_CP_END];
        descriptor_disc_flag = TRUE;
        break;
    case DFU_CCCD_DISC_END:
        cb_data.cb_content.disc_state = DFU_DISC_DONE;
        cb_flag = TRUE;
        break;
    default:
        /* No need to send char descriptor discovery. */
        break;
    }
    if (descriptor_disc_flag)
    {
        if (dfu_client_start_char_descriptor_discovery(conn_id, start_handle, end_handle) == FALSE)
        {
            cb_data.cb_content.disc_state = DFU_DISC_FAIL;
            cb_flag = TRUE;
        }
    }

    /* Send discover state to application if needed. */
    if (cb_flag && pf_dfu_client_app_cb)
    {
        (*pf_dfu_client_app_cb)(dfu_client_id, conn_id, &cb_data);
    }
}

/**
  * @brief  Called by profile client layer, when discover result fetched.
  * @param  resultType: indicate which type of value discovered in service discovery procedure.
  * @param  resultData: value discovered.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static void dfu_client_disc_result_cb(uint8_t conn_id, T_DISCOVERY_RESULT_TYPE result_type,
                                      T_DISCOVERY_RESULT_DATA result_data)
{
    printi("dfu_client_disc_result_cb: result_type = %d", result_type);
    switch (result_type)
    {
    case DISC_RESULT_SRV_DATA:
        /* send service handle range to application. */
        dfu_client_handle_set(HDL_DFU_SRV_START, result_data.p_srv_disc_data->att_handle);
        dfu_client_handle_set(HDL_DFU_SRV_END, result_data.p_srv_disc_data->end_group_handle);
        break;
    case DISC_RESULT_CHAR_UUID128:
        /* When use clientAPI_AllCharDiscovery. */
        if (dfu_end_hdl_idx < HDL_DFU_CACHE_LEN)
        {
            dfu_hdl_cache[dfu_end_hdl_idx] = result_data.p_char_uuid128_disc_data->decl_handle - 1;
            dfu_end_hdl_idx = HDL_DFU_CACHE_LEN;
        }
        /* we should inform intrested handles to upper application. */
        if (0 == memcmp(result_data.p_char_uuid128_disc_data->uuid128, dfu_data_uuid, 16))
        {
            dfu_client_handle_set(HDL_DFU_DATA, result_data.p_char_uuid128_disc_data->value_handle);
        }
        else if (0 == memcmp(result_data.p_char_uuid128_disc_data->uuid128, dfu_cp_uuid, 16))
        {
            dfu_client_handle_set(HDL_DFU_CP, result_data.p_char_uuid128_disc_data->value_handle);
            dfu_end_hdl_idx = HDL_DFU_CP_END;
        }
        break;
    case DISC_RESULT_CHAR_DESC_UUID16:
        /* When use clientAPI_AllCharDescriptorDiscovery. */
        if (result_data.p_char_desc_uuid16_disc_data->uuid16 == GATT_UUID_CHAR_CLIENT_CONFIG)
        {
            uint16_t temp_handle = result_data.p_char_desc_uuid16_disc_data->handle;
            if ((temp_handle > dfu_hdl_cache[HDL_DFU_CP])
                && (temp_handle <= dfu_hdl_cache[HDL_DFU_CP_END]))
            {
                dfu_client_handle_set(HDL_DFU_CP_CCCD, temp_handle);
            }
            else
            {
                /* have no intrest in this handle. */
            }
        }
        break;
    default:
        printw("dfu_client_disc_result_cb: Invalid Discovery Result Type!");
        break;
    }
}

/**
  * @brief  Called by profile client layer, when read request responsed.
  * @param  reqResult: read request from peer device success or not.
  * @param  wHandle: handle of the value in read response.
  * @param  iValueSize: size of the value in read response.
  * @param  pValue: pointer to the value in read response.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static void dfu_client_read_result_cb(uint8_t conn_id,  uint16_t cause, uint16_t handle,
                                      uint16_t len, uint8_t *pvalue)
{
    dfu_client_cb_data_t cb_data;
    cb_data.cb_type = DFU_CLIENT_CB_TYPE_READ_RESULT;
    cb_data.cb_content.read_result.cause = cause;
    printi("dfu_client_read_result_cb: cause = %d, handle = 0x%4.4x, size = %d", cause, handle, len);
    /* If read req success, branch to fetch value and send to application. */
    if (handle == dfu_hdl_cache[HDL_DFU_CP_CCCD])
    {
        cb_data.cb_content.read_result.type = DFU_READ_CP_CCCD;
        if (cause == GAP_SUCCESS && len == sizeof(uint16_t))
        {
            uint16_t cccd_value = LE_EXTRN2WORD(pvalue);
            cb_data.cb_content.read_result.data.dfu_cp_cccd = cccd_value &
                                                              GATT_CLIENT_CHAR_CONFIG_NOTIFY;
        }
    }

    /* Inform application the read result. */
    if (pf_dfu_client_app_cb)
    {
        (*pf_dfu_client_app_cb)(dfu_client_id, conn_id, &cb_data);
    }
}

/**
  * @brief  Called by profile client layer, when write request complete.
  * @param  reqResult: write request send success or not.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static void dfu_client_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle,
                                       uint16_t cause, uint8_t credits)
{
    dfu_client_cb_data_t cb_data;
    cb_data.cb_type = DFU_CLIENT_CB_TYPE_WRITE_RESULT;
    cb_data.cb_content.write_result.cause = cause;
    /* If write req success, branch to fetch value and send to application. */
    if (handle == dfu_hdl_cache[HDL_DFU_CP_CCCD])
    {
        cb_data.cb_content.write_result.type = DFU_WRITE_CP_CCCD;
    }
    else if (handle == dfu_hdl_cache[HDL_DFU_CP])
    {
        cb_data.cb_content.write_result.type = DFU_WRITE_CP;
    }
    else if (handle == dfu_hdl_cache[HDL_DFU_DATA])
    {
        cb_data.cb_content.write_result.type = DFU_WRITE_DATA;
    }
    else
    {
        return;
    }

    /* Inform application the write result. */
    if (pf_dfu_client_app_cb)
    {
        (*pf_dfu_client_app_cb)(dfu_client_id, conn_id, &cb_data);
    }
}

/**
  * @brief  Called by profile client layer, when notification or indication arrived.
  * @param  wHandle: handle of the value in received data.
  * @param  iValueSize: size of the value in received data.
  * @param  pValue: pointer to the value in received data.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static T_APP_RESULT dfu_client_notify_indicate_cb(uint8_t conn_id, bool notify,
                                                  uint16_t handle, uint16_t len, uint8_t *pvalue)
{
    T_APP_RESULT app_result = APP_RESULT_REJECT;
    dfu_client_cb_data_t cb_data;
    if (handle == dfu_hdl_cache[HDL_DFU_CP])
    {
        cb_data.cb_type = DFU_CLIENT_CB_TYPE_NOTIF_IND_RESULT;
        cb_data.cb_content.notif_ind_data.type = DFU_NOTIF_RECEIVE_DFU_CP_NOTIFY;
        cb_data.cb_content.notif_ind_data.value.len = len;
        cb_data.cb_content.notif_ind_data.value.pdata = pvalue;

        /* Inform application the notif/ind result. */
        if (pf_dfu_client_app_cb)
        {
            (*pf_dfu_client_app_cb)(dfu_client_id, conn_id, &cb_data);
        }
        app_result = APP_RESULT_SUCCESS;
    }

    return app_result;
}

/**
  * @brief  Called by profile client layer, when link disconnected.
  *         NOTE--we should reset some state when disconnected.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
void dfu_client_disconnect_cb(uint8_t conn_id)
{
    if (conn_id != dfu_client_ctx.conn_id)
    {
        return;
    }

    /* Reset some params, when disconnection occurs. */
    dfu_client_ctx.conn_id = DFU_CLIENT_INVALID_CONN_ID;
    dfu_end_hdl_idx = HDL_DFU_CACHE_LEN;
    dfu_cccd_disc_state = DFU_CCCD_DISC_START;
    dfu_fsm_auto_connect = DFU_FSM_AUTO_CONN_SCAN;
    dfu_client_init();
#if DFU_WO_SCAN
    if (dfu_ctx.bg_scan)
    {
        dfu_ctx.bg_scan = 0;
        gap_sched_scan(1);
    }
#endif
    if (dfu_ctx.fsm_client == DFU_CB_START)
    {
        dfu_ctx.fsm_client = DFU_CB_NONE;
        if (pf_dfu_client_app_info_cb)
        {
            dfu_cb_msg_t cb_msg;
            cb_msg.type = DFU_CB_FAIL;
            pf_dfu_client_app_info_cb(dfu_client_id, conn_id, &cb_msg);
        }
    }
}

bool aes256_ecb_encrypt_msb2lsb(uint8_t plaintext[16], const uint8_t key[32], uint8_t *encrypted);
bool dfu_encrypt(uint8_t image[16])
{
    bool ret;
    uint8_t image_tmp[16] = {0};
    plt_swap2(image_tmp, image, 16);
    ret = aes256_ecb_encrypt_msb2lsb(image_tmp, (uint8_t *)OTP->aes_key, image_tmp);
    plt_swap2(image, image_tmp, 16);
    return ret;
}

uint16_t dfu_client_crc_cal(uint8_t *buf, uint32_t length, uint16_t checksum16)
{
    uint32_t i;
    uint16_t *p16;
    p16 = (uint16_t *)buf;
    for (i = 0; i < length / 2; ++i)
    {
        checksum16 = checksum16 ^ (*p16);
        ++p16;
    }
    return checksum16;
}

bool dfu_client_push_image(void)
{
    if (dfu_fsm_write == DFU_FSM_WRITE_WAIT_WRITE_RESP)
    {
        // in case that the credit is bigger then the buffer check packet num
        printw("dfu_client_push_image: current waiting cp response");
        return false;
    }
    uint16_t mtu_size, msg_size;
#if 1
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, 0);
#else
    mtu_size = 23;
#endif
    mtu_size -= 3;
    mtu_size &= 0xfff0;
    uint32_t image_size = dfu_ctx.image_length - dfu_ctx.curr_offset;
    msg_size = image_size < mtu_size ? image_size : mtu_size;
    printi("dfu_client_push_image: image_length = %d curr_offset = %d msg_size = %d",
           dfu_ctx.image_length, dfu_ctx.curr_offset, msg_size);
    uint8_t *pheader = (uint8_t *)get_header_addr_by_img_id((T_IMG_ID)dfu_ctx.signature);
    uint8_t *pdata = plt_malloc(msg_size, RAM_TYPE_DATA_OFF);
    if (pdata == NULL)
    {
        return false;
    }
    memcpy(pdata, pheader + dfu_ctx.curr_offset, msg_size);
    dfu_ctx.curr_offset += msg_size;
    data_uart_debug("\b\b\b\b%3d%%", dfu_ctx.curr_offset * 100 / dfu_ctx.image_length);
    if (OTP->ota_with_encryption_data)
    {
        uint8_t *ptmp = pdata;
        for (uint16_t loop = 0; loop < msg_size / 16; loop++)
        {
            dfu_encrypt(ptmp);
            ptmp += 16;
        }
    }

    dfu_client_data_write(dfu_client_ctx.conn_id, pdata, msg_size);
#if BUFFER_CHECK_ENABLE
    if (!buffer_check_pkt_count)
    {
        buffer_check_crc = 0;
    }
    buffer_check_pkt_count++;
    buffer_check_crc = dfu_client_crc_cal(pdata, msg_size, buffer_check_crc);
    if (buffer_check_pkt_count == buffer_check_pkt_num || dfu_ctx.curr_offset == dfu_ctx.image_length)
    {
        uint8_t data[5];
        data[0] = DFU_OPCODE_REPORT_BUFFER_CRC;
        LE_WORD2EXTRN(data + 1, (buffer_check_pkt_count - 1) * mtu_size + msg_size);
        BE_WORD2EXTRN(data + 3, buffer_check_crc);
        dfu_client_cp_write(dfu_client_ctx.conn_id, data, 5);
        buffer_check_pkt_count = 0;
    }
#endif
    plt_free(pdata, RAM_TYPE_DATA_OFF);
    return true;
}

void dfu_client_handle_cp_msg(uint8_t conn_id, uint8_t *pdata, uint16_t len)
{
    uint8_t opcode = pdata[0];
    uint8_t req_opcode = pdata[1];
    printi("dfu_client_handle_cp_msg: notify opcode = %d req_opcode = %d len = %d.", opcode, req_opcode,
           len);
    if (opcode != DFU_OPCODE_NOTIFICATION)
    {
        return;
    }

    if (req_opcode == DFU_OPCODE_REPORT_TARGET_INFO && len == DFU_NOTIFY_LENGTH_REPORT_TARGET_INFO)
    {
        uint32_t target_version = LE_EXTRN2DWORD(pdata + 3);
        if (dfu_ctx.signature == AppPatch)
        {
            if (dfu_ctx.app_version <= target_version)
            {
                data_uart_debug("app no need ota!\r\n");
                dfu_ctx.signature = RomPatch;
                uint8_t data[DFU_LENGTH_REPORT_TARGET_INFO];
                data[0] = DFU_OPCODE_REPORT_TARGET_INFO;
                LE_WORD2EXTRN(data + 1, dfu_ctx.signature);
                dfu_client_cp_write(conn_id, data, DFU_LENGTH_REPORT_TARGET_INFO);
                return;
            }
        }
        else if (dfu_ctx.signature == RomPatch)
        {
            if (target_version >= dfu_ctx.patch_version)
            {
                data_uart_debug("patch no need ota!\r\n");
                return;
            }
        }
        else
        {
            return;
        }
#if DFU_WO_SCAN
        if (gap_scheduler.bg_scan)
        {
            dfu_ctx.bg_scan = 1;
            gap_sched_scan(0);
        }
#endif
        uint8_t *pheader = (uint8_t *)get_header_addr_by_img_id((T_IMG_ID)dfu_ctx.signature);
        uint8_t data[DFU_LENGTH_START_DFU];
        data[0] = DFU_OPCODE_START_DFU;
        memcpy(data + 1, pheader, DFU_HEADER_SIZE);
        memset(data + 1 + DFU_HEADER_SIZE, 0, 4);
        if (OTP->ota_with_encryption_data)
        {
            dfu_encrypt(data + 1);
        }
        dfu_client_cp_write(conn_id, data, DFU_LENGTH_START_DFU);
        dfu_ctx.curr_offset = DFU_HEADER_SIZE;
        dfu_ctx.image_length = ((T_IMG_CTRL_HEADER_FORMAT *)pheader)->payload_len + IMG_HEADER_SIZE;
        printi("dfu_client_handle_cp_msg: ota-ing %d(1:app, 0:patch), verision 0x%x->0x%x image_length = %d",
               dfu_ctx.signature == AppPatch,
               target_version, dfu_ctx.signature == AppPatch ? dfu_ctx.app_version : dfu_ctx.patch_version,
               dfu_ctx.image_length);
        data_uart_debug("%s being ota-ing %3d%%", dfu_ctx.signature == AppPatch ? "app" : "patch", 0);
        dfu_ctx.fsm_client = DFU_CB_START;
        if (pf_dfu_client_app_info_cb)
        {
            dfu_cb_msg_t cb_msg;
            cb_msg.type = DFU_CB_START;
            pf_dfu_client_app_info_cb(dfu_client_id, conn_id, &cb_msg);
        }
    }
    else if (req_opcode == DFU_OPCODE_START_DFU && len == DFU_NOTIFY_LENGTH_ARV)
    {
#if BUFFER_CHECK_ENABLE
        printi("dfu_client_handle_cp_msg: enable buffer check");
        uint8_t data[1];
        data[0] = DFU_OPCODE_BUFFER_CHECK_EN;
        dfu_client_cp_write(conn_id, data, 1);
    }
    else if (req_opcode == DFU_OPCODE_BUFFER_CHECK_EN && len == DFU_NOTIFY_LENGTH_ARV + 4)
    {
        uint16_t buffer_len = LE_EXTRN2WORD(pdata + 3);
        uint16_t mtu_size = LE_EXTRN2WORD(pdata + 5);
        buffer_check_pkt_num = buffer_len / ((mtu_size - 3) & 0xfff0);
        buffer_check_pkt_count = 0;
#endif
        //start push image
        for (uint8_t loop = 0; loop < 8; loop++)
        {
            dfu_client_push_image();
        }
    }
    else if (req_opcode == DFU_OPCODE_REPORT_BUFFER_CRC && len == DFU_NOTIFY_LENGTH_ARV + 4)
    {
        uint32_t offset = LE_EXTRN2DWORD(pdata + 3);
        printi("dfu_client_handle_cp_msg: report crc arv = %d offset %d->%d", pdata[2], dfu_ctx.curr_offset,
               offset);
        dfu_ctx.curr_offset = offset;
        if (dfu_ctx.image_length > dfu_ctx.curr_offset)
        {
            dfu_client_push_image();
        }
        else
        {
            uint8_t data[DFU_LENGTH_VALID_FW];
            data[0] = DFU_OPCODE_VALID_FW;
            LE_WORD2EXTRN(data + 1, dfu_ctx.signature);
            dfu_client_cp_write(conn_id, data, DFU_LENGTH_VALID_FW);
        }
    }
    else if (req_opcode == DFU_OPCODE_VALID_FW && len == DFU_NOTIFY_LENGTH_ARV)
    {
        printi("dfu_client_handle_cp_msg: Dfu verify image arv = %d (1 success)", pdata[2]);
        uint8_t data[DFU_LENGTH_ACTIVE_IMAGE_RESET];
        data[0] = pdata[2] == DFU_ARV_SUCCESS ? DFU_OPCODE_ACTIVE_IMAGE_RESET : DFU_OPCODE_SYSTEM_RESET;
        dfu_client_cp_write(conn_id, data, DFU_LENGTH_ACTIVE_IMAGE_RESET);
        dfu_ctx.fsm_client = DFU_CB_NONE;
        if (pf_dfu_client_app_info_cb)
        {
            dfu_cb_msg_t cb_msg;
            cb_msg.type = pdata[2] == DFU_ARV_SUCCESS ? DFU_CB_END : DFU_CB_FAIL;
            pf_dfu_client_app_info_cb(dfu_client_id, conn_id, &cb_msg);
        }
    }
}

T_APP_RESULT dfu_client_handle_msg(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
{
    dfu_client_cb_data_t *pcb_data = (dfu_client_cb_data_t *)p_data;
    printi("dfu_client_handle_msg: type = %d, dfu_fsm_write = %d.", pcb_data->cb_type, dfu_fsm_write);
    switch (pcb_data->cb_type)
    {
    case DFU_CLIENT_CB_TYPE_DISC_STATE:
        switch (pcb_data->cb_content.disc_state)
        {
        case DFU_DISC_DONE:
            /* Discovery Simple BLE service procedure successfully done. */
            data_uart_debug("Dfu service discovery end!\r\n");
            printi("dfu_client_handle_msg: discover procedure done.");
            dfu_client_cp_cccd_set(conn_id, 1);
            dfu_fsm_write = DFU_FSM_WRITE_CCCD_ENABLE;
            break;
        case DFU_DISC_FAIL:
        case DFU_DISC_NOT_FOUND:
            /* Discovery Request failed. */
            printw("dfu_client_handle_msg: discover request failed.");
            data_uart_debug("Dfu service discovery failed!\r\n");
            break;
        default:
            break;
        }
        break;
    case DFU_CLIENT_CB_TYPE_DISC_HDL:
        break;
    case DFU_CLIENT_CB_TYPE_READ_RESULT:
        switch (pcb_data->cb_content.read_result.type)
        {
        case DFU_READ_CP_CCCD:
            printi("dfu_client_handle_msg: dfu_cp_cccd = %d.",
                   pcb_data->cb_content.read_result.data.dfu_cp_cccd);
            break;
        default:
            break;
        }
        break;
    case DFU_CLIENT_CB_TYPE_WRITE_RESULT:
        if (pcb_data->cb_content.write_result.cause == 0)
        {
            switch (pcb_data->cb_content.write_result.type)
            {
            case DFU_WRITE_CP_CCCD:
                if (dfu_fsm_write == DFU_FSM_WRITE_CCCD_ENABLE)
                {
                    dfu_ctx.signature = AppPatch;
                    uint8_t data[DFU_LENGTH_REPORT_TARGET_INFO];
                    data[0] = DFU_OPCODE_REPORT_TARGET_INFO;
                    LE_WORD2EXTRN(data + 1, dfu_ctx.signature);
                    dfu_client_cp_write(conn_id, data, DFU_LENGTH_REPORT_TARGET_INFO);
                }
                break;
            case DFU_WRITE_DATA:
                if (dfu_fsm_write == DFU_FSM_WRITE_DFU_DATA)
                {
                    if (dfu_ctx.image_length > dfu_ctx.curr_offset)
                    {
                        dfu_client_push_image();
                    }
#if BUFFER_CHECK_ENABLE == 0
                    else
                    {
                        uint8_t data[DFU_LENGTH_VALID_FW];
                        data[0] = DFU_OPCODE_VALID_FW;
                        LE_WORD2EXTRN(data + 1, dfu_ctx.signature);
                        dfu_client_cp_write(data, DFU_LENGTH_VALID_FW);
                    }
#endif
                }
                break;
            case DFU_WRITE_CP:
                if (dfu_fsm_write == DFU_FSM_WRITE_WAIT_WRITE_RESP)
                {
                    dfu_fsm_write = DFU_FSM_WRITE_DO_NOT_CARE;
                    if (CpMsgLen)
                    {
                        dfu_client_handle_cp_msg(conn_id, CpMsg, CpMsgLen);
                        CpMsgLen = 0;
                    }
                }
                break;
            default:
                break;
            }
        }
        else
        {
            printw("dfu_client_handle_msg: fail to send msg type = %d, dfu_fsm_write = %d",
                   pcb_data->cb_content.write_result.type, dfu_fsm_write);
            le_disconnect(0);
        }
        break;
    case DFU_CLIENT_CB_TYPE_NOTIF_IND_RESULT:
        switch (pcb_data->cb_content.notif_ind_data.type)
        {
        case DFU_NOTIF_RECEIVE_DFU_CP_NOTIFY:
            {
                if (dfu_fsm_write == DFU_FSM_WRITE_DO_NOT_CARE)
                {
                    dfu_client_handle_cp_msg(conn_id, pcb_data->cb_content.notif_ind_data.value.pdata,
                                             pcb_data->cb_content.notif_ind_data.value.len);
                }
                else
                {
                    //cache
                    CpMsgLen = pcb_data->cb_content.notif_ind_data.value.len;
                    memcpy(CpMsg, pcb_data->cb_content.notif_ind_data.value.pdata,
                           CpMsgLen > 20 ? 20 : CpMsgLen);
                }
            }
            break;
        case DFU_NOTIF_IND_RECEIVE_FAIL:
            //DBG_BUFFER(MODULE_APP, LEVEL_WARN, "Proxy Client CB: notif/ind receive failed.",0);
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }
    return APP_RESULT_SUCCESS;
}

void dfu_client_handle_connecting(uint8_t conn_id)
{
    uint8_t bt_addr[6];
    T_GAP_REMOTE_ADDR_TYPE addr_type;
    if (dfu_fsm_auto_connect != DFU_FSM_AUTO_CONN_OTA)
    {
        return;
    }
    le_get_conn_addr(conn_id, bt_addr, &addr_type);
    if (0 != memcmp(bt_addr, dfu_client_ctx.bt_addr, 6))
    {
        return;
    }
    if (addr_type != dfu_client_ctx.addr_type)
    {
        return;
    }

    dfu_client_ctx.conn_id = conn_id;
}

void dfu_client_handle_connect(uint8_t conn_id)
{
    if (dfu_client_ctx.conn_id == conn_id)
    {
        dfu_client_start_discovery(conn_id);
    }
}

void dfu_client_handle_adv_pkt(T_LE_SCAN_INFO *p_le_scan_info)
{
    uint8_t *pbuffer = p_le_scan_info->data;
    uint8_t len = p_le_scan_info->data_len;
    uint8_t pos = 0;
    //printi("dfu_client_handle_adv_pkt: dfu_fsm_auto_connect = %d: ", dfu_fsm_auto_connect);
    //dprinti(pbuffer, len);
    if (dfu_fsm_auto_connect != DFU_FSM_AUTO_CONN_SCAN ||
        p_le_scan_info->adv_type != GAP_ADV_EVT_TYPE_UNDIRECTED)
    {
        return;
    }

    while (pos < len)
    {
        /* Length of the AD structure. */
        uint8_t length = pbuffer[pos++];
        uint8_t type;

        if (length < 0x02 || length > 0x1F || (pos + length) > 0x1F)
        {
            continue;
        }

        /* AD Type, one octet. */
        type = pbuffer[pos];
        switch (type)
        {
        case GAP_ADTYPE_MANUFACTURER_SPECIFIC:
            {
                if (length == 1 + MEMBER_OFFSET(manufacture_adv_data_t, data) + sizeof(manufacture_adv_data_dfu_t))
                {
                    manufacture_adv_data_t *pdata = (manufacture_adv_data_t *)(pbuffer + pos + 1);
                    if (pdata->company_id == MANUFACTURE_ADV_DATA_COMPANY_ID &&
                        pdata->type == MANUFACTURE_ADV_DATA_TYPE_DFU)
                    {
                        printi("dfu_client_handle_adv_pkt: product = %d-%d app = %d-%d patch = %d-%d",
                               pdata->dfu.product_id, dfu_ctx.product_id,
                               pdata->dfu.app_version, dfu_ctx.app_version,
                               pdata->dfu.patch_version, dfu_ctx.patch_version);
                        if (pdata->dfu.product_id == dfu_ctx.product_id && (dfu_ctx.app_version > pdata->dfu.app_version ||
                                                                            dfu_ctx.patch_version > pdata->dfu.patch_version))
                        {
                            T_GAP_LE_CONN_REQ_PARAM conn_req_param;
                            conn_req_param.scan_interval = 0x10;
                            conn_req_param.scan_window = 0x10;
                            conn_req_param.conn_interval_min = 0x10;
                            conn_req_param.conn_interval_max = 0x10;
                            conn_req_param.conn_latency = 0;
                            conn_req_param.supv_tout = 1000;
                            conn_req_param.ce_len_min = 2 * (conn_req_param.conn_interval_min - 1);
                            conn_req_param.ce_len_max = 2 * (conn_req_param.conn_interval_max - 1);
                            le_set_conn_param(GAP_CONN_PARAM_1M, &conn_req_param);
                            T_GAP_CAUSE cause;
                            if (GAP_CAUSE_SUCCESS == (cause = le_connect(GAP_PHYS_CONN_INIT_1M_BIT, p_le_scan_info->bd_addr,
                                                                         p_le_scan_info->remote_addr_type,
                                                                         GAP_LOCAL_ADDR_LE_PUBLIC, 1000)))
                            {
                                dfu_fsm_auto_connect = DFU_FSM_AUTO_CONN_OTA;
                                memcpy(dfu_client_ctx.bt_addr, p_le_scan_info->bd_addr, 6);
                                dfu_client_ctx.addr_type = p_le_scan_info->remote_addr_type;
                                data_uart_debug("Dfu client try connecting 0x%02x%02x%02x%02x%02x%02x\r\n>",
                                                p_le_scan_info->bd_addr[5], p_le_scan_info->bd_addr[4], p_le_scan_info->bd_addr[3],
                                                p_le_scan_info->bd_addr[2], p_le_scan_info->bd_addr[1], p_le_scan_info->bd_addr[0]);
                            }
                            else
                            {
                                printw("Dfu client: fail to try connect, cause = %d!", cause);
                            }
                        }
                    }
                }
                return;
            }
        //break;
        default:
            break;
        }

        pos += length;
    }

    return;
}

/**
 * @brief Dfu Client Callbacks.
*/
const T_FUN_CLIENT_CBS dfu_client_cbs =
{
    dfu_client_disc_state_cb,   //!< Discovery State callback function pointer
    dfu_client_disc_result_cb,  //!< Discovery result callback function pointer
    dfu_client_read_result_cb,      //!< Read response callback function pointer
    dfu_client_write_result_cb,     //!< Write result callback function pointer
    dfu_client_notify_indicate_cb,  //!< Notify Indicate callback function pointer
    NULL //dfu_client_disconnect_cb       //!< Link disconnection callback function pointer
};

static void dfu_client_init(void)
{
    dfu_ctx.product_id = DFU_PRODUCT_ID;
    T_IMG_HEADER_FORMAT *p_header = (T_IMG_HEADER_FORMAT *)get_header_addr_by_img_id(AppPatch);
    dfu_ctx.app_version = p_header->ctrl_header.payload_len ? DFU_APP_VERSION : 0x00000000;
    dfu_ctx.patch_version = DFU_PATCH_VERSION;
}

/**
  * @brief  add Dfu client to application.
  * @param  appCB: pointer of app callback function to handle specific client module data.
  * @retval Client ID of the specific client module.
  */
T_CLIENT_ID dfu_client_add(P_FUN_GENERAL_APP_CB app_cb)
{
    T_CLIENT_ID client_id;
    if (FALSE == client_register_spec_client_cb(&client_id, &dfu_client_cbs))
    {
        client_id = CLIENT_PROFILE_GENERAL_ID;
        printe("prov_client_add: fail!");
        return client_id;
    }
    dfu_client_id = client_id;
    printi("dfu_client_add: client ID = %d", dfu_client_id);

    /* register callback for profile to inform application that some events happened. */
    pf_dfu_client_app_cb = dfu_client_handle_msg;
    pf_dfu_client_app_info_cb = app_cb;
    dfu_client_init();
    dfu_client_ctx.conn_id = DFU_CLIENT_INVALID_CONN_ID;
    return dfu_client_id;
}


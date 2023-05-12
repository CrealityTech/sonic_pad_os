enum { __FILE_NUM__ = 0 };

/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    ota_client.c
  * @brief   ota service client source file.
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
#include "trace.h"
#include "endialig.h"
#include <string.h>
#include "FreeRTOS.h"
#include "gatt.h"
#include "ota_client.h"

/**
  * @{ Used for CCCD handle discovering in discovery procedure
  */
typedef enum
{
    OTA_CCCD_DISC_START,
    OTA_CCCD_DISC_END
} TOtaCCCD_DiscState, *POtaCCCD_DiscState;
/**
  * @}
  */

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
const uint8_t otaServiceUuid[16] = GATT_UUID_OTA_SERVICE;
/**<  Ota client ID. */
static TClientID otaClientID = AppProcessGeneralClientMsgID;
/**<  Ota discovery end handle control. */
static TOtaClientHandleType otaClientEndHdlIdx = HDL_OTA_CACHE_LEN;
/**<  Callback used to send data to app from Ota client layer. */
static pfnSpecificClientAppCB_t pfnOtaClientAppCB = NULL;
/**<  Handle Cache for simple BLE client to cache discovered handles. */
static uint16_t otaHdlCache[HDL_OTA_CACHE_LEN] = {0};
/**<  Discovery State indicate which CCCD is in discovery. */
static TOtaCCCD_DiscState otaClient_CccdDiscState = OTA_CCCD_DISC_START;

/**
  * @brief  Used by application, to set the handles in Ota handle cache.
  * @param  handle_type: handle types of this specific profile.
  * @param  handle_value: handle value to set.
  * @retval TRUE--set success.
  *         FALSE--set failed.
  */
bool OtaClient_SetHandle(TOtaClientHandleType handle_type, uint16_t handle_value)
{
    if (handle_type < HDL_OTA_CACHE_LEN)
    {
        otaHdlCache[handle_type] = handle_value;
        return TRUE;
    }
    return FALSE;
}

/**
  * @brief  Used by application, to get the handles in Ota handle cache.
  * @param  handle_type: handle types of this specific profile.
  * @retval handle value.
  */
uint16_t OtaClient_GetHandle(TOtaClientHandleType handle_type)
{
    return otaHdlCache[handle_type];
}

/**
  * @brief  Used by application, to start the discovery procedure of Ota server.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
bool OtaClient_StartDiscovery(void)
{
    DBG_BUFFER(MODULE_PROFILE, LEVEL_INFO, "OtaClient_StartDiscovery", 0);
    /* First clear handle cache. */
    memset(otaHdlCache, 0, sizeof(otaHdlCache));
    return (clientAPI_ByUuid128SrvDiscovery(otaClientID, otaServiceUuid) == ProfileResult_Success);
}

/**
  * @brief  Used internal, start the discovery of Ota characteristics.
  *         NOTE--user can offer this interface for application if required.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
static bool OtaClient_StartCharDiscovery(void)
{
    TCharDiscReq charReq;

    DBG_BUFFER(MODULE_PROFILE, LEVEL_INFO, "OtaClient_StartCharDiscovery", 0);
    charReq.wStartHandle = otaHdlCache[HDL_OTA_SRV_START];
    charReq.wEndHandle = otaHdlCache[HDL_OTA_SRV_END];
    return (clientAPI_AllCharDiscovery(otaClientID, charReq) == ProfileResult_Success);
}

/**
  * @brief  Used internal, start the discovery of Ota characteristics descriptor.
  *         NOTE--user can offer this interface for application if required.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
static bool OtaClient_StartCharDescriptorDiscovery(TCharDescriptorDiscReq charDescriptorReq)
{
    DBG_BUFFER(MODULE_PROFILE, LEVEL_INFO, "OtaClient_StartCharDescriptorDiscovery", 0);
    return (clientAPI_AllCharDescriptorDiscovery(otaClientID,
                                                 charDescriptorReq) == ProfileResult_Success);

}

/**
  * @brief  Used by application, read data from server by using handles.
  * @param  readCharType: one of characteristic that has the readable property.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
bool OtaClient_ReadByHandle(TOtaClientReadType readCharType)
{
    TReadHandleReq readHandle;
    bool hdl_valid = FALSE;

    DBG_BUFFER(MODULE_PROFILE, LEVEL_INFO, "OtaClient_ReadByHandle: charType = %d", 1, readCharType);

    switch (readCharType)
    {
    case OTA_READ_REQ_BD_ADDR:
        if (otaHdlCache[HDL_OTA_BD_ADDR])
        {
            readHandle.wHandle = otaHdlCache[HDL_OTA_BD_ADDR];
            hdl_valid = TRUE;
        }
        break;
    case OTA_READ_REQ_PATCH_VERSION:
        if (otaHdlCache[HDL_OTA_PATCH_VERSION])
        {
            readHandle.wHandle = otaHdlCache[HDL_OTA_PATCH_VERSION];
            hdl_valid = TRUE;
        }
        break;
    case OTA_READ_REQ_APP_VERSION:
        if (otaHdlCache[HDL_OTA_APP_VERSION])
        {
            readHandle.wHandle = otaHdlCache[HDL_OTA_APP_VERSION];
            hdl_valid = TRUE;
        }
        break;
    case OTA_READ_REQ_PATCH_EXT_VERSION:
        if (otaHdlCache[HDL_OTA_PATCH_EXT_VERSION])
        {
            readHandle.wHandle = otaHdlCache[HDL_OTA_PATCH_EXT_VERSION];
            hdl_valid = TRUE;
        }
        break;
    case OTA_READ_REQ_BANK_NUM:
        if (otaHdlCache[HDL_OTA_BANK_NUM])
        {
            readHandle.wHandle = otaHdlCache[HDL_OTA_BANK_NUM];
            hdl_valid = TRUE;
        }
        break;
    default:
        return FALSE;
    }

    if (hdl_valid)
    {
        if (clientAPI_AttribRead(otaClientID, readHandle) == ProfileResult_Success)
        {
            return TRUE;
        }
    }

    DBG_BUFFER(MODULE_PROFILE, LEVEL_WARN, "OtaClient_ReadByHandle: Request fail! Please check!", 0);
    return FALSE;
}

/**
  * @brief  Used by application, read data from server by using UUIDs.
  * @param  readCharType: one of characteristic that has the readable property.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
bool OtaClient_ReadByUUID(TOtaClientReadType readCharType)
{
    TReadUUIDReq readUUIDReq;

    DBG_BUFFER(MODULE_PROFILE, LEVEL_INFO, "OtaClient_ReadByUUID: charType = %d", 1, readCharType);

    switch (readCharType)
    {
    case OTA_READ_REQ_BD_ADDR:
        readUUIDReq.wStartHandle = otaHdlCache[HDL_OTA_SRV_START] + 1;
        readUUIDReq.wEndHandle = otaHdlCache[HDL_OTA_SRV_END];
        readUUIDReq.UUID16 = GATT_UUID_CHAR_MAC;
        break;
    default:
        return FALSE;
    }

    if (clientAPI_AttribReadUsingUUID(otaClientID, readUUIDReq) == ProfileResult_Success)
    {
        return TRUE;
    }
    return FALSE;
}

/**
  * @brief  Used internal, to send write request to peer server's V5 Control Point Characteristic.
  * @param  ctl_pnt_ptr: pointer of control point data to write.
  * @retval TRUE--send request to upper stack success.
  *         FALSE--send request to upper stack failed.
  */
bool OtaClient_OtaModeSet(uint8_t mode)
{
    TWriteReq writeReq;
    bool hdl_valid = FALSE;

    if (otaHdlCache[HDL_OTA_ENTER_OTA_MODE])
    {
        writeReq.wHandle = otaHdlCache[HDL_OTA_ENTER_OTA_MODE];
        writeReq.wType = blueAPI_GATTWriteTypeCommand;
        writeReq.wLength = sizeof(uint8_t);
        writeReq.pData = &mode;
        hdl_valid = TRUE;
    }

    if (hdl_valid)
    {
        if (clientAPI_AttribWrite(otaClientID, &writeReq) == ProfileResult_Success)
        {
            return TRUE;
        }
    }

    DBG_BUFFER(MODULE_PROFILE, LEVEL_WARN, "OtaClient_OtaDataWrite: Request fail! Please check!", 0);
    return FALSE;
}

bool OtaClient_ImageInfoSet(uint8_t image_count, uint16_t patch_version, uint16_t app_version)
{
    TWriteReq writeReq;
    bool hdl_valid = FALSE;
    uint8_t data[5];
    if (otaHdlCache[HDL_OTA_UPDATE_IMAGE_INFO])
    {
        data[0] = image_count;
        LE_WORD2EXTRN(data + 1, patch_version);
        LE_WORD2EXTRN(data + 3, app_version);
        writeReq.wHandle = otaHdlCache[HDL_OTA_UPDATE_IMAGE_INFO];
        writeReq.wType = blueAPI_GATTWriteTypeCommand;
        writeReq.wLength = sizeof(data);
        writeReq.pData = data;
        hdl_valid = TRUE;
    }

    if (hdl_valid)
    {
        if (clientAPI_AttribWrite(otaClientID, &writeReq) == ProfileResult_Success)
        {
            return TRUE;
        }
    }

    DBG_BUFFER(MODULE_PROFILE, LEVEL_WARN, "OtaClient_ImageInfoSet: Request fail! Please check!", 0);
    return FALSE;
}

/**
  * @brief  Used internal, switch to the next CCCD handle to be discovered.
  *         NOTE--this function only used when peer service has more than one CCCD.
  * @param  pSwitchState: CCCD discovery state.
  * @retval none.
  */
static void OtaClient_SwitchNextDiscDescriptor(TOtaCCCD_DiscState *pSwitchState)
{
    TOtaCCCD_DiscState new_state;

    switch (*pSwitchState)
    {
    default:
        new_state = OTA_CCCD_DISC_END;
        break;
    }

    *pSwitchState = new_state;
}

/**
  * @brief  Called by profile client layer, when discover state of discovery procedure changed.
  * @param  discoveryState: current service discovery state.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static TProfileResult OtaClientDiscoverStateCb(TDiscoveryState discoveryState)
{
    bool cb_flag = FALSE;
    bool descriptor_disc_flag = FALSE;
    TAppResult appResult = AppResult_Success;
    TCharDescriptorDiscReq charDescriptorReq;
    TOtaClientCB_Data cb_data;
    cb_data.cb_type = OTA_CLIENT_CB_TYPE_DISC_STATE;

    DBG_BUFFER(MODULE_PROFILE, LEVEL_INFO, "OtaClientDiscoverStateCb: discoveryState = %d", 1,
               discoveryState);

    switch (discoveryState)
    {
    case DISC_STATE_SRV_DONE:
        /* Indicate that service handle found. Start discover characteristic. */
        if ((otaHdlCache[HDL_OTA_SRV_START] != 0)
            || (otaHdlCache[HDL_OTA_SRV_END] != 0))
        {
            if (OtaClient_StartCharDiscovery() == FALSE)
            {
                cb_data.cb_content.disc_state = OTA_DISC_PENDING;
                cb_flag = TRUE;
            }
        }
        /* No Ota service handle found. Discover procedure complete. */
        else
        {
            cb_data.cb_content.disc_state = OTA_DISC_DONE;
            cb_flag = TRUE;
        }
        break;
    case DISC_STATE_CHAR_DONE:
        /* We should store the last char end handle if needed. */
        if (otaClientEndHdlIdx < HDL_OTA_CACHE_LEN)
        {
            otaHdlCache[otaClientEndHdlIdx] = otaHdlCache[HDL_OTA_SRV_END];
            otaClientEndHdlIdx = HDL_OTA_CACHE_LEN;
        }
        /* Find the first descriptor to be discovered. */
        otaClient_CccdDiscState = OTA_CCCD_DISC_START;
        OtaClient_SwitchNextDiscDescriptor(&otaClient_CccdDiscState);
        break;
    case DISC_STATE_CHAR_DESCRIPTOR_DONE:
        /* Find the next descriptor to be discovered. */
        OtaClient_SwitchNextDiscDescriptor(&otaClient_CccdDiscState);
        break;
    default:
        DBG_BUFFER(MODULE_PROFILE, LEVEL_WARN, "Invalid Discovery State!", 0);
        break;
    }

    /* Switch different char descriptor discovery, if has multi char descriptors. */
    switch (otaClient_CccdDiscState)
    {
    case OTA_CCCD_DISC_END:
        cb_data.cb_content.disc_state = OTA_DISC_DONE;
        cb_flag = TRUE;
        break;
    default:
        /* No need to send char descriptor discovery. */
        break;
    }
    if (descriptor_disc_flag)
    {
        if (OtaClient_StartCharDescriptorDiscovery(charDescriptorReq) == FALSE)
        {
            cb_data.cb_content.disc_state = OTA_DISC_PENDING;
            cb_flag = TRUE;
        }
    }

    /* Send discover state to application if needed. */
    if (cb_flag && pfnOtaClientAppCB)
    {
        appResult = (*pfnOtaClientAppCB)(otaClientID, &cb_data);
    }

    return (clientAPI_GetProfileResult(appResult));
}

/**
  * @brief  Called by profile client layer, when discover result fetched.
  * @param  resultType: indicate which type of value discovered in service discovery procedure.
  * @param  resultData: value discovered.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static TProfileResult OtaClientDiscoverResultCb(TDiscoverResultType resultType,
                                                TDiscoverResultData resultData)
{
    TAppResult appResult = AppResult_Success;
    bool char_cb_flag = FALSE;
    TOtaClientCB_Data cb_data;
    cb_data.cb_type = OTA_CLIENT_CB_TYPE_DISC_HDL;

    DBG_BUFFER(MODULE_PROFILE, LEVEL_INFO, "OtaClientDiscoverResultCb: resultType = %d", 1, resultType);

    switch (resultType)
    {
    case DISC_RESULT_SRV_DATA:
        /* send service handle range to application. */
        if (pfnOtaClientAppCB)
        {
            cb_data.cb_content.disc_hdl_info.handle_type = HDL_OTA_SRV_START;
            cb_data.cb_content.disc_hdl_info.handle_value = resultData.pSrvDiscData->attHandle;
            (*pfnOtaClientAppCB)(otaClientID, &cb_data);
            cb_data.cb_content.disc_hdl_info.handle_type = HDL_OTA_SRV_END;
            cb_data.cb_content.disc_hdl_info.handle_value = resultData.pSrvDiscData->endGroupHandle;
            appResult = (*pfnOtaClientAppCB)(otaClientID, &cb_data);
        }
        break;
    case DISC_RESULT_CHAR_UUID16:
        cb_data.cb_content.disc_hdl_info.handle_value = resultData.pCharUUID16DiscData->valueHandle;
        /* When use clientAPI_AllCharDiscovery. */
        if (otaClientEndHdlIdx < HDL_OTA_CACHE_LEN)
        {
            otaHdlCache[otaClientEndHdlIdx] = resultData.pCharUUID16DiscData->declHandle - 1;
            otaClientEndHdlIdx = HDL_OTA_CACHE_LEN;
        }
        /* we should inform intrested handles to upper application. */
        switch (resultData.pCharUUID16DiscData->UUID16)
        {
        case GATT_UUID_CHAR_OTA:
            cb_data.cb_content.disc_hdl_info.handle_type = HDL_OTA_ENTER_OTA_MODE;
            char_cb_flag = TRUE;
            break;
        case GATT_UUID_CHAR_MAC:
            cb_data.cb_content.disc_hdl_info.handle_type = HDL_OTA_BD_ADDR;
            char_cb_flag = TRUE;
            break;
        case GATT_UUID_CHAR_PATCH:
            cb_data.cb_content.disc_hdl_info.handle_type = HDL_OTA_PATCH_VERSION;
            char_cb_flag = TRUE;
            break;
        case GATT_UUID_CHAR_APP_VERSION:
            cb_data.cb_content.disc_hdl_info.handle_type = HDL_OTA_APP_VERSION;
            char_cb_flag = TRUE;
            break;
        case GATT_UUID_CHAR_PATCH_EXTENSION:
            cb_data.cb_content.disc_hdl_info.handle_type = HDL_OTA_PATCH_EXT_VERSION;
            char_cb_flag = TRUE;
            break;
        case GATT_UUID_CHAR_UPDATE_BANK_NUM:
            cb_data.cb_content.disc_hdl_info.handle_type = HDL_OTA_BANK_NUM;
            char_cb_flag = TRUE;
            break;
        case GATT_UUID_CHAR_IMAGE_COUNT_TO_UPDATE:
            cb_data.cb_content.disc_hdl_info.handle_type = HDL_OTA_UPDATE_IMAGE_INFO;
            char_cb_flag = TRUE;
            break;
        default:
            /* have no intrest on this handle. */
            break;
        }

        /* Inform application if needed. */
        if (char_cb_flag && pfnOtaClientAppCB)
        {
            appResult = (*pfnOtaClientAppCB)(otaClientID, &cb_data);
        }
        break;
    default:
        DBG_BUFFER(MODULE_PROFILE, LEVEL_WARN, "Invalid Discovery Result Type!", 0);
        break;
    }

    return (clientAPI_GetProfileResult(appResult));
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
static TProfileResult OtaClientReadResultCb(TClientRequestResult reqResult, uint16_t wHandle,
                                            int iValueSize, uint8_t *pValue)
{
    TAppResult appResult = AppResult_Success;
    TOtaClientCB_Data cb_data;
    cb_data.cb_type = OTA_CLIENT_CB_TYPE_READ_RESULT;

    DBG_BUFFER(MODULE_PROFILE, LEVEL_INFO,
               "OtaClientReadResultCb: result= %d, handle = 0x%4.4x, size = %d", 3, reqResult, wHandle,
               iValueSize);

    /* If read req success, branch to fetch value and send to application. */
    if (reqResult == CLIENT_REQ_SUCCESS)
    {
        if (wHandle == otaHdlCache[HDL_OTA_BD_ADDR])
        {
            if (iValueSize == 6)
            {
                uint16_t wCCCBits = *(uint16_t *)pValue;
                cb_data.cb_content.read_data.read_result = OTA_READ_RES_BD_ADDR;
                memcpy(cb_data.cb_content.read_data.read_content.bd_addr, pValue, 6);
            }
            else
            {
                cb_data.cb_content.read_data.read_result = OTA_READ_RES_FAIL;
            }
        }
        else if (wHandle == otaHdlCache[HDL_OTA_PATCH_VERSION])
        {
            if (iValueSize == sizeof(uint16_t))
            {
                uint16_t wCCCBits = *(uint16_t *)pValue;
                cb_data.cb_content.read_data.read_result = OTA_READ_RES_PATCH_VERSION;
                cb_data.cb_content.read_data.read_content.patch_version = LE_EXTRN2WORD(pValue);
            }
            else
            {
                cb_data.cb_content.read_data.read_result = OTA_READ_RES_FAIL;
            }
        }
        else if (wHandle == otaHdlCache[HDL_OTA_APP_VERSION])
        {
            if (iValueSize == sizeof(uint16_t))
            {
                uint16_t wCCCBits = *(uint16_t *)pValue;
                cb_data.cb_content.read_data.read_result = OTA_READ_RES_APP_VERSION;
                cb_data.cb_content.read_data.read_content.app_version = LE_EXTRN2WORD(pValue);
            }
            else
            {
                cb_data.cb_content.read_data.read_result = OTA_READ_RES_FAIL;
            }
        }
        else if (wHandle == otaHdlCache[HDL_OTA_PATCH_EXT_VERSION])
        {
            if (iValueSize == sizeof(uint16_t))
            {
                uint16_t wCCCBits = *(uint16_t *)pValue;
                cb_data.cb_content.read_data.read_result = OTA_READ_RES_PATCH_EXT_VERSION;
                cb_data.cb_content.read_data.read_content.patch_ext_version = LE_EXTRN2WORD(pValue);
            }
            else
            {
                cb_data.cb_content.read_data.read_result = OTA_READ_RES_FAIL;
            }
        }
        else if (wHandle == otaHdlCache[HDL_OTA_BANK_NUM])
        {
            if (iValueSize == sizeof(uint8_t))
            {
                uint16_t wCCCBits = *(uint16_t *)pValue;
                cb_data.cb_content.read_data.read_result = OTA_READ_RES_BANK_NUM;
                cb_data.cb_content.read_data.read_content.bank_num = *pValue;
            }
            else
            {
                cb_data.cb_content.read_data.read_result = OTA_READ_RES_FAIL;
            }
        }
        else
        {
            cb_data.cb_content.read_data.read_result = OTA_READ_RES_FAIL;
        }
    }
    /* Read req fail, inform application. */
    else
    {
        cb_data.cb_content.read_data.read_result = OTA_READ_RES_FAIL;
    }
    /* Inform application the read result. */
    if (pfnOtaClientAppCB)
    {
        appResult = (*pfnOtaClientAppCB)(otaClientID, &cb_data);
    }

    return (clientAPI_GetProfileResult(appResult));
}

/**
  * @brief  Called by profile client layer, when write request complete.
  * @param  reqResult: write request send success or not.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static TProfileResult OtaClientWriteResultCb(TClientRequestResult reqResult)
{
    TAppResult appResult = AppResult_Success;
    TOtaClientCB_Data cb_data;
    cb_data.cb_type = OTA_CLIENT_CB_TYPE_WRITE_RESULT;

    //DBG_BUFFER(MODULE_PROFILE, LEVEL_INFO, "OtaClientWriteResultCb: result = %d", 1, reqResult);

    /* If write req success, branch to fetch value and send to application. */
    if (reqResult == CLIENT_REQ_SUCCESS)
    {
        cb_data.cb_content.write_result = OTA_WRITE_RESULT_SUCCESS;
    }
    /* Read req fail, inform application. */
    else
    {
        cb_data.cb_content.write_result = OTA_WRITE_RESULT_FAIL;
    }
    /* Inform application the write result. */
    if (pfnOtaClientAppCB)
    {
        appResult = (*pfnOtaClientAppCB)(otaClientID, &cb_data);
    }

    return (clientAPI_GetProfileResult(appResult));
}

/**
  * @brief  Called by profile client layer, when notification or indication arrived.
  * @param  wHandle: handle of the value in received data.
  * @param  iValueSize: size of the value in received data.
  * @param  pValue: pointer to the value in received data.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static TProfileResult OtaClientNotifIndResultCb(uint16_t wHandle, int iValueSize, uint8_t *pValue)
{
    TAppResult appResult = AppResult_Success;

    //DBG_BUFFER(MODULE_PROFILE, LEVEL_INFO, "OtaClientNotifIndResultCb: handle = 0x%4.4x", 1, wHandle);

    /* Inform application the notif/ind result. */

    return (clientAPI_GetProfileResult(appResult));
}

/**
  * @brief  Called by profile client layer, when link disconnected.
  *         NOTE--we should reset some state when disconnected.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static TProfileResult OtaClientDisconnectCb(void)
{
    TProfileResult result = ProfileResult_Success;

    DBG_BUFFER(MODULE_PROFILE, LEVEL_INFO, "OtaClientDisconnectCb.", 0);

    /* Reset some params, when disconnection occurs. */
    otaClientEndHdlIdx = HDL_OTA_CACHE_LEN;
    otaClient_CccdDiscState = OTA_CCCD_DISC_START;

    /* There is no need to inform application, because GAP has done. */

    return (result);
}

/**
 * @brief Ota Client Callbacks.
*/
CONST TGATTClientCBs otaClientCbs =
{
    OtaClientDiscoverStateCb,   //!< Discovery State callback function pointer
    OtaClientDiscoverResultCb,  //!< Discovery result callback function pointer
    OtaClientReadResultCb,      //!< Read response callback function pointer
    OtaClientWriteResultCb,     //!< Write result callback function pointer
    OtaClientNotifIndResultCb,  //!< Notify Indicate callback function pointer
    OtaClientDisconnectCb       //!< Link disconnection callback function pointer
};

/**
  * @brief  add Ota client to application.
  * @param  appCB: pointer of app callback function to handle specific client module data.
  * @retval Client ID of the specific client module.
  */
TClientID Ota_AddClient(pfnSpecificClientAppCB_t appCB)
{
    TClientID ota_client_id;
    if (FALSE == clientAPI_RegisterSpecClientCB(&ota_client_id, otaHdlCache, &otaClientCbs))
    {
        ota_client_id = AppProcessGeneralClientMsgID;
        DBG_BUFFER(MODULE_PROFILE, LEVEL_ERROR, "Ota_AddClient Fail !!!", 0);
        return ota_client_id;
    }
    otaClientID = ota_client_id;
    DBG_BUFFER(MODULE_PROFILE, LEVEL_INFO, "Ota_AddClient: client ID = %d", 1, otaClientID);

    /* register callback for profile to inform application that some events happened. */
    pfnOtaClientAppCB = appCB;

    return ota_client_id;
}


/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     firmware_update_client.c
* @brief    Source file for firmware update client model.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2018-5-21
* @version  v1.0
* *************************************************************************************
*/

/* Add Includes here */
#include <string.h>
#include "mesh_api.h"
#include "firmware_update.h"

mesh_model_info_t fw_update_client;

static mesh_msg_send_cause_t fw_update_client_send(uint16_t dst, uint8_t *pmsg, uint16_t len)
{
    mesh_msg_t mesh_msg;
    mesh_msg.pmodel_info = &fw_update_client;
    access_cfg(&mesh_msg);
    mesh_msg.pbuffer = pmsg;
    mesh_msg.msg_len = len;
    mesh_msg.dst = dst;
    mesh_msg.app_key_index = 0; // TODO:
    mesh_msg.seg = 1;
    return access_send(&mesh_msg);
}

mesh_msg_send_cause_t fw_info_get(uint16_t dst)
{
    fw_info_get_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_FW_INFO_GET);
    return fw_update_client_send(dst, (uint8_t *)&msg, sizeof(fw_info_get_t));
}

mesh_msg_send_cause_t fw_update_get(uint16_t dst, uint16_t company_id,
                                    fw_update_fw_id_t firmware_id)
{
    fw_update_get_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_FW_UPDATE_GET);
    msg.company_id = company_id;
    FW_UPDATE_FW_ID(msg.firmware_id, firmware_id);
    return fw_update_client_send(dst, (uint8_t *)&msg, sizeof(fw_update_get_t));
}

mesh_msg_send_cause_t fw_update_prepare(uint16_t dst, uint16_t company_id,
                                        fw_update_fw_id_t firmware_id,
                                        uint8_t object_id[8], uint8_t vendor_validate_data[], uint16_t validate_len)
{
    mesh_msg_send_cause_t ret;
    fw_update_prepare_t *pmsg = (fw_update_prepare_t *)plt_malloc(MEMBER_OFFSET(fw_update_prepare_t,
                                                                                vendor_validate_data) + validate_len, RAM_TYPE_DATA_OFF);
    if (pmsg == NULL)
    {
        return MESH_MSG_SEND_CAUSE_NO_MEMORY;
    }
    ACCESS_OPCODE_BYTE(pmsg->opcode, MESH_MSG_FW_UPDATE_PREPARE);
    pmsg->company_id = company_id;
    FW_UPDATE_FW_ID(pmsg->firmware_id, firmware_id);
    memcpy(pmsg->object_id, object_id, sizeof(pmsg->object_id));
    memcpy(pmsg->vendor_validate_data, vendor_validate_data, validate_len);
    ret = fw_update_client_send(dst, (uint8_t *)pmsg, MEMBER_OFFSET(fw_update_prepare_t,
                                                                    vendor_validate_data) + validate_len);
    plt_free(pmsg, RAM_TYPE_DATA_OFF);
    return ret;
}

mesh_msg_send_cause_t fw_update_start(uint16_t dst, fw_update_policy_t policy, uint16_t company_id,
                                      fw_update_fw_id_t firmware_id)
{
    fw_update_start_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_FW_UPDATE_START);
    msg.policy = policy;
    msg.company_id = company_id;
    FW_UPDATE_FW_ID(msg.firmware_id, firmware_id);
    return fw_update_client_send(dst, (uint8_t *)&msg, sizeof(fw_update_start_t));
}

mesh_msg_send_cause_t fw_update_abort(uint16_t dst, uint16_t company_id,
                                      fw_update_fw_id_t firmware_id)
{
    fw_update_abort_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_FW_UPDATE_ABORT);
    msg.company_id = company_id;
    FW_UPDATE_FW_ID(msg.firmware_id, firmware_id);
    return fw_update_client_send(dst, (uint8_t *)&msg, sizeof(fw_update_abort_t));
}

mesh_msg_send_cause_t fw_update_apply(uint16_t dst, uint16_t company_id,
                                      fw_update_fw_id_t firmware_id)
{
    fw_update_apply_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_FW_UPDATE_APPLY);
    msg.company_id = company_id;
    FW_UPDATE_FW_ID(msg.firmware_id, firmware_id);
    return fw_update_client_send(dst, (uint8_t *)&msg, sizeof(fw_update_apply_t));
}

void fw_update_client_reg(model_receive_pf pf)
{
    fw_update_client.model_id = MESH_MODEL_FW_UPDATE_CLIENT;
    fw_update_client.model_receive = pf;
    mesh_model_reg(0, &fw_update_client);
}

/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     firmware_distribution_client.c
* @brief    Source file for firmware distribution client model.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2018-5-21
* @version  v1.0
* *************************************************************************************
*/

/* Add Includes here */
#include <string.h>
#include "mesh_api.h"
#include "firmware_distribution.h"

mesh_model_info_t fw_dist_client;

static mesh_msg_send_cause_t fw_dist_client_send(uint16_t dst, uint8_t *pmsg, uint16_t len)
{
    mesh_msg_t mesh_msg;
    mesh_msg.pmodel_info = &fw_dist_client;
    access_cfg(&mesh_msg);
    mesh_msg.pbuffer = pmsg;
    mesh_msg.msg_len = len;
    mesh_msg.dst = dst;
    mesh_msg.app_key_index = 0;
    return access_send(&mesh_msg);
}

mesh_msg_send_cause_t fw_dist_get(uint16_t dst, uint16_t company_id, uint16_t firmware_id)
{
    fw_dist_get_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_FW_DIST_GET);
    msg.company_id = company_id;
    msg.firmware_id = firmware_id;
    return fw_dist_client_send(dst, (uint8_t *)&msg, sizeof(fw_dist_get_t));
}

mesh_msg_send_cause_t fw_dist_start(uint16_t dst, uint16_t company_id, uint16_t firmware_id,
                                    uint16_t group_addr, uint16_t update_nodes_list[], uint16_t nodes_num)
{
    mesh_msg_send_cause_t ret;
    fw_dist_start_t *pmsg = (fw_dist_start_t *)plt_malloc(MEMBER_OFFSET(fw_dist_start_t,
                                                                        update_nodes_list) + nodes_num * 2, RAM_TYPE_DATA_OFF);
    if (pmsg == NULL)
    {
        return MESH_MSG_SEND_CAUSE_NO_MEMORY;
    }
    ACCESS_OPCODE_BYTE(pmsg->opcode, MESH_MSG_FW_INFO_STAT);
    pmsg->company_id = company_id;
    pmsg->firmware_id = firmware_id;
    pmsg->group_addr = group_addr;
    memcpy(pmsg->update_nodes_list, update_nodes_list, nodes_num * 2);
    ret = fw_dist_client_send(dst, (uint8_t *)pmsg, MEMBER_OFFSET(fw_dist_start_t,
                                                                  update_nodes_list) + nodes_num * 2);
    plt_free(pmsg, RAM_TYPE_DATA_OFF);
    return ret;
}

mesh_msg_send_cause_t fw_dist_stop(uint16_t dst, uint16_t company_id, uint16_t firmware_id)
{
    fw_dist_stop_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_FW_DIST_STOP);
    msg.company_id = company_id;
    msg.firmware_id = firmware_id;
    return fw_dist_client_send(dst, (uint8_t *)&msg, sizeof(fw_dist_stop_t));
}

mesh_msg_send_cause_t fw_dist_details_get(uint16_t dst, fw_dist_stat_stat_t stat,
                                          uint16_t company_id, uint16_t firmware_id)
{
    fw_dist_details_get_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_FW_DIST_DETAILS_GET);
    msg.stat = stat;
    msg.company_id = company_id;
    msg.firmware_id = firmware_id;
    return fw_dist_client_send(dst, (uint8_t *)&msg, sizeof(fw_dist_details_get_t));
}

bool fw_dist_client_receive(mesh_msg_p pmesh_msg)
{
    bool ret = TRUE;
    uint8_t *pbuffer = pmesh_msg->pbuffer + pmesh_msg->msg_offset;
    switch (pmesh_msg->access_opcode)
    {
    case MESH_MSG_FW_DIST_STAT:
        if (pmesh_msg->msg_len == sizeof(fw_dist_stat_t))
        {

        }
        break;
    case MESH_MSG_FW_DIST_DETAILS_LIST:
        break;
    default:
        ret = FALSE;
        break;
    }
    return ret;
}

void fw_dist_client_reg(void)
{
    fw_dist_client.model_id = MESH_MODEL_FW_DIST_CLIENT;
    fw_dist_client.model_receive = fw_dist_client_receive;
    mesh_model_reg(0, &fw_dist_client);
}

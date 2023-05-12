/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     object_transfer.c
* @brief    Source file for object transfer client model.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2018-5-21
* @version  v1.0
* *************************************************************************************
*/

/* Add Includes here */
#include <string.h>
#include "mesh_api.h"
#include "object_transfer.h"

mesh_model_info_t obj_transfer_client;

static mesh_msg_send_cause_t obj_transfer_client_send(uint16_t dst, uint8_t *pmsg, uint16_t len)
{
    mesh_msg_t mesh_msg;
    mesh_msg.pmodel_info = &obj_transfer_client;
    access_cfg(&mesh_msg);
    mesh_msg.pbuffer = pmsg;
    mesh_msg.msg_len = len;
    mesh_msg.dst = dst;
    mesh_msg.app_key_index = 0; // TODO:
    mesh_msg.seg = 1;
    return access_send(&mesh_msg);
}

mesh_msg_send_cause_t obj_transfer_phase_get(uint16_t dst)
{
    obj_transfer_phase_get_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_OBJ_TRANSFER_PHASE_GET);
    return obj_transfer_client_send(dst, (uint8_t *)&msg, sizeof(obj_transfer_phase_get_t));
}

mesh_msg_send_cause_t obj_transfer_get(uint16_t dst, uint8_t object_id[8])
{
    obj_transfer_get_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_OBJ_TRANSFER_GET);
    memcpy(msg.object_id, object_id, sizeof(msg.object_id));
    return obj_transfer_client_send(dst, (uint8_t *)&msg, sizeof(obj_transfer_get_t));
}

mesh_msg_send_cause_t obj_transfer_start(uint16_t dst, uint8_t object_id[8], uint32_t object_size,
                                         uint8_t curr_block_size_log)
{
    obj_transfer_start_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_OBJ_TRANSFER_START);
    memcpy(msg.object_id, object_id, 8);
    msg.object_size = object_size;
    msg.curr_block_size_log = curr_block_size_log;
    return obj_transfer_client_send(dst, (uint8_t *)&msg, sizeof(obj_transfer_start_t));
}

mesh_msg_send_cause_t obj_transfer_abort(uint16_t dst, uint8_t object_id[8])
{
    obj_transfer_abort_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_OBJ_TRANSFER_ABORT);
    memcpy(msg.object_id, object_id, sizeof(msg.object_id));
    return obj_transfer_client_send(dst, (uint8_t *)&msg, sizeof(obj_transfer_abort_t));
}

mesh_msg_send_cause_t obj_block_transfer_start(uint16_t dst, uint8_t object_id[8],
                                               uint16_t block_num,
                                               uint16_t chunk_size, obj_block_check_algo_t check_algo, uint8_t checksum_value[],
                                               uint16_t checksum_value_len)
{
    obj_block_transfer_start_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_OBJ_BLOCK_TRANSFER_START);
    memcpy(msg.object_id, object_id, sizeof(msg.object_id));
    msg.block_num = block_num;
    msg.chunk_size = chunk_size;
    msg.check_algo = check_algo;
    memcpy(msg.checksum_value, checksum_value, checksum_value_len);
    return obj_transfer_client_send(dst, (uint8_t *)&msg, MEMBER_OFFSET(obj_block_transfer_start_t,
                                                                        checksum_value) + checksum_value_len);
}

mesh_msg_send_cause_t obj_chunk_transfer(uint16_t dst, uint16_t chunk_num, uint8_t data[],
                                         uint16_t len)
{
    mesh_msg_send_cause_t ret;
    obj_chunk_transfer_t *pmsg = (obj_chunk_transfer_t *)plt_malloc(MEMBER_OFFSET(obj_chunk_transfer_t,
                                                                                  data) + len, RAM_TYPE_DATA_OFF);
    if (pmsg == NULL)
    {
        return MESH_MSG_SEND_CAUSE_NO_MEMORY;
    }
    ACCESS_OPCODE_BYTE(pmsg->opcode, MESH_MSG_OBJ_CHUNCK_TRANSFER);
    pmsg->chunk_num = chunk_num;
    memcpy(pmsg->data, data, len);
    ret = obj_transfer_client_send(dst, (uint8_t *)pmsg, MEMBER_OFFSET(obj_chunk_transfer_t,
                                                                       data) + len);
    plt_free(pmsg, RAM_TYPE_DATA_OFF);
    return ret;
}

mesh_msg_send_cause_t obj_block_get(uint16_t dst, uint8_t object_id[8], uint16_t block_num)
{
    obj_block_get_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_OBJ_BLOCK_GET);
    memcpy(msg.object_id, object_id, sizeof(msg.object_id));
    msg.block_num = block_num;
    return obj_transfer_client_send(dst, (uint8_t *)&msg, sizeof(obj_block_get_t));
}

mesh_msg_send_cause_t obj_info_get(uint16_t dst)
{
    obj_info_get_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_OBJ_INFO_GET);
    return obj_transfer_client_send(dst, (uint8_t *)&msg, sizeof(obj_info_get_t));
}

void obj_transfer_client_reg(model_receive_pf model_receive, model_send_cb_pf model_send_cb)
{
    obj_transfer_client.model_id = MESH_MODEL_OBJ_TRANSFER_CLIENT;
    obj_transfer_client.model_receive = model_receive;
    obj_transfer_client.model_send_cb = model_send_cb;
    mesh_model_reg(0, &obj_transfer_client);
}

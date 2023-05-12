/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     object_transfer.c
* @brief    Source file for object transfer server model.
* @details  Data types and external functions declaration.
* @author   bill
* @date     2018-5-21
* @version  v1.0
* *************************************************************************************
*/

#define MM_ID MM_MODEL

/* Add Includes here */
#include <string.h>
#include "mesh_api.h"
#include "object_transfer.h"

static const uint8_t checksum_len[OBJ_BLOCK_CHECK_ALGO_NUM] = {4};

mesh_model_info_t obj_transfer_server;
obj_transfer_server_ctx_t obj_transfer_server_ctx;

static mesh_msg_send_cause_t obj_transfer_server_send(mesh_msg_p pmesh_msg, uint8_t *pmsg,
                                                      uint16_t len)
{
    mesh_msg_t mesh_msg;
    mesh_msg.pmodel_info = pmesh_msg->pmodel_info;
    access_cfg(&mesh_msg);
    mesh_msg.pbuffer = pmsg;
    mesh_msg.msg_len = len;
    mesh_msg.dst = pmesh_msg->src;
    mesh_msg.app_key_index = pmesh_msg->app_key_index;
    return access_send(&mesh_msg);
}

mesh_msg_send_cause_t obj_transfer_phase_stat(mesh_msg_p pmesh_msg, obj_transfer_phase_t phase,
                                              uint8_t object_id[8])
{
    obj_transfer_phase_stat_t msg;
    msg.phase = phase;
    if (object_id)
    {
        memcpy(msg.object_id, object_id, sizeof(msg.object_id));
    }
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_OBJ_TRANSFER_PHASE_STAT);
    return obj_transfer_server_send(pmesh_msg, (uint8_t *)&msg,
                                    sizeof(obj_transfer_phase_stat_t) - (object_id != NULL ? 0 : 8));
}

mesh_msg_send_cause_t obj_transfer_stat(mesh_msg_p pmesh_msg, obj_transfer_stat_stat_t stat,
                                        uint8_t object_id[8], uint32_t object_size, uint8_t block_size_log)
{
    obj_transfer_stat_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_OBJ_TRANSFER_STAT);
    msg.stat = stat;
    memcpy(msg.object_id, object_id, sizeof(msg.object_id));
    msg.object_size = object_size;
    msg.block_size_log = block_size_log;
    return obj_transfer_server_send(pmesh_msg, (uint8_t *)&msg, sizeof(obj_transfer_stat_t));
}

mesh_msg_send_cause_t obj_block_transfer_stat(mesh_msg_p pmesh_msg,
                                              obj_block_transfer_stat_stat_t stat)
{
    obj_block_transfer_stat_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_OBJ_BLOCK_TRANSFER_STAT);
    msg.stat = stat;
    return obj_transfer_server_send(pmesh_msg, (uint8_t *)&msg, sizeof(obj_block_transfer_stat_t));
}

mesh_msg_send_cause_t obj_block_stat(mesh_msg_p pmesh_msg, obj_block_transfer_stat_stat_t stat,
                                     uint16_t chunk_list[], uint16_t chunck_count)
{
    mesh_msg_send_cause_t ret;
    obj_block_stat_t *pmsg = (obj_block_stat_t *)plt_malloc(MEMBER_OFFSET(obj_block_stat_t,
                                                                          chunk_list) + chunck_count * 2, RAM_TYPE_DATA_OFF);
    if (pmsg == NULL)
    {
        return MESH_MSG_SEND_CAUSE_NO_MEMORY;
    }
    ACCESS_OPCODE_BYTE(pmsg->opcode, MESH_MSG_OBJ_BLOCK_STAT);
    pmsg->stat = stat;
    if (chunck_count && chunk_list)
    {
        memcpy(pmsg->chunk_list, chunk_list, chunck_count * 2);
    }
    ret = obj_transfer_server_send(pmesh_msg, (uint8_t *)pmsg, MEMBER_OFFSET(obj_block_stat_t,
                                                                             chunk_list) + chunck_count * 2);
    plt_free(pmsg, RAM_TYPE_DATA_OFF);
    return ret;
}

mesh_msg_send_cause_t obj_info_stat(mesh_msg_p pmesh_msg, uint8_t min_block_size_log,
                                    uint8_t max_block_size_log, uint16_t max_chunk_num)
{
    obj_info_stat_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_OBJ_INFO_STAT);
    msg.min_block_size_log = min_block_size_log;
    msg.max_block_size_log = max_block_size_log;
    msg.max_chunk_num = max_chunk_num;
    return obj_transfer_server_send(pmesh_msg, (uint8_t *)&msg, sizeof(obj_info_stat_t));
}

bool obj_transfer_server_verify_checksum(void)
{
    bool ret;
    switch (obj_transfer_server_ctx.check_algo)
    {
    case OBJ_BLOCK_CHECK_ALGO_CRC32:
        ret = true;
        break;
    default:
        break;
    }
    return ret;
}

void obj_transfer_server_handle_obj_block_transfer_start(mesh_msg_p pmesh_msg)
{
    obj_block_transfer_stat_stat_t stat;
    uint8_t *pbuffer = pmesh_msg->pbuffer + pmesh_msg->msg_offset;
    obj_block_transfer_start_t *pmsg = (obj_block_transfer_start_t *)pbuffer;

    if (obj_transfer_server_ctx.phase == OBJ_TRANSFER_PHASE_IDLE)
    {
        stat = OBJ_BLOCK_TRANSFER_STAT_INVALID_STATE;
        goto end;
    }

    if (0 != memcmp(pmsg->object_id, obj_transfer_server_ctx.object_id,
                    sizeof(obj_transfer_server_ctx.object_id)))
    {
        stat = OBJ_BLOCK_TRANSFER_STAT_WRONG_OBJECT_ID;
        goto end;
    }

    if (obj_transfer_server_ctx.current_block_num < pmsg->block_num)
    {
#if OBJ_TRANSFER_D05R05
        stat = OBJ_BLOCK_TRANSFER_STAT_WRONG_CHECKSUM;
#else
        stat = OBJ_BLOCK_TRANSFER_STAT_INVALID_BLOCK_NUM;
#endif
        goto end;
    }
    else if (obj_transfer_server_ctx.current_block_num > pmsg->block_num)
    {
        stat = OBJ_BLOCK_TRANSFER_STAT_DUPLICATE_BLOCK;
        goto end;
    }

    if (!pmsg->chunk_size)
    {
        stat = OBJ_BLOCK_TRANSFER_STAT_INVALID_PARAMETER;
        goto end;
    }

    if (obj_transfer_server_ctx.current_block_num == obj_transfer_server_ctx.block_num - 1)
    {
        obj_transfer_server_ctx.current_block_size = obj_transfer_server_ctx.object_size %
                                                     obj_transfer_server_ctx.block_size;
        if (!obj_transfer_server_ctx.current_block_size)
        {
            obj_transfer_server_ctx.current_block_size = obj_transfer_server_ctx.block_size;
        }
    }
    else
    {
        obj_transfer_server_ctx.current_block_size = obj_transfer_server_ctx.block_size;
    }

    uint16_t chunk_num;
    chunk_num = (obj_transfer_server_ctx.current_block_size + pmsg->chunk_size - 1) / pmsg->chunk_size;
    if (chunk_num > OBJ_TRANSFER_MAX_CHUNK_NUM)
    {
        stat = OBJ_BLOCK_TRANSFER_STAT_WRONG_CHUNK_SIZE;
        goto end;
    }

    obj_transfer_server_ctx.checksum_len = pmesh_msg->msg_len - MEMBER_OFFSET(
                                               obj_block_transfer_start_t, checksum_value);
    if (pmsg->check_algo > OBJ_BLOCK_CHECK_ALGO_CRC32)
    {
        stat = OBJ_BLOCK_TRANSFER_STAT_UNKNOWN_CHECK_ALGO;
        goto end;
    }
    else
    {
        if (obj_transfer_server_ctx.checksum_len != checksum_len[pmsg->check_algo])
        {
            /* not check now */
            //stat = OBJ_BLOCK_TRANSFER_STAT_UNKNOWN_CHECK_ALGO;
            //goto end;
        }
    }

    stat = OBJ_BLOCK_TRANSFER_STAT_ACCEPTED;
    if (obj_transfer_server_ctx.phase == OBJ_TRANSFER_PHASE_WAITING_BLOCK)
    {
        obj_transfer_server_ctx.phase = OBJ_TRANSFER_PHASE_WAITING_CHUNK;
        //obj_transfer_server_ctx.current_block_num = pmsg->block_num;
        obj_transfer_server_ctx.chunk_size = pmsg->chunk_size;
        obj_transfer_server_ctx.check_algo = pmsg->check_algo;
        memset(obj_transfer_server_ctx.chunk_flag, 0, sizeof(obj_transfer_server_ctx.chunk_flag));
        memcpy(obj_transfer_server_ctx.checksum_value, pmsg->checksum_value,
               obj_transfer_server_ctx.checksum_len);
        printi("obj_transfer_server_handle_obj_block_transfer_start: block num %d, chunk size %d, check algo %d len %d, value ",
               pmsg->block_num, pmsg->chunk_size, pmsg->check_algo, obj_transfer_server_ctx.checksum_len);
        dprinti(pmsg->checksum_value, obj_transfer_server_ctx.checksum_len);
    }
end:
    if (stat != OBJ_BLOCK_TRANSFER_STAT_ACCEPTED)
    {
        printw("obj_transfer_server_handle_obj_block_transfer_start: stat = %d", stat);
    }
    obj_block_transfer_stat(pmesh_msg, stat);
}

void obj_transfer_server_handle_obj_chunk_transfer(mesh_msg_p pmesh_msg)
{
    uint32_t ret = 0;
    uint8_t *pbuffer = pmesh_msg->pbuffer + pmesh_msg->msg_offset;
    obj_chunk_transfer_t *pmsg = (obj_chunk_transfer_t *)pbuffer;
    if (obj_transfer_server_ctx.phase != OBJ_TRANSFER_PHASE_WAITING_CHUNK)
    {
        ret = __LINE__;
        goto end;
    }

    uint16_t chunk_num, chunk_size;
    chunk_num = (obj_transfer_server_ctx.current_block_size + obj_transfer_server_ctx.chunk_size - 1) /
                obj_transfer_server_ctx.chunk_size;
    chunk_size = pmesh_msg->msg_len - MEMBER_OFFSET(obj_chunk_transfer_t, data);
    printi("obj_transfer_server_handle_obj_chunk_transfer: block num %d/%d size %d, chunk num %d/%d size %d",
           obj_transfer_server_ctx.current_block_num, obj_transfer_server_ctx.block_num,
           obj_transfer_server_ctx.current_block_size,
           pmsg->chunk_num, chunk_num, chunk_size);

    if (pmsg->chunk_num >= chunk_num)
    {
        ret = __LINE__;
        goto end;
    }
    else
    {
        if (pmsg->chunk_num < chunk_num - 1)
        {
            if (chunk_size != obj_transfer_server_ctx.chunk_size)
            {
                ret = __LINE__;
                goto end;
            }
        }
        else
        {
            uint16_t last_chunk_size = obj_transfer_server_ctx.current_block_size %
                                       obj_transfer_server_ctx.chunk_size;
            if (!last_chunk_size)
            {
                last_chunk_size = obj_transfer_server_ctx.chunk_size;
            }
            if (chunk_size != last_chunk_size)
            {
                ret = __LINE__;
                goto end;
            }
        }
    }

    if (plt_bit_pool_get(obj_transfer_server_ctx.chunk_flag, pmsg->chunk_num))
    {
        // duplicate
        ret = __LINE__;
        goto end;
    }

    plt_bit_pool_set(obj_transfer_server_ctx.chunk_flag, pmsg->chunk_num, TRUE);
    memcpy(obj_transfer_server_ctx.block_data + pmsg->chunk_num * obj_transfer_server_ctx.chunk_size,
           pmsg->data, chunk_size);

    // check block complete
    uint16_t loop;
    for (loop = 0; loop < chunk_num; loop++)
    {
        if (!plt_bit_pool_get(obj_transfer_server_ctx.chunk_flag, loop))
        {
            break;
        }
    }
    if (loop < chunk_num)
    {
        goto end;
    }

    if (false == obj_transfer_server_verify_checksum())
    {
        /* What to do? */
        memset(obj_transfer_server_ctx.chunk_flag, 0, sizeof(obj_transfer_server_ctx.chunk_flag));
        ret = __LINE__;
        goto end;
    }
    else
    {
        if (obj_transfer_server.model_data_cb)
        {
            obj_transfer_server.model_data_cb(&obj_transfer_server, OBJ_TRANSFER_SERVER_CB_TYPE_DATA,
                                              &obj_transfer_server_ctx);
        }
    }

    obj_transfer_server_ctx.phase = OBJ_TRANSFER_PHASE_WAITING_BLOCK;
    obj_transfer_server_ctx.current_block_num++;

    // check object complete
    if (obj_transfer_server_ctx.current_block_num != obj_transfer_server_ctx.block_num)
    {
        goto end;
    }

    obj_transfer_server_ctx.phase = OBJ_TRANSFER_PHASE_IDLE;
    printi("obj_transfer_server_handle_obj_chunk_transfer: done!");
    plt_free(obj_transfer_server_ctx.block_data, RAM_TYPE_DATA_ON);
    obj_transfer_server_ctx.block_data = NULL;

end:
    if (ret != 0)
    {
        printi("obj_transfer_server_handle_obj_chunk_transfer: ret = %d", ret);
    }
}

void obj_transfer_server_handle_obj_block_get(mesh_msg_p pmesh_msg)
{
    uint8_t *pbuffer = pmesh_msg->pbuffer + pmesh_msg->msg_offset;
    obj_block_get_t *pmsg = (obj_block_get_t *)pbuffer;
    // delete this check?
    /*
    if (obj_transfer_server_ctx.phase == OBJ_TRANSFER_PHASE_IDLE)
    {
        obj_block_stat(pmesh_msg, OBJ_BLOCK_TRANSFER_STAT_INVALID_STATE, NULL, 0);
        return;
    }
    */

    if (0 != memcmp(pmsg->object_id, obj_transfer_server_ctx.object_id,
                    sizeof(obj_transfer_server_ctx.object_id)))
    {
        obj_block_stat(pmesh_msg, OBJ_BLOCK_TRANSFER_STAT_WRONG_OBJECT_ID, NULL, 0);
        return;
    }

    if (pmsg->block_num < obj_transfer_server_ctx.current_block_num)
    {
        obj_block_stat(pmesh_msg, OBJ_BLOCK_TRANSFER_STAT_ACCEPTED, NULL, 0);
    }
    else
    {
        // TODO:
        uint16_t *chunk_list = plt_malloc(OBJ_TRANSFER_MAX_CHUNK_NUM << 1, RAM_TYPE_DATA_OFF);
        uint16_t chunk_list_len = 0;
        uint16_t chunk_num = 0;
        if (chunk_list)
        {
            chunk_num = (obj_transfer_server_ctx.current_block_size +
                         obj_transfer_server_ctx.chunk_size - 1) /
                        obj_transfer_server_ctx.chunk_size;
            for (uint16_t loop = 0; loop < chunk_num; loop++)
            {
                if (!plt_bit_pool_get(obj_transfer_server_ctx.chunk_flag, loop))
                {
                    chunk_list[chunk_list_len] = loop;
                    chunk_list_len++;
                }
            }
        }
        obj_block_stat(pmesh_msg, chunk_list_len ? (chunk_list_len >= chunk_num ?
                                                    OBJ_BLOCK_TRANSFER_STAT_INVALID_BLOCK_NUM : OBJ_BLOCK_TRANSFER_STAT_NOT_ALL_CHUNCK_RECEIVED) :
                       OBJ_BLOCK_TRANSFER_STAT_ACCEPTED, chunk_list, chunk_list_len);
        plt_free(chunk_list, RAM_TYPE_DATA_OFF);
    }
}

bool obj_transfer_server_receive(mesh_msg_p pmesh_msg)
{
    bool ret = TRUE;
    uint8_t *pbuffer = pmesh_msg->pbuffer + pmesh_msg->msg_offset;
    switch (pmesh_msg->access_opcode)
    {
    case MESH_MSG_OBJ_TRANSFER_PHASE_GET:
        if (pmesh_msg->msg_len == sizeof(obj_transfer_phase_get_t))
        {
            obj_transfer_phase_stat(pmesh_msg, obj_transfer_server_ctx.phase,
                                    obj_transfer_server_ctx.phase != OBJ_TRANSFER_PHASE_IDLE ? obj_transfer_server_ctx.object_id :
                                    NULL);
        }
        break;
    case MESH_MSG_OBJ_TRANSFER_GET:
        if (pmesh_msg->msg_len == sizeof(obj_transfer_get_t))
        {
            obj_transfer_stat_stat_t stat;
            obj_transfer_get_t *pmsg = (obj_transfer_get_t *)pbuffer;
            if (obj_transfer_server_ctx.phase == OBJ_TRANSFER_PHASE_IDLE)
            {
                stat = OBJ_TRANSFER_STAT_READY;
            }
            else
            {
                if (0 == memcmp(pmsg->object_id, obj_transfer_server_ctx.object_id,
                                sizeof(obj_transfer_server_ctx.object_id)))
                {
                    stat = OBJ_TRANSFER_STAT_BUSY;
                }
                else
                {
                    stat = OBJ_TRANSFER_STAT_INVALID_PARAMETER;
                }
            }
            obj_transfer_stat(pmesh_msg, stat, obj_transfer_server_ctx.object_id,
                              obj_transfer_server_ctx.object_size, plt_log2(obj_transfer_server_ctx.block_size));
        }
        break;
    case MESH_MSG_OBJ_TRANSFER_START:
        if (pmesh_msg->msg_len == sizeof(obj_transfer_start_t))
        {
            obj_transfer_stat_stat_t stat;
            obj_transfer_start_t *pmsg = (obj_transfer_start_t *)pbuffer;
            if (obj_transfer_server_ctx.phase >= OBJ_TRANSFER_PHASE_WAITING_CHUNK)
            {
                stat = OBJ_TRANSFER_STAT_BUSY;
            }
            else if (pmsg->curr_block_size_log < BLOCK_SIZE_LOG_MIN ||
                     pmsg->curr_block_size_log > BLOCK_SIZE_LOG_MAX)
            {
                stat = OBJ_TRANSFER_STAT_INVALID_PARAMETER;
            }
            else
            {
                uint8_t loop = 0;
                for (; loop < sizeof(obj_transfer_server_ctx.object_id); loop++)
                {
                    if (pmsg->object_id[loop])
                    {
                        break;
                    }
                }
                if (loop >= sizeof(obj_transfer_server_ctx.object_id))
                {
                    stat = OBJ_TRANSFER_STAT_INVALID_PARAMETER;
                }
                else
                {
                    if (pmsg->object_size > obj_transfer_server_ctx.max_object_size)
                    {
                        stat = OBJ_TRANSFER_STAT_NOT_SUPPORTED;
                    }
                    else
                    {
                        if (obj_transfer_server_ctx.phase == OBJ_TRANSFER_PHASE_WAITING_BLOCK)
                        {
                            if (0 != memcmp(obj_transfer_server_ctx.object_id, pmsg->object_id,
                                            sizeof(obj_transfer_server_ctx.object_id)))
                            {
                                /* TODO: what to respond? */
                                stat = OBJ_TRANSFER_STAT_BUSY;//OBJ_TRANSFER_STAT_INVALID_PARAMETER
                            }
                            else
                            {
                                stat = OBJ_TRANSFER_STAT_BUSY;
                            }
                        }
                        else
                        {
                            memcpy(obj_transfer_server_ctx.object_id, pmsg->object_id,
                                   sizeof(obj_transfer_server_ctx.object_id));
                            obj_transfer_server_ctx.object_size = pmsg->object_size;
                            obj_transfer_server_ctx.block_size = plt_exp2(pmsg->curr_block_size_log);
                            obj_transfer_server_ctx.block_num = (obj_transfer_server_ctx.object_size +
                                                                 obj_transfer_server_ctx.block_size - 1) /
                                                                obj_transfer_server_ctx.block_size;
                            obj_transfer_server_ctx.current_block_num = 0;
                            if (obj_transfer_server_ctx.block_data)
                            {
                                plt_free(obj_transfer_server_ctx.block_data, RAM_TYPE_DATA_ON);
                            }
                            obj_transfer_server_ctx.block_data = plt_malloc(obj_transfer_server_ctx.block_size,
                                                                            RAM_TYPE_DATA_ON);
                            if (obj_transfer_server_ctx.block_data)
                            {
                                obj_transfer_server_ctx.phase = OBJ_TRANSFER_PHASE_WAITING_BLOCK;
                                stat = OBJ_TRANSFER_STAT_BUSY;
                            }
                            else
                            {
                                printe("obj_transfer_server_receive: malloc fail for the block of size %d!",
                                       obj_transfer_server_ctx.block_size);
                                stat = OBJ_TRANSFER_STAT_NOT_SUPPORTED;
                            }
                            printi("obj_transfer_server_receive: obj size %d, block size %d, block num %d, obj id",
                                   obj_transfer_server_ctx.object_size, obj_transfer_server_ctx.block_size,
                                   obj_transfer_server_ctx.block_num);
                            dprinti(obj_transfer_server_ctx.object_id, sizeof(obj_transfer_server_ctx.object_id));
                        }
                    }
                }
            }
            obj_transfer_stat(pmesh_msg, stat, obj_transfer_server_ctx.object_id,
                              obj_transfer_server_ctx.object_size, plt_log2(obj_transfer_server_ctx.block_size));
        }
        break;
    case MESH_MSG_OBJ_TRANSFER_ABORT:
        if (pmesh_msg->msg_len == sizeof(obj_transfer_abort_t))
        {
            obj_transfer_stat_stat_t stat;
            obj_transfer_abort_t *pmsg = (obj_transfer_abort_t *)pbuffer;
            if (obj_transfer_server_ctx.phase == OBJ_TRANSFER_PHASE_IDLE)
            {
                //OBJ_TRANSFER_STAT_CANNOT_ABORT
                stat = OBJ_TRANSFER_STAT_READY;
            }
            else
            {
                if (0 == memcmp(pmsg->object_id, obj_transfer_server_ctx.object_id,
                                sizeof(obj_transfer_server_ctx.object_id)))
                {
                    obj_transfer_server_ctx.phase = OBJ_TRANSFER_PHASE_IDLE;
                    plt_free(obj_transfer_server_ctx.block_data, RAM_TYPE_DATA_ON);
                    obj_transfer_server_ctx.block_data = NULL;
                    stat = OBJ_TRANSFER_STAT_READY;
                    if (obj_transfer_server.model_data_cb)
                    {
                        obj_transfer_server.model_data_cb(&obj_transfer_server, OBJ_TRANSFER_SERVER_CB_TYPE_ABORT,
                                                          &obj_transfer_server_ctx);
                    }
                }
                else
                {
                    stat = OBJ_TRANSFER_STAT_INVALID_PARAMETER;
                }
            }
            obj_transfer_stat(pmesh_msg, stat, obj_transfer_server_ctx.object_id,
                              obj_transfer_server_ctx.object_size, plt_log2(obj_transfer_server_ctx.block_size));
        }
        break;
    case MESH_MSG_OBJ_BLOCK_TRANSFER_START:
        obj_transfer_server_handle_obj_block_transfer_start(pmesh_msg);
        break;
    case MESH_MSG_OBJ_CHUNCK_TRANSFER:
        obj_transfer_server_handle_obj_chunk_transfer(pmesh_msg);
        break;
    case MESH_MSG_OBJ_BLOCK_GET:
        if (pmesh_msg->msg_len == sizeof(obj_block_get_t))
        {
            obj_transfer_server_handle_obj_block_get(pmesh_msg);
        }
        break;
    case MESH_MSG_OBJ_INFO_GET:
        if (pmesh_msg->msg_len == sizeof(obj_info_get_t))
        {
            obj_info_stat(pmesh_msg, obj_transfer_server_ctx.min_block_size_log,
                          obj_transfer_server_ctx.max_block_size_log, OBJ_TRANSFER_MAX_CHUNK_NUM);
        }
        break;
    default:
        ret = FALSE;
        break;
    }

    if (ret)
    {
        printi("obj_transfer_server_receive: opcode = 0x%x, phase = %d", pmesh_msg->access_opcode,
               obj_transfer_server_ctx.phase);
    }
    return ret;
}

void obj_transfer_server_reg(uint32_t max_object_size, model_data_cb_pf model_data_cb)
{
    obj_transfer_server.model_id = MESH_MODEL_OBJ_TRANSFER_SERVER;
    obj_transfer_server.model_receive = obj_transfer_server_receive;
    obj_transfer_server.model_data_cb = model_data_cb;
    mesh_model_reg(0, &obj_transfer_server);
    obj_transfer_server_ctx.max_object_size = max_object_size;
    obj_transfer_server_ctx.max_block_size_log = 12;
    obj_transfer_server_ctx.min_block_size_log = 10;
}

void obj_transfer_server_load(uint8_t object_id[8], uint32_t object_size, uint32_t block_size,
                              uint16_t current_block_num)
{
    memcpy(obj_transfer_server_ctx.object_id, object_id, 8);
    obj_transfer_server_ctx.object_size = object_size;
    obj_transfer_server_ctx.block_size = block_size;
    obj_transfer_server_ctx.current_block_num = current_block_num;
    obj_transfer_server_ctx.block_num = (obj_transfer_server_ctx.object_size +
                                         obj_transfer_server_ctx.block_size - 1) /
                                        obj_transfer_server_ctx.block_size;
    if (obj_transfer_server_ctx.block_data == NULL)
    {
        obj_transfer_server_ctx.block_data = plt_malloc(obj_transfer_server_ctx.block_size,
                                                        RAM_TYPE_DATA_ON);
    }
    if (obj_transfer_server_ctx.block_data)
    {
        obj_transfer_server_ctx.phase = OBJ_TRANSFER_PHASE_WAITING_BLOCK;
    }
    else
    {
        printe("obj_transfer_server_load: fail to allocate memory!");
    }
}

void obj_transfer_server_clear(void)
{
    obj_transfer_server_ctx.phase = OBJ_TRANSFER_PHASE_IDLE;
    if (obj_transfer_server_ctx.block_data)
    {
        plt_free(obj_transfer_server_ctx.block_data, RAM_TYPE_DATA_ON);
        obj_transfer_server_ctx.block_data = NULL;
    }
}

bool obj_transfer_server_busy(void)
{
    return obj_transfer_server_ctx.phase != OBJ_TRANSFER_PHASE_IDLE;
}


/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      dfu_distributor_app.c
* @brief     Smart mesh dfu application
* @details
* @author    bill
* @date      2018-6-6
* @version   v1.0
* *********************************************************************************************************
*/

#define MM_ID MM_MODEL

#include "dfu_distributor_app.h"
#include "patch_header_check.h"

#define DFU_DIST_NODE_NUM_MAX   5
#define DFU_DIST_RETRY_TIMES    3
#define DFU_DIST_RETRY_PERIOD   7000

typedef enum
{
    DFU_DIST_IDLE,
    DFU_DIST_STARTED,
    DFU_DIST_OBJ_INFO_GETTED,
    DFU_DIST_FW_PREPARED,
    DFU_DIST_FW_STARTED,
    DFU_DIST_OBJ_STARTED,
    DFU_DIST_OBJ_BLOCK_STARTED,
    DFU_DIST_OBJ_CHUNK_TRANSFERED, //!< then check
    DFU_DIST_OBJ_TRANSFERED
} dfu_dist_fsm_t;

typedef struct
{
    uint16_t addr;
    fw_update_phase_t phase;
} dfu_dist_node_info_t;

struct
{
    plt_timer_t timer;
    dfu_dist_fsm_t fsm;
    uint16_t dst; //!< usually group addr
    dfu_dist_node_info_t node_info[DFU_DIST_NODE_NUM_MAX];
    uint16_t node_num;
    uint16_t node_loop;
    uint16_t company_id;
    uint8_t firmware_id[FW_UPDATE_FW_ID_LEN];
    uint8_t count;
    uint32_t min_block_size;
    uint32_t max_block_size;
    uint16_t max_chunk_num;
    uint8_t object_id[8];
    uint32_t object_size;
    uint32_t block_size;
    uint16_t block_num;
    uint16_t block_loop;
    uint16_t chunk_size;
    uint16_t chunk_num;
    uint16_t chunk_loop;
    uint16_t *chunk_list;
    uint16_t chunk_list_num;
    uint16_t chunk_list_loop;
    fw_update_policy_t policy;
} dfu_dist_ctx;

void dfu_dist_timeout_cb(void *timer)
{

}

void dfu_dist_block_start(void)
{
    uint32_t block_size;
    if (dfu_dist_ctx.block_loop == dfu_dist_ctx.block_num - 1)
    {
        block_size = dfu_dist_ctx.object_size % dfu_dist_ctx.block_size;
        if (!block_size)
        {
            block_size = dfu_dist_ctx.block_size;
        }
    }
    else
    {
        block_size = dfu_dist_ctx.block_size;
    }
    dfu_dist_ctx.chunk_size = (block_size + dfu_dist_ctx.max_chunk_num - 1) /
                              dfu_dist_ctx.max_chunk_num;
    dfu_dist_ctx.chunk_num = (block_size + dfu_dist_ctx.chunk_size - 1) / dfu_dist_ctx.chunk_size;
    if (dfu_dist_ctx.chunk_size > (ACCESS_PAYLOAD_MAX_SIZE - MEMBER_OFFSET(obj_chunk_transfer_t, data)))
    {
        // TODO:
    }
    obj_block_transfer_start(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr,
                             dfu_dist_ctx.object_id, dfu_dist_ctx.block_loop,
                             dfu_dist_ctx.chunk_size, OBJ_BLOCK_CHECK_ALGO_CRC32, NULL, 0);
}

void dfu_dist_chunk_start(void)
{
    uint16_t chunk_size;
    uint32_t block_size;
    if (dfu_dist_ctx.block_loop == dfu_dist_ctx.block_num - 1)
    {
        block_size = dfu_dist_ctx.object_size % dfu_dist_ctx.block_size;
        if (!block_size)
        {
            block_size = dfu_dist_ctx.block_size;
        }
    }
    else
    {
        block_size = dfu_dist_ctx.block_size;
    }
    if (dfu_dist_ctx.chunk_loop == dfu_dist_ctx.chunk_num - 1)
    {
        chunk_size = block_size % dfu_dist_ctx.chunk_size;
        if (!chunk_size)
        {
            chunk_size = dfu_dist_ctx.chunk_size;
        }
    }
    else
    {
        chunk_size = dfu_dist_ctx.chunk_size;
    }
    uint8_t *chunk_data = plt_malloc(chunk_size, RAM_TYPE_DATA_OFF);
    if (NULL == chunk_data)
    {
        printe("dfu: dfu_dist_chunk_start malloc fail!");
        return;
    }
    uint8_t *pheader = (uint8_t *)get_header_addr_by_img_id(AppPatch);
    memcpy(chunk_data, pheader + dfu_dist_ctx.block_loop * dfu_dist_ctx.block_size +
           dfu_dist_ctx.chunk_loop * dfu_dist_ctx.chunk_size, chunk_size);
    printi("dfu: obj size %d, block num %d/%d size %d, chunk num %d/%d size %d",
           dfu_dist_ctx.object_size,
           dfu_dist_ctx.block_loop, dfu_dist_ctx.block_num, block_size,
           dfu_dist_ctx.chunk_loop, dfu_dist_ctx.chunk_num, chunk_size);
    obj_chunk_transfer(dfu_dist_ctx.dst, dfu_dist_ctx.chunk_loop, chunk_data, chunk_size);
    plt_free(chunk_data, RAM_TYPE_DATA_OFF);
}

void obj_transfer_client_send_cb(mesh_model_info_p pmodel_info, mesh_msg_send_stat_t stat,
                                 uint32_t access_opcode)
{
    if (access_opcode == MESH_MSG_OBJ_CHUNCK_TRANSFER)
    {
        if (dfu_dist_ctx.fsm == DFU_DIST_OBJ_BLOCK_STARTED)
        {
            dfu_dist_ctx.chunk_loop++;
            if (dfu_dist_ctx.chunk_loop < dfu_dist_ctx.chunk_num)
            {
                dfu_dist_chunk_start();
            }
            else
            {
                dfu_dist_ctx.fsm = DFU_DIST_OBJ_CHUNK_TRANSFERED;
                dfu_dist_ctx.node_loop = 0;
                obj_block_get(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr, dfu_dist_ctx.object_id,
                              dfu_dist_ctx.block_num);
            }
        }
        else if (dfu_dist_ctx.fsm == DFU_DIST_OBJ_CHUNK_TRANSFERED)
        {
            dfu_dist_ctx.chunk_list_loop++;
            if (dfu_dist_ctx.chunk_list_loop < dfu_dist_ctx.chunk_list_num)
            {
                dfu_dist_ctx.chunk_loop = dfu_dist_ctx.chunk_list[dfu_dist_ctx.chunk_list_loop];
                dfu_dist_chunk_start();
            }
            else
            {
                obj_block_get(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr, dfu_dist_ctx.object_id,
                              dfu_dist_ctx.block_num);
            }
        }
    }
}

bool obj_transfer_client_receive(mesh_msg_p pmesh_msg)
{
    bool ret = TRUE;
    uint8_t *pbuffer = pmesh_msg->pbuffer + pmesh_msg->msg_offset;
    switch (pmesh_msg->access_opcode)
    {
    case MESH_MSG_OBJ_INFO_STAT:
        if (pmesh_msg->msg_len == sizeof(obj_info_stat_t))
        {
            obj_info_stat_t *pmsg = (obj_info_stat_t *)pbuffer;
            if (pmesh_msg->src == dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr)
            {
                if (!dfu_dist_ctx.node_loop)
                {
                    dfu_dist_ctx.min_block_size = plt_exp2(pmsg->min_block_size_log);
                    dfu_dist_ctx.max_block_size = plt_exp2(pmsg->max_block_size_log);
                    dfu_dist_ctx.max_chunk_num = pmsg->max_chunk_num;
                }
                else
                {
                    if (dfu_dist_ctx.min_block_size < plt_exp2(pmsg->min_block_size_log))
                    {
                        dfu_dist_ctx.min_block_size = plt_exp2(pmsg->min_block_size_log);
                    }
                    if (dfu_dist_ctx.max_block_size > plt_exp2(pmsg->max_block_size_log))
                    {
                        dfu_dist_ctx.max_block_size = plt_exp2(pmsg->max_block_size_log);
                    }
                    if (dfu_dist_ctx.max_chunk_num > pmsg->max_chunk_num)
                    {
                        dfu_dist_ctx.max_chunk_num = pmsg->max_chunk_num;
                    }
                }

                plt_timer_reset(dfu_dist_ctx.timer, 0);
                dfu_dist_ctx.node_loop++;
                if (dfu_dist_ctx.node_loop < dfu_dist_ctx.node_num)
                {
                    obj_info_get(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr);
                }
                else
                {
                    if (dfu_dist_ctx.min_block_size > dfu_dist_ctx.max_block_size)
                    {
                        // TODO:
                    }
                    else
                    {
                        dfu_dist_ctx.block_size = dfu_dist_ctx.max_block_size;
                        dfu_dist_ctx.block_num = (dfu_dist_ctx.object_size + dfu_dist_ctx.block_size - 1) /
                                                 dfu_dist_ctx.block_size;
                    }
                    dfu_dist_ctx.fsm = DFU_DIST_OBJ_INFO_GETTED;
                    dfu_dist_ctx.node_loop = 0;
                    fw_update_prepare(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr, dfu_dist_ctx.company_id,
                                      dfu_dist_ctx.firmware_id,
                                      dfu_dist_ctx.object_id, NULL, 0);
                }
            }
        }
        break;
    case MESH_MSG_OBJ_TRANSFER_STAT:
        if (pmesh_msg->msg_len == sizeof(obj_transfer_stat_t))
        {
            if (pmesh_msg->src == dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr)
            {
                plt_timer_reset(dfu_dist_ctx.timer, 0);
                dfu_dist_ctx.node_loop++;
                if (dfu_dist_ctx.node_loop < dfu_dist_ctx.node_num)
                {
                    obj_transfer_start(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr, dfu_dist_ctx.object_id,
                                       dfu_dist_ctx.object_size, plt_log2(dfu_dist_ctx.block_size));
                }
                else
                {
                    dfu_dist_ctx.fsm = DFU_DIST_OBJ_STARTED;
                    dfu_dist_ctx.node_loop = 0;
                    dfu_dist_ctx.block_loop = 0;
                    dfu_dist_block_start();
                }
            }
        }
        break;
    case MESH_MSG_OBJ_BLOCK_TRANSFER_STAT:
        if (pmesh_msg->msg_len == sizeof(obj_block_transfer_stat_t))
        {
            if (pmesh_msg->src == dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr)
            {
                plt_timer_reset(dfu_dist_ctx.timer, 0);
                dfu_dist_ctx.node_loop++;
                if (dfu_dist_ctx.node_loop < dfu_dist_ctx.node_num)
                {
                    dfu_dist_block_start();
                }
                else
                {
                    dfu_dist_ctx.fsm = DFU_DIST_OBJ_BLOCK_STARTED;
                    dfu_dist_ctx.node_loop = 0;
                    dfu_dist_ctx.chunk_loop = 0;
                    dfu_dist_chunk_start();
                }
            }
        }
        break;
    case MESH_MSG_OBJ_BLOCK_STAT:
        if (pmesh_msg->src == dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr)
        {
            obj_block_stat_t *pmsg = (obj_block_stat_t *)pbuffer;
            if (pmsg->stat == OBJ_BLOCK_TRANSFER_STAT_NOT_ALL_CHUNCK_RECEIVED)
            {
                if (dfu_dist_ctx.chunk_list_num || dfu_dist_ctx.chunk_list)
                {
                    dfu_dist_ctx.chunk_list_num = 0;
                    plt_free(dfu_dist_ctx.chunk_list, RAM_TYPE_DATA_ON);
                    dfu_dist_ctx.chunk_list = NULL;
                }
                uint16_t chunk_list_len = (pmesh_msg->msg_len - MEMBER_OFFSET(obj_block_stat_t, chunk_list));
                if (chunk_list_len)
                {
                    dfu_dist_ctx.chunk_list = plt_malloc(chunk_list_len, RAM_TYPE_DATA_ON);
                    memcpy(dfu_dist_ctx.chunk_list, pmsg->chunk_list, chunk_list_len);
                    dfu_dist_ctx.chunk_list_num = chunk_list_len >> 1;
                    dfu_dist_ctx.chunk_list_loop = 0;
                    dfu_dist_ctx.chunk_loop = dfu_dist_ctx.chunk_list[0];
                    dfu_dist_chunk_start();
                }
                else
                {
                    // wrong case
                }
            }
            else
            {
                dfu_dist_ctx.node_loop++;
                if (dfu_dist_ctx.node_loop < dfu_dist_ctx.node_num)
                {
                    obj_block_get(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr, dfu_dist_ctx.object_id,
                                  dfu_dist_ctx.block_num);
                }
                else
                {
                    dfu_dist_ctx.node_loop = 0;
                    dfu_dist_ctx.block_loop++;
                    if (dfu_dist_ctx.block_loop < dfu_dist_ctx.block_num)
                    {
                        dfu_dist_ctx.fsm = DFU_DIST_OBJ_STARTED;
                        dfu_dist_block_start();
                    }
                    else
                    {
                        dfu_dist_ctx.fsm = DFU_DIST_OBJ_TRANSFERED;
                        fw_update_apply(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr, dfu_dist_ctx.company_id,
                                        dfu_dist_ctx.firmware_id);
                    }
                }
            }
        }
        break;
    default:
        ret = FALSE;
        break;
    }
    if (ret)
    {
        printi("obj_transfer_client_receive: opcode = 0x%x, fsm = %d", pmesh_msg->access_opcode,
               dfu_dist_ctx.fsm);
    }
    return ret;
}

bool fw_update_client_receive(mesh_msg_p pmesh_msg)
{
    bool ret = TRUE;
    uint8_t *pbuffer = pmesh_msg->pbuffer + pmesh_msg->msg_offset;
    switch (pmesh_msg->access_opcode)
    {
    case MESH_MSG_FW_INFO_STAT:
        break;
    case MESH_MSG_FW_UPDATE_STAT:
        if (pmesh_msg->msg_len == sizeof(fw_update_stat_t))
        {
            fw_update_stat_t *pmsg = (fw_update_stat_t *)pbuffer;
            if (pmesh_msg->src == dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr)
            {
                dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].phase = (fw_update_phase_t)pmsg->phase;
                plt_timer_reset(dfu_dist_ctx.timer, 0);
                dfu_dist_ctx.node_loop++;
                if (dfu_dist_ctx.node_loop < dfu_dist_ctx.node_num)
                {
                    if (dfu_dist_ctx.fsm == DFU_DIST_OBJ_INFO_GETTED)
                    {
                        fw_update_prepare(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr, dfu_dist_ctx.company_id,
                                          dfu_dist_ctx.firmware_id,
                                          dfu_dist_ctx.object_id, NULL, 0);
                    }
                    else if (dfu_dist_ctx.fsm == DFU_DIST_FW_PREPARED)
                    {
                        fw_update_start(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr, dfu_dist_ctx.policy,
                                        dfu_dist_ctx.company_id, dfu_dist_ctx.firmware_id);
                    }
                    else if (dfu_dist_ctx.fsm == DFU_DIST_OBJ_TRANSFERED)
                    {
                        fw_update_apply(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr, dfu_dist_ctx.company_id,
                                        dfu_dist_ctx.firmware_id);
                    }
                }
                else
                {
                    dfu_dist_ctx.node_loop = 0;
                    if (dfu_dist_ctx.fsm == DFU_DIST_OBJ_INFO_GETTED)
                    {
                        dfu_dist_ctx.fsm = DFU_DIST_FW_PREPARED;
                        fw_update_start(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr, dfu_dist_ctx.policy,
                                        dfu_dist_ctx.company_id, dfu_dist_ctx.firmware_id);
                    }
                    else if (dfu_dist_ctx.fsm == DFU_DIST_FW_PREPARED)
                    {
                        dfu_dist_ctx.fsm = DFU_DIST_FW_STARTED;
                        obj_transfer_start(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr, dfu_dist_ctx.object_id,
                                           dfu_dist_ctx.object_size, plt_log2(dfu_dist_ctx.block_size));
                    }
                    else if (dfu_dist_ctx.fsm == DFU_DIST_OBJ_TRANSFERED)
                    {
                        /* Oh my god, finally it is all over! */
                        dfu_dist_ctx.fsm = DFU_DIST_IDLE;
                        plt_timer_stop(dfu_dist_ctx.timer, 0);
                        // TODO: inform the upper layer
                    }
                }
            }
        }
        break;
    default:
        ret = FALSE;
        break;
    }
    if (ret)
    {
        printi("fw_update_client_receive: opcode = 0x%x, fsm = %d", pmesh_msg->access_opcode,
               dfu_dist_ctx.fsm);
    }
    return ret;
}

bool dfu_dist_start(uint16_t company_id, fw_update_fw_id_t firmware_id, uint8_t object_id[8],
                    uint16_t dst, uint16_t node_list[], uint16_t node_num)
{
    dfu_dist_ctx.dst = dst;
    T_IMG_HEADER_FORMAT *p_header = (T_IMG_HEADER_FORMAT *)get_header_addr_by_img_id(AppPatch);
    if (0 == p_header->ctrl_header.payload_len)
    {
        printe("dfu_dist_start: fail, wrong downloaded image!");
        return false;
    }
    dfu_dist_ctx.object_size = p_header->ctrl_header.payload_len + 1024;
    dfu_dist_ctx.company_id = company_id;
    FW_UPDATE_FW_ID(dfu_dist_ctx.firmware_id, firmware_id);
    dfu_dist_ctx.node_num = MIN(DFU_DIST_NODE_NUM_MAX, node_num);
    for (dfu_dist_ctx.node_loop = 0; dfu_dist_ctx.node_loop < dfu_dist_ctx.node_num;
         dfu_dist_ctx.node_loop++)
    {
        dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr = node_list[dfu_dist_ctx.node_loop];
    }
    //plt_rand(dfu_dist_ctx.object_id, sizeof(dfu_dist_ctx.object_id));
    memcpy(dfu_dist_ctx.object_id, object_id, sizeof(dfu_dist_ctx.object_id));
    if (!dfu_dist_ctx.timer)
    {
        dfu_dist_ctx.timer = plt_timer_create("dfu", DFU_DIST_RETRY_PERIOD, false, 0, dfu_dist_timeout_cb);
        if (!dfu_dist_ctx.timer)
        {
            return false;
        }
    }
    plt_timer_start(dfu_dist_ctx.timer, 0);
    dfu_dist_ctx.fsm = DFU_DIST_STARTED;
    dfu_dist_ctx.node_loop = 0;
    obj_info_get(dfu_dist_ctx.node_info[dfu_dist_ctx.node_loop].addr);
    return true;
}

void dfu_dist_models_init(void)
{
    fw_update_client_reg(fw_update_client_receive);
    obj_transfer_client_reg(obj_transfer_client_receive, obj_transfer_client_send_cb);
}

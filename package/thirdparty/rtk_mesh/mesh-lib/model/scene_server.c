/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     scene_server.c
* @brief    Source file for scene server model.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-8-23
* @version  v1.0
* *************************************************************************************
*/

#include "scene.h"
#include "generic_types.h"
#if MODEL_ENABLE_DELAY_EXECUTION
#include "delay_execution.h"
#endif


typedef struct
{
    scene_storage_memory_t *scenes;
    uint16_t num_scenes;
    scene_status_code_t status_recall;
    uint16_t target_scene;
#if MODEL_ENABLE_DELAY_EXECUTION
    generic_transition_time_t trans_time;
    uint32_t delay_time;
    scene_storage_memory_t *state_memory;
#endif
} scene_info_t;

void scene_server_set_storage_memory(mesh_model_info_p pmodel_info, scene_storage_memory_t *scenes,
                                     uint16_t num_scenes)
{
    scene_info_t *pinfo = pmodel_info->pargs;
    pinfo->scenes = scenes;
    pinfo->num_scenes = num_scenes;
}

static scene_storage_memory_t *scene_storage_memory_get(const mesh_model_info_p pmodel_info,
                                                        uint16_t scene_number)
{
    scene_info_t *pinfo = pmodel_info->pargs;
    for (uint16_t i = 0; i < pinfo->num_scenes; ++i)
    {
        if (pinfo->scenes[i].scene_number == scene_number)
        {
            return &pinfo->scenes[i];
        }
    }

    return NULL;
}

static mesh_msg_send_cause_t scene_server_send(mesh_model_info_p pmodel_info,
                                               uint16_t dst, void *pmsg, uint16_t msg_len, uint16_t app_key_index)
{
    mesh_msg_t mesh_msg;
    mesh_msg.pmodel_info = pmodel_info;
    access_cfg(&mesh_msg);
    mesh_msg.pbuffer = pmsg;
    mesh_msg.msg_len = msg_len;
    if (0 != dst)
    {
        mesh_msg.dst = dst;
        mesh_msg.app_key_index = app_key_index;
    }
    return access_send(&mesh_msg);
}

mesh_msg_send_cause_t scene_status(mesh_model_info_p pmodel_info, uint16_t dst,
                                   uint16_t app_key_index, scene_status_code_t status, uint16_t current_scene,
                                   bool optional, uint16_t target_scene,
                                   generic_transition_time_t remaining_time)
{
    scene_status_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_SCENE_STATUS);
    uint16_t msg_len;
    if (optional)
    {
        msg_len = sizeof(scene_status_t);
        msg.target_scene = target_scene;
        msg.remaining_time = remaining_time;
    }
    else
    {
        msg_len = MEMBER_OFFSET(scene_status_t, target_scene);
    }
    msg.status = status;
    msg.current_scene = current_scene;

    return scene_server_send(pmodel_info, dst, &msg, msg_len, app_key_index);
}

mesh_msg_send_cause_t scene_register_status(mesh_model_info_p pmodel_info, uint16_t dst,
                                            uint16_t app_key_index,
                                            scene_status_code_t status, uint16_t current_scene)
{
    scene_info_t *pinfo = pmodel_info->pargs;
    scene_register_status_t *pmsg = plt_malloc(sizeof(scene_register_status_t) + pinfo->num_scenes *
                                               sizeof(uint16_t), RAM_TYPE_DATA_ON);
    if (NULL == pmsg)
    {
        return MESH_MSG_SEND_CAUSE_NO_MEMORY;
    }
    ACCESS_OPCODE_BYTE(pmsg->opcode, MESH_MSG_SCENE_REGISTER_STATUS);
    pmsg->status = status;
    pmsg->current_scene = current_scene;
    uint16_t valid_scene_count = 0;
    for (uint16_t i = 0; i < pinfo->num_scenes; ++i)
    {
        if (IS_SCENE_NUMBER_VALID(pinfo->scenes[i].scene_number))
        {
            pmsg->scenes[valid_scene_count] = pinfo->scenes[i].scene_number;
            valid_scene_count ++;
        }
    }
    uint16_t msg_len = sizeof(scene_register_status_t) + valid_scene_count * sizeof(uint16_t);

    mesh_msg_send_cause_t cause = scene_server_send(pmodel_info, dst, pmsg, msg_len, app_key_index);
    plt_free(pmsg, RAM_TYPE_DATA_OFF);
    return cause;
}

mesh_msg_send_cause_t scene_publish(const mesh_model_info_p pmodel_info,
                                    uint16_t scene)
{
    mesh_msg_send_cause_t ret = MESH_MSG_SEND_CAUSE_INVALID_DST;
    if (mesh_model_pub_check(pmodel_info))
    {
        generic_transition_time_t trans_time = {0, 0};
        ret = scene_status(pmodel_info, 0, 0, SCENE_STATUS_SUCCESS, scene, FALSE, scene, trans_time);
    }

    return ret;
}

uint16_t get_current_scene(const mesh_model_info_p pmodel_info)
{
    scene_server_get_t get_data = {0};
    if (NULL != pmodel_info->model_data_cb)
    {
        pmodel_info->model_data_cb(pmodel_info, SCENE_SERVER_GET, &get_data);
    }

    return get_data.current_scene;
}

static int32_t scene_trans_step_change(const mesh_model_info_p pmodel_info, uint32_t type,
                                       generic_transition_time_t total_time,
                                       generic_transition_time_t remaining_time)
{
    int32_t ret = MODEL_SUCCESS;
    scene_info_t *pinfo = pmodel_info->pargs;
    if (NULL == pinfo)
    {
        return 0;
    }
    scene_storage_memory_t *state_memory = scene_storage_memory_get(pmodel_info, pinfo->target_scene);
    scene_server_recall_t set_data;
    if (NULL != state_memory)
    {
        set_data.scene_number = pinfo->target_scene;
        set_data.total_time = total_time;
        set_data.remaining_time = remaining_time;
        set_data.pmemory = state_memory->pmemory;
    }
    else
    {
        /* scene has been deleted before transition done */
        set_data.scene_number = 0;
        set_data.total_time = total_time;
        set_data.remaining_time = remaining_time;
        set_data.pmemory = NULL;
    }

    if (NULL != pmodel_info->model_data_cb)
    {
        ret = pmodel_info->model_data_cb(pmodel_info, SCENE_SERVER_RECALL, &set_data);
    }

    /* scene transition shall be terminated when target scene has bee deleted */
    if (NULL == state_memory)
    {
        ret = MODEL_STOP_TRANSITION;
    }

    if (0 == remaining_time.num_steps)
    {
        uint16_t current_scene = get_current_scene(pmodel_info);
        scene_publish(pmodel_info, current_scene);
    }

    return ret;
}

static uint16_t scene_process(const mesh_model_info_p pmodel_info,
                              const scene_storage_memory_t *state_memory,
                              uint16_t target_scene,
                              generic_transition_time_t trans_time)
{
    uint16_t scene_before_set;
    uint16_t scene_after_set = 0;

    /* get scene before set */
    scene_before_set = get_current_scene(pmodel_info);

    int32_t ret = MODEL_SUCCESS;
    scene_server_recall_t trans_set_data;
    trans_set_data.total_time = trans_time;
    trans_set_data.remaining_time = trans_time;
    trans_set_data.scene_number = target_scene;
    trans_set_data.pmemory = state_memory->pmemory;

    if (NULL != pmodel_info->model_data_cb)
    {
        ret = pmodel_info->model_data_cb(pmodel_info, SCENE_SERVER_RECALL, &trans_set_data);
    }

    if (GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE != trans_time.num_steps)
    {
        if ((ret >= 0) && (MODEL_STOP_TRANSITION != ret))
        {
            generic_transition_timer_start(pmodel_info, GENERIC_TRANSITION_TYPE_SCENE, trans_time,
                                           scene_trans_step_change);
        }
#if MODEL_ENABLE_USER_STOP_TRANSITION_NOTIFICATION
        else if (MODEL_STOP_TRANSITION == ret)
        {
            if (NULL != pmodel_info->model_data_cb)
            {
                ret = pmodel_info->model_data_cb(pmodel_info, SCENE_SERVER_RECALL, &trans_set_data);
            }
        }
#endif
    }

    /* get scene after set */
    if (0 == trans_time.num_steps)
    {
        scene_after_set = get_current_scene(pmodel_info);
    }

    if (scene_before_set != scene_after_set)
    {
        scene_publish(pmodel_info, scene_after_set);
    }

    return scene_after_set;
}

#if MODEL_ENABLE_DELAY_EXECUTION
static int32_t scene_delay_execution(mesh_model_info_t *pmodel_info, uint32_t delay_type)
{
    scene_info_t *pinfo = pmodel_info->pargs;
    if (NULL == pinfo)
    {
        return 0;
    }
    pinfo->delay_time = 0;
    scene_process(pmodel_info, pinfo->state_memory, pinfo->target_scene, pinfo->trans_time);

    return 0;
}
#endif

static bool scene_server_receive(mesh_msg_p pmesh_msg)
{
    bool ret = TRUE;
    uint8_t *pbuffer = pmesh_msg->pbuffer + pmesh_msg->msg_offset;
    mesh_model_info_p pmodel_info = pmesh_msg->pmodel_info;

    switch (pmesh_msg->access_opcode)
    {
    case MESH_MSG_SCENE_GET:
        if (pmesh_msg->msg_len == sizeof(scene_get_t))
        {
            uint16_t current_scene = 0;
            scene_info_t *pinfo = pmodel_info->pargs;
            generic_transition_time_t remaining_time;
#if MODEL_ENABLE_DELAY_EXECUTION
            if (pinfo->delay_time > 0)
            {
                remaining_time = pinfo->trans_time;
            }
            else
#endif
            {
                remaining_time = generic_transition_time_get(pmesh_msg->pmodel_info,
                                                             GENERIC_TRANSITION_TYPE_SCENE);
                if (0 == remaining_time.num_steps)
                {
                    current_scene = get_current_scene(pmodel_info);
                }
            }
            scene_status(pmodel_info, pmesh_msg->src, pmesh_msg->app_key_index,
                         pinfo->status_recall, current_scene,
                         (GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE == remaining_time.num_steps) ? FALSE : TRUE,
                         pinfo->target_scene, remaining_time);
        }
        break;
    case MESH_MSG_SCENE_RECALL:
    case MESH_MSG_SCENE_RECALL_UNACK:
        if ((pmesh_msg->msg_len == MEMBER_OFFSET(scene_recall_t, trans_time)) ||
            (pmesh_msg->msg_len == sizeof(scene_recall_t)))
        {
            scene_recall_t *pmsg = (scene_recall_t *)pbuffer;
            if (IS_SCENE_NUMBER_VALID(pmsg->scene_number))
            {
                generic_transition_time_t trans_time = {GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE, 0};
#if MODEL_ENABLE_DELAY_EXECUTION
                uint32_t delay_time = 0;
#endif
                if (pmesh_msg->msg_len == MEMBER_OFFSET(scene_recall_t, trans_time))
                {
                    if (NULL != pmodel_info->model_data_cb)
                    {
                        pmodel_info->model_data_cb(pmodel_info, SCENE_SERVER_GET_DEFAULT_TRANSITION_TIME,
                                                   &trans_time);
                    }
                }
                else
                {
                    trans_time = pmsg->trans_time;
#if MODEL_ENABLE_DELAY_EXECUTION
                    delay_time = pmsg->delay * DELAY_EXECUTION_STEP_RESOLUTION;
#endif
                }
                if (IS_GENERIC_TRANSITION_STEPS_VALID(trans_time.num_steps))
                {
                    scene_info_t *pinfo = pmodel_info->pargs;
                    uint16_t current_scene = 0;
                    pinfo->target_scene = pmsg->scene_number;
                    scene_storage_memory_t *state_memory = scene_storage_memory_get(pmodel_info, pmsg->scene_number);
#if MODEL_ENABLE_DELAY_EXECUTION
                    pinfo->trans_time = trans_time;
                    pinfo->delay_time = delay_time;
                    pinfo->state_memory = state_memory;
#endif
                    if (NULL != state_memory)
                    {
#if MODEL_ENABLE_DELAY_EXECUTION
                        if (delay_time > 0)
                        {
                            current_scene = 0;
                            delay_execution_timer_start(pmodel_info, DELAY_EXECUTION_TYPE_SCENE, delay_time,
                                                        scene_delay_execution);
                        }
                        else
#endif
                        {
                            current_scene = scene_process(pmodel_info, state_memory, pmsg->scene_number, trans_time);
                        }
                        pinfo->status_recall = SCENE_STATUS_SUCCESS;
                    }
                    else
                    {
                        current_scene = get_current_scene(pmodel_info);
                        pinfo->status_recall = SCENE_STATUS_NOT_FOUND;
                    }

                    if (MESH_MSG_SCENE_RECALL == pmesh_msg->access_opcode)
                    {
                        scene_status(pmodel_info, pmesh_msg->src, pmesh_msg->app_key_index,
                                     pinfo->status_recall, current_scene,
                                     (GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE == trans_time.num_steps) ? FALSE : TRUE,
                                     pmsg->scene_number, trans_time);
                    }
                }
            }
        }
        break;
    case MESH_MSG_SCENE_REGISTER_GET:
        if (pmesh_msg->msg_len == sizeof(scene_register_get_t))
        {
            uint16_t current_scene = get_current_scene(pmodel_info);
            scene_server_get_register_status_t get_data = {SCENE_STATUS_SUCCESS};
            if (NULL != pmodel_info->model_data_cb)
            {
                pmodel_info->model_data_cb(pmodel_info, SCENE_SERVER_GET_REGISTER_STATUS, &get_data);
            }
            scene_register_status(pmodel_info, pmesh_msg->src, pmesh_msg->app_key_index,
                                  get_data.status, current_scene);
        }
        break;
    default:
        ret = FALSE;
        break;
    }
    return ret;
}

static int32_t scene_server_publish(mesh_model_info_p pmodel_info, bool retrans)
{
    generic_transition_time_t trans_time = {0, 0};
    scene_status(pmodel_info, 0, 0, SCENE_STATUS_SUCCESS, get_current_scene(pmodel_info), FALSE, 0,
                 trans_time);
    return 0;
}

#if MESH_MODEL_ENABLE_DEINIT
static void scene_server_deinit(mesh_model_info_t *pmodel_info)
{
    if (pmodel_info->model_receive == scene_server_receive)
    {
#if MODEL_ENABLE_DELAY_EXECUTION
        /* stop delay execution */
        delay_execution_timer_stop(pmodel_info, DELAY_EXECUTION_TYPE_SCENE);
#endif
        /* stop step transition */
        generic_transition_timer_stop(pmodel_info, GENERIC_TRANSITION_TYPE_SCENE);

        /* now we can remove */
        plt_free(pmodel_info->pargs, RAM_TYPE_DATA_ON);
        pmodel_info->pargs = NULL;
        pmodel_info->model_receive = NULL;
    }
}
#endif

bool scene_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info)
{
    if (NULL == pmodel_info)
    {
        return FALSE;
    }

    pmodel_info->model_id = MESH_MODEL_SCENE_SERVER;
    if (NULL == pmodel_info->model_receive)
    {
        pmodel_info->pargs = plt_malloc(sizeof(scene_info_t), RAM_TYPE_DATA_ON);
        if (NULL == pmodel_info->pargs)
        {
            printe("scene_server_reg: fail to allocate memory for the new model extension data!");
            return FALSE;
        }
        memset(pmodel_info->pargs, 0, sizeof(scene_info_t));

        pmodel_info->model_receive = scene_server_receive;
        if (NULL == pmodel_info->model_data_cb)
        {
            printw("scene_server_reg: missing model data process callback!");
        }
#if MESH_MODEL_ENABLE_DEINIT
        pmodel_info->model_deinit = scene_server_deinit;
#endif
    }

    if (NULL == pmodel_info->model_pub_cb)
    {
        pmodel_info->model_pub_cb = scene_server_publish;
    }

    generic_transition_time_init();
#if MODEL_ENABLE_DELAY_EXECUTION
    delay_execution_init();
#endif
    return mesh_model_reg(element_index, pmodel_info);
}


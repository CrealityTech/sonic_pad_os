/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     light_hsl_saturation_server.c
* @brief    Source file for light hsl saturation server model.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-8-1
* @version  v1.0
* *************************************************************************************
*/

#include "light_hsl.h"
#if MODEL_ENABLE_DELAY_EXECUTION
#include "delay_execution.h"
#endif

extern mesh_msg_send_cause_t light_hsl_saturation_stat(mesh_model_info_p pmodel_info, uint16_t dst,
                                                       uint16_t app_key_index, uint16_t present_saturation, bool optional, uint16_t target_saturation,
                                                       generic_transition_time_t remaining_time);

typedef struct
{
    uint8_t tid;
    uint16_t target_saturation;
#if MODEL_ENABLE_DELAY_EXECUTION
    generic_transition_time_t trans_time;
    uint32_t delay_time;
#endif
} light_hsl_saturation_info_t;

typedef struct
{
    bool state_changed;
#if MODEL_ENABLE_PUBLISH_ALL_TIME
    bool use_transition;
#endif
} light_hsl_saturation_process_result_t;

int16_t light_saturation_to_generic_level(uint16_t saturation)
{
    return saturation - 32768;
}

uint16_t generic_level_to_light_saturation(int16_t level)
{
    return level + 32768;
}

static mesh_msg_send_cause_t light_hsl_saturation_server_send(mesh_model_info_p pmodel_info,
                                                              uint16_t dst, uint8_t *pmsg, uint16_t msg_len, uint16_t app_key_index)
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

mesh_msg_send_cause_t light_hsl_saturation_stat(mesh_model_info_p pmodel_info, uint16_t dst,
                                                uint16_t app_key_index, uint16_t present_saturation, bool optional, uint16_t target_saturation,
                                                generic_transition_time_t remaining_time)
{
    light_hsl_saturation_stat_t msg;
    uint32_t len;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_LIGHT_HSL_SATURATION_STAT);
    msg.present_saturation = present_saturation;
    if (optional)
    {
        len = sizeof(light_hsl_saturation_stat_t);
        msg.target_saturation = target_saturation;
        msg.remaining_time = remaining_time;
    }
    else
    {
        len = MEMBER_OFFSET(light_hsl_saturation_stat_t, target_saturation);
    }
    return light_hsl_saturation_server_send(pmodel_info, dst, (uint8_t *)&msg, len, app_key_index);
}

mesh_msg_send_cause_t light_hsl_saturation_publish(const mesh_model_info_p pmodel_info,
                                                   uint16_t saturation)
{
    mesh_msg_send_cause_t ret = MESH_MSG_SEND_CAUSE_INVALID_DST;
    if (mesh_model_pub_check(pmodel_info))
    {
        generic_transition_time_t trans_time = {0, 0};
        ret = light_hsl_saturation_stat(pmodel_info, 0, 0, saturation, FALSE, 0, trans_time);
    }

    return ret;
}

static void light_hsl_saturation_state_change_publish(const mesh_model_info_p pmodel_info,
                                                      uint16_t saturation, light_hsl_saturation_process_result_t result)
{
#if MODEL_ENABLE_PUBLISH_ALL_TIME
    if (result.use_transition)
    {
        return ;
    }
#endif

#if !MODEL_ENABLE_PUBLISH_ALL_TIME
    if (result.state_changed)
#endif
    {
        light_hsl_saturation_publish(pmodel_info, saturation);
    }
}

static uint16_t get_present_saturation(const mesh_model_info_p pmodel_info)
{
    light_hsl_server_get_saturation_t get_data = {0};
    if (NULL != pmodel_info->model_data_cb)
    {
        pmodel_info->model_data_cb(pmodel_info, LIGHT_HSL_SERVER_GET_SATURATION, &get_data);
    }

    return get_data.saturation;
}

static int32_t light_hsl_saturation_trans_step_change(const mesh_model_info_p pmodel_info,
                                                      uint32_t type,
                                                      generic_transition_time_t total_time,
                                                      generic_transition_time_t remaining_time)
{
    int32_t ret = 0;
    light_hsl_server_set_saturation_t trans_set_data;
    light_hsl_saturation_info_t *psaturation_info = pmodel_info->pargs;
    if (NULL == psaturation_info)
    {
        return 0;
    }
    trans_set_data.saturation = psaturation_info->target_saturation;
    trans_set_data.total_time = total_time;
    trans_set_data.remaining_time = remaining_time;

    if (NULL != pmodel_info->model_data_cb)
    {
        ret = pmodel_info->model_data_cb(pmodel_info, LIGHT_HSL_SERVER_SET_SATURATION,
                                         &trans_set_data);
    }

    if (0 == remaining_time.num_steps)
    {
        light_hsl_saturation_publish(pmodel_info, get_present_saturation(pmodel_info));
    }

    return ret;
}

static uint16_t light_hsl_saturation_process(const mesh_model_info_p pmodel_info,
                                             uint16_t target_saturation,
                                             generic_transition_time_t trans_time,
                                             light_hsl_saturation_process_result_t *presult)
{
    uint16_t saturation_before_set = 0;
    uint16_t saturation_after_set = 0;

    /* get saturation before set */
    saturation_before_set = get_present_saturation(pmodel_info);

    int32_t ret = MODEL_SUCCESS;
    light_hsl_server_set_saturation_t trans_set_data;
    trans_set_data.saturation = target_saturation;
    trans_set_data.total_time = trans_time;
    trans_set_data.remaining_time = trans_time;

    if (NULL != pmodel_info->model_data_cb)
    {
        pmodel_info->model_data_cb(pmodel_info, LIGHT_HSL_SERVER_SET_SATURATION,
                                   &trans_set_data);
    }

    if (GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE != trans_time.num_steps)
    {
        if ((ret >= 0) && (MODEL_STOP_TRANSITION != ret))
        {
#if MODEL_ENABLE_PUBLISH_ALL_TIME
            if (NULL != presult)
            {
                presult->use_transition = TRUE;
            }
#endif
            generic_transition_timer_start(pmodel_info, GENERIC_TRANSITION_TYPE_LIGHT_HSL_SATURATION,
                                           trans_time,
                                           light_hsl_saturation_trans_step_change);
        }
#if MODEL_ENABLE_USER_STOP_TRANSITION_NOTIFICATION
        else if (MODEL_STOP_TRANSITION == ret)
        {
            if (NULL != pmodel_info->model_data_cb)
            {
                ret = pmodel_info->model_data_cb(pmodel_info, LIGHT_HSL_SERVER_SET_SATURATION, &trans_set_data);
            }
        }
#endif
    }

    /* get saturation after set */
    saturation_after_set = get_present_saturation(pmodel_info);
    if (saturation_after_set != saturation_before_set)
    {
        if (NULL != presult)
        {
            presult->state_changed = TRUE;
        }
    }

    return saturation_after_set;
}

#if MODEL_ENABLE_DELAY_EXECUTION
static int32_t light_hsl_saturation_delay_execution(mesh_model_info_t *pmodel_info,
                                                    uint32_t delay_type)
{
    light_hsl_saturation_info_t *psaturation_info = pmodel_info->pargs;
    if (NULL == psaturation_info)
    {
        return 0;
    }
    psaturation_info->delay_time = 0;
    light_hsl_saturation_process_result_t result =
    {
        .state_changed = FALSE,
#if MODEL_ENABLE_PUBLISH_ALL_TIME
        .use_transition = FALSE
#endif
    };
    uint16_t present_saturation = light_hsl_saturation_process(pmodel_info,
                                                               psaturation_info->target_saturation,
                                                               psaturation_info->trans_time, &result);
    light_hsl_saturation_state_change_publish(pmodel_info, present_saturation, result);

    return 0;
}
#endif

static bool light_hsl_saturation_server_receive(mesh_msg_p pmesh_msg)
{
    bool ret = TRUE;
    uint8_t *pbuffer = pmesh_msg->pbuffer + pmesh_msg->msg_offset;
    mesh_model_info_p pmodel_info = pmesh_msg->pmodel_info;
    switch (pmesh_msg->access_opcode)
    {
    case MESH_MSG_LIGHT_HSL_SATURATION_GET:
        if (pmesh_msg->msg_len == sizeof(light_hsl_saturation_get_t))
        {
            light_hsl_saturation_info_t *psaturation_info = pmodel_info->pargs;
            generic_transition_time_t remaining_time;
#if MODEL_ENABLE_DELAY_EXECUTION
            if (psaturation_info->delay_time > 0)
            {
                remaining_time = psaturation_info->trans_time;
            }
            else
#endif
            {
                remaining_time = generic_transition_time_get(pmodel_info,
                                                             GENERIC_TRANSITION_TYPE_LIGHT_HSL_SATURATION);
            }


            light_hsl_saturation_stat(pmodel_info, pmesh_msg->src, pmesh_msg->app_key_index,
                                      get_present_saturation(pmodel_info),
                                      (GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE == remaining_time.num_steps) ? FALSE : TRUE,
                                      psaturation_info->target_saturation, remaining_time);
        }
        break;
    case MESH_MSG_LIGHT_HSL_SATURATION_SET:
    case MESH_MSG_LIGHT_HSL_SATURATION_SET_UNACK:
        {
            light_hsl_saturation_set_t *pmsg = (light_hsl_saturation_set_t *)pbuffer;
            generic_transition_time_t trans_time = {GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE, 0};
#if MODEL_ENABLE_DELAY_EXECUTION
            uint32_t delay_time = 0;
#endif
            if (pmesh_msg->msg_len == MEMBER_OFFSET(light_hsl_saturation_set_t, trans_time))
            {
                if (NULL != pmodel_info->model_data_cb)
                {
                    pmodel_info->model_data_cb(pmodel_info, LIGHT_HSL_SERVER_GET_DEFAULT_TRANSITION_TIME, &trans_time);
                }
            }
            else if (pmesh_msg->msg_len == sizeof(light_hsl_saturation_set_t))
            {
                trans_time = pmsg->trans_time;
#if MODEL_ENABLE_DELAY_EXECUTION
                delay_time = pmsg->delay * DELAY_EXECUTION_STEP_RESOLUTION;
#endif
            }
            if (IS_GENERIC_TRANSITION_STEPS_VALID(trans_time.num_steps))
            {
                light_hsl_server_get_range_t range = {0, 0, 0, 0};
                if (NULL != pmodel_info->model_data_cb)
                {
                    pmodel_info->model_data_cb(pmodel_info, LIGHT_HSL_SERVER_GET_RANGE, &range);
                }

                light_hsl_saturation_info_t *psaturation_info = pmodel_info->pargs;
                if ((0 != range.saturation_range_min) && (0 != range.saturation_range_max))
                {
                    psaturation_info->target_saturation = CLAMP(pmsg->saturation, range.saturation_range_min,
                                                                range.saturation_range_max);
                }
                else
                {
                    psaturation_info->target_saturation = pmsg->saturation;
                }
                psaturation_info->tid = pmsg->tid;
#if MODEL_ENABLE_DELAY_EXECUTION
                psaturation_info->trans_time = trans_time;
                psaturation_info->delay_time = delay_time;
#endif

                uint16_t present_saturation;
                light_hsl_saturation_process_result_t result =
                {
                    .state_changed = FALSE,
#if MODEL_ENABLE_PUBLISH_ALL_TIME
                    .use_transition = FALSE
#endif
                };
#if MODEL_ENABLE_DELAY_EXECUTION
                if (delay_time > 0)
                {
                    present_saturation = get_present_saturation(pmodel_info);
                    delay_execution_timer_start(pmodel_info, DELAY_EXECUTION_TYPE_LIGHT_HSL_SATURATION, delay_time,
                                                light_hsl_saturation_delay_execution);
                }
                else
#endif
                {
                    present_saturation = light_hsl_saturation_process(pmodel_info,
                                                                      psaturation_info->target_saturation, trans_time,
                                                                      &result);
                }

                if (pmesh_msg->access_opcode == MESH_MSG_LIGHT_HSL_SATURATION_SET)
                {
                    light_hsl_saturation_stat(pmodel_info, pmesh_msg->src, pmesh_msg->app_key_index,
                                              present_saturation,
                                              (GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE == trans_time.num_steps) ? FALSE : TRUE,
                                              psaturation_info->target_saturation, trans_time);
                }

                light_hsl_saturation_state_change_publish(pmodel_info, present_saturation, result);
            }
        }
        break;
    default:
        ret = FALSE;
        break;
    }
    return ret;
}

static int32_t light_saturation_server_publish(mesh_model_info_p pmodel_info, bool retrans)
{
    generic_transition_time_t trans_time = {0, 0};
    light_hsl_saturation_stat(pmodel_info, 0, 0, get_present_saturation(pmodel_info), FALSE, 0,
                              trans_time);
    return 0;
}

#if MESH_MODEL_ENABLE_DEINIT
static void light_hsl_saturation_server_deinit(mesh_model_info_t *pmodel_info)
{
    if (pmodel_info->model_receive == light_hsl_saturation_server_receive)
    {
#if MODEL_ENABLE_DELAY_EXECUTION
        /* stop delay execution */
        delay_execution_timer_stop(pmodel_info, DELAY_EXECUTION_TYPE_LIGHT_HSL_SATURATION);
#endif
        /* stop transition */
        generic_transition_timer_stop(pmodel_info, GENERIC_TRANSITION_TYPE_LIGHT_HSL_SATURATION);

        /* now we can remove */
        plt_free(pmodel_info->pargs, RAM_TYPE_DATA_ON);
        pmodel_info->pargs = NULL;
        pmodel_info->model_receive = NULL;
    }
}
#endif

bool light_hsl_saturation_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info)
{
    if (NULL == pmodel_info)
    {
        return FALSE;
    }

    pmodel_info->model_id = MESH_MODEL_LIGHT_HSL_SATURATION_SERVER;
    if (NULL == pmodel_info->model_receive)
    {
        light_hsl_saturation_info_t *psaturation_info = plt_malloc(sizeof(light_hsl_saturation_info_t),
                                                                   RAM_TYPE_DATA_ON);
        if (NULL == psaturation_info)
        {
            printe("light_hsl_saturation_server_reg: fail to allocate memory for the new model extension data!");
            return FALSE;
        }
        memset(psaturation_info, 0, sizeof(light_hsl_saturation_info_t));
        pmodel_info->pargs = psaturation_info;

        pmodel_info->model_receive = light_hsl_saturation_server_receive;
        if (NULL == pmodel_info->model_data_cb)
        {
            printw("light_hsl_saturation_server_reg: missing model data process callback!");
        }
#if MESH_MODEL_ENABLE_DEINIT
        pmodel_info->model_deinit = light_hsl_saturation_server_deinit;
#endif
    }

    if (NULL == pmodel_info->model_pub_cb)
    {
        pmodel_info->model_pub_cb = light_saturation_server_publish;
    }

    generic_transition_time_init();
#if MODEL_ENABLE_DELAY_EXECUTION
    delay_execution_init();
#endif
    return mesh_model_reg(element_index, pmodel_info);
}


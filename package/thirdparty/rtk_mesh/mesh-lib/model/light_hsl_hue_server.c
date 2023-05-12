/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     light_hsl_hue_server.c
* @brief    Source file for light hsl hue server model.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-8-1
* @version  v1.0
* *************************************************************************************
*/

/* Add Includes here */
#include "light_hsl.h"
#if MODEL_ENABLE_DELAY_EXECUTION
#include "delay_execution.h"
#endif

extern mesh_msg_send_cause_t light_hsl_hue_stat(mesh_model_info_p pmodel_info, uint16_t dst,
                                                uint16_t app_key_index, uint16_t present_hue, bool optional, uint16_t target_hue,
                                                generic_transition_time_t remaining_time);


typedef struct
{
    uint8_t tid;
    uint16_t target_hue;
#if MODEL_ENABLE_DELAY_EXECUTION
    generic_transition_time_t trans_time;
    uint32_t delay_time;
#endif
} light_hsl_hue_info_t;

typedef struct
{
    bool state_changed;
#if MODEL_ENABLE_PUBLISH_ALL_TIME
    bool use_transition;
#endif
} light_hsl_hue_process_result_t;


int16_t light_hue_to_generic_level(uint16_t hue)
{
    return hue - 32768;
}

uint16_t generic_level_to_light_hue(int16_t level)
{
    return level + 32768;
}

static mesh_msg_send_cause_t light_hsl_hue_server_send(mesh_model_info_p pmodel_info, uint16_t dst,
                                                       uint8_t *pmsg, uint16_t msg_len, uint16_t app_key_index)
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

mesh_msg_send_cause_t light_hsl_hue_stat(mesh_model_info_p pmodel_info, uint16_t dst,
                                         uint16_t app_key_index, uint16_t present_hue, bool optional, uint16_t target_hue,
                                         generic_transition_time_t remaining_time)
{
    light_hsl_hue_stat_t msg;
    uint32_t len;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_LIGHT_HSL_HUE_STAT);
    msg.present_hue = present_hue;
    if (optional)
    {
        len = sizeof(light_hsl_hue_stat_t);
        msg.target_hue = target_hue;
        msg.remaining_time = remaining_time;
    }
    else
    {
        len = MEMBER_OFFSET(light_hsl_hue_stat_t, target_hue);
    }
    return light_hsl_hue_server_send(pmodel_info, dst, (uint8_t *)&msg, len, app_key_index);
}

mesh_msg_send_cause_t light_hsl_hue_publish(const mesh_model_info_p pmodel_info, uint16_t hue)
{
    mesh_msg_send_cause_t ret = MESH_MSG_SEND_CAUSE_INVALID_DST;
    if (mesh_model_pub_check(pmodel_info))
    {
        generic_transition_time_t trans_time = {0, 0};
        ret = light_hsl_hue_stat(pmodel_info, 0, 0, hue, FALSE, 0, trans_time);
    }

    return ret;
}

static void light_hsl_hue_state_change_publish(const mesh_model_info_p pmodel_info, uint16_t hue,
                                               light_hsl_hue_process_result_t result)
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
        light_hsl_hue_publish(pmodel_info, hue);
    }
}

static uint16_t get_present_hue(const mesh_model_info_p pmodel_info)
{
    light_hsl_server_get_hue_t get_data = {0};
    if (NULL != pmodel_info->model_data_cb)
    {
        pmodel_info->model_data_cb(pmodel_info, LIGHT_HSL_SERVER_GET_HUE, &get_data);
    }

    return get_data.hue;
}

static int32_t light_hsl_hue_trans_step_change(const mesh_model_info_p pmodel_info,
                                               uint32_t type,
                                               generic_transition_time_t total_time,
                                               generic_transition_time_t remaining_time)
{
    int32_t ret = MODEL_SUCCESS;
    light_hsl_server_set_hue_t trans_set_data;
    light_hsl_hue_info_t *phue_info = pmodel_info->pargs;
    if (NULL == phue_info)
    {
        return 0;
    }
    trans_set_data.hue = phue_info->target_hue;
    trans_set_data.total_time = total_time;
    trans_set_data.remaining_time = remaining_time;

    if (NULL != pmodel_info->model_data_cb)
    {
        ret = pmodel_info->model_data_cb(pmodel_info, LIGHT_HSL_SERVER_SET_HUE, &trans_set_data);
    }

    if (0 == remaining_time.num_steps)
    {
        light_hsl_hue_publish(pmodel_info, get_present_hue(pmodel_info));
    }

    return ret;
}

static uint16_t light_hsl_hue_process(const mesh_model_info_p pmodel_info,
                                      uint16_t target_hue,
                                      generic_transition_time_t trans_time,
                                      light_hsl_hue_process_result_t *presult)
{
    uint16_t hue_before_set = {0};
    uint16_t hue_after_set = {0};

    /* get hue before set */
    hue_before_set = get_present_hue(pmodel_info);

    int32_t ret = MODEL_SUCCESS;
    light_hsl_server_set_hue_t trans_set_data;
    trans_set_data.hue = target_hue;
    trans_set_data.total_time = trans_time;
    trans_set_data.remaining_time = trans_time;

    if (NULL != pmodel_info->model_data_cb)
    {
        pmodel_info->model_data_cb(pmodel_info, LIGHT_HSL_SERVER_SET_HUE, &trans_set_data);
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
            generic_transition_timer_start(pmodel_info, GENERIC_TRANSITION_TYPE_LIGHT_HSL_HUE, trans_time,
                                           light_hsl_hue_trans_step_change);
        }
#if MODEL_ENABLE_USER_STOP_TRANSITION_NOTIFICATION
        else if (MODEL_STOP_TRANSITION == ret)
        {
            if (NULL != pmodel_info->model_data_cb)
            {
                ret = pmodel_info->model_data_cb(pmodel_info, LIGHT_HSL_SERVER_SET_HUE, &trans_set_data);
            }
        }
#endif
    }

    /* get hue after set */
    hue_after_set = get_present_hue(pmodel_info);
    if (hue_after_set != hue_before_set)
    {
        if (NULL != presult)
        {
            presult->state_changed = TRUE;
        }
    }

    return hue_after_set;
}

#if MODEL_ENABLE_DELAY_EXECUTION
static int32_t light_hsl_hue_delay_execution(mesh_model_info_t *pmodel_info, uint32_t delay_type)
{
    light_hsl_hue_info_t *phue_info = pmodel_info->pargs;
    if (NULL == phue_info)
    {
        return 0;
    }
    phue_info->delay_time = 0;
    light_hsl_hue_process_result_t result =
    {
        .state_changed = FALSE,
#if MODEL_ENABLE_PUBLISH_ALL_TIME
        .use_transition = FALSE
#endif
    };
    uint16_t present_hue = light_hsl_hue_process(pmodel_info, phue_info->target_hue,
                                                 phue_info->trans_time, &result);

    light_hsl_hue_state_change_publish(pmodel_info, present_hue, result);

    return 0;
}
#endif

static bool light_hsl_hue_server_receive(mesh_msg_p pmesh_msg)
{
    bool ret = TRUE;
    uint8_t *pbuffer = pmesh_msg->pbuffer + pmesh_msg->msg_offset;
    mesh_model_info_p pmodel_info = pmesh_msg->pmodel_info;
    switch (pmesh_msg->access_opcode)
    {
    case MESH_MSG_LIGHT_HSL_HUE_GET:
        if (pmesh_msg->msg_len == sizeof(light_hsl_hue_get_t))
        {
            light_hsl_hue_info_t *phue_info = pmodel_info->pargs;
            generic_transition_time_t remaining_time;
#if MODEL_ENABLE_DELAY_EXECUTION
            if (phue_info->delay_time > 0)
            {
                remaining_time = phue_info->trans_time;
            }
            else
#endif
            {
                remaining_time = generic_transition_time_get(pmodel_info,
                                                             GENERIC_TRANSITION_TYPE_LIGHT_HSL_HUE);
            }

            light_hsl_hue_stat(pmodel_info, pmesh_msg->src, pmesh_msg->app_key_index,
                               get_present_hue(pmodel_info),
                               (GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE == remaining_time.num_steps) ? FALSE : TRUE,
                               phue_info->target_hue, remaining_time);
        }
        break;
    case MESH_MSG_LIGHT_HSL_HUE_SET:
    case MESH_MSG_LIGHT_HSL_HUE_SET_UNACK:
        {
            light_hsl_hue_set_t *pmsg = (light_hsl_hue_set_t *)pbuffer;
            generic_transition_time_t trans_time = {GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE, 0};
#if MODEL_ENABLE_DELAY_EXECUTION
            uint32_t delay_time = 0;
#endif
            if (pmesh_msg->msg_len == MEMBER_OFFSET(light_hsl_hue_set_t, trans_time))
            {
                if (NULL != pmodel_info->model_data_cb)
                {
                    pmodel_info->model_data_cb(pmodel_info, LIGHT_HSL_SERVER_GET_DEFAULT_TRANSITION_TIME, &trans_time);
                }
            }
            else if (pmesh_msg->msg_len == sizeof(light_hsl_hue_set_t))
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

                light_hsl_hue_info_t *phue_info = pmodel_info->pargs;
                if ((0 != range.hue_range_min) && (0 != range.hue_range_max))
                {
                    phue_info->target_hue = CLAMP(pmsg->hue, range.hue_range_min, range.hue_range_max);
                }
                else
                {
                    phue_info->target_hue = pmsg->hue;
                }
                phue_info->tid = pmsg->tid;
#if MODEL_ENABLE_DELAY_EXECUTION
                phue_info->trans_time = trans_time;
                phue_info->delay_time = delay_time;
#endif

                uint16_t present_hue;
                light_hsl_hue_process_result_t result =
                {
                    .state_changed = FALSE,
#if MODEL_ENABLE_PUBLISH_ALL_TIME
                    .use_transition = FALSE
#endif
                };
#if MODEL_ENABLE_DELAY_EXECUTION
                if (delay_time > 0)
                {
                    present_hue = get_present_hue(pmodel_info);
                    delay_execution_timer_start(pmodel_info, DELAY_EXECUTION_TYPE_LIGHT_HSL_HUE, delay_time,
                                                light_hsl_hue_delay_execution);
                }
                else
#endif
                {
                    present_hue = light_hsl_hue_process(pmodel_info, phue_info->target_hue, trans_time, &result);
                }

                if (pmesh_msg->access_opcode == MESH_MSG_LIGHT_HSL_HUE_SET)
                {
                    light_hsl_hue_stat(pmesh_msg->pmodel_info, pmesh_msg->src, pmesh_msg->app_key_index,
                                       present_hue,
                                       (GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE == trans_time.num_steps) ? FALSE : TRUE,
                                       phue_info->target_hue, trans_time);
                }

                light_hsl_hue_state_change_publish(pmodel_info, present_hue, result);
            }
        }
        break;
    default:
        ret = FALSE;
        break;
    }
    return ret;
}

static int32_t light_hue_server_publish(mesh_model_info_p pmodel_info, bool retrans)
{
    generic_transition_time_t trans_time = {0, 0};
    light_hsl_hue_stat(pmodel_info, 0, 0, get_present_hue(pmodel_info), FALSE, 0, trans_time);
    return 0;
}

#if MESH_MODEL_ENABLE_DEINIT
static void light_hsl_hue_server_deinit(mesh_model_info_t *pmodel_info)
{
    if (pmodel_info->model_receive == light_hsl_hue_server_receive)
    {
#if MODEL_ENABLE_DELAY_EXECUTION
        /* stop delay execution */
        delay_execution_timer_stop(pmodel_info, DELAY_EXECUTION_TYPE_LIGHT_HSL_HUE);
#endif
        /* stop transition */
        generic_transition_timer_stop(pmodel_info, GENERIC_TRANSITION_TYPE_LIGHT_HSL_HUE);

        /* now we can remove */
        plt_free(pmodel_info->pargs, RAM_TYPE_DATA_ON);
        pmodel_info->pargs = NULL;
        pmodel_info->model_receive = NULL;
    }
}
#endif

bool light_hsl_hue_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info)
{
    if (NULL == pmodel_info)
    {
        return FALSE;
    }

    pmodel_info->model_id = MESH_MODEL_LIGHT_HSL_HUE_SERVER;
    if (NULL == pmodel_info->model_receive)
    {
        light_hsl_hue_info_t *phue_info = plt_malloc(sizeof(light_hsl_hue_info_t),
                                                     RAM_TYPE_DATA_ON);
        if (NULL == phue_info)
        {
            printe("light_hsl_hue_server_reg: fail to allocate memory for the new model extension data!");
            return FALSE;
        }
        memset(phue_info, 0, sizeof(light_hsl_hue_info_t));
        pmodel_info->pargs = phue_info;

        pmodel_info->model_receive = light_hsl_hue_server_receive;
        if (NULL == pmodel_info->model_data_cb)
        {
            printw("light_hsl_hue_server_reg: missing model data process callback!");
        }
#if MESH_MODEL_ENABLE_DEINIT
        pmodel_info->model_deinit = light_hsl_hue_server_deinit;
#endif
    }

    if (NULL == pmodel_info->model_pub_cb)
    {
        pmodel_info->model_pub_cb = light_hue_server_publish;
    }

    generic_transition_time_init();
#if MODEL_ENABLE_DELAY_EXECUTION
    delay_execution_init();
#endif
    return mesh_model_reg(element_index, pmodel_info);
}


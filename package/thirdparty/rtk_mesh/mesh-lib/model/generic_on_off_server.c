/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     generic_on_off_server.c
* @brief    Source file for generic on off server model.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-7-11
* @version  v1.0
* *************************************************************************************
*/

#include "generic_on_off.h"
#if MODEL_ENABLE_DELAY_EXECUTION
#include "delay_execution.h"
#endif

typedef struct
{
    uint8_t tid;
    generic_on_off_t target_on_off;
#if MODEL_ENABLE_DELAY_EXECUTION
    generic_transition_time_t trans_time;
    uint32_t delay_time;
#endif
} generic_on_off_info_t;

typedef struct
{
    bool state_changed;
#if MODEL_ENABLE_PUBLISH_ALL_TIME
    bool use_transition;
#endif
} generic_on_off_process_result_t;


static mesh_msg_send_cause_t generic_on_off_stat(mesh_model_info_p pmodel_info, uint16_t dst,
                                                 uint16_t app_key_index, generic_on_off_t present_on_off, bool optional,
                                                 generic_on_off_t target_on_off, generic_transition_time_t remaining_time)
{
    generic_on_off_stat_t msg;
    ACCESS_OPCODE_BYTE(msg.opcode, MESH_MSG_GENERIC_ON_OFF_STAT);
    uint16_t msg_len;
    if (optional)
    {
        msg_len = sizeof(generic_on_off_stat_t);
        msg.target_on_off = target_on_off;
        msg.remaining_time = remaining_time;
    }
    else
    {
        msg_len = MEMBER_OFFSET(generic_on_off_stat_t, target_on_off);
    }
    msg.present_on_off = present_on_off;

    mesh_msg_t mesh_msg;
    mesh_msg.pmodel_info = pmodel_info;
    access_cfg(&mesh_msg);
    mesh_msg.pbuffer = (uint8_t *)&msg;
    mesh_msg.msg_len = msg_len;
    if (0 != dst)
    {
        mesh_msg.dst = dst;
        mesh_msg.app_key_index = app_key_index;
    }
    return access_send(&mesh_msg);
}

mesh_msg_send_cause_t generic_on_off_publish(const mesh_model_info_p pmodel_info,
                                             generic_on_off_t on_off)
{
    mesh_msg_send_cause_t ret = MESH_MSG_SEND_CAUSE_INVALID_DST;
    if (mesh_model_pub_check(pmodel_info))
    {
        generic_transition_time_t trans_time = {0, 0};
        ret = generic_on_off_stat(pmodel_info, 0, 0, on_off, FALSE, on_off, trans_time);
    }

    return ret;
}

static void generic_on_off_state_change_publish(const mesh_model_info_p pmodel_info,
                                                generic_on_off_t on_off, generic_on_off_process_result_t result)
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
        generic_on_off_publish(pmodel_info, on_off);
    }
}

static generic_on_off_t get_present_on_off(const mesh_model_info_p pmodel_info)
{
    generic_on_off_server_get_t get_data = {GENERIC_OFF};
    if (NULL != pmodel_info->model_data_cb)
    {
        pmodel_info->model_data_cb(pmodel_info, GENERIC_ON_OFF_SERVER_GET, &get_data);
    }

    return get_data.on_off;
}

static int32_t generic_on_off_trans_step_change(const mesh_model_info_p pmodel_info, uint32_t type,
                                                generic_transition_time_t total_time,
                                                generic_transition_time_t remaining_time)
{
    int32_t ret = MODEL_SUCCESS;
    generic_on_off_server_set_t set_data;
    generic_on_off_info_t *ptarget = pmodel_info->pargs;
    if (NULL == ptarget)
    {
        return 0;
    }
    set_data.on_off = ptarget->target_on_off;
    set_data.total_time = total_time;
    set_data.remaining_time = remaining_time;
    if (NULL != pmodel_info->model_data_cb)
    {
        ret = pmodel_info->model_data_cb(pmodel_info, GENERIC_ON_OFF_SERVER_SET, &set_data);
    }

    if (0 == remaining_time.num_steps)
    {
        generic_on_off_t present_on_off = get_present_on_off(pmodel_info);
        generic_on_off_publish(pmodel_info, present_on_off);
    }

    return ret;
}

static generic_on_off_t generic_on_off_process(const mesh_model_info_p pmodel_info,
                                               generic_on_off_t target_on_off,
                                               generic_transition_time_t trans_time,
                                               generic_on_off_process_result_t *presult)
{
    generic_on_off_t on_off_before_set;
    generic_on_off_t on_off_after_set;

    /* get generic on/off before set */
    on_off_before_set = get_present_on_off(pmodel_info);

    int32_t ret = MODEL_SUCCESS;
    generic_on_off_server_set_t trans_set_data;
    trans_set_data.total_time = trans_time;
    trans_set_data.remaining_time = trans_time;
    trans_set_data.on_off = target_on_off;

    if (NULL != pmodel_info->model_data_cb)
    {
        ret = pmodel_info->model_data_cb(pmodel_info, GENERIC_ON_OFF_SERVER_SET, &trans_set_data);
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
            generic_transition_timer_start(pmodel_info, GENERIC_TRANSITION_TYPE_ON_OFF, trans_time,
                                           generic_on_off_trans_step_change);
        }
#if MODEL_ENABLE_USER_STOP_TRANSITION_NOTIFICATION
        else if (MODEL_STOP_TRANSITION == ret)
        {
            if (NULL != pmodel_info->model_data_cb)
            {
                ret = pmodel_info->model_data_cb(pmodel_info, GENERIC_ON_OFF_SERVER_SET, &trans_set_data);
            }
        }
#endif
    }

    /* get on/off after set */
    on_off_after_set = get_present_on_off(pmodel_info);

    if (on_off_before_set != on_off_after_set)
    {
        if (NULL != presult)
        {
            presult->state_changed = TRUE;
        }
    }

    return on_off_after_set;
}

#if MODEL_ENABLE_DELAY_EXECUTION
static int32_t generic_on_off_delay_execution(mesh_model_info_t *pmodel_info, uint32_t delay_type)
{
    generic_on_off_info_t *ponoff_info = pmodel_info->pargs;
    if (NULL == ponoff_info)
    {
        return 0;
    }
    ponoff_info->delay_time = 0;
    generic_on_off_process_result_t result =
    {
        .state_changed = FALSE,
#if MODEL_ENABLE_PUBLISH_ALL_TIME
        .use_transition = FALSE
#endif
    };
    generic_on_off_t present_on_off = generic_on_off_process(pmodel_info, ponoff_info->target_on_off,
                                                             ponoff_info->trans_time, &result);
    generic_on_off_state_change_publish(pmodel_info, present_on_off, result);

    return 0;
}
#endif

/**
 * @brief default generic on/off server receive function
 * @param[in] pmesh_msg: received mesh message
 * @return process result
 */
static bool generic_on_off_server_receive(mesh_msg_p pmesh_msg)
{
    bool ret = TRUE;
    uint8_t *pbuffer = pmesh_msg->pbuffer + pmesh_msg->msg_offset;
    mesh_model_info_p pmodel_info = pmesh_msg->pmodel_info;

    switch (pmesh_msg->access_opcode)
    {
    case MESH_MSG_GENERIC_ON_OFF_GET:
        if (pmesh_msg->msg_len == sizeof(generic_on_off_get_t))
        {
            /* get present on/off status */
            generic_on_off_t present_on_off = get_present_on_off(pmodel_info);
            /* get target on/off status*/
            generic_on_off_info_t *ponoff_info = pmodel_info->pargs;
            /* get remaining time */
            generic_transition_time_t remaining_time;
#if MODEL_ENABLE_DELAY_EXECUTION
            if (ponoff_info->delay_time > 0)
            {
                remaining_time = ponoff_info->trans_time;
            }
            else
#endif
            {
                remaining_time = generic_transition_time_get(pmesh_msg->pmodel_info,
                                                             GENERIC_TRANSITION_TYPE_ON_OFF);
            }

            generic_on_off_stat(pmodel_info, pmesh_msg->src, pmesh_msg->app_key_index,
                                present_on_off,
                                (GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE == remaining_time.num_steps) ? FALSE : TRUE,
                                ponoff_info->target_on_off, remaining_time);
        }
        break;
    case MESH_MSG_GENERIC_ON_OFF_SET:
    case MESH_MSG_GENERIC_ON_OFF_SET_UNACK:
        {
            generic_on_off_set_t *pmsg = (generic_on_off_set_t *)pbuffer;
            generic_transition_time_t trans_time = {GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE, 0};
#if MODEL_ENABLE_DELAY_EXECUTION
            uint32_t delay_time = 0;
#endif
            if (pmesh_msg->msg_len == MEMBER_OFFSET(generic_on_off_set_t, trans_time))
            {
                if (NULL != pmodel_info->model_data_cb)
                {
                    pmodel_info->model_data_cb(pmodel_info, GENERIC_ON_OFF_SERVER_GET_DEFAULT_TRANSITION_TIME,
                                               &trans_time);
                }
            }
            else if (pmesh_msg->msg_len == sizeof(generic_on_off_set_t))
            {
                trans_time = pmsg->trans_time;
#if MODEL_ENABLE_DELAY_EXECUTION
                delay_time = pmsg->delay * DELAY_EXECUTION_STEP_RESOLUTION;
#endif
            }
            if (IS_GENERIC_ON_OFF_VALID(pmsg->on_off) &&
                IS_GENERIC_TRANSITION_STEPS_VALID(trans_time.num_steps))
            {
                generic_on_off_info_t *ponoff_info = pmodel_info->pargs;
                ponoff_info->tid = pmsg->tid;
                ponoff_info->target_on_off = pmsg->on_off;
#if MODEL_ENABLE_DELAY_EXECUTION
                ponoff_info->trans_time = trans_time;
                ponoff_info->delay_time = delay_time;
#endif

                generic_on_off_t present_on_off;
                generic_on_off_process_result_t result =
                {
                    .state_changed = FALSE,
#if MODEL_ENABLE_PUBLISH_ALL_TIME
                    .use_transition = FALSE
#endif
                };
#if MODEL_ENABLE_DELAY_EXECUTION
                if (delay_time > 0)
                {
                    present_on_off = get_present_on_off(pmodel_info);
                    delay_execution_timer_start(pmodel_info, DELAY_EXECUTION_TYPE_ON_OFF, delay_time,
                                                generic_on_off_delay_execution);
                }
                else
#endif
                {
                    present_on_off = generic_on_off_process(pmodel_info, ponoff_info->target_on_off, trans_time,
                                                            &result);
                }

                if (pmesh_msg->access_opcode == MESH_MSG_GENERIC_ON_OFF_SET)
                {
                    generic_on_off_stat(pmodel_info, pmesh_msg->src, pmesh_msg->app_key_index,
                                        present_on_off,
                                        (GENERIC_TRANSITION_NUM_STEPS_IMMEDIATE == trans_time.num_steps) ? FALSE : TRUE,
                                        ponoff_info->target_on_off, trans_time);
                }

                generic_on_off_state_change_publish(pmodel_info, present_on_off, result);
            }
        }
        break;
    default:
        ret = FALSE;
        break;
    }
    return ret;
}

static int32_t generic_on_off_server_publish(mesh_model_info_p pmodel_info, bool retrans)
{
    generic_transition_time_t trans_time = {0, 0};
    generic_on_off_stat(pmodel_info, 0, 0, get_present_on_off(pmodel_info), FALSE, GENERIC_OFF,
                        trans_time);
    return 0;
}

#if MESH_MODEL_ENABLE_DEINIT
static void generic_on_off_server_deinit(mesh_model_info_t *pmodel_info)
{
    if (pmodel_info->model_receive == generic_on_off_server_receive)
    {
#if MODEL_ENABLE_DELAY_EXECUTION
        /* stop delay execution */
        delay_execution_timer_stop(pmodel_info, DELAY_EXECUTION_TYPE_ON_OFF);
#endif
        /* stop transition */
        generic_transition_timer_stop(pmodel_info, GENERIC_TRANSITION_TYPE_ON_OFF);

        /* now we can remove */
        plt_free(pmodel_info->pargs, RAM_TYPE_DATA_ON);
        pmodel_info->pargs = NULL;
        pmodel_info->model_receive = NULL;
    }
}
#endif

bool generic_on_off_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info)
{
    if (NULL == pmodel_info)
    {
        return FALSE;
    }

    pmodel_info->model_id = MESH_MODEL_GENERIC_ON_OFF_SERVER;
    if (NULL == pmodel_info->model_receive)
    {
        pmodel_info->pargs = plt_malloc(sizeof(generic_on_off_info_t), RAM_TYPE_DATA_ON);
        if (NULL == pmodel_info->pargs)
        {
            printe("generic_on_off_server_reg: fail to allocate memory for the new model extension data!");
            return FALSE;
        }
        memset(pmodel_info->pargs, 0, sizeof(generic_on_off_info_t));

        pmodel_info->model_receive = generic_on_off_server_receive;
        if (NULL == pmodel_info->model_data_cb)
        {
            printw("generic_on_off_server_reg: missing model data process callback!");
        }
#if MESH_MODEL_ENABLE_DEINIT
        pmodel_info->model_deinit = generic_on_off_server_deinit;
#endif
    }

    if (NULL == pmodel_info->model_pub_cb)
    {
        pmodel_info->model_pub_cb = generic_on_off_server_publish;
    }

    generic_transition_time_init();
#if MODEL_ENABLE_DELAY_EXECUTION
    delay_execution_init();
#endif
    return mesh_model_reg(element_index, pmodel_info);
}


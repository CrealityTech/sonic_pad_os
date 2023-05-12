/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      light_controller_app.c
* @brief     light controller
* @details
* @author    hector huang
* @date      2018-09-18
* @version   v1.0
* *********************************************************************************************************
*/

#include "light_controller_app.h"
#include "platform_diagnose.h"
#include "platform_os.h"

#define MAX_ACTION_NUM              5
/* timer interval, minimum value is 10ms */
#define LIGHT_MONITOR_INTERVAL      20
static plt_timer_t light_ctl_timer = NULL;

typedef struct
{
    uint16_t current_lightness;
    uint16_t target_lightness;
    uint8_t steps;
    int32_t step_delta;
} light_lightness_action_t;

typedef struct
{
    uint16_t lightness_begin;
    uint16_t lightness_end;
    bool in_begin_phase;
    uint32_t begin_count;
    uint32_t begin_cur_count;
    uint32_t end_count;
    uint32_t end_cur_count;
    uint32_t times;
} light_blink_action_t;

typedef struct
{
    uint16_t lightness_begin;
    uint16_t lightness_end;
    uint16_t current_lightness;
    bool in_forward_phase;
    uint8_t current_forward_steps;
    uint8_t forward_steps;
    int32_t forward_step_delta;
    uint8_t current_reverse_steps;
    uint8_t reverse_steps;
    int32_t reverse_step_delta;
    uint32_t times;
    bool half_breath_end;
} light_breath_action_t;

typedef enum
{
    LIGHT_ACTION_LIGHTNESS_LINEAR,
    LIGHT_ACTION_BLINK,
    LIGHT_ACTION_BREATH,
    LIGHT_ACTION_UNKNOWN,
} light_action_type_t;

typedef union
{
    light_lightness_action_t lightness;
    light_blink_action_t blink;
    light_breath_action_t breath;
} light_action_value_t;

typedef struct
{
    bool busy;
    bool need_change;
    light_t *light;
    light_change_done_cb change_done;
    light_action_type_t type;
    light_action_value_t value;
} light_action_t;

static light_action_t light_actions[MAX_ACTION_NUM];

static light_action_t *request_action(light_t *light)
{
    /* find same first */
    for (uint8_t i = 0; i < MAX_ACTION_NUM; ++i)
    {
        if ((light == light_actions[i].light) && (light_actions[i].busy))
        {
            return &light_actions[i];
        }
    }

    /* no same, find empty */
    for (uint8_t i = 0; i < MAX_ACTION_NUM; ++i)
    {
        if (!light_actions[i].busy)
        {
            light_actions[i].busy = TRUE;
            return &light_actions[i];
        }
    }

    return NULL;
}

static void release_action(light_action_t *action)
{
    action->busy = FALSE;
    if (NULL != action->change_done)
    {
        action->change_done(action->light);
    }
}

static bool is_all_light_idle(void)
{
    for (uint8_t channel = 0; channel < MAX_ACTION_NUM; ++channel)
    {
        if (light_actions[channel].busy && light_actions[channel].need_change)
        {
            return FALSE;
        }
    }
    return TRUE;
}

static void light_controller_timer_start(void)
{
    if (NULL != light_ctl_timer)
    {
        if (!plt_timer_is_active(light_ctl_timer))
        {
            plt_timer_start(light_ctl_timer, 0);
        }
    }
}

static void light_ctl_timeout_handle(void *pargs)
{
    UNUSED(pargs);

    for (uint8_t channel = 0; channel < MAX_ACTION_NUM; ++channel)
    {
        if (light_actions[channel].busy && light_actions[channel].need_change)
        {
            switch (light_actions[channel].type)
            {
            case LIGHT_ACTION_LIGHTNESS_LINEAR:
                if (light_actions[channel].value.lightness.steps > 1)
                {
                    light_lighten(light_actions[channel].light,
                                  light_actions[channel].value.lightness.current_lightness);
                    light_actions[channel].value.lightness.current_lightness +=
                        light_actions[channel].value.lightness.step_delta;
                    light_actions[channel].value.lightness.steps --;
                }
                else
                {
                    light_lighten(light_actions[channel].light,
                                  light_actions[channel].value.lightness.target_lightness);
                    if (light_actions[channel].value.lightness.target_lightness > 0)
                    {
                        light_actions[channel].light->lightness_last =
                            light_actions[channel].value.lightness.target_lightness;
                    }

                    light_actions[channel].need_change = FALSE;
                    release_action(&light_actions[channel]);
                }
                break;
            case LIGHT_ACTION_BLINK:
                if (light_actions[channel].value.blink.times > 0)
                {
                    if (light_actions[channel].value.blink.in_begin_phase)
                    {
                        if (light_actions[channel].value.blink.begin_cur_count >=
                            light_actions[channel].value.blink.begin_count)
                        {
                            light_actions[channel].value.blink.in_begin_phase = FALSE;
                            light_actions[channel].value.blink.begin_cur_count = 0;
                        }
                        else
                        {
                            if (0 == light_actions[channel].value.blink.begin_cur_count)
                            {
                                light_lighten(light_actions[channel].light,
                                              light_actions[channel].value.blink.lightness_begin);
                            }
                            light_actions[channel].value.blink.begin_cur_count ++;
                        }
                    }
                    else
                    {
                        if (light_actions[channel].value.blink.end_cur_count >=
                            light_actions[channel].value.blink.end_count)
                        {
                            light_actions[channel].value.blink.in_begin_phase = TRUE;
                            light_actions[channel].value.blink.end_cur_count = 0;

                            if (TIME_INFINITE != light_actions[channel].value.blink.times)
                            {
                                light_actions[channel].value.blink.times --;
                            }
                        }
                        else
                        {
                            if (0 == light_actions[channel].value.blink.end_cur_count)
                            {
                                light_lighten(light_actions[channel].light, light_actions[channel].value.blink.lightness_end);
                            }
                            light_actions[channel].value.blink.end_cur_count ++;
                        }
                    }
                }
                else
                {
                    light_actions[channel].need_change = FALSE;
                    release_action(&light_actions[channel]);
                }
                break;
            case LIGHT_ACTION_BREATH:
                if (light_actions[channel].value.breath.times > 0)
                {
                    if (light_actions[channel].value.breath.in_forward_phase)
                    {
                        if (light_actions[channel].value.breath.current_forward_steps >=
                            light_actions[channel].value.breath.forward_steps)
                        {
                            light_lighten(light_actions[channel].light,
                                          light_actions[channel].value.breath.lightness_end);
                            light_actions[channel].value.breath.current_lightness =
                                light_actions[channel].value.breath.lightness_end;
                            light_actions[channel].value.breath.current_reverse_steps = 0;
                            light_actions[channel].value.breath.in_forward_phase = FALSE;
                            if (1 == light_actions[channel].value.breath.times)
                            {
                                if (light_actions[channel].value.breath.half_breath_end)
                                {
                                    light_actions[channel].value.breath.times = 0;
                                    light_actions[channel].need_change = FALSE;
                                    release_action(&light_actions[channel]);
                                }
                            }
                        }
                        else
                        {
                            light_lighten(light_actions[channel].light,
                                          light_actions[channel].value.breath.current_lightness);
                            light_actions[channel].value.breath.current_lightness +=
                                light_actions[channel].value.breath.forward_step_delta;
                            light_actions[channel].value.breath.current_forward_steps ++;
                        }
                    }
                    else
                    {
                        if (light_actions[channel].value.breath.current_reverse_steps >=
                            light_actions[channel].value.breath.reverse_steps)
                        {
                            light_lighten(light_actions[channel].light,
                                          light_actions[channel].value.breath.lightness_begin);
                            light_actions[channel].value.breath.current_lightness =
                                light_actions[channel].value.breath.lightness_begin;
                            light_actions[channel].value.breath.current_forward_steps = 0;
                            light_actions[channel].value.breath.in_forward_phase = TRUE;

                            if (TIME_INFINITE != light_actions[channel].value.breath.times)
                            {
                                light_actions[channel].value.breath.times --;
                            }
                        }
                        else
                        {
                            light_lighten(light_actions[channel].light,
                                          light_actions[channel].value.breath.current_lightness);
                            light_actions[channel].value.breath.current_lightness +=
                                light_actions[channel].value.breath.reverse_step_delta;
                            light_actions[channel].value.breath.current_reverse_steps ++;
                        }
                    }
                }
                else
                {
                    light_actions[channel].need_change = FALSE;
                    release_action(&light_actions[channel]);
                }
                break;
            default:
                break;
            }
        }
    }

    /* Check light controler status */
    if (is_all_light_idle())
    {
        plt_timer_stop(light_ctl_timer, 0);
    }
}

void light_stop(light_t *light)
{
    for (uint8_t i = 0; i < MAX_ACTION_NUM; ++i)
    {
        if (light_actions[i].light == light)
        {
            light_actions[i].type = LIGHT_ACTION_UNKNOWN;
            light_actions[i].need_change = FALSE;
            light_actions[i].busy = FALSE;
            if (NULL != light_actions[i].change_done)
            {
                light_actions[i].change_done(light_actions[i].light);
            }
            break;
        }
    }
}

void light_set_lightness_linear(light_t *light, uint16_t lightness, uint32_t time,
                                light_change_done_cb pcb)
{
    light_action_t *paction = request_action(light);

    if (NULL != paction)
    {
        paction->light = light;
        paction->type = LIGHT_ACTION_LIGHTNESS_LINEAR;
        paction->change_done = pcb;
        paction->value.lightness.current_lightness = light->lightness;
        paction->value.lightness.target_lightness = lightness;
        paction->value.lightness.steps = time / LIGHT_MONITOR_INTERVAL;
        if (0 == paction->value.lightness.steps)
        {
            paction->value.lightness.steps = 1;
        }
        paction->value.lightness.step_delta = (int32_t)(
                                                  paction->value.lightness.target_lightness -
                                                  paction->value.lightness.current_lightness) / paction->value.lightness.steps;
        paction->need_change = TRUE;

        light_controller_timer_start();
    }
}

void light_blink(light_t *light, uint16_t lightness_begin, uint16_t lightness_end,
                 uint32_t interval, uint8_t begin_duty, uint32_t times, light_change_done_cb pcb)
{
    if (begin_duty > 100)
    {
        begin_duty = 100;
    }

    light_action_t *paction = request_action(light);
    if (NULL != paction)
    {
        uint32_t total_count = interval / LIGHT_MONITOR_INTERVAL + 1;
        paction->light = light;
        paction->type = LIGHT_ACTION_BLINK;
        paction->change_done = pcb;
        paction->value.blink.lightness_begin = lightness_begin;
        paction->value.blink.lightness_end = lightness_end;
        paction->value.blink.in_begin_phase = TRUE;
        paction->value.blink.begin_count = total_count * begin_duty / 100;
        paction->value.blink.begin_cur_count = 0;
        paction->value.blink.end_count = total_count - paction->value.blink.begin_count;
        paction->value.blink.end_cur_count = 0;
        paction->value.blink.times = times;
        paction->need_change = TRUE;

        light_controller_timer_start();
    }
}

void light_breath(light_t *light, uint16_t lightness_begin, uint16_t lightness_end,
                  uint32_t interval, uint8_t forward_duty, uint32_t times, bool half_breath_end,
                  light_change_done_cb pcb)
{
    if (forward_duty > 100)
    {
        forward_duty = 100;
    }

    light_action_t *paction = request_action(light);
    if (NULL != paction)
    {
        uint32_t total_steps = interval / LIGHT_MONITOR_INTERVAL;
        if (0 == total_steps)
        {
            total_steps = 1;
        }
        paction->light = light;
        paction->type = LIGHT_ACTION_BREATH;
        paction->change_done = pcb;
        paction->value.breath.lightness_begin = lightness_begin;
        paction->value.breath.lightness_end = lightness_end;
        paction->value.breath.current_lightness = lightness_begin;
        paction->value.breath.in_forward_phase = TRUE;
        paction->value.breath.current_forward_steps = 0;
        paction->value.breath.forward_steps = total_steps * forward_duty / 100;
        paction->value.breath.forward_step_delta = (int32_t)(lightness_end - lightness_begin) /
                                                   paction->value.breath.forward_steps;
        paction->value.breath.current_reverse_steps = 0;
        paction->value.breath.reverse_steps = total_steps -
                                              paction->value.breath.forward_steps;
        paction->value.breath.reverse_step_delta = (int32_t)(lightness_begin - lightness_end) /
                                                   paction->value.breath.reverse_steps;
        paction->value.breath.times = times;
        paction->value.breath.half_breath_end = half_breath_end;
        paction->need_change = TRUE;

        light_controller_timer_start();
    }
}

bool is_light_controller_busy(void)
{
    bool ret = FALSE;

    for (uint8_t i = 0; i < MAX_ACTION_NUM; ++i)
    {
        if (light_actions[i].busy)
        {
            ret = TRUE;
            break;
        }
    }

    return ret;
}

bool is_light_busy(const light_t *light)
{
    for (uint8_t i = 0; i < MAX_ACTION_NUM; ++i)
    {
        if (light == light_actions[i].light)
        {
            return light_actions[i].busy;
        }
    }

    return FALSE;;
}

bool light_controller_init(void)
{
    if (NULL != light_ctl_timer)
    {
        return TRUE;
    }
    else
    {
        light_ctl_timer = plt_timer_create("lightctl", LIGHT_MONITOR_INTERVAL, TRUE, 0,
                                           light_ctl_timeout_handle);
        if (NULL == light_ctl_timer)
        {
            printe("light_control_init: initialize light controller failed, can not allocate memory!");
            return FALSE;
        }

        plt_timer_start(light_ctl_timer, 0);
    }

    return TRUE;
}

void light_controller_deinit(void)
{
    if (NULL != light_ctl_timer)
    {
        plt_timer_delete(light_ctl_timer, 0);
        light_ctl_timer = NULL;
    }

    for (uint8_t i = 0; i < MAX_ACTION_NUM; ++i)
    {
        if (light_actions[i].busy)
        {
            light_actions[i].busy = FALSE;
            light_actions[i].type = LIGHT_ACTION_UNKNOWN;
            light_actions[i].need_change = FALSE;
            if (NULL != light_actions[i].change_done)
            {
                light_actions[i].change_done(light_actions[i].light);
            }
        }
    }
}


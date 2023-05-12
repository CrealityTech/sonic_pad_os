/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      light_lightness_server_app.c
* @brief     Smart mesh demo light lightness application
* @details
* @author    hector huang
* @date      2018-8-20
* @version   v1.0
* *********************************************************************************************************
*/

#include "mesh_api.h"
#include "light_lightness_server_app.h"
#include "generic_on_off.h"
#include "light_lightness.h"
#include "light_cwrgb_app.h"
#include "light_controller_app.h"
#include "light_config.h"
#include "light_storage_app.h"

#define USE_SIG_TRANSITION     0
#if USE_SIG_TRANSITION
static float delta_lightness = 0;
#endif

/* lightness light models */
static mesh_model_info_t generic_on_off_server;
static mesh_model_info_t light_lightness_server;
static mesh_model_info_t light_lightness_setup_server;

static void light_lightness_change_done(light_t *light)
{
    light_state_store();
}

static int32_t generic_on_off_server_data(const mesh_model_info_p pmodel_info, uint32_t type,
                                          void *pargs)
{
    int32_t ret = MODEL_SUCCESS;
    generic_on_off_t current_on_off = light_get_cold()->lightness ? GENERIC_ON : GENERIC_OFF;

    switch (type)
    {
    case GENERIC_ON_OFF_SERVER_GET:
        {
            generic_on_off_server_get_t *pdata = pargs;
            pdata->on_off = current_on_off;
        }
        break;
    case GENERIC_ON_OFF_SERVER_GET_DEFAULT_TRANSITION_TIME:
        {
        }
        break;
    case GENERIC_ON_OFF_SERVER_SET:
        {
            generic_on_off_server_set_t *pdata = pargs;
            if (pdata->on_off != current_on_off)
            {
                if (GENERIC_ON == pdata->on_off)
                {
                    if (0 == light_get_cold()->lightness_last)
                    {
                        light_set_lightness_linear(light_get_cold(), 65535, 500, light_lightness_change_done);
                    }
                    else
                    {
                        light_set_lightness_linear(light_get_cold(), light_get_cold()->lightness_last, 500, NULL);
                    }
                }
                else
                {
                    light_set_lightness_linear(light_get_cold(), 0, 500, NULL);
                }
            }
            ret = MODEL_STOP_TRANSITION;
        }
        break;
    default:
        break;
    }

    return ret;
}

static int32_t light_lightness_server_data(const mesh_model_info_p pmodel_info, uint32_t type,
                                           void *pargs)
{
    int32_t ret = MODEL_SUCCESS;
    switch (type)
    {
    case LIGHT_LIGHTNESS_SERVER_GET:
        {
            light_lightness_server_get_t *pdata = pargs;
            pdata->lightness = light_get_cold()->lightness;
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_GET_LINEAR:
        {
            light_lightness_server_get_t *pdata = pargs;
            pdata->lightness = light_lightness_actual_to_linear(light_get_cold()->lightness);
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_GET_DEFAULT:
        {
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_GET_LAST:
        {
            light_lightness_server_get_t *pdata = pargs;
            pdata->lightness = light_get_cold()->lightness_last;
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_GET_RANGE:
        {
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_GET_DEFAULT_TRANSITION_TIME:
        {
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_SET:
        {
            light_lightness_server_set_t *pdata = pargs;
#if USE_SIG_TRANSITION
            if (0 == pdata->total_time.num_steps)
            {
                /** no transition */
                light_set_cold_lightness(pdata->lightness);
                light_state_store();
            }
            else if (pdata->total_time.num_steps == pdata->remaining_time.num_steps)
            {
                if (1 == pdata->total_time.num_steps)
                {
                    /** do transition one time */
                    light_set_cold_lightness(pdata->lightness);
                    light_state_store();
                }
                else
                {
                    /** calculate delta and run change immediate */
                    delta_lightness = pdata->lightness - light_get_cold()->lightness;
                    delta_lightness /= pdata->total_time.num_steps;
                    light_set_cold_lightness(light_get_cold()->lightness + delta_lightness);
                }
            }
            else
            {
                if (pdata->remaining_time.num_steps > 1)
                {
                    /** do transition */
                    light_set_cold_lightness(light_get_cold()->lightness + delta_lightness);
                }
                else
                {
                    /** transition finished */
                }
            }
            ret = MODEL_SUCCESS;
#else
            light_set_lightness_linear(light_get_cold(), pdata->lightness, 500, light_lightness_change_done);
            ret = MODEL_STOP_TRANSITION;
#endif
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_SET_LINEAR:
        {
            light_lightness_server_set_t *pdata = pargs;
            light_set_lightness_linear(light_get_cold(), light_lightness_linear_to_actual(pdata->lightness),
                                       500, light_lightness_change_done);
            ret = MODEL_STOP_TRANSITION;
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_SET_LAST:
        {
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_SET_DEFAULT:
        {
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_SET_RANGE:
        {
        }
        break;
    default:
        break;
    }

    return ret;
}

void light_lightness_server_models_init(uint8_t element_index)
{
    /* binding models */
    light_lightness_server.pmodel_bound = &generic_on_off_server;
    light_lightness_setup_server.pmodel_bound = &light_lightness_server;

    /* register light lightness models */
    generic_on_off_server.model_data_cb = generic_on_off_server_data;
    generic_on_off_server_reg(element_index, &generic_on_off_server);

    light_lightness_server.model_data_cb = light_lightness_server_data;
    light_lightness_server_reg(element_index, &light_lightness_server);
    light_lightness_setup_server.model_data_cb = light_lightness_server_data;
    light_lightness_setup_server_reg(element_index, &light_lightness_setup_server);
}

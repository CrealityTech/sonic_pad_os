/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      light_hsl_server_app.c
* @brief     Smart mesh demo light hsl application
* @details
* @author    bill
* @date      2018-1-4
* @version   v1.0
* *********************************************************************************************************
*/

#include <math.h>
#include "light_hsl_server_app.h"
#include "generic_on_off.h"
#include "light_lightness.h"
#include "light_hsl.h"
#include "light_cwrgb_app.h"
#include "light_storage_app.h"

/** hsl light models */
static mesh_model_info_t generic_on_off_server;
static mesh_model_info_t light_lightness_server;
static mesh_model_info_t light_lightness_setup_server;
static mesh_model_info_t light_hsl_server;
static mesh_model_info_t light_hsl_setup_server;
static mesh_model_info_t light_hsl_hue_server;
static mesh_model_info_t light_hsl_saturation_server;


static int32_t generic_on_off_server_data(const mesh_model_info_p pmodel_info, uint32_t type,
                                          void *pargs)
{
    UNUSED(pmodel_info);
    generic_on_off_t current_on_off = GENERIC_OFF;
    if ((light_get_red()->lightness) ||
        (light_get_green()->lightness) ||
        (light_get_blue()->lightness))
    {
        current_on_off = GENERIC_ON;
    }

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
            if (pdata->total_time.num_steps == pdata->remaining_time.num_steps)
            {
                if (pdata->on_off != current_on_off)
                {
                    if (GENERIC_ON == pdata->on_off)
                    {
                        light_rgb_turn_on();
                        light_state_store();
                    }
                    else
                    {
                        light_rgb_turn_off();
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    return 0;
}

static int32_t light_lightness_server_data(const mesh_model_info_p pmodel_info, uint32_t type,
                                           void *pargs)
{
    switch (type)
    {
    case LIGHT_LIGHTNESS_SERVER_GET:
        {
            light_lightness_server_get_t *pdata = pargs;
            pdata->lightness = light_get_hsl().lightness;
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_GET_LINEAR:
        {
            light_lightness_server_get_t *pdata = pargs;
            pdata->lightness = light_lightness_actual_to_linear(light_get_hsl().lightness);
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_GET_DEFAULT:
        {
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_GET_LAST:
        {
            light_lightness_server_get_t *pdata = pargs;
            light_rgb_t rgb = {light_get_red()->lightness_last, light_get_green()->lightness_last, light_get_blue()->lightness_last};
            light_hsl_t hsl = rgb_2_hsl(rgb);
            pdata->lightness = hsl.lightness;
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
            if (pdata->total_time.num_steps == pdata->remaining_time.num_steps)
            {
                light_hsl_t hsl = light_get_hsl();
                hsl.lightness = pdata->lightness;
                light_set_hsl(hsl);
                light_state_store();
            }
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_SET_LINEAR:
        {
            light_lightness_server_set_t *pdata = pargs;
            if (pdata->total_time.num_steps == pdata->remaining_time.num_steps)
            {
                light_hsl_t hsl = light_get_hsl();
                hsl.lightness = light_lightness_linear_to_actual(pdata->lightness);
                light_set_hsl(hsl);
                light_state_store();
            }
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

    return 0;
}

static int32_t light_hsl_server_data(const mesh_model_info_p pmodel_info, uint32_t type,
                                     void *pargs)
{
    switch (type)
    {
    case LIGHT_HSL_SERVER_GET:
        {
            light_hsl_server_get_t *pdata = pargs;
            light_hsl_t hsl = light_get_hsl();
            pdata->lightness = hsl.lightness;
            pdata->hue = hsl.hue;
            pdata->saturation = hsl.saturation;
        }
        break;
    case LIGHT_HSL_SERVER_GET_HUE:
        {
            light_hsl_server_get_hue_t *pdata = pargs;
            light_hsl_t hsl = light_get_hsl();
            pdata->hue = hsl.hue;
        }
        break;
    case LIGHT_HSL_SERVER_GET_SATURATION:
        {
            light_hsl_server_get_saturation_t *pdata = pargs;
            light_hsl_t hsl = light_get_hsl();
            pdata->saturation = hsl.saturation;
        }
        break;
    case LIGHT_HSL_SERVER_GET_DEFAULT:
        {
        }
        break;
    case LIGHT_HSL_SERVER_GET_RANGE:
        {
        }
        break;
    case LIGHT_HSL_SERVER_GET_DEFAULT_TRANSITION_TIME:
        {
        }
        break;
    case LIGHT_HSL_SERVER_SET:
        {
            light_hsl_server_set_t *pdata = pargs;
            if (pdata->total_time.num_steps == pdata->remaining_time.num_steps)
            {
                light_hsl_t hsl;
                hsl.lightness = pdata->lightness;
                hsl.hue = pdata->hue;
                hsl.saturation = pdata->saturation;
                light_set_hsl(hsl);
                light_state_store();
            }
        }
        break;
    case LIGHT_HSL_SERVER_SET_HUE:
        {
            light_hsl_server_set_hue_t *pdata = pargs;
            if (pdata->total_time.num_steps == pdata->remaining_time.num_steps)
            {
                light_hsl_t hsl = light_get_hsl();
                hsl.hue = pdata->hue;
                light_set_hsl(hsl);
                light_state_store();
            }
        }
        break;
    case LIGHT_HSL_SERVER_SET_SATURATION:
        {
            light_hsl_server_set_saturation_t *pdata = pargs;
            if (pdata->total_time.num_steps == pdata->remaining_time.num_steps)
            {
                light_hsl_t hsl = light_get_hsl();
                hsl.saturation = pdata->saturation;
                light_set_hsl(hsl);
                light_state_store();
            }
        }
        break;
    case LIGHT_HSL_SERVER_SET_DEFAULT:
        {
        }
        break;
    case LIGHT_HSL_SERVER_SET_RANGE:
        {
        }
        break;
    default:
        break;
    }

    return 0;
}

void light_hsl_server_models_init(uint8_t element_index)
{
    /* binding models */
    light_lightness_server.pmodel_bound = &generic_on_off_server;
    light_lightness_setup_server.pmodel_bound = &light_lightness_server;
    light_hsl_server.pmodel_bound = &light_lightness_setup_server;
    light_hsl_setup_server.pmodel_bound = &light_hsl_server;
    light_hsl_hue_server.pmodel_bound = &light_hsl_setup_server;
    light_hsl_saturation_server.pmodel_bound = &light_hsl_hue_server;

    /* register light models */
    generic_on_off_server.model_data_cb = generic_on_off_server_data;
    generic_on_off_server_reg(element_index, &generic_on_off_server);

    light_lightness_server.model_data_cb = light_lightness_server_data;
    light_lightness_server_reg(element_index, &light_lightness_server);
    light_lightness_setup_server.model_data_cb = light_lightness_server_data;
    light_lightness_setup_server_reg(element_index, &light_lightness_setup_server);

    light_hsl_server.model_data_cb = light_hsl_server_data;
    light_hsl_server_reg(element_index, &light_hsl_server);
    light_hsl_setup_server.model_data_cb = light_hsl_server_data;
    light_hsl_setup_server_reg(element_index, &light_hsl_setup_server);
    light_hsl_hue_server.model_data_cb = light_hsl_server_data;
    light_hsl_hue_server_reg(element_index, &light_hsl_hue_server);
    light_hsl_saturation_server.model_data_cb = light_hsl_server_data;
    light_hsl_saturation_server_reg(element_index, &light_hsl_saturation_server);
}

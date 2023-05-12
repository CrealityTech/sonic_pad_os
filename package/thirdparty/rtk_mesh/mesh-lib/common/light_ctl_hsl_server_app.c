/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      light_ctl_hsl_server_app.c
* @brief     Smart mesh demo light ctl application
* @details
* @author    hector
* @date      2018-8-16
* @version   v1.0
* *********************************************************************************************************
*/

#include "mesh_api.h"
#include "light_ctl_hsl_server_app.h"
#include "generic_on_off.h"
#include "light_lightness.h"
#include "light_ctl.h"
#include "light_hsl.h"
#include "light_cwrgb_app.h"
#include "light_storage_app.h"


/* ctl & hsl light models */
static mesh_model_info_t generic_on_off_server;
static mesh_model_info_t light_lightness_server;
static mesh_model_info_t light_lightness_setup_server;
static mesh_model_info_t light_ctl_server;
static mesh_model_info_t light_ctl_setup_server;
static mesh_model_info_t light_ctl_temperature_server;
static mesh_model_info_t light_hsl_server;
static mesh_model_info_t light_hsl_setup_server;
static mesh_model_info_t light_hsl_hue_server;
static mesh_model_info_t light_hsl_saturation_server;


static int32_t generic_on_off_server_data(const mesh_model_info_p pmodel_info, uint32_t type,
                                          void *pargs)
{
    generic_on_off_t current_on_off = GENERIC_OFF;
    if ((light_get_cold()->lightness) ||
        (light_get_warm()->lightness) ||
        (light_get_red()->lightness) ||
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
                        light_cw_turn_on();
                        light_rgb_turn_on();
                        light_state_store();
                    }
                    else
                    {
                        light_cw_turn_off();
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
            if (pdata->total_time.num_steps == pdata->remaining_time.num_steps)
            {
                light_set_cold_lightness(pdata->lightness);
                light_state_store();
            }
        }
        break;
    case LIGHT_LIGHTNESS_SERVER_SET_LINEAR:
        {
            light_lightness_server_set_t *pdata = pargs;
            if (pdata->total_time.num_steps == pdata->remaining_time.num_steps)
            {
                light_set_cold_lightness(light_lightness_linear_to_actual(pdata->lightness));
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

static int32_t light_ctl_server_data(const mesh_model_info_p pmodel_info, uint32_t type,
                                     void *pargs)
{
    switch (type)
    {
    case LIGHT_CTL_SERVER_GET:
        {
            light_ctl_server_get_t *pdata = pargs;
            light_ctl_t ctl = light_get_ctl();
            pdata->lightness = ctl.lightness;
            pdata->temperature = ctl.temperature;
        }
        break;
    case LIGHT_CTL_SERVER_GET_DEFAULT:
        {
        }
        break;
    case LIGHT_CTL_SERVER_GET_TEMPERATURE:
        {
            light_ctl_server_get_temperature_t *pdata = pargs;
            light_ctl_t ctl = light_get_ctl();
            pdata->temperature = ctl.temperature;
            pdata->delta_uv = ctl.delta_uv;
        }
        break;
    case LIGHT_CTL_SERVER_GET_TEMPERATURE_RANGE:
        {
        }
        break;
    case LIGHT_CTL_SERVER_GET_DEFAULT_TRANSITION_TIME:
        {
        }
        break;
    case LIGHT_CTL_SERVER_SET:
        {
            light_ctl_server_set_t *pdata = pargs;
            if (pdata->total_time.num_steps == pdata->remaining_time.num_steps)
            {
                light_ctl_t ctl = light_get_ctl();
                //ctl.lightness = pdata->lightness;
                ctl.temperature = pdata->temperature;
                ctl.delta_uv = pdata->delta_uv;
                light_set_ctl(ctl);
                light_state_store();
            }
        }
        break;
    case LIGHT_CTL_SERVER_SET_TEMPERATURE:
        {
            light_ctl_server_set_temperature_t *pdata = pargs;
            if (pdata->total_time.num_steps == pdata->remaining_time.num_steps)
            {
                light_ctl_t ctl = light_get_ctl();
                ctl.temperature = pdata->temperature;
                ctl.delta_uv = pdata->delta_uv;
                light_set_ctl(ctl);
                light_state_store();
            }
        }
        break;
    case LIGHT_CTL_SERVER_SET_DEFAULT:
        {
        }
        break;
    case LIGHT_CTL_SERVER_SET_TEMPERATURE_RANGE:
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

void light_ctl_hsl_server_models_init(uint8_t element_index)
{
    /* binding models */
    light_lightness_server.pmodel_bound = &generic_on_off_server;
    light_lightness_setup_server.pmodel_bound = &light_lightness_server;
    light_ctl_server.pmodel_bound = &light_lightness_setup_server;
    light_ctl_setup_server.pmodel_bound = &light_ctl_server;
    light_ctl_temperature_server.pmodel_bound = &light_ctl_setup_server;
    light_hsl_server.pmodel_bound = &light_ctl_temperature_server;
    light_hsl_setup_server.pmodel_bound = &light_hsl_server;
    light_hsl_hue_server.pmodel_bound = &light_hsl_setup_server;
    light_hsl_saturation_server.pmodel_bound = &light_hsl_hue_server;

    /* register light ctl & hsl models */
    generic_on_off_server.model_data_cb = generic_on_off_server_data;
    generic_on_off_server_reg(element_index, &generic_on_off_server);

    light_lightness_server.model_data_cb = light_lightness_server_data;
    light_lightness_server_reg(element_index, &light_lightness_server);
    light_lightness_setup_server.model_data_cb = light_lightness_server_data;
    light_lightness_setup_server_reg(element_index, &light_lightness_setup_server);

    light_ctl_server.model_data_cb = light_ctl_server_data;
    light_ctl_server_reg(element_index, &light_ctl_server);
    light_ctl_setup_server.model_data_cb = light_ctl_server_data;
    light_ctl_setup_server_reg(element_index, &light_ctl_setup_server);
    light_ctl_temperature_server.model_data_cb = light_ctl_server_data;
    light_ctl_temperature_server_reg(element_index, &light_ctl_temperature_server);

    light_hsl_server.model_data_cb = light_hsl_server_data;
    light_hsl_server_reg(element_index, &light_hsl_server);
    light_hsl_setup_server.model_data_cb = light_hsl_server_data;
    light_hsl_setup_server_reg(element_index, &light_hsl_setup_server);
    light_hsl_hue_server.model_data_cb = light_hsl_server_data;
    light_hsl_hue_server_reg(element_index, &light_hsl_hue_server);
    light_hsl_saturation_server.model_data_cb = light_hsl_server_data;
    light_hsl_saturation_server_reg(element_index, &light_hsl_saturation_server);
}

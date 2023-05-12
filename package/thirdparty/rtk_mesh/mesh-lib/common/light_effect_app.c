/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     dimmable_light_app.c
* @brief    This file provides demo code for the operation of dimmable light.
* @details
* @author   hector_huang
* @date     2018-11-19
* @version  v1.0
*********************************************************************************************************
*/
#include "light_effect_app.h"
#include "light_config.h"
#include "light_cwrgb_app.h"
#include "light_storage_app.h"
#include "light_controller_app.h"

static void turn_on_light(void)
{
    light_cwrgb_turn_off();
#if LIGHT_TYPE == LIGHT_LIGHTNESS
    light_set_cold_lightness(65535);
#elif LIGHT_TYPE == LIGHT_CW
    light_cw_t cw = {65535, 65535};
    light_set_cw_lightness(cw);
#elif LIGHT_TYPE == LIGHT_RGB
    light_rgb_t rgb = {65535, 65535, 65535};
    light_set_rgb_lightness(rgb);
#else
    light_cw_t cw = {65535, 65535};
    light_rgb_t rgb = {65535, 65535, 65535};
    light_set_cw_lightness(cw);
    light_set_rgb_lightness(rgb);
#endif
    light_state_store();
}

#if MESH_ALI_CERTIFICATION
static bool on_power_up = TRUE;
static bool need_unprov_effect = FALSE;

void light_set_unprov_effect(bool flag)
{
    need_unprov_effect = flag;
}

void light_prov_unprov_cb(light_t *light)
{
    light_set_red_lightness(light->lightness * 0.9);
}

void light_prov_complete_cb(light_t *light)
{
    turn_on_light();
}
#endif

void light_prov_unprov(void)
{
#if MESH_ALI_CERTIFICATION
    light_t *light = light_get_red();
    if (on_power_up)
    {
        on_power_up = FALSE;
        if (need_unprov_effect)
        {
            light_cw_turn_off();
            light_set_green_lightness(0);
            light_set_blue_lightness(0);

            light_blink(light, 0, light->lightness, 1000, 50, 5, light_prov_unprov_cb);
        }
        else
        {
            /* set light default lightness */
            if (0 == light->lightness)
            {
                light->lightness = 65535;
            }
            light_set_red_lightness(light->lightness * 0.9);
        }
    }
    else
    {
        light_cw_turn_off();
        light_set_green_lightness(0);
        light_set_blue_lightness(0);

        light_blink(light, 0, light->lightness, 1000, 50, 5, light_prov_unprov_cb);
    }
#else
    light_cwrgb_turn_off();
    light_blink_red(1, 2, 50);
#endif
}

void light_prov_link_open(void)
{
    light_set_red_lightness(65535);
}

void light_prov_link_closed(void)
{
    light_blink_red(1, 2, 50);
}

void light_prov_start(void)
{
#if MESH_ALI_CERTIFICATION
#else
    light_blink_red(2, 1, 50);
#endif
}

void light_prov_complete(void)
{
#if MESH_ALI_CERTIFICATION
    light_t *light = light_get_red();
    light_blink(light, 0, light->lightness, 1000, 50, 3, light_prov_complete_cb);
#else
    turn_on_light();
#endif
}

void light_gap_state_disconnect(void)
{
    light_blink_red(1, 2, 50);
}

void light_gap_state_connected(void)
{
    light_set_red_lightness(65535);
}

void light_dfu_client_start(void)
{
    light_cwrgb_turn_off();
    light_blink_blue(2, 1, 50);
}

void light_dfu_client_end(void)
{
    light_cw_t cw = {light_get_cold()->lightness_last, light_get_warm()->lightness_last};
    light_rgb_t rgb = {light_get_red()->lightness_last, light_get_green()->lightness_last, light_get_blue()->lightness_last};
    light_set_cw_lightness(cw);
    light_set_rgb_lightness(rgb);
}

void light_dfu_client_fail(void)
{
    light_cw_t cw = {light_get_cold()->lightness_last, light_get_warm()->lightness_last};
    light_rgb_t rgb = {light_get_red()->lightness_last, light_get_green()->lightness_last, light_get_blue()->lightness_last};
    light_set_cw_lightness(cw);
    light_set_rgb_lightness(rgb);
}

void light_dfu_server_start(void)
{
    light_cwrgb_turn_off();
    light_blink_green(2, 1, 50);
}

void light_dfu_server_end(void)
{
    light_cw_t cw = {light_get_cold()->lightness_last, light_get_warm()->lightness_last};
    light_rgb_t rgb = {light_get_red()->lightness_last, light_get_green()->lightness_last, light_get_blue()->lightness_last};
    light_set_cw_lightness(cw);
    light_set_rgb_lightness(rgb);
}

void light_dfu_server_fail(void)
{
    light_blink_green(1, 2, 50);
}



/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     light_storage_app.h
* @brief    Head file for dimmable light flash storage.
* @details  Data structs and external functions implemention.
* @author   hector_huang
* @date     2018-11-16
* @version  v0.1
* *************************************************************************************
*/

#include "light_storage_app.h"
#include "light_cwrgb_app.h"
#include "mesh_api.h"
#include "light_config.h"

static bool light_state_restore(void)
{
    bool ret = TRUE;
    mesh_node_state_t node_state = mesh_node_state_restore();
    if (node_state == PROV_NODE)
    {
        light_flash_light_state_t light_state = {65535, 65535, 65535, 65535, 65535};
        ret = light_flash_read(LIGHT_FLASH_PARAM_TYPE_LIGHT_STATE, sizeof(light_flash_light_state_t),
                               &light_state);
#if LIGHT_TYPE == LIGHT_LIGHTNESS
        light_set_cold_lightness(light_state.state[0]);
#elif LIGHT_TYPE == LIGHT_CW
        light_set_cold_lightness(light_state.state[0]);
        light_set_warm_lightness(light_state.state[1]);
#elif LIGHT_TYPE == LIGHT_RGB
        light_set_red_lightness(light_state.state[2]);
        light_set_green_lightness(light_state.state[3]);
        light_set_blue_lightness(light_state.state[4]);
#else
        light_set_cold_lightness(light_state.state[0]);
        light_set_warm_lightness(light_state.state[1]);
        light_set_red_lightness(light_state.state[2]);
        light_set_green_lightness(light_state.state[3]);
        light_set_blue_lightness(light_state.state[4]);
#endif
    }

    return ret;
}

static bool light_user_data_restore(void)
{
    /** restore user data from flash */
    return TRUE;
}

bool light_state_store(void)
{
    bool ret = TRUE;
    light_cw_t cw = light_get_cw_lightness();
    light_rgb_t rgb = light_get_rgb_lightness();
    light_flash_light_state_t light_state = {cw.cold, cw.warm, rgb.red, rgb.green, rgb.blue};
    ret = light_flash_write(LIGHT_FLASH_PARAM_TYPE_LIGHT_STATE, sizeof(light_flash_light_state_t),
                            &light_state);
    return ret;
}

bool light_user_data_store(void)
{
    return TRUE;
}

bool light_flash_restore(void)
{
    light_flash_power_on_count_t power_on_count = {.count = 0};
    light_flash_read(LIGHT_FLASH_PARAM_TYPE_POWER_ON_COUNT, sizeof(light_flash_power_on_count_t),
                     &power_on_count);
    if (!light_power_on_count_check(power_on_count.count))
    {
        light_state_restore();
        light_user_data_restore();
        return FALSE;
    }
    return TRUE;
}


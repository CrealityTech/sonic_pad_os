/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     light_cwrgb_app.c
* @brief    This file provides demo code for the operation of dimmable light.
* @details
* @author   hector_huang
* @date     2018-11-16
* @version  v1.0
*********************************************************************************************************
*/
#include <math.h>
#include "light_cwrgb_app.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_tim.h"
#include "light_config.h"

#define DEFAULT_LIGHTNESS    0

/* light cwrgb channels, set pin_num to PIN_INVALID if do not use that color */
static light_t light_cwrgb[] =
{
    /** cold */
    {P4_3, timer_pwm2, TIM2, FALSE, DEFAULT_LIGHTNESS, DEFAULT_LIGHTNESS},
#if DEMO_BOARD
    /** warm: use blue channel */
    {P4_0, timer_pwm6, TIM6, FALSE, DEFAULT_LIGHTNESS, DEFAULT_LIGHTNESS},
#else
    /** warm: set to actual pin*/
    {PIN_INVALID, timer_pwm3, FALSE, TIM3, DEFAULT_LIGHTNESS, DEFAULT_LIGHTNESS},
#endif
    /** red */
    {P4_1, timer_pwm4, TIM4, FALSE, DEFAULT_LIGHTNESS, DEFAULT_LIGHTNESS},
    /** green */
    {P4_2, timer_pwm5, TIM5, FALSE, DEFAULT_LIGHTNESS, DEFAULT_LIGHTNESS},
    /** blue */
    {P4_0, timer_pwm6, TIM6, FALSE, DEFAULT_LIGHTNESS, DEFAULT_LIGHTNESS},
};

static light_hsl_t light_hsl;
static light_ctl_t light_ctl;

static float hue_2_rgb(float v1, float v2, float h)
{
    if (h < 0)
    {
        h += 1;
    }
    else if (h > 1)
    {
        h -= 1;
    }
    if (6 * h < 1)
    {
        return v1 + (v2 - v1) * 6 * h;
    }
    else if (2 * h < 1)
    {
        return v2;
    }
    else if (3 * h < 2)
    {
        return v1 + (v2 - v1) * (4 - 6 * h);
    }
    else
    {
        return v1;
    }
}

light_rgb_t hsl_2_rgb(light_hsl_t hsl)
{
    light_rgb_t rgb = {0, 0, 0};
    if (0 == hsl.saturation)
    {
        rgb.red = rgb.green = rgb.blue = hsl.lightness;
    }
    else
    {
        float h, s, l, v1, v2;
        h = (float)hsl.hue / 65536;
        s = (float)hsl.saturation / 65535;
        l = (float)hsl.lightness / 65535;
        if (l < 0.5f)
        {
            v2 = l * (1 + s);
        }
        else
        {
            v2 = (l + s) - (s * l);
        }
        v1 = 2 * l - v2;
        rgb.red = ceil(65535 * hue_2_rgb(v1, v2, h + 1.0 / 3));
        rgb.green = ceil(65535 * hue_2_rgb(v1, v2, h));
        rgb.blue = ceil(65535 * hue_2_rgb(v1, v2, h - 1.0 / 3));
    }

    return rgb;
}

light_hsl_t rgb_2_hsl(light_rgb_t rgb)
{
    light_hsl_t hsl = {0, 0, 0};
    uint16_t max, min;
    max = MAX(rgb.red, rgb.green);
    max = MAX(max, rgb.blue);
    min = MIN(rgb.red, rgb.green);
    min = MIN(min, rgb.blue);
    if (max == min)
    {
        hsl.hue = 0;
    }
    else if (max == rgb.red) //!< red: 0 degree
    {
        if (rgb.green >= rgb.blue)
        {
            hsl.hue = (65536 / 6) * (rgb.green - rgb.blue) / (max - min);
        }
        else
        {
            uint32_t tmp = 65536 - (65536 / 6) * (rgb.blue - rgb.green) / (max - min);
            if (tmp == 65536)
            {
                hsl.hue = 0;
            }
            else
            {
                hsl.hue = tmp;
            }
        }
    }
    else if (max == rgb.green) //!< green: 120 degree
    {
        hsl.hue = 65536 / 3 + (65536 / 6) * (rgb.blue - rgb.red) / (max - min);
    }
    else //!< blue: 240 degree
    {
        hsl.hue = 65536 * 2 / 3 + (65536 / 6) * (rgb.red - rgb.green) / (max - min);
    }
    hsl.lightness = (max + min) >> 1;
    if (hsl.lightness == 0 || max == min)
    {
        hsl.saturation = 0;
    }
    else if (hsl.lightness <= 32767)
    {
        hsl.saturation = ceil(65535 * (double)(max - min) / (max + min));
    }
    else
    {
        hsl.saturation = ceil(65535 * (double)(max - min) / (2 * 65535 - max - min));
    }

    return hsl;
}

uint16_t temperature_to_lightness(uint16_t temperature)
{
    float delta = 0x4e20 - 0x320;
    return (temperature - 0x320) / delta * 65535;
}

uint16_t lightness_to_temperature(uint16_t lightness)
{
    float delta = 0x4e20 - 0x320;
    return (uint16_t)(lightness / 65535.0 * delta) + 0x320;
}

void light_cwrgb_driver_init(void)
{
    for (uint8_t i = 0; i < sizeof(light_cwrgb) / sizeof(light_t); ++i)
    {
        light_pin_config(&light_cwrgb[i]);
    }
}

void light_cwrgb_enter_dlps(void)
{
    for (uint8_t i = 0; i < sizeof(light_cwrgb) / sizeof(light_t); ++i)
    {
        light_pin_dlps_config(&light_cwrgb[i]);
    }
}

void light_set_cold_lightness(uint16_t lightness)
{
    light_set_lightness(&light_cwrgb[0], lightness);
}

void light_lighten_cold(uint16_t lightness)
{
    light_lighten(&light_cwrgb[0], lightness);
}

void light_set_warm_lightness(uint16_t lightness)
{
    light_set_lightness(&light_cwrgb[1], lightness);
}

void light_lighten_warm(uint16_t lightness)
{
    light_lighten(&light_cwrgb[1], lightness);
}

void light_set_cw_lightness(light_cw_t lightness)
{
    light_set_lightness(&light_cwrgb[0], lightness.cold);
    light_set_lightness(&light_cwrgb[1], lightness.warm);
}

void light_lighten_cw(light_cw_t lightness)
{
    light_lighten(&light_cwrgb[0], lightness.cold);
    light_lighten(&light_cwrgb[1], lightness.warm);
}

light_cw_t light_get_cw_lightness(void)
{
    light_cw_t cw = {light_cwrgb[0].lightness, light_cwrgb[1].lightness};
    return cw;
}

void light_set_red_lightness(uint16_t lightness)
{
    light_set_lightness(&light_cwrgb[2], lightness);
}

void light_lighten_red(uint16_t lightness)
{
    light_lighten(&light_cwrgb[2], lightness);
}

void light_set_green_lightness(uint16_t lightness)
{
    light_set_lightness(&light_cwrgb[3], lightness);
}

void light_lighten_green(uint16_t lightness)
{
    light_lighten(&light_cwrgb[3], lightness);
}

void light_set_blue_lightness(uint16_t lightness)
{
    light_set_lightness(&light_cwrgb[4], lightness);
}

void light_lighten_blue(uint16_t lightness)
{
    light_lighten(&light_cwrgb[4], lightness);
}

void light_set_rgb_lightness(light_rgb_t lightness)
{
    light_set_lightness(&light_cwrgb[2], lightness.red);
    light_set_lightness(&light_cwrgb[3], lightness.green);
    light_set_lightness(&light_cwrgb[4], lightness.blue);
}

void light_lighten_rgb(light_rgb_t lightness)
{
    light_lighten(&light_cwrgb[2], lightness.red);
    light_lighten(&light_cwrgb[3], lightness.green);
    light_lighten(&light_cwrgb[4], lightness.blue);
}

light_rgb_t light_get_rgb_lightness(void)
{
    light_rgb_t rgb = {light_cwrgb[2].lightness, light_cwrgb[3].lightness, light_cwrgb[4].lightness};
    return rgb;
}

void light_cw_turn_off(void)
{
    light_lighten(&light_cwrgb[0], 0);
    light_lighten(&light_cwrgb[1], 0);
}

void light_cw_turn_on(void)
{
    if ((0 == light_cwrgb[0].lightness_last) &&
        (0 == light_cwrgb[1].lightness_last))

    {
        light_set_lightness(&light_cwrgb[0], 65535);
        light_set_lightness(&light_cwrgb[1], 65535);
    }
    else
    {
        light_lighten(&light_cwrgb[0], light_cwrgb[0].lightness_last);
        light_lighten(&light_cwrgb[1], light_cwrgb[1].lightness_last);
    }
}

void light_rgb_turn_off(void)
{
    light_lighten(&light_cwrgb[2], 0);
    light_lighten(&light_cwrgb[3], 0);
    light_lighten(&light_cwrgb[4], 0);
}

void light_rgb_turn_on(void)
{
    if ((0 == light_cwrgb[2].lightness_last) &&
        (0 == light_cwrgb[3].lightness_last) &&
        (0 == light_cwrgb[4].lightness_last))

    {
        light_set_lightness(&light_cwrgb[2], 65535);
        light_set_lightness(&light_cwrgb[3], 65535);
        light_set_lightness(&light_cwrgb[4], 65535);
    }
    else
    {
        light_lighten(&light_cwrgb[2], light_cwrgb[2].lightness_last);
        light_lighten(&light_cwrgb[3], light_cwrgb[3].lightness_last);
        light_lighten(&light_cwrgb[4], light_cwrgb[4].lightness_last);
    }
}

void light_cwrgb_turn_off(void)
{
    for (uint8_t i = 0; i < sizeof(light_cwrgb) / sizeof(light_t); ++i)
    {
        light_lighten(&light_cwrgb[i], 0);
    }
}

void light_cwrgb_turn_on(void)
{
    if ((0 == light_cwrgb[0].lightness_last) &&
        (0 == light_cwrgb[1].lightness_last) &&
        (0 == light_cwrgb[2].lightness_last) &&
        (0 == light_cwrgb[3].lightness_last) &&
        (0 == light_cwrgb[4].lightness_last))

    {
        light_set_lightness(&light_cwrgb[0], 65535);
        light_set_lightness(&light_cwrgb[1], 65535);
        light_set_lightness(&light_cwrgb[2], 65535);
        light_set_lightness(&light_cwrgb[3], 65535);
        light_set_lightness(&light_cwrgb[4], 65535);
    }
    else
    {
        light_lighten(&light_cwrgb[0], light_cwrgb[0].lightness_last);
        light_lighten(&light_cwrgb[1], light_cwrgb[1].lightness_last);
        light_lighten(&light_cwrgb[2], light_cwrgb[2].lightness_last);
        light_lighten(&light_cwrgb[3], light_cwrgb[3].lightness_last);
        light_lighten(&light_cwrgb[4], light_cwrgb[4].lightness_last);
    }
}

void light_blink_cold(uint32_t hz_numerator, uint32_t hz_denominator, uint8_t duty)
{
    light_blink_infinite(&light_cwrgb[0], hz_numerator, hz_denominator, duty);
}

void light_blink_warm(uint32_t hz_numerator, uint32_t hz_denominator, uint8_t duty)
{
    light_blink_infinite(&light_cwrgb[1], hz_numerator, hz_denominator, duty);
}

void light_blink_red(uint32_t hz_numerator, uint32_t hz_denominator, uint8_t duty)
{
    light_blink_infinite(&light_cwrgb[2], hz_numerator, hz_denominator, duty);
}

void light_blink_green(uint32_t hz_numerator, uint32_t hz_denominator, uint8_t duty)
{
    light_blink_infinite(&light_cwrgb[3], hz_numerator, hz_denominator, duty);
}

void light_blink_blue(uint32_t hz_numerator, uint32_t hz_denominator, uint8_t duty)
{
    light_blink_infinite(&light_cwrgb[4], hz_numerator, hz_denominator, duty);
}

void light_set_ctl(light_ctl_t ctl)
{
    light_set_lightness(&light_cwrgb[0], ctl.lightness);
    light_set_lightness(&light_cwrgb[1], temperature_to_lightness(ctl.temperature));
    light_ctl = ctl;
}

light_ctl_t light_get_ctl(void)
{
    light_ctl.lightness = light_cwrgb[0].lightness;
    light_ctl.temperature = lightness_to_temperature(light_cwrgb[1].lightness);
    return light_ctl;
}

void light_set_hsl(light_hsl_t hsl)
{
    light_rgb_t rgb = hsl_2_rgb(hsl);
    light_set_lightness(&light_cwrgb[2], rgb.red);
    light_set_lightness(&light_cwrgb[3], rgb.green);
    light_set_lightness(&light_cwrgb[4], rgb.blue);
    light_hsl = hsl;
}

light_hsl_t light_get_hsl(void)
{
    light_rgb_t rgb = {light_cwrgb[2].lightness, light_cwrgb[3].lightness, light_cwrgb[4].lightness};
    light_hsl = rgb_2_hsl(rgb);
    return light_hsl;
}

light_t *light_get_cold(void)
{
    return &light_cwrgb[0];
}

light_t *light_get_warm(void)
{
    return &light_cwrgb[1];
}

light_t *light_get_red(void)
{
    return &light_cwrgb[2];
}

light_t *light_get_green(void)
{
    return &light_cwrgb[3];
}

light_t *light_get_blue(void)
{
    return &light_cwrgb[4];
}

void light_set_cwrgb(const uint16_t *cwrgb)
{
#if LIGHT_TYPE == LIGHT_LIGHTNESS
    light_set_cold_lightness(cwrgb[0]);
#elif LIGHT_TYPE == LIGHT_CW
    light_cw_t cw = {cwrgb[0], cwrgb[1]};
    light_set_cw_lightness(cw);
#elif LIGHT_TYPE == LIGHT_RGB
    light_rgb_t rgb = {cwrgb[2], cwrgb[3], cwrgb[4]};
    light_set_rgb_lightness(rgb);
#else
    light_set_cold_lightness(cwrgb[0]);
    light_set_warm_lightness(cwrgb[1]);
    light_set_red_lightness(cwrgb[2]);
    light_set_green_lightness(cwrgb[3]);
    light_set_blue_lightness(cwrgb[4]);
#endif
}

void light_get_cwrgb(uint8_t *cwrgb)
{
    for (uint8_t i = 0; i < 5; ++i)
    {
        if (PIN_INVALID != light_cwrgb[i].pin_num)
        {
            cwrgb[i] = light_cwrgb[i].lightness / 65535.0 * 255;
        }
        else
        {
            cwrgb[i] = 0;
        }
    }
}



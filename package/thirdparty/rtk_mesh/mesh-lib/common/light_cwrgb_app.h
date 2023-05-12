/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     light_cwrgb_app.h
* @brief    Head file for dimmable light rgb.
* @details  Data structs and external functions declaration.
* @author   hector_huang
* @date     2018-11-16
* @version  v0.1
* *************************************************************************************
*/

#ifndef _LIGHT_CWRGB_APP_H_
#define _LIGHT_CWRGB_APP_H_

#include "dimmable_light.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_CWRGB_APP
 * @{
 */

/**
 * @defgroup Light_CWRGB_Exported_Macros Light CWRGB Exported Macros
 * @brief
 * @{
 */
typedef struct
{
    uint16_t cold;
    uint16_t warm;
} light_cw_t;

typedef struct
{
    uint16_t lightness;
    uint16_t temperature;
    int16_t delta_uv;
} light_ctl_t;

typedef struct
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} light_rgb_t;

typedef struct
{
    uint16_t lightness;
    uint16_t hue;
    uint16_t saturation;
} light_hsl_t;
/** @} */

/**
 * @defgroup Light_CWRGB_Exported_Functions Light CWRGB Exported Functions
 * @brief
 * @{
 */
/**
 * @brief initialize cwrgb light driver
 */
void light_cwrgb_driver_init(void);

/**
 * @brief cwrgb enter dlps
 */
void light_cwrgb_enter_dlps(void);

/**
 * @brief set cold lightness
 * @param[in] lightness: cold lightness
 * @note update lightness and lightness_last field
 */
void light_set_cold_lightness(uint16_t lightness);

/**
 * @brief set cold lightness
 * @param[in] lightness: cold lightness
 * @note update lightness field
 */
void light_lighten_cold(uint16_t lightness);

/**
 * @brief set warm lightness
 * @param[in] lightness: warm lightness
 * @note update lightness and lightness_last field
 */
void light_set_warm_lightness(uint16_t lightness);

/**
 * @brief set warm lightness
 * @param[in] lightness: warm lightness
 * @note update lightness field
 */
void light_lighten_warm(uint16_t lightness);

/**
 * @brief set cw lightness
 * @param[in] lightness: cw lightness
 * @note update lightness and lightness_last field
 */
void light_set_cw_lightness(light_cw_t lightness);

/**
 * @brief set cw lightness
 * @param[in] lightness: cw lightness
 * @note update lightness field
 */
void light_lighten_cw(light_cw_t lightness);

/**
 * @brief get cw lightness
 * @return cw lightness
 */
light_cw_t light_get_cw_lightness(void);

/**
 * @brief set red light lightness
 * @param[in] lightness: light lightness
 * @note update lightness and lightness_last field
 */
void light_set_red_lightness(uint16_t lightness);

/**
 * @brief set red light lightness
 * @param[in] lightness: light lightness
 * @note update lightness field
 */
void light_lighten_red(uint16_t lightness);

/**
 * @brief set green light lightness
 * @param[in] lightness: light lightness
 * @note update lightness and lightness_last field
 */
void light_set_green_lightness(uint16_t lightness);

/**
 * @brief set green light lightness
 * @param[in] lightness: light lightness
 * @note update lightness field
 */
void light_lighten_green(uint16_t lightness);

/**
 * @brief set blue light lightness
 * @param[in] lightness: light lightness
 * @note update lightness and lightness_last field
 */
void light_set_blue_lightness(uint16_t lightness);

/**
 * @brief set blue light lightness
 * @param[in] lightness: light lightness
 * @note update lightness field
 */
void light_lighten_blue(uint16_t lightness);

/**
 * @brief set rgb lightness
 * @param[in] lightness: light lightness
 * @note update lightness and lightness_last field
 */
void light_set_rgb_lightness(light_rgb_t lightness);

/**
 * @brief set rgb lightness
 * @param[in] lightness: light lightness
 * @note update lightness field
 */
void light_lighten_rgb(light_rgb_t lightness);

/**
 * @brief get rgb lightness
 * @return rgb lightness
 */
light_rgb_t light_get_rgb_lightness(void);

/**
 * @brief blink cold light
 * @param[in] hz_numerator: hz numerator
 * @param[in] hz_denominator: hz denominator
 * @param[in] duty: high duty
 * @note frequency = hz_numerator / hz_denominator Hz, range: 1/13 ~ 2.5K
 */
void light_blink_cold(uint32_t hz_numerator, uint32_t hz_denominator, uint8_t duty);

/**
 * @brief blink warm light
 * @param[in] hz_numerator: hz numerator
 * @param[in] hz_denominator: hz denominator
 * @param[in] duty: high duty
 * @note frequency = hz_numerator / hz_denominator Hz, range: 1/13 ~ 2.5K
 */
void light_blink_warm(uint32_t hz_numerator, uint32_t hz_denominator, uint8_t duty);

/**
 * @brief blink red light
 * @param[in] hz_numerator: hz numerator
 * @param[in] hz_denominator: hz denominator
 * @param[in] duty: high duty
 * @note frequency = hz_numerator / hz_denominator Hz, range: 1/13 ~ 2.5K
 */
void light_blink_red(uint32_t hz_numerator, uint32_t hz_denominator, uint8_t duty);

/**
 * @brief blink green light
 * @param[in] hz_numerator: hz numerator
 * @param[in] hz_denominator: hz denominator
 * @param[in] duty: high duty
 * @note frequency = hz_numerator / hz_denominator Hz, range: 1/13 ~ 2.5K
 */
void light_blink_green(uint32_t hz_numerator, uint32_t hz_denominator, uint8_t duty);

/**
 * @brief blink blue light
 * @param[in] hz_numerator: hz numerator
 * @param[in] hz_denominator: hz denominator
 * @param[in] duty: high duty
 * @note frequency = hz_numerator / hz_denominator Hz, range: 1/13 ~ 2.5K
 */
void light_blink_blue(uint32_t hz_numerator, uint32_t hz_denominator, uint8_t duty);

/**
 * @brief turn off cw light
 * @note update lightness field only
 */
void light_cw_turn_off(void);

/**
 * @brief turn on cw light
 */
void light_cw_turn_on(void);

/**
 * @brief turn off rgb light
 * @note update lightness field only
 */
void light_rgb_turn_off(void);

/**
 * @brief turn on rgb light
 * @note update lightness field only
 */
void light_rgb_turn_on(void);

/**
 * @brief turn off rgb light
 * @note update lightness field only
 */
void light_cwrgb_turn_off(void);

/**
 * @brief turn on rgb light
 * @note update lightness field only
 */
void light_cwrgb_turn_on(void);

/**
 * @brief get cold pin handle
 * @return cold pin handle
 */
light_t *light_get_cold(void);

/**
 * @brief get warm pin handle
 * @return warm pin handle
 */
light_t *light_get_warm(void);

/**
 * @brief get red light pin handle
 * @return red light pin handle
 */
light_t *light_get_red(void);

/**
 * @brief get green light pin handle
 * @return green light pin handle
 */
light_t *light_get_green(void);

/**
 * @brief get blue light pin handle
 * @return blue light pin handle
 */
light_t *light_get_blue(void);

/**
 * @brief convert lightness to temperature
 * @param[in] temperature: temperature value
 * @return lightness value
 */
uint16_t temperature_to_lightness(uint16_t temperature);

/**
 * @brief convert lightness to temperature
 * @param[in] lightness: lightness value
 * @return temperature value
 */
uint16_t lightness_to_temperature(uint16_t lightness);

/**
 * @brief set ctl value
 * @param[in] ctl: light ctl value
 */
void light_set_ctl(light_ctl_t ctl);

/**
 * @brief get ctl value
 * @return light ctl value
 */
light_ctl_t light_get_ctl(void);

/**
 * @brief convert rgb to hsl
 * @param[in] rgb: rgb value
 * @return hsl value
 */
light_hsl_t rgb_2_hsl(light_rgb_t rgb);

/**
 * @brief convert hsl to rgb
 * @param[in] hsl: hsl value
 * @return rgb value
 */
light_rgb_t hsl_2_rgb(light_hsl_t hsl);

/**
 * @brief set light hsl value
 * @param[in] hsl: hsl value
 */
void light_set_hsl(light_hsl_t hsl);

/**
 * @brief get hsl value
 * @return hsl value
 */
light_hsl_t light_get_hsl(void);


/**
 * @brief light cwrgb model set cwrgb value
 * @param[in] cwrgb: cwrgb value
 */
void light_set_cwrgb(const uint16_t *cwrgb);

/**
 * @brief light cergb model get cwrgb value
 * @param[out] cwrgb: cwrgb value
 */
void light_get_cwrgb(uint8_t *cwrgb);
/** @} */
/** @} */


END_DECLS

#endif /** _LIGHT_CWRGB_APP_H_ */


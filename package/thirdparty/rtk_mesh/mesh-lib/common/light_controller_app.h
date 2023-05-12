/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      light_controller_app.h
* @brief     light controller
* @details
* @author    hector huang
* @date      2018-09-18
* @version   v1.0
* *********************************************************************************************************
*/
#ifndef _LIGHT_CONTROLLER_APP_H_
#define _LIGHT_CONTROLLER_APP_H_

#include "platform_types.h"
#include "dimmable_light.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_CONTROLLER_APP
 * @{
 */

/**
 * @defgroup Light_Controller_Exported_Macros Light Controller Exported Macros
 * @brief
 * @{
 */
#define TIME_INFINITE    0xFFFFFFFF
/** @} */

/**
 * @defgroup Light_Controller_Exported_Types Light Controller Exported Types
 * @brief
 * @{
 */
typedef void (*light_change_done_cb)(light_t *light);
/** @} */

/**
 * @defgroup Light_Controller_Exported_Functions Light Controller Exported Functions
 * @brief
 * @{
 */

/**
 * @brief initialize light controller
 * @retval TRUE: initialize success
 * @retval FALSE: initialize failed
 */
bool light_controller_init(void);

/**
 * @brief deinitialize light controller
 */
void light_controller_deinit(void);

/**
 * @brief set lightness gradual change
 * @param[in] light: light channel
 * @param[in] lightness: target lightness
 * @param[in] time: gradual change time
 * @param[in] cb: light change done callback function
 */
void light_set_lightness_linear(light_t *light, uint16_t lightness, uint32_t time,
                                light_change_done_cb cb);

/**
 * @brief blink light
 * @param[in] light: light channel
 * @param[in] lightness_begin: blink start lightness
 * @param[in] lightness_end: blink end lightness
 * @param[in] interval: blink interval, the unit is ms
 * @param[in] begin_duty: start lightness duty in total interval, value range is 0-100
 *            for example, intreval is 1000ms, duty is 60, then start lightness will lighten
 *            600ms, end lightness will lighten 400ms
 * @param[in] cb: light change done callback function
 * @param[in] times: blink times
 */
void light_blink(light_t *light, uint16_t lightness_begin, uint16_t lightness_end,
                 uint32_t interval, uint8_t begin_duty, uint32_t times, light_change_done_cb cb);

/**
 * @brief breath light
 * @param[in] light: light channel
 * @param[in] lightness_begin: breath start lightness
 * @param[in] lightness_end: breath end lightness
 * @param[in] interval: breath interval, the unit is ms
 * @param[in] forward_duty: start lightness duty in total interval, value range is 0-100
 *            for example, intreval is 1000ms, duty is 60, then start lightness will lighten
 *            600ms, end lightness will lighten 400ms
 * @param[in] times: breath times
 * @param[in] half_breath_end: whether the last breath is half or not, if TURE, lightness will stay
 *            on lightness_end, otherwise lightness till stay on lightness_begin
 * @param[in] cb: light change done callback function
 */
void light_breath(light_t *light, uint16_t lightness_begin, uint16_t lightness_end,
                  uint32_t interval, uint8_t forward_duty, uint32_t times, bool half_breath_end,
                  light_change_done_cb cb);

/**
 * @brief stop light action
 * @param[in] light: light channel
 */
void light_stop(light_t *light);

/**
 * @brief check whether light controller is busy or not
 * @retval TRUE: busy
 * @retval FALSE: idle
 */
bool is_light_controller_busy(void);

/**
 * @brief check whether light is busy or not
 * @retval TRUE: busy
 * @retval FALSE: idle
 */
bool is_light_busy(const light_t *light);

/** @} */
/** @} */
END_DECLS

#endif /** _LIGHT_CONTROLLER_APP_H_ */


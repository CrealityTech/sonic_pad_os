/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      light_lightness_server_app.h
* @brief     Smart mesh light demo lightness application
* @details
* @author    hector
* @date      2018-08-20
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _LIGHT_LIGHTNESS_SERVER_APP_H
#define _LIGHT_LIGHTNESS_SERVER_APP_H

#include "platform_types.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_LIGHTNESS_SERVER_APP
 * @{
 */

/**
 * @defgroup Light_Lightness_Server_Exported_Functions Light Lightness Server Exported Functions
 * @brief
 * @{
 */
/**
 * @brief initialize lightness light server models
 * @param[in] element_index: model element index
 */
void light_lightness_server_models_init(uint8_t element_index);
/** @} */
/** @} */

END_DECLS

#endif /** _LIGHT_LIGHTNESS_SERVER_APP_H */


/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      light_hsl_server_app.h
* @brief     Smart mesh light demo hsl application
* @details
* @author    bill
* @date      2018-1-4
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _LIGHT_HSL_SERVER_APP_H
#define _LIGHT_HSL_SERVER_APP_H

#include "platform_types.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_HSL_SERVER_APP
 * @{
 */

/**
 * @defgroup Light_Hsl_Server_Exported_Functions Light HSL Server Exported Functions
 * @brief
 * @{
 */
/**
 * @brief initialize hsl light server models
 * @param[in] element_index: model element index
 */
void light_hsl_server_models_init(uint8_t element_index);
/** @} */
/** @} */

END_DECLS

#endif /** _LIGHT_HSL_SERVER_APP_H */

/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      light_ctl_hsl_server_app.h
* @brief     Smart mesh light demo ctl & hsl application
* @details
* @author    hector_huang
* @date      2018-08-16
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _LIGHT_CTL_HSL_SERVER_APP_H
#define _LIGHT_CTL_HSL_SERVER_APP_H

#include "platform_types.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_CTL_HSL_SERVER_APP
 * @{
 */

/**
 * @defgroup Light_Ctl_Hsl_Server_Exported_Functions Light CTL HSL Server Exported Functions
 * @brief
 * @{
 */
/**
 * @brief initialize ctl and hsl light server models
 * @param[in] element_index: model element index
 */
void light_ctl_hsl_server_models_init(uint8_t element_index);
/** @} */
/** @} */

END_DECLS

#endif /* _LIGHT_CTL_SERVER_APP_H */


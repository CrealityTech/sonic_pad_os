/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      light_ctl_server_app.h
* @brief     Smart mesh light demo ctl application
* @details
* @author    hector_huang
* @date      2018-08-16
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _LIGHT_CTL_SERVER_APP_H
#define _LIGHT_CTL_SERVER_APP_H

#include "platform_types.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_CTL_SERVER_APP
 * @{
 */

/**
 * @defgroup Light_Ctl_Server_Exported_Functions Light CTL Server Exported Functions
 * @brief
 * @{
 */
/**
 * @brief initialize ctl light server models
 * @param[in] element_index: model element index
 */
void light_ctl_server_models_init(uint8_t element_index);

/**
 * @brief self subscribe
 */
void light_ctl_server_models_sub(void);

/** @} */
/** @} */

END_DECLS

#endif /* _LIGHT_CTL_SERVER_APP_H */


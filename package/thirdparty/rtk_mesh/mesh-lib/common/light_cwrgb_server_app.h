/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      light_cwrgb_server_app.h
* @brief     Smart mesh light demo cwrgb application
* @details
* @author    bill
* @date      2018-1-4
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _LIGHT_CWRGB_MODELS_H
#define _LIGHT_CWRGB_MODELS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mesh_api.h"

/**
 * @addtogroup LIGHT_CWRGB_SERVER_APP
 * @{
 */

/**
 * @defgroup Light_CWRGB_Server_Exported_Functions Light CWRGB Server Exported Functions
 * @brief
 * @{
 */
/**
 * @brief initialize cwrgb light server models
 */
void light_cwrgb_server_models_init(void);
/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _LIGHT_CWRGB_MODELS_H */

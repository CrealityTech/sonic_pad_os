/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     light_effect_app.h
* @brief    Head file for dimmable light application.
* @details  Data structs and external functions declaration.
* @author   hector_huang
* @date     2018-11-19
* @version  v0.1
* *************************************************************************************
*/

#ifndef _LIGHT_EFFECT_APP_H
#define _LIGHT_EFFECT_APP_H

#include "platform_types.h"
#include "light_config.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_EFFECT_APP
 * @{
 */

/**
 * @defgroup Light_Effect_Exported_Functions Light Effect Exported Functions
 * @brief
 * @{
 */

/**
 * @brief gap disconnect light effect
 */
void light_gap_state_disconnect(void);

/**
 * @brief gap connect light effect
 */
void light_gap_state_connected(void);

/**
 * @brief provision link open light effect
 */
void light_prov_link_open(void);

/**
 * @brief provision link close light effect
 */
void light_prov_link_closed(void);

/**
 * @brief node unprovision light effect
 */
void light_prov_unprov(void);

/**
 * @brief provisione start light effect
 */
void light_prov_start(void);

/**
 * @brief provisione complete light effect
 */
void light_prov_complete(void);

/**
 * @brief dft client start light effect
 */
void light_dfu_client_start(void);

/**
 * @brief dfu client end light effect
 */
void light_dfu_client_end(void);

/**
 * @brief dfu client fail light effect
 */
void light_dfu_client_fail(void);

/**
 * @brief dfu server start light effect
 */
void light_dfu_server_start(void);

/**
 * @brief dfu server end light effect
 */
void light_dfu_server_end(void);

/**
 * @brief dfu server fail light effect
 */
void light_dfu_server_fail(void);

#if MESH_ALI_CERTIFICATION
void light_set_unprov_effect(bool flag);
#endif
/** @} */
/** @} */


END_DECLS

#endif /** _LIGHT_EFFECT_APP_H_ */


/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     light_storage_app.h
* @brief    Head file for dimmable light falsh storage.
* @details  Data structs and external functions declaration.
* @author   hector_huang
* @date     2018-11-16
* @version  v0.1
* *************************************************************************************
*/

#ifndef _LIGHT_STORAGE_APP_H_
#define _LIGHT_STORAGE_APP_H_

#include "platform_types.h"

BEGIN_DECLS

/**
 * @addtogroup LIGHT_STORAGE_APP
 * @{
 */

/**
 * @defgroup Light_Storage_Exported_Functions Light Storage Exported Functions
 * @brief
 * @{
 */

/**
 * @brief restore light data from flash
 * @retval TRUE: restore success
 * @retval FALSE: restore fail
 */
bool light_flash_restore(void);

/**
 * @brief store light data to flash
 * @retval TRUE: store success
 * @retval FALSE: store fail
 */
bool light_state_store(void);

/**
 * @brief store user data to flash
 * @retval TRUE: store success
 * @retval FALSE: store fail
 */
bool light_user_data_store(void);
/** @} */
/** @} */

END_DECLS

#endif /** _LIGHT_STORAGE_APP_H_ */



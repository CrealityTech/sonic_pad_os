/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_sdk.h
  * @brief    Head file for the common part used by the mesh sdk.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2019-4-15
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _MESH_SDK_H
#define _MESH_SDK_H

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "platform_types.h"

/**
 * @addtogroup MESH_SDK
 * @{
 */

/**
 * @defgroup Mesh_SDK_Exported_Macros Exported Macros
 * @brief
 * @{
 */

/** @} */

/**
 * @defgroup Mesh_SDK_Exported_Functions Exported Functions
 * @brief
 * @{
 */

/**
  * @brief print the mesh version
  *        & check the compatibility between the sdk and lib
  * @return the compatibility
  * @retval 0: compatible
  * @retval >0: incompatible due to the lib is old
  * @retval <0: incompatible due to the sdk is old
  */
int mesh_sdk_version(void);
/** @} */
/** @} */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif /* _MESH_SDK_H */

/**
****************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
****************************************************************************************************
* @file      ftl.h
* @brief     flash transport layer is used as abstraction layer for user application to save read/write
*            parameters in flash.
* @note      ftl is dedicate block in flash, only used for save read/write value, and offset here is
*            logical offset which is defined for whole ftl section.If value is only for one time read,
*            refer to fs_load_app_data_8 or other APIs in flash_device.h
* @author    Brenda_li
* @date      2016-12-27
* @version   v1.0
* **************************************************************************************************
*/

#ifndef _FTL_H_
#define _FTL_H_

#include <stdint.h>

#ifdef  __cplusplus
extern  "C" {
#endif  // __cplusplus

/** End of FTL_Exported_Types
    * @}
    */

#define MESH_FLASH_BIN_SIZE_MAX                     15360 //need sync with mesh_flash.h

/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup FTL_Exported_Functions Flash Transport Layer Exported Functions
    * @brief
    * @{
    */

uint32_t ftl_save(const char *section, const char *key, const char *value);

uint32_t ftl_save_bin(void *p_data, uint16_t offset, uint16_t size);

const char* ftl_load(const char *section, const char *key, const char *def_value);

uint32_t ftl_load_bin(void *p_data, uint16_t offset, uint16_t size);

uint32_t ftl_delete(const char *section, const char *key, uint8_t flag);

uint32_t ftl_init(uint32_t u32PageStartAddr, uint8_t pagenum);

void ftl_close(void);

void ftl_flush(void);

/** @} */ /* End of group FTL_Exported_Functions */


/** @} */ /* End of group FTL */

#ifdef  __cplusplus
}
#endif // __cplusplus

#endif // _FTL_H_


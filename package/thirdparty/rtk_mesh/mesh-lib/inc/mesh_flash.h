/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_flash.h
  * @brief    Head file for mesh flash storage.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2017-2-21
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _MESH_FLASH_H
#define _MESH_FLASH_H

/* Add Includes here */
#include "platform_misc.h"
#include "ftl.h"
#include "mesh_api.h"

#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup Mesh_Flash
  * @{
  */

/** @defgroup Mesh_Flash_Exported_Types Exported Types
  * @{
  */

typedef enum
{
    MESH_FLASH_PARAMS_NODE_INFO,
    MESH_FLASH_PARAMS_IV_INDEX,
    MESH_FLASH_PARAMS_IV_UPDATE_FLAG,
    MESH_FLASH_PARAMS_SEQ,
    MESH_FLASH_PARAMS_TTL,
    MESH_FLASH_PARAMS_FEATURES,
    MESH_FLASH_PARAMS_NWK_TX_PARAMS,
    MESH_FLASH_PARAMS_HB_PARAMS,
    MESH_FLASH_PARAMS_DEV_KEY,
    MESH_FLASH_PARAMS_NET_KEY,
    MESH_FLASH_PARAMS_APP_KEY,
    MESH_FLASH_PARAMS_VIR_ADDR,
    MESH_FLASH_PARAMS_RPL,
    MESH_FLASH_PARAMS_RPL_ENTRY,
    MESH_FLASH_PARAMS_RPL_SEQ,
    MESH_FLASH_PARAMS_MODEL_PUB_PARAMS,
    MESH_FLASH_PARAMS_MODEL_APP_KEY_BINDING,
    MESH_FLASH_PARAMS_MODEL_SUBSCRIBE_ADDR
} flash_params_type_t;

typedef struct
{
    uint8_t rpl_loop;
    uint16_t entry_loop;
} rpl_entry_info_t;

/** @} */

/** @defgroup Mesh_Flash_Exported_Functions Exported Functions
  * @{
  */

#define mesh_flash_save(section, key, value)            ftl_save(section, key, value)

#define mesh_flash_load(section, key, default_value)            ftl_load(section, key, default_value)

#define mesh_flash_delete(section, key, flag)						ftl_delete(section, key, flag)

#define mesh_flash_save_bin(pdata, size, offset)            ftl_save_bin(pdata, offset, size)

#define mesh_flash_load_bin(pdata, size, offset)            ftl_load_bin(pdata, offset, size)

/**
  * @brief load the node state from the NVM
  *
  * It can be usde to get the node state in advance of mesh_init.
  * !!!Attention: the API shall be called after all the models have bee registered!!!
  *
  * @return the node state
  */
mesh_node_state_t mesh_flash_restore_node_state(void);
#define mesh_node_state_restore mesh_flash_restore_node_state

///@cond
void mesh_flash_store_old_to_new(void);
void mesh_flash_store(flash_params_type_t param_type, void *param);
void mesh_flash_restore(void);
void mesh_flash_clear(void);
void mesh_flash_flush(void);
///@endcond

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif


#endif /* _MESH_FLASH_H */



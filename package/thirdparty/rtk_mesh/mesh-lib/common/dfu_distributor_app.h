/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      dfu_distributor_app.h
* @brief     Smart mesh dfu application
* @details
* @author    bill
* @date      2018-6-6
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _DFU_DIST_MODELS_H
#define _DFU_DIST_MODELS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mesh_api.h"
#include "firmware_distribution.h"
#include "firmware_update.h"
#include "object_transfer.h"

/**
 * @addtogroup DFU_DISTRIBUTOR_APP
 * @{
 */

/**
 * @defgroup Dfu_Distributor_Exported_Functions Dfu Distributor Exported Functions
 * @brief
 * @{
 */
void dfu_dist_models_init(void);
bool dfu_dist_start(uint16_t company_id, fw_update_fw_id_t firmware_id, uint8_t object_id[8],
                    uint16_t dst, uint16_t node_list[], uint16_t node_num);
/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _DFU_DIST_MODELS_H */

/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      dfu_updater_app.c
* @brief     Smart mesh dfu application
* @details
* @author    bill
* @date      2018-6-5
* @version   v1.0
* *********************************************************************************************************
*/

#include "dfu_updater_app.h"
#include "mem_config.h"
#include "dfu_flash.h"
#include "flash_device.h"
#include "patch_header_check.h"
#include "user_flash_driver.h"
#include "flash_adv_cfg.h"
#include "user_flash.h"
#include "ftl.h"

#if DFU_UPDATER_SUPPORT_POWER_OFF_GO_ON
typedef struct
{
    uint8_t firmware_id_new[FW_UPDATE_FW_ID_LEN];
    uint8_t object_id[8];
    uint32_t object_size; /**< shall be 4 alligned */
    uint32_t block_size;
    uint16_t current_block_num; /**< shall be 4 alligned */
    uint16_t image_id;
    fw_update_policy_t policy;
    uint8_t padding[3];
} dfu_updater_nvm_info_t;
dfu_updater_nvm_info_t dfu_updater_nvm_info;

void dfu_updater_nvm_load(void)
{
    uint32_t ret = ftl_load(&dfu_updater_nvm_info, DFU_UPDATER_NVM_OFFSET,
                            sizeof(dfu_updater_nvm_info));
    if (ret == 0 && dfu_updater_nvm_info.object_size != 0)
    {
        printi("dfu_updater: power on go on!");
    }
    else
    {
        dfu_updater_nvm_info.object_size = 0;
    }
}

void dfu_updater_nvm_clear(void)
{
    uint32_t ret;
    ret = ftl_load(&dfu_updater_nvm_info.object_size,
                   DFU_UPDATER_NVM_OFFSET + MEMBER_OFFSET(dfu_updater_nvm_info_t, object_size), 4);
    if (ret == 0 && dfu_updater_nvm_info.object_size != 0)
    {
        dfu_updater_nvm_info.object_size = 0;
        ftl_save(&dfu_updater_nvm_info.object_size,
                 DFU_UPDATER_NVM_OFFSET + MEMBER_OFFSET(dfu_updater_nvm_info_t, object_size), 4);
    }
}
#endif

struct
{
    fw_update_server_ctx_t *pfw_ctx;
    obj_transfer_server_ctx_t *pobj_ctx;
    uint16_t image_id;
} dfu_updater_app_ctx;

int32_t fw_update_server_data_cb(const mesh_model_info_p pmodel_info, uint32_t type,
                                 void *pargs)
{
    int32_t ret = 0;
    switch (type)
    {
    case FW_UPD_SERVER_CB_TYPE_PREPARE:
        {
            if (obj_transfer_server_busy())
            {
                printw("dfu_updater: obj transfer server busy!");
                ret = -1;
            }
        }
        break;
    case FW_UPD_SERVER_CB_TYPE_START:
        {
            fw_update_server_ctx_t *pctx = (fw_update_server_ctx_t *)pargs;
            dfu_updater_app_ctx.pfw_ctx = pctx;
            printi("dfu_updater: start ->");
            dprinti(pctx->firmware_id, FW_UPDATE_FW_ID_LEN);
            dprinti(pctx->firmware_id_new, FW_UPDATE_FW_ID_LEN);
#if DFU_UPDATER_SUPPORT_POWER_OFF_GO_ON
            memcpy(dfu_updater_nvm_info.firmware_id_new, pctx->firmware_id_new, FW_UPDATE_FW_ID_LEN);
            dfu_updater_nvm_info.policy = pctx->policy;
#endif
        }
        break;
    case FW_UPD_SERVER_CB_TYPE_ABORT:
        {
            obj_transfer_server_clear();
#if DFU_UPDATER_SUPPORT_POWER_OFF_GO_ON
            dfu_updater_nvm_clear();
#endif
        }
        break;
    case FW_UPD_SERVER_CB_TYPE_APPLY:
        {
            unlock_flash_all();
            flash_lock(FLASH_LOCK_USER_MODE_READ);//signal = os_lock();
            bool check_result = dfu_check_checksum(dfu_updater_app_ctx.image_id);
            flash_unlock(FLASH_LOCK_USER_MODE_READ);
            lock_flash();
#if DFU_UPDATER_SUPPORT_POWER_OFF_GO_ON
            dfu_updater_nvm_clear();
#endif
            if (false == check_result)
            {
                printw("dfu_updater: verify fail!");
                ret = -1;
            }
            else
            {
                DBG_DIRECT("dfu success, reboot!");
                unlock_flash_all();
                mesh_reboot(MESH_OTA, 1000);
            }
        }
        break;
    default:
        break;
    }
    return ret;
}

int32_t obj_transfer_server_data_cb(const mesh_model_info_p pmodel_info, uint32_t type,
                                    void *pargs)
{
    int32_t ret = 0;
    switch (type)
    {
    case OBJ_TRANSFER_SERVER_CB_TYPE_DATA:
        {
            obj_transfer_server_ctx_t *pctx = (obj_transfer_server_ctx_t *)pargs;
            if (pctx->current_block_num == 0)
            {
                dfu_updater_app_ctx.pobj_ctx = pctx;
                if (dfu_updater_app_ctx.pfw_ctx ||
                    0 != memcmp(dfu_updater_app_ctx.pfw_ctx->object_id, dfu_updater_app_ctx.pobj_ctx->object_id, 8))
                {
                    printe("dfu_updater: fail, object id isn't equal, pfw_ctx = %d!", dfu_updater_app_ctx.pfw_ctx);
                }
                T_IMG_HEADER_FORMAT *pimage_header = (T_IMG_HEADER_FORMAT *)pctx->block_data;
                dfu_updater_app_ctx.image_id = pimage_header->ctrl_header.image_id;
#if DFU_UPDATER_SUPPORT_POWER_OFF_GO_ON
                memcpy(dfu_updater_nvm_info.object_id, pctx->object_id, 8);
                dfu_updater_nvm_info.object_size = pctx->object_size;
                dfu_updater_nvm_info.block_size = pctx->block_size;
                dfu_updater_nvm_info.current_block_num = 1;
                dfu_updater_nvm_info.image_id = dfu_updater_app_ctx.image_id;
                ftl_save(&dfu_updater_nvm_info, DFU_UPDATER_NVM_OFFSET, sizeof(dfu_updater_nvm_info));
#endif
            }
            else
            {
#if DFU_UPDATER_SUPPORT_POWER_OFF_GO_ON
                dfu_updater_nvm_info.current_block_num = pctx->current_block_num + 1;
                ftl_save(&dfu_updater_nvm_info.current_block_num,
                         DFU_UPDATER_NVM_OFFSET + MEMBER_OFFSET(dfu_updater_nvm_info_t, current_block_num), 4);
#endif
            }
            unlock_flash_all();
            if (sil_dfu_update(dfu_updater_app_ctx.image_id, pctx->current_block_num * pctx->block_size,
                               pctx->current_block_size,
                               (uint32_t *)pctx->block_data) != 0)
            {
                /* something wrong */
            }
            lock_flash();
            printi("dfu_updater: %d%%", (pctx->current_block_num + 1) * 100 / pctx->block_num);
            if (pctx->current_block_num == pctx->block_num - 1)
            {
                fw_update_server_ready();
            }
        }
        break;
    case OBJ_TRANSFER_SERVER_CB_TYPE_ABORT:
        {
#if DFU_UPDATER_SUPPORT_POWER_OFF_GO_ON
            dfu_updater_nvm_clear();
#endif
        }
        break;
    default:
        break;
    }
    return ret;
}

#if DFU_UPDATER_SUPPORT_POWER_OFF_GO_ON
void dfu_updater_load(void)
{
    if (UNPROV_DEVICE == mesh_node_state_restore())
    {
        dfu_updater_nvm_clear();
    }
    else
    {
        dfu_updater_nvm_load();
        if (dfu_updater_nvm_info.object_size != 0)
        {
            dfu_updater_app_ctx.image_id = dfu_updater_nvm_info.image_id;
            fw_update_server_load(dfu_updater_nvm_info.object_id, dfu_updater_nvm_info.firmware_id_new,
                                  dfu_updater_nvm_info.policy);
            obj_transfer_server_load(dfu_updater_nvm_info.object_id, dfu_updater_nvm_info.object_size,
                                     dfu_updater_nvm_info.block_size, dfu_updater_nvm_info.current_block_num);
        }
    }
}

void dfu_updater_clear(void)
{
    dfu_updater_nvm_clear();
    fw_update_server_clear();
    obj_transfer_server_clear();
}
#endif

void dfu_updater_models_init(void)
{
    uint8_t firmware_id[FW_UPDATE_FW_ID_LEN] = {DFU_UPDATER_FW_ID};
    fw_update_server_reg(COMPANY_ID, firmware_id, fw_update_server_data_cb);
    obj_transfer_server_reg(OTA_TMP_SIZE, obj_transfer_server_data_cb);
}


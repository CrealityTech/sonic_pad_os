/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ais_server.c
  * @brief    Source file for ais server.
  * @details
  * @author   bill
  * @date     2018-12-4
  * @version  v1.0
  * *************************************************************************************
  */

#define MM_ID MM_SERVICE

#include "platform_misc.h"
#include "platform_diagnose.h"
#include "gap_scheduler.h"
#include "mesh_api.h"
#include "gap.h"
#include "profile_server.h"
#include "ais.h"
#include "mem_config.h"
#include "dfu_flash.h"
#include "flash_device.h"
#include "patch_header_check.h"
#include "user_flash_driver.h"
#include "flash_adv_cfg.h"
#include "user_flash.h"
#include "app_msg.h"

/** @brief  Index of each characteristic in service database. */
#define AIS_READ_INDEX                          0x02
#define AIS_WRITE_INDEX                         0x04
#define AIS_INDICATE_INDEX                      0x06
#define AIS_INDICATE_CCCD_INDEX                 0x07
#define AIS_WRITE_WO_RESP_INDEX                 0x09
#define AIS_NOTIFY_INDEX                        0x0B
#define AIS_NOTIFY_CCCD_INDEX                   0x0C

#define AIS_DEMO    0
#if AIS_DEMO
static const uint8_t ais_read[] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef};
#endif

static const uint16_t ais_image_id[AIS_IMAGE_TYPE_MAX] = {AppPatch, RomPatch};
struct
{
    struct
    {
        uint16_t crc16;
        uint8_t frame_seq: 4;
        uint8_t frame_num: 4;
        uint16_t image_id;
        uint32_t image_size;
        uint32_t image_ver;
        uint32_t rx_size;
        uint8_t frame_remainder_len;
        uint8_t frame_remainder[4];
    } ota;
    plt_timer_t timer;
} ais_server_ctx;

uint8_t ais_server_id;

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
static P_FUN_SERVER_GENERAL_CB ais_server_app_cb = NULL;

bool ais_server_notify(uint8_t conn_id, uint8_t *pvalue, uint16_t len);
bool ais_server_indicate(uint8_t conn_id, uint8_t *pvalue, uint16_t len);

/** @brief  profile/service definition.  */
const T_ATTRIB_APPL ais_server_table[] =
{
    /* <<Primary Service>>, Mesh Provision Service */
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),  /* wFlags     */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(AIS_SERVICE_UUID),      /* service UUID */
            HI_WORD(AIS_SERVICE_UUID)
        },
        UUID_16BIT_SIZE,                            /* bValueLen     */
        NULL,                                       /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },
    /* <<Characteristic>>, read */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ             /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* value */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(AIS_READ_UUID),
            HI_WORD(AIS_READ_UUID)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_READ                             /* wPermissions */
    },
    /* <<Characteristic>>, Write */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* value */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(AIS_WRITE_UUID),
            HI_WORD(AIS_WRITE_UUID)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_WRITE                              /* wPermissions */
    },
    /* <<Characteristic>>, Indicate */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_INDICATE                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* value */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(AIS_INDICATE_UUID),
            HI_WORD(AIS_INDICATE_UUID)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_NONE                              /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
    /* <<Characteristic>>, Write wo resp */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE_NO_RSP                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* value */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(AIS_WRITE_WO_RESP_UUID),
            HI_WORD(AIS_WRITE_WO_RESP_UUID)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_WRITE                              /* wPermissions */
    },
    /* <<Characteristic>>, Notify */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_NOTIFY                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* value */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(AIS_NOTIFY_UUID),
            HI_WORD(AIS_NOTIFY_UUID)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_NONE                              /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    }
};

static void ais_server_timeout_cb(void *ptimer)
{
    T_IO_MSG msg;
    msg.type = AIS_SERVER_TIMEOUT_MSG;
	
	struct timer_wrapper *timer =  (struct timer_wrapper *)ptimer;
	if((timer!=NULL)&&(!timer->reload) )
	{
		timer->state = false;
	}
	
	gap_send_msg_to_app(&msg);
}

void ais_server_timer_start(void)
{
    if (ais_server_ctx.timer == NULL)
    {
        ais_server_ctx.timer = plt_timer_create("ais", AIS_SERVER_ADV_PERIOD, true, 0,
                                                ais_server_timeout_cb);
    }

    if (ais_server_ctx.timer == NULL)
    {
        printe("dfu_server_timer_start: fail, timer is not created");
    }
    else
    {
        plt_timer_start(ais_server_ctx.timer, 0);
    }
}

void ais_server_timer_stop(void)
{
    if (ais_server_ctx.timer != NULL)
    {
        plt_timer_delete(ais_server_ctx.timer, 0);
        ais_server_ctx.timer = NULL;
    }
}

/**
 * @brief send ais advertising
 *
 * @return none
 */
void ais_server_adv(void)
{
    uint8_t *pbuffer;
    if (gap_sched_link_check())
    {
        return;
    }

    pbuffer = gap_sched_task_get();
    if (pbuffer == NULL)
    {
        return;
    }
    pbuffer[0] = 0x02;
    pbuffer[1] = GAP_ADTYPE_FLAGS;
    pbuffer[2] = GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED;
    pbuffer[3] = 0x3;
    pbuffer[4] = GAP_ADTYPE_16BIT_MORE;
    pbuffer[5] = BYTE0(AIS_SERVICE_UUID);
    pbuffer[6] = BYTE1(AIS_SERVICE_UUID);
    pbuffer[7] = 15;
    pbuffer[8] = GAP_ADTYPE_MANUFACTURER_SPECIFIC;
    pbuffer[9] = 0xA8;
    pbuffer[10] = 0x01;
    ais_adv_data_t *padv_data = (ais_adv_data_t *)(pbuffer + 11);
    padv_data->vid = 0x85;
    padv_data->_fmsk.bt_ver = AIS_BT_VER_BLE4_2;
    padv_data->_fmsk.ota = 1;
    padv_data->_fmsk.secure = 0;
    padv_data->_fmsk.enc = 1;
    padv_data->_fmsk.adv = 0;
    padv_data->_fmsk.rfu = 0;
    padv_data->pid = ALI_PRODUCT_ID;
    uint8_t bt_addr[6];
    gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
    memcpy(padv_data->mac, bt_addr, 6);
    printi("ais_server_adv: send");
    bearer_send(BEARER_PKT_TYPE_GATT_SERVICE_ADV, pbuffer, 3 + 4 + 16);
}

bool ais_server_send_device_report(uint8_t conn_id, ais_pdu_t *pmsg, uint16_t len)
{
    pmsg->header.cmd = AIS_DEVICE_REPORT;
    pmsg->header.msg_id = 0;
    pmsg->header.enc = 0;
    pmsg->header.ver = 0;
    pmsg->header.frame_seq = 0;
    pmsg->header.frame_num = 0;
    pmsg->header.frame_len = len - sizeof(ais_header_t);
    return ais_server_indicate(conn_id, (uint8_t *)pmsg, len);
}

bool ais_server_send_device_resp(uint8_t conn_id, ais_pdu_t *pmsg, uint16_t len, uint8_t msg_id)
{
    pmsg->header.cmd = AIS_DEVICE_RESP;
    pmsg->header.msg_id = msg_id;
    pmsg->header.enc = 0;
    pmsg->header.ver = 0;
    pmsg->header.frame_seq = 0;
    pmsg->header.frame_num = 0;
    pmsg->header.frame_len = len - sizeof(ais_header_t);
    return ais_server_indicate(conn_id, (uint8_t *)pmsg, len);
}

bool ais_server_send_device_exception(uint8_t conn_id, uint8_t msg_id)
{
    ais_pdu_t msg;
    msg.header.msg_id = msg_id;
    msg.header.enc = 0;
    msg.header.ver = 0;
    msg.header.cmd = AIS_DEVICE_EXCEPTION;
    msg.header.frame_seq = 0;
    msg.header.frame_num = 0;
    msg.header.frame_len = 0;
    return ais_server_indicate(conn_id, (uint8_t *)&msg, sizeof(ais_header_t));
}

bool ais_server_send_ota_msg(uint8_t conn_id, ais_pdu_t *pmsg, uint16_t len)
{
    pmsg->header.msg_id = 0;
    pmsg->header.enc = 0;
    pmsg->header.ver = 0;
    pmsg->header.frame_seq = 0;
    pmsg->header.frame_num = 0;
    pmsg->header.frame_len = len - sizeof(ais_header_t);
    return ais_server_notify(conn_id, (uint8_t *)pmsg, len);
}

uint32_t ais_server_get_image_ver(ais_image_type_t image_type)
{
    uint32_t ver;
    switch (image_type)
    {
    case AIS_IMAGE_TYPE_APP:
        ver = ALI_VERSION_ID;
        break;
    case AIS_IMAGE_TYPE_PATCH:
        ver = DFU_PATCH_VERSION;
        break;
    default:
        ver = 0xffffffff;
        break;
    }
    return ver;
}

void ais_server_handle_msg(uint8_t conn_id, ais_pdu_t *pmsg, uint16_t len)
{
    bool ret =  false;
    ais_pdu_t resp;
    printi("ais_server_handle_msg: image id 0x%04x, size %d/%d, remain %d, expected frame seq %d, conn_id %d, cmd 0x%x, len %d, pmsg =",
           ais_server_ctx.ota.image_id, ais_server_ctx.ota.rx_size, ais_server_ctx.ota.image_size,
           ais_server_ctx.ota.frame_remainder_len, ais_server_ctx.ota.frame_seq, conn_id, pmsg->header.cmd,
           len);
    dprinti((uint8_t *)pmsg, len);
    switch (pmsg->header.cmd)
    {
    case AIS_OTA_GET_VER:
        if (len == sizeof(ais_header_t) + sizeof(ais_ota_get_ver_t))
        {
            ret = true;
            resp.header.cmd = AIS_OTA_REPORT_VER;
            resp.ota_report_ver.image_type = pmsg->ota_get_ver.image_type;
            resp.ota_report_ver.ver = ais_server_get_image_ver(pmsg->ota_get_ver.image_type);
            ais_server_send_ota_msg(conn_id, &resp, sizeof(ais_header_t) + sizeof(ais_ota_report_ver_t));
        }
        break;
    case AIS_OTA_UPD_REQ:
        if (len == sizeof(ais_header_t) + sizeof(ais_ota_upd_req_t))
        {
            ret = true;
            if (pmsg->ota_upd_req.ver <= ais_server_get_image_ver(pmsg->ota_upd_req.image_type) ||
                OTA_TMP_SIZE < pmsg->ota_upd_req.fw_size || 0 == pmsg->ota_upd_req.fw_size
                || (pmsg->ota_upd_req.fw_size & 0x3) != 0)
            {
                resp.ota_upd_resp.state = 0;
                resp.ota_upd_resp.rx_size = 0;
            }
            else
            {
                if (pmsg->ota_upd_req.ota_type == AIS_OTA_TYPE_FULL)
                {
                    ais_server_ctx.ota.rx_size = 0;
                }
                else if (ais_server_ctx.ota.rx_size != 0)
                {
                    if (ais_image_id[pmsg->ota_upd_req.image_type] != ais_server_ctx.ota.image_id ||
                        pmsg->ota_upd_req.ver != ais_server_ctx.ota.image_ver ||
                        pmsg->ota_upd_req.fw_size != ais_server_ctx.ota.image_size ||
                        pmsg->ota_upd_req.crc16 != ais_server_ctx.ota.crc16)
                    {
                        ais_server_ctx.ota.rx_size = 0;
                    }
                }

                if (ais_server_ctx.ota.rx_size == 0)
                {
                    ais_server_ctx.ota.image_id = ais_image_id[pmsg->ota_upd_req.image_type];
                    ais_server_ctx.ota.image_size = pmsg->ota_upd_req.fw_size;
                    ais_server_ctx.ota.crc16 = pmsg->ota_upd_req.crc16;
                    ais_server_ctx.ota.image_ver = pmsg->ota_upd_req.ver;
                    ais_server_ctx.ota.frame_remainder_len = 0;
                }
                ais_server_ctx.ota.frame_seq = 0;
                ais_server_ctx.ota.frame_num = 0;
                resp.ota_upd_resp.state = 1;
                resp.ota_upd_resp.rx_size = ais_server_ctx.ota.rx_size;
                if (ais_server_app_cb)
                {
                    ais_cb_msg_t cb_msg = {conn_id, AIS_CB_OTA, {.ota = {.state = AIS_OTA_START}}};
                    ais_server_app_cb(ais_server_id, &cb_msg);
                }
            }
            resp.header.cmd = AIS_OTA_UPD_RESP;
            ais_server_send_ota_msg(conn_id, &resp, sizeof(ais_header_t) + sizeof(ais_ota_upd_resp_t));
        }
        break;
    case AIS_OTA_FW_INFO_REQ:
        if (len == sizeof(ais_header_t) + sizeof(ais_ota_fw_info_req_t))
        {
            ret = true;
            resp.header.cmd = AIS_OTA_FW_INFO;
            resp.ota_fw_info.state = 0;
            if (pmsg->ota_fw_info_req.state == 1)
            {
                // TODO: calculate the fw crc first
                if (ais_server_ctx.ota.image_size == ais_server_ctx.ota.rx_size)
                {
                    unlock_flash_all();
                    flash_lock(FLASH_LOCK_USER_MODE_READ);
                    resp.ota_fw_info.state = dfu_check_checksum(ais_server_ctx.ota.image_id);
                    flash_unlock(FLASH_LOCK_USER_MODE_READ);
                    lock_flash();
                }
            }
            ais_server_send_ota_msg(conn_id, &resp, sizeof(ais_header_t) + sizeof(ais_ota_fw_info_t));
            if (ais_server_app_cb)
            {
                ais_cb_msg_t cb_msg = {conn_id, AIS_CB_OTA, {.ota = {.state = resp.ota_fw_info.state ? AIS_OTA_SUCCESS : AIS_OTA_FAIL}}};
                ais_server_app_cb(ais_server_id, &cb_msg);
            }
            if (resp.ota_fw_info.state)
            {
                DBG_DIRECT("dfu success, reboot!");
                unlock_flash_all();
                mesh_reboot(MESH_OTA, 1000);
            }
        }
        break;
    case AIS_OTA_FW_DATA:
        {
            if (pmsg->header.frame_seq != ais_server_ctx.ota.frame_seq)
            {
                printw("ais_server_handle_msg: fail, frame seq expected %d, rx %d", ais_server_ctx.ota.frame_seq,
                       pmsg->header.frame_seq);
            }
            else if (pmsg->header.frame_len + sizeof(ais_header_t) != len)
            {
                printw("ais_server_handle_msg: fail, frame len declared %d, rx %d", pmsg->header.frame_len,
                       len - sizeof(ais_header_t));
            }
            else
            {
                uint8_t offset = 0;
                uint8_t *payload = pmsg->payload;
                uint16_t payload_len = pmsg->header.frame_len;
                ais_server_ctx.ota.frame_seq = (pmsg->header.frame_seq + 1) % 16;
                ais_server_ctx.ota.frame_num = pmsg->header.frame_num;
                if (ais_server_ctx.ota.rx_size == 0)
                {
                    T_IMG_HEADER_FORMAT *pimage_header = (T_IMG_HEADER_FORMAT *)pmsg->payload;
                    if (ais_server_ctx.ota.image_id != pimage_header->ctrl_header.image_id)
                    {
                        printe("ais_server_handle_msg: fail, image wrong!");
                        break;
                    }
                }

                if (ais_server_ctx.ota.frame_remainder_len)
                {
                    memcpy(ais_server_ctx.ota.frame_remainder + ais_server_ctx.ota.frame_remainder_len, pmsg->payload,
                           4 - ais_server_ctx.ota.frame_remainder_len);
                    unlock_flash_all();
                    sil_dfu_update(ais_server_ctx.ota.image_id,
                                   ais_server_ctx.ota.rx_size - ais_server_ctx.ota.frame_remainder_len, 4,
                                   (uint32_t *)ais_server_ctx.ota.frame_remainder);
                    lock_flash();
                    offset = 4 - ais_server_ctx.ota.frame_remainder_len;
                    payload += offset;
                    payload_len -= MIN(payload_len, offset);
                }
                ais_server_ctx.ota.frame_remainder_len = payload_len % 4;
                payload_len = payload_len & 0xfffc;
                if (payload_len)
                {
                    unlock_flash_all();
                    sil_dfu_update(ais_server_ctx.ota.image_id, ais_server_ctx.ota.rx_size + offset, payload_len,
                                   (uint32_t *)payload);
                    lock_flash();
                }

                if (ais_server_ctx.ota.frame_remainder_len)
                {
                    memcpy(ais_server_ctx.ota.frame_remainder, payload + payload_len,
                           ais_server_ctx.ota.frame_remainder_len);
                }
                ais_server_ctx.ota.rx_size += pmsg->header.frame_len;
                if (ais_server_app_cb)
                {
                    ais_cb_msg_t cb_msg = {conn_id, AIS_CB_OTA, {.ota = {.state = AIS_OTA_GOING, .progress = ais_server_ctx.ota.rx_size * 100 / ais_server_ctx.ota.image_size}}};
                    ais_server_app_cb(ais_server_id, &cb_msg);
                }
            }
            ret = true;
            resp.header.cmd = AIS_OTA_FRAME_INFO;
            resp.ota_frame_info.frame_seq = (ais_server_ctx.ota.frame_seq + 15) % 16;
            resp.ota_frame_info.frame_num = ais_server_ctx.ota.frame_num;
            resp.ota_frame_info.rx_size = ais_server_ctx.ota.rx_size;
            ais_server_send_ota_msg(conn_id, &resp, sizeof(ais_header_t) + sizeof(ais_ota_frame_info_t));
        }
        break;
    default:
        break;
    }
    if (false == ret)
    {
        ais_server_send_device_exception(conn_id, pmsg->header.msg_id);
    }
}

/**
 * @brief read characteristic data from service.
 *
 * @param ServiceID          ServiceID to be read.
 * @param index          Attribute index of characteristic.
 * @param offset            length of value to be written.
 * @param p_length            length of value to be read.
 * @param pp_value            value to be read.
 * @return Profile procedure result
 */
T_APP_RESULT ais_server_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT  cause = APP_RESULT_SUCCESS;
    switch (attrib_index)
    {
    case AIS_READ_INDEX:
        {
#if AIS_DEMO
            *pp_value = (uint8_t *)ais_read;
            *p_length = sizeof(ais_read);
#else
            if (ais_server_app_cb)
            {
                ais_cb_msg_t cb_msg = {conn_id, AIS_CB_READ, {.read = {.p_length = p_length, .pp_value = pp_value}}};
                ais_server_app_cb(service_id, &cb_msg);
            }
#endif
        }
        break;
    default:
        cause = APP_RESULT_ATTR_NOT_FOUND;
        break;
    }
    return cause;
}

/**
 * @brief write characteristic data from service.
 *
 * @param ServiceID          ServiceID to be written.
 * @param index          Attribute index of characteristic.
 * @param len            length of value to be written.
 * @param pvalue            value to be written.
 * @return Profile procedure result
 */
T_APP_RESULT ais_server_write_cb(uint8_t conn_id, T_SERVER_ID server_id, uint16_t index,
                                 T_WRITE_TYPE write_type, uint16_t len, uint8_t *pvalue, P_FUN_WRITE_IND_POST_PROC *ppost_proc)
{
    T_APP_RESULT  cause = APP_RESULT_SUCCESS;
    switch (index)
    {
    case AIS_WRITE_INDEX:
        {
#if AIS_DEMO
            ais_pdu_t *pmsg = (ais_pdu_t *)pvalue;
            ais_server_send_device_resp(conn_id, pmsg, len, pmsg->header.msg_id);
#else
            if (ais_server_app_cb)
            {
                ais_cb_msg_t cb_msg = {conn_id, AIS_CB_WRITE_REQ, {.write_req = {.pmsg = (ais_pdu_t *)pvalue, .msg_len = len}}};
                ais_server_app_cb(server_id, &cb_msg);
            }
#endif
        }
        break;
    case AIS_WRITE_WO_RESP_INDEX:
        ais_server_handle_msg(conn_id, (ais_pdu_t *)pvalue, len);
        break;
    default:
        printe("ais_server_write_cb: fail, index = 0x%x len=%d", index, len);
        cause = APP_RESULT_ATTR_NOT_FOUND;
        break;
    }

    return cause;
}

bool ais_server_notify(uint8_t conn_id, uint8_t *pvalue, uint16_t len)
{
    ais_pdu_t *pmsg = (ais_pdu_t *)pvalue;
    printi("ais_server_notify: cmd 0x%x, len %d", pmsg->header.cmd, len);
    dprinti(pvalue, len);
    return server_send_data(conn_id, ais_server_id, AIS_NOTIFY_INDEX, pvalue, len,
                            GATT_PDU_TYPE_ANY);
}

bool ais_server_indicate(uint8_t conn_id, uint8_t *pvalue, uint16_t len)
{
    ais_pdu_t *pmsg = (ais_pdu_t *)pvalue;
    printi("ais_server_indicate: cmd 0x%x, len %d", pmsg->header.cmd, len);
    dprinti(pvalue, len);
    return server_send_data(conn_id, ais_server_id, AIS_INDICATE_INDEX, pvalue, len,
                            GATT_PDU_TYPE_ANY);
}

/**
 * @brief update CCCD bits from stack.
 *
 * @param server_id          Service ID.
 * @param index          Attribute index of characteristic data.
 * @param cccd_bits         CCCD bits from stack.
 * @return None
 */
void ais_server_cccd_update_cb(uint8_t conn_id, T_SERVER_ID server_id, uint16_t index,
                               uint16_t cccd_bits)
{
    printi("ais_server_cccd_update_cb: index = %d, cccd_bits = 0x%x", index, cccd_bits);
    switch (index)
    {
    case AIS_INDICATE_CCCD_INDEX:
        break;
    case AIS_NOTIFY_CCCD_INDEX:
        break;
    default:
        break;
    }
}

/**
 * @brief Service Callbacks.
 */
const T_FUN_GATT_SERVICE_CBS ais_server_cbs =
{
    ais_server_read_cb,  // Read callback function pointer
    ais_server_write_cb, // Write callback function pointer
    ais_server_cccd_update_cb  // CCCD update callback function pointer
};

/**
  * @brief add service to the stack database.
  *
  * @param[in] pFunc          pointer of app callback function called by profile.
  * @return service ID auto generated by profile layer.
  * @retval server_id
  */
uint8_t ais_server_add(void *pcb)
{
    uint8_t server_id;
    if (FALSE == server_add_service(&server_id, (uint8_t *)ais_server_table, sizeof(ais_server_table),
                                    ais_server_cbs))
    {
        printe("ais_server_add: server_id = %d", server_id);
        server_id = 0xff;
        return server_id;
    }
    ais_server_app_cb = (P_FUN_SERVER_GENERAL_CB)pcb;
    ais_server_timer_start();
    ais_server_id = server_id;
    return server_id;
}

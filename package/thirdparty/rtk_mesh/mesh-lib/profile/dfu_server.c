#include <string.h>
#include <gatt.h>
#include <bt_types.h>
#include "trace.h"
#include "gap_le.h"
#include "gap_conn_le.h"
#include "gap_msg.h"
#include "gap_adv.h"
#include "gap_manufacture_adv.h"
#include "app_msg.h"
#include "flash_device.h"
#include "rtl876x_wdg.h"
#include "dfu_api.h"
#include "ota_server.h"
#include "dfu_server.h"
#include "dfu_flash.h"
#include "os_sched.h"
#include "patch_header_check.h"
#include "mesh_api.h"
#include "mem_config.h"
#include "os_sync.h"
#include "platform_utils.h"
#include "flash_device.h"
#include "flash_adv_cfg.h"
#include "user_flash_driver.h"
#include "app_section.h"
#include "otp.h"
#include "user_flash.h"

extern gap_sched_t gap_scheduler;

P_FUN_SERVER_GENERAL_CB pfnDfuExtendedCB = NULL;
uint8_t dfu_server_id;

#define DFU_SERVER_INVALID_CONN_ID 0xff
static uint8_t dfu_server_conn_id = DFU_SERVER_INVALID_CONN_ID;

extern bool gOtaPeriod;
extern bool OtaActRstPending;
bool gOtaValidFlg = false;
plt_timer_t dfu_server_adv_timer;
#define SECTOR_SIZE 0x1000

extern bool allowedRcuEnterDlps;

uint8_t *g_pOtaTempBufferHead;
//uint8_t OtaTempBufferHead[DFU_TEMP_BUFFER_SIZE];
//uint16_t g_OtaTempBufferUsedSize;

/*begin must replace for actual sdk*/
bool gota_with_encryption_data = 0;
/*end must replace for actual sdk*/

bool gSilBufCheckEN = false;
bool gSilFirstPage = false;
uint32_t gSilDfuResendOffset = 0;

uint16_t ota_tmp_buf_used_size;
uint16_t mBufSize;
uint16_t mCrcVal;

void silence_BufferCheckProc(uint16_t _mBufferSize, uint16_t _mCrc);
uint32_t dfu_update(uint16_t signature, uint32_t offset, uint32_t length, void *p_void);
uint32_t dfu_flash_check_blank(uint16_t signature, uint32_t offset, uint16_t nSize);


/* also needed in client*/
dfu_ctx_t dfu_ctx;
const uint8_t dfu_service_uuid[16] = {GATT_UUID128_DFU_SERVICE};

const T_ATTRIB_APPL dfu_service_table[] =
{

    /*-------------------------- DFU Service ---------------------------*/
    /* <<Primary Service>>, .. */
    {
        (ATTRIB_FLAG_VOID | ATTRIB_FLAG_LE),           /* flags     */
        {
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),              /* type_value */
        },
        UUID_128BIT_SIZE,                                    /* bValueLen     */
        (void *)dfu_service_uuid,           /* p_value_context */
        GATT_PERM_READ                                      /* permissions  */
    },



    /* <<Characteristic>>, .. */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* flags */
        {                                           /* type_value */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE_NO_RSP/* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* permissions */
    },
    /*--- DFU packet characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT,                              /* flags */
        {                                                           /* type_value */
            GATT_UUID128_DFU_DATA
        },
        0,                                                 /* bValueLen */
        NULL,
        GATT_PERM_WRITE                 /* permissions */
    },
    /* <<Characteristic>>, .. */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* flags */
        {                                           /* type_value */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_WRITE |                   /* characteristic properties */
             GATT_CHAR_PROP_NOTIFY)
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* permissions */
    },
    /*--- DFU Control Point value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT,                              /* flags */
        {                                                           /* type_value */
            GATT_UUID128_DFU_CONTROL_POINT
        },
        0,                                                 /* bValueLen */
        NULL,
        GATT_PERM_WRITE                 /* permissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* flags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* type_value */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* permissions */
    }

};

bool flstatus = false;//flash lock status
uint32_t signal;

bool aes256_ecb_decrypt_msb2lsb(uint8_t *input, const uint8_t *key, uint8_t *output);
bool dfu_decrypt(uint8_t image[16])
{
    bool ret;
    uint8_t image_tmp[16] = {0};
    plt_swap2(image_tmp, image, 16);
    ret = aes256_ecb_decrypt_msb2lsb(image_tmp, (uint8_t *)OTP->aes_key, image_tmp);
    plt_swap2(image, image_tmp, 16);
    return ret;

}

void dfu_server_adv_send(void)
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
    pbuffer[3] = 1 + MEMBER_OFFSET(manufacture_adv_data_t, data) + sizeof(manufacture_adv_data_dfu_t);
    pbuffer[4] = GAP_ADTYPE_MANUFACTURER_SPECIFIC;
    manufacture_adv_data_t *padv_data = (manufacture_adv_data_t *)(pbuffer + 5);
    padv_data->company_id = MANUFACTURE_ADV_DATA_COMPANY_ID;
    padv_data->type = MANUFACTURE_ADV_DATA_TYPE_DFU;
    padv_data->dfu.product_id = DFU_PRODUCT_ID;
    padv_data->dfu.app_version = DFU_APP_VERSION;
    padv_data->dfu.patch_version = DFU_PATCH_VERSION;
    printi("dfu_server_adv_send: product_id = %d app version = %d patch version = %d",
           padv_data->dfu.product_id, padv_data->dfu.app_version, padv_data->dfu.patch_version);
    bearer_send(BEARER_PKT_TYPE_GATT_SERVICE_ADV, pbuffer, 3 + 2 + MEMBER_OFFSET(manufacture_adv_data_t,
                                                                                 data) + sizeof(manufacture_adv_data_dfu_t));
}

static void dfu_server_timeout_cb(void *ptimer)
{
       T_IO_MSG msg;

       msg.type = DFU_SERVER_TIMEOUT_MSG;
	   gap_send_msg_to_app(&msg);
}

void dfu_server_timer_start(void)
{
    if (dfu_server_adv_timer == NULL)
    {
        dfu_server_adv_timer = plt_timer_create("dfu", DFU_SERVER_ADV_PERIOD, true, 0,
                                                dfu_server_timeout_cb);
    }

    if (dfu_server_adv_timer == NULL)
    {
        printe("dfu_server_timer_start: fail, timer is not created");
    }
    else
    {
        plt_timer_start(dfu_server_adv_timer, 0);
    }
}

void dfu_server_timer_stop(void)
{
    if (dfu_server_adv_timer != NULL)
    {
        plt_timer_delete(dfu_server_adv_timer, 0);
        dfu_server_adv_timer = NULL;
    }
}

/**
 * @brief dfu_server_buffer_check
 *
 * @param _mBufferSize     size for buffer check.
 * @param _mCrc            calced buffer crc value.
 * @return None
*/
void dfu_server_buffer_check(uint16_t _mBufferSize, uint16_t _mCrc)
{
    uint16_t offset = 0;
    uint8_t notif_data[7] = {0};
    notif_data[0] = DFU_OPCODE_NOTIF;
    notif_data[1] = DFU_OPCODE_REPORT_BUFFER_CRC;

    if (mBufSize > DFU_TEMP_BUFFER_SIZE)
    {
        //invalid para
        ota_tmp_buf_used_size = 0;
        notif_data[2] = DFU_ARV_FAIL_INVALID_PARAMETER;
        LE_UINT32_TO_ARRAY(&notif_data[3], dfu_ctx.curr_offset);
        server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                         notif_data, 7, GATT_PDU_TYPE_NOTIFICATION);
        return;
    }
    if (ota_tmp_buf_used_size == mBufSize ||
        dfu_ctx.curr_offset + ota_tmp_buf_used_size == dfu_ctx.image_length)
    {
        if (dfu_checkbufcrc(g_pOtaTempBufferHead, ota_tmp_buf_used_size, _mCrc))     //crc error

        {
            ota_tmp_buf_used_size = 0;
            notif_data[2] = DFU_ARV_FAIL_CRC_ERROR;
            LE_UINT32_TO_ARRAY(&notif_data[3], dfu_ctx.curr_offset);
            server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                             notif_data, 7, GATT_PDU_TYPE_NOTIFICATION);
            return;
        }

        else //crc ok
        {
            if (OTP->ota_with_encryption_data)
            {
                //dfu_init();
                //aes
                do
                {
                    if ((ota_tmp_buf_used_size - offset) >= 16)
                    {
                        dfu_decrypt(g_pOtaTempBufferHead + offset);
                        offset += 16;
                    }
                    else
                    {
                        break;
                    }
                }
                while (1);
            }

            DFU_PRINT_TRACE1(" p_dfu->cur_offset, %x", dfu_ctx.curr_offset);
            unlock_flash_all();
            uint32_t result = sil_dfu_update(dfu_ctx.signature, dfu_ctx.curr_offset, ota_tmp_buf_used_size,
                                             (uint32_t *)g_pOtaTempBufferHead);

            if (result == 0)
            {
                lock_flash();
                dfu_ctx.curr_offset += ota_tmp_buf_used_size;

                if ((dfu_ctx.curr_offset - gSilDfuResendOffset) >= SECTOR_SIZE)
                {
                    gSilDfuResendOffset += SECTOR_SIZE;
                }
                ota_tmp_buf_used_size = 0;
                notif_data[2] = DFU_ARV_SUCCESS; //valid
                LE_UINT32_TO_ARRAY(&notif_data[3], dfu_ctx.curr_offset);
                server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                 notif_data, 7, GATT_PDU_TYPE_NOTIFICATION);
                return;
            }
            else
            {
                uint32_t ret = 0;
                uint32_t cnt = 0;
                do
                {
                    dfu_flash_erase(dfu_ctx.signature, gSilDfuResendOffset);
                    //delay_ms_asm(200);
                    signal = os_lock();
                    ret = dfu_flash_check_blank(dfu_ctx.signature, gSilDfuResendOffset, SECTOR_SIZE);
                    os_unlock(signal);
                    if (ret)
                    {
                        cnt++;
                    }
                    else
                    {
                        break;
                    }
                    if (cnt >= 3)     //check 0xff failed,erase failed
                    {
                        lock_flash();
                        //erase error
                        ota_tmp_buf_used_size = 0;
                        dfu_ctx.curr_offset =  gSilDfuResendOffset;
                        notif_data[2] = DFU_ARV_FAIL_ERASE_ERROR;
                        LE_UINT32_TO_ARRAY(&notif_data[3], dfu_ctx.curr_offset);
                        server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                         notif_data, 7, GATT_PDU_TYPE_NOTIFICATION);
                        return;
                    }
                }
                while (1);

                if ((dfu_ctx.curr_offset - gSilDfuResendOffset) > SECTOR_SIZE) //need erase two sector
                {
                    cnt = 0;
                    do
                    {
                        //erase sector :addr ~ gDfuResendOffset+SECTOR_SIZE;
                        dfu_flash_erase(dfu_ctx.signature, gSilDfuResendOffset + SECTOR_SIZE);
                        signal = os_lock();
                        ret = dfu_flash_check_blank(dfu_ctx.signature, gSilDfuResendOffset + SECTOR_SIZE, SECTOR_SIZE);
                        os_unlock(signal);
                        if (ret)
                        {
                            cnt++;
                        }
                        else
                        {
                            break;
                        }
                        if (cnt >= 3)     //check 0xff failed,erase failed
                        {
                            lock_flash();
                            //erase error
                            ota_tmp_buf_used_size = 0;
                            dfu_ctx.curr_offset =  gSilDfuResendOffset;
                            notif_data[2] = DFU_ARV_FAIL_ERASE_ERROR;
                            LE_UINT32_TO_ARRAY(&notif_data[3], dfu_ctx.curr_offset);
                            server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                             notif_data, 7, GATT_PDU_TYPE_NOTIFICATION);
                            return;
                        }
                    }
                    while (1);
                }
                lock_flash();
                ota_tmp_buf_used_size = 0;
                dfu_ctx.curr_offset =  gSilDfuResendOffset;
                notif_data[2] = DFU_ARV_FAIL_PROG_ERROR;
                LE_UINT32_TO_ARRAY(&notif_data[3], dfu_ctx.curr_offset);
                server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                 notif_data, 7, GATT_PDU_TYPE_NOTIFICATION);
                return;
            }
        }
    }
    else
    {
        DFU_PRINT_TRACE2("ota_temp_buf_used_size is %d,_mBufferSize=%d", ota_tmp_buf_used_size,
                         _mBufferSize);
        //flush buffer.
        ota_tmp_buf_used_size = 0;
        notif_data[2] = DFU_ARV_FAIL_LENGTH_ERROR;
        LE_UINT32_TO_ARRAY(&notif_data[3], dfu_ctx.curr_offset);
        server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                         notif_data, 7, GATT_PDU_TYPE_NOTIFICATION);
        return;
    }
}

/**
 * @brief dfu_server_handle_cp
 *
 * @param length     control point cmd length.
 * @param p_value    control point cmd address..
 * @return None
*/
void  dfu_server_handle_cp(uint16_t length, uint8_t *p_value)
{
    T_DFU_CTRL_POINT dfu_control_point;
    uint8_t notif_data[11] = {0};
    dfu_control_point.opcode = *p_value;
    uint8_t *p = p_value + 1;
    DFU_PRINT_TRACE2("dfu_service_handle_control_point_req: opcode=0x%x, length=%d",
                     dfu_control_point.opcode, length);
    if (dfu_control_point.opcode >= DFU_OPCODE_MAX || dfu_control_point.opcode <= DFU_OPCODE_MIN)
    {
        notif_data[0] = DFU_OPCODE_NOTIF;
        notif_data[1] = dfu_control_point.opcode;
        notif_data[2] = 0xff;
        server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                         notif_data, 3, GATT_PDU_TYPE_NOTIFICATION);
        return;
    }

    switch (dfu_control_point.opcode)
    {
    case DFU_OPCODE_START_DFU:
        if (length == DFU_LENGTH_START_DFU)/*4 bytes is pending for encrypt*/
        {
#if DFU_WO_SCAN
            if (gap_scheduler.bg_scan)
            {
                dfu_ctx.bg_scan = 1;
                gap_sched_scan(0);
            }
#endif

            if (OTP->ota_with_encryption_data)
            {
                //DFU_PRINT_TRACE1("Data before decryped: %b", TRACE_BINARY(16, p));
                dfu_decrypt(p);
                //DFU_PRINT_TRACE1("Data after decryped: %b", TRACE_BINARY(16, p));
            }

            dfu_control_point.p.start_dfu.ic_type = (*p);
            p += 1;
            dfu_control_point.p.start_dfu.secure_version = (*p);
            p += 1;
            LE_ARRAY_TO_UINT16(dfu_control_point.p.start_dfu.ctrl_flag.value, p);
            p += 2;
            LE_ARRAY_TO_UINT16(dfu_control_point.p.start_dfu.signature, p);
            p += 2;
            LE_ARRAY_TO_UINT16(dfu_control_point.p.start_dfu.crc16, p);
            p += 2;
            LE_ARRAY_TO_UINT32(dfu_control_point.p.start_dfu.image_length, p);

            DFU_PRINT_TRACE6("DFU_OPCODE_START_DFU: ic_type=0x%x, secure_version=0x%x, ctrl_flag.value=0x%x, signature=0x%x,crc16=0x%x, image_length=0x%x",
                             dfu_control_point.p.start_dfu.ic_type,
                             dfu_control_point.p.start_dfu.secure_version,
                             dfu_control_point.p.start_dfu.ctrl_flag.value,
                             dfu_control_point.p.start_dfu.signature,
                             dfu_control_point.p.start_dfu.crc16,
                             dfu_control_point.p.start_dfu.image_length
                            );
            dfu_ctx.ic_type = dfu_control_point.p.start_dfu.ic_type;
            dfu_ctx.ctrl_flag.value = dfu_control_point.p.start_dfu.ctrl_flag.value;
            dfu_ctx.signature = dfu_control_point.p.start_dfu.signature;
            // p_dfu->version = dfu_control_point.p.start_dfu.version;
            dfu_ctx.crc16 = dfu_control_point.p.start_dfu.crc16;
            dfu_ctx.image_length = dfu_control_point.p.start_dfu.image_length;

            dfu_ctx.image_length += IMG_HEADER_SIZE;

            data_uart_debug("%s being ota-ed %3d%%", dfu_ctx.signature == AppPatch ? "app" : "patch", 0);
            dfu_ctx.fsm = DFU_CB_START;
            if (pfnDfuExtendedCB)
            {
                dfu_cb_msg_t cb_msg;
                cb_msg.type = DFU_CB_START;
                pfnDfuExtendedCB(dfu_server_id, &cb_msg);
            }

            unlock_flash_all();
            if (sil_dfu_update(dfu_ctx.signature, 0, DFU_HEADER_SIZE,
                               (uint32_t *)&dfu_control_point.p.start_dfu.ic_type) == 0)
            {
                lock_flash();
                dfu_ctx.curr_offset = DFU_HEADER_SIZE;
            }
            else
            {
                if (pfnDfuExtendedCB)
                {
                    dfu_cb_msg_t cb_msg;
                    cb_msg.type = DFU_CB_FAIL;
                    pfnDfuExtendedCB(dfu_server_id, &cb_msg);
                }
                lock_flash();
                dfu_reset(dfu_ctx.signature);
                dfu_fw_active_reset();
            }

            notif_data[0] = DFU_OPCODE_NOTIFICATION;
            notif_data[1] = DFU_OPCODE_START_DFU;
            notif_data[2] = DFU_ARV_SUCCESS;
            server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, notif_data, 3,
                             GATT_PDU_TYPE_NOTIFICATION);
            gSilFirstPage = true;
        }
        break;
    case DFU_OPCODE_RECEIVE_FW_IMAGE_INFO:
        if (length == DFU_LENGTH_RECEIVE_FW_IMAGE_INFO)
        {
            LE_ARRAY_TO_UINT16(dfu_ctx.signature, p);
            p += 2;
            LE_ARRAY_TO_UINT32(dfu_ctx.curr_offset, p);
            if (dfu_ctx.curr_offset == 0 || dfu_ctx.curr_offset == 0xc)
            {
                ota_tmp_buf_used_size = 0;
                gSilDfuResendOffset = 0;
            }
            DFU_PRINT_TRACE2("DFU_OPCODE_RECEIVE_FW_IMAGE_INFO: signature = 0x%x, cur_offset = %d",
                             dfu_ctx.signature, dfu_ctx.curr_offset);
        }
        else
        {
            DFU_PRINT_TRACE0("DFU_OPCODE_RECEIVE_FW_IMAGE_INFO: invalid length");
        }
        break;
    case DFU_OPCODE_VALID_FW:
        if (length == DFU_LENGTH_VALID_FW)
        {
            bool check_result;
            LE_ARRAY_TO_UINT16(dfu_ctx.signature, p);
            DFU_PRINT_TRACE1("DFU_OPCODE_VALID_FW: signature = 0x%x", dfu_ctx.signature);
            unlock_flash_all();
            flstatus = flash_lock(FLASH_LOCK_USER_MODE_READ);//signal = os_lock();
            check_result = dfu_check_checksum(dfu_ctx.signature);
            flash_unlock(FLASH_LOCK_USER_MODE_READ);
            lock_flash();
            DFU_PRINT_INFO1("dfu_act_notify_valid, check_result:%d (1: Success, 0: Fail)", check_result);
            if (check_result)
            {
                notif_data[2] = DFU_ARV_SUCCESS;
            }
            else
            {
                notif_data[2] = DFU_ARV_FAIL_CRC_ERROR;
                if (pfnDfuExtendedCB)
                {
                    dfu_cb_msg_t cb_msg;
                    cb_msg.type = DFU_CB_FAIL;
                    pfnDfuExtendedCB(dfu_server_id, &cb_msg);
                }
            }
            notif_data[0] = DFU_OPCODE_NOTIF;
            notif_data[1] = DFU_OPCODE_VALID_FW;
            server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                             notif_data, 3, GATT_PDU_TYPE_NOTIFICATION);
        }
        else
        {
            DFU_PRINT_TRACE0("DFU_OPCODE_VALID_FW: invalid length");
        }
        break;
    case DFU_OPCODE_ACTIVE_IMAGE_RESET:
        {
            /*notify bootloader to reset and use new image*/
            DFU_PRINT_TRACE0("DFU_OPCODE_ACTIVE_IMAGE_RESET:");
            os_delay(20);
            le_disconnect(0);
            if (pfnDfuExtendedCB)
            {
                dfu_cb_msg_t cb_msg;
                cb_msg.type = DFU_CB_END;
                pfnDfuExtendedCB(dfu_server_id, &cb_msg);
            }
            os_delay(20);
            DBG_DIRECT("dfu success, reboot!");
            unlock_flash_all();
            mesh_reboot(MESH_OTA, 1000);
        }
        break;
    case DFU_OPCODE_SYSTEM_RESET:
        DFU_PRINT_ERROR0("DFU_OPCODE_SYSTEM_RESET");
        DBG_DIRECT("dfu reboot!");
        WDG_SystemReset(RESET_ALL, DFU_SYSTEM_RESET);
        break;
    case DFU_OPCODE_REPORT_TARGET_INFO:
        if (length == DFU_LENGTH_REPORT_TARGET_INFO)
        {
            LE_ARRAY_TO_UINT16(dfu_ctx.signature, p);
            //dfu_report_target_fw_info(dfu_ctx.signature, &dfu_ctx.origin_image_version,
            //                          (uint32_t *)&dfu_ctx.curr_offset);
            dfu_ctx.origin_image_version = dfu_ctx.signature == AppPatch ? DFU_APP_VERSION : DFU_PATCH_VERSION;
            dfu_ctx.curr_offset = 0; //< mesh don't support ota resuming
            ota_tmp_buf_used_size = 0;
            notif_data[0] = DFU_OPCODE_NOTIFICATION;
            notif_data[1] = DFU_OPCODE_REPORT_TARGET_INFO;
            notif_data[2] = DFU_ARV_SUCCESS;
            LE_UINT32_TO_ARRAY(&notif_data[3], dfu_ctx.origin_image_version);
            LE_UINT32_TO_ARRAY(&notif_data[7], dfu_ctx.curr_offset);
            PROFILE_PRINT_INFO1("dfu_ctx.curr_offset is %x\r\n", dfu_ctx.curr_offset);
            server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                             notif_data, DFU_NOTIFY_LENGTH_REPORT_TARGET_INFO, GATT_PDU_TYPE_NOTIFICATION);
        }
        else
        {
            DFU_PRINT_TRACE0("DFU_OPCODE_REPORT_TARGET_INFO: invalid length");
        }
        break;
    case DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ:
        {
            //uint8_t notif_data[3] = {0};
            notif_data[0] = DFU_OPCODE_NOTIF;
            notif_data[1] = DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ;
            if (length  == DFU_LENGTH_CONN_PARA_TO_UPDATE_REQ)
            {
                if (dfu_ctx.ota_conn_para_upd_in_progress)
                {
                    DFU_PRINT_ERROR0("DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ bOTA_ConnParaUpdInProgress!!");
                    notif_data[2] = DFU_ARV_FAIL_OPERATION;
                    server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                     notif_data, 3, GATT_PDU_TYPE_NOTIFICATION);
                }
                else
                {
                    uint16_t conn_interval_min;
                    uint16_t conn_interval_max;
                    uint16_t conn_latency;
                    uint16_t superv_tout;

                    LE_ARRAY_TO_UINT16(conn_interval_min, p_value + 1);
                    LE_ARRAY_TO_UINT16(conn_interval_max, p_value + 3);
                    LE_ARRAY_TO_UINT16(conn_latency, p_value + 5);
                    LE_ARRAY_TO_UINT16(superv_tout, p_value + 7);

                    if (le_update_conn_param(0, conn_interval_min, conn_interval_max, conn_latency,
                                             2000 / 10, conn_interval_min * 2 - 2, conn_interval_max * 2 - 2) == GAP_CAUSE_SUCCESS)
                    {
                        /* Connection Parameter Update Request sent successfully, means this procedure is in progress. */
                        dfu_ctx.ota_conn_para_upd_in_progress = true;
                        DFU_PRINT_INFO4("DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ intMin=0x%x, intMax=0x%x, lat=0x%x, supto=0x%x.",
                                        conn_interval_min, conn_interval_max, conn_latency, superv_tout);
                    }
                    else
                    {
                        notif_data[2] = DFU_ARV_FAIL_OPERATION;
                        server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                         notif_data, 3, GATT_PDU_TYPE_NOTIFICATION);
                    }
                }
            }
            else
            {
                /*TODO: to be masked.*/
                DFU_PRINT_ERROR1("DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ length = %d Error!", length);
                notif_data[2] = DFU_ARV_FAIL_INVALID_PARAMETER;
                server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                 notif_data, 3, GATT_PDU_TYPE_NOTIFICATION);
            }
        }
        break;
    case DFU_OPCODE_BUFFER_CHECK_EN: //0x09
        {
            le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &dfu_ctx.mtu_size, 0);
            DFU_PRINT_TRACE1("DFU_OPCODE_BUFFER_CHECK_EN,MTUSIZE is %d", dfu_ctx.mtu_size);
            gSilBufCheckEN = true;
            notif_data[0] = DFU_OPCODE_NOTIF;
            notif_data[1] = DFU_OPCODE_BUFFER_CHECK_EN;
            notif_data[2] = 0x01;
            LE_UINT16_TO_ARRAY(&notif_data[3], DFU_TEMP_BUFFER_SIZE);
            LE_UINT16_TO_ARRAY(&notif_data[5], dfu_ctx.mtu_size);
            server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                             notif_data, 7, GATT_PDU_TYPE_NOTIFICATION);
        }
        break;
    case DFU_OPCODE_REPORT_BUFFER_CRC:        //0x0a
        if (length == 5)
        {
            LE_ARRAY_TO_UINT16(mBufSize, p);
            p += 2;
            LE_ARRAY_TO_UINT16(mCrcVal, p);
            DFU_PRINT_TRACE2("DFU_OPCODE_REPORT_BUFFER_CRC mBufferSize is 0x%x,mCrc is 0x%x", mBufSize,
                             mCrcVal);
            dfu_server_buffer_check(mBufSize, mCrcVal);
        }
        break;
    case DFU_OPCODE_RECEIVE_IC_TYPE:
        {
            uint8_t ic_type;
            notif_data[0] = DFU_OPCODE_NOTIF;
            notif_data[1] = DFU_OPCODE_RECEIVE_IC_TYPE;
            if (dfu_report_target_ic_type(OTA, &ic_type))
            {
                notif_data[2] = DFU_ARV_FAIL_INVALID_PARAMETER;
                notif_data[3] =  0xff;
            }
            else
            {
                notif_data[2] = DFU_ARV_SUCCESS;
                notif_data[3] =  ic_type;
            }
            server_send_data(0, dfu_server_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                             notif_data, 4, GATT_PDU_TYPE_NOTIFICATION);
        }
        break;
    default:
        {
            DFU_PRINT_TRACE1("dfu_service_handle_control_point_req: Unknown Opcode=0x%x",
                             dfu_control_point.opcode
                            );
        }
        break;
    }
}

/**
 * @brief dfu_server_handle_data
 *
 * @param length     data reviewed length.
 * @param p_value    data receive point address.
 * @return None
*/
void dfu_server_handle_data(uint16_t length, uint8_t *p_value)
{
    DFU_PRINT_TRACE4("dfu_service_handle_packet_req: length=%d, cur_offset =%d, ota_temp_buf_used_size = %d,image_total_length= %d",
                     length,
                     dfu_ctx.curr_offset,
                     ota_tmp_buf_used_size,
                     dfu_ctx.image_length
                    );

    if (dfu_ctx.curr_offset + ota_tmp_buf_used_size + length > dfu_ctx.image_length)
    {
        DFU_PRINT_TRACE4("dfu_service_handle_packet_req: p_dfu->cur_offset=%d, ota_temp_buf_used_size =%d, length= %d, image_total_length = %d ",
                         dfu_ctx.curr_offset,
                         ota_tmp_buf_used_size,
                         length,
                         dfu_ctx.image_length
                        );
        PROFILE_PRINT_INFO0("dfu_act_reset_and_activate");
    }
    else
    {
        if (gSilBufCheckEN == true)
        {
            memcpy(g_pOtaTempBufferHead + ota_tmp_buf_used_size, p_value, length);
            ota_tmp_buf_used_size += length;
        }
        else
        {
            if (OTP->ota_with_encryption_data)
            {
                uint16_t offset = 0;
                while (length - offset >= 16)
                {
                    dfu_decrypt(p_value + offset);
                    offset += 16;
                }
            }

            memcpy(g_pOtaTempBufferHead + ota_tmp_buf_used_size, p_value, length);
            ota_tmp_buf_used_size += length;
            uint32_t left_length = dfu_ctx.image_length - dfu_ctx.curr_offset - ota_tmp_buf_used_size;
            if (left_length == 0 || ota_tmp_buf_used_size == DFU_TEMP_BUFFER_SIZE ||
                ota_tmp_buf_used_size + MIN(left_length, length) > DFU_TEMP_BUFFER_SIZE)
            {
                unlock_flash_all();
                if (sil_dfu_update(dfu_ctx.signature, dfu_ctx.curr_offset, ota_tmp_buf_used_size,
                                   (uint32_t *)g_pOtaTempBufferHead) == 0)
                {
                    lock_flash();
                }
                else
                {
                    lock_flash();
                    /*eflash write fail, we should restart ota procedure.*/
                    dfu_reset(dfu_ctx.signature);
                    dfu_fw_active_reset();
                }
                dfu_ctx.curr_offset += ota_tmp_buf_used_size;
                ota_tmp_buf_used_size = 0;
            }
        } //if(gBufCheckEN == true)
        data_uart_debug("\b\b\b\b%3d%%",
                        (dfu_ctx.curr_offset + ota_tmp_buf_used_size) * 100 / dfu_ctx.image_length);
    }
}

/**
 * @brief write characteristic data from service.
 *
 * @param ServiceID          ServiceID to be written.
 * @param iAttribIndex       Attribute index of characteristic.
 * @param wLength            length of value to be written.
 * @param pValue             value to be written.
 * @return Profile procedure result
*/
T_APP_RESULT dfu_attr_write_cb(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index,
                               T_WRITE_TYPE write_type,
                               uint16_t length, uint8_t *p_value, P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    //TDFU_CALLBACK_DATA callback_data;
    T_APP_RESULT  wCause = APP_RESULT_SUCCESS;

    if (attrib_index == INDEX_DFU_CONTROL_POINT_CHAR_VALUE)
    {
        dfu_server_handle_cp(length, p_value);
    }
    else if (attrib_index == INDEX_DFU_PACKET_VALUE)
    {
        dfu_server_handle_data(length, p_value);
    }
    else
    {
        PROFILE_PRINT_INFO1("!!!dfuServiceAttribPut Fail: iAttribIndex=%d.", attrib_index);

        return APP_RESULT_ATTR_NOT_FOUND;

    }
    return wCause;
}

/**
 * @brief update CCCD bits from stack.
 *
 * @param ServiceId          Service ID.
 * @param Index          Attribute index of characteristic data.
 * @param wCCCBits         CCCD bits from stack.
 * @return None
*/
void dfu_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    PROFILE_PRINT_INFO2("DfuCccdUpdateCb  Index = %d wCCCDBits %x", index, ccc_bits);
    switch (index)
    {
    case INDEX_DFU_CHAR_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                dfu_server_conn_id = conn_id;
                // Enable Notification
                if (NULL == g_pOtaTempBufferHead)
                {
                    g_pOtaTempBufferHead = plt_malloc(DFU_TEMP_BUFFER_SIZE, RAM_TYPE_DATA_ON);
                }
            }
            else
            {
            }
            break;
        }
    default:
        break;
    }
    /* Notify Application. */
    return;
}

void dfu_server_disconnect_cb(uint8_t conn_id)
{
    if (dfu_server_conn_id != conn_id)
    {
        return;
    }
    else
    {
        dfu_server_conn_id = DFU_SERVER_INVALID_CONN_ID;
    }

    if (NULL != g_pOtaTempBufferHead)
    {
        plt_free(g_pOtaTempBufferHead, RAM_TYPE_DATA_ON);
        g_pOtaTempBufferHead = NULL;
    }
#if DFU_WO_SCAN
    if (dfu_ctx.bg_scan)
    {
        dfu_ctx.bg_scan = 0;
        gap_sched_scan(1);
    }
#endif
    if (dfu_ctx.fsm == DFU_CB_START)
    {
        dfu_ctx.fsm = DFU_CB_NONE;
        if (pfnDfuExtendedCB)
        {
            dfu_cb_msg_t cb_msg;
            cb_msg.type = DFU_CB_FAIL;
            pfnDfuExtendedCB(dfu_server_id, &cb_msg);
        }
    }
}
/**
 * @brief OTA ble Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS dfu_server_cbs =
{
    NULL,   // Read callback function pointer
    dfu_attr_write_cb,  // Write callback function pointer
    dfu_cccd_update_cb                    // CCCD update callback function pointer
};

/**
 * @brief  add OTA ble service to application.
 *
 * @param  pFunc          pointer of app callback function called by profile.
 * @return service ID auto generated by profile layer.
 * @retval ServiceId
*/
uint8_t dfu_server_add(void *pcb)
{
    uint8_t service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)dfu_service_table,
                                    sizeof(dfu_service_table),
                                    dfu_server_cbs))
    {
        PROFILE_PRINT_ERROR1("DFUService_AddService: ServiceId %d", service_id);
        service_id = 0xff;
        return service_id;
    }
    dfu_server_id = service_id;
    pfnDfuExtendedCB = (P_FUN_SERVER_GENERAL_CB)pcb;
#if DFU_AUTO_BETWEEN_DEVICES
    dfu_server_timer_start();
#endif
    //g_pOtaTempBufferHead = OtaTempBufferHead;
    return service_id;
}

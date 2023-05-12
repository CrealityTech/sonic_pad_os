/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mp_cmd_parse.c
  * @brief    Source file for command parse.
  * @details  Parse mp commands and execute right commands.
  * @author   hector
  * @date     2018-08-13
  * @version  v1.0
  * *************************************************************************************
  */

#include <gap.h>
#include "mp_cmd_parse.h"
#include "platform_diagnose.h"
#include "platform_os.h"
#include "crc16btx.h"
#include "data_uart.h"

typedef enum
{
    PARSE_HEAD,
    PARSE_OPCODE,
    PARSE_PAYLOAD,
    PARSE_CRC
} mp_parse_state_t;

#define MP_HEAD           0x87
#define MP_HEAD_LEN       1
#define MP_OPCODE_LEN     2
#define MP_CRC_LEN        2


typedef struct
{
    mp_parse_state_t state;
    uint8_t *pdata;
    const mp_cmd_table_t *pcmd_info;
    uint16_t opcode;
    uint8_t state_count;
    uint8_t data[MP_CMD_MAX_SIZE];
} mp_packet_t;

static mp_packet_t *mp_pkt = NULL;

static const mp_cmd_table_t *cmd_table = NULL;
static uint32_t cmd_table_len = 0;


static const mp_cmd_table_t *mp_get_cmd_info(uint16_t opcode)
{
    if (NULL == cmd_table)
    {
        return NULL;
    }

    for (uint16_t i = 0; i < cmd_table_len; ++i)
    {
        if (cmd_table[i].opcode == opcode)
        {
            return &cmd_table[i];
        }
    }

    return NULL;
}

static void mp_cmd_response(uint16_t opcode, mp_cmd_process_result_t result, const uint8_t *payload,
                            uint32_t payload_len)
{
    diag_assert((payload_len > 0) ? (NULL != payload) : TRUE);
    uint8_t buf[MP_CMD_MAX_SIZE];
    uint8_t *pbuf = buf;
    *pbuf ++ = MP_HEAD;
    *pbuf ++ = opcode;
    *pbuf ++ = opcode >> 8;
    *pbuf ++ = (uint8_t)result;
    *pbuf ++ = payload_len;
    *pbuf ++ = payload_len >> 8;
    *pbuf ++ = payload_len >> 16;
    *pbuf ++ = payload_len >> 24;
    while (payload_len --)
    {
        *pbuf ++ = *payload ++;
    }
    uint16_t calc_crc = btxfcs(0, buf, pbuf - buf);
    *pbuf ++ = calc_crc;
    *pbuf ++ = calc_crc >> 8;
    data_uart_send_string(buf, pbuf - buf);
}

void mp_cmd_parse(const uint8_t *pdata, uint8_t len)
{
    diag_assert(NULL != mp_pkt);
    while (len --)
    {
        switch (mp_pkt->state)
        {
        case PARSE_HEAD:
            if (MP_HEAD == *pdata)
            {
                mp_pkt->state = PARSE_OPCODE;
                mp_pkt->state_count = MP_OPCODE_LEN;
                mp_pkt->pdata = mp_pkt->data;
                *mp_pkt->pdata ++ = MP_HEAD;
            }
            break;
        case PARSE_OPCODE:
            *mp_pkt->pdata ++ = *pdata;
            mp_pkt->state_count --;
            if (0 == mp_pkt->state_count)
            {
                /** get command table */
                mp_pkt->opcode = (mp_pkt->pdata[-1] << 8) + mp_pkt->pdata[-2];
                mp_pkt->pcmd_info = mp_get_cmd_info(mp_pkt->opcode);
                if (NULL == mp_pkt->pcmd_info)
                {
                    /** invalid opcode, reponse error */
                    printe("mp_cmd_parse: receive invalid opcode = 0x%x", mp_pkt->opcode);
                    mp_cmd_response(mp_pkt->opcode, MP_CMD_RESULT_ERROR, NULL, 0);
                    mp_pkt->state = PARSE_HEAD;
                }
                else
                {
                    if (mp_pkt->pcmd_info->payload_len > 0)
                    {
                        mp_pkt->state = PARSE_PAYLOAD;
                        mp_pkt->state_count = mp_pkt->pcmd_info->payload_len;
                    }
                    else
                    {
                        mp_pkt->state = PARSE_CRC;
                        mp_pkt->state_count = MP_CRC_LEN;
                    }
                }
            }
            break;
        case PARSE_PAYLOAD:
            *mp_pkt->pdata ++ = *pdata;
            mp_pkt->state_count --;
            if (0 == mp_pkt->state_count)
            {
                mp_pkt->state = PARSE_CRC;
                mp_pkt->state_count = MP_CRC_LEN;
            }
            break;
        case PARSE_CRC:
            *mp_pkt->pdata ++ = *pdata;
            mp_pkt->state_count --;
            if (0 == mp_pkt->state_count)
            {
                /** get recived crc */
                uint16_t recv_crc = (mp_pkt->pdata[-1] << 8) + mp_pkt->pdata[-2];

                /** validate crc */
                uint16_t calc_crc = btxfcs(0, mp_pkt->data, mp_pkt->pdata - mp_pkt->data - MP_CRC_LEN);
                if (calc_crc == recv_crc)
                {
                    /** get valid packet */
                    mp_cmd_process_result_t result = mp_pkt->pcmd_info->cmd_process(mp_pkt->opcode,
                                                                                    mp_pkt->data + MP_HEAD_LEN + MP_OPCODE_LEN,
                                                                                    mp_pkt->pcmd_info->payload_len);
                    mp_cmd_response(mp_pkt->opcode, result, NULL, 0);
                }
                else
                {
                    printe("mp_cmd_parse: received crc does not match calculated crc!");
                    /** crc check failed */
                    mp_cmd_response(mp_pkt->opcode, MP_CMD_RESULT_ERROR, NULL, 0);
                }

                mp_pkt->state = PARSE_HEAD;
            }
            break;
        default:
            /** should never reach here */
            diag_assert(0);
            break;
        }

        pdata ++;
    }
}

bool mp_cmd_init(const mp_cmd_table_t *pcmd_table, uint32_t table_len)
{
    cmd_table = pcmd_table;
    cmd_table_len = table_len;
    mp_pkt = plt_malloc(sizeof(mp_packet_t), RAM_TYPE_DATA_OFF);
    if (NULL == mp_pkt)
    {
        printe("mp_cmd_init: initialize mp parser failed!");
        return FALSE;
    }
    mp_pkt->state = PARSE_HEAD;
    printi("mp_cmd_init: mp parser ready");

    return TRUE;
}




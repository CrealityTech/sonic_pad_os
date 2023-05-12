/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      light_cwrgb_server_app.c
* @brief     Smart mesh demo light cwrgb application
* @details
* @author    bill
* @date      2018-1-4
* @version   v1.0
* *********************************************************************************************************
*/

#include "light_cwrgb_server_app.h"
#include "light_cwrgb.h"
#include "light_cwrgb_app.h"
#include "light_storage_app.h"

/* cwrgb model */
/* 0 - primary element */
mesh_model_info_t light_cwrgb_server;

static bool light_cwrgb_server_receive(mesh_msg_p pmesh_msg)
{
    bool ret = TRUE;
    uint8_t *pbuffer = pmesh_msg->pbuffer + pmesh_msg->msg_offset;

    switch (pmesh_msg->access_opcode)
    {
    case MESH_MSG_LIGHT_CWRGB_GET:
        if (pmesh_msg->msg_len == sizeof(light_cwrgb_get_t))
        {
            uint8_t cwrgb[5];
            light_get_cwrgb(cwrgb);
            light_cwrgb_stat(&light_cwrgb_server, pmesh_msg->src, pmesh_msg->app_key_index, cwrgb);
        }
        break;
    case MESH_MSG_LIGHT_CWRGB_SET:
    case MESH_MSG_LIGHT_CWRGB_SET_UNACK:
        if (pmesh_msg->msg_len == sizeof(light_cwrgb_set_t))
        {
            light_cwrgb_set_t *pmsg = (light_cwrgb_set_t *)pbuffer;
            uint16_t cwrgb[5];
            for (uint8_t channel = 0; channel < sizeof(pmsg->cwrgb); channel++)
            {
                cwrgb[channel] = pmsg->cwrgb[channel] * 65535 / 255;
            }
            light_set_cwrgb(cwrgb);
            light_state_store();

            if (pmesh_msg->access_opcode == MESH_MSG_LIGHT_CWRGB_SET)
            {
                light_cwrgb_stat(&light_cwrgb_server, pmesh_msg->src, pmesh_msg->app_key_index, pmsg->cwrgb);
            }
        }
        break;
    default:
        ret = FALSE;
        break;
    }
    return ret;
}

void light_cwrgb_server_models_init(void)
{
    /* proprietary vendor light model initiation */
    light_cwrgb_server_reg(0, &light_cwrgb_server, light_cwrgb_server_receive);
}

#include <os_sched.h>
#include <string.h>
#include <trace.h>
#include <gap.h>
#include <gap_scan.h>
#include <gap_msg.h>
#include <gap_adv.h>
#include <platform_utils.h>
#include "mesh_api.h"
#include "health.h"
#include "tp.h"
#include "mesh_cmd.h"
#include "RTKMeshTypes.h"
#include "trace.h"
#include <app_msg.h>
#include "gap_wrapper.h"
#include "provision_provisioner.h"

#define USER_MESH_DEVICE_UUID  {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0xcc}
bool prov_manual;
uint32_t attn_dur;
prov_auth_value_type_t prov_auth_value_type;
uint32_t prov_start_time;
mesh_model_info_t model_goo_client_info;
mesh_model_info_t model_llc_client_info;
mesh_model_info_t model_light_ctl_client_info;
mesh_model_info_t model_light_hsl_client_info;

model_data_cb_pf mesh_cfg_client_recv_cb;
model_data_cb_pf mesh_goo_recv_cb;
model_data_cb_pf mesh_llc_recv_cb;
model_data_cb_pf mesh_light_ctl_recv_cb;
model_data_cb_pf mesh_light_hsl_recv_cb;

vendor_model_queue_t vendor_model_queue;
uint8_t element_num = 2;

void rtkmesh_element_init(uint8_t elementNum)
{
	element_num = elementNum;
}

void vendor_model_queue_init()
{
    vendor_model_queue_t* pvendor_model_queue = &vendor_model_queue;
    pvendor_model_queue->vendor_model_free.pfirst = NULL;
    pvendor_model_queue->vendor_model_free.plast = NULL;
    pvendor_model_queue->vendor_model_free.count = 0;
    pvendor_model_queue->vendor_model_used.pfirst = NULL;
    pvendor_model_queue->vendor_model_used.plast = NULL;
    pvendor_model_queue->vendor_model_used.count = 0;
    for (uint16_t record_loop = 0; record_loop < NUM_VENDOR_MODEL_INFO; record_loop++)
    {
        plt_list_push(&pvendor_model_queue->vendor_model_free, &pvendor_model_queue->vendor_model_entry[record_loop]);
    }
}

int32_t vendor_model_add(uint8_t element_index, mesh_model_id_t model_id, model_receive_pf recv_cb)
{
    plt_list_t *pqueue = &vendor_model_queue.vendor_model_used;
    plt_list_t *pqueue_free = &vendor_model_queue.vendor_model_free;
    vendor_model_entry_t* precord;
    if (pqueue_free->count != 0)
    {
        precord = plt_list_pop(pqueue_free);
		precord->element_index = element_index;
    	precord->model_id = model_id;
		precord->recv_cb = recv_cb;
    	plt_list_push(pqueue, precord);
		APP_PRINT_INFO3("vendor_model_add: element index= %d, company_id = 0x%04x, model_id = 0x%04x", element_index, model_id.company_id, model_id.model_id);
		return 0;
    }
    else
    {
        APP_PRINT_ERROR0("no space for adding one more vendor model ");
		return -1;
    }
}
void vendor_model_deinit(mesh_model_info_p pmodel_info)
{
	plt_free(pmodel_info,RAM_TYPE_DATA_ON);
	pmodel_info = NULL;
	return ;
}
void  vendor_model_reg(void )
{
	uint32_t rtk_model_id = 0x00;
	bool ret;

	plt_list_t *pqueue = &vendor_model_queue.vendor_model_used;
	vendor_model_entry_t* precord = (vendor_model_entry_t* )pqueue->pfirst;
	while(precord)
	{
		mesh_model_info_p pmodel_info = (mesh_model_info_t*)plt_malloc(sizeof(mesh_model_info_t),RAM_TYPE_DATA_ON);

	    if (NULL == pmodel_info)
	    {
			APP_PRINT_ERROR0("pmodel_info is null");
			return;
	    }
		memset(pmodel_info,0,sizeof(mesh_model_info_t));

	    rtk_model_id = precord->model_id.model_id;
	    rtk_model_id = (rtk_model_id<<16) | precord->model_id.company_id;

	    pmodel_info->model_id = rtk_model_id;
		pmodel_info->model_receive = precord->recv_cb;
		pmodel_info->model_deinit = vendor_model_deinit;
	    ret = mesh_model_reg(precord->element_index, pmodel_info);
		if(!ret)
		{
			APP_PRINT_ERROR2("vendor model id 0x%04x, vendor company id 0x%04x register fail", precord->model_id.model_id,
																							   precord->model_id.company_id);
		}
		else
		{
			APP_PRINT_INFO3("vendor model id 0x%04x, vendor company id 0x%04x, rtk model id 0x%08x success",precord->model_id.model_id,
																							   precord->model_id.company_id,
																							   rtk_model_id);
		}

		precord = precord->pnext;
	}
}

void app_handle_gap_msg(T_IO_MSG *p_gap_msg);

/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void app_handle_io_msg(T_IO_MSG io_msg)
{
	uint16_t msg_type = io_msg.type;

	switch (msg_type) {
	case IO_MSG_TYPE_BT_STATUS:
		app_handle_gap_msg(&io_msg);
		break;
	case DISC_TIMEOUT_MSG:
		disc_handle_timeout();
		break;
	case PROV_START_BY_STEP_TIMEOUT_MSG:
		prov_start_by_step_handle_timeout();
		break;
	default:
		break;
	}
}

/**
 * @brief    All the BT GAP MSG are pre-handled in this function.
 * @note     Then the event handling function shall be called according to the
 *           sub_type of T_IO_MSG
 * @param[in] p_gap_msg Pointer to GAP msg
 * @return   void
 */
void app_handle_gap_msg(T_IO_MSG * p_gap_msg)
{
	mesh_inner_msg_t mesh_inner_msg;

	APP_PRINT_TRACE2("%s: sub_type %d", __func__, p_gap_msg->subtype);

	mesh_inner_msg.type = MESH_BT_STATUS_UPDATE;
	mesh_inner_msg.sub_type = p_gap_msg->subtype;
	mesh_inner_msg.parm = p_gap_msg->u.param;

	if(1 == first_start_flag &&
		GAP_MSG_LE_DEV_STATE_CHANGE == mesh_inner_msg.sub_type)
	{
		T_GAP_DEV_STATE device_new_status = *(T_GAP_DEV_STATE*)&mesh_inner_msg.parm;
		if(  1 == device_new_status.gap_init_state )
		{
			APP_PRINT_TRACE2("%s: gap_init_state 0x%x",__func__,device_new_status.gap_init_state);
			first_start_flag = 0; //mesh stack has been enabled

			MESH_BT_EVENT_T init_done;
			init_done.evt_id = MESH_EVT_INIT_DONE;
			if(mesh_event_cb != NULL)
			{
				mesh_event_cb(&init_done);
			}
		}
	}
	gap_sched_handle_bt_status_msg(&mesh_inner_msg);

	return;
}

/******************************************************************
 * @fn      device_info_cb
 * @brief   device_info_cb callbacks are handled in this function.
 *
 * @param   cb_data  -  @ref prov_cb_data_t
 * @return  void
 */
void device_info_cb(uint8_t bt_addr[6], uint8_t bt_addr_type, int8_t rssi,
		    device_info_t * pinfo)
{
	if (!dev_info_show_flag) {
		return;
	}
	switch (pinfo->type)
	{
		case DEVICE_INFO_UDB:
		{
			MESH_BT_EVENT_T udb_event;
			MESH_EVT_PROV_SCAN_UD_T *pevt;

			memset(&udb_event,0,sizeof(MESH_BT_EVENT_T));
			pevt = &udb_event.evt.mesh_evt.mesh.prov_scan_ud;

			udb_event.evt_id = MESH_EVT_PROV_SCAN_UD_RESULT;

			memcpy(pevt->uuid, pinfo->pbeacon_udb->dev_uuid, 16);
			memcpy(&(pevt->oob_info), pinfo->pbeacon_udb->oob_info, 2);

			if(mesh_event_cb != NULL)
			{
				mesh_event_cb(&udb_event );//??
			}

			//APP_PRINT_INFO0("udb=");
			//data_uart_dump(pinfo->pbeacon_udb->dev_uuid, 16);
			break;
		}
		default:
			break;
	}

}

/******************************************************************
 * @fn      prov_cb
 * @brief   Provisioning callbacks are handled in this function.
 *
 * @param   cb_data  -  @ref prov_cb_data_t
 * @return  the operation result
 */
bool prov_cb(prov_cb_type_t cb_type, prov_cb_data_t cb_data)
{
    APP_PRINT_INFO1("prov_cb: type = %d", cb_type);

    switch (cb_type)
    {
    case PROV_CB_TYPE_PB_ADV_LINK_STATE:
        switch (cb_data.pb_generic_cb_type)
        {
			case PB_GENERIC_CB_LINK_OPENED:
			{
				uint8_t rett;
				prov_start_time = plt_time_read_ms();
				rett = prov_invite(attn_dur);

				if( false == rett)
				{
					//
					APP_PRINT_ERROR0("PB-ADV: send provisioning invite failed!\r\n>");
					prov_ctx_p pprov_ctx = &prov_ctx;
					prov_timer_stop();
					pprov_ctx->fsm = PROV_INVITE;
					MESH_BT_EVENT_T LinkOpen_fail;
					memset(&LinkOpen_fail,0,sizeof(MESH_BT_EVENT_T));
					LinkOpen_fail.evt_id = MESH_EVT_PROV_DONE;
					LinkOpen_fail.evt.mesh_evt.mesh.prov_done.reason = AG_MESH_PROV_FAILED_ERROR_CODE_UNEXPECTED_PDU; //??
					if(mesh_event_cb != NULL)
					{
						mesh_event_cb(&LinkOpen_fail );	//notify app that prov proc has failed.
					}
					prov_disconnect(PB_ADV_LINK_CLOSE_PROVISIONING_FAIL);
				}
				APP_PRINT_INFO0("PB-ADV Link Opened!\r\n>");
				break;
			}
			case PB_GENERIC_CB_LINK_OPEN_FAILED:
			{
				MESH_BT_EVENT_T LinkOpen_fail;
				memset(&LinkOpen_fail,0,sizeof(MESH_BT_EVENT_T));
				LinkOpen_fail.evt_id = MESH_EVT_PROV_DONE;
				LinkOpen_fail.evt.mesh_evt.mesh.prov_done.reason = AG_MESH_PROV_FAILED_ERROR_CODE_GW_CONN_TOUT; //??
				if(mesh_event_cb != NULL)
				{
					mesh_event_cb(&LinkOpen_fail );
				}

				APP_PRINT_ERROR0("PB-ADV Link Open Failed!\r\n>");
				break;
			}
			case PB_GENERIC_CB_LINK_CLOSED:
			{
				APP_PRINT_INFO0("PB-ADV Link Closed!\r\n>");
				break;
			}
			default:
				break;
		}
        break;
    case PROV_CB_TYPE_PATH_CHOOSE:
        {
            if (prov_manual)
            {
                /* use cmd "authpath" to select oob/no oob public key and no oob/static oob/input oob/output oob auth data according to the device capabilities */
                prov_capabilities_p pprov_capabilities = cb_data.pprov_capabilities;
				MESH_BT_EVENT_T ProvCap;
				memset(&ProvCap,0,sizeof(MESH_BT_EVENT_T));
				MESH_PROV_CAPABILITIES_T* pProvCapValue;
				pProvCapValue = &ProvCap.evt.mesh_evt.mesh.prov_cap.cap;

				ProvCap.evt_id = MESH_EVT_PROV_CAPABILITIES;
				pProvCapValue->number_of_elements = pprov_capabilities->element_num;
				pProvCapValue->algorithms = pprov_capabilities->algorithm;
				pProvCapValue->public_key_type = pprov_capabilities->public_key;
				pProvCapValue->static_oob_type = pprov_capabilities->static_oob;
				pProvCapValue->output_oob_size = pprov_capabilities->output_oob_size;
				pProvCapValue->output_oob_action = pprov_capabilities->output_oob_action;
				pProvCapValue->input_oob_size = pprov_capabilities->input_oob_size;
				pProvCapValue->input_oob_action = pprov_capabilities->input_oob_action;

                APP_PRINT_INFO8("prov capabilities: en-%d al-%d pk-%d so-%d os-%d oa-%d is-%d ia-%d\r\n",
                                pprov_capabilities->element_num, pprov_capabilities->algorithm,
                                pprov_capabilities->public_key, pprov_capabilities->static_oob,
                                pprov_capabilities->output_oob_size, pprov_capabilities->output_oob_action,
                                pprov_capabilities->input_oob_size, pprov_capabilities->input_oob_action);
				if(mesh_event_cb != NULL)
				{
					mesh_event_cb(&ProvCap );
				}
            }
            else
            {
                /* select no oob public key and no oob auth data as default provision method */
                prov_start_t prov_start;
                memset(&prov_start, 0, sizeof(prov_start_t));
                prov_path_choose(&prov_start);
            }
        }
        break;
    case PROV_CB_TYPE_PUBLIC_KEY:
        {
            APP_PRINT_INFO0("prov_cb: get the public key from the device");
			MESH_BT_EVENT_T ProvOOBPublicKey;
			memset(&ProvOOBPublicKey,0,sizeof(MESH_BT_EVENT_T));
			ProvOOBPublicKey.evt_id = MESH_EVT_PROV_PUBLIC_KEY;
			if(mesh_event_cb != NULL)
			{
				mesh_event_cb(&ProvOOBPublicKey );
			}
        }
        break;
    case PROV_CB_TYPE_AUTH_DATA:
        {
            prov_start_p pprov_start = cb_data.pprov_start;
			MESH_BT_EVENT_T ProvAuthData;
			memset(&ProvAuthData,0,sizeof(MESH_BT_EVENT_T));
			ProvAuthData.evt_id = MESH_EVT_PROV_AUTH_DATA;
            prov_auth_value_type = prov_auth_value_data_type_get();
            /* use cmd to set auth data */
            APP_PRINT_INFO4("auth method=%d[nsoi] action=%d size=%d type=%d[nbNa]\r\n>",
                            pprov_start->auth_method,
                            (int)(pprov_start->auth_action.oob_action),
                            (int)(pprov_start->auth_size.oob_size), prov_auth_value_type);
			if(mesh_event_cb != NULL)
			{
				mesh_event_cb(&ProvAuthData );
			}
        }
        break;
    case PROV_CB_TYPE_COMPLETE:
        {
			prov_data_p pprov_data = cb_data.pprov_data;
			MESH_BT_EVENT_T prov_complete;
			MESH_EVT_PROV_DONE_T * pProvComplete;
			uint16_t dev_index = 0;

      		memset(&prov_complete, 0 ,sizeof(MESH_BT_EVENT_T));
			prov_complete.evt_id = MESH_EVT_PROV_DONE;
			pProvComplete = &(prov_complete.evt.mesh_evt.mesh.prov_done);

			pProvComplete->reason = AG_MESH_PROV_SUCCESS; //
			pProvComplete->address = pprov_data->unicast_address;
			pProvComplete->success = true;
			pProvComplete->gatt_bearer = false;
			APP_PRINT_INFO1("prov complete: unicast addr 0x%x",pProvComplete->address);

			for(dev_index = 0; dev_index < mesh_node.dev_key_num; dev_index++)
			{
				//APP_PRINT_INFO3("dev key index0x%x: used %d addr0x%x",dev_index, mesh_node.dev_key_list[dev_index].used,
				//														mesh_node.dev_key_list[dev_index].unicast_addr);
				if(1 == mesh_node.dev_key_list[dev_index].used
				     && pprov_data->unicast_address == mesh_node.dev_key_list[dev_index].unicast_addr)
				{
					break;
				}
			}
			if(dev_index == mesh_node.dev_key_num )
			{
				pProvComplete->success = false;
				memset(pProvComplete->device_key, 0, 16);
				pProvComplete->reason = AG_MESH_PROV_FAILED_ERROR_CODE_OUT_OF_RESOURCES;
				APP_PRINT_ERROR0("prov_cb: prov complete, but not store dev key!");
			}
			else
			{
				memcpy(pProvComplete->device_key, mesh_node.dev_key_list[dev_index].dev_key, 16);
				nmc_node_clear(pProvComplete->address);
				trans_rx_ctx_remove(pProvComplete->address);
				APP_PRINT_INFO0("prov_cb: prov complete success!");
			}
			APP_PRINT_INFO0("devkey =");
			data_uart_dump(pProvComplete->device_key, 16);

			if(mesh_event_cb !=NULL)
			{
				mesh_event_cb(&prov_complete );	//notify app that prov proc has failed.
			}

            //data_uart_debug("Has provisioned device with addr 0x%04x in %dms!\r\n", pprov_data->unicast_address,
            //                plt_time_read_ms() - prov_start_time);
        }
        break;
    case PROV_CB_TYPE_FAIL:
		{
			prov_cb_fail_t prov_fail = (prov_cb_fail_t ) cb_data.prov_fail;

			MESH_BT_EVENT_T provFail;
			memset(&provFail, 0, sizeof(MESH_BT_EVENT_T));
			provFail.evt_id = MESH_EVT_PROV_DONE;
			APP_PRINT_ERROR1("provision fail, type=%d!\r\n", cb_data.prov_fail.fail_type);
			switch(prov_fail.fail_type)
			{

				case PROV_CB_FAIL_TYPE_PROCEDURE_ABNORMAL:
				{
					provFail.evt.mesh_evt.mesh.prov_done.reason = prov_fail.prov_result; //
					break;
				}
				case PROV_CB_FAIL_TYPE_PROCEDURE_TIMEOUT:
				{
					provFail.evt.mesh_evt.mesh.prov_done.reason = AG_MESH_PROV_FAILED_ERROR_CODE_PROVISION_TIMEOUT; //
					break;
				}
				//the follow 3 cases is caused by LinkClose
				case PROV_CB_FAIL_TYPE_BEARER_IDLE:
				{
					provFail.evt.mesh_evt.mesh.prov_done.reason = AG_MESH_PROV_FAILED_ERROR_CODE_UNEXPECTED_ERROR; //?? error code
					break;
				}
				case PROV_CB_FAIL_TYPE_BEARER_LOSS:
				{
					provFail.evt.mesh_evt.mesh.prov_done.reason = AG_MESH_PROV_FAILED_ERROR_CODE_UNEXPECTED_ERROR; //?? error code
					break;
				}
				case PROV_CB_FAIL_TYPE_BEARER_TIMEOUT:
				{
					provFail.evt.mesh_evt.mesh.prov_done.reason = AG_MESH_PROV_FAILED_ERROR_CODE_TRANSACTION_TIMEOUT; //
					break;
				}
			}
			if(mesh_event_cb != NULL)
			{
				mesh_event_cb(&provFail );
			}
			break;
		}
    case PROV_CB_TYPE_PROV:
        // stack ready
        data_uart_debug("ms addr: 0x%04x\r\n>", mesh_node.unicast_addr);
        break;
    default:
        break;
    }
    return true;
}

/******************************************************************
 * @fn      fn_cb
 * @brief   fn callbacks are handled in this function.
 *
 * @param   frnd_index
 * @param   type
 * @param   fn_addr
 * @return  void
 */
void fn_cb(uint8_t frnd_index, fn_cb_type_t type, uint16_t lpn_addr) //extern fn_cb to mesh_cmd.c
{
	if (type == FN_CB_TYPE_ESTABLISH_SUCCESS ||
	    type == FN_CB_TYPE_FRND_LOST) {
		user_cmd_time(NULL);
	}
}

/******************************************************************
 * @fn      hb_cb
 * @brief   heartbeat callbacks are handled in this function.
 *
 * @param   type
 * @param   pdata
 * @return  void
 */
void hb_cb(hb_data_type_t type, void *pargs)
{
		APP_PRINT_INFO2("%s: type is %d",__func__,type);
		switch (type)
    {
    case HB_DATA_PUB_TIMER_STATE:
        {
            hb_data_timer_state_t *pdata = pargs;
            if (HB_TIMER_STATE_START == pdata->state)
            {
                APP_PRINT_TRACE1("heartbeat publish timer start, period = %d\r\n", pdata->period);
            }
            else
            {
                APP_PRINT_TRACE0("heartbeat publish timer stop\r\n");
            }
        }
        break;
    case HB_DATA_SUB_TIMER_STATE:
        {
            hb_data_timer_state_t *pdata = pargs;
            if (HB_TIMER_STATE_START == pdata->state)
            {
                APP_PRINT_TRACE1("heartbeat subscription timer start, period = %d\r\n", pdata->period);
            }
            else
            {
                APP_PRINT_TRACE0("heartbeat subscription timer stop\r\n");
            }
        }
        break;
    case HB_DATA_PUB_COUNT_UPDATE:
        {
            hb_data_pub_count_update_t *pdata = pargs;
            APP_PRINT_TRACE1("heartbeat publish count update: %d\r\n", pdata->count);
        }
        break;
    case HB_DATA_SUB_PERIOD_UPDATE:
        {
            hb_data_sub_period_update_t *pdata = pargs;
            APP_PRINT_TRACE1("heartbeat subscription period update: %d\r\n", pdata->period);
        }
        break;
    case HB_DATA_SUB_RECEIVE:
        {
            hb_data_sub_receive_t *pdata = pargs;
            APP_PRINT_TRACE7("receive heartbeat: src = %d, init_ttl = %d, features = %d-%d-%d-%d, ttl = %d\r\n",
                            pdata->src, pdata->init_ttl, pdata->features.relay, pdata->features.proxy,
                            pdata->features.frnd, pdata->features.lpn, pdata->ttl);

						MESH_BT_EVENT_T hb_event;

						hb_event.evt_id = MESH_EVT_HEARTBEAT;

						hb_event.evt.mesh_evt.mesh.heartbeat.address = pdata->src;
						if(pdata->init_ttl >= pdata->ttl)
						{
							hb_event.evt.mesh_evt.mesh.heartbeat.active = true;
						}
						else
						{
							hb_event.evt.mesh_evt.mesh.heartbeat.active = false;
						}
						mesh_event_cb(&hb_event );
        }
        break;
    default:
        break;
    }
}
void mesh_cfg_client_recv_cb_register(model_data_cb_pf cb)
{
	mesh_cfg_client_recv_cb = cb;
}
void mesh_goo_recv_cb_register(model_data_cb_pf cb)
{
	mesh_goo_recv_cb = cb;
}
void mesh_llc_recv_cb_register(model_data_cb_pf cb)
{
	mesh_llc_recv_cb = cb;
}
void mesh_light_ctl_recv_cb_register(model_data_cb_pf cb)
{
	mesh_light_ctl_recv_cb = cb;
}
void mesh_light_hsl_recv_cb_register(model_data_cb_pf cb)
{
	mesh_light_hsl_recv_cb = cb;
}
void mesh_stack_init(uint16_t local_unicast_addr)
{

	/* set ble stack log level, disable nonsignificant log */
	//log_module_bitmap_trace_set(0xFFFFFFFFFFFFFFFF, LEVEL_TRACE, 1);
	//log_module_bitmap_trace_set(0xFFFFFFFFFFFFFFFF, LEVEL_INFO, 1);
	//log_module_trace_set(MODULE_LOWERSTACK, LEVEL_ERROR, 1);

	/* set mesh stack log level, default all on, disable the log of level LEVEL_TRACE */
	//uint32_t module_bitmap[MESH_LOG_LEVEL_SIZE] = { 0xFFFeFF7d };
	//diag_level_set(LEVEL_TRACE, module_bitmap);

	uint64_t mask[LEVEL_NUM];
	uint32_t module_bitmap[MESH_LOG_LEVEL_SIZE] = { 0xFFFeFF7d };
	uint32_t module_bitmap_errlevel[MESH_LOG_LEVEL_SIZE] = { 0xFFFFFFFF };

	memset(mask,0,LEVEL_NUM*sizeof(uint64_t));
	log_module_trace_init(mask);

	module_bitmap[0] = MESHloglayer;
	if(loglevel >= LEVEL_NUM)
	{
		loglevel= LEVEL_TRACE;
	}

	if(loglevel >= LEVEL_TRACE)
	{
		log_module_bitmap_trace_set(APPloglayer, LEVEL_TRACE, 1);
		diag_level_set(LEVEL_TRACE, module_bitmap);
	}
	if(loglevel >= LEVEL_INFO)
	{
		log_module_bitmap_trace_set(APPloglayer, LEVEL_INFO, 1);
		diag_level_set(LEVEL_INFO, module_bitmap);
	}
	if(loglevel >= LEVEL_WARN)
	{
		log_module_bitmap_trace_set(APPloglayer, LEVEL_WARN, 1);
		diag_level_set(LEVEL_WARN, module_bitmap);
	}
	if(loglevel >= LEVEL_ERROR)
	{
		log_module_bitmap_trace_set(0xFFFFFFFFFFFFFFFF, LEVEL_ERROR, 1);
		diag_level_set(LEVEL_ERROR, module_bitmap_errlevel);
	}
	APP_PRINT_INFO2("log level %d, dev num %d ",loglevel,devnum);
	APP_PRINT_ERROR0("20200103: mesh wrapper version V2.00, mesh lib version V2.00");
	/* set device name and appearance */
	char *dev_name = "Mesh Provisioner";
	uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;

	gap_sched_params_set(GAP_SCHED_PARAMS_DEVICE_NAME, dev_name,
			     GAP_DEVICE_NAME_LEN);
	gap_sched_params_set(GAP_SCHED_PARAMS_APPEARANCE, &appearance,
			     sizeof(appearance));

	/* configure provisioning parameters */
	prov_params_set(PROV_PARAMS_CALLBACK_FUN, prov_cb, sizeof(prov_cb_pf));

	/* config node parameters */
	mesh_node_features_t features = {
		.role = MESH_ROLE_PROVISIONER,
		.relay = 0,
		.proxy = 2,
		.fn = 2,
		.lpn = 2,
		.prov = 2,
		.udb = 0,
		.snb = 1,
		.bg_scan = 1,
		.flash = 1,
		.flash_rpl = 1
	};
	mesh_node_cfg_t node_cfg = {
		.dev_key_num = devnum,
		.net_key_num = 10,
		.app_key_num = 10,
		.vir_addr_num = 3,
		.rpl_num = 0,
		.sub_addr_num = 5,
		.proxy_num = 0
	};
	mesh_node_cfg(features, &node_cfg);
	/* configure provisioner */
	mesh_node.node_state = PROV_NODE;
	mesh_node.unicast_addr = local_unicast_addr;

	mesh_node.net_trans_count = 5;
	mesh_node.net_trans_steps = 1;

	mesh_node.net_trans_count_base = 2;


	/* set device uuid */
	uint8_t dev_uuid[16] = USER_MESH_DEVICE_UUID;
	device_uuid_set(dev_uuid); //set mesh_node.dev_uuid

	/* create elements and register models */
	/*please consider carefully how much elements user need
	   mesh lib do not support add elements after mesh thread is lauched
	*/
	mesh_element_create(GATT_NS_DESC_UNKNOWN); //element 0 is created
	for(uint8_t index = 0; index < (element_num - 1); index++)
	{
		mesh_element_create(GATT_NS_DESC_UNKNOWN); //element 0 is created
	}

	cfg_client.model_data_cb = mesh_cfg_client_recv_cb;
	cfg_client_reg();
	cfg_server_reg();

	model_goo_client_info.model_data_cb = mesh_goo_recv_cb;
    generic_on_off_client_reg(0, &model_goo_client_info);

	model_llc_client_info.model_data_cb = mesh_llc_recv_cb;
	light_lightness_client_reg(0, &model_llc_client_info);

	model_light_ctl_client_info.model_data_cb = mesh_light_ctl_recv_cb;
	light_ctl_client_reg(0, &model_light_ctl_client_info);

	model_light_hsl_client_info.model_data_cb = mesh_light_hsl_recv_cb;
	light_hsl_client_reg(0, &model_light_hsl_client_info);

	vendor_model_reg();

	compo_data_page0_header_t compo_data_page0_header =
	    { COMPANY_ID, PRODUCT_ID, VERSION_ID };
	compo_data_page0_gen(&compo_data_page0_header);

	/* set net key and app key */
	/*
	const uint8_t net_key[] = MESH_NET_KEY;
	const uint8_t net_key1[] = MESH_NET_KEY1;
	const uint8_t app_key[] = MESH_APP_KEY;
	const uint8_t app_key1[] = MESH_APP_KEY1;
	uint8_t net_key_index = net_key_add(0, net_key);    //global key index
	uint8_t net_key_index1 = net_key_add(1, net_key1);//global key index
	app_key_add(net_key_index, 0, app_key);
	app_key_add(net_key_index1, 1, app_key1);
	*/

	/* init mesh stack */
	mesh_init();

	if(mesh_node.unicast_addr == 0)
	{
		if(mesh_node.dev_key_list[0].used == 1)
		{
			mesh_node.unicast_addr = mesh_node.dev_key_list[0].unicast_addr;
		}
		else
		{
			mesh_node.unicast_addr = 0x56;
			APP_PRINT_ERROR1("mesh node unicast address is not found, using default address 0x%04x",mesh_node.unicast_addr);
		}
	}
	else
	{
		if(mesh_node.dev_key_list[0].used == 1)
		{
			if(mesh_node.unicast_addr != mesh_node.dev_key_list[0].unicast_addr)
			{
				uint16_t netkeyLocalIndex = 0;
				mesh_node.dev_key_list[0].unicast_addr = mesh_node.unicast_addr;
				mesh_flash_store(MESH_FLASH_PARAMS_DEV_KEY,&netkeyLocalIndex);
			}
		}
	}

	/* the device wanna join in the mesh network will been assigned from address assign_addr */
	assign_addr = mesh_node.unicast_addr+mesh_node.element_queue.count;

	/* register udb/provision adv/proxy adv callback */
	device_info_cb_reg(device_info_cb);

	/* register heartbeat info callback */
	hb_init(hb_cb);

}


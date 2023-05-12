#include <unistd.h>
#include "app_handler.h"
extern int app_mesh_msg_pipe[2];
extern pthread_mutex_t app_mutex;
extern int32_t app_light_lightness_client_data_handler(MESH_MODEL_LLS_RECV_DATA_T *p);

uint32_t node_addr;

void data_array_print_dump(uint8_t *pbuffer, uint32_t len)
{
        printf("0");
        printf("x");
	uint32_t loop;
        for (loop = 0; loop < len; loop++)
        {
            char data = "0123456789ABCDEF"[pbuffer[loop] >> 4];
            printf("%1c",data);
            data = "0123456789ABCDEF"[pbuffer[loop] & 0x0f];
            printf("%1c",data);
        }
        printf("\r");
        printf("\n");
}

void app_mesh_event_cb(MESH_BT_EVENT_T *ag_event)
{
	uint8_t event;
	MESH_BT_EVENT_T *mesh_event = (MESH_BT_EVENT_T *)ag_event;
	MESH_BT_EVENT_T *mesh_event_duplicate = NULL;
	printf("**** app mesh event cb, event 0x%x..\n",mesh_event->evt_id);

	mesh_event_duplicate = malloc(sizeof(MESH_BT_EVENT_T));
	if(mesh_event_duplicate == NULL)
	{
		printf(">> error: event_duplicate create fail \n");
		return;
	}
	memset(mesh_event_duplicate,0,sizeof(MESH_BT_EVENT_T));
	memcpy(mesh_event_duplicate,mesh_event,sizeof(MESH_BT_EVENT_T));

	event = APP_HANDLE_MESH_EVET;
	pthread_mutex_lock(&app_mutex);
	if(!(os_event_send(&app_mesh_msg_pipe[1],&event,TIME_ALAWAYS_WAIT)))
	{
		printf(">> error: write app_mesh_msg_pipe fail \n");
	}
	else
	{
		bool ret = false;
		ret = queue_push_tail(mesh_event_msgq, mesh_event_duplicate);
		if(!ret)
		{
			printf(">> error: push mesh event to queue fail \n");
		}
	}
	pthread_mutex_unlock(&app_mutex);
}

void app_mesh_event_handler(MESH_BT_EVENT_T *ag_event)
{
	MESH_BT_EVENT_T *event = (MESH_BT_EVENT_T *)ag_event;
	printf("#### app handle mesh event 0x%x..\n",event->evt_id);
	switch(event->evt_id)
	{
		case MESH_EVT_PROV_SCAN_UD_RESULT:
		{
			MESH_EVT_PROV_SCAN_UD_T *pevt;
			pevt = &event->evt.mesh_evt.mesh.prov_scan_ud;
			printf(">> udb=");
			data_array_print_dump(pevt->uuid, 16);
			break;
		}
		case MESH_EVT_UD_RESULT_COMPLETE:
		{
			printf(">> stop scan \n");
			break;
		}
		case MESH_EVT_INIT_DONE:
		{
			printf(">> mesn gap enable !! \n");
			break;
		}
		case MESH_EVT_PROV_DONE:
		{
			if(0 != event->evt.mesh_evt.mesh.prov_done.reason)
			{
				printf(">> prov error %d\n",event->evt.mesh_evt.mesh.prov_done.reason);
			}
			else
			{
				MESH_EVT_PROV_DONE_T * pProvComplete;
				uint8_t index = 0;
				pProvComplete = &event->evt.mesh_evt.mesh.prov_done;
				printf(">> prov success addr 0x%x\n",pProvComplete->address);
				printf("devkey = 0x");
				for(index = 0; index < 16; index++)
				{
					printf("%02x",pProvComplete->device_key[index]);
				}
				printf("\n");
				/*get composition data */
				printf("get composition data \n");
				rtkmesh_cfg_model_compo_data_get(pProvComplete->address,0);

				node_addr = pProvComplete->address;
			}
			break;
		}
		case MESH_EVT_PROV_CAPABILITIES:
		{
			MESH_PROV_CAPABILITIES_T* pprov_capabilities = &event->evt.mesh_evt.mesh.prov_cap.cap;

			printf("prov capabilities of the device: \n");
			printf(" number_of_elements: %d\n",pprov_capabilities->number_of_elements);
			printf(" algorithms: %d\n",pprov_capabilities->algorithms);
			printf(" public_key_type: %d\n",pprov_capabilities->public_key_type);
			printf(" static_oob_type: %d\n",pprov_capabilities->static_oob_type);
			printf(" output_oob_size: %d\n",pprov_capabilities->output_oob_size);
			printf(" output_oob_action: %d\n",pprov_capabilities->output_oob_action);
			printf(" input_oob_size: %d\n",pprov_capabilities->input_oob_size);
			printf(" input_oob_action: %d\n",pprov_capabilities->input_oob_action);

			printf("!! please use \"provstart \" cmd to select prov method based on the capabilities of the device \n");
			break;
		}
		case MESH_EVT_PROV_PUBLIC_KEY:
		{
			printf("!!get the public key from the device,\n and use \"devpublickey\" cmd to notify provisioner the key");
			break;
		}
		case MESH_EVT_PROV_AUTH_DATA:
		{
            printf("!!auth method=%d action=%d size=%d type=%d \r\n>",
                            (uint8_t)(auth_method_record),
                            (uint8_t)(auth_action_record.oob_action),
                            (uint8_t)(auth_size_record.oob_size), (uint8_t)auth_value_type_record);
			switch (auth_method_record)
            {
            case PROV_AUTH_METHOD_STATIC_OOB:
                //prov_auth_value_set(auth_data, sizeof(auth_data));
                printf("Please share the oob data with the device (use \"authvalue\" cmd)\n");
                break;
            case PROV_AUTH_METHOD_OUTPUT_OOB:
                //prov_auth_value_set(auth_data, pprov_start->auth_size.output_oob_size);
                printf("Please input the oob data provided by the device, output size = %d, action = %d (use \"authvalue\" cmd)\n",
                                auth_size_record.output_oob_size, auth_action_record.output_oob_action);
                break;
            case PROV_AUTH_METHOD_INPUT_OOB:
                //prov_auth_value_set(auth_data, pprov_start->auth_size.input_oob_size);
                printf("Please output the oob data to the device, input size = %d, action = %d (use \"authvalue\" cmd)\n",
                                auth_size_record.input_oob_size, auth_action_record.input_oob_action);
                break;
            default:
                break;
            }

		}
		case MESH_EVT_HEARTBEAT:
		{
				MESH_EVT_HEARTBEAT_T* pHB_evt = &event->evt.mesh_evt.mesh.heartbeat;
				if(!pHB_evt->active)
				{
						printf("~~~ mesh node 0x%04x is off-line \n",pHB_evt->address);
				}
				else
				{
						printf("+++ mesh node 0x%04x is online \n",pHB_evt->address);
				}
				break;
		}
		default:
			break;
	}
}
int32_t app_light_lightness_client_data_recv(const mesh_model_info_p pmodel_info, uint32_t type,void *pargs)
{
	printf("### recv lightness service message...\n");
	switch (type)
    {
    case LIGHT_LIGHTNESS_CLIENT_STATUS:
        {
			MESH_MODEL_LLS_RECV_DATA_T *pdata;
			pdata = malloc(sizeof(MESH_MODEL_LLS_RECV_DATA_T));
			if(pdata == NULL)
			{
				printf("llc data recv: pdata alloc fail\n");
				return 1;
			}

			memset(pdata,0,sizeof(MESH_MODEL_LLS_RECV_DATA_T));
			pdata->type = type;
			pdata->pargs = malloc(sizeof(light_lightness_client_status_t));
			if(pdata->pargs == NULL)
			{
				printf("llc data recv: pdata->args alloc fail\n");
				free(pdata);
				pdata = NULL;
				return 2;
			}
			memcpy(pdata->pargs,pargs,sizeof(light_lightness_client_status_t));

			uint8_t event = APP_HANDLE_LLS_DATA;
			pthread_mutex_lock(&app_mutex);
			if(!(os_event_send(&app_mesh_msg_pipe[1],&event,TIME_ALAWAYS_WAIT)))
			{
				printf(">> error: write app_mesh_msg_pipe fail \n");
			}
			else
			{
				bool ret = false;
				ret = queue_push_tail(mesh_event_msgq, pdata);
				if(!ret)
				{
					printf(">> error: push mesh event to queue fail \n");
				}
			}
			pthread_mutex_unlock(&app_mutex);
        }
        break;
    default:
        break;
    }

    return 0;
}

int32_t app_light_lightness_client_data_handler(MESH_MODEL_LLS_RECV_DATA_T *p)
{
	uint32_t type = p->type;
	void *pargs = p->pargs;
	if(pargs == NULL)
	{
		printf("!!! light lightness client receive: pargs is NULL \n");
		return 1;
	}
	switch (type)
    {
    case LIGHT_LIGHTNESS_CLIENT_STATUS:
        {
            light_lightness_client_status_t *pdata = pargs;
            if (pdata->optional)
            {
                printf("light lightness client receive: src = %d, present = %d, target = %d, remain time = step(%d), resolution(%d)\r\n",
                                pdata->src,
                                pdata->present_lightness,
                                pdata->target_lightness,
                                pdata->remaining_time.num_steps,
                                pdata->remaining_time.step_resolution);
            }
            else
            {
                printf("light lightness client receive: src = %d, present = %d\r\n", pdata->src,
                                pdata->present_lightness);
            }
        }
        break;
    default:
        break;
    }
	if(pargs != NULL)
	{
		free(pargs);
		pargs = NULL;
	}
    return 0;
}

int32_t generic_on_off_client_data_recv(const mesh_model_info_p pmodel_info, uint32_t type,
                                    void *pargs)
{
	printf("### recv lightness service message...\n");


	struct itimerspec itimer;
	memset(&itimer, 0, sizeof(itimer));

	if(need_one_response)
	{
		goo_recv_count ++;
		need_one_response = 0;
	}

	timerfd_settime(waitForStateTimer_fd, 0, &itimer, NULL);

	switch (type)
	{
	case GENERIC_ON_OFF_CLIENT_STATUS:
		{
			MESH_MODEL_GOOS_RECV_DATA_T *pdata;

			rtkmesh_stop_send_access_msg(MESH_MSG_GENERIC_ON_OFF_SET );

			pdata = malloc(sizeof(MESH_MODEL_GOOS_RECV_DATA_T));
			if(pdata == NULL)
			{
				printf("goos data recv: pdata alloc fail\n");
				return 1;
			}

			memset(pdata,0,sizeof(MESH_MODEL_GOOS_RECV_DATA_T));
			pdata->type = type;
			pdata->pargs = malloc(sizeof(generic_on_off_client_status_t));
			if(pdata->pargs == NULL)
			{
				printf("goos data recv: pdata->args alloc fail\n");
				free(pdata);
				pdata = NULL;
				return 2;
			}
			memcpy(pdata->pargs,pargs,sizeof(generic_on_off_client_status_t));

			uint8_t event = APP_HANDLE_GOOS_DATA;
			pthread_mutex_lock(&app_mutex);
			if(!(os_event_send(&app_mesh_msg_pipe[1],&event,TIME_ALAWAYS_WAIT)))
			{
				printf(">> error: write app_mesh_msg_pipe fail \n");
			}
			else
			{
				bool ret = false;
				ret = queue_push_tail(mesh_event_msgq, pdata);
				if(!ret)
				{
					printf(">> error: push mesh event to queue fail \n");
				}
			}
			pthread_mutex_unlock(&app_mutex);
		}
		break;
	default:
		break;
	}

	return 0;

}

int32_t generic_on_off_client_data_handler(MESH_MODEL_GOOS_RECV_DATA_T *p)
{
		uint32_t type = p->type;
		void *pargs = p->pargs;
		if(pargs == NULL)
		{
			printf("!!! goo client receive: pargs is NULL \n");
			return 1;
		}

		switch (type)
		{
		case GENERIC_ON_OFF_CLIENT_STATUS:
			{
				generic_on_off_client_status_t *pdata = pargs;
				if (pdata->optional)
				{
					printf("goo client receive: src = 0x%x, present = %d, target = %d, remain time = step(%d), \
	resolution(%d)\r\n", pdata->src, pdata->present_on_off, pdata->target_on_off,
									pdata->remaining_time.num_steps, pdata->remaining_time.step_resolution);
				}
				else
				{
					printf("goo client receive: src = 0x%x, present = %d\r\n", pdata->src,
									pdata->present_on_off);
				}
			}
			break;
		default:
			break;
		}
		if(pargs != NULL)
		{
			free(pargs);
			pargs = NULL;
		}
		return 0;

}

int32_t light_ctl_client_data_recv(const mesh_model_info_p pmodel_info, uint32_t type,
                                    void *pargs)
{
	printf("### recv light ctl service message...\n");
	uint8_t event = APP_HANDLE_CTL_TEMPRATUE;
	MESH_MODEL_LC_RECV_DATA_T *pRecvData;
	pRecvData = malloc(sizeof(MESH_MODEL_LC_RECV_DATA_T));
	if(pRecvData == NULL)
	{
		printf("lc data recv: pdata alloc fail\n");
		return 1;
	}
	memset(pRecvData,0,sizeof(MESH_MODEL_LC_RECV_DATA_T));
	pRecvData->type = type;

	switch (type)
	{
	case LIGHT_CTL_CLIENT_STATUS:
		{
			pRecvData->pargs = malloc(sizeof(light_ctl_client_status_t));
			if(pRecvData->pargs == NULL)
			{
				printf("lc data recv: pdata->args alloc fail\n");
				free(pRecvData);
				pRecvData = NULL;
				return 2;
			}
			memcpy(pRecvData->pargs,pargs,sizeof(light_ctl_client_status_t));
		}
		break;

	case LIGHT_CTL_CLIENT_STATUS_TEMPERATURE:
		{
			pRecvData->pargs = malloc(sizeof(light_ctl_client_status_temperature_t));
			if(pRecvData->pargs == NULL)
			{
				printf("lc data recv: pdata->args alloc fail\n");
				free(pRecvData);
				pRecvData = NULL;
				return 2;
			}
			memcpy(pRecvData->pargs,pargs,sizeof(light_ctl_client_status_temperature_t));
		}
		break;

	case LIGHT_CTL_CLIENT_STATUS_TEMPERATURE_RANGE:
		{
			pRecvData->pargs = malloc(sizeof(light_ctl_client_status_temperature_range_t));
			if(pRecvData->pargs == NULL)
			{
				printf("lc data recv: pdata->args alloc fail\n");
				free(pRecvData);
				pRecvData = NULL;
				return 2;
			}
			memcpy(pRecvData->pargs,pargs,sizeof(light_ctl_client_status_temperature_range_t));
		}
		break;
	case LIGHT_CTL_CLIENT_STATUS_DEFAULT:
		{
			pRecvData->pargs = malloc(sizeof(light_ctl_client_status_default_t));
			if(pRecvData->pargs == NULL)
			{
				printf("lc data recv: pdata->args alloc fail\n");
				free(pRecvData);
				pRecvData = NULL;
				return 2;
			}
			memcpy(pRecvData->pargs,pargs,sizeof(light_ctl_client_status_default_t));
		}
		break;
	default:
		break;
	}

	pthread_mutex_lock(&app_mutex);
	if(!(os_event_send(&app_mesh_msg_pipe[1],&event,TIME_ALAWAYS_WAIT)))
	{
		printf(">> error: write app_mesh_msg_pipe fail \n");
	}
	else
	{
		bool ret = false;
		ret = queue_push_tail(mesh_event_msgq, (void*)pRecvData);
		if(!ret)
		{
			printf(">> error: push mesh event to queue fail \n");
		}
	}
	pthread_mutex_unlock(&app_mutex);
	return 0;
}

int32_t light_ctl_client_data_handler(MESH_MODEL_LC_RECV_DATA_T *p)
{
	uint32_t type = p->type;
	void *pargs = p->pargs;
	if(pargs == NULL)
	{
		printf("!!! lc client receive: pargs is NULL \n");
		return 1;
	}

	switch (type)
	{
	case LIGHT_CTL_CLIENT_STATUS:
		{
			light_ctl_client_status_t *pdata = pargs;
            if (pdata->optional)
            {
                printf("light ctl client receive: src = %d, present lightness = %d, present temperature = %d, target lightness  = %d, target temperature = %d, remain time = step(%d), resolution(%d)\r\n",
                                pdata->src,
                                pdata->present_lightness,
                                pdata->present_temperature,
                                pdata->target_lightness, pdata->target_temperature,
                                pdata->remaining_time.num_steps,
                                pdata->remaining_time.step_resolution);
            }
            else
            {
                printf("light ctl client receive: src = %d, present lightness = %d, present temperature = %d\r\n",
                                pdata->src, pdata->present_lightness, pdata->present_temperature);
            }
			break;
		}
	case LIGHT_CTL_CLIENT_STATUS_TEMPERATURE:
		{
			light_ctl_client_status_temperature_t *pdata = pargs;
            if (pdata->optional)
            {
                printf("light ctl client receive: src = %d, present temperature = %d, present delta_uv= %d, target temperature = %d, target delta_uv = %d, remain time = step(%d), resolution(%d)\r\n",
                                pdata->src,
                                pdata->present_temperature,
                                pdata->present_delta_uv,
                                pdata->target_temperature, pdata->target_delta_uv,
                                pdata->remaining_time.num_steps,
                                pdata->remaining_time.step_resolution);
            }
            else
            {
                printf("light ctl client receive: src = %d, present temperature = %d, present delta_uv = %d\r\n",
                                pdata->src, pdata->present_temperature, pdata->present_delta_uv);
            }
			break;
		}
	case LIGHT_CTL_CLIENT_STATUS_TEMPERATURE_RANGE:
		{
			light_ctl_client_status_temperature_range_t *pdata = pargs;
			printf("light ctl client receive: status = %d, min = %d, max = %d\r\n",
						pdata->status, pdata->range_min, pdata->range_max);

			break;
		}
    case LIGHT_CTL_CLIENT_STATUS_DEFAULT:
    	{
        	light_ctl_client_status_default_t *pdata = pargs;
        	printf("light ctl client receive: lightness = %d, temperature = %d, delta_uv = %d\r\n",
                        pdata->lightness, pdata->temperature, pdata->delta_uv);
		    break;
		}
	default:
		break;
	}
	if(pargs != NULL)
	{
		free(pargs);
		pargs = NULL;
	}
	return 0;
}

int32_t config_client_data_recv(const mesh_model_info_p pmodel_info, uint32_t type,
                                    void *pargs)
{
	printf("### recv config service model message...\n");
	uint8_t event = APP_HANDLE_CFG_SERVICE_STATUS;

	MESH_MODEL_RECV_DATA_T *pRecvData;
	pRecvData = malloc(sizeof(MESH_MODEL_RECV_DATA_T));
	if(pRecvData == NULL)
	{
		printf("[APP] config client data recv: pdata alloc fail\n");
		return 1;
	}

	memset(pRecvData,0,sizeof(MESH_MODEL_RECV_DATA_T));
	pRecvData->type = type;

	switch (type)
	{
		case CFG_CLIENT_BEACON_STATUS:
			{
				pRecvData->pargs = malloc(sizeof(cfg_client_status_beacon_t));
				if(pRecvData->pargs == NULL)
				{
					printf("[APP]  config client data recv: pdata->args alloc fail\n");
					free(pRecvData);
					pRecvData = NULL;
					return 2;
				}
				memcpy(pRecvData->pargs,pargs,sizeof(cfg_client_status_beacon_t));
			}
			break;
		case CFG_CLIENT_COMPO_DATA_STATUS:
			{
				cfg_client_status_compo_data_t *pData = malloc(sizeof(cfg_client_status_compo_data_t));
				cfg_client_status_compo_data_t *pCopy = pargs;

				rtkmesh_stop_send_access_msg(MESH_MSG_CFG_COMPO_DATA_GET );

				if(pData == NULL)
				{
					printf("[APP] config client data recv: pdata->args alloc fail\n");
					free(pRecvData);
					pRecvData = NULL;
					return 2;
				}
				memcpy(pData,pCopy,sizeof(cfg_client_status_compo_data_t));

				pData->pCompoData = malloc(pData->compoDataLen);
				if(!pData->pCompoData)
				{
					printf("[APP] the memory of pCompoData alloc fail\n");
					free(pData);
					pData = NULL;
					free(pRecvData);
					pRecvData = NULL;
					return 3;
				}
				memcpy(pData->pCompoData,pCopy->pCompoData,pCopy->compoDataLen);
				pRecvData->pargs = pData;
			}
			break;
		case CFG_CLIENT_DEFAULT_TTL_STATUS:
			{
				cfg_client_status_default_ttl_t *pData = malloc(sizeof(cfg_client_status_default_ttl_t));
				cfg_client_status_default_ttl_t *pCopy = pargs;


				if(pData == NULL)
				{
					printf("[APP] config client data recv: pdata->args alloc fail\n");
					free(pRecvData);
					pRecvData = NULL;
					return 2;
				}
				memcpy(pData,pCopy,sizeof(cfg_client_status_default_ttl_t));
				pRecvData->pargs = pData;
			}
			break;
		case CFG_CLIENT_PROXY_STATUS:
			{
				cfg_client_status_proxy_t *pData = malloc(sizeof(cfg_client_status_proxy_t));
				cfg_client_status_proxy_t *pCopy = pargs;
				if(pData == NULL)
				{
					printf("[APP] config client data recv: pdata->args alloc fail\n");
					free(pRecvData);
					pRecvData = NULL;
					return 2;
				}
				memcpy(pData,pCopy,sizeof(cfg_client_status_proxy_t));
				pRecvData->pargs = pData;
			}
			break;
		case CFG_CLIENT_RELAY_STATUS:
			{
				cfg_client_status_relay_t *pData = malloc(sizeof(cfg_client_status_relay_t));
				cfg_client_status_relay_t *pCopy = pargs;
				if(pData == NULL)
				{
					printf("[APP] config client data recv: pdata->args alloc fail\n");
					free(pRecvData);
					pRecvData = NULL;
					return 2;
				}
				memcpy(pData,pCopy,sizeof(cfg_client_status_relay_t));
				pRecvData->pargs = pData;
			}
			break;
		case CFG_CLIENT_NET_TRANS_STATUS:
			{
				cfg_client_status_net_transmit_t *pData = malloc(sizeof(cfg_client_status_net_transmit_t));
				cfg_client_status_net_transmit_t *pCopy = pargs;
				if(pData == NULL)
				{
					printf("[APP] config client data recv: pdata->args alloc fail\n");
					free(pRecvData);
					pRecvData = NULL;
					return 2;
				}
				memcpy(pData,pCopy,sizeof(cfg_client_status_net_transmit_t));
				pRecvData->pargs = pData;
			}
			break;
		case CFG_CLIENT_MODEL_PUB_STATUS:
			{
				cfg_client_status_model_pub_t *pData = malloc(sizeof(cfg_client_status_model_pub_t));
				cfg_client_status_model_pub_t *pCopy = pargs;
				if(pData == NULL)
				{
					printf("[APP] config client data recv: pdata->args alloc fail\n");
					free(pRecvData);
					pRecvData = NULL;
					return 2;
				}
				memcpy(pData,pCopy,sizeof(cfg_client_status_model_pub_t));
				pRecvData->pargs = pData;
			}
			break;
		case CFG_CLIENT_MODEL_SUB_STATUS:
			{
				cfg_client_status_model_sub_t *pData = malloc(sizeof(cfg_client_status_model_sub_t));
				cfg_client_status_model_sub_t *pCopy = pargs;

				if(pData == NULL)
				{
					printf("[APP] config client data recv: pdata->args alloc fail\n");
					free(pRecvData);
					pRecvData = NULL;
					return 2;
				}
				memcpy(pData,pCopy,sizeof(cfg_client_status_model_sub_t));
				pRecvData->pargs = pData;
			}
			break;
		case CFG_CLIENT_SIG_MODEL_SUB_LIST:
			{

			}
			break;
		case CFG_CLIENT_VENDOR_MODEL_SUB_LIST:
			{

			}
			break;
		case CFG_CLIENT_NET_KEY_STATUS:
			{

			}
			break;
		case CFG_CLIENT_NET_KEY_LIST:
			{

			}
			break;
		case CFG_CLIENT_APP_KEY_STATUS:
			{

			}
			break;
		case CFG_CLIENT_APP_KEY_LIST:
			{

			}
			break;
		case CFG_CLIENT_NODE_IDENTITY_STATUS:
			{

			}
			break;
		case CFG_CLIENT_MODEL_APP_STATUS:
			{

			}
			break;
		case CFG_CLIENT_SIG_MODEL_APP_LIST:
			{

			}
			break;
		case CFG_CLIENT_VENDOR_MODEL_APP_LIST:
			{

			}
			break;
		case CFG_CLIENT_NODE_RESET_STATUS:
			{

			}
			break;
		case CFG_CLIENT_FRND_STATUS:
			{

			}
			break;
		case CFG_CLIENT_LPN_POLL_TO_STATUS:
			{

			}
			break;
		case CFG_CLIENT_KEY_REFRESH_PHASE_STATUS:
			{

			}
			break;
		case CFG_CLIENT_HB_PUB_STATUS:
			{

			}
			break;
		case CFG_CLIENT_HB_SUB_STATUS:
			{

			}
			break;
		default:
			break;
	}

	pthread_mutex_lock(&app_mutex);
	if(!(os_event_send(&app_mesh_msg_pipe[1],&event,TIME_ALAWAYS_WAIT)))
	{
		printf(">> error: write app_mesh_msg_pipe fail \n");
	}
	else
	{
		bool ret = false;
		ret = queue_push_tail(mesh_event_msgq, (void*)pRecvData);
		if(!ret)
		{
			printf(">> error: push mesh event to queue fail \n");
		}
	}
	pthread_mutex_unlock(&app_mutex);
	return 0;

}
int32_t config_client_data_handler(MESH_MODEL_RECV_DATA_T *p)
{
	uint32_t type = p->type;
	void *pargs = p->pargs;
	/*if(pargs == NULL)
	{
		printf("[APP]!!!config client receive: pargs is NULL \n");
		return 1;
	} */

	switch (type)
	{
		case CFG_CLIENT_BEACON_STATUS:
			{
				cfg_client_status_beacon_t *pmsg = pargs;
				printf("[APP] beacon state: %d\r\n", pmsg->state);
				break;
			}
		case CFG_CLIENT_COMPO_DATA_STATUS:
			{
				cfg_client_status_compo_data_t *pmsg = pargs;
				if (pmsg->page == 0)
				{
					uint8_t element_index = 0;
					uint8_t *pdata = pmsg->pCompoData;
					uint8_t *pend = pdata + pmsg->compoDataLen;
					if (pmsg->compoDataLen < + 10)
					{
						goto compo_data_end;
					}
					printf("[APP] cdp0 parsed: src=0x%04x cid=0x%04x pid=0x%04x vid=0x%04x rpl=%d features=0x%04x\r\n",
									pmsg->cfg_server_addr, LE_EXTRN2WORD(pdata), LE_EXTRN2WORD(pdata + 2),
									LE_EXTRN2WORD(pdata + 4), LE_EXTRN2WORD(pdata + 6), LE_EXTRN2WORD(pdata + 8));
					pdata += 10;
					while (pdata < pend)
					{
						if (pend - pdata < 4)
						{
							goto compo_data_end;
						}
						uint8_t sig_model_num = pdata[2];
						uint8_t vendor_model_num = pdata[3];
						uint8_t model_num;
						printf("[APP] element %d: loc=%d sig=%d vendor=%d\r\n", element_index, LE_EXTRN2WORD(pdata),
										sig_model_num, vendor_model_num);
						pdata += 4;
						if (sig_model_num)
						{
							if (pend - pdata < (sig_model_num << 1))
							{
								goto compo_data_end;
							}
							printf("\ts:");
							for (model_num = 0; model_num < sig_model_num; model_num++, pdata += 2)
							{
								printf(" 0x%04xffff", LE_EXTRN2WORD(pdata));
							}
							printf("\r\n");
						}
						if (vendor_model_num)
						{
							if (pend - pdata < (vendor_model_num << 2))
							{
								goto compo_data_end;
							}
							printf("\tv:");
							for (model_num = 0; model_num < vendor_model_num; model_num++, pdata += 4)
							{
								printf(" 0x%08lx", LE_EXTRN2DWORD(pdata));
							}
							printf("\r\n");
						}
						element_index++;
					}

					if(pmsg->pCompoData != NULL)
					{
						free(pmsg->pCompoData);
						pmsg->pCompoData = NULL;
					}
					printf("---> cfg_appkey_add \n");
					usleep(500000);
					rtkmesh_cfg_appkey_add(node_addr, 0, 3);

					break;
compo_data_end:
					printf("[APP] cdp0 of 0x%04x invalid!\r\n", pmsg->cfg_server_addr);
				}

				if(pmsg->pCompoData != NULL)
				{
					free(pmsg->pCompoData);
					pmsg->pCompoData = NULL;
				}
				break;
			}
		case CFG_CLIENT_DEFAULT_TTL_STATUS:
			{
				cfg_client_status_default_ttl_t *pmsg = pargs;
				printf("[APP] default ttl: %d\r\n", pmsg->ttl);
				break;
			}
		case CFG_CLIENT_PROXY_STATUS:
			{
				cfg_client_status_proxy_t *pmsg = pargs;
				printf("[APP] proxy state: %d\r\n", pmsg->state);
				break;
			}
		case CFG_CLIENT_RELAY_STATUS:
			{
				cfg_client_status_relay_t *pmsg = pargs;
				printf("[APP] relay state: %d, count = %d step = %d\r\n", pmsg->state, pmsg->count, pmsg->steps);
				break;
			}
		case CFG_CLIENT_NET_TRANS_STATUS:
			{
				cfg_client_status_net_transmit_t *pmsg = pargs;
				printf("[APP] net transmit state: count = %d step = %d\r\n", pmsg->count, pmsg->steps);
				break;
			}
		case CFG_CLIENT_MODEL_PUB_STATUS:
			{
				cfg_client_status_model_pub_t *pmsg = pargs;
				if (pmsg->stat == MESH_MSG_STAT_SUCCESS)
				{
					printf("[APP] Pub stat: pub addr = 0x%04x ttl = %d!\r\n", pmsg->pub_addr, pmsg->pub_ttl);
				}
				else
				{
					printf("[APP] Fail, status = %d!\r\n", pmsg->stat);
				}

				break;
			}
		case CFG_CLIENT_MODEL_SUB_STATUS:
			{
				cfg_client_status_model_sub_t *pmsg = pargs;
				rtkmesh_stop_send_access_msg(MESH_MSG_CFG_MODEL_SUB_ADD );
				if (pmsg->stat == MESH_MSG_STAT_SUCCESS)
				{
					printf("[APP] Success!\r\n");
				}
				else
				{
					printf("[APP] Fail, status = %d!\r\n", pmsg->stat);
				}

				printf("baidu prov and config test is OK!\n");
				break;
			}
		case CFG_CLIENT_SIG_MODEL_SUB_LIST:
			{

			}
			break;
		case CFG_CLIENT_VENDOR_MODEL_SUB_LIST:
			{

			}
			break;
		case CFG_CLIENT_NET_KEY_STATUS:
			{

			}
			break;
		case CFG_CLIENT_NET_KEY_LIST:
			{

			}
			break;
		case CFG_CLIENT_APP_KEY_STATUS:
			{
				rtkmesh_stop_send_access_msg(MESH_MSG_CFG_APP_KEY_ADD );
				printf("---> cfg_model_appkey_bind \n");
				rtkmesh_cfg_model_appkey_bind(node_addr, 0, 0x1000ffff, 3);
			}
			break;
		case CFG_CLIENT_APP_KEY_LIST:
			{

			}
			break;
		case CFG_CLIENT_NODE_IDENTITY_STATUS:
			{

			}
			break;
		case CFG_CLIENT_MODEL_APP_STATUS:
			{
				rtkmesh_stop_send_access_msg(MESH_MSG_CFG_MODEL_APP_BIND );
				printf("---> cfg_model_sub_add \n");
				rtkmesh_cfg_model_sub_add(node_addr, 0, 0x1000ffff, 0xc000);
			}
			break;
		case CFG_CLIENT_SIG_MODEL_APP_LIST:
			{

			}
			break;
		case CFG_CLIENT_VENDOR_MODEL_APP_LIST:
			{

			}
			break;
		case CFG_CLIENT_NODE_RESET_STATUS:
			{

			}
			break;
		case CFG_CLIENT_FRND_STATUS:
			{

			}
			break;
		case CFG_CLIENT_LPN_POLL_TO_STATUS:
			{

			}
			break;
		case CFG_CLIENT_KEY_REFRESH_PHASE_STATUS:
			{

			}
			break;
		case CFG_CLIENT_HB_PUB_STATUS:
			{
				printf("recv hb pub status\n");
			}
			break;
		case CFG_CLIENT_HB_SUB_STATUS:
			{

			}
			break;
		default:
			break;

	}
	if(pargs != NULL)
	{
		free(pargs);
		pargs = NULL;
	}
	return 0;

}


bool app_generic_on_off_client_receive(mesh_msg_p pmesh_msg)
{
    bool ret = TRUE;
    uint8_t *pbuffer = pmesh_msg->pbuffer + pmesh_msg->msg_offset;
    switch (pmesh_msg->access_opcode)
    {
    case MESH_MSG_GENERIC_ON_OFF_STAT: // If the opcodes match, it must return true
        {
            generic_on_off_stat_p pmsg = (generic_on_off_stat_p)pbuffer;
            generic_on_off_client_status_t status_data;
            status_data.src = pmesh_msg->src;
            status_data.present_on_off = pmsg->present_on_off;
            status_data.optional = FALSE;

			printf("app_generic_on_off_client_receive ");
            if (pmesh_msg->msg_len == sizeof(generic_on_off_stat_t))
            {
                status_data.optional = TRUE;
                status_data.target_on_off = pmsg->target_on_off;
                status_data.remaining_time = pmsg->remaining_time;
            }

            { //same as generic_on_off_client_data_recv
				MESH_MODEL_GOOS_RECV_DATA_T *pdata;
				pdata = malloc(sizeof(MESH_MODEL_GOOS_RECV_DATA_T));
				if(pdata == NULL)
				{
					printf("goos data recv: pdata alloc fail\n");
					return true;
				}
				memset(pdata,0,sizeof(MESH_MODEL_GOOS_RECV_DATA_T));
				pdata->type = GENERIC_ON_OFF_CLIENT_STATUS;
				pdata->pargs = malloc(sizeof(generic_on_off_client_status_t));
							if(pdata->pargs == NULL)
				{
					printf("goos data recv: pdata->args alloc fail\n");
					free(pdata);
					pdata = NULL;
					return true;
				}
				memcpy(pdata->pargs,&status_data,sizeof(generic_on_off_client_status_t));

				//Throw it out and let another thread handle it
				uint8_t event = APP_HANDLE_GOOS_DATA;
				pthread_mutex_lock(&app_mutex);
				if(!(os_event_send(&app_mesh_msg_pipe[1],&event,TIME_ALAWAYS_WAIT)))
				{
					printf(">> error: write app_mesh_msg_pipe fail \n");
				}
				else
				{
					bool ret = false;
					ret = queue_push_tail(mesh_event_msgq, pdata);
					if(!ret)
					{
						printf(">> error: push mesh event to queue fail \n");
					}
				}
				pthread_mutex_unlock(&app_mutex);
            }
        }
        break;
    default:
        ret = FALSE;
        break;
    }
    return ret;
}

int32_t light_hsl_client_data_recv(const mesh_model_info_p pmodel_info, uint32_t type,
                                    void *pargs)
{
	printf("### recv light hsl service message...\n");
	uint8_t event = APP_HANDLE_HSL_DATA;
	MESH_MODEL_RECV_DATA_T *pRecvData;
	pRecvData = malloc(sizeof(MESH_MODEL_RECV_DATA_T));
	if(pRecvData == NULL)
	{
		printf("hsl data recv: pdata alloc fail\n");
		return 1;
	}
	memset(pRecvData,0,sizeof(MESH_MODEL_RECV_DATA_T));
	pRecvData->type = type;

	switch (type)
	{
	case LIGHT_HSL_CLIENT_STATUS:
		{
			pRecvData->pargs = malloc(sizeof(light_hsl_client_status_t));
			if(pRecvData->pargs == NULL)
			{
				printf("hsl data recv: pdata->args alloc fail\n");
				free(pRecvData);
				pRecvData = NULL;
				return 2;
			}
			memcpy(pRecvData->pargs,pargs,sizeof(light_hsl_client_status_t));
		}
		break;

	case LIGHT_HSL_CLIENT_STATUS_TARGET:
		{
			pRecvData->pargs = malloc(sizeof(light_hsl_client_status_t));
			if(pRecvData->pargs == NULL)
			{
				printf("hsl data recv: pdata->args alloc fail\n");
				free(pRecvData);
				pRecvData = NULL;
				return 2;
			}
			memcpy(pRecvData->pargs,pargs,sizeof(light_hsl_client_status_t));
		}
		break;

	case LIGHT_HSL_CLIENT_STATUS_HUE:
		{
			pRecvData->pargs = malloc(sizeof(light_hsl_client_status_hue_t));
			if(pRecvData->pargs == NULL)
			{
				printf("hsl data recv: pdata->args alloc fail\n");
				free(pRecvData);
				pRecvData = NULL;
				return 2;
			}
			memcpy(pRecvData->pargs,pargs,sizeof(light_hsl_client_status_hue_t));
		}
		break;
	case LIGHT_HSL_CLIENT_STATUS_SATURATION:
		{
			pRecvData->pargs = malloc(sizeof(light_hsl_client_status_saturation_t));
			if(pRecvData->pargs == NULL)
			{
				printf("hsl data recv: pdata->args alloc fail\n");
				free(pRecvData);
				pRecvData = NULL;
				return 2;
			}
			memcpy(pRecvData->pargs,pargs,sizeof(light_hsl_client_status_saturation_t));
		}
		break;
	case LIGHT_HSL_CLIENT_STATUS_DEFAULT:
		{
			pRecvData->pargs = malloc(sizeof(light_hsl_client_status_default_t));
			if(pRecvData->pargs == NULL)
			{
				printf("hsl data recv: pdata->args alloc fail\n");
				free(pRecvData);
				pRecvData = NULL;
				return 2;
			}
			memcpy(pRecvData->pargs,pargs,sizeof(light_hsl_client_status_default_t));
		}
		break;
	case LIGHT_HSL_CLIENT_STATUS_RANGE:
		{
			pRecvData->pargs = malloc(sizeof(light_hsl_client_status_range_t));
			if(pRecvData->pargs == NULL)
			{
				printf("hsl data recv: pdata->args alloc fail\n");
				free(pRecvData);
				pRecvData = NULL;
				return 2;
			}
			memcpy(pRecvData->pargs,pargs,sizeof(light_hsl_client_status_range_t));
		}
		break;
	default:
		break;
	}

	pthread_mutex_lock(&app_mutex);
	if(!(os_event_send(&app_mesh_msg_pipe[1],&event,TIME_ALAWAYS_WAIT)))
	{
		printf(">> error: write app_mesh_msg_pipe fail \n");
	}
	else
	{
		bool ret = false;
		ret = queue_push_tail(mesh_event_msgq, (void*)pRecvData);
		if(!ret)
		{
			printf(">> error: push mesh event to queue fail \n");
		}
	}
	pthread_mutex_unlock(&app_mutex);
	return 0;
}

int32_t light_hsl_client_data_handler(MESH_MODEL_RECV_DATA_T *p)
{
	uint32_t type = p->type;
	void *pargs = p->pargs;
	if(pargs == NULL)
	{
		printf("!!! hsl client receive: pargs is NULL \n");
		return 1;
	}

	switch (type)
	{
    case LIGHT_HSL_CLIENT_STATUS:
	    {
        light_hsl_client_status_t *pdata = pargs;
        if (pdata->optional)
        {
          printf("light hsl client receive: src = %d, present lightness = %d, present hue = %d, present saturation = %d, remain time = step(%d), resolution(%d)\r\n",
                          pdata->src,
                          pdata->lightness,
                          pdata->hue, pdata->saturation,
                          pdata->remaining_time.num_steps,
                          pdata->remaining_time.step_resolution);
        }
        else
        {
          printf("light hsl client receive: src = %d, present lightness = %d, present hue = %d, presnet saturation = %d\r\n",
                          pdata->src, pdata->lightness, pdata->hue, pdata->saturation);
        }
	    }
	    break;
    case LIGHT_HSL_CLIENT_STATUS_TARGET:
	    {
        light_hsl_client_status_t *pdata = pargs;
        if (pdata->optional)
        {
          printf("light hsl client receive: src = %d, target lightness = %d, target hue = %d, target saturation = %d, remain time = step(%d), resolution(%d)\r\n",
                          pdata->src,
                          pdata->lightness,
                          pdata->hue, pdata->saturation,
                          pdata->remaining_time.num_steps,
                          pdata->remaining_time.step_resolution);
        }
        else
        {
          printf("light hsl client receive: src = %d, target lightness = %d, target hue = %d, target saturation = %d\r\n",
                          pdata->src, pdata->lightness, pdata->hue, pdata->saturation);
        }
	    }
	    break;
    case LIGHT_HSL_CLIENT_STATUS_HUE:
	    {
        light_hsl_client_status_hue_t *pdata = pargs;
        if (pdata->optional)
        {
          printf("light hsl client receive: src = %d, present hue = %d, target hue = %d, remain time = step(%d), resolution(%d)\r\n",
                          pdata->src,
                          pdata->present_hue,
                          pdata->target_hue,
                          pdata->remaining_time.num_steps,
                          pdata->remaining_time.step_resolution);
        }
        else
        {
          printf("light hsl client receive: src = %d, present hue = %d\r\n",
                          pdata->src, pdata->present_hue);
        }
	    }
	    break;
		case LIGHT_HSL_CLIENT_STATUS_SATURATION:
			{
				light_hsl_client_status_saturation_t *pdata = pargs;
				if (pdata->optional)
				{
					printf("light hsl client receive: src = %d, present saturation = %d, target saturation = %d, remain time = step(%d), resolution(%d)\r\n",
													pdata->src,
													pdata->present_saturation,
													pdata->target_saturation,
													pdata->remaining_time.num_steps,
													pdata->remaining_time.step_resolution);
				}
				else
				{
					printf("light hsl client receive: src = %d, present saturation = %d\r\n",
													pdata->src, pdata->present_saturation);
				}
			}
			break;
		case LIGHT_HSL_CLIENT_STATUS_DEFAULT:
				{
					light_hsl_client_status_default_t *pdata = pargs;
					printf("light hsl client receive: lightness = %d, hue = %d, saturation = %d\r\n",
													pdata->lightness, pdata->hue, pdata->saturation);
				}
				break;
		case LIGHT_HSL_CLIENT_STATUS_RANGE:
				{
					light_hsl_client_status_range_t *pdata = pargs;
					printf("light hsl client receive: status = %d, hue min = %d, hue max = %d, saturation min = %d, saturation max = %d\r\n",
													pdata->status, pdata->hue_range_min, pdata->hue_range_max,
													pdata->saturation_range_min, pdata->saturation_range_max);
				}
				break;
		default:
			break;
	}
	if(pargs != NULL)
	{
		free(pargs);
		pargs = NULL;
	}
	return 0;
}



#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "mesh_api.h"
#include "gap_internal.h"
#include "RTKMeshTypes.h"
#include "gap_scheduler.h"
#include "provision_provisioner.h"

int first_start_flag = 1;

AppMeshBtEventCbk mesh_event_cb;

plt_timer_t discover_timer = NULL;
plt_timer_t prov_start_by_step_timer = NULL;

static uint8_t prov_dev_uuid[16];

compo_data_page0_header_t compo_data_page0_header_exceptFeature=
	    { COMPANY_ID, PRODUCT_ID, VERSION_ID };

void prov_start_by_step_handle_timeout(void )
{
	if (!pb_adv_link_open(0, prov_dev_uuid))
    	{
        	data_uart_debug("PB_ADV: Link Busy!\r\n");
    	}
	plt_timer_delete(prov_start_by_step_timer,0);
	prov_start_by_step_timer = NULL;

}
void prov_start_by_step_timeout_cb(void *ptimer)
{
	T_IO_MSG msg;

	msg.type = PROV_START_BY_STEP_TIMEOUT_MSG;
	gap_send_msg_to_app(&msg);
}
/*
*    discover devices
*/
void disc_handle_timeout(void )
{
	dev_info_show_flag = false;
	{	//for Ali
		if(mesh_event_cb != NULL)
		{
			MESH_BT_EVENT_T disc_complete;
			disc_complete.evt_id = MESH_EVT_UD_RESULT_COMPLETE;
			mesh_event_cb(&disc_complete );
		}
	}
	plt_timer_delete(discover_timer,0);
	discover_timer = NULL;

}
void discover_timeout_cb(void *ptimer)
{
	T_IO_MSG msg;

	msg.type = DISC_TIMEOUT_MSG;
	gap_send_msg_to_app(&msg);
}

int32_t user_app_mesh_modelApp_bind(uint8_t element_index, uint32_t model_id, uint32_t appKeyIndex_g, uint8_t bindAllAppKeyFlag)
{
	int32_t ret = 0;
	mesh_model_info_t* pmodelInfo = NULL;
	mesh_model_p pmodel = NULL;
	mesh_element_p pelement = NULL;

	if(bindAllAppKeyFlag == 0x02) //all model bind all application key
	{
		int i = 0;
		int j = 0;
		mesh_model_bind_all_key();

		i = 0;
		pelement = mesh_element_get(i);
		while(pelement)
		{
			j = 0;
			pmodel = mesh_model_get(i,j);
			while(pmodel)
			{
				mesh_flash_store(MESH_FLASH_PARAMS_MODEL_APP_KEY_BINDING, pmodel);
				j++;
				pmodel = mesh_model_get(i,j);
			}
			i++;
			pelement = mesh_element_get(i);
		}

		return 0;
	}
	else
	{
		pelement = mesh_element_get(element_index);
		if(pelement == NULL)
		{
			APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
			return 1;
		}
		pmodel = mesh_model_get_by_model_id(pelement, model_id);
		if(pmodel ==NULL)
		{
			APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,model_id,element_index);
			return 2;
		}
		pmodelInfo =  pmodel->pmodel_info;

		if(bindAllAppKeyFlag == 0x01) //one model bind all application key
		{
			mesh_model_bind_all(pmodelInfo,true);
			mesh_flash_store(MESH_FLASH_PARAMS_MODEL_APP_KEY_BINDING, pmodel);
			return 0;
		}

		if(bindAllAppKeyFlag == 0x00)//one model bind one application key
		{
			uint16_t app_key_array_index; // local  application key index
			app_key_array_index = app_key_index_from_global(appKeyIndex_g ); //
			if(app_key_array_index == mesh_node.app_key_num )
			{
				APP_PRINT_ERROR2("%s(): can not find appkey (global index %d)",__func__,appKeyIndex_g);
				return 3;
			}
			mesh_model_bind_one(pmodelInfo,app_key_array_index,true);
			mesh_flash_store(MESH_FLASH_PARAMS_MODEL_APP_KEY_BINDING, pmodel);
			return 0;
		}
		//bindAllAppKeyFlag is not 0x00, 0x01, 0x02
		return 4;
	}
}

int32_t user_app_mesh_unprov_scan(bool start, int32_t duration) //rtk_mesh_unprov_scanuser_cmd_parse_value_t *p
{
	if( start == true )
	{
		if(NULL == discover_timer )
		{
			discover_timer = plt_timer_create("disc", duration, false, 0, discover_timeout_cb);
			if(discover_timer == NULL)
			{
				APP_PRINT_ERROR0("discover_timer is NULL" );
				return -1;
			}
			plt_timer_start(discover_timer, 0);
			dev_info_show_flag = true;
		}
		else
		{
			return -2;
		}

	}
	else
	{
		if(discover_timer != NULL)
		{
			plt_timer_delete(discover_timer,0);
			discover_timer = NULL;
		}
		dev_info_show_flag = false;
		{	//for Ali
			if(mesh_event_cb !=NULL)
			{
				MESH_BT_EVENT_T disc_complete;
				disc_complete.evt_id = MESH_EVT_UD_RESULT_COMPLETE;

				mesh_event_cb(&disc_complete );
			}
		}
	}
	return 0;
}

int32_t user_app_mesh_stack_enable(uint8_t *localUUID)
{
	uint8_t dev_uuid[16];
	int i = 0;
	memcpy(dev_uuid,localUUID,16);
	for (i = 0; i < 16; i++) {
		printf("%x-", dev_uuid[i]);
	}
	printf("\n");
	device_uuid_set(dev_uuid);
	gap_stack_ready();
	return 0;
}

int32_t user_app_list_mesh_info(void )  //same as  user_cmd_list in mesh_cmd.c
{
    data_uart_debug("MeshState:\t%d\r\n", mesh_node.node_state);
    data_uart_debug("DevUUID:\t");
    data_uart_dump(mesh_node.dev_uuid, 16);
    uint8_t bt_addr[6];
    gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
    data_uart_debug("BTAddr:\t\t0x%02x%02x%02x%02x%02x%02x\r\n",
                    bt_addr[5], bt_addr[4], bt_addr[3], bt_addr[2], bt_addr[1], bt_addr[0]);
    uint16_t index = 0;
    for (index = 0; index < mesh_node.dev_key_num; index++)
    {
        if (mesh_node.dev_key_list[index].used && mesh_node.dev_key_list[index].element_num != 0)
        {
            data_uart_debug("DevKey:\t\t%d-0x%04x-%d-", index, mesh_node.dev_key_list[index].unicast_addr,
                            mesh_node.dev_key_list[index].element_num);
            data_uart_dump(mesh_node.dev_key_list[index].dev_key, 16);
        }
    }
    for (index = 0; index < mesh_node.app_key_num; index++)
    {
        if (mesh_node.app_key_list[index].key_state != MESH_KEY_STATE_INVALID)
        {
            data_uart_debug("AppKey:\t\t%d-0x%04x-%d-%d-%d\r\n", index,
                            mesh_node.app_key_list[index].app_key_index_g, mesh_node.app_key_list[index].key_state,
                            key_state_to_tx_loop(mesh_node.app_key_list[index].key_state),
                            mesh_node.app_key_list[index].net_key_binding);
	     uint8_t loop = 0;
            for (loop = 0; loop < 2; loop++)
            {
                if (mesh_node.app_key_list[index].papp_key[loop] != NULL)
                {
                    data_uart_debug("\t\t");
                    data_uart_dump(mesh_node.app_key_list[index].papp_key[loop]->app_key, 16);
                }
            }
        }
    }
    for (index = 0; index < mesh_node.net_key_num; index++)
    {
        if (mesh_node.net_key_list[index].key_state != MESH_KEY_STATE_INVALID)
        {
            data_uart_debug("NetKey:\t\t%d-0x%04x-%d-%d-%d\r\n", index,
                            mesh_node.net_key_list[index].net_key_index_g, mesh_node.net_key_list[index].key_state,
                            key_state_to_tx_loop(mesh_node.net_key_list[index].key_state),
                            key_state_to_key_refresh_phase(mesh_node.net_key_list[index].key_state));
            if (mesh_node.net_key_list[index].net_key_index_g & 0x8000)
            {
                break;
            }
	     uint8_t loop = 0;
            for (loop = 0; loop < 2; loop++)
            {
                if (mesh_node.net_key_list[index].pnet_key[loop] != NULL)
                {
                    data_uart_debug("\t\t");
                    data_uart_dump(mesh_node.net_key_list[index].pnet_key[loop]->net_key, 16);
                }
            }
        }
    }
    data_uart_debug("IVindex:\t%d-0x%x\r\n", mesh_node.iv_update_flag, mesh_node.iv_index);
    data_uart_debug("Seq:\t\t0x%06x\r\n", mesh_node.seq);
    data_uart_debug("NodeAddr:\t0x%04x-%d-%d\r\n", mesh_node.unicast_addr,
                    mesh_node.element_queue.count, mesh_node.model_num);
    mesh_element_p pelement = (mesh_element_p)mesh_node.element_queue.pfirst;
    while (pelement != NULL)
    {
        data_uart_debug("Element:\t%d-%d\r\n", pelement->element_index, pelement->model_queue.count);
        mesh_model_p pmodel = (mesh_model_p)pelement->model_queue.pfirst;
        while (pmodel != NULL)
        {
            data_uart_debug("Model:\t\t%d-%d-0x%08x", pmodel->pmodel_info->model_index,
                            pmodel->model_index, pmodel->pmodel_info->model_id);
            uint8_t key_flag = true;
	     uint16_t index = 0;
            for (index = 0; index < mesh_node.app_key_num; index++)
            {
                if (plt_bit_pool_get(pmodel->app_key_binding, index) &&
                    mesh_node.app_key_list[index].key_state != MESH_KEY_STATE_INVALID)
                {
                    if (key_flag)
                    {
                        key_flag = false;
                        data_uart_debug("-(key:%d", index);
                    }
                    else
                    {
                        data_uart_debug("-%d", index);
                    }
                }
            }
            if (!key_flag)
            {
                data_uart_debug(")");
            }
            if (MESH_NOT_UNASSIGNED_ADDR(pmodel->pub_params.pub_addr))
            {
                data_uart_debug("-(pub:0x%04x-%d-%d)", pmodel->pub_params.pub_addr, pmodel->pub_params.pub_ttl,
                                pmodel->pub_params.pub_key_info.app_key_index);
            }
            mesh_model_p pmodelb = pmodel;
            while (pmodelb->pmodel_info->pmodel_bound != NULL)
            {
                pmodelb = (mesh_model_p)pmodelb->pmodel_info->pmodel_bound->pmodel;
            }
            mesh_addr_member_p paddr_element = (mesh_addr_member_p)pmodelb->sub_queue.pfirst;
            while (paddr_element != NULL)
            {
                if (paddr_element == (mesh_addr_member_p)pmodelb->sub_queue.pfirst)
                {
                    if (pmodelb != pmodel)
                    {
                        data_uart_debug("-(sub:-%d-%d-0x%04x",
                                        ((mesh_model_p)pmodel->pmodel_info->pmodel_bound->pmodel)->model_index,
                                        pmodelb->model_index, paddr_element->mesh_addr);
                    }
                    else
                    {
                        data_uart_debug("-(sub:0x%04x", paddr_element->mesh_addr);
                    }
                }
                else
                {
                    data_uart_debug("-0x%04x", paddr_element->mesh_addr);
                }
                paddr_element = paddr_element->pnext;
                if (paddr_element == NULL)
                {
                    data_uart_debug(")");
                }
            }
            pmodel = pmodel->pnext;
            data_uart_debug("\r\n");
        }
        pelement = pelement->pnext;
    }
    return 0;
}

int32_t user_app_mesh_iv_init(uint32_t iv_index)
{
	APP_PRINT_INFO1("before: iv_index 0x%08x ", mesh_node.iv_index );
	if(mesh_node.iv_index > iv_index)
	{
		return -1;
	}
	else
	{
		mesh_node.iv_index = iv_index;
	}
	mesh_flash_store(MESH_FLASH_PARAMS_IV_INDEX, NULL);
	APP_PRINT_INFO1("after: iv_index 0x%08x ", mesh_node.iv_index );
	return 0;
}

int32_t user_app_mesh_seq_init(uint32_t seq_number)
{
	APP_PRINT_INFO1("before: seq: 0x%06x ", mesh_node.seq );
	if(mesh_node.seq > (seq_number&0x00ffffff) )
	{
		return -1;
	}
	else
	{
		mesh_node.seq = seq_number&0x00ffffff;
	}
	mesh_flash_store(MESH_FLASH_PARAMS_SEQ, &mesh_node.seq);
	APP_PRINT_INFO1("after: seq: 0x%06x ", mesh_node.seq );
	return 0;
}

int32_t user_app_mesh_iv_seq_get(uint32_t *pseq_num, uint32_t *piv_index)
{
	*pseq_num = mesh_node.seq;
	*piv_index = mesh_node.iv_index;
	return 0;
}

int32_t user_app_mesh_node_num_get(uint32_t *pnode_num)
{
	uint32_t node_num = 0;
	uint32_t index = 0;
    	for (index = 0; index < mesh_node.dev_key_num; index++)
    	{
       	if (mesh_node.dev_key_list[index].used && mesh_node.dev_key_list[index].element_num
			&& mesh_node.dev_key_list[index].unicast_addr != mesh_node.unicast_addr)
        	{
			node_num++;
       	}
       }
	*pnode_num= node_num;
	return 0;
}

int32_t user_app_set_hb_monitor(int32_t num, int32_t timeout)
{
	APP_PRINT_INFO2("hb pub state: count %d, period %d",num, timeout);
	if((num >= 0x01 )||(num <= 0x11))
	{
		ali_hb_pub_period = 0x01<<(num-1);
		ali_hb_monitor_timeout= timeout;
		APP_PRINT_INFO1("hb pub state: perid %ds",ali_hb_pub_period);
		return 0;
	}
    return -1;
}

int32_t user_app_mesh_get_local_elementAddr(uint8_t element_index, uint16_t *elementAddr)
{
	if(element_index < 0 || element_index > 255)
	{
		APP_PRINT_ERROR1("elementIndex is too large: %d", element_index);
		return -1;
	}

	if(element_index >= mesh_node.element_queue.count || element_index < 0)
	{
		APP_PRINT_ERROR2("elementIndex %d is bigger than element count: %d ",
									element_index, mesh_node.element_queue.count);
		return -1;
	}

	*elementAddr = (uint16_t )(mesh_node.unicast_addr + element_index);
	return 0;
}
int32_t user_app_mesh_set_netKey(int32_t netKeyIndex, uint8_t *pNetkey, bool bUpdate)
{
	uint8_t netKey[16]={0};
	memcpy(netKey, pNetkey, 16);
	int i = 0;
	printf("add network key:");
	for (i = 0; i < 16; i++) {
		printf("%x-", netKey[i]);
	}
	printf("\n");

	uint16_t net_key_arrayIndex;
	if(netKeyIndex < 0 || netKeyIndex > 0x0FFF)
	{
		APP_PRINT_ERROR0("netKeyIndex is too large");
		return -1;
	}
	net_key_arrayIndex = net_key_index_from_global((uint16_t)netKeyIndex);
	if(net_key_arrayIndex >= mesh_node.net_key_num)
	{
		// network key is not exist
		net_key_arrayIndex = net_key_add((uint16_t)netKeyIndex, netKey);
		if(net_key_arrayIndex >= mesh_node.net_key_num)
		{
			APP_PRINT_ERROR1("net key(id %d)is not exist and set failed", netKeyIndex);
			return -2;
		}
		else
		{
			APP_PRINT_INFO1("net key(id %d)is not exist and set success", netKeyIndex);
			mesh_flash_store(MESH_FLASH_PARAMS_NET_KEY_APP, &net_key_arrayIndex);
			return 0;
		}
	}
	else
	{
		// network key is exist
		if(bUpdate ==true)
		{
			{
				APP_PRINT_WARN0(" frocely changeing net key is disallowed");
			}
			return -3;
		}
		else
		{
			APP_PRINT_WARN3("net key(id %d) already exist, array index %d, update? %s",netKeyIndex, net_key_arrayIndex,bUpdate==1?"Y":"N");
			uint8_t net_key_loop = key_state_to_tx_loop(mesh_node.net_key_list[net_key_arrayIndex].key_state);
			if(memcmp(netKey,mesh_node.net_key_list[net_key_arrayIndex].pnet_key[net_key_loop]->net_key,16))
			{
				APP_PRINT_WARN1("the NetKey in arrayindex %d is different!!",net_key_arrayIndex);
				data_uart_dump(netKey,16);
				data_uart_dump(mesh_node.net_key_list[net_key_arrayIndex].pnet_key[net_key_loop]->net_key,16);
				return 6;
			}
			else
			{
				APP_PRINT_INFO1("the NetKey in arrayindex %d is same!!",net_key_arrayIndex);
				data_uart_dump(netKey,16);
				data_uart_dump(mesh_node.net_key_list[net_key_arrayIndex].pnet_key[net_key_loop]->net_key,16);
				return 0;
			}
		}
	}
	/*
	* provisioner dont need store this info? rtk mesh lib provisioner never save net key info
	* mesh_flash_store(MESH_FLASH_PARAMS_NET_KEY_APP, &net_key_arrayIndex); //
	*/
}
int32_t user_app_mesh_set_appKey(int32_t appKeyIndex, int32_t netKeyIndex, uint8_t* pAppkey, bool bUpdate)
{
	uint8_t appKey[16];

	int i = 0;
	memcpy(appKey, pAppkey, 16);
	printf("add network key:");
	for (i = 0; i < 16; i++) {
		printf("%x-", appKey[i]);
	}
	printf("\n");

	uint16_t app_key_arrayIndex;
	uint16_t net_key_arrayIndex;
	if(appKeyIndex < 0 || appKeyIndex > 0x0FFF )
	{
		APP_PRINT_ERROR0("appKeyIndex is too large");
		return -1;
	}
	if(netKeyIndex < 0 || netKeyIndex > 0x0FFF)
	{
		APP_PRINT_ERROR0("netKeyIndex is too large");
		return -2;
	}

	net_key_arrayIndex = net_key_index_from_global((uint16_t)netKeyIndex);
	if(net_key_arrayIndex >= mesh_node.net_key_num )
	{
		APP_PRINT_ERROR0("set_appKey: netKeyIndex is not exist");
		return -3;
	}

	app_key_arrayIndex = app_key_index_from_global((uint16_t )appKeyIndex);
	if(app_key_arrayIndex >= mesh_node.app_key_num)
	{
		//appkey is not exist
		app_key_arrayIndex = app_key_add(net_key_arrayIndex, appKeyIndex, appKey );
		if(app_key_arrayIndex >= mesh_node.app_key_num )
		{
			APP_PRINT_ERROR1("app key(id %d)is not exist and set failed", appKeyIndex);
			return -4;
		}
		else
		{
			APP_PRINT_INFO1("app key(id %d)is not exist and set success", appKeyIndex);
			mesh_flash_store(MESH_FLASH_PARAMS_APP_KEY_APP, &app_key_arrayIndex);
			return 0;
		}
	}
	else
	{
		//appkey is exist
		if(bUpdate == true )
		{
			APP_PRINT_WARN0(" frocely changeing net key is disallowed");
			return -5;
		}
		else
		{
			APP_PRINT_WARN3("app key(id %d) already exist, array index %d, update? %s"
						,appKeyIndex, app_key_arrayIndex,bUpdate==1?"Y":"N");

			uint8_t app_key_loop = key_state_to_tx_loop(mesh_node.app_key_list[app_key_arrayIndex].key_state);
			if(memcmp(appKey,mesh_node.app_key_list[app_key_arrayIndex].papp_key[app_key_loop]->app_key,16))
			{
				APP_PRINT_WARN1("the AppKey in arrayindex %d is different!!",app_key_arrayIndex);
				data_uart_dump(appKey,16);
				data_uart_dump(mesh_node.app_key_list[app_key_arrayIndex].papp_key[app_key_loop]->app_key,16);
				return 6;
			}
			else
			{
				APP_PRINT_WARN1("the AppKey in arrayindex %d is same!!",app_key_arrayIndex);
				data_uart_dump(appKey,16);
				data_uart_dump(mesh_node.app_key_list[app_key_arrayIndex].papp_key[app_key_loop]->app_key,16);
				return 0;
			}
		}
	}
	/*
	* provisioner dont need store this info? rtk mesh_lib provisioner never save app key info
	* mesh_flash_store(MESH_FLASH_PARAMS_APP_KEY_APP, &app_key_arrayIndex);
	*/
}
int32_t user_app_mesh_ttl_get(uint8_t *currentTTL )
{
	*currentTTL = mesh_node.ttl;
	return 0;
}
int32_t user_app_mesh_getBtMac(uint8_t* btMac)
{
	struct hci_dev_info di;
	uint8_t *addr;
	int ctl = 0;
	di.dev_id = 0;

	if ((ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI)) < 0) {
                APP_PRINT_ERROR0("Can't open HCI socket.");
                return -1;
        }

	if (ioctl(ctl, HCIGETDEVINFO, (void *) &di)) {
                APP_PRINT_ERROR0("Can't get device info");
                return -2;
        }
 	addr = (uint8_t *)&di.bdaddr;

	memcpy(btMac,addr,6);

	close(ctl);
    return 0;
}
int32_t user_app_set_mesh_mode(uint8_t enable)
{
	int32_t ret = 0;
	APP_PRINT_INFO1("Mesh mode enable = %d",enable);

	ret = (int32_t)le_vendor_aggressive_enable(enable);

	return ret;
}

int32_t user_app_set_scan_para_when_a2dp_active(uint16_t scan_interval, uint16_t scan_windows)
{
	int32_t ret = 0;
	APP_PRINT_INFO2("set default scan param interval 0x%x, windows 0x%x",scan_interval,scan_windows);

	ret = le_vendor_set_scan_para_when_a2dp_active(scan_interval, scan_windows);

	return ret;
}

int32_t user_app_set_scan_high_prioirty(uint8_t enable, uint8_t highTime )
{
	int32_t ret = 0;

	APP_PRINT_INFO0("set ble scan high prioirty");

	ret = le_vendor_set_scan_high_prioirty(enable, highTime);

	return ret;
}

int32_t user_app_set_ble_scan_param(uint16_t mesh_ble_scan_interval, uint16_t mesh_ble_scan_window)
{
	APP_PRINT_INFO2("Mesh ble scan interval = 0x%x, window = 0x%x",mesh_ble_scan_interval,mesh_ble_scan_window);

	gap_sched_params_set(GAP_SCHED_PARAMS_SCAN_INTERVAL,&mesh_ble_scan_interval,2);
	gap_sched_params_set(GAP_SCHED_PARAMS_SCAN_WINDOW,&mesh_ble_scan_window,2);

	//for driving the gap scheduler to send set_scan_param hci cmd immediately, send one beacon
	beacon_send();
	return 0;
}

int32_t user_app_stop_send_access_message(uint32_t access_opcode)
{
	gap_sched_retrans_count_clear(access_opcode);
	return 0;
}

int32_t user_app_local_mesh_node_reset(void )
{
	/*
		reset app data
	*/
	mesh_node_reset_for_baidu();
	return 0;
}
int32_t user_app_add_devKey(uint16_t addr, uint8_t element_num, uint8_t *pdevkey)
{
	int netkeyLocalIndex = 0;
	uint8_t deviceKey[16];
	int key_index;

	int i = 0;
	memcpy(deviceKey, pdevkey, 16);
	printf("add network key:");
	for (i = 0; i < 16; i++) {
		printf("%x-", deviceKey[i]);
	}
	printf("\n");

	if (MESH_UNASSIGNED_ADDR == addr)
	{
		APP_PRINT_ERROR0("Addr is MESH_UNASSIGNED_ADDR");
		return -1;
	}
	if (mesh_node.unicast_addr == addr)
	{
		element_num = mesh_node.element_queue.count;
		APP_PRINT_INFO0("Addr is provisioner self element address");
	}
	key_index = dev_key_find(addr );
	if(key_index >= 0)
	{
		uint8_t pDevkey[16];
		dev_key_get(key_index, pDevkey);
		if(memcmp(pDevkey,deviceKey,16))
		{
			APP_PRINT_ERROR1("device 0x%04x is already exist, and the new devkey is not same",addr);
			return -2;
		}
		else
		{
			APP_PRINT_WARN1("device 0x%04x is already exist, and the new devkey is same",addr);
			return 0;
		}
	}
	netkeyLocalIndex= dev_key_add(addr, element_num, deviceKey);
	if(netkeyLocalIndex < 0)
	{
		if(netkeyLocalIndex == -1)
		{
			APP_PRINT_WARN1("%s:dev_key already in the key list!",__func__);
			return 0;
		}
		else
		{
			APP_PRINT_ERROR1("%s:dev_key_list is already full, add devkey fail!",__func__);
			return -1;
		}

	}
	mesh_flash_store(MESH_FLASH_PARAMS_DEV_KEY,&netkeyLocalIndex);
	return 0;
}
int32_t user_app_delete_devKey(uint16_t addr )
{
    int dev_key_index = dev_key_find(addr);
    if (dev_key_index < 0)
    {
		APP_PRINT_WARN0("Addr is not exist, delete devkey fail!");
		return 0;
    }
    dev_key_delete(dev_key_index);
	mesh_flash_store(MESH_FLASH_PARAMS_DEV_KEY, &dev_key_index);
    return 0;
}

void modelInfo_free(mesh_model_info_p pmodel_info)
{
	plt_free(pmodel_info,RAM_TYPE_DATA_ON);
	return ;
}
int32_t user_app_set_model_data(MESH_MODEL_DATA_T *p)
{
	int32_t ret = 0;
	MESH_MODEL_DATA_T* md = p;
	MESH_ADD_MODEL_OPCODE_T opcode = md->opcode;
	APP_PRINT_TRACE2(" %s() opcode 0x%x", __func__, opcode);
	switch(opcode)
	{
		case ADD_CONFIGURATION_SERVER_MODEL_OP:
		{
			bool rret  = false;
			cfg_server_reg();

			if(rret )
			{
				ret = 0;
			}
			else
			{
				ret = 1;
			}
			break;
		}
        case ADD_LIGHTNESS_CLIENT_MODEL_OP:
        {
			bool rret = false;
			mesh_model_info_t* lightness_client = (mesh_model_info_t*)plt_malloc(sizeof(mesh_model_info_t),RAM_TYPE_DATA_ON);
			if(lightness_client == NULL)
			{
				APP_PRINT_ERROR1("%s(): there is no memory for lightness_client model info",__func__);
				ret = 2;
				goto end;
			}

			lightness_client->model_data_cb = md->callback;
			lightness_client->model_deinit = modelInfo_free;
   			rret = light_lightness_client_reg(md->element_index, lightness_client);
			if(rret )
			{
				APP_PRINT_INFO1("%s(): register lightness client model successfully",__func__);
				ret = 0;
			}
			else
			{
				APP_PRINT_ERROR1("%s(): register lightness client model fail",__func__);
				modelInfo_free(lightness_client);
				ret = 1;
			}
			break;
       	 }

		default:
		    ret = -1;
		    break;
	}

end:
	if(ret != 0 )
	{
		APP_PRINT_ERROR2("set_model_data: opcode 0x%x  error code %d ",opcode ,ret );
	}
	else
	{
		if( true == compo_data_page0_gen(&compo_data_page0_header_exceptFeature) )
		{
			ret = 0;
		}
		else
		{
			ret = -5;
		}
	}

	return ret;
}

mesh_msg_send_cause_t access_send_direct(int32_t dst, int32_t dstAddrType, int32_t src, int32_t ttl,
						int32_t netKeyIndex, int32_t appKeyIndex, uint8_t* data, int32_t data_len)
{
	//dstAddrType netKeyIndex is useless
	mesh_msg_send_cause_t ret = MESH_MSG_SEND_CAUSE_SUCCESS;
	mesh_msg_t mesh_msg;
	memset(&mesh_msg,0, sizeof(mesh_msg_t));

	uint8_t buff[512];
	memset(buff,0,512);

	mesh_msg.pmodel_info = NULL;
	access_cfg(&mesh_msg);
	mesh_msg.dst = dst;
	mesh_msg.src = src;
	mesh_msg.ttl = (ttl&0x7F);

	if(512 < data_len)
	{
		return MESH_MSG_SEND_CAUSE_NO_BUFFER_AVAILABLE;
	}

	memcpy(buff,data,data_len);
	mesh_msg.pbuffer = buff;

	mesh_msg.msg_offset = 0;
	mesh_msg.msg_len = data_len;
	mesh_msg.net_key_index = net_key_index_from_global(netKeyIndex);
	mesh_msg.app_key_index = app_key_index_from_global(appKeyIndex); //appkey binding netkey

	//APP_PRINT_TRACE0(" send data =");
	//data_uart_dump(buff,data_len);

	ret = access_send(&mesh_msg);
    return ret;
}

int32_t user_app_send_packet(int32_t dst, int32_t dstAddrType, int32_t src, int32_t ttl,
						int32_t netKeyIndex, int32_t appKeyIndex, uint8_t* data, int32_t data_len)
{
	int32_t ret = 0;

	ret = access_send_direct(dst, dstAddrType, src, ttl, netKeyIndex, appKeyIndex, data, data_len);
	if(ret != 0)
	{
		APP_PRINT_ERROR0("send vendor packet failed !!!");
	}
	return ret;
}

int32_t  user_app_cfg_app_key_add(uint16_t dst, uint16_t netKeyIndex_g, uint16_t appKeyIndex_g )
{

	uint16_t net_key_index = net_key_index_from_global(netKeyIndex_g);
    uint16_t app_key_index = app_key_index_from_global(appKeyIndex_g);

    if (net_key_index >= mesh_node.net_key_num ||
        mesh_node.net_key_list[net_key_index].key_state == MESH_KEY_STATE_INVALID)
    {
		APP_PRINT_ERROR1("%s(): netkey is not exist",__func__);
		return 1;
    }
    if (app_key_index >= mesh_node.app_key_num ||
        mesh_node.app_key_list[app_key_index].key_state == MESH_KEY_STATE_INVALID)
    {
		APP_PRINT_ERROR1("%s(): appkey is not exist",__func__);
		return 2;
    }
    cfg_app_key_add(dst, mesh_node.net_key_list[net_key_index].net_key_index_g,
                    mesh_node.app_key_list[app_key_index].app_key_index_g,
                    mesh_node.app_key_list[app_key_index].papp_key[key_state_to_new_loop(
                                                                       mesh_node.app_key_list[app_key_index].key_state)]->app_key);
    return 0;
}

int32_t  user_app_cfg_model_app_bind(uint16_t dst, uint8_t element_index, uint32_t model_id, uint16_t appKeyIndex_g)
{
    uint16_t app_key_index = app_key_index_from_global(appKeyIndex_g);

    if (app_key_index >= mesh_node.app_key_num ||
        mesh_node.app_key_list[app_key_index].key_state == MESH_KEY_STATE_INVALID)
    {
		APP_PRINT_ERROR1("%s(): appkey is not exist",__func__);
		return 1;
    }
    cfg_model_app_bind(dst, dst + element_index, mesh_node.app_key_list[app_key_index].app_key_index_g,
                       model_id);
    return 0;
}
int32_t user_app_cfg_model_net_trans_set(uint16_t dst, uint8_t count, uint8_t steps)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}
	ret = (int32_t)cfg_net_transmit_set(dst, count, steps);

	return ret;
}
int32_t user_app_cfg_model_net_trans_get(uint16_t dst )
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}

	ret =(int32_t )cfg_net_transmit_get(dst);
	return ret;
}
int32_t user_app_cfg_model_beacon_get(uint16_t dst )
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}

	ret =(int32_t )cfg_beacon_get(dst);
	return ret;

}
int32_t user_app_cfg_model_beacon_set(uint16_t dst, uint8_t state)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}

	ret =(int32_t )cfg_beacon_set(dst,state);
	return ret;

}
int32_t user_app_cfg_model_compo_data_get(uint16_t dst, uint8_t page )
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}
	APP_PRINT_INFO2("get composition data 0x%x, page %d",dst,page);
	ret =(int32_t )cfg_compo_data_get(dst, page);
	return ret;

}
int32_t user_app_cfg_model_default_ttl_get(uint16_t dst)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}

	ret =(int32_t )cfg_default_ttl_get(dst );

	return ret;
}
int32_t user_app_cfg_model_default_ttl_set(uint16_t dst, uint8_t ttl)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}

	ret =(int32_t )cfg_default_ttl_set(dst, ttl);

	return ret;

}
int32_t user_app_cfg_model_proxy_stat_get(uint16_t dst)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}

	ret =(int32_t )cfg_proxy_get(dst );

	return ret;

}
int32_t user_app_cfg_model_proxy_stat_set(uint16_t dst, uint8_t proxy_state)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}

	ret =(int32_t )cfg_proxy_set(dst, proxy_state );

	return ret;

}
int32_t user_app_cfg_model_relay_get(uint16_t dst)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}

	ret =(int32_t )cfg_relay_get(dst );

	return ret;

}
int32_t user_app_cfg_model_relay_set(uint16_t dst, uint8_t state, uint8_t count, uint8_t steps)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}

	ret =(int32_t )cfg_relay_set(dst, state, count, steps );

	return ret;

}

int32_t user_app_cfg_model_pub_get(uint16_t dst, uint16_t element_index, uint32_t model_id)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}
	ret = (int32_t)cfg_model_pub_get(dst, dst+element_index, model_id);

	return ret;
}

int32_t user_app_cfg_model_pub_set(uint16_t dst, uint16_t element_index, uint16_t publish_addr, pub_key_info_t pub_key_info, uint8_t ttl,
							pub_period_t pub_period, pub_retrans_info_t pub_retrans_info, uint32_t model_id )
{
	bool va_flag = 0; // va_flag=1: virtual address
	int32_t ret;

	uint8_t pub_addr[16];
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}
    if (0 == va_flag)
    {
        LE_WORD2EXTRN(pub_addr, publish_addr);
    }
    ret = (int32_t)cfg_model_pub_set(dst, dst+element_index, va_flag,
                              pub_addr, pub_key_info, ttl, pub_period,
                              pub_retrans_info, model_id);
	return ret;
}

int32_t user_app_cfg_model_vir_pub_set(uint16_t dst, uint16_t element_index, uint8_t publish_addr[16], pub_key_info_t pub_key_info, uint8_t ttl,
							pub_period_t pub_period, pub_retrans_info_t pub_retrans_info, uint32_t model_id )
{
	bool va_flag = 1; // va_flag=1: virtual address
	int32_t ret;

	uint8_t pub_addr[16];
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}
    if (1 == va_flag)
    {
        memcpy(pub_addr, publish_addr, 16);
    }
    ret = (int32_t)cfg_model_pub_set(dst, dst+element_index, va_flag,
                              pub_addr, pub_key_info, ttl, pub_period,
                              pub_retrans_info, model_id);
	return ret;
}

int32_t user_app_cfg_model_sub_add(uint16_t dst, uint8_t element_index, uint32_t model_id, uint16_t group_addr)
{
	int32_t ret;

	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}

	if (MESH_NOT_GROUP_ADDR(group_addr))
    {
        return 2;
    }
    ret = (int32_t)cfg_model_sub_add(dst, dst + element_index, false, (uint8_t *)&group_addr, model_id);
    return ret;
}

int32_t user_app_cfg_model_vir_sub_add(uint16_t dst, uint8_t element_index, uint32_t model_id, uint8_t group_addr[16])
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}
    ret = (int32_t)cfg_model_sub_add(dst, dst + element_index, true, group_addr, model_id);
    return ret;
}

int32_t user_app_cfg_model_sub_delete(uint16_t dst, uint8_t element_index, uint32_t model_id, uint16_t group_addr)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}
	if (MESH_NOT_GROUP_ADDR(group_addr))
	{
		return 2;
    }
    ret = (int32_t)cfg_model_sub_delete(dst, dst + element_index, false, (uint8_t *)&group_addr, model_id);
    return ret;
}
int32_t user_app_cfg_model_vir_sub_delete(uint16_t dst, uint8_t element_index, uint32_t model_id, uint8_t group_addr[16])
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}
    ret = (int32_t)cfg_model_sub_delete(dst, dst + element_index, true, group_addr, model_id);
    return ret;
}
int32_t user_app_cfg_model_sub_overwrite(uint16_t dst, uint16_t element_index, uint16_t sub_addr, uint32_t model_id)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}
	ret = (int32_t)cfg_model_sub_overwrite( dst, dst + element_index, false, (uint8_t *)&sub_addr, model_id);
	return ret;
}
int32_t user_app_cfg_model_vir_sub_overwrite(uint16_t dst, uint16_t element_index, uint8_t sub_addr[16], uint32_t model_id)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}
	ret = (int32_t)cfg_model_sub_overwrite( dst, dst + element_index, true, sub_addr, model_id);
	return ret;
}

int32_t user_app_cfg_model_sub_delete_all(uint16_t dst, uint16_t element_index,
												   uint32_t model_id)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}
	ret = (int32_t)cfg_model_sub_delete_all( dst, dst + element_index, model_id);
	return ret;
}

int32_t user_app_cfg_model_hb_pub_set(uint16_t dst, uint16_t dst_pub, uint8_t count_log,
                                     uint8_t period_log,
                                     uint8_t ttl, hb_pub_features_t features, uint16_t net_key_index)
{
	int32_t ret;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}
	ret = (int32_t)cfg_hb_pub_set(dst, dst_pub, count_log, period_log, ttl, features, net_key_index);
	return ret;
}

int32_t user_app_cfg_model_node_reset(uint16_t dst )
{
	int32_t ret;
	//int key_index;
	if(MESH_NOT_UNICAST_ADDR(dst))
	{
		APP_PRINT_ERROR1("dst addr is not unicast address: 0x%x",dst);
		return 1;
	}
	ret = (int32_t)cfg_node_reset(dst);
	//key_index = dev_key_find(dst);
	//dev_key_delete(key_index);
	//mesh_flash_store(MESH_FLASH_PARAMS_DEV_KEY, &key_index);
	return ret;
}

int32_t user_app_generic_on_off_set(uint16_t element_index, uint16_t dst, uint16_t appKeyIndex_g, generic_on_off_t on_off,
									bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
    static uint8_t tid = 0;
	if(tid == 0)
	{
		tid = (uint8_t)(mesh_node.seq & 0xff);
	}
    tid++;
	if(tid == 0)
	{
		tid++;
	}

	mesh_model_info_t* pmodelInfo = NULL;
	mesh_model_p pmodel = NULL;
	mesh_element_p pelement = NULL;

	uint16_t app_key_index = app_key_index_from_global(appKeyIndex_g);
	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_GENERIC_ON_OFF_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_LIGHTNESS_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret = (int32_t)generic_on_off_set(pmodelInfo, dst, app_key_index, on_off,
                       tid, optional, trans_time, delay, ack);
    return ret;
}

int32_t user_app_generic_on_off_get(uint16_t element_index, uint16_t dst, uint16_t appKeyIndex_g )
{
	int32_t ret = 0;
	mesh_model_info_t* pmodelInfo = NULL;
	mesh_model_p pmodel = NULL;
	mesh_element_p pelement = NULL;

	uint16_t app_key_index = app_key_index_from_global(appKeyIndex_g);
	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_GENERIC_ON_OFF_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_LIGHTNESS_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

	ret = (int32_t)generic_on_off_get(pmodelInfo, dst, app_key_index);
    return ret;
}

int32_t user_app_light_lightness_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	uint16_t app_key_index;
    mesh_model_info_t* pmodelInfo = NULL;
    mesh_model_p pmodel = NULL;
    mesh_element_p pelement = NULL;

    app_key_index = app_key_index_from_global(appKeyIndex_g);

	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_LIGHTNESS_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_LIGHTNESS_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret = (int32_t)light_lightness_get(pmodelInfo, dst, app_key_index);

    return ret;
}

int32_t user_app_light_lightness_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t lightness,
								bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
    static uint8_t tid = 0;
	if(tid == 0)
	{
		tid = (uint8_t)(mesh_node.seq & 0xff);
	}
    tid++;
	if(tid == 0)
	{
		tid++;
	}

    uint16_t app_key_index;
    mesh_model_info_t* pmodelInfo = NULL;
    mesh_model_p pmodel = NULL;
    mesh_element_p pelement = NULL;

	app_key_index = app_key_index_from_global(appKeyIndex_g);
	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_LIGHTNESS_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_LIGHTNESS_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret =(int32_t)light_lightness_set(pmodelInfo, dst, app_key_index,lightness, tid, optional,
							trans_time,delay, ack);
    return ret;
}

int32_t user_app_light_hsl_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	uint16_t app_key_index;
  mesh_model_info_t* pmodelInfo = NULL;
  mesh_model_p pmodel = NULL;
  mesh_element_p pelement = NULL;

  app_key_index = app_key_index_from_global(appKeyIndex_g);

	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

  pelement = mesh_element_get(element_index);
  if(pelement == NULL)
  {
    APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
		return 2;
  }
  pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_HSL_CLIENT);
  if(pmodel ==NULL)
  {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_HSL_CLIENT,element_index);
	  return 3;
  }
  pmodelInfo =  pmodel->pmodel_info;

  ret = (int32_t)light_hsl_get(pmodelInfo, dst, app_key_index);
  return ret;
}

int32_t user_app_light_hsl_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t lightness, uint16_t hue,
								int16_t saturation,bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
    static uint8_t tid = 0;
	if(tid == 0)
	{
		tid = (uint8_t)(mesh_node.seq & 0xff);
	}
    tid++;
	if(tid == 0)
	{
		tid++;
	}

	uint16_t app_key_index;
  mesh_model_info_t* pmodelInfo = NULL;
	mesh_model_p pmodel = NULL;
  mesh_element_p pelement = NULL;

	app_key_index = app_key_index_from_global(appKeyIndex_g);
	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_HSL_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_HSL_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret =(int32_t)light_hsl_set(pmodelInfo, dst, app_key_index, lightness, hue, saturation, tid, optional,
							trans_time,delay, ack);
    return ret;
}

int32_t user_app_light_hsl_target_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	uint16_t app_key_index;
  mesh_model_info_t* pmodelInfo = NULL;
  mesh_model_p pmodel = NULL;
  mesh_element_p pelement = NULL;

  app_key_index = app_key_index_from_global(appKeyIndex_g);

	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

  pelement = mesh_element_get(element_index);
  if(pelement == NULL)
  {
    APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
		return 2;
  }
  pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_HSL_CLIENT);
  if(pmodel ==NULL)
  {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_HSL_CLIENT,element_index);
	  return 3;
  }
  pmodelInfo =  pmodel->pmodel_info;

  ret = (int32_t)light_hsl_target_get(pmodelInfo, dst, app_key_index);
  return ret;
}

int32_t user_app_light_hsl_hue_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	uint16_t app_key_index;
  mesh_model_info_t* pmodelInfo = NULL;
  mesh_model_p pmodel = NULL;
  mesh_element_p pelement = NULL;

  app_key_index = app_key_index_from_global(appKeyIndex_g);

	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

  pelement = mesh_element_get(element_index);
  if(pelement == NULL)
  {
    APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
		return 2;
  }
  pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_HSL_CLIENT);
  if(pmodel ==NULL)
  {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_HSL_CLIENT,element_index);
	  return 3;
  }
  pmodelInfo =  pmodel->pmodel_info;

  ret = (int32_t)light_hsl_hue_get(pmodelInfo, dst, app_key_index);
  return ret;
}

int32_t user_app_light_hsl_hue_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t hue,
								bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
    static uint8_t tid = 0;
	if(tid == 0)
	{
		tid = (uint8_t)(mesh_node.seq & 0xff);
	}
    tid++;
	if(tid == 0)
	{
		tid++;
	}

	uint16_t app_key_index;
  mesh_model_info_t* pmodelInfo = NULL;
	mesh_model_p pmodel = NULL;
  mesh_element_p pelement = NULL;

	app_key_index = app_key_index_from_global(appKeyIndex_g);
	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_HSL_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_HSL_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret =(int32_t)light_hsl_hue_set(pmodelInfo, dst, app_key_index, hue, tid, optional, trans_time,delay, ack);
    return ret;
}

int32_t user_app_light_hsl_saturation_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	uint16_t app_key_index;
  mesh_model_info_t* pmodelInfo = NULL;
  mesh_model_p pmodel = NULL;
  mesh_element_p pelement = NULL;

  app_key_index = app_key_index_from_global(appKeyIndex_g);

	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

  pelement = mesh_element_get(element_index);
  if(pelement == NULL)
  {
    APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
		return 2;
  }
  pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_HSL_CLIENT);
  if(pmodel ==NULL)
  {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_HSL_CLIENT,element_index);
	  return 3;
  }
  pmodelInfo =  pmodel->pmodel_info;

  ret = (int32_t)light_hsl_saturation_get(pmodelInfo, dst, app_key_index);
  return ret;
}

int32_t user_app_light_hsl_saturation_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t saturation,
								bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
  static uint8_t tid = 0;
	if(tid == 0)
	{
		tid = (uint8_t)(mesh_node.seq & 0xff);
	}
    tid++;
	if(tid == 0)
	{
		tid++;
	}

	uint16_t app_key_index;
  mesh_model_info_t* pmodelInfo = NULL;
	mesh_model_p pmodel = NULL;
  mesh_element_p pelement = NULL;

	app_key_index = app_key_index_from_global(appKeyIndex_g);
	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_HSL_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_HSL_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret =(int32_t)light_hsl_saturation_set(pmodelInfo, dst, app_key_index, saturation, tid, optional, trans_time,delay, ack);
    return ret;
}

int32_t user_app_light_hsl_default_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	uint16_t app_key_index;
  mesh_model_info_t* pmodelInfo = NULL;
  mesh_model_p pmodel = NULL;
  mesh_element_p pelement = NULL;

  app_key_index = app_key_index_from_global(appKeyIndex_g);

	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

  pelement = mesh_element_get(element_index);
  if(pelement == NULL)
  {
    APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
		return 2;
  }
  pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_HSL_CLIENT);
  if(pmodel ==NULL)
  {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_HSL_CLIENT,element_index);
	  return 3;
  }
  pmodelInfo =  pmodel->pmodel_info;

  ret = (int32_t)light_hsl_default_get(pmodelInfo, dst, app_key_index);
  return ret;
}

int32_t user_app_light_hsl_default_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t lightness,
								 														uint16_t hue, uint16_t saturation, bool ack)
{
	int32_t ret = 0;
  static uint8_t tid = 0;
	if(tid == 0)
	{
		tid = (uint8_t)(mesh_node.seq & 0xff);
	}
    tid++;
	if(tid == 0)
	{
		tid++;
	}

	uint16_t app_key_index;
  mesh_model_info_t* pmodelInfo = NULL;
	mesh_model_p pmodel = NULL;
  mesh_element_p pelement = NULL;

	app_key_index = app_key_index_from_global(appKeyIndex_g);
	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_HSL_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_HSL_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret =(int32_t)light_hsl_default_set(pmodelInfo, dst, app_key_index, lightness, hue, saturation, ack);
    return ret;
}

int32_t user_app_light_hsl_range_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	uint16_t app_key_index;
  mesh_model_info_t* pmodelInfo = NULL;
  mesh_model_p pmodel = NULL;
  mesh_element_p pelement = NULL;

  app_key_index = app_key_index_from_global(appKeyIndex_g);

	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

  pelement = mesh_element_get(element_index);
  if(pelement == NULL)
  {
    APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
		return 2;
  }
  pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_HSL_CLIENT);
  if(pmodel ==NULL)
  {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_HSL_CLIENT,element_index);
	  return 3;
  }
  pmodelInfo =  pmodel->pmodel_info;

  ret = (int32_t)light_hsl_range_get(pmodelInfo, dst, app_key_index);
  return ret;
}

int32_t user_app_light_hsl_range_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t hue_range_min,
								uint16_t hue_range_max, uint16_t saturation_range_min, uint16_t saturation_range_max, bool ack)
{
	int32_t ret = 0;
  static uint8_t tid = 0;
	if(tid == 0)
	{
		tid = (uint8_t)(mesh_node.seq & 0xff);
	}
    tid++;
	if(tid == 0)
	{
		tid++;
	}

	uint16_t app_key_index;
  mesh_model_info_t* pmodelInfo = NULL;
	mesh_model_p pmodel = NULL;
  mesh_element_p pelement = NULL;

	app_key_index = app_key_index_from_global(appKeyIndex_g);
	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_HSL_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_HSL_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret =(int32_t)light_hsl_range_set(pmodelInfo, dst, app_key_index, hue_range_min, hue_range_max, saturation_range_min, saturation_range_max, ack);
    return ret;
}

int32_t user_app_light_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	uint16_t app_key_index;
	mesh_model_info_t* pmodelInfo = NULL;
	mesh_model_p pmodel = NULL;
	mesh_element_p pelement = NULL;

	app_key_index = app_key_index_from_global(appKeyIndex_g);

	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

	pelement = mesh_element_get(element_index);
	if(pelement == NULL)
	{
		 APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
	}
	pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_CTL_CLIENT);
	if(pmodel ==NULL)
	{
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_CTL_CLIENT,element_index);
	  return 3;
	}
	pmodelInfo =  pmodel->pmodel_info;

	ret = (int32_t)light_ctl_get(pmodelInfo, dst, app_key_index);

	return ret;
}

int32_t user_app_light_ctl_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t lightness, uint16_t temperature,
								int16_t delta_uv,bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
    static uint8_t tid = 0;
	if(tid == 0)
	{
		tid = (uint8_t)(mesh_node.seq & 0xff);
	}
    tid++;
	if(tid == 0)
	{
		tid++;
	}


    uint16_t app_key_index;
    mesh_model_info_t* pmodelInfo = NULL;
    mesh_model_p pmodel = NULL;
    mesh_element_p pelement = NULL;

	app_key_index = app_key_index_from_global(appKeyIndex_g);
	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_CTL_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_CTL_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret =(int32_t)light_ctl_set(pmodelInfo, dst, app_key_index, lightness, temperature, delta_uv, tid, optional,
							trans_time,delay, ack);
    return ret;
}

int32_t user_app_light_ctl_temperature_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	uint16_t app_key_index;
    mesh_model_info_t* pmodelInfo = NULL;
    mesh_model_p pmodel = NULL;
    mesh_element_p pelement = NULL;

    app_key_index = app_key_index_from_global(appKeyIndex_g);

	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_CTL_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_CTL_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret = (int32_t)light_ctl_temperature_get(pmodelInfo, dst, app_key_index);

    return ret;
}

int32_t user_app_light_ctl_temperature_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t temperature,
								int16_t delta_uv,bool optional, generic_transition_time_t trans_time, uint8_t delay, bool ack)
{
	int32_t ret = 0;
    static uint8_t tid = 0;
	if(tid == 0)
	{
		tid = (uint8_t)(mesh_node.seq & 0xff);
	}
    tid++;
	if(tid == 0)
	{
		tid++;
	}


    uint16_t app_key_index;
    mesh_model_info_t* pmodelInfo = NULL;
    mesh_model_p pmodel = NULL;
    mesh_element_p pelement = NULL;

	app_key_index = app_key_index_from_global(appKeyIndex_g);
	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_CTL_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_CTL_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret =(int32_t)light_ctl_temperature_set(pmodelInfo, dst, app_key_index,temperature, delta_uv, tid, optional,
							trans_time,delay, ack);
    return ret;
}

int32_t user_app_light_ctl_temperature_range_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	uint16_t app_key_index;
    mesh_model_info_t* pmodelInfo = NULL;
    mesh_model_p pmodel = NULL;
    mesh_element_p pelement = NULL;

    app_key_index = app_key_index_from_global(appKeyIndex_g);

	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_CTL_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_CTL_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret = (int32_t)light_ctl_temperature_range_get(pmodelInfo, dst, app_key_index);

    return ret;
}

int32_t user_app_light_ctl_temperature_range_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t range_min,
																uint16_t range_max, bool ack)
{
	int32_t ret = 0;
    static uint8_t tid = 0;
	if(tid == 0)
	{
		tid = (uint8_t)(mesh_node.seq & 0xff);
	}
    tid++;
	if(tid == 0)
	{
		tid++;
	}


    uint16_t app_key_index;
    mesh_model_info_t* pmodelInfo = NULL;
    mesh_model_p pmodel = NULL;
    mesh_element_p pelement = NULL;

	app_key_index = app_key_index_from_global(appKeyIndex_g);
	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_CTL_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_CTL_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret =(int32_t)light_ctl_temperature_range_set(pmodelInfo, dst, app_key_index, range_min, range_max, ack);
    return ret;
}

int32_t user_app_light_ctl_default_get(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g)
{
	int32_t ret = 0;
	uint16_t app_key_index;
    mesh_model_info_t* pmodelInfo = NULL;
    mesh_model_p pmodel = NULL;
    mesh_element_p pelement = NULL;

    app_key_index = app_key_index_from_global(appKeyIndex_g);

	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_CTL_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_CTL_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret = (int32_t)light_ctl_default_get(pmodelInfo, dst, app_key_index);

    return ret;
}

int32_t user_app_light_ctl_default_set(uint8_t element_index, uint16_t dst, uint16_t appKeyIndex_g, uint16_t lightness,
																uint16_t temperature, int16_t delta_uv, bool ack)
{
	int32_t ret = 0;

    uint16_t app_key_index;
    mesh_model_info_t* pmodelInfo = NULL;
    mesh_model_p pmodel = NULL;
    mesh_element_p pelement = NULL;

	app_key_index = app_key_index_from_global(appKeyIndex_g);
	if(app_key_index >= mesh_node.app_key_num)
	{
		APP_PRINT_ERROR1("%s(): app key is not exist",__func__);
		return 1;
	}

    pelement = mesh_element_get(element_index);
    if(pelement == NULL)
    {
         APP_PRINT_ERROR2("%s(): can not find element (index %d)",__func__,element_index);
	  return 2;
    }
    pmodel = mesh_model_get_by_model_id(pelement, MESH_MODEL_LIGHT_CTL_CLIENT);
    if(pmodel ==NULL)
    {
	  APP_PRINT_ERROR3("%s(): can not find model 0x%x in element (index %d)",__func__,MESH_MODEL_LIGHT_CTL_CLIENT,element_index);
	  return 3;
    }
    pmodelInfo =  pmodel->pmodel_info;

    ret =(int32_t)light_ctl_default_set(pmodelInfo, dst, app_key_index, lightness, temperature, delta_uv, ack);
    return ret;
}

int32_t user_app_prov(uint8_t* deviceUUID, uint16_t assign_address, uint32_t attention_dur, bool enable) //same as user_cmd_prov in  provision_cmd.c
{
    	data_uart_debug("provision...\r\n");
	int devkey_index = -1;
	prov_ctx_p pprov_ctx = &prov_ctx;

	if(pprov_ctx->fsm != PROV_INVITE)
	{
		APP_PRINT_WARN0("PB-ADV link termination early");
		prov_timer_stop();
		pprov_ctx->fsm = PROV_INVITE;
		prov_disconnect(PB_ADV_LINK_CLOSE_PROVISIONING_FAIL);
	}

	assign_addr = 0;
	if(assign_address != 0x00)
	{
		if(MESH_NOT_UNICAST_ADDR(assign_address))
		{
			APP_PRINT_ERROR1("assign_address 0x%x is not unicast address ",assign_address);
			return 1;
		}
		if(MESH_IS_MY_ADDR(assign_address))
		{
			APP_PRINT_ERROR1("assign_address 0x%x is provisioner address ",assign_address);
			return 2;
		}

		memcpy(prov_dev_uuid,deviceUUID,16);
		assign_addr = assign_address;
    		attn_dur = attention_dur;
    		prov_manual = enable;

		devkey_index = dev_key_find(assign_address);
		if(devkey_index >= 0 )
		{
			cfg_node_reset(mesh_node.dev_key_list[devkey_index].unicast_addr);

			dev_key_delete(devkey_index);
			mesh_flash_store(MESH_FLASH_PARAMS_DEV_KEY, &devkey_index);

			APP_PRINT_WARN1("assign_address 0x%x is already exist, now overwrite it",assign_address);

			if(!prov_start_by_step_timer )
			{
				prov_start_by_step_timer = plt_timer_create("prov", 300 , false, 0, prov_start_by_step_timeout_cb);
			}
			if(prov_start_by_step_timer )
			{
				plt_timer_start(prov_start_by_step_timer, 0);
			}
			else
			{
				APP_PRINT_ERROR0("prov start by step timer create fail");
				return -2;
			}
			return 0;
		}
	}

    if (!pb_adv_link_open(0, prov_dev_uuid))
    {
        data_uart_debug("PB_ADV: Link Busy!\r\n");
    }
    return 0;
}

int32_t user_app_prov_method_choose(prov_start_public_key_t public_key, prov_auth_method_t auth_method,
											prov_auth_action_t auth_action, prov_auth_size_t auth_size)
{
	bool ret;
	prov_start_t prov_start;

	data_uart_debug("provision start...\r\n");
	APP_PRINT_INFO4("public_key:%d , auth_method:%d, auth_action:%d, auth_size:%d \n",(uint8_t)public_key, (uint8_t)auth_method, (uint8_t)auth_action.oob_action, (uint8_t)auth_size.oob_size);

	memset(&prov_start,0,sizeof(prov_start_t));
	prov_start.algorithm = 0;
	prov_start.public_key = public_key;
	prov_start.auth_method = auth_method;
	prov_start.auth_action = auth_action;
	prov_start.auth_size = auth_size;

	ret = prov_path_choose(&prov_start);
	if(!ret)
	{
		return 1;
	}
    return 0;
}

int32_t user_app_prov_device_public_key_set(uint8_t public_key[64])
{
	bool ret;

	ret = prov_device_public_key_set(public_key);
	if(!ret)
	{
		return 1;
	}
	return 0;
}

int32_t user_app_prov_auth_value_set(uint8_t* pvalue, uint8_t len)
{
	bool ret;

	ret = prov_auth_value_set(pvalue, len);
	if(!ret)
	{
		return 1;
	}
	return 0;

}

int32_t user_app_set_intensive_adv(uint32_t interval, uint32_t duration, uint32_t adjust_interval, uint32_t scan_window_per_interval )
{
#ifdef INTENSIVE_ADV_ENABLE_P0
	APP_PRINT_INFO5("%s(): interval %dms, duration %dms, adjust_interval %dms, scan_window_per_interval %dms",__func__, interval, duration, adjust_interval, scan_window_per_interval);
	if(interval < 20 || scan_window_per_interval < 5)
	{
		return 1;
	}

	if(interval - adjust_interval < 10)
	{
		return 2;
	}

	float duration_temp = duration;
	float interval_temp = scan_window_per_interval;
	float temp = interval_temp/0.625;
	float temp_interval = 0;

	intensive_adv_scan_window = (uint16_t)temp;
	intensive_adv_scan_interval = intensive_adv_scan_window+2;
	temp_interval = (intensive_adv_scan_window + intensive_adv_scan_interval)*0.625;

	if(temp_interval > interval )
	{
		return 3;
	}

	intensive_adv_retrans_interval = 3;
	wait_to_send_adv_interval = interval - adjust_interval;

	intensive_adv_retrans_count = duration_temp/(interval)+5;

	intensive_adv_enable = true;

	return  0;
#else
	return 1;
#endif
}

void user_app_local_mesh_node_deinit(void)
{
	first_start_flag = 1;

	dev_info_show_flag = 0;
	attn_dur = 0;
    prov_manual = 0;

	if(discover_timer != NULL)
	{
		plt_timer_delete(discover_timer, 0);
		discover_timer = NULL;
	}

	if(prov_start_by_step_timer != NULL)
	{
		plt_timer_delete(prov_start_by_step_timer, 0);
		prov_start_by_step_timer = NULL;
	}
	gap_sched_deinit();
	mesh_node_deinit();
}

int32_t mesh_bta_cmd_process(uint16_t opcode, int argc, void **argv)
{
	int32_t ret = -1; //0 - success

	if( RTK_MESH_ENABLE_OP == opcode)
	{
		if(1== first_start_flag )
		{
			uint8_t *p = (uint8_t *)argv[0];
			return user_app_mesh_stack_enable(p);
		}
		return -1; //0 - success
	}

	APP_PRINT_INFO1("[rtkmesh][mesh] mesh_bta_cmd_process opcode=0x%04x",opcode);
	switch(opcode)
	{
		case RTK_MESH_INFO_LIST:
		{
			ret = user_app_list_mesh_info();
			break;
		}
		case RTK_MESH_IV_INIT_OP:
		{
			uint32_t iv_index = *(uint32_t*)argv[0];

			ret = user_app_mesh_iv_init(iv_index);
			break;
		}
		case RTK_MESH_SEQ_INIT_OP:
		{
			uint32_t seq_numer = *(uint32_t*)argv[0];

			ret = user_app_mesh_seq_init(seq_numer);
			break;
		}
		case RTK_MESH_IV_SEQ_GET_OP:
		{
			uint32_t *pseq_numer =(uint32_t*)argv[0];
			uint32_t *piv_index = (uint32_t*)argv[1];

			ret = user_app_mesh_iv_seq_get(pseq_numer, piv_index);
			break;
		}
		case RTK_MESH_NODE_NUM_GET_OP:
		{
			uint32_t *pnode_num =(uint32_t*)argv[0];

			ret = user_app_mesh_node_num_get(pnode_num);
			break;
		}
		case RTK_MESH_HB_MONITOR_SET_OP:
		{
			int32_t num = *(int32_t*)argv[0];
			int32_t timeout = *(int32_t*)argv[1];

			ret = user_app_set_hb_monitor(num, timeout);
			break;
		}
		case RTK_MESH_GET_LOCAL_ELEMENT_ADDR_OP:
		{
			uint8_t element_index =*(uint8_t*)argv[0];
			uint16_t *pelementAddr = (uint16_t*)argv[1];

			ret = user_app_mesh_get_local_elementAddr(element_index, pelementAddr);
			break;
		}
		case RTK_MESH_ADD_DEV_KEY_OP:
		{
			uint16_t addr = *(uint16_t*)argv[0];
			uint8_t element_num = *(uint8_t*)argv[1];
			uint8_t *pdevkey = (uint8_t*)argv[2];

			ret = user_app_add_devKey(addr, element_num, pdevkey);
			break;
		}
		case RTK_MESH_DEL_DEV_KEY_OP:
		{
			uint16_t addr = *(uint16_t*)argv[0];

			ret =user_app_delete_devKey(addr);
			break;
		}
		case RTK_MESH_SET_LOCAL_NET_KEY_OP:
		{
			int32_t netKeyIndex = *(int32_t*)argv[0];
			uint8_t *pNetkey = (uint8_t *)argv[1];
			bool bUpdate = *(bool *)argv[2];

			ret = user_app_mesh_set_netKey(netKeyIndex, pNetkey, bUpdate);
			break;
		}
		case RTK_MESH_SET_LOCAL_APP_KEY_OP:
		{
			user_cmd_parse_value_t *p = (user_cmd_parse_value_t *)argv[0];
			int32_t appKeyIndex = *(int32_t*)argv[0];
			int32_t netKeyIndex = *(int32_t*)argv[1];
			uint8_t* pAppkey = (uint8_t*)argv[2];
			bool bUpdate = *(bool* )argv[3];

			ret = user_app_mesh_set_appKey(appKeyIndex, netKeyIndex, pAppkey, bUpdate);
			break;
		}
		case RTK_MESH_BIND_LOCAL_APP_KEY_OP:
		{
			uint8_t element_index = *(uint8_t*)argv[0];
			uint32_t model_id = *(uint32_t*)argv[1];
			uint32_t appKeyIndex_g = *(uint32_t*)argv[2];
			uint8_t bindAllAppKeyFlag = *(uint8_t*)argv[3];

			ret = user_app_mesh_modelApp_bind(element_index, model_id, appKeyIndex_g, bindAllAppKeyFlag);
			break;
		}
		case RTK_MESH_GET_LOCAL_DEF_TTL_OP:
		{
			uint8_t *currentTTL = (uint8_t * )argv[0];
			ret = user_app_mesh_ttl_get(currentTTL );
			break;
		}
		case RTK_MESH_GET_BT_MAC_ADDR_OP:
		{
			uint8_t* btMac = (uint8_t *)argv[0];
			ret = user_app_mesh_getBtMac(btMac );
			break;
		}
		case RTK_MESH_SET_BLE_SCAN_PARAM_OP:
		{
			uint16_t mesh_ble_scan_interval = *(uint16_t*)argv[0];
			uint16_t mesh_ble_scan_window = *(uint16_t*)argv[1];

			ret = user_app_set_ble_scan_param(mesh_ble_scan_interval, mesh_ble_scan_window);
			break;
		}
		case RTK_MESH_STOP_SEND_OP:
		{
			uint32_t access_opcode = *(uint32_t* )argv[0];
			ret = user_app_stop_send_access_message(access_opcode );
			break;
		}
		case RTK_MESH_NET_INFO_ERASE_OP:
		{
			ret = user_app_local_mesh_node_reset();
			break;
		}
		case RTK_MESH_SET_MODEL_DATA_OP:
		{
			MESH_MODEL_DATA_T *p = (MESH_MODEL_DATA_T *)argv[0];
			ret = user_app_set_model_data(p );
			break;
		}
		case RTK_MESH_SEND_PKT_OP:
		{
			if(first_start_flag == 0)
			{
				int32_t dst = *(int32_t * )argv[0];
				int32_t dstAddrType = *(int32_t * )argv[1];
				int32_t src = *(int32_t *)argv[2];
				int32_t ttl = *(int32_t *)argv[3];
				int32_t netKeyIndex = *(int32_t *)argv[4];
				int32_t appKeyIndex = *(int32_t *)argv[5];
				uint8_t *data = (uint8_t *)argv[6];
				int32_t data_len = *(int32_t *)argv[7];
				ret = user_app_send_packet(dst, dstAddrType, src, ttl, netKeyIndex, appKeyIndex, data, data_len);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_APP_KEY_ADD_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t* )argv[0];
				uint16_t netKeyIndex_g = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];

				ret = user_app_cfg_app_key_add(dst, netKeyIndex_g, appKeyIndex_g);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_APP_BIND_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t element_index = *(uint8_t*)argv[1];
				uint32_t model_id = *(uint32_t*)argv[2];
				uint16_t appKeyIndex_g = *(uint16_t*)argv[3];

				ret = user_app_cfg_model_app_bind(dst, element_index, model_id, appKeyIndex_g);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_NET_TRANS_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t count = *(uint8_t*)argv[1];
				uint8_t steps = *(uint8_t*)argv[2];

				ret = user_app_cfg_model_net_trans_set(dst, count, steps);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_NET_TRANS_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];

				ret = user_app_cfg_model_net_trans_get(dst);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_BEACON_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];

				ret = user_app_cfg_model_beacon_get(dst);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_BEACON_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t state = *(uint8_t*)argv[1];

				ret = user_app_cfg_model_beacon_set(dst, state );
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;

		}
		case RTK_MESH_MSG_CFG_MODEL_COMPO_DATA_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t page = *(uint8_t*)argv[1];

				ret = user_app_cfg_model_compo_data_get(dst, page );
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;

		}
		case RTK_MESH_MSG_CFG_MODEL_TTL_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];

				ret = user_app_cfg_model_default_ttl_get(dst );
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;

		}
		case RTK_MESH_MSG_CFG_MODEL_TTL_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t ttl = *(uint8_t*)argv[1];

				ret = user_app_cfg_model_default_ttl_set(dst, ttl);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_PROXY_STAT_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];

				ret = user_app_cfg_model_proxy_stat_get(dst );
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;

		}
		case RTK_MESH_MSG_CFG_MODEL_PROXY_STAT_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t proxy_state = *(uint8_t*)argv[1];

				ret = user_app_cfg_model_proxy_stat_set(dst, proxy_state );
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;

		}
		case RTK_MESH_MSG_CFG_MODEL_RELAY_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];

				ret = user_app_cfg_model_relay_get(dst );
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;

		}
		case RTK_MESH_MSG_CFG_MODEL_RELAY_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t state = *(uint8_t*)argv[1];
				uint8_t count = *(uint8_t*)argv[2];
				uint8_t steps = *(uint8_t*)argv[3];

				ret = user_app_cfg_model_relay_set(dst, state, count, steps);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;

		}
		case RTK_MESH_MSG_CFG_MODEL_PUB_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t* )argv[0];
				uint16_t element_index = *(uint16_t* )argv[1];
				uint32_t model_id = *(uint32_t* )argv[2];

				ret = user_app_cfg_model_pub_get(dst,element_index, model_id);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_PUB_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t* )argv[0];
				uint16_t element_index = *(uint16_t* )argv[1];
				uint16_t publish_addr = *(uint16_t* )argv[2];
				pub_key_info_t pub_key_info = *(pub_key_info_t* )argv[3];
				uint8_t ttl = *(uint8_t* )argv[4];
				pub_period_t pub_period = *(pub_period_t* )argv[5];
				pub_retrans_info_t pub_retrans_info = *(pub_retrans_info_t* )argv[6];
				uint32_t model_id = *(uint32_t* )argv[7];

				ret = user_app_cfg_model_pub_set(dst, element_index, publish_addr, pub_key_info, ttl,
							pub_period, pub_retrans_info, model_id );
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_VIR_PUB_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t* )argv[0];
				uint16_t element_index = *(uint16_t* )argv[1];
				uint8_t* publish_addr = (uint8_t* )argv[2];
				pub_key_info_t pub_key_info = *(pub_key_info_t* )argv[3];
				uint8_t ttl = *(uint8_t* )argv[4];
				pub_period_t pub_period = *(pub_period_t* )argv[5];
				pub_retrans_info_t pub_retrans_info = *(pub_retrans_info_t* )argv[6];
				uint32_t model_id = *(uint32_t* )argv[7];

				ret = user_app_cfg_model_vir_pub_set(dst, element_index, publish_addr, pub_key_info, ttl,
							pub_period, pub_retrans_info, model_id );
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_SUB_ADD_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t element_index = *(uint8_t*)argv[1];
				uint32_t model_id = *(uint32_t*)argv[2];
				uint16_t group_addr = *(uint16_t*)argv[3];

				ret = user_app_cfg_model_sub_add(dst, element_index, model_id, group_addr);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_VIR_SUB_ADD_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t element_index = *(uint8_t*)argv[1];
				uint32_t model_id = *(uint32_t*)argv[2];
				uint8_t *group_addr = (uint8_t*)argv[3];

				ret = user_app_cfg_model_vir_sub_add(dst, element_index, model_id, group_addr);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_SUB_DELETE_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t element_index = *(uint8_t*)argv[1];
				uint32_t model_id = *(uint32_t*)argv[2];
				uint16_t group_addr = *(uint16_t*)argv[3];

				ret = user_app_cfg_model_sub_delete(dst, element_index, model_id, group_addr);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_VIR_SUB_DELETE_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t element_index = *(uint8_t*)argv[1];
				uint32_t model_id = *(uint32_t*)argv[2];
				uint8_t* group_addr = (uint8_t*)argv[3];

				ret = user_app_cfg_model_vir_sub_delete(dst, element_index, model_id, group_addr);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_SUB_OVERWRITE_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t element_index = *(uint8_t*)argv[1];
				uint32_t model_id = *(uint32_t*)argv[2];
				uint16_t sub_addr = *(uint16_t*)argv[3];

				ret = user_app_cfg_model_sub_overwrite(dst, element_index, sub_addr, model_id);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_VIR_SUB_OVERWRITE_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t element_index = *(uint8_t*)argv[1];
				uint32_t model_id = *(uint32_t*)argv[2];
				uint8_t *sub_addr = (uint8_t*)argv[3];

				ret = user_app_cfg_model_vir_sub_overwrite(dst, element_index, sub_addr, model_id);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_SUB_DELETE_ALL_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint8_t element_index = *(uint8_t*)argv[1];
				uint32_t model_id = *(uint32_t*)argv[2];

				ret = user_app_cfg_model_sub_delete_all(dst, element_index, model_id);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_HB_PUB_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];
				uint16_t dst_pub = *(uint16_t*)argv[1];
				uint8_t count_log = *(uint8_t*)argv[2];
				uint8_t period_log = *(uint8_t*)argv[3];
				uint8_t ttl = *(uint8_t*)argv[4];
				uint16_t features_temp = *(uint16_t*)argv[5];
				uint16_t net_key_index = *(uint16_t*)argv[6];
				hb_pub_features_t features;
				features.relay = (features_temp&0x01)?1:0;
				features.proxy = (features_temp&0x02)?1:0;
				features.frnd = (features_temp&0x04)?1:0;
				features.lpn = (features_temp&0x08)?1:0;

				ret = user_app_cfg_model_hb_pub_set(dst, dst_pub, count_log, period_log, ttl,
													features, net_key_index);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_CFG_MODEL_NODE_RESET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t dst = *(uint16_t*)argv[0];

				ret = user_app_cfg_model_node_reset(dst );
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_UNPROV_SCAN_OP:
		{
			if(first_start_flag == 0)
			{
				bool start = *(bool *)argv[0];
				int32_t duration = *(int32_t *)argv[1];
				ret = user_app_mesh_unprov_scan(start, duration);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_PROV_INVITE_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t* deviceUUID = (uint8_t*)argv[0];
				uint16_t assign_address = *(uint16_t*)argv[1];
				uint32_t attention_dur = *(uint32_t*)argv[2];
				bool enable = *(bool*)argv[3];

				ret = user_app_prov(deviceUUID, assign_address, attention_dur, enable);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_PROV_START_OP:
		{
			if(first_start_flag == 0)
			{
				prov_start_public_key_t public_key = *(prov_start_public_key_t*)argv[0];
				prov_auth_method_t auth_method = *(prov_auth_method_t*)argv[1];
				prov_auth_action_t auth_action = *(prov_auth_action_t*)argv[2];
				prov_auth_size_t auth_size = *(prov_auth_size_t*)argv[3];

				ret = user_app_prov_method_choose(public_key, auth_method, auth_action, auth_size);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_PROV_DEVICE_PUBLIC_KEY_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t* public_key = (prov_start_public_key_t*)argv[0];

				ret = user_app_prov_device_public_key_set(public_key);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_PROV_AUTH_VALUE_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t* pvalue = (uint8_t*)argv[0];
				uint8_t len = *(uint8_t*)argv[1];

				ret = user_app_prov_auth_value_set(pvalue, len);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_GOO_MODEL_ON_OFF_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t element_index = *(uint16_t *)argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];

				ret = user_app_generic_on_off_get(element_index, dst, appKeyIndex_g );
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_GOO_MODEL_ON_OFF_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint16_t element_index = *(uint16_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];
				generic_on_off_t on_off = *(generic_on_off_t* )argv[3];
				bool optional = *(bool* )argv[4];
				generic_transition_time_t trans_time = *(generic_transition_time_t* )argv[5];
				uint8_t delay =*(uint8_t* )argv[6];
				bool ack = *(bool* )argv[7];

				ret = user_app_generic_on_off_set(element_index, dst, appKeyIndex_g, on_off,
									optional, trans_time, delay, ack);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_LLC_MODEL_LIGHTNESS_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];

				ret = user_app_light_lightness_get(element_index, dst, appKeyIndex_g);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_LLC_MODEL_LIGHTNESS_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];
				uint16_t lightness = *(uint16_t* )argv[3];
				bool optional = *(bool* )argv[4];
				generic_transition_time_t trans_time = *(generic_transition_time_t* )argv[5];
				uint8_t delay = *(uint8_t* )argv[6];
				bool ack = *(bool* )argv[7];

				ret = user_app_light_lightness_set(element_index, dst, appKeyIndex_g, lightness,
								optional, trans_time, delay, ack);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_HSL_MODEL_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];

				ret = user_app_light_hsl_get(element_index, dst, appKeyIndex_g);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_HSL_MODEL_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index =  *(uint8_t*)argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];
				uint16_t lightness = *(uint16_t*)argv[3];
				uint16_t hue = *(uint16_t*)argv[4];
				uint16_t saturation = *(uint16_t*)argv[5];
				bool optional = *(bool*)argv[6];
				generic_transition_time_t trans_time = *(generic_transition_time_t*)argv[7];
				uint8_t delay = *(uint8_t*)argv[8];
				bool ack = *(bool* )argv[9];

				ret = user_app_light_hsl_set(element_index, dst, appKeyIndex_g, lightness, hue,
								saturation, optional, trans_time, delay, ack);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_HSL_MODEL_TARGET_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];

				ret = user_app_light_hsl_target_get(element_index, dst, appKeyIndex_g);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_HSL_MODEL_HUE_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];

				ret = user_app_light_hsl_hue_get(element_index, dst, appKeyIndex_g);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_HSL_MODEL_HUE_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index =  *(uint8_t*)argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];
				uint16_t hue = *(uint16_t*)argv[3];
				bool optional = *(bool*)argv[4];
				generic_transition_time_t trans_time = *(generic_transition_time_t*)argv[5];
				uint8_t delay = *(uint8_t*)argv[6];
				bool ack = *(bool* )argv[7];

				ret = user_app_light_hsl_hue_set(element_index, dst, appKeyIndex_g, hue,
								 optional, trans_time, delay, ack);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_HSL_MODEL_SATURATION_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];

				ret = user_app_light_hsl_saturation_get(element_index, dst, appKeyIndex_g);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_HSL_MODEL_SATURATION_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index =  *(uint8_t*)argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];
				uint16_t saturation = *(uint16_t*)argv[3];
				bool optional = *(bool*)argv[4];
				generic_transition_time_t trans_time = *(generic_transition_time_t*)argv[5];
				uint8_t delay = *(uint8_t*)argv[6];
				bool ack = *(bool* )argv[7];

				ret = user_app_light_hsl_saturation_set(element_index, dst, appKeyIndex_g, saturation,
								 optional, trans_time, delay, ack);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_HSL_MODEL_DEFAULT_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];

				ret = user_app_light_hsl_default_get(element_index, dst, appKeyIndex_g);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_HSL_MODEL_DEFAULT_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index =  *(uint8_t*)argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];
				uint16_t lightness = *(uint16_t*)argv[3];
				uint16_t hue = *(uint16_t*)argv[4];
				uint16_t saturation = *(uint16_t*)argv[5];
				bool ack = *(bool* )argv[6];

				ret = user_app_light_hsl_default_set(element_index, dst, appKeyIndex_g, lightness, hue, saturation, ack);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_HSL_MODEL_RANGE_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];

				ret = user_app_light_hsl_range_get(element_index, dst, appKeyIndex_g);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_HSL_MODEL_RANGE_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index =  *(uint8_t*)argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];
				uint16_t hue_range_min = *(uint16_t*)argv[3];
				uint16_t hue_range_max = *(uint16_t*)argv[4];
				uint16_t saturation_range_min = *(uint16_t*)argv[5];
				uint16_t saturation_range_max = *(uint16_t*)argv[6];
				bool ack = *(bool* )argv[7];

				ret = user_app_light_hsl_range_set(element_index, dst, appKeyIndex_g, hue_range_min, hue_range_max,
								saturation_range_min, saturation_range_max, ack);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_LIGHT_CTL_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];

				ret = user_app_light_get(element_index, dst, appKeyIndex_g);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_LIGHT_CTL_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];
				uint16_t lightness = *(uint16_t* )argv[3];
				uint16_t temperature = *(uint16_t* )argv[4];
				int16_t delta_uv = *(uint16_t* )argv[5];
				bool optional = *(bool* )argv[6];
				generic_transition_time_t trans_time = *(generic_transition_time_t* )argv[7];
				uint8_t delay = *(uint8_t* )argv[8];
				bool ack = *(bool* )argv[9];

				ret = user_app_light_ctl_set(element_index, dst, appKeyIndex_g, lightness, temperature,
								delta_uv, optional, trans_time, delay, ack);

			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_LIGHT_CTL_TEMPERATURE_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];

				ret = user_app_light_ctl_temperature_get(element_index, dst, appKeyIndex_g);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_LIGHT_CTL_TEMPERATURE_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];
				uint16_t temperature = *(uint16_t* )argv[3];
				int16_t delta_uv = *(uint16_t* )argv[4];
				bool optional = *(bool* )argv[5];
				generic_transition_time_t trans_time = *(generic_transition_time_t* )argv[6];
				uint8_t delay = *(uint8_t* )argv[7];
				bool ack = *(bool* )argv[8];

				ret = user_app_light_ctl_temperature_set(element_index, dst, appKeyIndex_g, temperature,
								delta_uv, optional, trans_time, delay, ack);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_LIGHT_CTL_TEMPERATURE_RANGE_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];

				ret = user_app_light_ctl_temperature_range_get(element_index, dst, appKeyIndex_g);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_LIGHT_CTL_TEMPERATURE_RANGE_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];
				uint16_t range_min = *(uint16_t* )argv[3];
				uint16_t range_max = *(uint16_t* )argv[4];
				bool ack = *(bool* )argv[5];

				ret = user_app_light_ctl_temperature_range_set(element_index, dst, appKeyIndex_g, range_min,
								range_max, ack);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_LIGHT_CTL_DEFAULT_GET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];

				ret = user_app_light_ctl_default_get(element_index, dst, appKeyIndex_g);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_MSG_LIGHT_CTL_DEFAULT_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint8_t element_index = *(uint8_t* )argv[0];
				uint16_t dst = *(uint16_t* )argv[1];
				uint16_t appKeyIndex_g = *(uint16_t* )argv[2];
				uint16_t lightness = *(uint16_t* )argv[3];
				uint16_t temperature = *(uint16_t* )argv[4];
				int16_t delta_uv = *(uint16_t* )argv[5];
				bool ack = *(bool* )argv[6];

				ret = user_app_light_ctl_default_set(element_index, dst, appKeyIndex_g, lightness, temperature,
								delta_uv, ack);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		case RTK_MESH_ENABLE_AGGRESSIVE_SET_OP:
		{
			bool enable = *(bool *)argv[0];
			ret = user_app_set_mesh_mode(enable);
			break;
		}
		case RTK_MESH_SET_SCAN_PARAM_WHEN_A2DP_ACTIVE_OP:
		{
			uint16_t scan_interval = *(uint16_t*)argv[0];
			uint16_t scan_windows = *(uint16_t*)argv[1];

			ret = user_app_set_scan_para_when_a2dp_active(scan_interval, scan_windows);
			break;
		}
		case RTK_MESH_SET_SCAN_HIGH_PRIOIRTY_OP:
		{
			uint8_t enable = *(uint8_t*)argv[0];
			uint8_t highTime = *(uint8_t*)argv[1];

			ret = user_app_set_scan_high_prioirty(enable, highTime);
			break;
		}
		case RTK_MESH_INTENSIVE_ADV_SET_OP:
		{
			if(first_start_flag == 0)
			{
				uint32_t interval = *(uint32_t*)argv[0];
				uint32_t duration = *(uint32_t*)argv[1];
				uint32_t adjust_interval = *(uint32_t*)argv[2];
				uint32_t scan_window_per_interval = *(uint32_t*)argv[3];

				ret = user_app_set_intensive_adv(interval, duration, adjust_interval, scan_window_per_interval);
			}
			else
			{
				APP_PRINT_ERROR0("gap is not ready");
				ret = -7;
			}
			break;
		}
		default:
		{
			ret = -1;
			break;
		}
	}
	return ret;

}




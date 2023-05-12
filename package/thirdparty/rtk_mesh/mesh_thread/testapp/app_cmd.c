
#include "app_cmd.h"



uint8_t defaultUUID[16] ={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x00};
char* defaultUUIDStr = "0x112233445566778899aabbccddeeff00";
//uint8_t uuid[16]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x00};

uint8_t defaultDevKey[16]={0xA8,0x01,0x51,0xCF,0x01,0x00,0x00,0x27,0x01,0xF5,0x07,0xDA,0x78,0x00,0x00,0x33};
char* defaultDevKeyStr = "0xA80151CF0100002701F507DA78000033";
uint8_t defaultNetKey[16]={0xA8,0x01,0x51,0xCF,0x01,0x00,0x00,0x27,0x01,0xF5,0x07,0xDA,0x78,0x00,0x00,0x03};
char* defaultNetKeyStr = "0xA80151CF0100002701F507DA78000003";
uint8_t defaultAppKey[16]={0xA8,0x01,0x51,0xCF,0x01,0x00,0x00,0x27,0x01,0xF5,0x07,0xDA,0x78,0x00,0x00,0x30};
char* defaultAppKeyStr = "0xA80151CF0100002701F507DA78000030";
uint8_t zeroArray[16]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

uint8_t confirmationkey[16];
uint8_t confirmation[16];
uint8_t Dev_confirmation[16];
uint8_t Cal_confirmation[16];
uint8_t Pro_rand[16];
uint8_t Dev_rand[16];
uint8_t authvalue[16];
uint8_t authret = 0;

uint8_t Dconfiromkey[16] ={0xB0,0xE2,0xD5,0x22,0xC2,0x0B,0x1D,0x5C,0xC5,0x6A,0x29,0x8C,0xBF,0x15,0xB6,0x02};
uint8_t Dconfirom[16] =   {0x6C,0x17,0x99,0x3C,0x79,0x3C,0x24,0x84,0xA3,0x90,0x6B,0x66,0x41,0x10,0x6C,0x43};
uint8_t Drandom[16] =     {0x0B,0x54,0x6E,0x37,0x5E,0x29,0x66,0x27,0xFD,0x4D,0x87,0xBC,0xAD,0x54,0x65,0x90};
uint8_t Dauthvalue[16] =  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

int waitForStateTimer_fd;
int OnOffTestTimer_fd;
uint16_t element_index_record = 0;
uint16_t dst_record = 0;
uint16_t appkeyindex_g_record = 0;
int inquiry_retry_count = 0;
uint32_t timer_interval = 200000000;
uint32_t goo_send_count = 0;
uint32_t goo_recv_count = 0;
uint32_t need_one_response = 0;

int status_get_retry = 3;

prov_auth_method_t auth_method_record;
prov_auth_action_t auth_action_record;
prov_auth_size_t   auth_size_record;
prov_auth_value_type_t auth_value_type_record;

static uint16_t local_unicast_addr = 0x0056;


extern int __main_terminated;

static void cmd_quit(user_cmd_parse_value_t *p)
{
	printf("!! Quit\n");
	__main_terminated = 1;
}

static void cmd_mesh_thread_launch(user_cmd_parse_value_t *p)
{
	int32_t ret;
	mesh_model_id_t model_id;

	model_id.company_id = 0xffff;
	model_id.model_id = 0x1001;

	if(p->para_count >= 1)
	{
		local_unicast_addr = (uint16_t )p->dw_parameter[0];
	}
	rtkmesh_element_init(2);
	vendor_model_queue_init();
	ret = vendor_model_add(1, model_id, app_generic_on_off_client_receive);
	if(ret != 0)
	{
		printf("!!! add model_id 0x%04x (company id 0x%04x) fail\n",model_id.model_id, model_id.company_id);
	}
	mesh_cfg_client_recv_cb_register(config_client_data_recv);
	mesh_goo_recv_cb_register(generic_on_off_client_data_recv);
	mesh_llc_recv_cb_register(app_light_lightness_client_data_recv);
	mesh_light_ctl_recv_cb_register(light_ctl_client_data_recv);
	mesh_light_hsl_recv_cb_register(light_hsl_client_data_recv);

	//ret = mesh_init_ali(app_mesh_event_cb,0xFFFFFFFF,0xFFFFFFFF,4,200,local_unicast_addr);
	ret = mesh_init_ali(app_mesh_event_cb,0x00010000,0x00004620,2,200,local_unicast_addr);
	//ret = mesh_init_ali(NULL,0xFFFFFFFF,0xFFFFFFFF,4,10);
	if(ret == 0)
	{
		printf(">>>>mesh thread lanuch success\n");
	}
	else
	{
		printf(">>>>mesh thread lanuch fail, err code %d\n", ret);
	}
}

static void cmd_mesh_iv_init(user_cmd_parse_value_t *p)
{
	int32_t ret;
    	uint32_t iv_index;

	if(p->para_count < 1)
	{
		printf(">>>>set iv_index fail, too few parameters\n");
	}

	iv_index = p->dw_parameter[0];

	ret = rtkmesh_iv_init(iv_index);

	if(ret == 0)
	{
		printf(">>>>set iv_index success\n");
	}
	else
	{
		printf(">>>>set iv_index fail, err code %d\n", ret);
	}
}

static void cmd_mesh_seq_init(user_cmd_parse_value_t *p)
{
	int32_t ret;
    	uint32_t seq;

	if(p->para_count < 1)
	{
		printf(">>>>set seq number fail, too few parameters\n");
	}
	seq = p->dw_parameter[0]&0x00ffffff;

	ret = rtkmesh_seq_init(seq );

	if(ret == 0)
	{
		printf(">>>>set seq number success\n");
	}
	else
	{
		printf(">>>>set seq number fail, err code %d\n", ret);
	}
}

static void cmd_mesh_iv_seq_get(user_cmd_parse_value_t *p)
{
	int32_t ret;
    	uint32_t seq; //!< 3 bytes used, unique per node
    	uint32_t iv_index;

	ret = rtkmesh_iv_seq_get(&seq, &iv_index);

	if(ret == 0)
	{
		printf(">>>>get iv_index and seq number success, seq 0x%08x, iv index 0x%08x\n",seq,iv_index);
	}
	else
	{
		printf(">>>>get iv_index and seq number fail, err code %d\n", ret);
	}
}

static void cmd_mesh_node_num_get(user_cmd_parse_value_t *p)
{
	int32_t ret;
    	uint32_t node_num;

	ret = rtkmesh_node_num_get(&node_num);

	if(ret == 0)
	{
		printf(">>>>get node number success, node number 0x%08x \n",node_num);
	}
	else
	{
		printf(">>>>get node number fail, err code %d\n", ret);
	}
}

static void cmd_mesh_hb_monitor_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
	int32_t count = 0;
	int32_t timeout = 0;
	if(p->para_count < 2)
	{
		printf(">>>>set heartbeat monitor fail, too few parameters\n");
		return;
	}
	count = (int32_t)p->dw_parameter[0];
	timeout = (int32_t)p->dw_parameter[1];

	ret = rtkmesh_hb_monitor_set(count, timeout);
	if(ret == 0)
	{
		printf(">>>>set heartbeat monitor success\n");
	}
	else
	{
		printf(">>>>set heartbeat monitor fail, err code %d\n", ret);
	}
	return;

}

static void cmd_device_scan(user_cmd_parse_value_t *p)
{
	int32_t ret;
	bool start = 0;
	int32_t duration = 0;
	if(p->para_count < 2)
	{
		printf(">>>>mesh udb scan start fail, too few parameters\n");
		return;
	}
	start = (bool)p->dw_parameter[0];
	duration = (int32_t)p->dw_parameter[1];

	ret = rtkmesh_unprov_scan(start, duration);
	if(ret == 0)
	{
		printf(">>>>mesh udb scan start ...\n");
	}
	else
	{
		printf(">>>>mesh udb scan start fail, err code %d\n", ret);
	}
	return;
}

static void cmd_mesh_stack_enable(user_cmd_parse_value_t *p)
{
	int32_t ret;
	char* strUUID = NULL;
	uint8_t uuid[16];
	if(p->para_count < 1)
	{
		strUUID = defaultUUIDStr;
	}
	else
	{
		strUUID = p->pparameter[0];
	}

	plt_hex_to_bin(uuid, (uint8_t*)strUUID, 16);
	ret = rtkmesh_stack_enable(uuid);

	if(ret == 0)
	{
		printf(">>>>mesh enable success\n");
	}
	else
	{
		printf(">>>>mesh enable fail, err code %d\n", ret);
	}

}

static void cmd_get_local_element_addr(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint8_t element_index;
	uint16_t element_addr;

	if(p->para_count < 1)
	{
		printf(">>>>get  element addr fail, too few parameters\n");
		return;
	}
	element_index = p->dw_parameter[0];

	ret = rtkmesh_local_element_addr_get(element_index, &element_addr);

	if(ret == 0)
	{
		printf(">>>>get  element addr success, addr 0x%x\n",element_addr);
	}
	else
	{
		printf(">>>>get  element addr fail, err code %d\n", ret);
	}
	return;
}

static void cmd_set_local_dev_key(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t addr = 0;
	uint8_t element_num = 0;
	char* strDevkey = NULL;
	uint8_t devkey[16];

	if(p->para_count < 2)
	{
		printf(">>>>mesh set dev key fail, parameters are too few");
		return;
	}
	if(p->dw_parameter[1] == 0)
	{
		printf(">>>>mesh set dev key fail, element num is 0");
		return;
	}
	addr = (uint16_t )p->dw_parameter[0];
	element_num = (uint8_t)p->dw_parameter[1];

	if(p->para_count < 3)
	{
		strDevkey = defaultDevKeyStr;
	}
	else
	{
		strDevkey = p->pparameter[2];
	}
	plt_hex_to_bin(devkey, (uint8_t*)strDevkey, 16);

	ret = rtkmesh_devkey_add(addr, element_num, devkey);
	if(ret == 0)
	{
		printf(">>>>mesh set dev key success\n");
	}
	else
	{
		printf(">>>>mesh set dev key fail, err code %d\n", ret);
	}
}

static void cmd_del_local_dev_key(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t addr;

	if(p->para_count < 1)
	{
		printf(">>>>mesh delete dev key fail, too few parameters\n");
		return;
	}
	addr = (uint16_t )p->dw_parameter[0];
	ret = rtkmesh_devkey_delete(addr );
	if(ret == 0)
	{
		printf(">>>>mesh delete dev key success\n");
	}
	else
	{
		printf(">>>>mesh delete dev key fail, err code %d\n", ret);
	}
}

static void cmd_reg_model(user_cmd_parse_value_t *p)
{
    int32_t ret = 0;

	MESH_MODEL_DATA_T lightness_client;
	lightness_client.opcode = ADD_LIGHTNESS_CLIENT_MODEL_OP;
	lightness_client.element_index = 1;
	lightness_client.callback = app_light_lightness_client_data_recv;

    ret = rtkmesh_model_data_set(&lightness_client );

	if(ret == 0)
	{
		printf(">>>>register lightness client model success, element index %d, model id 0x1302ffff\n",lightness_client.element_index);
	}
	else
	{
		printf(">>>>register lightness client model fail, err code %d\n", ret);
	}
}

static void  cmd_cfg_appkey_add(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst = 0;
	uint16_t netKeyIndex_g = 0;
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 3)
	{
		printf(">>>>mesh cfg net key fail, too few parameters\n");
		return;
	}

	dst = (uint16_t)p->dw_parameter[0];
	netKeyIndex_g = (uint16_t)p->dw_parameter[1];
	appKeyIndex_g = (uint16_t)p->dw_parameter[2];

	ret = rtkmesh_cfg_appkey_add(dst, netKeyIndex_g, appKeyIndex_g);
	if(ret == 0)
	{
		printf(">>>>mesh cfg net key success\n");
	}
	else
	{
		printf(">>>>mesh cfg net key fail, err code %d\n", ret);
	}
}

static void  cmd_cfg_model_appkey_bind(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst = 0;
	uint8_t element_index = 0;
	uint32_t model_id = 0;
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 4)
	{
		printf(">>>>mesh cfg model bind fail, too few parameters\n");
		return;
	}
	dst = p->dw_parameter[0];
	element_index = p->dw_parameter[1];
	model_id = p->dw_parameter[2];
	appKeyIndex_g = p->dw_parameter[3];

	ret = rtkmesh_cfg_model_appkey_bind(dst, element_index, model_id, appKeyIndex_g);
	if(ret == 0)
	{
		printf(">>>>mesh cfg model bind success\n");
	}
	else
	{
		printf(">>>>mesh cfg model bind fail, err code %d\n", ret);
	}
}
static void cmd_goo_set(user_cmd_parse_value_t *p)
{
	int32_t ret;

    bool optional = FALSE;
	uint16_t element_index = 0;
	uint16_t dst = p->dw_parameter[0];
	generic_on_off_t on_off = (generic_on_off_t)p->dw_parameter[1];
	bool ack = p->dw_parameter[2];
	uint16_t appKeyIndex_g = 0;

    generic_transition_time_t trans_time = {p->dw_parameter[4],
                                            p->dw_parameter[5]
                                           };
	uint8_t delay = p->dw_parameter[6];

	if(p->para_count < 3)
	{
		printf(">>>>mesh goo set fail, too few parameters\n");
		return;
	}

	if(p->para_count > 3)
	{
		appKeyIndex_g = (uint16_t)p->dw_parameter[3];
	}

    if (p->para_count == 7 )
    {
        optional = TRUE;
    }

	ret = rtkmesh_generic_on_off_set(element_index, dst, appKeyIndex_g, on_off,
									optional, trans_time, delay, ack);
	if(ret == 0)
	{
		printf(">>>>mesh goo set success\n");
	}
	else
	{
		printf(">>>>mesh goo set fail, err code %d\n", ret);
	}
}

static void cmd_goo_set_by_element1(user_cmd_parse_value_t *p)
{
	int32_t ret;

    bool optional = FALSE;
	uint16_t element_index = 1;
	uint16_t dst = p->dw_parameter[0];
	generic_on_off_t on_off = (generic_on_off_t)p->dw_parameter[1];
	bool ack = p->dw_parameter[2];
	uint16_t appKeyIndex_g = 0;

    generic_transition_time_t trans_time = {p->dw_parameter[4],
                                            p->dw_parameter[5]
                                           };
	uint8_t delay = p->dw_parameter[6];

	if(p->para_count < 3)
	{
		printf(">>>>mesh goo set fail, too few parameters\n");
		return;
	}

	if(p->para_count > 3)
	{
		appKeyIndex_g = (uint16_t)p->dw_parameter[3];
	}

    if (p->para_count == 7 )
    {
        optional = TRUE;
    }

	ret = rtkmesh_generic_on_off_set(element_index, dst, appKeyIndex_g, on_off,
									optional, trans_time, delay, ack);
	if(ret == 0)
	{
		printf(">>>>mesh goo set success\n");
	}
	else
	{
		printf(">>>>mesh goo set fail, err code %d\n", ret);
	}
}

static void cmd_goo_get(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t element_index = 0;
	uint16_t dst = 0;
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 1)
	{
		printf(">>>>mesh goo get fail, too few parameters\n");
		return;
	}

	dst = (uint16_t)p->dw_parameter[0];
	if(p->para_count == 2 )
	{
		appKeyIndex_g = (uint16_t )p->dw_parameter[1];
	}

	ret = rtkmesh_generic_on_off_get(element_index, dst, appKeyIndex_g );
	if(ret == 0)
	{
		printf(">>>>mesh goo get success\n");
	}
	else
	{
		printf(">>>>mesh goo get fail, err code %d\n", ret);
	}
}

static void cmd_llc_get(user_cmd_parse_value_t *p)
{
	int32_t ret;

    uint8_t element_index = (uint8_t)p->dw_parameter[0];
    uint16_t dst = (uint16_t )p->dw_parameter[1];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 2)
	{
		printf(">>>>mesh llc get fail, too few parameters\n");
		return;
	}
	if(p->para_count == 3 )
	{
		appKeyIndex_g = (uint16_t )p->dw_parameter[2];
	}

	ret = rtkmesh_light_lightness_get(element_index, dst, appKeyIndex_g);
	if(ret == 0)
	{
		printf(">>>>mesh llc get success\n");
	}
	else
	{
		printf(">>>>mesh llc get fail, err code %d\n", ret);
	}
}

static void cmd_llc_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
    bool optional = FALSE;
	uint8_t element_index = p->dw_parameter[0];
	uint16_t dst = p->dw_parameter[1];
	uint16_t lightness = p->dw_parameter[2];
	bool ack = p->dw_parameter[3];
	uint16_t appKeyIndex_g = 0;
    generic_transition_time_t trans_time = {p->dw_parameter[5],
                                        p->dw_parameter[6]
                                       };
	uint8_t delay = p->dw_parameter[7];

	if(p->para_count < 4)
	{
		printf(">>>>mesh llc set fail, too few parameters\n");
		return;
	}
	if(p->para_count > 4)
	{
		appKeyIndex_g = p->dw_parameter[4];
	}
	if (p->para_count == 8)
    {
        optional = TRUE;
    }

	ret = rtkmesh_light_lightness_set(element_index, dst, appKeyIndex_g, lightness,
								optional, trans_time, delay, ack);
	if(ret == 0)
	{
		printf(">>>>mesh llc set success\n");
	}
	else
	{
		printf(">>>>mesh llc set fail, err code %d\n", ret);
	}
}

static void cmd_light_hsl_get(user_cmd_parse_value_t *p)
{
	int32_t ret;

  uint8_t element_index = (uint8_t)p->dw_parameter[0];
  uint16_t dst = (uint16_t )p->dw_parameter[1];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 2)
	{
		printf(">>>>mesh hsl get fail, too few parameters\n");
		return;
	}
	if(p->para_count == 3 )
	{
		appKeyIndex_g = (uint16_t )p->dw_parameter[2];
	}

	ret = rtkmesh_light_hsl_get(element_index, dst, appKeyIndex_g);
	if(ret == 0)
	{
		printf(">>>>mesh hsl get success\n");
	}
	else
	{
		printf(">>>>mesh hsl get fail, err code %d\n", ret);
	}
}

static void cmd_light_hsl_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
  bool optional = FALSE;
	uint8_t element_index = p->dw_parameter[0];
	uint16_t dst = p->dw_parameter[1];
	uint16_t lightness = p->dw_parameter[2];
	uint16_t hue = p->dw_parameter[3];
	int16_t saturation = (int16_t)p->dw_parameter[4];
	bool ack = p->dw_parameter[5];
	uint16_t appKeyIndex_g = 0;
  generic_transition_time_t trans_time = {p->dw_parameter[7],
                                        p->dw_parameter[8]
                                       };
	uint8_t delay = p->dw_parameter[9];

	if(p->para_count < 6)
	{
		printf(">>>>mesh light hsl set fail, too few parameters\n");
		return;
	}
	if(p->para_count > 6)
	{
		appKeyIndex_g = p->dw_parameter[6];
	}
	if (p->para_count == 10)
    {
        optional = TRUE;
    }

	ret = rtkmesh_light_hsl_set(element_index, dst, appKeyIndex_g, lightness, hue, saturation,
                                    optional, trans_time, delay, ack);
	if(ret == 0)
	{
		printf(">>>>mesh light hsl set success\n");
	}
	else
	{
		printf(">>>>mesh light hsl set fail, err code %d\n", ret);
	}
}

static void cmd_light_hsl_target_get(user_cmd_parse_value_t *p)
{
	int32_t ret;

  uint8_t element_index = (uint8_t)p->dw_parameter[0];
  uint16_t dst = (uint16_t )p->dw_parameter[1];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 2)
	{
		printf(">>>>mesh hsl target get fail, too few parameters\n");
		return;
	}
	if(p->para_count == 3 )
	{
		appKeyIndex_g = (uint16_t )p->dw_parameter[2];
	}

	ret = rtkmesh_light_hsl_target_get(element_index, dst, appKeyIndex_g);
	if(ret == 0)
	{
		printf(">>>>mesh hsl target get success\n");
	}
	else
	{
		printf(">>>>mesh hsl target get fail, err code %d\n", ret);
	}
}

static void cmd_light_hsl_hue_get(user_cmd_parse_value_t *p)
{
	int32_t ret;

  uint8_t element_index = (uint8_t)p->dw_parameter[0];
  uint16_t dst = (uint16_t )p->dw_parameter[1];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 2)
	{
		printf(">>>>mesh hsl hue get fail, too few parameters\n");
		return;
	}
	if(p->para_count == 3 )
	{
		appKeyIndex_g = (uint16_t )p->dw_parameter[2];
	}

	ret = rtkmesh_light_hsl_hue_get(element_index, dst, appKeyIndex_g);
	if(ret == 0)
	{
		printf(">>>>mesh hsl hue get success\n");
	}
	else
	{
		printf(">>>>mesh hsl hue get fail, err code %d\n", ret);
	}
}

static void cmd_light_hsl_hue_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
  bool optional = FALSE;
	uint8_t element_index = p->dw_parameter[0];
	uint16_t dst = p->dw_parameter[1];
	uint16_t hue = p->dw_parameter[2];
	bool ack = p->dw_parameter[3];
	uint16_t appKeyIndex_g = 0;
  generic_transition_time_t trans_time = {p->dw_parameter[5],
                                        p->dw_parameter[6]
                                       };
	uint8_t delay = p->dw_parameter[7];

	if(p->para_count < 4)
	{
		printf(">>>>mesh light hsl hue set fail, too few parameters\n");
		return;
	}
	if(p->para_count > 4)
	{
		appKeyIndex_g = p->dw_parameter[4];
	}
	if (p->para_count == 8)
    {
        optional = TRUE;
    }

	ret = rtkmesh_light_hsl_hue_set(element_index, dst, appKeyIndex_g, hue,
                                    optional, trans_time, delay, ack);
	if(ret == 0)
	{
		printf(">>>>mesh light hsl hue set success\n");
	}
	else
	{
		printf(">>>>mesh light hsl hue set fail, err code %d\n", ret);
	}
}


static void cmd_light_hsl_saturation_get(user_cmd_parse_value_t *p)
{
	int32_t ret;

  uint8_t element_index = (uint8_t)p->dw_parameter[0];
  uint16_t dst = (uint16_t )p->dw_parameter[1];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 2)
	{
		printf(">>>>mesh hsl saturation get fail, too few parameters\n");
		return;
	}
	if(p->para_count == 3 )
	{
		appKeyIndex_g = (uint16_t )p->dw_parameter[2];
	}

	ret = rtkmesh_light_hsl_saturation_get(element_index, dst, appKeyIndex_g);
	if(ret == 0)
	{
		printf(">>>>mesh hsl saturation get success\n");
	}
	else
	{
		printf(">>>>mesh hsl saturation get fail, err code %d\n", ret);
	}
}

static void cmd_light_hsl_saturation_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
  bool optional = FALSE;
	uint8_t element_index = p->dw_parameter[0];
	uint16_t dst = p->dw_parameter[1];
	uint16_t saturation = p->dw_parameter[2];
	bool ack = p->dw_parameter[3];
	uint16_t appKeyIndex_g = 0;
  generic_transition_time_t trans_time = {p->dw_parameter[5],
                                        p->dw_parameter[6]
                                       };
	uint8_t delay = p->dw_parameter[7];

	if(p->para_count < 4)
	{
		printf(">>>>mesh light hsl hue saturation fail, too few parameters\n");
		return;
	}
	if(p->para_count > 4)
	{
		appKeyIndex_g = p->dw_parameter[4];
	}
	if (p->para_count == 8)
    {
        optional = TRUE;
    }

	ret = rtkmesh_light_hsl_saturation_set(element_index, dst, appKeyIndex_g, saturation,
                                    optional, trans_time, delay, ack);
	if(ret == 0)
	{
		printf(">>>>mesh light hsl saturation set success\n");
	}
	else
	{
		printf(">>>>mesh light hsl saturation set fail, err code %d\n", ret);
	}
}

static void cmd_light_hsl_default_get(user_cmd_parse_value_t *p)
{
	int32_t ret;

  uint8_t element_index = (uint8_t)p->dw_parameter[0];
  uint16_t dst = (uint16_t )p->dw_parameter[1];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 2)
	{
		printf(">>>>mesh hsl saturation get fail, too few parameters\n");
		return;
	}
	if(p->para_count == 3 )
	{
		appKeyIndex_g = (uint16_t )p->dw_parameter[2];
	}

	ret = rtkmesh_light_hsl_default_get(element_index, dst, appKeyIndex_g);
	if(ret == 0)
	{
		printf(">>>>mesh hsl default get success\n");
	}
	else
	{
		printf(">>>>mesh hsl default get fail, err code %d\n", ret);
	}
}

static void cmd_light_hsl_default_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint8_t element_index = p->dw_parameter[0];
	uint16_t dst = p->dw_parameter[1];
	uint16_t lightness = p->dw_parameter[2];
	uint16_t hue = p->dw_parameter[3];
	uint16_t saturation = p->dw_parameter[4];
	bool ack = p->dw_parameter[5];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 5)
	{
		printf(">>>>mesh light hsl default set fail, too few parameters\n");
		return;
	}
	if(p->para_count > 5)
	{
		appKeyIndex_g = p->dw_parameter[6];
	}

	ret = rtkmesh_light_hsl_default_set(element_index, dst, appKeyIndex_g, lightness,
                                    hue, saturation, ack);
	if(ret == 0)
	{
		printf(">>>>mesh light hsl default set success\n");
	}
	else
	{
		printf(">>>>mesh light hsl default set fail, err code %d\n", ret);
	}
}

static void cmd_light_hsl_range_get(user_cmd_parse_value_t *p)
{
	int32_t ret;

  uint8_t element_index = (uint8_t)p->dw_parameter[0];
  uint16_t dst = (uint16_t )p->dw_parameter[1];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 2)
	{
		printf(">>>>mesh hsl range get fail, too few parameters\n");
		return;
	}
	if(p->para_count == 3 )
	{
		appKeyIndex_g = (uint16_t )p->dw_parameter[2];
	}

	ret = rtkmesh_light_hsl_range_get(element_index, dst, appKeyIndex_g);
	if(ret == 0)
	{
		printf(">>>>mesh hsl range get success\n");
	}
	else
	{
		printf(">>>>mesh hsl range get fail, err code %d\n", ret);
	}
}

static void cmd_light_hsl_range_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint8_t element_index = p->dw_parameter[0];
	uint16_t dst = p->dw_parameter[1];
	uint16_t hue_range_min = p->dw_parameter[2];
	uint16_t hue_range_max = p->dw_parameter[3];
	uint16_t saturation_range_min = p->dw_parameter[4];
	uint16_t saturation_range_max = p->dw_parameter[5];
	bool ack = p->dw_parameter[6];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 7)
	{
		printf(">>>>mesh light hsl range set fail, too few parameters\n");
		return;
	}
	if(p->para_count > 7)
	{
		appKeyIndex_g = p->dw_parameter[7];
	}

	ret = rtkmesh_light_hsl_range_set(element_index, dst, appKeyIndex_g, hue_range_min,
                                    hue_range_max, saturation_range_min, saturation_range_max, ack);
	if(ret == 0)
	{
		printf(">>>>mesh light hsl range set success\n");
	}
	else
	{
		printf(">>>>mesh light hsl range set fail, err code %d\n", ret);
	}
}

static void cmd_light_ctl_get(user_cmd_parse_value_t *p)
{
	int32_t ret;

	uint8_t element_index = (uint8_t)p->dw_parameter[0];
	uint16_t dst = (uint16_t )p->dw_parameter[1];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 2)
	{
		printf(">>>>mesh light ctl get fail, too few parameters\n");
		return;
	}
	if(p->para_count == 3 )
	{
		appKeyIndex_g = (uint16_t )p->dw_parameter[2];
	}

	ret = rtkmesh_light_ctl_get(element_index, dst, appKeyIndex_g);
	if(ret == 0)
	{
		printf(">>>>mesh light ctl get success\n");
	}
	else
	{
		printf(">>>>mesh light ctl get fail, err code %d\n", ret);
	}
}

static void cmd_light_ctl_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
    bool optional = FALSE;
	uint8_t element_index = p->dw_parameter[0];
	uint16_t dst = p->dw_parameter[1];
	uint16_t lightness = p->dw_parameter[2];
	uint16_t temperature = p->dw_parameter[3];
	int16_t delta_uv = (int16_t)p->dw_parameter[4];
	bool ack = p->dw_parameter[5];
	uint16_t appKeyIndex_g = 0;
    generic_transition_time_t trans_time = {p->dw_parameter[7],
                                        p->dw_parameter[8]
                                       };
	uint8_t delay = p->dw_parameter[9];

	if(p->para_count < 6)
	{
		printf(">>>>mesh light ctl set fail, too few parameters\n");
		return;
	}
	if(p->para_count > 6)
	{
		appKeyIndex_g = p->dw_parameter[6];
	}
	if (p->para_count == 10)
    {
        optional = TRUE;
    }

	ret = rtkmesh_light_ctl_set(element_index, dst, appKeyIndex_g, lightness, temperature, delta_uv,
								optional, trans_time, delay, ack);
	if(ret == 0)
	{
		printf(">>>>mesh light ctl set success\n");
	}
	else
	{
		printf(">>>>mesh light ctl set fail, err code %d\n", ret);
	}
}

static void cmd_light_ctl_temperature_get(user_cmd_parse_value_t *p)
{
	int32_t ret;

    uint8_t element_index = (uint8_t)p->dw_parameter[0];
    uint16_t dst = (uint16_t )p->dw_parameter[1];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 2)
	{
		printf(">>>>mesh light temperature get fail, too few parameters\n");
		return;
	}
	if(p->para_count == 3 )
	{
		appKeyIndex_g = (uint16_t )p->dw_parameter[2];
	}

	ret = rtkmesh_light_ctl_temperature_get(element_index, dst, appKeyIndex_g);
	if(ret == 0)
	{
		printf(">>>>mesh light temperature get success\n");
	}
	else
	{
		printf(">>>>mesh light temperature fail, err code %d\n", ret);
	}
}

static void cmd_light_ctl_temperature_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
    bool optional = FALSE;
	uint8_t element_index = p->dw_parameter[0];
	uint16_t dst = p->dw_parameter[1];
	uint16_t temperature = p->dw_parameter[2];
	int16_t delta_uv = (int16_t)p->dw_parameter[3];
	bool ack = p->dw_parameter[4];
	uint16_t appKeyIndex_g = 0;
    generic_transition_time_t trans_time = {p->dw_parameter[6],
                                        p->dw_parameter[7]
                                       };
	uint8_t delay = p->dw_parameter[8];

	if(p->para_count < 5)
	{
		printf(">>>>mesh light temperature set fail, too few parameters\n");
		return;
	}
	if(p->para_count > 5)
	{
		appKeyIndex_g = p->dw_parameter[5];
	}
	if (p->para_count == 9)
    {
        optional = TRUE;
    }

	ret = rtkmesh_light_ctl_temperature_set(element_index, dst, appKeyIndex_g, temperature, delta_uv,
								optional, trans_time, delay, ack);
	if(ret == 0)
	{
		printf(">>>>mesh light temperature set success\n");
	}
	else
	{
		printf(">>>>mesh light temperature set fail, err code %d\n", ret);
	}
}

static void cmd_light_ctl_temperature_range_get(user_cmd_parse_value_t *p)
{
	int32_t ret;

    uint8_t element_index = (uint8_t)p->dw_parameter[0];
    uint16_t dst = (uint16_t )p->dw_parameter[1];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 2)
	{
		printf(">>>>mesh light temperature range get fail, too few parameters\n");
		return;
	}
	if(p->para_count == 3 )
	{
		appKeyIndex_g = (uint16_t )p->dw_parameter[2];
	}

	ret = rtkmesh_light_ctl_temperature_range_get(element_index, dst, appKeyIndex_g);
	if(ret == 0)
	{
		printf(">>>>mesh light temperature range get success\n");
	}
	else
	{
		printf(">>>>mmesh light temperature range get fail, err code %d\n", ret);
	}
}

static void cmd_light_ctl_temperature_range_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint8_t element_index = p->dw_parameter[0];
	uint16_t dst = p->dw_parameter[1];
	uint16_t range_min = p->dw_parameter[2];
	uint16_t range_max = p->dw_parameter[3];
	bool ack = p->dw_parameter[4];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 5)
	{
		printf(">>>>mesh light temperature range set fail, too few parameters\n");
		return;
	}
	if(p->para_count > 5)
	{
		appKeyIndex_g = p->dw_parameter[5];
	}

	ret = rtkmesh_light_ctl_temperature_range_set(element_index, dst, appKeyIndex_g, range_min, range_max, ack);

	if(ret == 0)
	{
		printf(">>>>mesh light temperature range set success\n");
	}
	else
	{
		printf(">>>>mesh light temperature range set fail, err code %d\n", ret);
	}
}

static void cmd_light_ctl_default_get(user_cmd_parse_value_t *p)
{
	int32_t ret;

    uint8_t element_index = (uint8_t)p->dw_parameter[0];
    uint16_t dst = (uint16_t )p->dw_parameter[1];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 2)
	{
		printf(">>>>mesh light ctl default get fail, too few parameters\n");
		return;
	}
	if(p->para_count == 3 )
	{
		appKeyIndex_g = (uint16_t )p->dw_parameter[2];
	}

	ret = rtkmesh_light_ctl_default_get(element_index, dst, appKeyIndex_g);
	if(ret == 0)
	{
		printf(">>>>mesh light ctl default get success\n");
	}
	else
	{
		printf(">>>>mesh light ctl default get fail, err code %d\n", ret);
	}
}

static void cmd_light_ctl_default_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint8_t element_index = p->dw_parameter[0];
	uint16_t dst = p->dw_parameter[1];
	uint16_t lightness = p->dw_parameter[2];
	uint16_t temperature = p->dw_parameter[3];
	int16_t delta = p->dw_parameter[4];
	bool ack = p->dw_parameter[5];
	uint16_t appKeyIndex_g = 0;

	if(p->para_count < 6)
	{
		printf(">>>>mesh light ctl default set fail, too few parameters\n");
		return;
	}
	if(p->para_count > 6)
	{
		appKeyIndex_g = p->dw_parameter[6];
	}

	ret = rtkmesh_light_ctl_default_set(element_index, dst, appKeyIndex_g, lightness, temperature, delta, ack);

	if(ret == 0)
	{
		printf(">>>>mesh light ctl default set success\n");
	}
	else
	{
		printf(">>>>mesh light ctl default set fail, err code %d\n", ret);
	}
}

static void cmd_cfg_model_net_trans_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint8_t count;
	uint8_t steps;

	if(p->para_count < 3)
	{
		printf(">>>>mesh cfg net trans set fail, too few parameters\n");
		return;
	}
	dst = (uint16_t)p->dw_parameter[0];
	count = (uint8_t )p->dw_parameter[1];
	steps = (uint8_t )p->dw_parameter[2];

	ret = rtkmesh_cfg_model_net_trans_set(dst, count, steps);
	if(ret == 0)
	{
		printf(">>>>mesh cfg net trans set success\n");
	}
	else
	{
		printf(">>>>mesh cfg net trans set fail, err code %d\n", ret);
	}

}

static void cmd_cfg_model_net_trans_get(user_cmd_parse_value_t *p)
{

	int32_t ret;
	uint16_t dst;

	if(p->para_count < 1)
	{
		printf(">>>>mesh cfg net trans get fail, too few parameters\n");
		return;
	}
	dst = (uint16_t)p->dw_parameter[0];

	ret = rtkmesh_cfg_model_net_trans_get(dst );
	if(ret == 0)
	{
		printf(">>>>mesh cfg net trans get success\n");
	}
	else
	{
		printf(">>>>mesh cfg net trans get fail, err code %d\n", ret);
	}
}
static void cmd_cfg_model_beacon_get(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;

	if(p->para_count < 1)
	{
		printf(">>>>mesh cfg beacon get fail, too few parameters\n");
		return;
	}
	dst = (uint16_t)p->dw_parameter[0];

	ret = rtkmesh_cfg_model_beacon_get(dst );
	if(ret == 0)
	{
		printf(">>>>mesh cfg beacon get success\n");
	}
	else
	{
		printf(">>>>esh cfg beacon get fail, err code %d\n", ret);
	}

}
static void cmd_cfg_model_beacon_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint8_t state;

	if(p->para_count < 2)
	{
		printf(">>>>mesh cfg beacon set fail, too few parameters\n");
		return;
	}
	dst = (uint16_t)p->dw_parameter[0];
	state = (uint16_t)p->dw_parameter[1];

	ret = rtkmesh_cfg_model_beacon_set(dst,state );
	if(ret == 0)
	{
		printf(">>>>mesh cfg beacon set success\n");
	}
	else
	{
		printf(">>>>esh cfg beacon set fail, err code %d\n", ret);
	}

}
static void cmd_cfg_model_compo_data_get(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint8_t page;

	if(p->para_count < 2)
	{
		printf(">>>>mesh cfg compo data get fail, too few parameters\n");
		return;
	}
	dst = (uint16_t)p->dw_parameter[0];
	page = (uint16_t)p->dw_parameter[1];

	ret = rtkmesh_cfg_model_compo_data_get(dst,page );
	if(ret == 0)
	{
		printf(">>>>mesh cfg compo data get success\n");
	}
	else
	{
		printf(">>>>mesh cfg compo data get fail, err code %d\n", ret);
	}

}
static void cmd_cfg_model_default_ttl_get(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;

	if(p->para_count < 1)
	{
		printf(">>>>mesh cfg default ttl get fail, too few parameters\n");
		return;
	}
	dst = (uint16_t)p->dw_parameter[0];

	ret = rtkmesh_cfg_model_default_ttl_get(dst );
	if(ret == 0)
	{
		printf(">>>>mesh cfg default ttl get success\n");
	}
	else
	{
		printf(">>>>mesh cfg default ttl get fail, err code %d\n", ret);
	}

}
static void cmd_cfg_model_default_ttl_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint8_t ttl;

	if(p->para_count < 2)
	{
		printf(">>>>mesh cfg default ttl set fail, too few parameters\n");
		return;
	}
	dst = (uint16_t)p->dw_parameter[0];
	ttl = (uint8_t)p->dw_parameter[1];

	ret = rtkmesh_cfg_model_default_ttl_set(dst, ttl);
	if(ret == 0)
	{
		printf(">>>>mesh cfg default ttl set success\n");
	}
	else
	{
		printf(">>>>mesh cfg default ttl set fail, err code %d\n", ret);
	}

}
static void cmd_cfg_model_proxy_stat_get(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;

	if(p->para_count < 1)
	{
		printf(">>>>mesh cfg proxy state get fail, too few parameters\n");
		return;
	}
	dst = (uint16_t)p->dw_parameter[0];

	ret = rtkmesh_cfg_model_proxy_stat_get(dst);
	if(ret == 0)
	{
		printf(">>>>mesh cfg proxy state get success\n");
	}
	else
	{
		printf(">>>>mesh cfg proxy state get fail, err code %d\n", ret);
	}

}
static void cmd_cfg_model_proxy_stat_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint8_t proxy_state;

	if(p->para_count < 2)
	{
		printf(">>>>mesh cfg proxy state set fail, too few parameters\n");
		return;
	}
	dst = (uint16_t)p->dw_parameter[0];
	proxy_state = (uint8_t)p->dw_parameter[1];

	ret = rtkmesh_cfg_model_proxy_stat_set(dst, proxy_state );
	if(ret == 0)
	{
		printf(">>>>mesh cfg proxy state set success\n");
	}
	else
	{
		printf(">>>>mesh cfg proxy state set fail, err code %d\n", ret);
	}

}
static void cmd_cfg_model_relay_get(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;

	if(p->para_count < 1)
	{
		printf(">>>>mesh cfg relay get fail, too few parameters\n");
		return;
	}
	dst = (uint16_t)p->dw_parameter[0];

	ret = rtkmesh_cfg_model_relay_get(dst);
	if(ret == 0)
	{
		printf(">>>>mesh cfg relay get success\n");
	}
	else
	{
		printf(">>>>mesh cfg relay get fail, err code %d\n", ret);
	}

}
static void cmd_cfg_model_relay_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint8_t state;
	uint8_t count;
	uint8_t steps;

	if(p->para_count < 4)
	{
		printf(">>>>mesh cfg relay set fail, too few parameters\n");
		return;
	}
	dst = (uint16_t)p->dw_parameter[0];
	state = (uint8_t)p->dw_parameter[1];
	count = (uint8_t)p->dw_parameter[2];
	steps = (uint8_t)p->dw_parameter[3];

	ret = rtkmesh_cfg_model_relay_set(dst, state, count, steps);
	if(ret == 0)
	{
		printf(">>>>mesh cfg relay set success\n");
	}
	else
	{
		printf(">>>>mesh cfg relay set fail, err code %d\n", ret);
	}

}
static void cmd_cfg_model_pub_get(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint16_t element_index;
	uint32_t model_id;

	if(p->para_count < 3)
	{
		printf(">>>>mesh cfg model pub get fail, too few parameters\n");
		return;
	}

	dst = (uint16_t)p->dw_parameter[0];
	element_index = (uint16_t)p->dw_parameter[1];
	model_id = (uint32_t)p->dw_parameter[2];

	ret = rtkmesh_cfg_model_pub_get(dst, element_index, model_id );

	if(ret == 0)
	{
		printf(">>>>mesh cfg model pub get success\n");
	}
	else
	{
		printf(">>>>mesh cfg model pub get fail, err code %d\n", ret);
	}
}
static void cmd_cfg_model_pub_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint16_t element_index;
	uint16_t publish_addr;
	pub_key_info_t pub_key_info = {p->dw_parameter[4], p->dw_parameter[5], 0};;
	uint8_t ttl;
	pub_period_t pub_period = {p->dw_parameter[7] & 0x3f, p->dw_parameter[7] >> 6};
	pub_retrans_info_t pub_retrans_info = {p->dw_parameter[8], p->dw_parameter[9]};
	uint32_t model_id;
	bool va_flag;

	if(p->para_count < 11)
	{
		printf(">>>>mesh cfg model pub set fail, too few parameters\n");
		return;
	}

	dst = (uint16_t)p->dw_parameter[0];
	element_index = (uint16_t)p->dw_parameter[1];
	va_flag = p->dw_parameter[2]; // va_flag=1: virtual address
	publish_addr = (uint16_t)p->dw_parameter[3];

	ttl= p->dw_parameter[6];
	model_id = p->dw_parameter[10];

	ret = rtkmesh_cfg_model_pub_set(dst, element_index, publish_addr, pub_key_info, ttl, pub_period, pub_retrans_info, model_id );
	if(ret == 0)
	{
		printf(">>>>mesh cfg model pub set success\n");
	}
	else
	{
		printf(">>>>mesh cfg model pub set fail, err code %d\n", ret);
	}
}
static void cmd_cfg_model_vir_pub_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint16_t element_index;
	uint8_t publish_addr[16];
	pub_key_info_t pub_key_info = {p->dw_parameter[4], p->dw_parameter[5], 0};;
	uint8_t ttl;
	pub_period_t pub_period = {p->dw_parameter[7] & 0x3f, p->dw_parameter[7] >> 6};
	pub_retrans_info_t pub_retrans_info = {p->dw_parameter[8], p->dw_parameter[9]};
	uint32_t model_id;
	bool va_flag;

	if(p->para_count < 11)
	{
		printf(">>>>mesh cfg model vir pub set fail, too few parameters\n");
		return;
	}

	dst = (uint16_t)p->dw_parameter[0];
	element_index = (uint16_t)p->dw_parameter[1];
	va_flag = p->dw_parameter[2]; // va_flag=1: virtual address
	plt_hex_to_bin(publish_addr, (uint8_t*)p->pparameter[3], 16);

	ttl= p->dw_parameter[6];
	model_id = p->dw_parameter[10];

	ret = rtkmesh_cfg_model_vir_pub_set(dst, element_index, publish_addr, pub_key_info, ttl, pub_period, pub_retrans_info, model_id );
	if(ret == 0)
	{
		printf(">>>>mesh cfg model vir pub set success\n");
	}
	else
	{
		printf(">>>>mesh cfg model vir pub set fail, err code %d\n", ret);
	}
}

static void cmd_cfg_model_sub_add(user_cmd_parse_value_t *p)
{
	int32_t ret;

	uint16_t dst = 0;
	uint8_t element_index = 0;
	uint32_t model_id = 0;
	uint16_t group_addr = 0;

	if(p->para_count < 4)
	{
		printf(">>>>mesh cfg model sub add fail, too few parameters\n");
		return;
	}

	dst = (uint16_t)p->dw_parameter[0];
	element_index = (uint8_t)p->dw_parameter[1];
	model_id = (uint32_t)p->dw_parameter[2];
	group_addr = (uint16_t)p->dw_parameter[3];

	ret = rtkmesh_cfg_model_sub_add(dst, element_index, model_id, group_addr);
	if(ret == 0)
	{
		printf(">>>>mesh cfg model sub add success\n");
	}
	else
	{
		printf(">>>>mesh cfg model sub add fail, err code %d\n", ret);
	}
}

static void cmd_cfg_model_vir_sub_add(user_cmd_parse_value_t *p)
{
	int32_t ret;

	uint16_t dst = 0;
	uint8_t element_index = 0;
	uint32_t model_id = 0;
	uint8_t group_addr[16];

	if(p->para_count < 4)
	{
		printf(">>>>mesh cfg model vir sub add fail, too few parameters\n");
		return;
	}

	dst = (uint16_t)p->dw_parameter[0];
	element_index = (uint8_t)p->dw_parameter[1];
	model_id = (uint32_t)p->dw_parameter[2];
	plt_hex_to_bin(group_addr, (uint8_t*)p->pparameter[3],16);

	ret = rtkmesh_cfg_model_vir_sub_add(dst, element_index, model_id, group_addr);
	if(ret == 0)
	{
		printf(">>>>mesh cfg model vir sub add success\n");
	}
	else
	{
		printf(">>>>mesh cfg model vir sub add fail, err code %d\n", ret);
	}
}

static void cmd_cfg_model_sub_delete(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint8_t element_index;
	uint32_t model_id;
	uint16_t group_addr;

	if(p->para_count < 4)
	{
		printf(">>>>mesh cfg model sub delete fail, too few parameters\n");
		return;
	}

	dst = (uint16_t)p->dw_parameter[0];
	element_index = (uint8_t)p->dw_parameter[1];
	model_id = (uint32_t )p->dw_parameter[2];
	group_addr = (uint16_t )p->dw_parameter[3];

	ret = rtkmesh_cfg_model_sub_delete(dst, element_index, model_id, group_addr);
	if(ret == 0)
	{
		printf(">>>>mesh cfg model sub delete success\n");
	}
	else
	{
		printf(">>>>mesh cfg model sub delete fail, err code %d\n", ret);
	}
}

static void cmd_cfg_model_vir_sub_delete(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint8_t element_index;
	uint32_t model_id;
	uint8_t group_addr[16];

	if(p->para_count < 4)
	{
		printf(">>>>mesh cfg model sub delete fail, too few parameters\n");
		return;
	}

	dst = (uint16_t)p->dw_parameter[0];
	element_index = (uint8_t)p->dw_parameter[1];
	model_id = (uint32_t )p->dw_parameter[2];
	plt_hex_to_bin(group_addr, (uint8_t* )p->pparameter[3], 16);

	ret = rtkmesh_cfg_model_vir_sub_delete(dst, element_index, model_id, group_addr);
	if(ret == 0)
	{
		printf(">>>>mesh cfg model sub delete success\n");
	}
	else
	{
		printf(">>>>mesh cfg model sub delete fail, err code %d\n", ret);
	}
}

static void cmd_cfg_model_sub_overwrite(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint8_t element_index;
	uint32_t model_id;
	uint16_t sub_addr;

	if(p->para_count < 4)
	{
		printf(">>>>mesh cfg model sub overwrite fail, too few parameters\n");
		return;
	}

	dst = (uint16_t)p->dw_parameter[0];
	element_index = (uint8_t)p->dw_parameter[1];
	model_id = (uint32_t )p->dw_parameter[2];
	sub_addr = (uint16_t )p->dw_parameter[3];

	ret = rtkmesh_cfg_model_sub_overwrite(dst, element_index, model_id, sub_addr);
	if(ret == 0)
	{
		printf(">>>>mesh cfg model sub overwrite success\n");
	}
	else
	{
		printf(">>>>mesh cfg model sub overwrite fail, err code %d\n", ret);
	}
}

static void cmd_cfg_model_vir_sub_overwrite(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint8_t element_index;
	uint32_t model_id;
	uint8_t sub_addr[16];

	if(p->para_count < 4)
	{
		printf(">>>>mesh cfg model vir sub overwrite fail, too few parameters\n");
		return;
	}

	dst = (uint16_t)p->dw_parameter[0];
	element_index = (uint8_t)p->dw_parameter[1];
	model_id = (uint32_t )p->dw_parameter[2];
	plt_hex_to_bin(sub_addr, (uint8_t*)p->pparameter[3],16);

	ret = rtkmesh_cfg_model_vir_sub_overwrite(dst, element_index, model_id, sub_addr);
	if(ret == 0)
	{
		printf(">>>>mesh cfg model vir sub overwrite success\n");
	}
	else
	{
		printf(">>>>mesh cfg model vir sub overwrite fail, err code %d\n", ret);
	}
}

static void cmd_cfg_model_sub_delete_all(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint8_t element_index;
	uint32_t model_id;

	if(p->para_count < 3)
	{
		printf(">>>>mesh cfg model sub delete all fail, too few parameters\n");
		return;
	}

	dst = (uint16_t)p->dw_parameter[0];
	element_index = (uint8_t)p->dw_parameter[1];
	model_id = (uint32_t )p->dw_parameter[2];

	ret = rtkmesh_cfg_model_sub_delete_all(dst, element_index, model_id);
	if(ret == 0)
	{
		printf(">>>>mesh cfg model sub delete all success\n");
	}
	else
	{
		printf(">>>>mesh cfg model sub delete fail, err code %d\n", ret);
	}
}

static void cmd_cfg_model_hb_pub_set(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;
	uint16_t dst_pub;
	uint8_t count_log;
	uint8_t period_log;
	uint8_t ttl;
	uint16_t features;
	uint16_t net_key_index;

	if(p->para_count == 1)
	{
		dst = (uint16_t)p->dw_parameter[0];
		dst_pub = local_unicast_addr;
		count_log = 0xff;
		period_log = 0x07;
		ttl = 10;
		features = 0;
		net_key_index = 0;

	}
	else if(p->para_count < 7)
	{
		printf(">>>>mesh cfg model hb publish set fail, too few parameters\n");
		return;
	}
	else
	{
		dst = (uint16_t)p->dw_parameter[0];
		dst_pub = (uint16_t)p->dw_parameter[1];
		count_log = (uint8_t )p->dw_parameter[2];
		period_log = (uint8_t)p->dw_parameter[3];
		ttl = (uint8_t)p->dw_parameter[4];
		features = (uint16_t )p->dw_parameter[5];
		net_key_index = (uint16_t )p->dw_parameter[6];

	}
	ret = rtkmesh_cfg_model_hb_pub_set(dst, dst_pub, count_log, period_log,
                                     ttl, features, net_key_index);
	if(ret == 0)
	{
		printf(">>>>mesh cfg model hb publish set success\n");
	}
	else
	{
		printf(">>>>mesh cfg model hb publish set fail, err code %d\n", ret);
	}
}


static void cmd_cfg_model_node_reset(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint16_t dst;

	if(p->para_count < 1)
	{
		printf(">>>>mesh cfg model node reset fail, too few parameters\n");
		return;
	}

	dst = (uint16_t)p->dw_parameter[0];

	ret = rtkmesh_cfg_model_node_reset(dst );
	if(ret == 0)
	{
		printf(">>>>mesh cfg model node reset success\n");
	}
	else
	{
		printf(">>>>mesh cfg model node reset fail, err code %d\n", ret);
	}
}

static void cmd_prov_invite(user_cmd_parse_value_t *p)
{
	int32_t ret;
	char* strDeviceUUID;
	uint8_t deviceUUID[16]={0};
	uint16_t assign_address = 0x00;
	uint32_t attention_dur = 0;
	bool enable = 0;

	if(p->para_count <1)
	{
		printf(">>>>invite prov fail, too few parameter\n");
		return;
	}
	strDeviceUUID = p->pparameter[0];
	plt_hex_to_bin(deviceUUID, (uint8_t *)strDeviceUUID, 16);

	if(p->para_count >= 2)
	{
		assign_address = (uint16_t )p->dw_parameter[1];
	}
	if(p->para_count >= 4)
	{
		attention_dur = (uint32_t )p->dw_parameter[2];
		enable = !!p->dw_parameter[3];
	}
	ret = rtkmesh_prov(deviceUUID, assign_address, attention_dur, enable);

	if(ret == 0)
	{
		printf(">>>>invite prov success\n");
	}
	else
	{
		printf(">>>>invite prov fail, err code %d\n", ret);
	}
}

static void cmd_prov_method_choose(user_cmd_parse_value_t *p)
{
	int32_t ret;
	prov_start_public_key_t public_key = 0;
	prov_auth_method_t auth_method = 0;
	prov_auth_action_t auth_action;
	prov_auth_size_t auth_size;

	memset(&auth_action,0,sizeof(prov_auth_action_t));
	memset(&auth_size,0,sizeof(prov_auth_size_t));

	if(p->para_count == 4 )
	{
		printf(">>>>set prov_start PDU\n");
		public_key = (prov_start_public_key_t)p->dw_parameter[0];
		auth_method = (prov_auth_method_t)p->dw_parameter[1];
		auth_action.oob_action= (uint8_t)p->dw_parameter[2];
		auth_size.oob_size= (uint8_t)p->dw_parameter[3];
	}
	auth_method_record = auth_method;
	auth_action_record = auth_action;
	auth_size_record = auth_size;

	switch (auth_method)
    {
    case PROV_AUTH_METHOD_STATIC_OOB:
        auth_value_type_record = PROV_AUTH_VALUE_TYPE_BINARY;
        break;
    case PROV_AUTH_METHOD_OUTPUT_OOB:
        if (auth_action.output_oob_action == PROV_START_OUTPUT_OOB_ACTION_OUTPUT_ALPHANUMERIC)
        {
            auth_value_type_record = PROV_AUTH_VALUE_TYPE_ALPHANUMERIC;
        }
        else
        {
            auth_value_type_record = PROV_AUTH_VALUE_TYPE_NUMERIC;
        }
        break;
    case PROV_AUTH_METHOD_INPUT_OOB:
        if (auth_action.input_oob_action == PROV_START_INPUT_OOB_ACTION_INPUT_ALPHANUMERIC)
        {
            auth_value_type_record = PROV_AUTH_VALUE_TYPE_ALPHANUMERIC;
        }
        else
        {
            auth_value_type_record = PROV_AUTH_VALUE_TYPE_NUMERIC;
        }
        break;
    default:
        auth_value_type_record = PROV_AUTH_VALUE_TYPE_NONE;
        break;
    }

	ret = rtkmesh_prov_method_choose(public_key, auth_method, auth_action, auth_size);

	if(ret == 0)
	{
		printf(">>>>choose prov method success\n");
	}
	else
	{
		printf(">>>>choose prov method fail, err code %d\n", ret);
	}
}

static void cmd_prov_device_public_key_set(user_cmd_parse_value_t *p)
{
	int32_t ret;

	uint8_t public_key[64];

	if(p->para_count < 1)
	{
		printf(">>>mesh set device public key fail, too few parameter\n");
		return;
	}

	plt_hex_to_bin(public_key, (uint8_t*)p->pparameter[0], sizeof(public_key));

	ret = rtkmesh_prov_device_public_key_set(public_key );

	if(ret == 0)
	{
		printf(">>>>mesh set device public key success\n");
	}
	else
	{
		printf(">>>>mesh set device public key fail, err code %d\n", ret);
	}
}

static void cmd_prov_auth_value_set(user_cmd_parse_value_t *p)
{
	int32_t ret;

	uint8_t auth_data[16];
	uint8_t auth_value_len;
	uint8_t *auth_value;

	if(p->para_count < 1)
	{
		printf(">>>set auth value fail, too few parameter\n");
		return;
	}

	if (auth_value_type_record != PROV_AUTH_VALUE_TYPE_ALPHANUMERIC)
	{
		auth_value_len = plt_hex_to_bin(auth_data, (uint8_t *)p->pparameter[0],
										sizeof(auth_data));
		auth_value = auth_data + sizeof(auth_data) - auth_value_len;
	}
	else
	{
		auth_value_len = strlen(p->pparameter[0]);
		auth_value = (uint8_t *)p->pparameter[0];
	}
	data_uart_debug("AuthValue: len=%d, value=", auth_value_len);
	data_uart_dump(auth_value, auth_value_len);

	ret = rtkmesh_prov_auth_value_set(auth_value, auth_value_len);

	if(ret == 0)
	{
		printf(">>>>set auth value success\n");
	}
	else
	{
		printf(">>>>set auth value fail, err code %d\n", ret);
	}
}

static void cmd_set_local_net_key(user_cmd_parse_value_t *p)
{
	int32_t ret;
	int32_t netKeyIndex = 0;
	char *strNetkey = NULL;
	uint8_t netkey[16];
	bool bUpdate = 0;
	if(p->para_count < 1)
	{
		printf(">>>mesh set net key,too few parameter\n");
		return;
	}
	netKeyIndex = p->dw_parameter[0];
	if(p->para_count < 2)
	{
		strNetkey = defaultNetKeyStr;
	}
	else
	{
		strNetkey = p->pparameter[1];
	}
	plt_hex_to_bin(netkey, (uint8_t*)strNetkey , sizeof(netkey));

	ret = rtkmesh_set_local_netkey(netKeyIndex, netkey, bUpdate );

	if(ret == 0)
	{
		printf(">>>>mesh set net key success\n");
	}
	else
	{
		printf(">>>>mesh set net key fail, err code %d\n", ret);
	}
}

static void cmd_set_local_app_key(user_cmd_parse_value_t *p)
{
	int32_t ret;
	int32_t appKeyIndex = 0;
	int32_t netKeyIndex = 0;
	char* strAppkey = NULL;
	uint8_t appkey[16]={0};
	bool bUpdate = 0;

	if(p->para_count < 2)
	{
		printf(">>>>mesh set app key fail, too few parameters\n");
		return;
	}
	appKeyIndex = p->dw_parameter[0];
	netKeyIndex = p->dw_parameter[1];
	if(p->para_count < 3)
	{
		strAppkey = defaultAppKeyStr;
	}
	else
	{
		strAppkey = p->pparameter[2];
	}
	plt_hex_to_bin(appkey, (uint8_t*)strAppkey, sizeof(appkey));

	ret = rtkmesh_set_local_appkey(appKeyIndex, netKeyIndex, appkey, bUpdate);
	if(ret == 0)
	{
		printf(">>>>mesh set app key success\n");
	}
	else
	{
		printf(">>>>mesh set app key fail, err code %d\n", ret);
	}
}

static void cmd_set_model_bind_local_app_key(user_cmd_parse_value_t *p)
{
	int32_t ret;
	uint8_t element_index = 0;
	uint32_t model_id = 0;
	uint32_t appKeyIndex_g = 0;
	uint8_t bindAllAppKeyFlag = 0;

	if(p->para_count < 4)
	{
		printf(">>>>mesh set model bind local appkey fail, too few parameters\n");
		return;
	}

	if(p->dw_parameter[3] > 0x02)
	{
		printf(">>>>mesh set model bind local appkey fail, parameter 3 is error\n");
		return;
	}
	element_index =(uint8_t)p->dw_parameter[0];
	model_id = p->dw_parameter[1];
	appKeyIndex_g = p->dw_parameter[2];
	bindAllAppKeyFlag = (uint8_t)p->dw_parameter[3];

	ret = rtkmesh_model_appkey_bind(element_index, model_id, appKeyIndex_g, bindAllAppKeyFlag);

	if(ret == 0)
	{
		printf(">>>>mesh set model bind local appkey success\n");
	}
	else
	{
		printf(">>>>mesh set model bind local appkey fail, err code %d\n", ret);
	}
}

static void cmd_get_local_current_ttl(user_cmd_parse_value_t *p)
{
	int32_t ret;

	uint8_t ttl;

	ret = rtkmesh_local_ttl_get(&ttl);

	if(ret == 0)
	{
		printf(">>>>mesh get default ttl success, ttl is %d\n",ttl);
	}
	else
	{
		printf(">>>>mesh get default ttl fail, err code %d\n", ret);
	}
}
static void cmd_get_local_btMAC(user_cmd_parse_value_t *p)
{
	int32_t ret = 0;

	uint8_t addr[6];

	ret = rtkmesh_bt_MAC_get(addr);

	if(ret == 0)
	{
		printf(">>>>mesh get MAC addr success\n");
		printf("addr %x:%x:%x:%x:%x:%x\n",addr[5],addr[4],addr[3],addr[2],addr[1],addr[0]);
	}
	else
	{
		printf(">>>>mesh get MAC addr fail, err code %d\n", ret);
	}

}
static void cmd_set_mesh_ble_scan(user_cmd_parse_value_t *p)
{
	int32_t ret = 0;

	uint16_t mesh_ble_scan_interval = 0x20;
	uint16_t mesh_ble_scan_window = 0x20;

	if(p->para_count != 2)
	{
		printf(">>>>mesh set scan param err: param count is error \n");
		return;
	}
	mesh_ble_scan_interval = (uint16_t)p->dw_parameter[0];
	mesh_ble_scan_window = (uint16_t)p->dw_parameter[1];

	ret = rtkmesh_set_scan_param(mesh_ble_scan_interval, mesh_ble_scan_window);

	if(ret == 0)
	{
		printf(">>>>mesh set scan param success\n");
	}
	else
	{
		printf(">>>>mesh set scan param fail, err code %d\n", ret);
	}
}
static void cmd_set_mesh_mode(user_cmd_parse_value_t *p)
{
	int32_t ret = 0;
	uint8_t enable = 0;
	if(p->para_count != 1)
	{
		printf(">>>>mesh set mode: param count is error \n");
		return;
	}
	enable = (uint8_t)p->dw_parameter[0];
	ret = rtkmesh_set_mesh_mode(enable);

	if(ret == 0)
	{
		printf(">>>>mesh set mode success\n");
	}
	else
	{
		printf(">>>>mesh set mode fail, err code %d\n", ret);
	}
}
static void cmd_set_scan_param_when_a2dp_active(user_cmd_parse_value_t *p)
{
	int32_t ret = 0;
	uint16_t scan_interval;
	uint16_t scan_windows;
	if(p->para_count != 2)
	{
		printf(">>>>mesh set default scan param : param count is error \n");
		return;
	}
	scan_interval = (uint16_t)p->dw_parameter[0];
	scan_windows = (uint16_t)p->dw_parameter[1];

	ret = rtkmesh_set_scan_param_when_a2dp_active(scan_interval, scan_windows);

	if(ret == 0)
	{
		printf(">>>>mesh set default scan param success\n");
	}
	else
	{
		printf(">>>>mesh set default scan param fail, err code %d\n", ret);
	}

	return;
}
static void cmd_set_scan_high_prioirty(user_cmd_parse_value_t *p)
{
	int32_t ret = 0;
	uint8_t enable = 0;
	uint8_t highTime = 0;

	if(p->para_count != 2)
	{
		printf(">>>>mesh set scan high : param count is error \n");
		return;
	}

	enable = (uint8_t)p->dw_parameter[0];
	highTime = (uint8_t)p->dw_parameter[1];

	ret = rtkmesh_set_scan_high_prioirty(enable, highTime);

	if(ret == 0)
	{
		printf(">>>>mesh set scan high prioirty success\n");
	}
	else
	{
		printf(">>>>mesh set scan high prioirty fail, err code %d\n", ret);
	}

	return;
}

static void cmd_reset_local_node(user_cmd_parse_value_t *p)
{
	int32_t ret = 0;

	ret = rtkmesh_local_node_reset();

	if(ret == 0)
	{
		printf(">>>>mesh reset local node success\n");
	}
	else
	{
		printf(">>>>mesh reset local node fail, err code %d\n", ret);
	}

}
static void cmd_list_node_info()
{
	int32_t ret = 0;
	ret = rtkmesh_mesh_info_list();
	if(ret == 0)
	{
		printf(">>>>list all mesh node info success\n");
	}
	else
	{
		printf(">>>>list all mesh node info fail, err code %d\n", ret);
	}
}
static void cmd_mesh_thread_quit()
{
	quit_mesh_bta_thread();
	return;
}
void cmd_goo_set_for_get_status(user_cmd_parse_value_t *p)
{
	int32_t ret;

    bool optional = FALSE;
	uint16_t element_index = 0;
	uint16_t dst = p->dw_parameter[0];
	generic_on_off_t on_off = (generic_on_off_t)p->dw_parameter[1];
	bool ack = p->dw_parameter[2];
	uint16_t appKeyIndex_g = 0;

    generic_transition_time_t trans_time = {p->dw_parameter[4],
                                            p->dw_parameter[5]
                                           };
	uint8_t delay = p->dw_parameter[6];

	if(p->para_count < 3)
	{
		printf(">>>>mesh goo set fail, too few parameters\n");
		return;
	}

	if(p->para_count > 3)
	{
		appKeyIndex_g = (uint16_t)p->dw_parameter[3];
	}

    if (p->para_count == 7 )
    {
        optional = TRUE;
    }

	ret = rtkmesh_generic_on_off_set(element_index, dst, appKeyIndex_g, on_off,
									optional, trans_time, delay, ack);
	if(ret == 0)
	{

		struct itimerspec itimer;
		memset(&itimer, 0, sizeof(itimer));

		itimer.it_interval.tv_sec = 0;
		itimer.it_interval.tv_nsec = 0;

		itimer.it_value.tv_sec = timer_interval/1000000000;
		itimer.it_value.tv_nsec = timer_interval- (timer_interval/1000000000)*1000000000;

		element_index_record = 0;
		dst_record = dst;
		appkeyindex_g_record = appKeyIndex_g;
		inquiry_retry_count = status_get_retry;

		timerfd_settime(waitForStateTimer_fd, 0, &itimer, NULL);

		printf(">>>>mesh goo set success\n");
	}
	else
	{
		printf(">>>>mesh goo set fail, err code %d\n", ret);
	}
}
static void cmd_set_inquiry_retry_times(user_cmd_parse_value_t *p)
{
	if(p->para_count < 1)
	{
		printf(">>>>set get status retry times, too few parameters\n");
		return;
	}

	status_get_retry = p->dw_parameter[0];
	printf(">>>>set get status retry times %d \n",status_get_retry);
	return;
}
static void cmd_set_inquiry_retry_interval(user_cmd_parse_value_t *p)
{
	if(p->para_count < 1)
	{
		printf(">>>>set get status retry times, too few parameters\n");
		return;
	}

	timer_interval = p->dw_parameter[0];
	printf(">>>>set get status retry interval %d ns \n",timer_interval);
	return;
}
static void cmd_goo_set_test(user_cmd_parse_value_t *p)
{
	bool test_start = p->dw_parameter[4];
	if(test_start)
	{
		struct itimerspec itimer;
		goo_send_count = 0;
		goo_recv_count = 0;
		printf("start light on/off test...\n");

		memset(&itimer, 0, sizeof(itimer));

		itimer.it_interval.tv_sec = 0;
		itimer.it_interval.tv_nsec = 0;

		itimer.it_value.tv_sec = 10;
		itimer.it_value.tv_nsec = 0;

		timerfd_settime(OnOffTestTimer_fd, 0, &itimer, NULL);
		cmd_goo_set_for_get_status(p);
		goo_send_count ++;
		need_one_response = 1;
	}
	else
	{
		struct itimerspec itimer;
		printf("stop light on/off test...\n");

		memset(&itimer, 0, sizeof(itimer));

		itimer.it_interval.tv_sec = 0;
		itimer.it_interval.tv_nsec = 0;

		itimer.it_value.tv_sec = 0;
		itimer.it_value.tv_nsec = 0;

		timerfd_settime(OnOffTestTimer_fd, 0, &itimer, NULL);
	}

}

static void cmd_intensive_adv_set(user_cmd_parse_value_t *p)
{
	int32_t ret = 0;
	uint32_t interval = 20; //20ms
	uint32_t duration = 1000; //1000ms
	uint32_t adjust_interval = 5;
	uint32_t scan_window_per_interval = 9;

	if(p->para_count != 4)
	{
		printf(">>>>intensive adv set: use the default parameter \n");
	}
	else
	{
		interval = (uint32_t)p->dw_parameter[0];
		duration = (uint32_t)p->dw_parameter[1];
		adjust_interval = (uint32_t)p->dw_parameter[2];
		scan_window_per_interval = (uint32_t)p->dw_parameter[3];
		if(duration < interval)
		{
			printf(">>>>intensive adv set error: duration < interval \n");
			return;
		}
	}
	ret = rtkmesh_intensive_adv_set( interval, duration, adjust_interval, scan_window_per_interval);

	if(ret == 0)
	{
		printf(">>>>intensive adv set success: interval %d ms, duration %d ms adjust_interval %d ms, scan_window_per_interval %d ms \n",interval,duration,adjust_interval,scan_window_per_interval);
	}
	else
	{
		printf(">>>>intensive adv set fail, err code %d\n", ret);
	}
}

cmd_list_t cmd_table[85] = {
	{ "quit",					NULL,		cmd_quit, 									"quit: Quit program" },
	{ "launch",					NULL,		cmd_mesh_thread_launch, 					"lanuch: lanuch mesh thread" },
	{ "shutdown", 				NULL,   	cmd_mesh_thread_quit, 						"shutdown: quit mesh thread"},
	{ "seqinit",				NULL,		cmd_mesh_seq_init,							"seqinit[seq]:seq init"},
	{ "ivinit",					NULL,		cmd_mesh_iv_init,							"ivinit[iv_index]:iv_index init"},
	{ "seqget",					NULL,		cmd_mesh_iv_seq_get,						"seqget:get seq and iv"},
	{ "nng",					NULL,		cmd_mesh_node_num_get,						"nng:get node number"},
	{ "hbms",					NULL,		cmd_mesh_hb_monitor_set,					"hbms[count][timeout]:set heartbeat monitor"},
	{ "scan",         			NULL,		cmd_device_scan, 							"scan [enable][msec]:scan udb"},
	{ "enable",        			NULL,		cmd_mesh_stack_enable, 						"enable <uuid>: enable mesh stack"},
	{ "getaddr", 				NULL,		cmd_get_local_element_addr,					"getaddr [element_index]: get element addr"},
	{ "setnetkey", 				NULL,		cmd_set_local_net_key,						"setnetkey <netkey_index_g> <key> <update>: set net key"},
	{ "setappkey", 				NULL,		cmd_set_local_app_key,						"setappkey [appkey_index_g] [netkey_index_g] <key> <update>: set net key"},
	{ "appkeybind", 			NULL,		cmd_set_model_bind_local_app_key,			"appkeybind [element_index] [model_id] [appkeyindex_g] [flag]: set model bind local appkey"},
	{ "getttl",         		NULL,      	cmd_get_local_current_ttl, 					"getttl: get local ttl"},
	{ "mac",         			NULL,      	cmd_get_local_btMAC, 						"mac: get local mac addr"},
	{ "set_ble_scan",       	NULL,      	cmd_set_mesh_ble_scan, 						"set_ble_scan [interval] [window]: set ble scan param"},
	{ "set_mesh_mode",       	NULL,      	cmd_set_mesh_mode, 							"set_mesh_mode [enable]: set mesh active or passive"},
	{ "reset",         			NULL,      	cmd_reset_local_node, 						"reset: clear local mesh info"},
	{ "setdevkey",         		NULL,      	cmd_set_local_dev_key, 						"setdevkey [primary_addr][element_num]<devKey>: set dev key"},
	{ "deldevkey",         		NULL,      	cmd_del_local_dev_key, 						"deldevkey [addr]: del dev key"},
	{ "modelreg",         		NULL,      	cmd_reg_model, 								"modelreg: register lightness client model"},
	{ "aka_g",         			NULL,      	cmd_cfg_appkey_add, 						"aka_g [dst] [netkey_index_g] [appkey_index_g]: cfg app key add"},
	{ "mab_g",        			NULL,      	cmd_cfg_model_appkey_bind, 					"mab_g [dst] [element_index] [model_id] [app_key_index_g]: model appkey bind"},
	{ "ls",						NULL,		cmd_list_node_info,							"ls: list all mesh node info"},
	{ "provinv",				NULL, 		cmd_prov_invite,							"prov[uuid]<assign_addr><attn_dur><manual>: provision a new mesh device "},
	{ "provstart",				NULL,		cmd_prov_method_choose,						"provstart[public_key][auth_method][auth_action][auth_size]: set prov_start pdu data "},
	{ "devpublickey",			NULL,		cmd_prov_device_public_key_set, 			"devpublickey[public_key]: set device public key"},
	{ "authvalue",				NULL,		cmd_prov_auth_value_set, 					"devpublickey[public_key]: set device public key"},
	{ "goos",         			NULL,      	cmd_goo_set, 								"goos [dst] [on/off] [ack] <appkey_index_g> <steps> <resolution> <delay>:generic on off set"},
	{ "goog",         			NULL,      	cmd_goo_get, 								"goog [dst] <appkey_index_g>:generic on off get"},
	{ "lls",         			NULL,      	cmd_llc_set, 								"lls [elementIndex][dst] [lightness] [ack] <appkey_index_g> <steps> <resolution> <delay>:light lightness linear set"},
	{ "llg",         			NULL,      	cmd_llc_get, 								"llg [elementIndex][dst] <appkey_index_g>:light lightness get"},
	{ "hslg",         			NULL,      	cmd_light_hsl_get, 							"hslg [elementIndex][dst] <appkey_index_g>:light hsl get"},
	{ "hsls",         			NULL,      	cmd_light_hsl_set, 							"hsls [elementIndex][dst][lightness][hue][saturation][ack]<appkey_index_g><steps><resolution><delay>:light hsl set"},
	{ "hsltg", 					NULL, 		cmd_light_hsl_target_get,					"hsltg [elementIndex][dst] <appkey_index_g>:light hsl target get"},
	{ "hslhg",         			NULL,      	cmd_light_hsl_hue_get, 						"hslhg [elementIndex][dst] <appkey_index_g>:light hsl hue get"},
	{ "hslhs",         			NULL,      	cmd_light_hsl_hue_set, 						"hslhs [elementIndex][dst][hue][ack]<appkey_index_g><steps><resolution><delay>:light hsl hue set"},
	{ "hslsg",					NULL, 		cmd_light_hsl_saturation_get,				"hslsg [elementIndex][dst] <appkey_index_g>:light hsl saturation get"},
	{ "hslss",					NULL, 		cmd_light_hsl_saturation_set,				"hslss [elementIndex][dst][saturation][ack]<appkey_index_g><steps><resolution><delay>:light hsl default set"},
	{ "hsldg",					NULL, 		cmd_light_hsl_default_get, 					"hsldg [elementIndex][dst] <appkey_index_g>:light hsl default get"},
	{ "hslds",					NULL, 		cmd_light_hsl_default_set, 					"hslds [elementIndex][dst][lightness][hue][saturation][ack]<appkey_index_g>:light hsl default set"},
	{ "hslrg",					NULL, 		cmd_light_hsl_range_get,					"hslrg [elementIndex][dst] <appkey_index_g>:light hsl range get"},
	{ "hslrs",					NULL, 		cmd_light_hsl_range_set,					"hslrs [elementIndex][dst][hue_min][hue_max][saturation_min][saturation_max][ack]<appkey_index_g>:light hsl range set"},
	{ "lcg",         			NULL,      	cmd_light_ctl_get, 							"lcg [elementIndex][dst] <appkey_index_g>:light ctl get"},
	{ "lcs",					NULL,		cmd_light_ctl_set,							"lcs [elementIndex][dst][lightness][temperature] [delay_uv] [ack] <appkey_index_g> <steps> <resolution> <delay>:light ctl set"},
	{ "lctg",					NULL,		cmd_light_ctl_temperature_get,				"lctg [elementIndex][dst] <appkey_index_g> :light temperature get"},
	{ "lcts",					NULL,		cmd_light_ctl_temperature_set,				"lcts [elementIndex][dst] [temperature] [delay_uv] [ack] <appkey_index_g> <steps> <resolution> <delay>:light temperature set"},
	{ "lctrg",					NULL,		cmd_light_ctl_temperature_range_get,		"lctrg [elementIndex][dst] <appkey_index_g> :light temperature range get"},
	{ "lctrs",					NULL,		cmd_light_ctl_temperature_range_set,		"lctrs [elementIndex][dst][min][max][ack] <appkey_index_g> :light temperature range set"},
	{ "lcdg",					NULL,		cmd_light_ctl_default_get,					"lcdg [elementIndex][dst] <appkey_index_g>:light ctl default get"},
	{ "lcds",					NULL,		cmd_light_ctl_default_set,					"lcds [elementIndex][dst][lightness][temperature][delta_uv][ack]<appkey_index_g>:light ctl default set"},
	{ "nts",         			NULL,      	cmd_cfg_model_net_trans_set, 				"nts [dst] [count] [steps]:cfg network transmit set"},
	{ "ntg",					NULL,	   	cmd_cfg_model_net_trans_get, 				"ntg [dst] :cfg network transmit get"},
	{ "cbg",					NULL,	   	cmd_cfg_model_beacon_get, 					"cbg [dst] :cfg beacon get"},
	{ "cbs",					NULL,	   	cmd_cfg_model_beacon_set, 					"cbs [dst] [state] :cfg beacon set"},
	{ "ccdg",					NULL,	   	cmd_cfg_model_compo_data_get, 				"ccdg [dst] [page] :cfg composition data get"},
	{ "cttlg",					NULL,	   	cmd_cfg_model_default_ttl_get, 				"cttlg [dst] :cfg default ttl get"},
	{ "cttls",					NULL,	   	cmd_cfg_model_default_ttl_set,				"cttls [dst][ttl] :cfg default ttl set"},
	{ "cpsg",					NULL,	   	cmd_cfg_model_proxy_stat_get,				"cpsg [dst] :cfg proxy state get"},
	{ "cpss",					NULL,	   	cmd_cfg_model_proxy_stat_set,				"cpss [dst][proxy_state] :cfg proxy state set"},
	{ "crg",					NULL,	   	cmd_cfg_model_relay_get,					"crg [dst] :cfg relay state get"},
	{ "crs",					NULL,	   	cmd_cfg_model_relay_set, 					"crs [dst] :cfg relay state set"},
	{ "cpg",					NULL,	   	cmd_cfg_model_pub_get, 						"cpg [dst] [element_index] [model id] :cfg publish get"},
	{ "mps",					NULL,	   	cmd_cfg_model_pub_set,						"mps [dst] [element_index] [pub_addr][appkeyindex_g][va_flag][CredentialFlag][ttl][pub_period][RetransmitCount][RetransmitIntervalSteps][model_id]: model publish set"},
	{ "mvps",					NULL,	   	cmd_cfg_model_vir_pub_set,					"mvps [dst] [element_index] [pub_addr][appkeyindex_g][va_flag][CredentialFlag][ttl][pub_period][RetransmitCount][RetransmitIntervalSteps][model_id]: model viryual publish set"},
	{ "msa",					NULL,	   	cmd_cfg_model_sub_add,						"msa [dst] [element_index] [model_id] [group_addr]: model subsribe add"},
	{ "mvsa",					NULL,	   	cmd_cfg_model_vir_sub_add,					"mvsa [dst] [element_index] [model_id] [group_addr]: model virtual subsribe add"},
	{ "msd",					NULL,	   	cmd_cfg_model_sub_delete,					"msd [dst] [element_index] [model_id] [group_addr]: model subsribe delete"},
	{ "mvsd",					NULL,	   	cmd_cfg_model_vir_sub_delete,				"mvsd [dst] [element_index] [model_id] [group_addr]: model virtual subsribe delete"},
	{ "mso",					NULL,		cmd_cfg_model_sub_overwrite,				"mso [dst] [element_index] [model_id] [sub_addr]: model subsribe overwrite"},
	{ "mvso",					NULL,		cmd_cfg_model_vir_sub_overwrite,			"mvso [dst] [element_index] [model_id] [sub_addr]: model virtual subsribe overwrite"},
	{ "msdl",					NULL,		cmd_cfg_model_sub_delete_all,				"msdl [dst] [element_index] [model_id]: model subsribe delete all"},
	{ "hbps",					NULL,		cmd_cfg_model_hb_pub_set,					"hbps [dst] [dst_pub] [count_log] [period_log] [ttl] [features] [net_key_index]:hearbeat publish set"},
	{ "mnr",					NULL,		cmd_cfg_model_node_reset,					"mnr [dst]: model node reset"},
	{ "set_ble_scan2",         	NULL,      	cmd_set_scan_param_when_a2dp_active, 		"set_ble_scan2 [scan_interval] [scan_windows]"},
	{ "set_scan_high",         	NULL,      	cmd_set_scan_high_prioirty, 				"set_scan_high :set ble scan high prioirty"},
	{ "goos2",					NULL,		cmd_goo_set_for_get_status, 				"goos2 [dst] [on/off] [ack] <appkey_index_g> <steps> <resolution> <delay>:generic on off set"},
	{ "goos_test",				NULL,		cmd_goo_set_test,							"goos_test [dst][on/off] [ack] [appkey_index_g] [start]"},
	{ "setretrytimes",			NULL,		cmd_set_inquiry_retry_times, 				"setretrytime [times] set the times to retry get light on/off status when no reponse returns to goos2 cmd"},
	{ "setinterval",			NULL,		cmd_set_inquiry_retry_interval,				"setinterval [interval] set the interval to retry get light on/off status when no reponse returns to goos2 cmd"},
	{ "vgoos",         			NULL,      	cmd_goo_set_by_element1, 					"vgoos [dst] [on/off] [ack] <appkey_index_g> <steps> <resolution> <delay>:generic on off set"},
	{ "intensive_adv_set",		NULL,		cmd_intensive_adv_set,						"intensive_adv_set [interval] [duration]: set intensive adv param"},
	{ "help", 					NULL, 		NULL,										NULL },
	{ NULL,						NULL, 		NULL, 										NULL },
}; // 28

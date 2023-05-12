#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/signalfd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <getopt.h>
#include <bt_manager.h>

#include "bt_dev_list.h"
#include "bt_cmd.h"
#include "bt_log.h"

static dev_list_t *gatt_discovered_devices = NULL;
static btmg_callback_t *bt_callback = NULL;

#define TEST_SERVICE_UUID    "1112"
#define TEST_CHAR_UUID1      "2223"
#define TEST_CHAR_UUID2      "3334"
#define DEV_NAME_CHAR_UUID2      "2A00"

#define TEST_DESC_UUID      "00006666-0000-1000-8000-00805F9B34FB"

static uint16_t service_handle;
void bt_test_gatt_connection_cb(char *addr, gatt_connection_event_t event)
{
	if(event == BT_GATT_CONNECTION) {
		printf("gatt server connected to device: %s.\n", addr);
	} else if(event == BT_GATT_DISCONNECT) {
		printf("gatt server disconnected to device: %s.\n", addr);
	} else {
		printf("gatt server event unkown.\n");
	}

}

void bt_test_gatt_add_service_cb(gatt_add_svc_msg_t *msg)
{
	if(msg != NULL) {
		service_handle = msg->svc_handle;
		printf("add service handle is %d of number_hanle: %d\n",service_handle, msg->num_handle);
	}

}

static int char_handle;
static int dev_name_char_handle;
void bt_test_gatt_add_char_cb(gatt_add_char_msg_t *msg)
{
	if(msg != NULL) {
		printf("add char,uuid: %s,chr handle is %d\n",msg->uuid,msg->char_handle);
		if (strcmp(msg->uuid, DEV_NAME_CHAR_UUID2) == 0)
			dev_name_char_handle = msg->char_handle;
		else
			char_handle = msg->char_handle;
	}
}

void bt_test_gatt_add_desc_cb(gatt_add_desc_msg_t *msg)
{
	if(msg != NULL) {
		printf("service handle is %d\n", msg->desc_handle);
	}

}

void bt_test_gatt_char_read_request_cb(gatt_char_read_req_t *chr_read)
{
	char value[1];
	static unsigned char count = 0;
	char dev_name[] = "aw_ble_test_1149";

	printf("trans_id:%d,attr_handle:%d,offset:%d\n",chr_read->trans_id,
		chr_read->attr_handle,chr_read->offset);

	if(chr_read) {
		if (chr_read->attr_handle == dev_name_char_handle) {
			gatt_send_read_rsp_t data;
			data.trans_id = chr_read->trans_id;
			data.svc_handle = chr_read->attr_handle;
			data.status = 0x0b;
			data.auth_req = 0x00;
			value[0]= count;
			data.value = dev_name;
			data.value_len = strlen(dev_name);
			bt_manager_gatt_send_read_response(&data);
			return;
		}
		gatt_send_read_rsp_t data;
		data.trans_id = chr_read->trans_id;
		data.svc_handle = chr_read->attr_handle;
		data.status = 0x0b;
		data.auth_req = 0x00;
		value[0]= count;
		data.value = value;
		data.value_len = 1;
		bt_manager_gatt_send_read_response(&data);
		count ++;
	}
}
void bt_test_gatt_send_indication_cb(gatt_send_indication_t *send_ind)
{
	printf("enter\n");
}

void bt_test_gatt_char_write_request_cb(gatt_char_write_req_t *char_write)
{
	printf("enter\n");
	static unsigned char data_value[1] = {0};
	int ret = 0;

	if(char_write) {
		printf("handle: %d,tran_id: %d\n",char_write->attr_handle,
			char_write->trans_id);
		printf("Value:");
		bt_manager_hex_dump(" ", 20,char_write->value,char_write->value_len);
	}

/*
	gatt_notify_rsp_t data;
	data.attr_handle = char_write->attr_handle;
	data.value = data_value;
	data.value_len = 1;

	data_value[0] ++ ;
	printf("send data value:%d\n",data_value[0]);
	bt_manager_gatt_send_notification(&data);
*/

	if (char_write->need_rsp) {
		gatt_write_rsp_t data;
		data.trans_id = char_write->trans_id;
		data.attr_handle = char_write->attr_handle;
		data.state = BT_GATT_SUCCESS;
		ret = bt_manager_gatt_send_write_response(&data);
		if (ret)
			printf("send write response failed!\n");
	}
}

void bt_test_gatt_char_notify_request_cb(gatt_notify_req_t *char_notify)
{
	printf("enter\n");

}

void bt_test_gatt_desc_read_requset_cb(gatt_desc_read_req_t *desc_read)
{
	printf("enter\n");
	char value[1];
	static unsigned char count = 0;

	printf("trans_id:%d,attr_handle:%d,offset:%d\n",desc_read->trans_id,
		desc_read->attr_handle,desc_read->offset);

	if(desc_read) {
		gatt_send_read_rsp_t data;
		data.trans_id = desc_read->trans_id;
		data.svc_handle = desc_read->attr_handle;
		data.status = 0x0b;
		data.auth_req = 0x00;
		value[0]= count;
		data.value = value;
		data.value_len = 1;
		bt_manager_gatt_send_read_response(&data);
		count ++;
	}
}

void bt_test_gatt_desc_write_request_cb(gatt_desc_write_req_t *desc_write)
{
	printf("enter\n");
	if (desc_write->need_rsp) {
		gatt_write_rsp_t data;
		data.trans_id = desc_write->trans_id;
		data.attr_handle = desc_write->attr_handle;
		data.state = BT_GATT_SUCCESS;
		bt_manager_gatt_send_write_response(&data);
	}
}

static void set_adv_param(void)
{
	btmg_le_advertising_parameters_t adv_params;

	adv_params.min_interval = 0x0020;
	adv_params.max_interval = 0x01E0; /*range from 0x0020 to 0x4000*/
	adv_params.own_addr_type = BTMG_LE_PUBLIC_ADDRESS;
	adv_params.adv_type =  BTMG_LE_ADV_IND; /*ADV_IND*/
	adv_params.chan_map = BTMG_LE_ADV_CHANNEL_ALL; /*0x07, *bit0:channel 37, bit1: channel 38, bit2: channel39*/
	adv_params.filter = BTMG_LE_PROCESS_ALL_REQ;
	bt_manager_set_adv_param(&adv_params);

}


static void le_set_adv_data()
{
	int dd;
	uint8_t manuf_len;
	uint16_t ogf, ocf;
	int index;
	char advdata[32] = { 0 };
	char uuid_buf[5] = {0};
	const char *ble_name="aw-ble-test-007";
//#define AD_LEN_MANUF	16
#define AD_LEN_FLAG		3
//#define AD_LEN_APPEARANCE	4
//#define AD_LEN_LIST_OF_UUID	4
#define AD_LEN_LOCAL_NAME	10

	index = 1;
//	manuf_len = 1 + 14;
//	advdata[index] = manuf_len;	/* manuf len */
//	advdata[index + 1] = 0xff;		/* ad type */
//	memcpy(advdata + 3, manufacturer_data, MANUFACTURER_DATA_LEN + WIFI_LEN);

//	index += AD_LEN_MANUF;
	advdata[index] = 0x02;			/* flag len */
	advdata[index + 1] = 0x01;		/* type for flag */
	advdata[index + 2] = 0x1A;  //0x05
	index += AD_LEN_FLAG;

//	advdata[index] = 3;				/* appearance len */
//	advdata[index + 1] = 0x19;		/* type for appearance */
//	advdata[index + 2] = 0x01;
//	advdata[index + 3] = 0x00;

//	index += AD_LEN_APPEARANCE;
//	advdata[index] = 0x03;			/* uuid len */
//	advdata[index + 1] = 0x03;		/* type for complete list of 16-bit uuid */
//	advdata[index + 2] = 0x20;
//	advdata[index + 3] = 0x9e;


//	advdata[index] = 2+1;			/* uuid len */
//	advdata[index + 1] = 0x03;		/* type for  complete list of 16-bit uuid*/
//	sscanf(uuid_buf+2, "%2x", &advdata[index + 2]);		/* type for  complete list of 16-bit uuid*/
//	uuid_buf[2] = '\0';
//	sscanf(uuid, "%2x", &advdata[index + 3]);		/* type for  complete list of 16-bit uuid*/
//	index += AD_LEN_LIST_OF_UUID;

//	advdata[index] = 0x0A;			/* uuid len */
	advdata[index] = strlen(ble_name)+1;			/* name len */
	advdata[index + 1] = 0x09;		/* type for local name */
	index+=2;
	int i, name_len;
	name_len = strlen(ble_name);
	for(i=0;i<=name_len;i++) {
		advdata[index + i] = ble_name[i];
	}

	// total length
//	advdata[0] = AD_LEN_MANUF + AD_LEN_FLAG + AD_LEN_APPEARANCE	+ AD_LEN_LIST_OF_UUID + AD_LEN_LOCAL_NAME;
	//advdata[0] = AD_LEN_FLAG + AD_LEN_LIST_OF_UUID + (name_len + 2);
	advdata[0] = AD_LEN_FLAG + (name_len + 2);

	btmg_adv_rsp_data_t adv;
	adv.data_len = advdata[0];
	memcpy(adv.data,&advdata[1],31);

	bt_manager_le_set_adv_data(&adv);

	bt_manager_le_enable_adv(true);
}


static int bt_gatt_init(void)
{
	gatt_add_svc_t svc;
	gatt_add_char_t chr1;
	gatt_add_char_t chr2;
	gatt_add_char_t chr3;
	gatt_add_desc_t desc;
	gatt_star_svc_t start_svc;

	if(bt_manager_preinit(&bt_callback) != 0) {
		printf("bt preinit failed!\n");
		return -1;
	}


	bt_callback->btmg_gatt_server_cb.gatt_add_svc_cb = bt_test_gatt_add_service_cb;
	bt_callback->btmg_gatt_server_cb.gatt_add_char_cb = bt_test_gatt_add_char_cb;
	bt_callback->btmg_gatt_server_cb.gatt_add_desc_cb = bt_test_gatt_add_desc_cb;
	bt_callback->btmg_gatt_server_cb.gatt_connection_event_cb = bt_test_gatt_connection_cb;
	bt_callback->btmg_gatt_server_cb.gatt_char_read_req_cb = bt_test_gatt_char_read_request_cb;
	bt_callback->btmg_gatt_server_cb.gatt_char_write_req_cb = bt_test_gatt_char_write_request_cb;
	bt_callback->btmg_gatt_server_cb.gatt_char_notify_req_cb = bt_test_gatt_char_notify_request_cb;
	bt_callback->btmg_gatt_server_cb.gatt_desc_read_req_cb = bt_test_gatt_desc_read_requset_cb;
	bt_callback->btmg_gatt_server_cb.gatt_desc_write_req_cb = bt_test_gatt_desc_write_request_cb;
	bt_callback->btmg_gatt_server_cb.gatt_send_indication_cb = bt_test_gatt_send_indication_cb;

	if(bt_manager_init(bt_callback) != 0) {
		printf("bt manager init failed.\n");
		goto Failed;
	}
	bt_manager_gatt_server_init();

	printf("add service,uuid:%s\n",TEST_SERVICE_UUID);
	svc.number = 8;
	svc.uuid = TEST_SERVICE_UUID;
	svc.primary = true;
	bt_manager_gatt_create_service(&svc);

	chr1.permissions = BT_GATT_PERM_READ | BT_GATT_PERM_WRITE;
	chr1.properties = BT_GATT_CHAR_PROPERTY_READ | BT_GATT_CHAR_PROPERTY_WRITE;
	chr1.svc_handle = service_handle;
	chr1.uuid = TEST_CHAR_UUID1;
	bt_manager_gatt_add_characteristic(&chr1);

	chr2.permissions = BT_GATT_PERM_READ | BT_GATT_PERM_WRITE;
	chr2.properties = BT_GATT_CHAR_PROPERTY_READ | BT_GATT_CHAR_PROPERTY_WRITE |
		BT_GATT_CHAR_PROPERTY_NOTIFY;
	chr2.svc_handle = service_handle;
	chr2.uuid = TEST_CHAR_UUID2;
	bt_manager_gatt_add_characteristic(&chr2);

	chr3.permissions = BT_GATT_PERM_READ;
	chr3.properties = BT_GATT_CHAR_PROPERTY_READ;
	chr3.svc_handle = service_handle;
	chr3.uuid = DEV_NAME_CHAR_UUID2;
	bt_manager_gatt_add_characteristic(&chr3);

	desc.permissions = BT_GATT_PERM_READ | BT_GATT_PERM_WRITE;
	desc.uuid = TEST_DESC_UUID;
	desc.svc_handle = service_handle;
	bt_manager_gatt_add_descriptor(&desc);

	bt_manager_gatt_start_service(&start_svc);

	set_adv_param();
	le_set_adv_data();
	return 0;
Failed:
	bt_manager_deinit(bt_callback);
	return -1;
}
static int bt_gatt_deinit()
{
	bt_manager_gatt_server_deinit();
	bt_manager_deinit(bt_callback);
	return 0;
}

void main_loop_stop(int sig)
{
	struct sigaction sigact;

	sigact.sa_handler = SIG_DFL;

	sigaction(sig, &sigact, NULL);
	bt_gatt_deinit();

	exit(0);
}


int main(int argc, char **argv)
{
	bt_gatt_init();

	struct sigaction sigact ;

	sigact.sa_handler = main_loop_stop;

	sigaction(SIGTERM, &sigact, NULL);
	sigaction(SIGINT, &sigact, NULL);
	while(1) {
		select(0,NULL,NULL,NULL,NULL);
	}

}

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
#include "bt_log.h"
#include "bt_dev_list.h"

static btmg_callback_t *bt_callback = NULL;

static dev_list_t *discovered_devices = NULL;

void *gattc_cn_handle = NULL;

#define CMD_ARGS_MAX	20
int __main_terminated;

static void bt_test_gattc_discovery_status_cb(btmg_discovery_state_t status)
{
	if (status == BTMG_DISC_STARTED) {
		BTMG_INFO("bt start scanning.");
	} else if (status == BTMG_DISC_STOPPED_AUTO) {
		BTMG_INFO("scanning stop automatically");
	} else if (status == BTMG_DISC_START_FAILED) {
		BTMG_INFO("start scan failed.");
	} else if (status == BTMG_DISC_STOPPED_BY_USER) {
		BTMG_INFO("stop scan by user.");
	}

}
static void bt_test_gattc_dev_add_cb(btmg_bt_device_t *device)
{
	dev_node_t *dev_node = NULL;

	BTMG_INFO("address:%s,name:%s,class:%d,address type:%s", device->remote_address,
			device->remote_name, device->r_class,device->address_type);

	dev_node = btmg_dev_list_find_device(discovered_devices, device->remote_address);
	if (dev_node != NULL) {
		return;
	}
	btmg_dev_list_add_device(discovered_devices, device->remote_name, device->remote_address);
}

static void bt_test_gattc_dev_remove_cb(btmg_bt_device_t *device)
{
	dev_node_t *dev_node = NULL;

	BTMG_INFO("address:%s,name:%s,class:%d,address type:%s", device->remote_address,
			device->remote_name, device->r_class,device->address_type);

	dev_node = btmg_dev_list_find_device(discovered_devices, device->remote_address);
	if (dev_node != NULL) {
		return;
	}
	btmg_dev_list_remove_device(discovered_devices, device->remote_address);
}

static void bt_test_gattc_scan_report_cb(btmg_le_scan_report_t *data)
{
	bt_manager_hex_dump("peer_addr",6,data->peer_addr,6);
	BTMG_INFO("addr type: %d , adv type:%d, rssi:%d",data->addr_type,
			data->adv_type,data->rssi);
	bt_manager_hex_dump("adv data",20,data->report.data,data->report.data_len);
}

static void bt_test_gattc_update_rssi_cb(const char *address,const char *name, int rssi)
{
	dev_node_t *dev_node = NULL;

	dev_node = btmg_dev_list_find_device(discovered_devices, address);
	if (dev_node != NULL) {
		BTMG_INFO("address:%s,name:%s,rssi:%d", address, name, rssi);
		return;
	}

}


void bt_test_gattc_notify_cb(gattc_notify_cb_para_t *data)
{
	int i;

	printf("\n\tHandle Value Not/Ind: 0x%04x - ", data->value_handle);

	if (data->length == 0) {
		printf("(0 bytes)\n");
		return;
	}

	printf("(%u bytes): ", data->length);

	for (i = 0; i < data->length; i++)
		printf("%02x ", data->value[i]);

	printf("\n");

}

void bt_test_gattc_write_long_cb(gattc_write_long_cb_para_t *data)
{
	if (data->success) {
		printf("Write successful\n");
	} else if(data->reliable_error) {
		printf("Reliable write not verified\n");
	} else {
		printf("\nWrite failed: %s (0x%02x)\n",
				bt_manager_gatt_client_ecode_to_string(data->att_ecode),
					data->att_ecode);
	}
}

void bt_test_gattc_write_cb(gattc_write_cb_para_t *data)
{
	if (data->success) {
		printf("\nWrite successful\n");
	} else {
		printf("\nWrite failed: %s (0x%02x)\n",
				bt_manager_gatt_client_ecode_to_string(data->att_ecode),
					data->att_ecode);
	}
}

void bt_test_gattc_read_cb(gattc_read_cb_para_t *data)
{
	int i;

	if (!data->success) {
		printf("\nRead request failed: %s (0x%02x)\n",
				bt_manager_gatt_client_ecode_to_string(data->att_ecode),
					data->att_ecode);
		return;
	}

	printf("\nRead value");

	if (data->length == 0) {
		printf(": 0 bytes\n");
		return;
	}

	printf(" (%u bytes): ", data->length);

	for (i = 0; i < data->length; i++)
		printf("%02x ", data->value[i]);

	printf("\n");

}

void bt_test_gattc_conn_cb(gattc_conn_cb_para_t *data)
{
	if (!data->success) {
		printf("GATT discovery procedures failed - error code: 0x%02x\n",
								data->att_ecode);
		return;
	}
	gattc_cn_handle = data->cn_handle;
	printf("GATT discovery procedures complete\n");

}

void bt_test_gattc_disconn_cb(gattc_disconn_cb_para_t *data)
{
	printf("Device disconnected:%s\n",strerror(data->err));
}

void bt_test_gattc_service_changed_cb(gattc_service_changed_cb_para_t *data)
{
	printf("\nService Changed handled - start: 0x%04x end: 0x%04x\n",
			data->start_handle, data->end_handle);
}

static void print_uuid(const btmg_uuid_t *uuid)
{
	char uuid_str[37];
	btmg_uuid_t uuid128;

	bt_manager_uuid_to_uuid128(uuid, &uuid128);
	bt_manager_uuid_to_string(&uuid128, uuid_str, sizeof(uuid_str));

	printf("%s\n", uuid_str);
}


void bt_test_gattc_dis_service_cb(gattc_dis_service_cb_para_t *data)
{

	printf("service - start: 0x%04x, "
				"end: 0x%04x, type: %s, uuid: ",
				data->start_handle, data->end_handle,
				data->primary ? "primary" : "secondary");
	print_uuid(&data->uuid);
	bt_manager_gatt_client_discover_service_all_char(data->attr);
}

void bt_test_gattc_dis_char_cb(gattc_dis_char_cb_para_t *data)
{

	printf("\t  charac- start: 0x%04x, value: 0x%04x, "
				"props: 0x%02x, ext_props: 0x%04x, uuid: ",
				data->start_handle, data->value_handle, data->properties,
				data->ext_prop);
	print_uuid(&data->uuid);
	bt_manager_gatt_client_discover_char_all_descriptor(data->attr);
}

void bt_test_gattc_dis_desc_cb(gattc_dis_desc_cb_para_t *data)
{

	printf("\t\t  descr - handle: 0x%04x, uuid: ",data->handle);
	print_uuid(&data->uuid);
}

typedef struct {
	const char *cmd;
	const char *arg;
	void (*func) (int argc, char *args[]);
	const char *desc;
} cmd_tbl_t;


static int _bt_gatt_client_init(void)
{
	if(bt_manager_preinit(&bt_callback) != 0) {
		printf("bt preinit failed!\n");
		return -1;
	}

	bt_callback->btmg_gatt_client_cb.gattc_conn_cb = bt_test_gattc_conn_cb;
	bt_callback->btmg_gatt_client_cb.gattc_disconn_cb = bt_test_gattc_disconn_cb;
	bt_callback->btmg_gatt_client_cb.gattc_read_cb = bt_test_gattc_read_cb;
	bt_callback->btmg_gatt_client_cb.gattc_write_cb = bt_test_gattc_write_cb;
	bt_callback->btmg_gatt_client_cb.gattc_write_long_cb = bt_test_gattc_write_long_cb;
	bt_callback->btmg_gatt_client_cb.gattc_service_changed_cb = bt_test_gattc_service_changed_cb;
	bt_callback->btmg_gatt_client_cb.gattc_notify_cb = bt_test_gattc_notify_cb;
	bt_callback->btmg_gatt_client_cb.gattc_dis_service_cb = bt_test_gattc_dis_service_cb;
	bt_callback->btmg_gatt_client_cb.gattc_dis_char_cb = bt_test_gattc_dis_char_cb;
	bt_callback->btmg_gatt_client_cb.gattc_dis_desc_cb = bt_test_gattc_dis_desc_cb;

	bt_callback->btmg_gap_cb.gap_disc_status_cb = bt_test_gattc_discovery_status_cb;
	bt_callback->btmg_gap_cb.gap_device_add_cb = bt_test_gattc_dev_add_cb;
	bt_callback->btmg_gap_cb.gap_device_remove_cb = bt_test_gattc_dev_remove_cb;
	bt_callback->btmg_gap_cb.gap_le_scan_report_cb = bt_test_gattc_scan_report_cb;
	bt_callback->btmg_gap_cb.gap_update_rssi_cb = bt_test_gattc_update_rssi_cb;

	bt_manager_enable_profile(BTMG_GATT_CLIENT_ENABLE);
	if(bt_manager_init(bt_callback) != 0) {
		printf("bt manager init failed.\n");
		goto Failed;
	}

	discovered_devices = btmg_dev_list_new();

	return bt_manager_enable(true);

Failed:
	bt_manager_deinit(bt_callback);
	return -1;

}

static int _bt_deinit()
{
	bt_manager_enable(false);
	btmg_dev_list_free(discovered_devices);
	bt_manager_deinit(bt_callback);
}


static void cmd_gatt_client_connect(int argc,char *args[])
{
	if(argc != 1) {
		goto end;
	}

	if(strlen(args[0]) < 17) {
		goto end;
	}

	bt_manager_gatt_client_connect(0,args[0],BTMG_BDADDR_LE_PUBLIC,
			517,BTMG_SECURITY_LOW);
	return;
end:
	BTMG_ERROR("Unexpected argc: %d, see help", argc);
}

static void cmd_gatt_client_disconnect(int argc,char *args[])
{
	if(argc != 1) {
		goto end;
	}

	if(strlen(args[0]) < 17) {
		goto end;
	}

	bt_manager_profile_disconnect(args[0],BTMG_GATT);
	return;
end:
	BTMG_ERROR("Unexpected argc: %d, see help", argc);
}


static void cmd_gatt_client_write_request(int argc,char *args[])
{
	static uint8_t value[1] = {0};
	uint16_t handle;
	char *endptr = NULL;
	if(argc != 1) {
		goto end;
	}

	handle = strtol(args[0], &endptr, 0);
	bt_manager_gatt_client_write_request(gattc_cn_handle,handle,value,1);
	value[0]++;

	return;
end:
	BTMG_ERROR("Unexpected argc: %d, see help", argc);
}


static void cmd_gatt_client_write_command(int argc,char *args[])
{
	uint16_t handle;
	char *endptr = NULL;
	static uint8_t value[1] = {0};
	if(argc != 1) {
		goto end;
	}

	handle = strtol(args[0], &endptr, 0);

	bt_manager_gatt_client_write_command(gattc_cn_handle,handle,
			false,value,1);
	value[0]++;
	return;
end:
	BTMG_ERROR("Unexpected argc: %d, see help", argc);
}

static void cmd_gatt_client_read_request(int argc,char *args[])
{
	uint16_t handle;
	char *endptr = NULL;
	if(argc != 1) {
		goto end;
	}

	handle = strtol(args[0], &endptr, 0);

	bt_manager_gatt_client_read_request(gattc_cn_handle,handle);
	return;
end:
	BTMG_ERROR("Unexpected argc: %d, see help", argc);
}

static void cmd_gatt_client_dis_all_pri_svcs(int argc,char *args[])
{

	bt_manager_gatt_client_discover_all_primary_services(gattc_cn_handle,
			0x0001,0xffff);
	return;
end:
	BTMG_ERROR("Unexpected argc: %d, see help", argc);
}

static void cmd_gatt_client_dis_pri_svcs_by_uuid(int argc,char *args[])
{
	if(argc != 1) {
		goto end;
	}

	if(strlen(args[0]) < 6) {
		goto end;
	}
	bt_manager_gatt_client_discover_primary_services_by_uuid(gattc_cn_handle,
			args[0],0x0001,0xffff);
	return;
end:
	BTMG_ERROR("Unexpected argc: %d, see help", argc);

}


static void cmd_ble_discovery(int argc,char *args[])
{
	int value = 0;
#if 0
	int ret = -1;
	btmg_scan_filter_t scan_filter = {0};

	scan_filter.type = BTMG_SCAN_LE;
	scan_filter.rssi = -90;

	if(argc != 1) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}

	value = atoi(args[0]);
	if(value) {
		bt_manager_discovery_filter(&scan_filter);
		bt_manager_start_discovery();
	}
	else
		bt_manager_cancel_discovery();
#else
	static void *scan_handle = NULL;
	if(argc != 1) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}

	value = atoi(args[0]);
	if(value) {
		btmg_le_scan_param_t scan_param;
		btmg_le_scan_type_t scan_type;
		scan_param.scan_interval = 0x0010;
		scan_param.scan_window = 0x0010;
		scan_type = LE_SCAN_TYPE_PASSIVE;
		bt_manager_le_scan_start(0,scan_type,&scan_param,&scan_handle);
	}else {
		bt_manager_le_scan_stop(0,scan_handle);
	}
#endif
}

static void cmd_gatt_client_get_mtu(int argc,char *args[])
{
	int mtu;
	mtu = bt_manager_gatt_client_get_mtu(gattc_cn_handle);
	if(mtu > 0) {
		printf("[MTU]:%d\b",mtu);
	}else {
		printf("get mtu failed.\n");
	}
	return;
}


cmd_tbl_t bt_gattc_cmd_table[] = {
	{"connect",           NULL , cmd_gatt_client_connect,         "connect [mac]:gatt client method to connect"},
	{"disconnect",        NULL , cmd_gatt_client_disconnect,      "disconnect [mac]:gatt client method to connect"},
	{"write-request",     NULL , cmd_gatt_client_write_request,   "write-request [handle]:gatt write request"},
	{"write-command",     NULL , cmd_gatt_client_write_command,   "write-command [handle]:gatt write command without response"},
	{"read-request",      NULL , cmd_gatt_client_read_request,    "read-request  [handle]:gatt read request"},
	{"ble_scan",          NULL , cmd_ble_discovery,               "ble_scan [0/1]: scan for devices"},
	{"dis_pri_svcs",      NULL , cmd_gatt_client_dis_all_pri_svcs,"services:discovery all primary services"},
	{"dis_svcs_uuid",     NULL , cmd_gatt_client_dis_pri_svcs_by_uuid,"dis_svcs_uuid [uuid]: discovery services by uuid"},
	{"get_mtu",           NULL , cmd_gatt_client_get_mtu,         "get_mtu:get MTU"},
//	{"read-long-request", NULL , cmd_bt_device_connect,        "connect [mac]:generic method to connect"},
//	{"set-security",      NULL , cmd_bt_device_connect,        "connect [mac]:generic method to connect"},
//	{"get-security",      NULL , cmd_bt_device_connect,        "connect [mac]:generic method to connect"},
//	{"register-notify",   NULL , cmd_bt_device_connect,        "connect [mac]:generic method to connect"},
//	{"upregister-notify", NULL , cmd_bt_device_connect,        "connect [mac]:generic method to connect"},
	{ NULL, NULL, NULL, NULL},
};

#define COLOR_OFF	"\x1B[0m"
#define COLOR_BLUE	"\x1B[0;34m"
static void print_prompt(void)
{
	printf(COLOR_BLUE "[bt]" COLOR_OFF "#");
	fflush(stdout);
}


static void  process_cmdline(char *input_str, uint32_t len)
{
	char *cmd, *arg, *parse_arg;
	char *args[CMD_ARGS_MAX];
	int argc, i;

	/*
	 * If user enter CTL + d, program will read an EOF and len is zero.
	 */
	if (!len) {
		printf("empty command\n");
		goto done;
	}

	if (!strlen(input_str))
		goto done;

	if (input_str[0] == '\n' || input_str[len - 1] == '\r')
		input_str[len - 1] = '\0';

	cmd = strtok_r(input_str, " ", &arg);
	if (!cmd)
		goto done;

	if (arg) {
		int len = strlen(arg);

		if (len > 0 && arg[len - 1] == ' ')
			arg[len - 1] = '\0';

		for (argc = 0; argc < CMD_ARGS_MAX; argc++) {
			parse_arg = strtok_r(NULL, " ", &arg);
			if (!parse_arg)
				break;

			args[argc] = parse_arg;
		}
	}

	for (i = 0; bt_gattc_cmd_table[i].cmd; i++) {
		if (strcmp(cmd, bt_gattc_cmd_table[i].cmd))
			continue;

		if (bt_gattc_cmd_table[i].func) {
			bt_gattc_cmd_table[i].func(argc, args);
			goto done;
		}
	}
	if(strcmp(cmd,"quit") == 0) {
		_bt_deinit();
		__main_terminated = 1;
		return ;
	}
	if (strcmp(cmd, "help")) {
		printf("Invalid command\n");
		goto done;
	}

	printf("Available commands:\n");
	for (i = 0; bt_gattc_cmd_table[i].cmd; i++) {
		if (bt_gattc_cmd_table[i].desc)
			printf("\t%-20s\t\t%-15s\n", bt_gattc_cmd_table[i].cmd,
							bt_gattc_cmd_table[i].desc);
	}

done:
	print_prompt();
	return;
}


static void stdin_read_handler(int fd)
{
	ssize_t read;
	size_t len = 0;
	char *line = NULL;

	read = getline(&line, &len, stdin);
	if (read < 0)
		return;

	if (read <= 1) {
		print_prompt();
		return;
	}

	line[read - 1] = '\0';

	process_cmdline(line, strlen(line) + 1);
}

static int sigfd_setup(void)
{
	sigset_t mask;
	int fd;

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGTERM);

	if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
		printf("Failed to set signal mask");
		return -1;
	}

	fd = signalfd(-1, &mask, 0);
	if (fd < 0) {
		printf("Failed to create signal descriptor");
		return -1;
	}

	return fd;
}


int main(int argc, char **argv)
{

	int sigfd;
	struct pollfd pfd[2];

    char fifo_r[1024] ={'\0'};
    uint32_t fifo_r_len = 0;

	_bt_gatt_client_init();

	sigfd = sigfd_setup();
	if (sigfd < 0)
		return -1;

	pfd[0].fd = sigfd;
	pfd[0].events = POLLIN | POLLHUP | POLLERR;
	pfd[1].fd = fileno(stdin);


    if(pfd[1].fd < 0)
    {
        printf("pfd[1].fd  =%d,%s\n",pfd[1].fd,strerror(errno));
		return -1;
    }

	pfd[1].events = POLLIN | POLLHUP | POLLERR;

	print_prompt();

	while (!__main_terminated) {
		pfd[0].revents = 0;
		pfd[1].revents = 0;

		if (poll(pfd, 2, -1) == -1) {
			if (errno = EINTR)
				continue;
			printf("Poll error: %s", strerror(errno));
			return -1;
		}

		if (pfd[0].revents & (POLLHUP | POLLERR)) {
			printf("fd :%d POLLHUP or POLLERR\n",pfd[0].fd);
			return -1;
		}

		if (pfd[1].revents & (POLLHUP | POLLERR)) {
			printf("fd :%d POLLHUP or POLLERR\n",pfd[1].fd);
			return -1;
		}

		if (pfd[0].revents & POLLIN) {
			struct signalfd_siginfo si;
			ssize_t ret;


			ret = read(pfd[0].fd, &si, sizeof(si));
			if (ret != sizeof(si))
				return -1;

			switch (si.ssi_signo) {
				case SIGINT:
				case SIGTERM:
					__main_terminated = 1;
					break;
			}

		}
		if (pfd[1].revents & POLLIN)
			stdin_read_handler(pfd[1].fd);
	}

	return 0;


}

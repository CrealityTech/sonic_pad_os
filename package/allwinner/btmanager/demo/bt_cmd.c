#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/signalfd.h>
#include <pthread.h>
#include "bt_dev_list.h"
#include "bt_cmd.h"
#include "bt_log.h"
int32_t __main_terminated;

dev_list_t *bonded_devices = NULL;
dev_list_t *discovered_devices = NULL;

static void _print_discovered_devices()
{
	dev_node_t *dev_node = NULL;

	dev_node = discovered_devices->head;
	while (dev_node != NULL) {
		BTMG_INFO("%s %s", dev_node->dev_addr, dev_node->dev_name);
		dev_node = dev_node->next;
	}
}

static void _pair_devices_list()
{
	bt_paried_device *devices = NULL;
	int count = -1;

	bt_manager_get_paired_devices(&devices,&count);
	bt_paried_device *iter = devices;
	if(iter) {
		while(iter) {
			printf("\n \
			addres: %s,\n \
			name:%s \n \
			rssi:%d \n \
			bonded:%d \n \
			connected:%d \n",iter->remote_address,
			iter->remote_name,iter->rssi,
			iter->is_bonded,iter->is_connected);

			iter = iter->next;
		}
		bt_manager_free_paired_devices(devices);
	} else {
		BTMG_ERROR("bonded devices is empty\n");
	}
}
static void cmd_bt_set_log_level(int argc,char *args[])
{
	int value = 0;
	if(argc != 1) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	value = atoi(args[0]);
	bt_manager_set_loglevel((btmg_log_level_t)value);
}

static void cmd_bt_enable(int argc, char *args[])
{
	int value = 0;

	if(argc != 1) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	value = atoi(args[0]);
	if(value)
		bt_manager_enable(true);
	else
		bt_manager_enable(false);
}

static void cmd_bt_discovery(int argc,char *args[])
{
	int value = 0;
	int ret = -1;

	if(argc != 1) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}

	value = atoi(args[0]);
	if(value)
		bt_manager_start_discovery();
	else
		bt_manager_cancel_discovery();

}

static void cmd_bt_pair(int argc,char *args[])
{
	if(argc != 1 && strlen(args[0]) != 17) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}

	if(bt_manager_pair(args[0]))
		BTMG_ERROR("Pair failed:%s",args[0]);
}

static void cmd_bt_unpair(int argc,char *args[])
{
	if(argc != 1 && strlen(args[0]) != 17) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}

	if(bt_manager_unpair(args[0]))
		BTMG_ERROR("Pair failed:%s",args[0]);
}

static void cmd_bt_inquiry_list(int argc,char *args[])
{
	if (bt_manager_is_discovering()) {
		BTMG_ERROR("BT is still in discovering, please try later\n");
	} else {
		_print_discovered_devices();
	}
}

static void cmd_bt_pair_devices_list(int argc,char *args[])
{
	_pair_devices_list();
}

static void cmd_bt_get_state(int argc,char *args[])
{
	btmg_state_t bt_state;
	bt_state = bt_manager_get_state();
	if (bt_state == BTMG_STATE_OFF)
		BTMG_INFO("The BT state is OFF");
	else if (bt_state == BTMG_STATE_ON)
		BTMG_INFO("The BT state is ON");
	else if (bt_state == BTMG_STATE_TURNING_ON)
		BTMG_INFO("The BT state is turning ON");
	else if (bt_state == BTMG_STATE_TURNING_OFF)
		BTMG_INFO("The BT state is turning OFF");
}

static void cmd_bt_get_name(int argc,char *args[])
{
	char bt_name[128] = {0};
	bt_manager_get_name(bt_name,sizeof(bt_name));
	BTMG_INFO("BT name:%s",bt_name);
}

static void cmd_bt_set_name(int argc,char *args[])
{
	int ret = -1;

	if(argc != 1) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	if(bt_manager_set_name(args[0])) {
		BTMG_ERROR("Set bt name failed.");
	}

}

static void cmd_bt_get_remote_name(int argc,char *args[])
{
	char remote_name[128] = {0};
	bt_manager_get_remote_name(remote_name);
	BTMG_INFO("get remote device name:%s", remote_name);
}

static void cmd_bt_set_vol(int argc,char *args[])
{
	int vol_value = 0;

	vol_value = atoi(args[0]);
	if (vol_value > 100)
		vol_value = 100;

	if (vol_value < 0)
		vol_value = 0;

	bt_manager_vol_changed_noti(vol_value);
	BTMG_INFO("set vol:%d", vol_value);
}

static void cmd_bt_get_vol(int argc,char *args[])
{
	int vol_value = 0;

	vol_value = bt_manager_get_vol();
	BTMG_INFO("get vol:%d", vol_value);
}

static void cmd_bt_get_address(int argc,char *args[])
{
	char bt[18] = {0};
	int ret = -1;

	ret = bt_manager_get_address(bt, sizeof(bt));
	if(ret) {
		BTMG_ERROR("Got bt adress failed.");
	}else {
		BTMG_INFO("BT address:%s",bt);
	}
}

static void cmd_bt_avrcp(int argc,char *args[])
{
	int avrcp_type = -1;
	char bt[18] = {0};
	int i;
	int ret;
	const char *cmd_type_str[] = {
		"play",
		"pause",
		"stop",
		"fastforward",
		"rewind",
		"forward",
		"backward",
	};
	if(argc != 1) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	for(i=0;i< sizeof(cmd_type_str)/sizeof(char *);i++) {
		if(strcmp(args[0],cmd_type_str[i]) == 0) {
			avrcp_type = i;
			break;
		}
	}

	if(avrcp_type == -1) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}

	ret = bt_manager_get_address(bt, sizeof(bt));
	if(ret) {
		BTMG_ERROR("Got bt adress failed.");
		return;
	}

	bt_manager_avrcp_command(bt,(btmg_avrcp_command_t)avrcp_type);
}

static void cmd_bt_profile_conncet(int argc,char *args[])
{
	int value;
	if(argc != 2 && strlen(args[0]) != 17) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	value = atoi(args[1]);
	if(value > BTMG_HSP_AG || value < 0) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	bt_manager_profile_connect(args[0],(btmg_profile_t)value);
}

static void cmd_bt_profile_disconncet(int argc,char *args[])
{
	int value;
	if(argc != 2 && strlen(args[0]) != 17) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	value = atoi(args[1]);
	if(value > BTMG_PROFILE_MAX || value < 0) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	bt_manager_profile_disconnect(args[0],(btmg_profile_t)value);
}

static void cmd_bt_device_connect(int argc,char *args[])
{
	if(argc != 1 && strlen(args[0]) != 17) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	bt_manager_connect(args[0]);
}

static void cmd_bt_device_disconnect(int argc,char *args[])
{
	if(argc != 1 && strlen(args[0]) != 17) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	bt_manager_disconnect(args[0]);
}

static void cmd_bt_remove_device(int argc,char *args[])
{
	if(argc != 1 && strlen(args[0]) != 17) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	bt_manager_remove_device(args[0]);
}

static bool a2dp_src_loop = false;
static bool a2dp_src_enable = false;

static pthread_t bt_test_a2dp_source_thread;

typedef struct {
        unsigned int    riff_type;
        unsigned int    riff_size;
        unsigned int    wave_type;
        unsigned int    format_type;
        unsigned int    format_size;
        unsigned short  compression_code;
        unsigned short  num_channels;
        unsigned int    sample_rate;
        unsigned int    bytes_per_second;
        unsigned short  block_align;
        unsigned short  bits_per_sample;
        unsigned int    data_type;
        unsigned int    data_size;
} wav_header_t;

#define A2DP_SRC_BUFF_SIZE (1024*4)
static void *_a2dp_source_thread_func(void *arg)
{
	int ret = -1;
	char buffer[A2DP_SRC_BUFF_SIZE] = {0};
	int fd = -1;
	unsigned int c, written = 0, count;
	wav_header_t wav_header;
	a2dp_src_enable = true;
	int len;
	fd = open("/tmp/44100-stereo-s16_le-10s.wav",O_RDONLY);
	if (fd < 0) {
		BTMG_ERROR("Cannot open input file:%s",strerror(errno));
		pthread_exit((void *)-1);
	}
	ret = read(fd,&wav_header,sizeof(wav_header_t));
	if(ret != sizeof(wav_header_t)) {
		BTMG_ERROR("read wav file header failed.");
		close(fd);
		pthread_exit((void *)-1);
	}
	//pthread_cleanup_push(_a2dp_source_thread_exit,NULL);

	bt_manager_a2dp_src_init(wav_header.num_channels,
			wav_header.sample_rate);

	bt_manager_a2dp_src_stream_start(A2DP_SRC_BUFF_SIZE);

	count = wav_header.data_size;

    BTMG_INFO("start a2dp src loop,data size:%d,ch:%d,sample:%d",
			wav_header.data_size,wav_header.num_channels,wav_header.sample_rate);
	a2dp_src_loop = true;
	ret = -1;
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	while (a2dp_src_loop) {
		if(ret != 0) {
			c = count - written;
			if(c > A2DP_SRC_BUFF_SIZE) {
				c= A2DP_SRC_BUFF_SIZE;
			}
			len = read(fd,buffer,c);
			if(len == 0) {
				lseek(fd,0,SEEK_SET);
				written = 0;
				continue;
			}
			if(len < 0) {
				BTMG_ERROR("read file error,ret:%d,c=%d",len,c);
				break;
			}
		}else {
			usleep(1000*20);
		}
		if(len > 0) {
			ret = bt_manager_a2dp_src_stream_send(buffer,len,true);
			written += ret;
		}
	}
	if(fd != -1) {
		close(fd);
	}

	BTMG_INFO("a2dp source thread quit.");
	bt_manager_a2dp_src_stream_stop(false);
	bt_manager_a2dp_src_deinit();
	a2dp_src_enable = false;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	return NULL;
}

static void cmd_bt_a2dp_src_start(int argc,char *args[])
{
	int err = 0;
	if(a2dp_src_loop == false) {
		err = pthread_create(&bt_test_a2dp_source_thread,
				NULL, _a2dp_source_thread_func, NULL);

	}
}

static void cmd_bt_a2dp_src_stop(int argc,char *args[])
{
	if(a2dp_src_enable == false) {
		BTMG_INFO("a2dp source doesn't run.");
		return ;
	}
	a2dp_src_loop = false;
	pthread_join(bt_test_a2dp_source_thread, NULL);

}

static void cmd_bt_hfp_answer_call(int argc,char *args[])
{
	bt_manager_hfp_client_send_at_ata();
}

static void cmd_bt_hfp_hangup(int argc,char *args[])
{
	bt_manager_hfp_client_send_at_chup();
}

static void cmd_bt_hfp_dial_num(int argc,char *args[])
{
	bt_manager_hfp_client_send_at_atd(args[0]);
}

static void cmd_bt_hfp_subscriber_number(int argc,char *args[])
{
	bt_manager_hfp_client_send_at_cnum();
}

static void cmd_bt_hfp_last_num_dial(int argc,char *args[])
{
	bt_manager_hfp_client_send_at_bldn();
}
static void cmd_bt_test_cmd(int argc,char *args[])
{
	bt_manager_test_cmd();
}

static void cmd_bt_hfp_hf_vgs_volume(int argc,char *args[])
{
	int val;

	if(argc != 1) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	val = atoi(args[0]);
	if(val > 15 || val < 0) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	bt_manager_hfp_client_send_at_vgs(val);
}

static void cmd_bt_set_ex_debug_mask(int argc,char *args[])
{
	int val;

	if(argc != 1) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	val = atoi(args[0]);
	bt_manager_set_ex_debug_mask(val);
}

static void cmd_bt_set_discovery_mode(int argc,char *args[])
{

	int val;

	if(argc != 1) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}

	val = atoi(args[0]);
	if(val > 2 || val < 0) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	bt_manager_set_discovery_mode((btmg_discovery_mode_t)val);
}

static void cmd_bt_set_io_capability(int argc,char *args[])
{
	int val;

	if(argc != 1) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}

	val = atoi(args[0]);
	if(val > 4 || val < 0) {
		BTMG_ERROR("Unexpected argc: %d, see help", argc);
		return;
	}
	bt_manager_gap_set_io_capability((btmg_io_capability_t)val);
}

static void cmd_bt_get_version(int argc,char *args[])
{
	BTMG_INFO("bt manager version:%s,builed time:%s-%s",BTMGVERSION,__DATE__,__TIME__);
}

cmd_tbl_t cmd_table[] = {
	{"enable",        NULL , cmd_bt_enable,               "enable [0/1]: open bt or not"},
	{"scan",          NULL , cmd_bt_discovery,            "scan [0/1]: scan for devices"},
	{"scan_list",     NULL , cmd_bt_inquiry_list,         "scan_list: list available devices"},
	{"pair",          NULL , cmd_bt_pair,                 "pair [mac]: pair with devices"},
	{"unpair",        NULL , cmd_bt_unpair,               "uppair [mac]: unpair with devices"},
	{"paired_list",   NULL , cmd_bt_pair_devices_list,    "paired_list: list paired devices"},
	{"get_state",     NULL , cmd_bt_get_state,            "get_state: get bt state"},
	{"get_name",      NULL , cmd_bt_get_name,             "get_name: get bt name"},
	{"set_name",      NULL , cmd_bt_set_name,             "set_name [name]: set bt name"},
	{"remote_name",   NULL , cmd_bt_get_remote_name,      "remote_name: get remote device name"},
	{"set_vol",       NULL , cmd_bt_set_vol,              "set_vol: set a2dp source volme"},
	{"get_vol",       NULL , cmd_bt_get_vol,              "get_vol: get a2dp source volme"},
	{"get_addr",      NULL , cmd_bt_get_address,          "get_addr: get bt address"},
	{"set_dis",       NULL , cmd_bt_set_discovery_mode,   "set_dis [0~2]:0-NONE,1-page scan,2-inquiry scan&page scan"},
	{"set_io_cap",    NULL , cmd_bt_set_io_capability,    "set_io_cap [0~4]:0-keyboarddisplay,\
		1-displayonly,2-displayyesno,3-keyboardonly,4-noinputnooutput"},
	{"avrcp",         NULL , cmd_bt_avrcp,                "avrcp [play/pause/stop/fastforward/rewind\
/forward/backward]: avrcp control"},
	{"profile_cn",    NULL , cmd_bt_profile_conncet,       "profile_cn [mac]:a2dp sink connect"},
	{"profile_dis",   NULL , cmd_bt_profile_disconncet,    "profile_dis [mac]:a2dp sink disconnect"},
	{"connect",       NULL , cmd_bt_device_connect,        "connect [mac]:generic method to connect"},
	{"disconnect",    NULL , cmd_bt_device_disconnect,     "disconnect [mac]:generic method to disconnect"},
	{"remove",        NULL , cmd_bt_remove_device,         "remove [mac]:removes the remote device"},
	{"a2dp_src_start",NULL , cmd_bt_a2dp_src_start,        "a2dp_src_start:start a2dp source playing"},
	{"a2dp_src_stop", NULL , cmd_bt_a2dp_src_stop,         "a2dp_src_stop:stop a2dp source playing"},
	{"hfp_answer",    NULL , cmd_bt_hfp_answer_call,       "hfp_answer: answer the phone"},
	{"hfp_hangup",    NULL , cmd_bt_hfp_hangup,            "hfp_hangup: hangup the phone"},
	{"hfp_dial",      NULL , cmd_bt_hfp_dial_num,          "hfp_dial [num]: call to a phone number"},
	{"hfp_cnum",      NULL , cmd_bt_hfp_subscriber_number, "hfp_cum: Subscriber Number Information"},
	{"hfp_last_num",  NULL , cmd_bt_hfp_last_num_dial,     "hfp_last_num: calling the last phone number dialed"},
	{"hfp_vol",       NULL , cmd_bt_hfp_hf_vgs_volume,     "hfp_vol [0~15]: update phone's volume."},
	{"get_version",   NULL , cmd_bt_get_version,           "get_version: get btmanager version"},
	{"debug",         NULL , cmd_bt_set_log_level,         "debug [0~5]: set debug level"},
	{"ex_dbg",        NULL , cmd_bt_set_ex_debug_mask,     "ex_dbg [mask]: set ex debug mask"},
	{"test",          NULL , cmd_bt_test_cmd,              "test : test cmd"},
	{ NULL, NULL, NULL, NULL},
};

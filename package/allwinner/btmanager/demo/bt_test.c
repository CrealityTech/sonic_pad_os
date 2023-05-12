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
#define COLOR_OFF	"\x1B[0m"
#define COLOR_BLUE	"\x1B[0;34m"

#define CMD_ARGS_MAX	20

#define ARRAYSIZE(a) (sizeof(a) / sizeof(*(a)))
static int song_playing_pos = 0;
static int song_playing_len = 0;
static int is_background = 1;

static int _bt_deinit();

static void bt_test_manager_cb(int event_id)
{
	BTMG_INFO("bt test callback function enter, event_id: %d", event_id);
}

static void bt_test_adapter_power_state_cb(btmg_adapter_power_state_t state)
{

	if (state == BTMG_ADAPTER_TURN_ON_SUCCESSED) {
		BTMG_INFO("Turn on bt successfully");
	} else if (state == BTMG_ADAPTER_TURN_ON_FAILED) {
		BTMG_INFO("Failed to turn on bt");
	} else if (state == BTMG_ADAPTER_TURN_OFF_SUCCESSED) {
		BTMG_INFO("Turn off bt successfully");
	} else if (state == BTMG_ADAPTER_TURN_OFF_FAILED) {
		BTMG_INFO("Failed to turn off bt");
	}
}

static void bt_test_status_cb(btmg_state_t status)
{
    char bt_addr[18] = {0};
	char bt_name_buf[64] = {0};
    char bt_name[64] = {0};
    int len = sizeof(bt_addr);

	if (status == BTMG_STATE_OFF) {
		BTMG_INFO("BT is off");
	} else if (status == BTMG_STATE_ON) {
		BTMG_INFO("BT is ON");
        bt_manager_get_address(bt_addr, len);
        if (bt_addr[0] != '\0') {
            snprintf(bt_name_buf, 14, "aw-bt-test-%s-", (char *)(bt_addr + 12));
            sprintf(bt_name, "%s-%s", bt_name_buf, (char *)(bt_addr + 15));
            bt_manager_set_name(bt_name);
        } else {
            bt_manager_set_name("aw-bt-test");
        }
	} else if (status == BTMG_STATE_TURNING_ON) {
		BTMG_INFO("bt is turnning on.");
	} else if (status == BTMG_STATE_TURNING_OFF) {
		BTMG_INFO("bt is turnning off.");
	}
}

static void bt_test_discovery_status_cb(btmg_discovery_state_t status)
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

static void bt_test_gap_connected_changed_cb(btmg_bt_device_t *device)
{
	printf("addr:%s,name:%s,state:%s\n",device->remote_address,device->remote_name,
			device->connected ? "CONNECTED":"DISCONNECTED");
	if(device->connected == false)
		bt_manager_set_discovery_mode(BTMG_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
}

static void bt_test_dev_add_cb(btmg_bt_device_t *device)
{
	dev_node_t *dev_node = NULL;

	BTMG_INFO("address:%s,name:%s,class:%d,icon:%s,address type:%s,rssi:%d", device->remote_address,
			device->remote_name, device->r_class, device->icon, device->address_type, device->rssi);

	dev_node = btmg_dev_list_find_device(discovered_devices, device->remote_address);
	if (dev_node != NULL) {
		return;
	}
	btmg_dev_list_add_device(discovered_devices, device->remote_name, device->remote_address);
}

static void bt_test_dev_remove_cb(btmg_bt_device_t *device)
{
	dev_node_t *dev_node = NULL;

	BTMG_INFO("address:%s,name:%s,class:%d,address type:%s", device->remote_address,
			device->remote_name, device->r_class,device->address_type);
	btmg_dev_list_remove_device(discovered_devices, device->remote_address);
}

static void bt_test_update_rssi_cb(const char *address, int rssi)
{
	dev_node_t *dev_node = NULL;

	dev_node = btmg_dev_list_find_device(discovered_devices, address);
	if (dev_node != NULL) {
		BTMG_INFO("address:%s,name:%s,rssi:%d", dev_node->dev_addr, dev_node->dev_name, rssi);
		return;
	}
}

static void bt_test_bond_state_cb(btmg_bond_state_t state,const  char *bd_addr,const char *name)
{
	dev_node_t *dev_bonded_node = NULL;
	dev_node_t *dev_discovered_node = NULL;

	BTMG_DEBUG("bonded device state:%d, addr:%s, name:%s", state, bd_addr, name);
	dev_bonded_node = btmg_dev_list_find_device(bonded_devices, bd_addr);
	dev_discovered_node = btmg_dev_list_find_device(discovered_devices, bd_addr);

	if (state == BTMG_BOND_STATE_BONDED) {

        if (dev_bonded_node == NULL) {
			btmg_dev_list_add_device(bonded_devices, name, bd_addr);
        }

		if(dev_discovered_node != NULL) {
			btmg_dev_list_remove_device(discovered_devices, bd_addr);
        }

        BTMG_INFO("Pairing state for %s is BONDED", name);
	} else if (state == BTMG_BOND_STATE_NONE) {
		if (dev_bonded_node != NULL) {
			btmg_dev_list_remove_device(bonded_devices, bd_addr);
        }
        BTMG_INFO("Pairing state for %s is BOND NONE", name);
	} else if (state == BTMG_BOND_STATE_BONDING) {
		BTMG_INFO("Pairing state for %s is BONDEDING", name);
	}
}
#define BUFFER_SIZE 17
static void bt_test_pair_ask(const char *prompt,char *buffer)
{
	printf("%s", prompt);
	if (fgets(buffer, BUFFER_SIZE, stdin)  == NULL)
		fprintf(stdout, "\ncmd fgets error\n");
}

void bt_test_gap_request_pincode_cb(void *handle,char *device)
{
	char buffer[BUFFER_SIZE] = {0};

	fprintf(stdout, "AGENT:Request pincode (%s)\n",device);

	bt_test_pair_ask("Enter PIN Code: ",buffer);

	bt_manager_gap_send_pincode(handle,buffer);
}

void bt_test_gap_display_pin_code_cb(char *device,char *pincode)
{
	fprintf(stdout, "AGENT: Pincode %s\n", pincode);
}

void bt_test_gap_request_passkey_cb(void *handle,char *device)
{
	unsigned long passkey;
	char buffer[BUFFER_SIZE] = {0};

	fprintf(stdout, "AGENT: Request passkey (%s)\n",
		device);
	//bt_test_pair_ask("Enter passkey (1~999999): ",buffer);
	//passkey = strtoul(buffer, NULL, 10);
	//if ((passkey > 0) && (passkey < 999999))
		bt_manager_gap_send_passkey(handle,passkey);
	//else
	//	fprintf(stdout, "AGENT: get passkey error\n");
}

void bt_test_gap_display_passkey_cb(char *device,unsigned int passkey,
		unsigned int entered)
{
	fprintf(stdout, "AGENT: Passkey %06u\n", passkey);
}

void bt_test_gap_confirm_passkey_cb(void *handle,char *device,unsigned int passkey)
{
	char buffer[BUFFER_SIZE] = {0};

	fprintf(stdout, "AGENT: Request confirmation (%s)\nPasskey: %06u\n",
		device, passkey);
	//bt_test_pair_ask("Confirm passkey? (yes/no): ",buffer);
	//if (!strncmp(buffer, "yes", 3))
		bt_manager_gap_pair_send_empty_response(handle);
	//else
	//	bt_manager_gap_send_pair_error(handle,BT_PAIR_REQUEST_REJECTED,"");
}

void bt_test_gap_authorize_cb(void *handle,char *device)
{

	char buffer[BUFFER_SIZE] = {0};
	fprintf(stdout, "AGENT: Request authorization (%s)\n",device);

	bt_test_pair_ask("Authorize? (yes/no): ",buffer);

	//if (!strncmp(buffer, "yes", 3))
		bt_manager_gap_pair_send_empty_response(handle);
	//else
	//	bt_manager_gap_send_pair_error(handle,BT_PAIR_REQUEST_REJECTED,"");
}

void bt_test_gap_authorize_service_cb(void *handle,char *device,char *uuid)
{
	char buffer[BUFFER_SIZE] = {0};
	fprintf(stdout, "AGENT: Authorize Service (%s, %s)\n",
		device, uuid);
	//if(is_background == 0) {
	//	bt_test_pair_ask("Authorize connection? (yes/no): ",buffer);

	//	if (!strncmp(buffer, "yes", 3))
			bt_manager_gap_pair_send_empty_response(handle);
	//	else
	//		bt_manager_gap_send_pair_error(handle,BT_PAIR_REQUEST_REJECTED,"");
	//}else {
	//	bt_manager_gap_pair_send_empty_response(handle);
	//}
}


static void bt_test_a2dp_sink_connection_state_cb(const char *bd_addr, btmg_a2dp_sink_connection_state_t state)
{

	if (state == BTMG_A2DP_SINK_DISCONNECTED) {
		BTMG_INFO("A2DP sink disconnected with device: %s", bd_addr);
		bt_manager_set_discovery_mode(BTMG_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
	} else if (state == BTMG_A2DP_SINK_CONNECTING) {
		BTMG_INFO("A2DP sink connecting with device: %s", bd_addr);
	} else if (state == BTMG_A2DP_SINK_CONNECTED) {
		BTMG_INFO("A2DP sink connected with device: %s", bd_addr);
	} else if (state == BTMG_A2DP_SINK_DISCONNECTING) {
		BTMG_INFO("A2DP sink disconnecting with device: %s", bd_addr);
	}
}

static void bt_test_a2dp_sink_audio_state_cb(const char *bd_addr, btmg_a2dp_sink_audio_state_t state)
{
	if (state == BTMG_A2DP_SINK_AUDIO_SUSPENDED) {
		BTMG_INFO("A2DP sink audio suspended with device: %s", bd_addr);
	} else if (state == BTMG_A2DP_SINK_AUDIO_STOPPED) {
		BTMG_INFO("A2DP sink audio stopped with device: %s", bd_addr);
	} else if (state == BTMG_A2DP_SINK_AUDIO_STARTED) {
		BTMG_INFO("A2DP sink audio started with device: %s", bd_addr);
	}
}

static void bt_test_a2dp_source_connection_state_cb(const char *bd_addr, btmg_a2dp_source_connection_state_t state)
{

	if (state == BTMG_A2DP_SOURCE_DISCONNECTED) {
		BTMG_INFO("A2DP source disconnected with device: %s", bd_addr);
		bt_manager_set_discovery_mode(BTMG_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
	} else if (state == BTMG_A2DP_SOURCE_CONNECTING) {
		BTMG_INFO("A2DP source connecting with device: %s", bd_addr);
	} else if (state == BTMG_A2DP_SOURCE_CONNECTED) {
		BTMG_INFO("A2DP source connected with device: %s", bd_addr);
	} else if (state == BTMG_A2DP_SOURCE_DISCONNECTING) {
		BTMG_INFO("A2DP source disconnecting with device: %s", bd_addr);
	} else if (state == BTMG_A2DP_SOURCE_CONNECT_FAILED) {
		BTMG_INFO("A2DP source connect with device: %s failed!", bd_addr);
	} else if (state == BTMG_A2DP_SOURCE_DISCONNEC_FAILED) {
		BTMG_INFO("A2DP source disconnect with device: %s failed!", bd_addr);
	}
}

static void bt_test_a2dp_source_audio_state_cb(const char *bd_addr, btmg_a2dp_source_audio_state_t state)
{
	if (state == BTMG_A2DP_SOURCE_AUDIO_SUSPENDED) {
		BTMG_INFO("A2DP source audio suspended with device: %s", bd_addr);
	} else if (state == BTMG_A2DP_SOURCE_AUDIO_STOPPED) {
		BTMG_INFO("A2DP source audio stopped with device: %s", bd_addr);
	} else if (state == BTMG_A2DP_SOURCE_AUDIO_STARTED) {
		BTMG_INFO("A2DP source audio started with device: %s", bd_addr);
	}
}

static void bt_test_avrcp_play_state_cb(const char *bd_addr, btmg_avrcp_play_state_t state)
{
	if (state == BTMG_AVRCP_PLAYSTATE_STOPPED) {
		BTMG_INFO("BT playing music stopped with device: %s", bd_addr);
	} else if (state == BTMG_AVRCP_PLAYSTATE_PLAYING) {
		BTMG_INFO("BT palying music playing with device: %s", bd_addr);
	} else if (state == BTMG_AVRCP_PLAYSTATE_PAUSED) {
		BTMG_INFO("BT palying music paused with device: %s", bd_addr);
	} else if (state == BTMG_AVRCP_PLAYSTATE_FWD_SEEK) {
		BTMG_INFO("BT palying music FWD SEEK with device: %s", bd_addr);
	} else if (state == BTMG_AVRCP_PLAYSTATE_REV_SEEK) {
		BTMG_INFO("BT palying music REV SEEK with device: %s", bd_addr);
	} else if (state == BTMG_AVRCP_PLAYSTATE_ERROR) {
		BTMG_INFO("BT palying music ERROR with device: %s", bd_addr);
	}

}
static void bt_test_avrcp_track_changed_cb(const char *bd_addr, btmg_track_info_t track_info)
{
	BTMG_DEBUG("BT playing device: %s", bd_addr);
	BTMG_DEBUG("BT playing music title: %s", track_info.title);
	BTMG_DEBUG("BT playing music artist: %s", track_info.artist);
	BTMG_DEBUG("BT playing music album: %s", track_info.album);
	BTMG_DEBUG("BT playing music track number: %s", track_info.track_num);
	BTMG_DEBUG("BT playing music total number of tracks: %s", track_info.num_tracks);
	BTMG_DEBUG("BT playing music genre: %s", track_info.genre);
	BTMG_DEBUG("BT playing music playing time: %s", track_info.playing_time);
}

static void bt_test_avrcp_audio_volume_cb(const char *bd_addr, unsigned int volume)
{
	BTMG_INFO("AVRCP audio volume:%s : %d", bd_addr, volume);
}

static void bt_test_avrcp_play_position_cb(const char *bd_addr, int song_len, int song_pos)
{
	if (song_playing_pos > song_pos && song_playing_len != song_len) {
		BTMG_INFO("AVRCP playing song switched");
	}
	song_playing_len = song_len;
	song_playing_pos = song_pos;
	BTMG_DEBUG("%s,playing song len:%d,position:%d",bd_addr, song_len, song_pos);

}

static const char* _hfp_event_to_string(btmg_hfp_even_t event)
{
	switch(event) {
		case BTMG_HFP_CONNECT:
			return "HFP_CONNECT";
		case BTMG_HFP_CONNECT_LOST:
			return "HFP_CONNECT_LOST";

		case BTMG_HFP_CIND:
			return "HFP_CIND";
		case BTMG_HFP_CIEV:
			return "HFP_CIEV";
		case BTMG_HFP_RING:
			return "HFP_RING";
		case BTMG_HFP_CLIP:
			return "HFP_CLIP";
		case BTMG_HFP_BSIR:
			return "HFP_BSIR";
		case BTMG_HFP_BVRA:
			return "HFP_BVRA";
		case BTMG_HFP_CCWA:
			return "HFP_CCWA";
		case BTMG_HFP_CHLD:
			return "HFP_CHLD";
		case BTMG_HFP_VGM:
			return "HFP_VGM";
		case BTMG_HFP_VGS:
			return "HFP_VGS";
		case BTMG_HFP_BINP:
			return "HFP_BINP";
		case BTMG_HFP_BTRH:
			return "HFP_BTRH";
		case BTMG_HFP_CNUM:
			return "HFP_CNUM";
		case BTMG_HFP_COPS:
			return "HFP_COPS";
		case BTMG_HFP_CMEE:
			return "HFP_CMEE";
		case BTMG_HFP_CLCC:
			return "HFP_CLCC";
		case BTMG_HFP_UNAT:
			return "HFP_UNAT";
		case BTMG_HFP_OK:
			return "HFP_OK";
		case BTMG_HFP_ERROR:
			return "HFP_ERROR";
		case BTMG_HFP_BCS:
			return "HFP_BCS";
	}
}

static void bt_test_hfp_event_cb(btmg_hfp_even_t event, btmg_hfp_data_t *data)
{
	BTMG_INFO("event %s",_hfp_event_to_string(event));
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

	for (i = 0; cmd_table[i].cmd; i++) {
		if (strcmp(cmd, cmd_table[i].cmd))
			continue;

		if (cmd_table[i].func) {
			cmd_table[i].func(argc, args);
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
	for (i = 0; cmd_table[i].cmd; i++) {
		if (cmd_table[i].desc)
			printf("\t%-20s\t\t%-15s\n", cmd_table[i].cmd,
							cmd_table[i].desc);
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

static btmg_callback_t *bt_callback = NULL;

static int _bt_init(int profile)
{
	if(bt_manager_preinit(&bt_callback) != 0) {
		printf("bt preinit failed!\n");
		return -1;
	}
	bt_callback->btmg_manager_cb.bt_mg_cb = bt_test_manager_cb;
	bt_callback->btmg_adapter_cb.adapter_power_state_cb = bt_test_adapter_power_state_cb;

	bt_callback->btmg_gap_cb.gap_status_cb = bt_test_status_cb;
	bt_callback->btmg_gap_cb.gap_disc_status_cb = bt_test_discovery_status_cb;
	bt_callback->btmg_gap_cb.gap_device_add_cb = bt_test_dev_add_cb;
	bt_callback->btmg_gap_cb.gap_device_remove_cb = bt_test_dev_remove_cb;
	bt_callback->btmg_gap_cb.gap_update_rssi_cb =	bt_test_update_rssi_cb;
	bt_callback->btmg_gap_cb.gap_bond_state_cb = bt_test_bond_state_cb;
	bt_callback->btmg_gap_cb.gap_connect_changed = bt_test_gap_connected_changed_cb;

	/* bt security callback setting.*/
	bt_callback->btmg_gap_cb.gap_request_pincode = bt_test_gap_request_pincode_cb;
	bt_callback->btmg_gap_cb.gap_display_pin_code = bt_test_gap_display_pin_code_cb;
	bt_callback->btmg_gap_cb.gap_request_passkey = bt_test_gap_request_passkey_cb;
	bt_callback->btmg_gap_cb.gap_display_passkey = bt_test_gap_display_passkey_cb;
	bt_callback->btmg_gap_cb.gap_confirm_passkey = bt_test_gap_confirm_passkey_cb;
	bt_callback->btmg_gap_cb.gap_authorize  = bt_test_gap_authorize_cb;
	bt_callback->btmg_gap_cb.gap_authorize_service = bt_test_gap_authorize_service_cb;

	/* bt a2dp sink callback*/
	bt_callback->btmg_a2dp_sink_cb.a2dp_sink_connection_state_cb = bt_test_a2dp_sink_connection_state_cb;
	bt_callback->btmg_a2dp_sink_cb.a2dp_sink_audio_state_cb = bt_test_a2dp_sink_audio_state_cb;

	/* bt a2dp source callback*/
	bt_callback->btmg_a2dp_source_cb.a2dp_source_connection_state_cb = bt_test_a2dp_source_connection_state_cb;
	bt_callback->btmg_a2dp_source_cb.a2dp_source_audio_state_cb = bt_test_a2dp_source_audio_state_cb;

	/* bt avrcp callback*/
	bt_callback->btmg_avrcp_cb.avrcp_play_state_cb = bt_test_avrcp_play_state_cb;
	bt_callback->btmg_avrcp_cb.avrcp_play_position_cb = bt_test_avrcp_play_position_cb;
	bt_callback->btmg_avrcp_cb.avrcp_track_changed_cb = bt_test_avrcp_track_changed_cb;
	bt_callback->btmg_avrcp_cb.avrcp_audio_volume_cb = bt_test_avrcp_audio_volume_cb;

	/* bt hfp callback*/
	bt_callback->btmg_hfp_cb.hfp_hf_event_cb = bt_test_hfp_event_cb;

	if(profile == 0) {
		bt_manager_set_enable_default(true);
	}else {
		bt_manager_enable_profile(profile);
	}

	if(bt_manager_init(bt_callback) != 0) {
		printf("bt manager init failed.\n");
		return -1;
	}

	bonded_devices = btmg_dev_list_new();
	if(bonded_devices == NULL)
		goto Failed;

	discovered_devices = btmg_dev_list_new();
	if(discovered_devices == NULL)
		goto Failed;

	bt_manager_enable(true);
	if(is_background)
		bt_manager_gap_set_io_capability(BTMG_IO_CAP_NOINPUTNOOUTPUT);
	else
		bt_manager_gap_set_io_capability(BTMG_IO_CAP_KEYBOARDDISPLAY);

	bt_manager_set_discovery_mode(BTMG_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
	return 0;
Failed:
	bt_manager_deinit(bt_callback);
	return -1;
}

static int _bt_deinit()
{
	bt_manager_enable(false);
	bt_manager_deinit(bt_callback);
	btmg_dev_list_free(discovered_devices);
	btmg_dev_list_free(bonded_devices);
}

int daemon_init(int debug)
{
    int i ;
    pid_t pid;
    if((pid = fork()) < 0)
        return -1;
    else if(pid)
        _exit(0);
    if(setsid() < 0)
        return -1;
    signal(SIGHUP,SIG_IGN);
    if((pid = fork()) < 0)
        return -1;
    else if(pid)
        _exit(0);
    chdir("/");
    if(!debug) {
        for(i = 0; i < 64; i++)
            close(i);
        open("/dev/null",O_RDONLY);
        open("/dev/null",O_RDWR);
        open("/dev/null",O_RDWR);
    }
    return 0;
}
void main_loop_stop(int sig)
{
	struct sigaction sigact;

	sigact.sa_handler = SIG_DFL;

	sigaction(sig, &sigact, NULL);
    if ( bt_manager_get_state()  == BTMG_STATE_ON) {
		bt_manager_enable(false);
    }

	while (bt_manager_get_state()  != BTMG_STATE_OFF) {
		sleep(1);
	}

	bt_manager_deinit(bt_callback);
	btmg_dev_list_free(discovered_devices);
	btmg_dev_list_free(bonded_devices);
	exit(0);

}

int main(int argc, char **argv)
{
	int opt;

	const char *opts = "hp:d:is";
	const struct option longopts[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "stop", no_argument , NULL, 's' },
		{ "interaction", no_argument, NULL, 'i' },
		{ "profile", required_argument, NULL, 'p' },
		{ "debug", required_argument, NULL, 'd' },
		{ 0, 0, 0, 0 },
	};
	int profile = 0;

	opterr = 0;
	while ((opt = getopt_long(argc, argv, opts, longopts, NULL)) != -1) {
		switch (opt) {
		case 'h':
		printf("Usage:\n"
					"  [OPTION]...\n"
					"\nOptions:\n"
					"  -h, --help\t\tprint this help and exit\n"
					"  -d, --debug\t open debug :-d [0~5]\n"
					"  -s, --stop\t stop bt_test\n"
					"  -p, --profile=NAME\tenable BT profile\n"
					"  -i, --interaction\t interaction\n"
					"  - a2dp-source\tAdvanced Audio Source\n"
					"  - a2dp-sink\tAdvanced Audio Sink\n"
					"  - hfp-hf\tHands-Free \n"
					"  - hfp-ag\tHands-Free Audio Gateway\n"
					"  - hsp-hs\tHeadset \n"
					"  - hsp-ag\tHeadset Audio Gateway\n");
			return EXIT_SUCCESS;
		case 'p':
			{
				size_t i;
				struct {
					const char *name;
					int value;
				} map[] = {
					{ "a2dp-sink", BTMG_A2DP_SINK_ENABLE },
					{ "a2dp-source",BTMG_A2DP_SOUCE_ENABLE},
					{ "avrcp", BTMG_AVRCP_ENABLE },
					{ "hfp-hf", BTMG_HFP_HF_ENABLE },
					{ "hfp-ag", BTMG_HFP_AG_ENABLE},
				};

				for (i = 0; i < ARRAYSIZE(map); i++)
					if (strcasecmp(optarg, map[i].name) == 0) {
						profile |= map[i].value;
						break;
					}

				if (i == ARRAYSIZE(map)) {
					printf("Invalid BT profile name: %s\n", optarg);
					return EXIT_FAILURE;
				}
			}
			break;
		case 'd':
			btmg_log_level_t debug;
			debug = (btmg_log_level_t)atoi(optarg);
			printf("debug loglevel :%d\n",(int)debug);
			bt_manager_set_loglevel(debug);
			break;
		case 'i':
			is_background = 0;
			break;
		case 's':
			printf("stop bt_test\n");
			system("killall -9 bluealsa");
			usleep(1000*10);
			system("killall -9 bt_test");
			return EXIT_SUCCESS;
		}
	}

	int sigfd;
	struct pollfd pfd[2];

    char fifo_r[1024] ={'\0'};
    uint32_t fifo_r_len = 0;

	if(is_background) {
		daemon_init(1);
	}
	if(_bt_init(profile) != 0)
		return -1;

	if(is_background == 1) {
		struct sigaction sigact ;

		sigact.sa_handler = main_loop_stop;

		sigaction(SIGTERM, &sigact, NULL);
		sigaction(SIGINT, &sigact, NULL);
		while(1) {
			select(0,NULL,NULL,NULL,NULL);
		}
	}

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

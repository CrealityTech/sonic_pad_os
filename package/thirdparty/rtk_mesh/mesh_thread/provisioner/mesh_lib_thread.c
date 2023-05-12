#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <semaphore.h>
#include <getopt.h>
#include <stdbool.h>
//#include <wordexp.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/signalfd.h>
#include <sched.h>

#include <monitor/bt.h>
#include <shared/io.h>
#include <shared/mainloop.h>
#include <shared/queue.h>

/* gap and mesh */
#include <gap_msg.h>
#include <gap_internal.h>
#include <gap_le.h>
#include <gap_adv.h>
#include <btif.h>
#include <trace.h>
#include <app_msg.h>
#include <mesh_common.h>
#include <platform_os.h>
//#include <mesh_cmd.h>
#include <ping_app.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <pthread.h>

#include "ftl.h"
#include "ftl_old.h"
#include "hci_imp.h"

#include "RTKMeshTypes.h"
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

/* .h */
/******************************************************************************/
#define plt_time_init() os_sys_time_init()
#define plt_os_queue_destory(queue_handle) os_msg_queue_destory(queue_handle)
/******************************************************************************/

#define MAX_NUMBER_OF_IO_MESSAGE	0x20

#define BTA_THREAD_EXIT_CODE	0x65

struct cmd_info {
	char *cmd;
	void (*func)(int argc, char **argv);
	char *doc;
};

plt_os_queue_handle_t io_queue_handle;
plt_os_queue_handle_t mesh_queue_handle;
static int msg_pipe[2];

static sem_t bt_sem;
static pthread_t mesh_bta_tid;
static int sockfds[2];

static bool thread_enable = false;
static uint16_t local_unicast_addr = 0x0056;

uint64_t APPloglayer = 0;
uint32_t MESHloglayer = 0;
uint8_t loglevel = 0;
uint16_t devnum = 0;

static void le_adv_report(const uint8_t *data, uint8_t size,
			  void *user_data)
{
	const struct bt_hci_evt_le_adv_report *evt = (void *)data;
	int i;
	T_LE_SCAN_INFO scinfo;
	//APP_PRINT_INFO1(" there are %d adv reports need to be handled!!!***",evt->num_reports);
	for (i = 0; i < evt->num_reports; i++) {
		scinfo.adv_type = evt->event_type;
		scinfo.remote_addr_type = evt->addr_type;
		scinfo.data_len = evt->data_len;
		memcpy(scinfo.bd_addr, evt->addr, 6);
		memcpy(scinfo.data, evt->data, evt->data_len);
		scinfo.rssi = (int8_t)*(evt->data + evt->data_len);

		/*APP_PRINT_INFO6("Received adv: addr-%x-%x-%x-%x-%x-%x\n",
		       scinfo.bd_addr[0], scinfo.bd_addr[1],
		       scinfo.bd_addr[2], scinfo.bd_addr[3],
		       scinfo.bd_addr[4], scinfo.bd_addr[5]);*/
		/* mesh stack doesn't hold the buffer, just pass local
		 * local variable to it
		 */
		gap_sched_handle_adv_report(&scinfo);
		//APP_PRINT_INFO1("adv %d has been handled",i);
	}
}

static void hci_report_le_adv(const void *data, uint8_t size,
							void *user_data)
{
	uint8_t evt_code = ((const uint8_t *)data)[0];
	//APP_PRINT_INFO0("hci_report_le_adv start!!!***");
	switch (evt_code) {
	case BT_HCI_EVT_LE_ADV_REPORT:
		le_adv_report((const uint8_t *)data + 1, size - 1, user_data);
		break;
	}
	//APP_PRINT_INFO0("hci_report_le_adv finish!!!***");
}

static void msg_pipe_read_cb(int fd, uint32_t events, void *user_data)
{
	bool result;
	uint8_t event;
	T_IO_MSG io_msg;

	//APP_PRINT_INFO0("Pipe read\n");

	if (events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
		APP_PRINT_ERROR0("Pipe error");
		return;
	}

	result = os_event_recv((void*)&fd, &event, 0);;
	if (!result) {
		APP_PRINT_WARN1("%s: recv event failed", __func__);
		return;
	}

	//APP_PRINT_INFO1("Message 0x%02x\n", event);
	switch (event) {
	case MESH_INIT_STEP0:
		le_gap_msg_init(&msg_pipe[1], io_queue_handle);
		mesh_start(EVENT_MESH, 0, &msg_pipe[1], mesh_queue_handle);
		mesh_stack_init(local_unicast_addr);

		/* Give message to gap_sched_handle_bt_status_msg */
		//gap_stack_ready();
		/* Input 'enable' command, handler will call gap_stack_ready. */

		plt_time_init();

		break;
	case EVENT_MESH:
		mesh_inner_msg_handle(event);
		break;
	case EVENT_IO_TO_APP:
		/* Call mesh stack api */
		if (plt_os_queue_receive(io_queue_handle, &io_msg, 0))
		{
			app_handle_io_msg(io_msg);
		}
		else
		{
			APP_PRINT_WARN1("*!! no related msg recv ,subtype 0x%04x\n", io_msg.subtype);
		}
		break;
	default:
		APP_PRINT_WARN1("Unknown message 0x%02x\n", event);
		break;
	}

	return;
}
static void mesh_bta_cmd_handler(int fd, uint32_t events, void *user_data)
{
	ssize_t result;
	uint8_t c;
	APP_PRINT_TRACE1(" PID %lu ",pthread_self());
	//APP_PRINT_INFO0("mesh_bta_cmd_handler");
	result = read(fd, &c, 1);
	if (result < 0) {
		APP_PRINT_ERROR0("Read channel error");
		mainloop_quit2();
	}

	if (result == 1 && c == BTA_THREAD_EXIT_CODE) {
		APP_PRINT_INFO0("Received bta thread exit code\n");
		mainloop_quit2();
	}

	mesh_bta_run_command();
}

#ifdef MESH_ANDROID

#else
static int hci_open_dev2(int dev_id)
{
    struct sockaddr_hci a;
    int dd, err;

    /* Check for valid device id */
    if (dev_id < 0) {
        errno = ENODEV;
        return -1;
    }

    /* Create HCI socket */
    dd = socket(AF_BLUETOOTH, SOCK_RAW | SOCK_CLOEXEC, BTPROTO_HCI);
    if (dd < 0)
        return dd;

    /* Bind socket to the HCI device */
    memset(&a, 0, sizeof(a));
    a.hci_family = AF_BLUETOOTH;
    a.hci_dev = dev_id;
    if (bind(dd, (struct sockaddr *) &a, sizeof(a)) < 0)
        goto failed;

    return dd;

failed:
    err = errno;
    close(dd);
    errno = err;

    return -1;
}
#endif

static int hci_close_dev2(int dd)
{
    return close(dd);
}
static int hci_send_cmd2(int dd, uint16_t ogf, uint16_t ocf,
        uint8_t plen, void *param)
{
    uint8_t type = 0x01;//HCI_COMMAND_PKT;
    hci_command_hdr hc;
    struct iovec iv[3];
    int ivn;
	uint8_t *p;
    hc.opcode = htobs(cmd_opcode_pack(ogf, ocf));
    hc.plen= plen;

	p=(uint8_t *)&hc.opcode;
	APP_PRINT_ERROR4("opcode 0x%04x, 0x%04x, 0x%02x, 0x%02x ",hc.opcode,htobs(cmd_opcode_pack(ogf, ocf)),p[0],p[1]);

    iv[0].iov_base = &type;
    iv[0].iov_len  = 1;
    iv[1].iov_base = &hc;
    iv[1].iov_len  = HCI_COMMAND_HDR_SIZE;
    ivn = 2;

    if (plen) {
        iv[2].iov_base = param;
        iv[2].iov_len  = plen;
        ivn = 3;
    }

    while (writev(dd, iv, ivn) < 0) {
        if (errno == EAGAIN || errno == EINTR)
            continue;
        return -1;
    }
    return 0;
}

static bool snd_cmd_2_android( int fd)
{
  int sk;
  uint16_t ogf, ocf;
	uint8_t param_len = 0;

#define HCI_VSC_MESH_NOTIFY             0xFFFE

  sk = fd;
  ogf = (HCI_VSC_MESH_NOTIFY >> 10) & 0x3f;
  ocf = HCI_VSC_MESH_NOTIFY & 0x03ff;

  APP_PRINT_ERROR1("snd_cmd_2_android fd %d",sk);
  if (hci_send_cmd2(sk, ogf, ocf, param_len, NULL) < 0) {
    APP_PRINT_ERROR2("snd_cmd_2_android failed:%s (%d)\n",strerror(errno), errno);
    hci_close_dev2(sk);
    return false;
  }

  return true;
}

static int disable_ble_scan( int fd)
{
  int sk;
  uint16_t ogf, ocf;
	uint8_t param[2]={0x00,0x00};
	uint8_t param_len = 2;

#ifdef MESH_ANDROID
  sk = fd;
#else
  sk = hci_open_dev2(0);
  if (sk < 0) {
    APP_PRINT_ERROR0("HCI device open failed");
    return -3;
  }
#endif

#ifdef MESH_ANDROID
  ogf = 0x3f;
  ocf = 0x00a9;
#else
  ogf = 0x08;
  ocf = 0x000C;
#endif

  APP_PRINT_ERROR1("disable ble scan fd %d ",sk);
  if (hci_send_cmd2(sk, ogf, ocf, param_len, param) < 0) {
    APP_PRINT_ERROR2("disable ble scan failed:%s (%d)\n",strerror(errno), errno);
    hci_close_dev2(sk);
    return -4;
  }

  APP_PRINT_INFO0("disable ble scan successfully");

#ifdef MESH_ANDROID

#else
  hci_close_dev2(sk);
#endif

    return 0;
}

static int disable_ble_adv(int fd)
{
    int sk;
    uint16_t ogf, ocf;
	uint8_t param[1]={0x00};
	uint8_t param_len = 1;

#ifdef MESH_ANDROID
    sk = fd;
#else
    sk = hci_open_dev2(0);
    if (sk < 0) {
        APP_PRINT_ERROR0("HCI device open failed");
        return -3;
    }
#endif
    ogf = 0x08;
    ocf = 0x000A;

	APP_PRINT_ERROR1("disable ble adv fd %d ",sk);
    if (hci_send_cmd2(sk, ogf, ocf, param_len, param) < 0) {
        APP_PRINT_ERROR2("disable ble adv failed:%s (%d)\n",strerror(errno), errno);
        hci_close_dev2(sk);
        return -4;
    }

    APP_PRINT_INFO0("disable ble adv successfully");

#ifdef MESH_ANDROID

#else
    hci_close_dev2(sk);
#endif

    return 0;
}

void *mesh_bta_thread_handler(void* arg)
{
	//struct io *input;		//no need input
	uint16_t index = 0;
	int status;
	uint8_t params[8];
	bool ret = false;
	uint64_t mask[LEVEL_NUM] = {
		0xffffffffffffffff,
		0xffffffffffffffff,
		0xffffffffffffffff,
		0xffffffffffffffff,
	};
	uint8_t event;
	int result;
	int addfdret;
  int fd = -1;

	//set_realtime_priority();

	local_unicast_addr = *(uint16_t*)arg;

#ifdef MESH_ANDROID
  fd = hci_socket_connect();
  APP_PRINT_ERROR1("connect return fd %d",fd);
  snd_cmd_2_android(fd);
#endif
	log_module_trace_init(mask);

	/*before mesh stack starts to work, ble scan and ble must be stopped*/
	disable_ble_scan(fd);
	disable_ble_adv(fd);

#ifdef BAIDU_CUBE_PROJ
	if(ftl_init_old(0,0))
	{
		APP_PRINT_ERROR0("ftl_init_old failed ");
		ftl_close_old();
		return NULL;
	}
#endif

	if(ftl_init(0,0))
	{
		APP_PRINT_ERROR0("ftl_init failed ");
		ftl_close();
		return NULL;
	}

	mainloop_init2();

	result = socketpair(PF_UNIX, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
			    0, sockfds);
	if (result == -1) {
		APP_PRINT_ERROR0("Couldn't create paired sockets");
		return NULL;
	}
	APP_PRINT_INFO2("sockfds %d %d\n", sockfds[0], sockfds[1]);
	set_mesh_bta_mainloop_id(pthread_self());
	set_mesh_bta_cmd_fd(sockfds);

	io_queue_handle = plt_os_queue_create(MAX_NUMBER_OF_IO_MESSAGE,
					      sizeof(T_IO_MSG));
	if (!io_queue_handle) {
		APP_PRINT_ERROR0("Create io queue error\n");
		goto err0;
	}

	mesh_queue_handle = plt_os_queue_create(MESH_INNER_MSG_NUM,
						sizeof(mesh_inner_msg_t));
	if (!mesh_queue_handle) {
		APP_PRINT_ERROR0("Create mesh queue error\n");
		goto err1;
	}

	if (pipe(msg_pipe) < 0) {
		APP_PRINT_ERROR0("Pipe error\n");
		goto err2;
	}

	/* Write mesh init message */
	event = MESH_INIT_STEP0;
	if (write(msg_pipe[1], &event, 1) < 0) {
		APP_PRINT_ERROR0("Write pipe error, mesh init failed\n");
		goto err3;
	}
	ret = hci_init(&index);
	if (!ret) {
		APP_PRINT_ERROR0("Unable to open mgmt_socket\n");
		goto err3;
	}

	ret = hci_event_register(BT_HCI_EVT_LE_META_EVENT,
			       hci_report_le_adv, NULL,
			       NULL);
	if (!ret) {
		APP_PRINT_ERROR0("Reister hci mgmt failed\n");
		goto err4;
	}

	addfdret = mainloop_add_fd2(msg_pipe[0], EPOLLIN, msg_pipe_read_cb,
			    NULL, NULL);
	if (addfdret < 0) {
		APP_PRINT_ERROR1("Failed to add listen socket,addfdret=%d\n",addfdret);
		goto err5;
	}
	APP_PRINT_INFO1(" before add sockfds:PID %lu ",pthread_self());
	addfdret = mainloop_add_fd2(sockfds[0], EPOLLIN, mesh_bta_cmd_handler,
			    NULL, NULL);
	if (addfdret < 0) {
		APP_PRINT_ERROR1("Failed to install command handler,addfdret=%d\n",addfdret);
		goto err6;
	}


	sem_post(&bt_sem);
	status = mainloop_run2();
	APP_PRINT_INFO1(" exit mainloop_run2:PID %lu ",pthread_self());

	APP_PRINT_INFO0("mainloop exit");
	user_app_local_mesh_node_deinit();
	APP_PRINT_INFO0("mesh_node has deinit");

	mainloop_remove_fd2(sockfds[0]);
	APP_PRINT_INFO0("mesh thread remove sockfds");

	mainloop_remove_fd2(msg_pipe[0]);
	APP_PRINT_INFO0("mesh thread remove msg_pipe");

	hci_event_unregister(BT_HCI_EVT_LE_META_EVENT );
	APP_PRINT_INFO0("mesh thread unregister hci_mgmt");

	hci_deinit();
	APP_PRINT_INFO0("mesh thread release hci_mgmt");

	close(msg_pipe[0]);
	close(msg_pipe[1]);
	APP_PRINT_INFO0("mesh thread close msg_pipe");

	plt_os_queue_destory(mesh_queue_handle);
	APP_PRINT_INFO0("mesh thread destory mesh_queue_handle");
	plt_os_queue_destory(io_queue_handle);
	APP_PRINT_INFO0("mesh thread destory io_queue_handle");

	close(sockfds[0]);
	close(sockfds[1]);
	APP_PRINT_INFO0("mesh thread close sockfds");

  log_disable();
	ftl_close();
#ifdef BAIDU_CUBE_PROJ
	ftl_close_old();
#endif
	APP_PRINT_INFO0("mesh thread exit");

	APP_PRINT_INFO1("mesh disable: stopping ble adv/scan,fd %d",fd);
	disable_ble_scan(fd);
	disable_ble_adv(fd);
#ifdef MESH_ANDROID
	hci_socket_close();
#endif
	return NULL;
err6:
	mainloop_remove_fd2(msg_pipe[0]);
err5:
	hci_event_unregister(BT_HCI_EVT_LE_META_EVENT );
err4:
	hci_deinit();
err3:
	close(msg_pipe[0]);
	close(msg_pipe[1]);
err2:
	plt_os_queue_destory(mesh_queue_handle);
err1:
	plt_os_queue_destory(io_queue_handle);
err0:
	close(sockfds[0]);
	close(sockfds[1]);
  log_disable();
	ftl_close();
#ifdef BAIDU_CUBE_PROJ
	ftl_close_old();
#endif
	sem_post(&bt_sem);
#ifdef MESH_ANDROID
	hci_socket_close();
#endif
	return NULL;
}
int mesh_bta_thread_create(uint16_t local_unicast_addr)
{
	int result;
	result = sem_init(&bt_sem, 0, 0);
	if (result != 0) {
			APP_PRINT_ERROR0("sem_init failed");
			return result;
	}

	result = pthread_create(&mesh_bta_tid, NULL, mesh_bta_thread_handler, (void*)&local_unicast_addr);
	if (result != 0)
	{
		APP_PRINT_INFO0("Couldn't create bluetooth thread");
		goto mesh_destroy_sem;
	}
	result = sem_wait(&bt_sem);
	if(result == -1)
	{
		APP_PRINT_ERROR0("sem wait error");
		goto mesh_destroy_sem;
	}

mesh_destroy_sem:
	sem_destroy(&bt_sem);
	return result;
}

int32_t quit_mesh_bta_thread(void)
{
	uint8_t c;
	int result;
	static uint32_t thread_quit_count = 0;
	thread_quit_count ++;
	APP_PRINT_INFO1("count of calling thread_quit func count %d\n",thread_quit_count);
	if(thread_enable == true)
	{
		APP_PRINT_INFO0("Write exit code\n");
		c = BTA_THREAD_EXIT_CODE;
		result = write(sockfds[1], &c, 1);
		if (result < 0)
		{
			APP_PRINT_ERROR1("Couldn't send exit code, %s", strerror(errno));
			return 1;
		}
		else
		{
			APP_PRINT_INFO0("send exit code success");
			thread_enable = false;
			pthread_join(mesh_bta_tid, NULL);
			mesh_bta_deinit();
			return 0;
		}
	}
	else
	{
		return 0;
	}
}


int32_t mesh_init_ali(AppMeshBtEventCbk cb,uint32_t logLayer1,uint32_t logLayer2,uint32_t logLevel,int32_t maxNodeSize,uint16_t local_unicast_addr)
{
	int result;
	APPloglayer = 0;

	mesh_event_cb = cb;
	APPloglayer = (0x00000000FFFFFFFF | (((uint64_t)logLayer1) << 32));
	MESHloglayer = logLayer2;
	loglevel = logLevel;
	devnum = maxNodeSize;

	static uint32_t thread_enter_count = 0;
	thread_enter_count ++;
	APP_PRINT_INFO1("count of calling thread_enter func count %d\n",thread_enter_count);

	if(thread_enable == false)
	{
		result = mesh_bta_thread_create(local_unicast_addr);
		if (result < 0)
		{
			APP_PRINT_ERROR1("Couldn't creat mesh thread, %s", strerror(errno));
			return EXIT_FAILURE;
		}
		else
		{
			APP_PRINT_INFO0("creat mesh thread success");
			thread_enable = true;
		}
	}

	return 0;
}
void set_realtime_priority(void)
{
     int ret;
     pthread_t this_thread = pthread_self();
     struct sched_param params;
     int policy = 0;

     params.sched_priority = sched_get_priority_max(SCHED_FIFO);

     APP_PRINT_INFO1("Trying to set thread realtime prio %d", params.sched_priority);

     ret = pthread_setschedparam(this_thread, SCHED_FIFO, &params);
     if (ret != 0) {
         APP_PRINT_ERROR1("Unsuccessful in setting thread realtime prio, %s",
		  strerror(errno));
         return;
     }

     ret = pthread_getschedparam(this_thread, &policy, &params);
     if (ret != 0) {
         APP_PRINT_ERROR0("Couldn't retrieve real-time scheduling paramers");
         return;
     }

     if(policy != SCHED_FIFO) {
         APP_PRINT_WARN0("Scheduling is NOT SCHED_FIFO!");
     } else {
         APP_PRINT_INFO0("SCHED_FIFO OK");
     }

     APP_PRINT_INFO1("Thread priority is %d", params.sched_priority);
}

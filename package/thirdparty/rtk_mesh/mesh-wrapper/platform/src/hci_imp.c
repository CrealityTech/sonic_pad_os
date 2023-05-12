#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "trace.h"

#include "hci_imp.h"
#include "hci_mgmt.h"

#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BT_EVENT_MONITOR_NUM 256
#ifdef MESH_ANDROID
#include <sys/un.h>
#include <utils/Log.h>
#include <assert.h>

#define UNIX_DOMAIN "@/data/misc/bluedroid/rtkbt_service.sock"
#endif

static void* hci_handle;

static struct hci_id_array_t 
{
	unsigned int id;
	uint8_t event;
}hci_id[BT_EVENT_MONITOR_NUM];
static uint16_t hci_id_num;

static int socket_fd = -1;

#ifdef MESH_ANDROID
int hci_socket_connect() {
    struct sockaddr_un un;
    int len;

    DFU_PRINT_ERROR1(" &*& %s\n",__func__);
    APP_PRINT_ERROR1(" &*& %s\n",__func__);

    memset(&un, 0, sizeof(un));        /* fill socket address structure with our address */
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, UNIX_DOMAIN);
    un.sun_path[0]=0;
    len = offsetof(struct sockaddr_un, sun_path) + strlen(UNIX_DOMAIN);

    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	APP_PRINT_ERROR1("create socket fd %d\n",socket_fd);
	assert(socket_fd >= 0);
    if(socket_fd < 0)
    {
      DFU_PRINT_ERROR1("socket failed %s\n",strerror(errno));
	  APP_PRINT_ERROR1("socket failed %s\n",strerror(errno));
        //return -1;
      goto err1;
    }
    if(connect(socket_fd,(struct sockaddr *)&un, len) < 0)
    {
      DFU_PRINT_ERROR1("connect failed %s\n",strerror(errno));
	  APP_PRINT_ERROR1("connect failed %s\n",strerror(errno));
	  assert(false);
      goto err2;
    }

    return socket_fd;
err2:
  close(socket_fd);
  socket_fd = -1;
err1:
  return socket_fd;
}
void hci_socket_close(void )
{
	int ret = 0;
	ret = close(socket_fd);
	if(ret == -1)
	{
		APP_PRINT_ERROR1("close fd failed %s\n",strerror(errno));
	}
	APP_PRINT_ERROR1("close fd %d",socket_fd);
	socket_fd = -1;
}
#endif
bool hci_init(void *param)
{
	struct hci_mgmt *hci_mgmt = NULL;
	uint16_t index = *(uint16_t *)param;
	
	GAP_PRINT_ERROR1(" &*& %s\n",__func__);
	APP_PRINT_ERROR1(" &*& %s\n",__func__);

	if(hci_handle != NULL )
	{
		GAP_PRINT_ERROR0("mgmt_socket has already opened");
		return false;
	}

	hci_mgmt = hci_mgmt_new_default(index, socket_fd);
	if (!hci_mgmt) {
		GAP_PRINT_ERROR1("%s:Unable to open mgmt_socket",__func__);
		return false;
	}
	
	hci_handle = hci_mgmt;
	return true;
}

bool hci_event_register(uint8_t event, hci_callback_func_t callback, 
						void *user_data, hci_destroy_func_t destroy)
{
	uint16_t index;
	unsigned int id;
	if(hci_id_num >= BT_EVENT_MONITOR_NUM)
	{
		GAP_PRINT_ERROR0("can not register more event");
		return false;
	}
	for(index = 0; index < hci_id_num; index++ )
	{
		if(hci_id[index].event == event)
		{
			GAP_PRINT_ERROR1("event 0x%x has been registered",event);
			return false;
		}
	}
	id = hci_mgmt_register((struct hci_mgmt *)hci_handle, event,
						(bt_hci_callback_func_t)callback, 
						(struct hci_mgmt *)hci_handle,
						(bt_hci_destroy_func_t)destroy);
	if (!id) {
		GAP_PRINT_ERROR1("Reister hci mgmt failed, event 0x%x",event);
		return false;
	}
	hci_id[hci_id_num].event = event;
	hci_id[hci_id_num].id = id;
	hci_id_num ++;
	return true;
}

bool hci_event_unregister(uint8_t event )
{
	uint16_t index;
	bool ret = false;
	for(index = 0; index < hci_id_num; index ++ )
	{
		if(hci_id[index].event == event )
		{
			break;
		}
	}
	if(index >= hci_id_num)
	{
		GAP_PRINT_ERROR1("event 0x%x is not registered",event);
		return false;
	}
	ret = hci_mgmt_unregister((struct hci_mgmt *)hci_handle, hci_id[index].id);
	if(ret )
	{
		for(; index < (hci_id_num-1); index ++)
		{
			hci_id[index]= hci_id[index+1];
		}
		hci_id_num --;
	}
	return ret;
}
unsigned int hci_cmd_send( uint16_t opcode,
			   const void *param, uint8_t size,
			   hci_callback_func_t callback, void *user_data,
			   hci_destroy_func_t destroy)
{
	return hci_mgmt_send((struct hci_mgmt *)hci_handle, opcode,
			   param, size,(bt_hci_callback_func_t) callback, user_data,
			   (bt_hci_destroy_func_t) destroy);
	
}

bool hci_deinit(void )
{
	hci_mgmt_release((struct hci_mgmt *)hci_handle);
	hci_id_num = 0;
	hci_handle = NULL;
	return true;
}
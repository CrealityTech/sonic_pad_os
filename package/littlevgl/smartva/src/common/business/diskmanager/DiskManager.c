#include <sys/epoll.h>
#include <netinet/in.h> 
#include <linux/netlink.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/vfs.h> 
#include <sys/mount.h>
#include <pthread.h>
#include "DiskManager.h"

#define MAX_EPOLL_EVENTS 40
#define UEVENT_MSG_LEN 2048
#define SYS_BLOCK_DEV_PATH "/sys/block"
#define FSYS_ATTR_DIRECTORY	(4)

#define MOUNTPOINT_MAX_LENGTH	(64)
#define USB_MASSSTORAGE_MAX_MOUNTPOINT	(2)
#define SDCARD_MAX_MOUNTPOINT	(2)
//#define DISKMANAGER_DEBUG
#define MMC_DEVICE_MAJOR	(179)
#define USB_DEVICE_MAJOR	(8)
#ifdef DISKMANAGER_DEBUG
#define LOG_HERE(fmt, ...) do { \
						printf("%s %d %s \n",__FILE__,__LINE__,__func__);\
						}\
						while(0)
#define LOG_INFO(fmt, ...) do { \
						printf("%s %d %s ",__FILE__,__LINE__,__func__);\
						printf(fmt,##__VA_ARGS__);}\
						while(0)
#define LOG_ERR(fmt, ...) do { \
						printf("%s %d %s ",__FILE__,__LINE__,__func__);\
						printf(fmt,##__VA_ARGS__);}\
						while(0)
#else
#define LOG_HERE(fmt, ...) \
			do {;}while(0)

#define LOG_INFO(fmt, ...) \
			do {;} while (0)

#define LOG_ERR(fmt, ...) do { \
						printf("%s %d %s ",__FILE__,__LINE__,__func__);\
						printf(fmt,##__VA_ARGS__);}\
						while(0)
#endif
typedef struct _DiskManager_CallBack {
	RegisterInfo_t RegisterInfo;
	struct _DiskManager_CallBack *Next;
} DiskManager_CallBack_t;

typedef struct _MountPoint_Info {
	char NeedCreate_Clean;
	char MountPoint[MOUNTPOINT_MAX_LENGTH];
} MountPoint_Info_t;

static int uevent_fd = 0;
static int epollfd = 0;
static int eventct = 0;
static pthread_mutex_t DiskManager_Mutex;
static DiskManager_CallBack_t DiskManager_Message_List_Head;
static DiskInfo_t DiskManager_List_Head;

static MountPoint_Info_t UsbMountPoint[USB_MASSSTORAGE_MAX_MOUNTPOINT] = {
	{0, "/mnt/exUDISK"},
	{1, "/tmp/exUDISK"}
};
static MountPoint_Info_t SdcardMountPoint[SDCARD_MAX_MOUNTPOINT] = {
	{0, "/mnt/SDCARD"},
	{1, "/tmp/SDCARD"}
};
	
static int DiskManager_MountDevice(DiskInfo_t *DeviceInfo);
static int DiskManager_UnMountDevice(DiskInfo_t *DeviceInfo);
static int DiskManager_BootCheck(void);
static int DiskManager_Clean_MountPoint(void);

static ssize_t uevent_kernel_multicast_uid_recv(int socket, void *buffer, size_t length, uid_t * user)
{
	struct iovec iov = { buffer, length };
	struct sockaddr_nl addr;
	int len = 0;
#ifdef struct_ucred	
	char control[CMSG_SPACE(sizeof(struct ucred))] = {0};
#endif
	struct msghdr hdr = {
		&addr,
		sizeof(addr),
		&iov,
		1,
	};
#ifdef struct_ucred	
	hdr.msg_control = (void *)control;
	hdr.msg_controllen = sizeof(control);
#endif
	*user = -1;
	ssize_t n = recvmsg(socket, &hdr, 0);
	if (n <= 0) {
		LOG_INFO("there\n");
		return n;
	}
	return n;
}
static ssize_t uevent_kernel_multicast_recv(int socket, void *buffer, size_t length)
{
	uid_t user = -1;
	return uevent_kernel_multicast_uid_recv(socket, buffer, length, &user);
}
static void uevent_event(int arg)
{
	int recv_len = 0, Index = 0, add = 0, remove = 0;
	char msg[UEVENT_MSG_LEN + 2] = {0}, *cp = NULL;
	DiskInfo_t DeviceInfo;

	recv_len = uevent_kernel_multicast_recv(uevent_fd, msg, UEVENT_MSG_LEN);
	if (recv_len <= 0){
		LOG_INFO("there\n");
		return;
	}		
	if (recv_len >= UEVENT_MSG_LEN){
		LOG_INFO("there\n");
		return;
	}
	msg[recv_len] = '\0';
	msg[recv_len + 1] = '\0';
	cp = msg;
	
	/*此处只关心block设备插入情况*/
	if (!strncmp(msg, "add@/block/", strlen("add@/block/")) || !strncmp(msg, "remove@/block/", strlen("remove@/block/"))) {
		memset(&DeviceInfo, 0x00, sizeof(DeviceInfo));
		for (Index = 0; Index < recv_len; Index++) {
			if (!strncmp(cp, "ACTION=", strlen("ACTION="))) {
				if (!strncmp(cp, "ACTION=add", strlen("ACTION=add"))) {
					add = 1;
					remove = 0;
				} else if (!strncmp(cp, "ACTION=remove", strlen("ACTION=remove"))) {
					add = 0;
					remove = 1;
				}
				cp += strlen(cp) + 1;
				continue;
			} else if (!strncmp(cp, "SUBSYSTEM=", strlen("SUBSYSTEM="))) {
				/*DEVTYPE=partition 此时才会挂载成功*/
				if (!strncmp(cp, "SUBSYSTEM=block", strlen("SUBSYSTEM=block"))) {
					cp += strlen(cp) + 1;
					continue;
				} else {
					goto finish;
				}
//			} else if (!strncmp(cp, "DEVTYPE=", strlen("DEVTYPE="))) {
//				/*DEVTYPE=partition 此时才会挂载成功*/
//				if (!strncmp(cp, "DEVTYPE=partition", strlen("DEVTYPE=partition"))) {
//					cp += strlen(cp) + 1;
//					continue;
//				} else {
//					goto finish;
//				}
			} else if (!strncmp(cp, "DEVNAME=", strlen("DEVNAME="))) {
				sprintf(DeviceInfo.DeviceName, "/dev/%s", cp + strlen("DEVNAME="));
				cp += strlen(cp) + 1;
				continue;
			} else if (!strncmp(cp, "MAJOR=", strlen("MAJOR="))) {
				DeviceInfo.Major = (__u32 )*(cp + strlen("MAJOR="));
				cp += strlen(cp) + 1;
				if (DeviceInfo.Major == USB_DEVICE_MAJOR) {
					DeviceInfo.MediaType = MEDIA_USB_MASSSTORAGE;
				} else if (DeviceInfo.Major == MMC_DEVICE_MAJOR) {
					DeviceInfo.MediaType = MEDIA_SD_CARD;
				}
				LOG_INFO("DeviceInfo.Major:%d\n", DeviceInfo.Major);
				continue;
			} else if (!strncmp(cp, "MINOR=", strlen("MINOR="))) {
				DeviceInfo.Minor = (__u32 )*(cp + strlen("MINOR="));
				cp += strlen(cp) + 1;
				LOG_INFO("DeviceInfo.Minor:%d\n", DeviceInfo.Minor);
				continue;
			}
			cp++;
		}
		if (add == 1) {
			/*等待Mount 成功*/
			DiskManager_MountDevice(&DeviceInfo);
		} else {
			DiskManager_UnMountDevice(&DeviceInfo);
		}
	}
finish:
	add = 0;
	remove = 0;
	return ;
}
static int uevent_open_socket(int buf_sz)
{
	struct sockaddr_nl addr;
	int on = 1;
	int s;

	memset(&addr, 0, sizeof(addr));
	addr.nl_family = AF_NETLINK;
	addr.nl_pid = getpid();
	addr.nl_groups = 0xffffffff;
	s = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (s < 0)
		return -1;

	setsockopt(s, SOL_SOCKET, SO_RCVBUFFORCE, &buf_sz, sizeof(buf_sz));
	setsockopt(s, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on));
	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		close(s);
		return -1;
	}
	return s;
}
static int healthd_register_event(int fd, void (*handler) (int))
{
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.ptr = (void *)handler;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		LOG_ERR("epoll_ctl failed; errno=%d\n", errno);
		return -1;
	}

	eventct++;
	return 0;
}
static void uevent_init(void)
{
	uevent_fd = uevent_open_socket(64 * 1024);

	if (uevent_fd < 0) {
		LOG_ERR("uevent_init: uevent_open_socket failed\n");
		return;
	}
	fcntl(uevent_fd, F_SETFL, O_NONBLOCK);
	if (healthd_register_event(uevent_fd, uevent_event)) {
		LOG_ERR("register for uevent events failed\n");
		return ;
	}

}
static void *healthd_mainloop(void *arg){

	LOG_INFO("healthd_mainloop start\n");
	DiskManager_Clean_MountPoint();
	DiskManager_BootCheck();
	while (1) {
		struct epoll_event events[eventct];
		int nevents;
		int mode_timeout;
		nevents = epoll_wait(epollfd, events, eventct, -1);
		if (nevents == -1) {
			if (errno == EINTR)
				continue;
			LOG_ERR("healthd_mainloop: epoll_wait failed\n");
			break;
		}

		for (int n = 0; n < nevents; ++n) {
			if (events[n].data.ptr)
				(*(void (*)(int))events[n].data.ptr) (events[n].events);
		}
	}
	LOG_INFO("healthd_mainloop end\n");	
}
static int _DiskManager_BootCheck(char *FilePath) {
	FILE *fp = NULL;
	int Major = 0, Minor = 0;
	char buf[256] = {0};
	DiskInfo_t DeviceInfo;
	
	fp = fopen(FilePath, "r");		
	if (NULL == fp) {
		LOG_INFO("failed to open %s\n", buf);
		return -1;
	}
	
	memset(&DeviceInfo, 0x00, sizeof(DeviceInfo));
	while(!feof(fp))
	{
		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf) - 1, fp); // 包含了换行符		
		LOG_INFO("buf:%s\n", buf);
//		if (!strncmp(buf, "DEVTYPE=partition", strlen("DEVTYPE=partition"))) {	
//			DiskManager_MountDevice(&DeviceInfo);
//			fclose(fp);
//			return 0;
//		} else 
		if (!strncmp(buf, "DEVNAME=", strlen("DEVNAME="))) {
			strncpy(DeviceInfo.DeviceName, "/dev/", strlen("/dev/"));
			if (strchr(buf, '\n') != NULL) {
				strncat(DeviceInfo.DeviceName, buf + strlen("DEVNAME="), strchr(buf, '\n') - (buf + strlen("DEVNAME=")));
			} else {
				LOG_ERR("strchr(buf, '\n') null\n");				
				fclose(fp);
				return 0;
			}
			DiskManager_MountDevice(&DeviceInfo);
			fclose(fp);
			return 0;
		} else if (!strncmp(buf, "MAJOR=", strlen("MAJOR="))) {
			DeviceInfo.Major = (__u32 )*(buf + strlen("MAJOR="));
			if (DeviceInfo.Major == USB_DEVICE_MAJOR) {
				DeviceInfo.MediaType = MEDIA_USB_MASSSTORAGE;
			} else if (DeviceInfo.Major == MMC_DEVICE_MAJOR) {
				DeviceInfo.MediaType = MEDIA_SD_CARD;
			}
		} else if (!strncmp(buf, "MINOR=", strlen("MINOR="))) {
			DeviceInfo.Minor = (__u32 )*(buf + strlen("MINOR="));
		}
	}
	fclose(fp);
}
static int DiskManager_BootCheck(void) {
	DIR *pCurDir = NULL, *TmpDir = NULL;
	struct dirent *pDirent = NULL, *Tmp_Dirent = NULL;
	unsigned char buf[256] = {0};

	pCurDir = opendir(SYS_BLOCK_DEV_PATH);
	if (NULL == pCurDir) {
		return -1;
	}
	while (NULL != (pDirent = readdir(pCurDir))) {
		int removable = 0;
		FILE *fp = NULL;
		
		if (pDirent->d_type == FSYS_ATTR_DIRECTORY && pDirent->d_name[0] != '.') {
			memset(buf, 0x00, sizeof(buf));
			sprintf(buf, "%s/%s/removable", SYS_BLOCK_DEV_PATH, (char*)pDirent->d_name);
			fp = fopen(buf, "r");		
			if (NULL == fp) {
				LOG_ERR("failed to open %s\n", buf);
				return -1;
			}
			fscanf(fp, "%d", &removable);
			fclose(fp);
			if (removable == 1) {
				memset(buf, 0x00, sizeof(buf));
				sprintf(buf, "%s/%s", SYS_BLOCK_DEV_PATH, (char*)pDirent->d_name);
				TmpDir = opendir(buf);
				if (NULL == TmpDir) {
					return -1;
				}
				while (NULL != (Tmp_Dirent = readdir(TmpDir))) {
					if (Tmp_Dirent->d_type == FSYS_ATTR_DIRECTORY && Tmp_Dirent->d_name[0] != '.' &&
						!strncmp((char*)pDirent->d_name, (char*)Tmp_Dirent->d_name, strlen((char*)pDirent->d_name))) {
						memset(buf, 0x00, sizeof(buf));
						sprintf(buf, "%s/%s/%s/%s", SYS_BLOCK_DEV_PATH, (char*)pDirent->d_name, (char*)Tmp_Dirent->d_name, "uevent");
						_DiskManager_BootCheck(buf);
					} else if ((Tmp_Dirent->d_type != FSYS_ATTR_DIRECTORY) && (strcmp(Tmp_Dirent->d_name, "uevent") == 0)){
						sprintf(buf, "%s/%s/%s", SYS_BLOCK_DEV_PATH, (char*)pDirent->d_name, (char*)Tmp_Dirent->d_name);
						_DiskManager_BootCheck(buf);
					}	
				}
			}
		}
	}
}
static int DiskManager_Check_StaticDisk(DiskInfo_t *StaticDiskInfo) {
	char buf[2048] = {0};
	
    FILE *fp = fopen("/proc/mounts", "r");
    if (NULL == fp) {
        LOG_ERR("failed to open /proc/mounts\n");
        return 1;
    }
    while(!feof(fp))
    {
        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf) - 1, fp); // 包含了换行符
		LOG_INFO("buf:%s\n", buf);
		if (!strncmp(buf, StaticDiskInfo->DeviceName, strlen(StaticDiskInfo->DeviceName)))	{
			fclose(fp);
			return 0;
		}
    }
	LOG_INFO("buf end\n");
	fclose(fp);
	return -1;
}

static void DiskManager_Add_Info(DiskInfo_t *DeviceInfo) {
	DiskInfo_t *DiskManager_tmp = &DiskManager_List_Head;
	DiskManager_CallBack_t *DiskManager_Message = DiskManager_Message_List_Head.Next;
	LOG_INFO("DeviceInfo->DeviceName:%s\n", DeviceInfo->DeviceName);
	LOG_INFO("DeviceInfo->MountPoint:%s\n", DeviceInfo->MountPoint);
	LOG_INFO("DeviceInfo->MediaType:%d\n", DeviceInfo->MediaType);
	LOG_INFO("DeviceInfo->Major:%d\n", DeviceInfo->Major);
	LOG_INFO("DeviceInfo->Minor:%d\n", DeviceInfo->Minor);
	pthread_mutex_lock(&DiskManager_Mutex);
	while (DiskManager_tmp->Next) {
		DiskManager_tmp = DiskManager_tmp->Next;
	}
	DiskManager_tmp->Next = DeviceInfo;
	DeviceInfo->Next = NULL;
	DeviceInfo->Prev = DiskManager_tmp;
	
	DiskManager_tmp = &DiskManager_List_Head;
	while (DiskManager_tmp->Next) {
		DiskManager_tmp = DiskManager_tmp->Next;
		LOG_INFO("DeviceName:%s\n", DiskManager_tmp->DeviceName);
		LOG_INFO("MountPoint:%s\n", DiskManager_tmp->MountPoint);
	}
	pthread_mutex_unlock(&DiskManager_Mutex);
	while (DiskManager_Message) {		
		LOG_INFO("Send Message To %s\n", DiskManager_Message->RegisterInfo.Cur_Win);
		DiskManager_Message->RegisterInfo.CallBackFunction(DeviceInfo);
		DiskManager_Message = DiskManager_Message->Next;
	}
}
static void DiskManager_Del_Info(DiskInfo_t *DeviceInfo) {
	DiskInfo_t *Prev = NULL, *Next = NULL;

	LOG_INFO("DeviceInfo->DeviceName:%s\n", DeviceInfo->DeviceName);
	LOG_INFO("DeviceInfo->MountPoint:%s\n", DeviceInfo->MountPoint);
	LOG_INFO("DeviceInfo->MediaType:%d\n", DeviceInfo->MediaType);
	LOG_INFO("DeviceInfo->Major:%d\n", DeviceInfo->Major);
	LOG_INFO("DeviceInfo->Minor:%d\n", DeviceInfo->Minor);
	if (DeviceInfo != NULL) {
		pthread_mutex_lock(&DiskManager_Mutex);
		Prev = DeviceInfo->Prev;
		Next = DeviceInfo->Next;
		Prev->Next = Next;
		if (Next != NULL) {
			Next->Prev = Prev;
		}
		pthread_mutex_unlock(&DiskManager_Mutex);
	}	
}
static DiskInfo_t *DiskManager_Find_ItemInfo(DiskInfo_t *DeviceInfo) {
	DiskInfo_t *tmp = &DiskManager_List_Head, *disk_tmp = NULL;
	
	pthread_mutex_lock(&DiskManager_Mutex);
	while (tmp->Next) {
		disk_tmp = tmp->Next;
		
		LOG_INFO("DeviceInfo->DeviceName:%s\n", DeviceInfo->DeviceName);
		LOG_INFO("DeviceInfo->MountPoint:%s\n", DeviceInfo->MountPoint);
		LOG_INFO("DeviceInfo->MediaType:%d\n", DeviceInfo->MediaType);
		LOG_INFO("DeviceInfo->Major:%d\n", DeviceInfo->Major);
		LOG_INFO("DeviceInfo->Minor:%d\n", DeviceInfo->Minor);
		if (!strncmp(disk_tmp->DeviceName, DeviceInfo->DeviceName, strlen(DeviceInfo->DeviceName))) {
			LOG_INFO("dev:%s found\n", DeviceInfo->DeviceName);
			pthread_mutex_unlock(&DiskManager_Mutex);
			return disk_tmp;
		}
		tmp = tmp->Next;
	}
	pthread_mutex_unlock(&DiskManager_Mutex);
	LOG_INFO("dev:%s not found\n", DeviceInfo->DeviceName);
	return NULL;
}
static int DiskManager_Clean_MountPoint(void) {
	int Index = 0;
	FILE *fp = NULL;
	char buf[256] = {0};
	
	fp = fopen("/proc/sys/kernel/hotplug", "wb+");		
	if (NULL != fp) {
		strcpy(buf, "Not Need Mdev HotPlug");
		fwrite(buf, sizeof(char), strlen(buf), fp);
		fclose(fp);
	}
	
	for (;Index < sizeof(UsbMountPoint)/sizeof(UsbMountPoint[0]); Index++) {
		umount(UsbMountPoint[Index].MountPoint);
		if (UsbMountPoint[Index].NeedCreate_Clean) {
			char Cmd[64] = {0};
			sprintf(Cmd, "rm -rf %s", UsbMountPoint[Index].MountPoint);
			system(Cmd);
		}
	}

	Index = 0;
	for (;Index < sizeof(SdcardMountPoint)/sizeof(SdcardMountPoint[0]); Index++) {
		umount(SdcardMountPoint[Index].MountPoint);
		if (SdcardMountPoint[Index].NeedCreate_Clean) {
			char Cmd[64] = {0};
			sprintf(Cmd, "rm -rf %s", SdcardMountPoint[Index].MountPoint);
			system(Cmd);
		}
	}
}

static int DiskManager_MountPoint_Is_Used(const char *MountPoint) {
	DiskInfo_t *tmp = &DiskManager_List_Head, *disk_tmp = NULL;
	
	pthread_mutex_lock(&DiskManager_Mutex);
	while (tmp->Next) {
		disk_tmp = tmp->Next;
		
		if (!strncmp(disk_tmp->MountPoint, MountPoint, strlen(MountPoint))) {
			LOG_INFO("disk_tmp->DeviceName:%s\n", disk_tmp->DeviceName);
			LOG_INFO("disk_tmp->MountPoint:%s\n", disk_tmp->MountPoint);
			LOG_INFO("disk_tmp->MediaType:%d\n", disk_tmp->MediaType);
			LOG_INFO("disk_tmp->Major:%d\n", disk_tmp->Major);
			LOG_INFO("disk_tmp->Minor:%d\n", disk_tmp->Minor);
			pthread_mutex_unlock(&DiskManager_Mutex);
			return 1;
		}
		tmp = tmp->Next;
	}
	pthread_mutex_unlock(&DiskManager_Mutex);
	return 0;
}
static int DiskManager_GetMountPoint(DiskInfo_t *DeviceInfo) {
	int Index = 0;
	if (DeviceInfo->MediaType == MEDIA_USB_MASSSTORAGE) {
		for (;Index < sizeof(UsbMountPoint)/sizeof(UsbMountPoint[0]); Index++) {
			if (DiskManager_MountPoint_Is_Used(UsbMountPoint[Index].MountPoint) == 0) {
				if (UsbMountPoint[Index].NeedCreate_Clean) {
					char Cmd[64] = {0};
					sprintf(Cmd, "mkdir -p %s", UsbMountPoint[Index].MountPoint);
					system(Cmd);
				}
				strcpy(DeviceInfo->MountPoint, UsbMountPoint[Index].MountPoint);
				return 0;
			}
		}
	} else if (DeviceInfo->MediaType == MEDIA_SD_CARD) {
		for (;Index < sizeof(SdcardMountPoint)/sizeof(SdcardMountPoint[0]); Index++) {
			if (DiskManager_MountPoint_Is_Used(SdcardMountPoint[Index].MountPoint) == 0) {
				if (SdcardMountPoint[Index].NeedCreate_Clean) {
					char Cmd[64] = {0};
					sprintf(Cmd, "mkdir -p %s", SdcardMountPoint[Index].MountPoint);
					system(Cmd);
				}
				strcpy(DeviceInfo->MountPoint, SdcardMountPoint[Index].MountPoint);
				return 0;
			}
		}
	}
	return -1;
}
static int DiskManager_Del_MountPoint(DiskInfo_t *DeviceInfo){
	int Index = 0;
	if (DeviceInfo->MediaType == MEDIA_USB_MASSSTORAGE) {
		for (;Index < sizeof(UsbMountPoint)/sizeof(UsbMountPoint[0]); Index++) {
			if (strcmp(DeviceInfo->MountPoint, UsbMountPoint[Index].MountPoint) == 0) {
				if (UsbMountPoint[Index].NeedCreate_Clean) {
					char Cmd[64] = {0};
					sprintf(Cmd, "rm -rf %s", UsbMountPoint[Index].MountPoint);
					system(Cmd);
				}
				return 0;
			}
		}
	} else if (DeviceInfo->MediaType == MEDIA_SD_CARD) {
		for (;Index < sizeof(SdcardMountPoint)/sizeof(SdcardMountPoint[0]); Index++) {
			if (strcmp(DeviceInfo->MountPoint, UsbMountPoint[Index].MountPoint) == 0) {
				if (SdcardMountPoint[Index].NeedCreate_Clean) {
					char Cmd[64] = {0};
					sprintf(Cmd, "rm -rf %s", SdcardMountPoint[Index].MountPoint);
					system(Cmd);
				}
				return 0;
			}
		}
	}
	return -1;
}
static int DiskManager_Check_DiskMountStatus(DiskInfo_t *DeviceInfo) {
	char buf[2048] = {0};
	
    FILE *fp = fopen("/proc/mounts", "r");
    if (NULL == fp) {
        LOG_ERR("failed to open /proc/mounts\n");
        return 1;
    }
    while(!feof(fp))
    {
        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf) - 1, fp); // 包含了换行符
		LOG_INFO("buf:%s\n", buf);
		if (!strncmp(buf, DeviceInfo->DeviceName, strlen(DeviceInfo->DeviceName)))	{
			char str[64] = {0}, *temp;
			sprintf(str, "%s ", DeviceInfo->DeviceName);
			LOG_INFO("str:%s\n", str);
			if (strstr(buf, str) != NULL) {
				temp = buf + strlen(str);
				LOG_INFO("temp:%s\n", buf);
				LOG_INFO("strchr(temp, ' ') - temp:%d\n", strchr(temp, ' ') - temp);
				strncpy(DeviceInfo->MountPoint, temp, strchr(temp, ' ') - temp);
				LOG_INFO("MountPoint:%s\n", DeviceInfo->MountPoint);
				fclose(fp);
				return 1;
			}			
			fclose(fp);
			return 0;
		}
    }
	LOG_INFO("buf end\n");
	fclose(fp);
	return 0;
}

static int DiskManager_MountDevice(DiskInfo_t *DeviceInfo) {
	int ret = 0;
	DiskInfo_t *DiskManager_Item = NULL;
	
	if (DiskManager_Check_DiskMountStatus(DeviceInfo) == 0)
	{		
		if (DiskManager_GetMountPoint(DeviceInfo) != 0) {
			LOG_ERR("DiskManager_GetMountPoint fail\n");
			return -1;
		}
		LOG_INFO("MountPoint:%s\n", DeviceInfo->MountPoint);
		LOG_INFO("DeviceName:%s\n", DeviceInfo->DeviceName);
		ret = mount(DeviceInfo->DeviceName, DeviceInfo->MountPoint, "vfat", 0, NULL);
		if (ret != 0) {
			LOG_ERR("DeviceName:%s\n", DeviceInfo->DeviceName);
			LOG_ERR("MountPoint:%s\n", DeviceInfo->MountPoint);
			LOG_ERR("mount fail:%s\n", strerror(errno));		
			DiskManager_Del_MountPoint(DeviceInfo);
			return -1;
		} 
	}
	DiskManager_Item = (DiskInfo_t *)malloc(sizeof(DiskInfo_t));
	if (DiskManager_Item == NULL) {			
		umount(DeviceInfo->MountPoint);
		DiskManager_Del_MountPoint(DeviceInfo);
		LOG_ERR("malloc fail\n");
		return -1;
	}
	DeviceInfo->operate = MEDIA_PLUGIN;
	memset(DiskManager_Item, 0x00, sizeof(DiskInfo_t));
	memcpy(DiskManager_Item, DeviceInfo, sizeof(DiskInfo_t));
	DiskManager_Add_Info(DiskManager_Item);
	return 0;
}
static int DiskManager_UnMountDevice(DiskInfo_t *DeviceInfo) {
	int ret = 0;
	DiskInfo_t *DiskManager_Item = NULL;
	DiskManager_CallBack_t *DiskManager_Message = DiskManager_Message_List_Head.Next;

	DiskManager_Item = DiskManager_Find_ItemInfo(DeviceInfo);
	if (DiskManager_Item == NULL) {
		LOG_ERR("dev:%s not found\n", DeviceInfo->DeviceName);
		return -1;
	}
	while (DiskManager_Message) {
		DiskManager_Item->operate = MEDIA_PLUGOUT;
		LOG_INFO("Send Message To %s\n", DiskManager_Message->RegisterInfo.Cur_Win);
		DiskManager_Message->RegisterInfo.CallBackFunction(DiskManager_Item);
		DiskManager_Message = DiskManager_Message->Next;
	}	
	ret = umount(DiskManager_Item->MountPoint);
	if (ret != 0) {
		LOG_ERR("unmount fail\n");
		return -1;
	}
	DiskManager_Del_MountPoint(DiskManager_Item);
	DiskManager_Del_Info(DiskManager_Item);
	return 0;
}

int DiskManager_Register_StaticDisk(DiskInfo_t *DeviceInfo) {
	int ret = 0;
	DiskInfo_t *DiskManager_Item = NULL;

	if (DeviceInfo == NULL) {
		LOG_ERR("Paramter Error\n");
		return -1;
	}
	
	DiskManager_Item = DiskManager_Find_ItemInfo(DeviceInfo);
	if (DiskManager_Item != NULL) {
		LOG_ERR("dev:%s exist\n", DeviceInfo->DeviceName);
		return -1;
	}
	ret = DiskManager_Check_StaticDisk(DeviceInfo);
	if (ret == -1) {
		LOG_ERR("dev:%s not mount\n", DeviceInfo->DeviceName);
		return -1;
	}
	DiskManager_Item = (DiskInfo_t *)malloc(sizeof(DiskInfo_t));
	memset(DiskManager_Item, 0x00, sizeof(DiskInfo_t));
	memcpy(DiskManager_Item, DeviceInfo, sizeof(DiskInfo_t));
	DiskManager_Add_Info(DiskManager_Item);
	return 0;
}
int DiskManager_UnRegister_StaticDisk(DiskInfo_t *DeviceInfo) {
	DiskInfo_t *DiskManager_Item = NULL;

	if (DeviceInfo == NULL) {
		LOG_ERR("Paramter Error\n");
		return -1;
	}
	
	DiskManager_Item = DiskManager_Find_ItemInfo(DeviceInfo);
	if (DiskManager_Item == NULL) {
		LOG_ERR("dev:%s not exist\n", DeviceInfo->DeviceName);
		return -1;
	}
	DiskManager_Del_Info(DiskManager_Item);
	return 0;
}
unsigned int DiskManager_GetDiskNum(void) {
	unsigned int count = 0;
	DiskInfo_t *tmp = &DiskManager_List_Head;
	
	pthread_mutex_lock(&DiskManager_Mutex);
	while (tmp->Next) {
		tmp = tmp->Next;
		count++;
	}
	pthread_mutex_unlock(&DiskManager_Mutex);
	return count;
}
DiskInfo_t *DiskManager_GetDiskInfoByIndex(unsigned int Index) {
	unsigned int i = 0;
	DiskInfo_t *tmp = &DiskManager_List_Head;
	
	pthread_mutex_lock(&DiskManager_Mutex);
	while (tmp->Next) {
		tmp = tmp->Next;
		if (i == Index) {
			break;
		}
		i++;
	}
	pthread_mutex_unlock(&DiskManager_Mutex);
	if (i < Index) {
		return NULL;
	} else {
		return tmp;
	}
}
int DiskManager_Register(RegisterInfo_t *RegisterInfo) {
	
	if (RegisterInfo != NULL) {
		DiskManager_CallBack_t *Tmp_CallBack = &DiskManager_Message_List_Head;
		while (Tmp_CallBack->Next != NULL) {
			Tmp_CallBack = Tmp_CallBack->Next;
		}
		Tmp_CallBack->Next = (DiskManager_CallBack_t *)malloc(sizeof(DiskManager_CallBack_t));
		memset(Tmp_CallBack->Next, 0x00, sizeof(DiskManager_CallBack_t));
		Tmp_CallBack->Next->RegisterInfo.CallBackFunction = RegisterInfo->CallBackFunction;
		LOG_INFO("Register %s CallBack!\n", RegisterInfo->Cur_Win);
		strcpy(Tmp_CallBack->Next->RegisterInfo.Cur_Win, RegisterInfo->Cur_Win);
		Tmp_CallBack->Next->Next = NULL;		
		return 0;
	} else {
		LOG_ERR("Paramter Error!\n");
		return -1;
	}
}
int DiskManager_UnRegister(RegisterInfo_t *RegisterInfo) {
	
	if (RegisterInfo != NULL) {
		DiskManager_CallBack_t *Tmp_CallBack = &DiskManager_Message_List_Head, *temp = NULL;
		while (Tmp_CallBack->Next != NULL) {
			if (!strcmp(Tmp_CallBack->Next->RegisterInfo.Cur_Win, RegisterInfo->Cur_Win)) {
				temp = Tmp_CallBack->Next;
				Tmp_CallBack->Next = Tmp_CallBack->Next->Next;
				LOG_INFO("UnRegister %s CallBack!\n", RegisterInfo->Cur_Win);
				free(temp);
				return 0;
			}
			Tmp_CallBack = Tmp_CallBack->Next;
		}		
		LOG_ERR("Not Found %s CallBack!\n", RegisterInfo->Cur_Win);
		return -1;
	} else {
		LOG_ERR("Paramter Error!\n");
		return -1;
	}
}

int DiskManager_Init(void) {
	int ret = 0;	
	pthread_t wait_pthread;
	
	memset(&DiskManager_List_Head, 0x00, sizeof(DiskManager_List_Head));
	memset(&DiskManager_Message_List_Head, 0x00, sizeof(DiskManager_Message_List_Head));
	pthread_mutex_init(&DiskManager_Mutex,NULL);

	epollfd = epoll_create(MAX_EPOLL_EVENTS);
	if (epollfd == -1) {
		LOG_INFO("epoll_create failed; errno=%d\n", errno);
		return -1;
	}
	uevent_init();	
	ret = pthread_create(&wait_pthread, NULL, &healthd_mainloop, NULL);
	if (ret < 0){
		LOG_INFO("pthread_create error\n");
		return -1;
	}
}

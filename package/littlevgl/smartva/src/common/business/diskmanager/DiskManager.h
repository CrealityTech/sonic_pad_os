#ifndef __DEVICE_CHECK_H__
#define __DEVICE_CHECK_H__
typedef enum {
	MEDIA_USB_MASSSTORAGE = 0,
	MEDIA_SD_CARD
}Media_type_t;

typedef enum {
	MEDIA_PLUGIN = 0,
	MEDIA_PLUGOUT
}Media_operate_t;

typedef struct _DiskInfo_tag {
	unsigned int Major;
	unsigned int Minor;	
	Media_operate_t operate;
	Media_type_t MediaType;
	char DeviceName[64];
	char MountPoint[64];
	struct _DiskInfo_tag *Prev;
	struct _DiskInfo_tag *Next;
} DiskInfo_t;

typedef void (*DiskManager_CallBack)(DiskInfo_t *);
typedef struct _RegisterInfo {
	char Cur_Win[64];
	DiskManager_CallBack CallBackFunction;
} RegisterInfo_t;

int DiskManager_Init(void);
unsigned int DiskManager_GetDiskNum(void);
DiskInfo_t *DiskManager_GetDiskInfoByIndex(unsigned int Index);
int DiskManager_Register_StaticDisk(DiskInfo_t *DeviceInfo);
int DiskManager_UnRegister_StaticDisk(DiskInfo_t *DeviceInfo);
int DiskManager_Register(RegisterInfo_t *RegisterInfo);
int DiskManager_UnRegister(RegisterInfo_t *RegisterInfo);

#endif

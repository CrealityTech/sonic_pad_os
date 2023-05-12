#include <stdio.h>
#include <stdlib.h>
#include <sys/statfs.h>
#include <alsa/asoundlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <signal.h>
#include "va_system.h"
#include "va_driver.h"

#define CARD_DEV_NODE1 "/dev/mmcblk0"
#define CARD_DEV_NODE2 "/dev/mmcblk0p1"
#define MOUNT_PATH "/mnt/SDCARD"
#define BACK_CAR_DEV "/dev/back_car"

typedef struct disk_cluster_sz_S
{
	unsigned	disk_sz_min ;	// unit:MByte
	unsigned	disk_sz_max ;
	unsigned	cluster_sz ;	// unit:Byte
}disk_cluster_sz_t;

static disk_cluster_sz_t disk_cluster_sz_array[] =
{
	{0L,	1024,	4096},			// <=1G , format to 4K
	{1024,	2048,	16384},			// >1G && <= 2G ,format to 16K
	{2048,	4096,	32768},			// >2G && <= 4G ,format to 32K
	{4096,	0x1ffffff,	65536},			// >4G , format to 64K
};


/* 1 tf卡存在，0不存在 */
int va_tfcard_is_exist(void)
{
	if(access(CARD_DEV_NODE1, F_OK) == 0)
	{
		return 1;
	}
	return 0;
}

/*1 挂载 成功，0 未挂载*/

int va_tfcard_is_mount_correct(void)
{
	FILE *fp = NULL;
	char buf[256];
	char *p = NULL;
	int  ret = 0;

	fp = fopen("/etc/mtab", "r");
	if (fp == NULL) {
		fprintf(stderr, "open /etc/mtab file err\n");
		return -1;
	}
	memset(buf, 0, sizeof(buf));
	while(fgets(buf, sizeof(buf), fp) != NULL){
		p = strstr(buf, CARD_DEV_NODE1);//p为str的出现位置,NULL则为没找到
		if(!p){
			memset(buf, 0, sizeof(buf));
			continue;
		}

		p = strstr(buf, "SDCARD");//p为str的出现位置,NULL则为没找到
		if(!p){
			memset(buf, 0, sizeof(buf));
			continue;
		}

		ret = 1;
		break;
    }

	fclose(fp);

	return ret;
}

int va_tfcard_get_total_size(void)
{
	struct statfs diskInfo;
	int size;

	if(!va_tfcard_is_mount_correct())
		return -1;

	statfs(MOUNT_PATH, &diskInfo);

	unsigned long long blocksize = diskInfo.f_bsize;    //每个block里包含的字节数
	unsigned long long totalsize = blocksize * diskInfo.f_blocks;   //总的字节数，f_blocks为block的数目

	size  = totalsize>>10;

    return size;
}

int va_tfcard_get_free_size(void)
{
	struct statfs diskInfo;
	int size;

	if(!va_tfcard_is_mount_correct())
		return -1;

	statfs(MOUNT_PATH, &diskInfo);

	unsigned long long blocksize = diskInfo.f_bsize;    //每个block里包含的字节数
	unsigned long long freeDisk  = blocksize * diskInfo.f_bfree;   //总的字节数，f_blocks为block的数目

	size  = freeDisk>>10;

	return size;
}

int va_tfcard_get_block_size(void)
{
	struct statfs diskInfo;
	int size;

	if(!va_tfcard_is_mount_correct())
		return -1;

	statfs(MOUNT_PATH, &diskInfo);

	return diskInfo.f_bsize;
}

//判断sd卡格式是否正确。-1 sd卡不存在,  0 格式不正确, 1 格式正确
int va_tfcard_format_is_correct(void)
{
	int total = 0;
	int i;
	int bsize = 0;

	if(!va_tfcard_is_mount_correct())
		return -1;

	total = va_tfcard_get_total_size();
	total >>= 10;
	bsize = va_tfcard_get_block_size();
	for(i = 0; i < sizeof(disk_cluster_sz_array)/sizeof(disk_cluster_sz_array[0]); i++){
		if(total > disk_cluster_sz_array[i].disk_sz_min && total < disk_cluster_sz_array[i].disk_sz_max){
			if(bsize == disk_cluster_sz_array[i].cluster_sz)
				return 1;
		}
	}

	return 0;
}

int va_tfcard_format(void)
{
	char com_line[64];
	char dev_node[32];
	int total = 0;
	int b_count = 0;
	int i = 0;

	if(!va_tfcard_is_mount_correct())
		return -1;

	total = va_tfcard_get_total_size();
	total >>= 10;

	for(i = 0; i < sizeof(disk_cluster_sz_array)/sizeof(disk_cluster_sz_array[0]); i++){
		if(total > disk_cluster_sz_array[i].disk_sz_min && total < disk_cluster_sz_array[i].disk_sz_max){
			b_count = disk_cluster_sz_array[i].cluster_sz/512;
		}
	}

	memset(com_line, 0, sizeof(com_line));
	memset(dev_node, 0, sizeof(dev_node));

	if(access(CARD_DEV_NODE2, F_OK) == 0){
		strcpy(dev_node, CARD_DEV_NODE2);
	}else{
		strcpy(dev_node, CARD_DEV_NODE1);
	}
	sprintf(com_line, "mkfs.vfat -s %d %s ", b_count, dev_node);
	system("umount /mnt/SDCARD");
	system(com_line);
	memset(com_line, 0, sizeof(com_line));
	sprintf(com_line, "mount %s %s ", dev_node, MOUNT_PATH);
	system(com_line);
	return 0;
}

int va_tfcard_fs_flex(void)
{
	if(access(CARD_DEV_NODE2, F_OK) == 0){
		system("fsck_msdos  -fp /dev/mmcblk0p1");
	}else if(access(CARD_DEV_NODE1, F_OK) == 0){
		system("fsck_msdos  -fp /dev/mmcblk0");
	}else{
		return -1;
	}
	return 0;
}

// 从机 u盘功能
int va_usb_storage_adcard_on(void)
{
	if(!va_tfcard_is_mount_correct())
		return -1;
	system("umount /mnt/SDCARD");
	system("echo 0 > /sys/class/android_usb/android0/enable");
	system("echo 18d1 > /sys/class/android_usb/android0/idVendor");
	system("echo 0001 > /sys/class/android_usb/android0/idProduct");
	system("echo mass_storage > /sys/class/android_usb/android0/functions");
	if(access(CARD_DEV_NODE2, F_OK) == 0){
		system("echo /dev/mmcblk0p1 > /sys/class/android_usb/android0/f_mass_storage/lun/file");
	}else if(access(CARD_DEV_NODE1, F_OK) == 0){
		system("echo /dev/mmcblk0 > /sys/class/android_usb/android0/f_mass_storage/lun/file");
	}
	system("echo 1 > /sys/class/android_usb/android0/enable");
	return 0;
}

int va_usb_storage_adcard_off(void)
{
	system("echo 0 > /sys/class/android_usb/android0/enable");
	system("echo 18d1 > /sys/class/android_usb/android0/idVendor");
	system("echo d002 > /sys/class/android_usb/android0/idProduct");
	system("echo adb > /sys/class/android_usb/android0/functions");
	system("echo 1 > /sys/class/android_usb/android0/enable");
	if(access(CARD_DEV_NODE2, F_OK) == 0){
		system("mount /dev/mmcblk0p1 /mnt/SDCARD/");
	}else if(access(CARD_DEV_NODE1, F_OK) == 0){
		system("mount /dev/mmcblk0 /mnt/SDCARD/");
	}
	return 0;
}

int va_rtc_get_local_time(struct tm *u_time)
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep); /*取得当地时间*/
	u_time->tm_year = 1900+p->tm_year;
	u_time->tm_mon = 1 + p->tm_mon;
	u_time->tm_mday = p->tm_mday;
	u_time->tm_hour = p->tm_hour;
	u_time->tm_min = p->tm_min;
	u_time->tm_sec = p->tm_sec;

	return 0;
}

int va_rtc_set_local_time(struct tm *u_time)
{
	char buff[64];
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "date -s \"%d-%d-%d %d:%d:%d\"", \
			u_time->tm_year, u_time->tm_mon, u_time->tm_mday, u_time->tm_hour, u_time->tm_min, u_time->tm_sec);
	system(buff);
	system("hwclock -w");
	return 0;
}

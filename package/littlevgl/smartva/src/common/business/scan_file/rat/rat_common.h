#ifndef __RAT_COMMON_H__
#define __RAT_COMMON_H__
#include	"../common.h"
#define RAT_MAX_FULL_PATH_LEN (768+1)

typedef	__hdle	HRAT;
typedef	HRAT HRATNPL;

/*!
* \brief
*     浏览的媒体类型
*/
typedef enum {
    RAT_MEDIA_TYPE_ALL = 0,     //所有媒体类型
	RAT_MEDIA_TYPE_PIC,         //所有图片文件
	RAT_MEDIA_TYPE_AUDIO,       //所有音频文件
	RAT_MEDIA_TYPE_VIDEO,       //所有视频文件	
	RAT_MEDIA_TYPE_EBOOK,       //所有电子书文件	
	RAT_MEDIA_TYPE_FIRMWARE,    //所有固件文件
	RAT_MEDIA_TYPE_UNKNOWN,
	RAT_MEDIA_TYPE_VIDEO_AND_AUDIO,	// 音视频文件
	RAT_MEDIA_TYPE_FOLDER,
	RAT_MEDIA_TYPE_SD_DEVICE,
	RAT_MEDIA_TYPE_LOCAL_DEVICE,
	RAT_MEDIA_TYPE_USB_DEVICE,
	RAT_MEDIA_TYPE_MAX,			//
}rat_media_type_t;

/* play mode */                  
typedef enum __rat_play_mode_e {
	RAT_PLAY_MODE_ONLY_ONCE = 0, 
	RAT_PLAY_MODE_ROTATE_ONE   ,
	RAT_PLAY_MODE_ROTATE_ALL   ,
	RAT_PLAY_MODE_SEQUENCE     ,
	RAT_PLAY_MODE_RANDOM       ,
	RAT_PLAY_MODE_MAX
} rat_play_mode_e;

typedef enum {
	RAT_LOCAL,          //本地磁盘
	RAT_TF,             //TF卡
	RAT_USB,            //USB设备
} Device_Type_t;

#define FSYS_ATTR_DIRECTORY	(4)
#define FSYS_ATTR_DIRECTORY	(4)
#define FSYS_ATTR_DIRECTORY	(4)

#endif//__RAT_COMMON_H__

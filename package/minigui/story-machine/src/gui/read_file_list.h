#ifndef _READ_FILE_LIST_H_
#define _READ_FILE_LIST_H_

enum mediaFileType{
	VIDEO_FILE_TYPE,
	AUDIO_FILE_TYPE,
	IMAGE_FILE_TYPE,
};


#define VIDEO_FILE_TYPE_NUM 11
#define AUDIO_FILE_TYPE_NUM 10
#define IMAGE_FILE_TYPE_NUM 5

#define FILE_TYPE_LEN 10
#define FILE_MAX_NUM   100
//#define FILE_BASIC_PATH "/mnt/SDCARD/lei_media_test/"
#define FILE_BASIC_PATH "/mnt/"

typedef enum _char_enc
{
    ENCODE_ISO_8859_1 = 0,
    ENCODE_USC_2 = 1,
    ENCODE_UTF_16_BE = 2,
    ENCODE_UTF_8 = 3,
    ENCODE_ASCII = 4,
	ENCODE_UNICODE = 5,
	ENCODE_UTF_32 = 6,
	ENCODE_GB2312 = 7,
    ENCODE_DEFAULT = ENCODE_UTF_8,
}char_enc;

typedef struct mediaFileInfo
{
	char path[512];
	char name[128];
	char lang;
	int  Num;
}mediaFileInfoType;

unsigned int getFilePath(enum mediaFileType FileType);

extern char *PlayerFilePath;
extern char *PlayerFileName;
extern char mediaFilePath[FILE_MAX_NUM][512];
extern char mediaFileName[FILE_MAX_NUM][512];
extern char mediaFileLang[FILE_MAX_NUM][1];

#endif

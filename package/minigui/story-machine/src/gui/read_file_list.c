#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "read_file_list.h"
#include "aw_list.h"


//struct mediaFileInfo FIlePath[1024];

AW_LIST_HEAD(file_list_head);

char mediaFilePath[FILE_MAX_NUM][512] = {0};
char mediaFileName[FILE_MAX_NUM][512] = {0};
char mediaFileLang[FILE_MAX_NUM][1] = {0};

char *PlayerFilePath;
char *PlayerFileName;

static unsigned int mediaFileNum = 0;

char videoFileTypeList[VIDEO_FILE_TYPE_NUM][FILE_TYPE_LEN] =
	{".avi",".mkv",".flv",".ts", ".mp4", ".3gp",".asf",".mpg",".mov",".vob",".m2ts"};
	//{".avi",".mkv",".flv",".ts", ".mp4", ".3gp",".webm",".asf",".mpg",".mov",".vob",".rmvb",".m2ts"};

char audioFileTypeList[AUDIO_FILE_TYPE_NUM][FILE_TYPE_LEN] =
	{".mp1",".mp2",".mp3",".ogg",".flac",".ape",".wav",".m4a",".amr",".aac"};
	//{".mp1",".mp2",".mp3",".ogg",".flac",".ape",".wav",".m4a",".amr",".aac",".dts",".swf",".ra",".wma"};
char imageFileTypeList[IMAGE_FILE_TYPE_NUM][FILE_TYPE_LEN] =
	{".jpg", ".jpeg", ".png", ".gif", ".bmp"};

static char_enc getFileLang(char *file)
{
	FILE *fp;
	char bom[4] = {0};
	fp = fopen(file, "r");
	char_enc lang;
	if(fp == NULL){
		printf("%s %d\n",__func__,__LINE__);
		lang = ENCODE_UTF_8;
		return lang;
	}

	fread(bom, 1, sizeof(bom), fp);
    if(bom[0] == 0xef && bom[1] == 0xbb && bom[2] == 0xbf){
        lang = ENCODE_UTF_8;
    }
    else if (bom[0] == 0xff && bom[1] == 0xfe){
		lang = ENCODE_UNICODE;
    }
    else if(bom[0] == 0xfe && bom[1] == 0xff) {
		lang = ENCODE_USC_2;
    }
    else if(bom[0] == 0 && bom[1] == 0 && bom[2] == 0xfe && bom[3] == 0xff)  {
		lang = ENCODE_UTF_32;
    }
	else if(bom[0] == 0xff && bom[1] == 0xf3) {
		lang = ENCODE_GB2312;
    }
	else{
		lang = ENCODE_DEFAULT;
    }
	fclose(fp);
	return lang;
}
static int readFileList(char *basePath, enum mediaFileType mideaType)
{
    DIR *dir;
    struct dirent *ptr;
    char base[512];
	char FIlePath[512] = {0};
	char *lastStrPos;
	char *fileType;
	int fileTypeNum;

	switch(mideaType){
		case VIDEO_FILE_TYPE:
			fileType = videoFileTypeList;
			fileTypeNum = VIDEO_FILE_TYPE_NUM;
			break;
		case AUDIO_FILE_TYPE:
			fileType = audioFileTypeList;
			fileTypeNum = AUDIO_FILE_TYPE_NUM;
			break;
		case IMAGE_FILE_TYPE:
			fileType = imageFileTypeList;
			fileTypeNum = IMAGE_FILE_TYPE_NUM;
			break;
		default:
			perror("donot support file type!!!");
			return -1;
	}

    if ((dir=opendir(basePath)) == NULL)
    {
        perror("Open dir error!!!");
        return -1;
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8){    ///file
			if((lastStrPos = strrchr(ptr->d_name,'.')) != NULL){
				int i = 0;
				for(i = 0; i < fileTypeNum; i++){
					if(!strncasecmp(lastStrPos, &fileType[i*FILE_TYPE_LEN],strlen(&fileType[i*FILE_TYPE_LEN]))){
					//	printf("find the matched type:%s %s %p %p\n", &videoFileTypeList[i], &fileType[i*FILE_TYPE_LEN], fileType, videoFileTypeList);
						if(mediaFileNum >= FILE_MAX_NUM){
							printf("File list num too many mediaFileNum = %ud \n",mediaFileNum);
							mediaFileNum = FILE_MAX_NUM;
							goto END;
						}

						sprintf(FIlePath, "%s/%s", basePath, ptr->d_name);
						//printf("d_name:%s %d\n",FIlePath, mediaFileNum);
						memset(mediaFilePath[mediaFileNum], 0, 512);
						strcpy(mediaFilePath[mediaFileNum], FIlePath);
						mediaFileLang[mediaFileNum][0] = getFileLang((char *)FIlePath);

						memset(mediaFileName[mediaFileNum], 0, 512);
						strcpy(mediaFileName[mediaFileNum], ptr->d_name);
						mediaFileNum++;
						break;
					}
				}
			}

		}
        else if(ptr->d_type == 10){    ///link file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
        }else if(ptr->d_type == 4){	    ///dir
			//printf("d_name:%s/%s\n",basePath,ptr->d_name);
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_name);
            readFileList(base, mideaType);
        }
    }
END:
    closedir(dir);
    return mediaFileNum;
}

unsigned int getFilePath(enum mediaFileType FileType)
{
	int i;
	for(i = 0; i < FILE_MAX_NUM; i++){
		memset(mediaFileName[i], 0, sizeof(mediaFileName[i]));
		memset(mediaFilePath[i], 0, sizeof(mediaFilePath[i]));
		memset(mediaFileLang[i], 0, sizeof(mediaFileLang[i]));
	}

	mediaFileNum = 0;
    return readFileList(FILE_BASIC_PATH, FileType);
}

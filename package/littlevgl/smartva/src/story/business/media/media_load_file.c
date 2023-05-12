#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "media_load_file.h"
#include "rat.h"
#include "debug.h"

static media_file_list_t meida_file_list;

media_file_list_t *media_load_file(rat_media_type_t media_type, char *path)
{	
	if(media_type != RAT_MEDIA_TYPE_AUDIO && media_type != RAT_MEDIA_TYPE_VIDEO){
		return NULL;
	}
	if(meida_file_list.media_type == media_type){
		return &meida_file_list;
	}

	debug_info("media is loading file...");

	memset(&meida_file_list, 0, sizeof(meida_file_list));

	meida_file_list.media_hrat = rat_open(path, media_type, 0); //È«²¿ËÑË÷
	if(!meida_file_list.media_hrat){
		debug_err("media:rat open failed!!!");
		return NULL;
	}
	meida_file_list.total_num = rat_get_cur_scan_cnt(meida_file_list.media_hrat);
	if(meida_file_list.total_num > MEDIA_FILE_LIST_MAX_NUM){
		debug_warn("media file too many");
		meida_file_list.total_num = MEDIA_FILE_LIST_MAX_NUM;
	}
	
	debug_info("media load file finish, total_num = %d", meida_file_list.total_num);
	meida_file_list.media_type = media_type;
	return &meida_file_list;
}

int media_unload_file(void)
{
	int ret = 0;
	if(meida_file_list.media_hrat){
		ret = rat_close(meida_file_list.media_hrat);
		memset(&meida_file_list, 0, sizeof(meida_file_list));
	}
	return ret;
}

char *media_get_path_to_name(char *path)
{
	char *next;
	char *pos;
	int i;

	if(!path){
		return NULL;
	}
	
	next = path;
	
	while(next){
		next = strchr(next, '.');
		if(next){
			pos = next;
			next++;
		}	
	}

	while(pos - path){
		pos--;
		if(*pos == '/'){
			return pos+1;
		}
	}
	return NULL;
}

char *media_get_file_path(int index)
{
	int ret;
	rat_entry_t ItemInfo;
	
	memset(&ItemInfo,0,sizeof(rat_entry_t));	
	ret = rat_get_item_info_by_index(meida_file_list.media_hrat, index, &ItemInfo); 
	if(ret < -1){
		return NULL;
	}
	return ItemInfo.Path;
}

char *media_get_next_file_path(void)
{
	int ret;
	rat_entry_t ItemInfo;

	memset(&ItemInfo,0,sizeof(rat_entry_t));
	ret = rat_move_cursor_forward(meida_file_list.media_hrat, 1);
	if(!ret){
		return NULL;
	}
	
	ret = rat_get_cur_item_info(meida_file_list.media_hrat, &ItemInfo);
	if(ret < -1){
		return NULL;
	}

	return ItemInfo.Path;
}

char *media_get_last_file_path(void)
{
	int ret;
	rat_entry_t ItemInfo;

	ret = rat_move_cursor_backward(meida_file_list.media_hrat, 1);
	if(!ret){
		return NULL;
	}
	
	ret = rat_get_cur_item_info(meida_file_list.media_hrat, &ItemInfo);
	if(ret < -1){
		return NULL;
	}
	
	return ItemInfo.Path;
}

unsigned int media_get_file_total_num(void)
{
	return meida_file_list.total_num;
}

void media_set_play_file_index(int index)
{
	if(index >= meida_file_list.total_num){
		meida_file_list.play_index = 0;
	}else if(index < 0){
		meida_file_list.play_index = meida_file_list.total_num;
	}else{
		meida_file_list.play_index = index;
	}
}

unsigned int media_get_play_file_index(void)
{
	return meida_file_list.play_index;
}

int find_music_lrc(char *basePath, char *lrc_name,char *lrc_path)
{
    DIR *dir;
    struct dirent *ptr;
    char base[512];
	static int ret = -1;

	if(!basePath){
		goto END;
	}

	if(!lrc_path){
		goto END;
	}

	if(!lrc_name){
		goto END;
	}

    if ((dir=opendir(basePath)) == NULL){
        perror("Open dir error!!!");
		goto END;
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8){    ///file ptr->d_name
			if(strcmp(ptr->d_name, lrc_name) == 0){
				sprintf(lrc_path, "%s/%s", basePath, ptr->d_name);
				ret = 0;
				break;
			}
		} else if(ptr->d_type == 10){    ///link file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
        }else if(ptr->d_type == 4){	    ///dir
			//printf("d_name:%s/%s\n",basePath,ptr->d_name);
            memset(base, '\0', sizeof(base));
            strcpy(base, basePath);
            strcat(base, "/");
            strcat(base, ptr->d_name);
            find_music_lrc(base, lrc_name, lrc_path);
        }
    }
END:
	if(dir)
		closedir(dir);
    return ret;
}



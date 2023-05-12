#ifndef __MEDIA_UI_H__
#define __MEDIA_UI_H__

#include <pthread.h>

#include "lvgl.h"
#include "page.h"
#include "ui_resource.h"
#include "player_int.h"
#include "media_load_file.h"
#include "DiskManager.h"
#include "debug.h"

typedef void (*media_task_func)(void *param);

#define FILE_NAME_LEN 256

#define FILE_PATH_LEN 512

typedef struct breaktag_s
{
	rat_media_type_t	fileType;
	int				offset; //offset for time
	int					index; // the index of file in list.
	char				filename[FILE_NAME_LEN];
}breaktag_s;

typedef struct PLAYER_UI_T{
	lv_task_t *			lv_task;
	player_context_t	*tplayer;
	pthread_mutex_t		mutex;
	
	media_file_list_t  	*media_list;

	char				*play_path;
	char				*play_name;
	int 				play_index;
	int					play_time;
	
	bool				list_loop;
	int					online_mode;
//disk insert and pullout	
	Media_type_t		disk_use;
	bool				disk_check[2];
	bool				insert_vaild[2];
	int					play_enable;
	
	breaktag_s			break_tag;

	media_task_func  fun;

}player_ui_t;


void media_ui_pthread_create(media_task_func fun);
void media_ui_pthread_destroy(void);
void media_func_register(media_task_func fun);
void media_func_unregister(void);

int media_play_next_or_last(const lv_obj_t * list, int next_last);
void media_play_event(lv_obj_t * btn, lv_event_t event);
void media_volume_bar_event(lv_obj_t * btn, lv_event_t event);
void media_progressbar_event(lv_obj_t * btn, lv_event_t event);
void media_loop_event(lv_obj_t * btn, lv_event_t event);
int media_list_loop(const lv_obj_t * list);
char *media_get_disk_mount_point(void);
int media_get_usb_status(void);
int media_get_tf_status(void);
int media_insert_play_enable(void);
void media_update_progressbar(lv_obj_t * btn, lv_obj_t *t_lable, lv_obj_t *c_lable);
void media_downloading(lv_obj_t * donwnload);
void media_play_file(int index);
int media_update_file_list(lv_obj_t * list, rat_media_type_t fileType, lv_event_cb_t event_cb);
#endif

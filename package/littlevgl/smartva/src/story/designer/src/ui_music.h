#ifndef __UI_MUSIC_H__
#define __UI_MUSIC_H__

#ifdef __cplusplus
extern "C" {
#endif

/**********************
 *      includes
 **********************/
#include "lvgl.h"


/**********************
 *       variables
 **********************/
typedef struct
{
	uint8_t id;
	lv_obj_t *cont;
	lv_obj_t *media_list;
	lv_obj_t *image_1;
	lv_obj_t *image_2;
	lv_obj_t *download;
	lv_obj_t *container_1;
	lv_obj_t *play;
	lv_obj_t *progressbar;
	lv_obj_t *volume_bar;
	lv_obj_t *curr_time;
	lv_obj_t *total_time;
	lv_obj_t *next;
	lv_obj_t *last;
	lv_obj_t *loop;
	lv_obj_t *volume;
	lv_obj_t *file_name;
	lv_obj_t *lrc;
	lv_obj_t *lrc_no;
	lv_obj_t *back;
} music_ui_t;


/**********************
 * functions
 **********************/
void music_auto_ui_create(music_ui_t *ui);
void music_auto_ui_destory(music_ui_t *ui);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__UI_MUSIC_H__*/

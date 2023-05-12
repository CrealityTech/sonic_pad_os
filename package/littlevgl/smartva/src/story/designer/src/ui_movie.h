#ifndef __UI_MOVIE_H__
#define __UI_MOVIE_H__

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
	lv_obj_t *image_4;
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
	lv_obj_t *download;
	lv_obj_t *label_1;
	lv_obj_t *label_2;
	lv_obj_t *order;
	lv_obj_t *full_button;
	lv_obj_t *back;
} movie_ui_t;


/**********************
 * functions
 **********************/
void movie_auto_ui_create(movie_ui_t *ui);
void movie_auto_ui_destory(movie_ui_t *ui);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__UI_MOVIE_H__*/

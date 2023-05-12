#ifndef __UI_HOME_H__
#define __UI_HOME_H__

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
	lv_obj_t *image_button_1;
	lv_obj_t *image_button_2;
	lv_obj_t *image_button_3;
	lv_obj_t *image_button_4;
	lv_obj_t *image_button_5;
	lv_obj_t *image_button_6;
	lv_obj_t *image_button_7;
	lv_obj_t *image_button_8;
	lv_obj_t *label_1;
	lv_obj_t *label_2;
	lv_obj_t *label_3;
	lv_obj_t *label_4;
	lv_obj_t *label_5;
	lv_obj_t *label_6;
	lv_obj_t *label_7;
	lv_obj_t *label_8;
	lv_obj_t *container_1;
	lv_obj_t *container_3;
	lv_obj_t *label_10;
	lv_obj_t *container_4;
	lv_obj_t *label_11;
	lv_obj_t *container_5;
	lv_obj_t *label_9;
	lv_obj_t *container_6;
	lv_obj_t *label_14;
	lv_obj_t *container_7;
	lv_obj_t *label_15;
	lv_obj_t *container_8;
	lv_obj_t *label_13;
	lv_obj_t *container_9;
	lv_obj_t *label_12;
	lv_obj_t *container_10;
	lv_obj_t *label_16;
	lv_obj_t *line_1;
	lv_obj_t *line_2;
	lv_obj_t *line_3;
	lv_obj_t *line_4;
	lv_obj_t *line_5;
	lv_obj_t *button_1;
} home_ui_t;


/**********************
 * functions
 **********************/
void home_auto_ui_create(home_ui_t *ui);
void home_auto_ui_destory(home_ui_t *ui);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__UI_HOME_H__*/

#ifndef __UI_SETTING_H__
#define __UI_SETTING_H__

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
	lv_obj_t *line_1;
	lv_obj_t *line_2;
	lv_obj_t *label_1;
	lv_obj_t *list_1;
	lv_obj_t *label_10;
	lv_obj_t *image_1;
	lv_obj_t *infomation_cont;
	lv_obj_t *container_1;
	lv_obj_t *label_28;
	lv_obj_t *label_29;
	lv_obj_t *label_30;
	lv_obj_t *label_31;
	lv_obj_t *label_32;
	lv_obj_t *label_33;
	lv_obj_t *line_19;
	lv_obj_t *line_20;
	lv_obj_t *container_3;
	lv_obj_t *label_34;
	lv_obj_t *label_35;
	lv_obj_t *language_cont;
	lv_obj_t *check_box_1;
	lv_obj_t *check_box_2;
	lv_obj_t *check_box_3;
	lv_obj_t *check_box_4;
	lv_obj_t *check_box_5;
	lv_obj_t *check_box_6;
	lv_obj_t *display_cont;
	lv_obj_t *container_8;
	lv_obj_t *slider_1;
	lv_obj_t *label_6;
	lv_obj_t *label_8;
	lv_obj_t *container_9;
	lv_obj_t *label_7;
	lv_obj_t *line_7;
	lv_obj_t *switch_2;
	lv_obj_t *label_9;
	lv_obj_t *label_12;
	lv_obj_t *label_13;
	lv_obj_t *slider_2;
	lv_obj_t *slider_3;
	lv_obj_t *slider_4;
	lv_obj_t *line_17;
	lv_obj_t *line_18;
	lv_obj_t *label_25;
	lv_obj_t *label_26;
	lv_obj_t *label_27;
	lv_obj_t *general_cont;
	lv_obj_t *line_5;
} setting_ui_t;


/**********************
 * functions
 **********************/
void setting_auto_ui_create(setting_ui_t *ui);
void setting_auto_ui_destory(setting_ui_t *ui);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__UI_SETTING_H__*/

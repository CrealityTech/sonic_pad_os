#ifndef __UI_FOLDER_H__
#define __UI_FOLDER_H__

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
	lv_obj_t *container_1;
	lv_obj_t *image_1;
	lv_obj_t *label_1;
	lv_obj_t *list_1;
	lv_obj_t *direct_cont;
	lv_obj_t *image_3;
	lv_obj_t *page_1;
	lv_obj_t *label_2;
	lv_obj_t *label_3;
	lv_obj_t *label_4;
	lv_obj_t *image_2;
	lv_obj_t *label_5;
	lv_obj_t *label_6;
	lv_obj_t *label_7;
	lv_obj_t *image_4;
} folder_ui_t;


/**********************
 * functions
 **********************/
void folder_auto_ui_create(folder_ui_t *ui);
void folder_auto_ui_destory(folder_ui_t *ui);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__UI_FOLDER_H__*/

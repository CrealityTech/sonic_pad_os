#ifndef __UI_EXPLORER_H__
#define __UI_EXPLORER_H__

#ifdef __cplusplus
extern "C" {
#endif

/**********************
 *      includes
 **********************/
#include "lvgl.h"
#include "rat_common.h"


/**********************
 *       variables
 **********************/
typedef struct
{
	uint8_t id;
	lv_obj_t *cont;
	lv_obj_t *list_2;
	lv_obj_t *medium_cont;
	lv_obj_t *medium_list_1;
	lv_obj_t *container_7;
	lv_obj_t *label_2;
	lv_obj_t *container_3;
	lv_obj_t *image_1;
	lv_obj_t *label_5;
	lv_obj_t *container_1;
	lv_obj_t *container_5;
	lv_obj_t *label_1;
	lv_obj_t *label_4;
	lv_obj_t *label_7;
	lv_obj_t *label_6;
	lv_obj_t *label_8;
	lv_obj_t *direct_cont;
	lv_obj_t *image_3;
	lv_obj_t *image_2;
} explorer_ui_t;


/**********************
 * functions
 **********************/
void explorer_auto_ui_create(explorer_ui_t *ui);
void explorer_auto_ui_destory(explorer_ui_t *ui);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__UI_EXPLORER_H__*/

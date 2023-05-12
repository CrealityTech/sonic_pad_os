#ifndef __UI_CALENDAR_H__
#define __UI_CALENDAR_H__

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
	lv_obj_t *calendar_1;
	lv_obj_t *container_1;
	lv_obj_t *label_1;
	lv_obj_t *image_1;
	lv_obj_t *container_2;
	lv_obj_t *label_2;
	lv_obj_t *label_3;
	lv_obj_t *label_4;
	lv_obj_t *image_2;
} calendar_ui_t;


/**********************
 * functions
 **********************/
void calendar_auto_ui_create(calendar_ui_t *ui);
void calendar_auto_ui_destory(calendar_ui_t *ui);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__UI_CALENDAR_H__*/

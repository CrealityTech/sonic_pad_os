#ifndef __UI_CALCULATOR_H__
#define __UI_CALCULATOR_H__

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
	lv_obj_t *image_1;
	lv_obj_t *text_area_1;
	lv_obj_t *keyboard_1;
} calculator_ui_t;


/**********************
 * functions
 **********************/
void calculator_auto_ui_create(calculator_ui_t *ui);
void calculator_auto_ui_destory(calculator_ui_t *ui);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__UI_CALCULATOR_H__*/

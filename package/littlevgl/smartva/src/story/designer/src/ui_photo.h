#ifndef __UI_PHOTO_H__
#define __UI_PHOTO_H__

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
} photo_ui_t;


/**********************
 * functions
 **********************/
void photo_auto_ui_create(photo_ui_t *ui);
void photo_auto_ui_destory(photo_ui_t *ui);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__UI_PHOTO_H__*/

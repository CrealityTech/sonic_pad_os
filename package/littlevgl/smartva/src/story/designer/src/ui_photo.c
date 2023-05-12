/**********************
 *      includes
 **********************/
#include "ui_photo.h"
#include "lvgl.h"
#include "common.h"
#include "ui_resource.h"


/**********************
 *       variables
 **********************/
static lv_style_t style_screen;

/**********************
 *  images and fonts
 **********************/
static void *image_1_calendar_set_png = NULL;

/**********************
 *  functions
 **********************/
void photo_auto_ui_create(photo_ui_t *ui)
{
	lv_style_copy(&style_screen, &lv_style_scr);
	style_screen.body.main_color = lv_color_hex(0x55aaff);
	style_screen.body.grad_color = lv_color_hex(0x55aaff);
	lv_obj_set_style(ui->cont, &style_screen);

#ifdef LV_USE_IMG
	ui->image_1 = lv_img_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_1, 220, 128);
	lv_obj_set_size(ui->image_1, 64, 64);
	image_1_calendar_set_png = (void *)parse_image_form_file(LV_IMAGE_PATH"calendar_set.png");
	lv_img_set_src(ui->image_1, image_1_calendar_set_png);

	lv_obj_set_drag(ui->image_1,true);
#endif // LV_USE_IMG

}

void photo_auto_ui_destory(photo_ui_t *ui)
{
	lv_obj_clean(ui->cont);
	free_image(image_1_calendar_set_png);
}

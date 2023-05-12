/**********************
 *      includes
 **********************/
#include "ui_calculator.h"
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
static void *image_1_home_return_png = NULL;

/**********************
 *  functions
 **********************/
void calculator_auto_ui_create(calculator_ui_t *ui)
{
	lv_style_copy(&style_screen, &lv_style_scr);
	style_screen.body.main_color = lv_color_hex(0x55aa7f);
	style_screen.body.grad_color = lv_color_hex(0x55aa7f);
	lv_obj_set_style(ui->cont, &style_screen);

#ifdef LV_USE_IMG
	ui->image_1 = lv_img_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_1, 18, 14);
	lv_obj_set_size(ui->image_1, 32, 32);
	image_1_home_return_png = (void *)parse_image_form_file(LV_IMAGE_PATH"home_return.png");
	lv_img_set_src(ui->image_1, image_1_home_return_png);

#endif // LV_USE_IMG

#ifdef LV_USE_TA
	ui->text_area_1 = lv_ta_create(ui->cont, NULL);
	lv_obj_set_pos(ui->text_area_1, 139, 18);
	lv_obj_set_size(ui->text_area_1, 510, 121);
	lv_ta_set_text(ui->text_area_1, "");
	lv_ta_set_placeholder_text(ui->text_area_1, "");
	lv_ta_set_cursor_type(ui->text_area_1, LV_CURSOR_NONE);
	lv_ta_set_cursor_blink_time(ui->text_area_1, 144);
	lv_ta_set_one_line(ui->text_area_1, false);
	lv_ta_set_pwd_mode(ui->text_area_1, false);
	lv_ta_set_max_length(ui->text_area_1, 0);
	lv_ta_set_text_align(ui->text_area_1, 0);
	lv_ta_set_sb_mode(ui->text_area_1, 2);
	lv_ta_set_scroll_propagation(ui->text_area_1, false);
	lv_ta_set_edge_flash(ui->text_area_1, false);
#endif // LV_USE_TA

#ifdef LV_USE_KB
	ui->keyboard_1 = lv_kb_create(ui->cont, NULL);
	lv_obj_set_pos(ui->keyboard_1, 139, 139);
	lv_obj_set_size(ui->keyboard_1, 510, 331);
	lv_kb_set_mode(ui->keyboard_1, LV_KB_MODE_NUM);
	lv_kb_set_cursor_manage(ui->keyboard_1, false);
#endif // LV_USE_KB

}

void calculator_auto_ui_destory(calculator_ui_t *ui)
{
	lv_obj_clean(ui->cont);
	free_image(image_1_home_return_png);
}

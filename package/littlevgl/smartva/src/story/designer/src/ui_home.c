/**********************
 *      includes
 **********************/
#include "ui_home.h"
#include "lvgl.h"
#include "common.h"
#include "ui_resource.h"


/**********************
 *       variables
 **********************/
static lv_style_t style_screen;
static lv_style_t style0_label_1;
static lv_style_t style0_label_2;
static lv_style_t style0_label_3;
static lv_style_t style0_label_4;
static lv_style_t style0_label_5;
static lv_style_t style0_label_6;
static lv_style_t style0_label_7;
static lv_style_t style0_label_8;
static lv_style_t style0_container_1;
static lv_style_t style0_container_3;
static lv_style_t style0_container_4;
static lv_style_t style0_container_5;
static lv_style_t style0_container_6;
static lv_style_t style0_container_7;
static lv_style_t style0_container_8;
static lv_style_t style0_container_9;
static lv_style_t style0_container_10;

/**********************
 *  images and fonts
 **********************/
static void *image_button_1_home_movie_png_state0 = NULL;
static void *image_button_1_home_movie_png_state1 = NULL;
static void *image_button_2_home_music_png_state0 = NULL;
static void *image_button_2_home_music_png_state1 = NULL;
static void *image_button_3_home_calendar_png_state0 = NULL;
static void *image_button_3_home_calendar_png_state1 = NULL;
static void *image_button_4_home_timer_png_state0 = NULL;
static void *image_button_4_home_timer_png_state1 = NULL;
static void *image_button_5_home_photo_png_state0 = NULL;
static void *image_button_5_home_photo_png_state1 = NULL;
static void *image_button_6_home_calculator_png_state0 = NULL;
static void *image_button_6_home_calculator_png_state1 = NULL;
static void *image_button_7_home_explorer_png_state0 = NULL;
static void *image_button_7_home_explorer_png_state1 = NULL;
static void *image_button_8_home_setting_png_state0 = NULL;
static void *image_button_8_home_setting_png_state1 = NULL;

/**********************
 *  functions
 **********************/
void home_auto_ui_create(home_ui_t *ui)
{
	lv_style_copy(&style_screen, &lv_style_scr);
	style_screen.body.main_color = lv_color_hex(0x55aa7f);
	style_screen.body.grad_color = lv_color_hex(0x55aa7f);
	lv_obj_set_style(ui->cont, &style_screen);

#ifdef LV_USE_IMGBTN
	ui->image_button_1 = lv_imgbtn_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_button_1, 112, 97);
	lv_obj_set_size(ui->image_button_1, 59, 60);
	lv_imgbtn_set_state(ui->image_button_1, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->image_button_1, false);
	image_button_1_home_movie_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_movie.png");
	lv_imgbtn_set_src(ui->image_button_1, LV_BTN_STATE_REL, image_button_1_home_movie_png_state0);
	image_button_1_home_movie_png_state1 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_movie.png");
	lv_imgbtn_set_src(ui->image_button_1, LV_BTN_STATE_PR, image_button_1_home_movie_png_state1);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_IMGBTN
	ui->image_button_2 = lv_imgbtn_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_button_2, 290, 97);
	lv_obj_set_size(ui->image_button_2, 59, 60);
	lv_imgbtn_set_state(ui->image_button_2, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->image_button_2, false);
	image_button_2_home_music_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_music.png");
	lv_imgbtn_set_src(ui->image_button_2, LV_BTN_STATE_REL, image_button_2_home_music_png_state0);
	image_button_2_home_music_png_state1 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_music.png");
	lv_imgbtn_set_src(ui->image_button_2, LV_BTN_STATE_PR, image_button_2_home_music_png_state1);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_IMGBTN
	ui->image_button_3 = lv_imgbtn_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_button_3, 470, 99);
	lv_obj_set_size(ui->image_button_3, 59, 60);
	lv_imgbtn_set_state(ui->image_button_3, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->image_button_3, false);
	image_button_3_home_calendar_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_calendar.png");
	lv_imgbtn_set_src(ui->image_button_3, LV_BTN_STATE_REL, image_button_3_home_calendar_png_state0);
	image_button_3_home_calendar_png_state1 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_calendar.png");
	lv_imgbtn_set_src(ui->image_button_3, LV_BTN_STATE_PR, image_button_3_home_calendar_png_state1);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_IMGBTN
	ui->image_button_4 = lv_imgbtn_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_button_4, 644, 99);
	lv_obj_set_size(ui->image_button_4, 59, 60);
	lv_imgbtn_set_state(ui->image_button_4, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->image_button_4, false);
	image_button_4_home_timer_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_timer.png");
	lv_imgbtn_set_src(ui->image_button_4, LV_BTN_STATE_REL, image_button_4_home_timer_png_state0);
	image_button_4_home_timer_png_state1 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_timer.png");
	lv_imgbtn_set_src(ui->image_button_4, LV_BTN_STATE_PR, image_button_4_home_timer_png_state1);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_IMGBTN
	ui->image_button_5 = lv_imgbtn_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_button_5, 110, 313);
	lv_obj_set_size(ui->image_button_5, 59, 60);
	lv_imgbtn_set_state(ui->image_button_5, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->image_button_5, false);
	image_button_5_home_photo_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_photo.png");
	lv_imgbtn_set_src(ui->image_button_5, LV_BTN_STATE_REL, image_button_5_home_photo_png_state0);
	image_button_5_home_photo_png_state1 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_photo.png");
	lv_imgbtn_set_src(ui->image_button_5, LV_BTN_STATE_PR, image_button_5_home_photo_png_state1);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_IMGBTN
	ui->image_button_6 = lv_imgbtn_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_button_6, 287, 313);
	lv_obj_set_size(ui->image_button_6, 59, 60);
	lv_imgbtn_set_state(ui->image_button_6, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->image_button_6, false);
	image_button_6_home_calculator_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_calculator.png");
	lv_imgbtn_set_src(ui->image_button_6, LV_BTN_STATE_REL, image_button_6_home_calculator_png_state0);
	image_button_6_home_calculator_png_state1 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_calculator.png");
	lv_imgbtn_set_src(ui->image_button_6, LV_BTN_STATE_PR, image_button_6_home_calculator_png_state1);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_IMGBTN
	ui->image_button_7 = lv_imgbtn_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_button_7, 474, 313);
	lv_obj_set_size(ui->image_button_7, 59, 60);
	lv_imgbtn_set_state(ui->image_button_7, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->image_button_7, false);
	image_button_7_home_explorer_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_explorer.png");
	lv_imgbtn_set_src(ui->image_button_7, LV_BTN_STATE_REL, image_button_7_home_explorer_png_state0);
	image_button_7_home_explorer_png_state1 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_explorer.png");
	lv_imgbtn_set_src(ui->image_button_7, LV_BTN_STATE_PR, image_button_7_home_explorer_png_state1);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_IMGBTN
	ui->image_button_8 = lv_imgbtn_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_button_8, 644, 314);
	lv_obj_set_size(ui->image_button_8, 59, 60);
	lv_imgbtn_set_state(ui->image_button_8, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->image_button_8, false);
	image_button_8_home_setting_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_setting.png");
	lv_imgbtn_set_src(ui->image_button_8, LV_BTN_STATE_REL, image_button_8_home_setting_png_state0);
	image_button_8_home_setting_png_state1 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_setting.png");
	lv_imgbtn_set_src(ui->image_button_8, LV_BTN_STATE_PR, image_button_8_home_setting_png_state1);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_1, &lv_style_transp);
	style0_label_1.text.color = lv_color_hex(0x000000);
	style0_label_1.text.line_space = 2;

	ui->label_1 = lv_label_create(ui->cont, NULL);
	lv_label_set_text(ui->label_1, "movie");
	lv_label_set_long_mode(ui->label_1, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_1, 121, 167);
	lv_obj_set_size(ui->label_1, 44, 19);
	lv_label_set_style(ui->label_1, LV_LABEL_STYLE_MAIN, &style0_label_1);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_2, &lv_style_transp);
	style0_label_2.text.color = lv_color_hex(0x000000);
	style0_label_2.text.line_space = 2;

	ui->label_2 = lv_label_create(ui->cont, NULL);
	lv_label_set_text(ui->label_2, "music");
	lv_label_set_long_mode(ui->label_2, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_2, 298, 168);
	lv_obj_set_size(ui->label_2, 43, 19);
	lv_label_set_style(ui->label_2, LV_LABEL_STYLE_MAIN, &style0_label_2);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_3, &lv_style_transp);
	style0_label_3.text.color = lv_color_hex(0x000000);
	style0_label_3.text.line_space = 2;

	ui->label_3 = lv_label_create(ui->cont, NULL);
	lv_label_set_text(ui->label_3, "calendar");
	lv_label_set_long_mode(ui->label_3, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_3, 468, 170);
	lv_obj_set_size(ui->label_3, 62, 19);
	lv_label_set_style(ui->label_3, LV_LABEL_STYLE_MAIN, &style0_label_3);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_4, &lv_style_transp);
	style0_label_4.text.color = lv_color_hex(0x000000);
	style0_label_4.text.line_space = 2;

	ui->label_4 = lv_label_create(ui->cont, NULL);
	lv_label_set_text(ui->label_4, "example");
	lv_label_set_long_mode(ui->label_4, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_4, 645, 170);
	lv_obj_set_size(ui->label_4, 62, 19);
	lv_label_set_style(ui->label_4, LV_LABEL_STYLE_MAIN, &style0_label_4);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_5, &lv_style_transp);
	style0_label_5.text.color = lv_color_hex(0x000000);
	style0_label_5.text.line_space = 2;

	ui->label_5 = lv_label_create(ui->cont, NULL);
	lv_label_set_text(ui->label_5, "photo");
	lv_label_set_long_mode(ui->label_5, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_5, 120, 381);
	lv_obj_set_size(ui->label_5, 41, 19);
	lv_label_set_style(ui->label_5, LV_LABEL_STYLE_MAIN, &style0_label_5);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_6, &lv_style_transp);
	style0_label_6.text.color = lv_color_hex(0x000000);
	style0_label_6.text.line_space = 2;

	ui->label_6 = lv_label_create(ui->cont, NULL);
	lv_label_set_text(ui->label_6, "calculator");
	lv_label_set_long_mode(ui->label_6, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_6, 283, 381);
	lv_obj_set_size(ui->label_6, 70, 19);
	lv_label_set_style(ui->label_6, LV_LABEL_STYLE_MAIN, &style0_label_6);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_7, &lv_style_transp);
	style0_label_7.text.color = lv_color_hex(0x000000);
	style0_label_7.text.line_space = 2;

	ui->label_7 = lv_label_create(ui->cont, NULL);
	lv_label_set_text(ui->label_7, "explorer");
	lv_label_set_long_mode(ui->label_7, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_7, 478, 381);
	lv_obj_set_size(ui->label_7, 58, 19);
	lv_label_set_style(ui->label_7, LV_LABEL_STYLE_MAIN, &style0_label_7);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_8, &lv_style_transp);
	style0_label_8.text.color = lv_color_hex(0x000000);
	style0_label_8.text.line_space = 2;

	ui->label_8 = lv_label_create(ui->cont, NULL);
	lv_label_set_text(ui->label_8, "setting");
	lv_label_set_long_mode(ui->label_8, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_8, 650, 382);
	lv_obj_set_size(ui->label_8, 49, 19);
	lv_label_set_style(ui->label_8, LV_LABEL_STYLE_MAIN, &style0_label_8);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_1, &lv_style_pretty);
	style0_container_1.body.main_color = lv_color_hex(0xaaaaff);
	style0_container_1.body.grad_color = lv_color_hex(0xaaaaff);
	style0_container_1.body.radius = 0;
	style0_container_1.body.border.width = 0;

	ui->container_1 = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->container_1, 0, 0);
	lv_obj_set_size(ui->container_1, 800, 480);
	lv_cont_set_fit4(ui->container_1, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_obj_set_hidden(ui->container_1,true);
	lv_cont_set_style(ui->container_1, LV_CONT_STYLE_MAIN, &style0_container_1);
#endif // LV_USE_CONT

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_3, &lv_style_pretty);
	style0_container_3.body.main_color = lv_color_hex(0xff55ff);
	style0_container_3.body.grad_color = lv_color_hex(0xff55ff);

	ui->container_3 = lv_cont_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->container_3, 307, 138);
	lv_obj_set_size(ui->container_3, 197, 66);
	lv_cont_set_fit4(ui->container_3, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_3, LV_CONT_STYLE_MAIN, &style0_container_3);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	ui->label_10 = lv_label_create(ui->container_3, NULL);
	lv_label_set_text(ui->label_10, "电影");
	lv_label_set_long_mode(ui->label_10, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_10, 44, 18);
	lv_obj_set_size(ui->label_10, 0, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_4, &lv_style_pretty);
	style0_container_4.body.main_color = lv_color_hex(0x55aaff);
	style0_container_4.body.grad_color = lv_color_hex(0x55aaff);

	ui->container_4 = lv_cont_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->container_4, 96, 69);
	lv_obj_set_size(ui->container_4, 255, 55);
	lv_cont_set_fit4(ui->container_4, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_4, LV_CONT_STYLE_MAIN, &style0_container_4);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	ui->label_11 = lv_label_create(ui->container_4, NULL);
	lv_label_set_text(ui->label_11, "商城");
	lv_label_set_long_mode(ui->label_11, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_11, 97, 10);
	lv_obj_set_size(ui->label_11, 0, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_5, &lv_style_pretty);
	style0_container_5.body.main_color = lv_color_hex(0x00ffff);
	style0_container_5.body.grad_color = lv_color_hex(0x00ffff);

	ui->container_5 = lv_cont_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->container_5, 96, 126);
	lv_obj_set_size(ui->container_5, 205, 146);
	lv_cont_set_fit4(ui->container_5, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_5, LV_CONT_STYLE_MAIN, &style0_container_5);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	ui->label_9 = lv_label_create(ui->container_5, NULL);
	lv_label_set_text(ui->label_9, "天气");
	lv_label_set_long_mode(ui->label_9, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_9, 71, 52);
	lv_obj_set_size(ui->label_9, 0, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_6, &lv_style_pretty);
	style0_container_6.body.main_color = lv_color_hex(0x55aa00);
	style0_container_6.body.grad_color = lv_color_hex(0x55aa00);

	ui->container_6 = lv_cont_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->container_6, 509, 136);
	lv_obj_set_size(ui->container_6, 205, 67);
	lv_cont_set_fit4(ui->container_6, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_6, LV_CONT_STYLE_MAIN, &style0_container_6);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	ui->label_14 = lv_label_create(ui->container_6, NULL);
	lv_label_set_text(ui->label_14, "相册");
	lv_label_set_long_mode(ui->label_14, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_14, 58, 20);
	lv_obj_set_size(ui->label_14, 0, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_7, &lv_style_pretty);
	style0_container_7.body.main_color = lv_color_hex(0x00ff7f);
	style0_container_7.body.grad_color = lv_color_hex(0xaaff7f);

	ui->container_7 = lv_cont_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->container_7, 482, 66);
	lv_obj_set_size(ui->container_7, 129, 59);
	lv_cont_set_fit4(ui->container_7, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_7, LV_CONT_STYLE_MAIN, &style0_container_7);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	ui->label_15 = lv_label_create(ui->container_7, NULL);
	lv_label_set_text(ui->label_15, "日历");
	lv_label_set_long_mode(ui->label_15, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_15, 37, 11);
	lv_obj_set_size(ui->label_15, 0, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_8, &lv_style_pretty);
	style0_container_8.body.main_color = lv_color_hex(0xffaa00);
	style0_container_8.body.grad_color = lv_color_hex(0xffaa00);

	ui->container_8 = lv_cont_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->container_8, 306, 214);
	lv_obj_set_size(ui->container_8, 409, 48);
	lv_cont_set_fit4(ui->container_8, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_8, LV_CONT_STYLE_MAIN, &style0_container_8);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	ui->label_13 = lv_label_create(ui->container_8, NULL);
	lv_label_set_text(ui->label_13, "电子书");
	lv_label_set_long_mode(ui->label_13, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_13, 157, 2);
	lv_obj_set_size(ui->label_13, 0, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_9, &lv_style_pretty);
	style0_container_9.body.main_color = lv_color_hex(0x5555ff);
	style0_container_9.body.grad_color = lv_color_hex(0x5555ff);

	ui->container_9 = lv_cont_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->container_9, 361, 68);
	lv_obj_set_size(ui->container_9, 114, 57);
	lv_cont_set_fit4(ui->container_9, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_9, LV_CONT_STYLE_MAIN, &style0_container_9);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	ui->label_12 = lv_label_create(ui->container_9, NULL);
	lv_label_set_text(ui->label_12, "计算器");
	lv_label_set_long_mode(ui->label_12, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_12, 23, 14);
	lv_obj_set_size(ui->label_12, 0, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_10, &lv_style_pretty);
	style0_container_10.body.main_color = lv_color_hex(0x00aa7f);
	style0_container_10.body.grad_color = lv_color_hex(0x00aa7f);

	ui->container_10 = lv_cont_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->container_10, 615, 68);
	lv_obj_set_size(ui->container_10, 100, 56);
	lv_cont_set_fit4(ui->container_10, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_10, LV_CONT_STYLE_MAIN, &style0_container_10);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	ui->label_16 = lv_label_create(ui->container_10, NULL);
	lv_label_set_text(ui->label_16, "音乐");
	lv_label_set_long_mode(ui->label_16, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_16, 24, 8);
	lv_obj_set_size(ui->label_16, 0, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LINE
	ui->line_1 = lv_line_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->line_1, 242, 300);
	lv_obj_set_size(ui->line_1, 202, 17);
	static lv_point_t line_1_points[2] = {
		{0,15},
		{200,15},
		};
	lv_line_set_points(ui->line_1, line_1_points, 2);
#endif // LV_USE_LINE

#ifdef LV_USE_LINE
	ui->line_2 = lv_line_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->line_2, 242, 305);
	lv_obj_set_size(ui->line_2, 202, 17);
	static lv_point_t line_2_points[2] = {
		{0,15},
		{200,15},
		};
	lv_line_set_points(ui->line_2, line_2_points, 2);
#endif // LV_USE_LINE

#ifdef LV_USE_LINE
	ui->line_3 = lv_line_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->line_3, 550, 274);
	lv_obj_set_size(ui->line_3, 2, 102);
	static lv_point_t line_3_points[2] = {
		{0,15},
		{0,100},
		};
	lv_line_set_points(ui->line_3, line_3_points, 2);
#endif // LV_USE_LINE

#ifdef LV_USE_LINE
	ui->line_4 = lv_line_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->line_4, 555, 274);
	lv_obj_set_size(ui->line_4, 2, 102);
	static lv_point_t line_4_points[2] = {
		{0,15},
		{0,100},
		};
	lv_line_set_points(ui->line_4, line_4_points, 2);
#endif // LV_USE_LINE

#ifdef LV_USE_LINE
	ui->line_5 = lv_line_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->line_5, 241, 350);
	lv_obj_set_size(ui->line_5, 202, 17);
	static lv_point_t line_5_points[2] = {
		{0,15},
		{200,15},
		};
	lv_line_set_points(ui->line_5, line_5_points, 2);
#endif // LV_USE_LINE

#ifdef LV_USE_BTN
	ui->button_1 = lv_btn_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->button_1, 192, 42);
	lv_obj_set_size(ui->button_1, 100, 35);
#endif // LV_USE_BTN

}

void home_auto_ui_destory(home_ui_t *ui)
{
	lv_obj_clean(ui->cont);
	free_image(image_button_1_home_movie_png_state0);
	free_image(image_button_1_home_movie_png_state1);
	free_image(image_button_2_home_music_png_state0);
	free_image(image_button_2_home_music_png_state1);
	free_image(image_button_3_home_calendar_png_state0);
	free_image(image_button_3_home_calendar_png_state1);
	free_image(image_button_4_home_timer_png_state0);
	free_image(image_button_4_home_timer_png_state1);
	free_image(image_button_5_home_photo_png_state0);
	free_image(image_button_5_home_photo_png_state1);
	free_image(image_button_6_home_calculator_png_state0);
	free_image(image_button_6_home_calculator_png_state1);
	free_image(image_button_7_home_explorer_png_state0);
	free_image(image_button_7_home_explorer_png_state1);
	free_image(image_button_8_home_setting_png_state0);
	free_image(image_button_8_home_setting_png_state1);
}

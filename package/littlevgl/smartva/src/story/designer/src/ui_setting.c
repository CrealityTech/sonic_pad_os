/**********************
 *      includes
 **********************/
#include "ui_setting.h"
#include "lvgl.h"
#include "common.h"
#include "ui_resource.h"


/**********************
 *       variables
 **********************/
static lv_style_t style_screen;
static lv_style_t style0_line_1;
static lv_style_t style0_line_2;
static lv_style_t style0_label_1;
static lv_style_t style1_list_1;
static lv_style_t style3_list_1;
static lv_style_t style4_list_1;
static lv_style_t style5_list_1;
static lv_style_t style6_list_1;
static lv_style_t style7_list_1;
static lv_style_t style0_label_10;
static lv_style_t style0_image_1;
static lv_style_t style0_infomation_cont;
static lv_style_t style0_container_1;
static lv_style_t style0_line_19;
static lv_style_t style0_line_20;
static lv_style_t style0_container_3;
static lv_style_t style0_language_cont;
static lv_style_t style0_display_cont;
static lv_style_t style0_container_8;
static lv_style_t style1_slider_1;
static lv_style_t style2_slider_1;
static lv_style_t style0_container_9;
static lv_style_t style0_line_7;
static lv_style_t style0_switch_2;
static lv_style_t style1_switch_2;
static lv_style_t style2_switch_2;
static lv_style_t style3_switch_2;
static lv_style_t style1_slider_2;
static lv_style_t style2_slider_2;
static lv_style_t style1_slider_3;
static lv_style_t style2_slider_3;
static lv_style_t style1_slider_4;
static lv_style_t style2_slider_4;
static lv_style_t style0_line_17;
static lv_style_t style0_line_18;
static lv_style_t style0_general_cont;
static lv_style_t style0_line_5;

/**********************
 *  images and fonts
 **********************/
static void *image_1_home_return_png = NULL;
static void *list_1_1_setting_general_png = NULL;
static void *list_1_2_setting_display_png = NULL;
static void *list_1_3_setting_language_png = NULL;
static void *list_1_4_setting_information_png = NULL;
static void *list_1_5_setting_wallpaper_png = NULL;
static void *list_1_6_setting_password_png = NULL;
static void *list_1_7_setting_date_and_time_png = NULL;

/**********************
 *  functions
 **********************/
void setting_auto_ui_create(setting_ui_t *ui)
{
	lv_style_copy(&style_screen, &lv_style_scr);
	style_screen.body.main_color = lv_color_hex(0xf2f2f2);
	style_screen.body.grad_color = lv_color_hex(0xf2f2f2);
	lv_obj_set_style(ui->cont, &style_screen);

#ifdef LV_USE_LINE
	lv_style_copy(&style0_line_1, &lv_style_pretty);
	style0_line_1.line.color = lv_color_hex(0xb9b8bd);
	style0_line_1.line.width = 1;

	ui->line_1 = lv_line_create(ui->cont, NULL);
	lv_obj_set_pos(ui->line_1, 200, 0);
	lv_obj_set_size(ui->line_1, 2, 482);
	static lv_point_t line_1_points[2] = {
		{0,0},
		{0,480},
		};
	lv_line_set_points(ui->line_1, line_1_points, 2);
	lv_line_set_style(ui->line_1, LV_LINE_STYLE_MAIN, &style0_line_1);
#endif // LV_USE_LINE

#ifdef LV_USE_LINE
	lv_style_copy(&style0_line_2, &lv_style_pretty);
	style0_line_2.line.color = lv_color_hex(0xb9b8bd);
	style0_line_2.line.width = 1;

	ui->line_2 = lv_line_create(ui->cont, NULL);
	lv_obj_set_pos(ui->line_2, 0, 54);
	lv_obj_set_size(ui->line_2, 202, 17);
	static lv_point_t line_2_points[2] = {
		{0,15},
		{200,15},
		};
	lv_line_set_points(ui->line_2, line_2_points, 2);
	lv_line_set_style(ui->line_2, LV_LINE_STYLE_MAIN, &style0_line_2);
#endif // LV_USE_LINE

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_1, &lv_style_transp);
	style0_label_1.text.color = lv_color_hex(0x3d3d3d);
	style0_label_1.text.font = &microsoft_yahei_bold_28_4;
	style0_label_1.text.line_space = 2;

	ui->label_1 = lv_label_create(ui->cont, NULL);
	lv_label_set_text(ui->label_1, "Setting");
	lv_label_set_long_mode(ui->label_1, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_1, 49, 13);
	lv_obj_set_size(ui->label_1, 102, 37);
	lv_label_set_style(ui->label_1, LV_LABEL_STYLE_MAIN, &style0_label_1);
#endif // LV_USE_LABEL

#ifdef LV_USE_LIST
	lv_style_copy(&style1_list_1, &lv_style_pretty);
	style1_list_1.body.padding.top = 0;
	style1_list_1.body.padding.bottom = 0;
	style1_list_1.body.padding.left = 0;
	style1_list_1.body.padding.right = 0;
	style1_list_1.body.padding.inner = 0;

	lv_style_copy(&style3_list_1, &lv_style_transp);
	style3_list_1.body.main_color = lv_color_hex(0x5596d8);
	style3_list_1.body.grad_color = lv_color_hex(0x5596d8);
	style3_list_1.body.radius = 0;
	style3_list_1.body.opa = 255;
	style3_list_1.body.border.color = lv_color_hex(0x000000);
	style3_list_1.body.border.width = 0;
	style3_list_1.body.border.part = 15;
	style3_list_1.body.border.opa = 255;
	style3_list_1.body.shadow.color = lv_color_hex(0x808080);
	style3_list_1.body.shadow.width = 0;
	style3_list_1.body.shadow.type = 1;
	style3_list_1.body.padding.top = 5;
	style3_list_1.body.padding.bottom = 5;
	style3_list_1.body.padding.left = 5;
	style3_list_1.body.padding.right = 5;
	style3_list_1.body.padding.inner = 5;
	style3_list_1.text.color = lv_color_hex(0x000000);
	style3_list_1.text.sel_color = lv_color_hex(0x5596d8);
	style3_list_1.text.font = &lv_font_roboto_16;
	style3_list_1.text.letter_space = 0;
	style3_list_1.text.line_space = 2;
	style3_list_1.text.opa = 255;
	style3_list_1.image.color = lv_color_hex(0xf0f0f0);
	style3_list_1.image.intense = 0;
	style3_list_1.image.opa = 255;

	lv_style_copy(&style4_list_1, &lv_style_btn_rel);
	style4_list_1.body.main_color = lv_color_hex(0xffffff);
	style4_list_1.body.grad_color = lv_color_hex(0xffffff);
	style4_list_1.body.radius = 0;
	style4_list_1.body.border.color = lv_color_hex(0xe3e3e3);
	style4_list_1.body.border.part = 1;
	style4_list_1.text.color = lv_color_hex(0x2d2d2d);
	style4_list_1.text.line_space = 2;

	lv_style_copy(&style5_list_1, &lv_style_btn_pr);
	style5_list_1.body.main_color = lv_color_hex(0xc0c0c0);
	style5_list_1.body.grad_color = lv_color_hex(0xc0c0c0);
	style5_list_1.body.radius = 0;
	style5_list_1.body.border.width = 0;
	style5_list_1.body.padding.inner = 20;
	style5_list_1.text.color = lv_color_hex(0x2d2d2d);
	style5_list_1.text.line_space = 2;

	lv_style_copy(&style6_list_1, &lv_style_btn_tgl_rel);
	style6_list_1.body.main_color = lv_color_hex(0xc0c0c0);
	style6_list_1.body.grad_color = lv_color_hex(0xc0c0c0);
	style6_list_1.body.radius = 0;
	style6_list_1.body.border.width = 0;
	style6_list_1.body.padding.inner = 20;
	style6_list_1.text.color = lv_color_hex(0x2d2d2d);
	style6_list_1.text.line_space = 2;

	lv_style_copy(&style7_list_1, &lv_style_btn_tgl_pr);
	style7_list_1.body.main_color = lv_color_hex(0xc0c0c0);
	style7_list_1.body.grad_color = lv_color_hex(0xc0c0c0);
	style7_list_1.body.radius = 0;
	style7_list_1.body.border.width = 0;
	style7_list_1.body.border.part = 1;
	style7_list_1.body.padding.inner = 20;
	style7_list_1.text.color = lv_color_hex(0x2d2d2d);
	style7_list_1.text.line_space = 2;

	ui->list_1 = lv_list_create(ui->cont, NULL);
	lv_obj_set_pos(ui->list_1, 0, 70);
	lv_obj_set_size(ui->list_1, 200, 410);
	
	list_1_1_setting_general_png = (void *)parse_image_form_file(LV_IMAGE_PATH"setting_general.png");
	lv_list_add_btn(ui->list_1, list_1_1_setting_general_png, "General");
	
	list_1_2_setting_display_png = (void *)parse_image_form_file(LV_IMAGE_PATH"setting_display.png");
	lv_list_add_btn(ui->list_1, list_1_2_setting_display_png, "Display");
	
	list_1_3_setting_language_png = (void *)parse_image_form_file(LV_IMAGE_PATH"setting_language.png");
	lv_list_add_btn(ui->list_1, list_1_3_setting_language_png, "Language");
	
	list_1_4_setting_information_png = (void *)parse_image_form_file(LV_IMAGE_PATH"setting_information.png");
	lv_list_add_btn(ui->list_1, list_1_4_setting_information_png, "Information");
	
	list_1_5_setting_wallpaper_png = (void *)parse_image_form_file(LV_IMAGE_PATH"setting_wallpaper.png");
	lv_list_add_btn(ui->list_1, list_1_5_setting_wallpaper_png, "Wallpaper");
	
	list_1_6_setting_password_png = (void *)parse_image_form_file(LV_IMAGE_PATH"setting_password.png");
	lv_list_add_btn(ui->list_1, list_1_6_setting_password_png, "Password");
	
	list_1_7_setting_date_and_time_png = (void *)parse_image_form_file(LV_IMAGE_PATH"setting_date_and_time.png");
	lv_list_add_btn(ui->list_1, list_1_7_setting_date_and_time_png, "Time");
	lv_list_set_single_mode(ui->list_1, true);
	lv_list_set_scroll_propagation(ui->list_1, false);
	lv_list_set_edge_flash(ui->list_1, true);
	lv_list_set_style(ui->list_1, LV_LIST_STYLE_SCRL, &style1_list_1);
	lv_list_set_style(ui->list_1, LV_LIST_STYLE_EDGE_FLASH, &style3_list_1);
	lv_list_set_style(ui->list_1, LV_LIST_STYLE_BTN_REL, &style4_list_1);
	lv_list_set_style(ui->list_1, LV_LIST_STYLE_BTN_PR, &style5_list_1);
	lv_list_set_style(ui->list_1, LV_LIST_STYLE_BTN_TGL_REL, &style6_list_1);
	lv_list_set_style(ui->list_1, LV_LIST_STYLE_BTN_TGL_PR, &style7_list_1);
#endif // LV_USE_LIST

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_10, &lv_style_transp);
	style0_label_10.text.font = &lv_font_roboto_28;
	style0_label_10.text.line_space = 2;

	ui->label_10 = lv_label_create(ui->cont, NULL);
	lv_label_set_text(ui->label_10, "General");
	lv_label_set_long_mode(ui->label_10, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_10, 448, 24);
	lv_obj_set_size(ui->label_10, 95, 32);
	lv_label_set_style(ui->label_10, LV_LABEL_STYLE_MAIN, &style0_label_10);
#endif // LV_USE_LABEL

#ifdef LV_USE_IMG
	lv_style_copy(&style0_image_1, &lv_style_plain);
	style0_image_1.image.intense = 255;

	ui->image_1 = lv_img_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_1, 13, 17);
	lv_obj_set_size(ui->image_1, 32, 32);
	image_1_home_return_png = (void *)parse_image_form_file(LV_IMAGE_PATH"home_return.png");
	lv_img_set_src(ui->image_1, image_1_home_return_png);

	lv_img_set_style(ui->image_1, LV_IMG_STYLE_MAIN, &style0_image_1);
#endif // LV_USE_IMG

#ifdef LV_USE_CONT
	lv_style_copy(&style0_infomation_cont, &lv_style_pretty);
	style0_infomation_cont.body.main_color = lv_color_hex(0xf2f2f2);
	style0_infomation_cont.body.grad_color = lv_color_hex(0xf2f2f2);
	style0_infomation_cont.body.radius = 10;
	style0_infomation_cont.body.border.width = 0;

	ui->infomation_cont = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->infomation_cont, 11, 244);
	lv_obj_set_size(ui->infomation_cont, 550, 223);
	lv_cont_set_fit4(ui->infomation_cont, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_obj_set_hidden(ui->infomation_cont,true);
	lv_cont_set_style(ui->infomation_cont, LV_CONT_STYLE_MAIN, &style0_infomation_cont);
#endif // LV_USE_CONT

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_1, &lv_style_pretty);
	style0_container_1.body.grad_color = lv_color_hex(0xffffff);
	style0_container_1.body.radius = 10;
	style0_container_1.body.border.width = 1;

	ui->container_1 = lv_cont_create(ui->infomation_cont, NULL);
	lv_obj_set_pos(ui->container_1, 0, 0);
	lv_obj_set_size(ui->container_1, 550, 147);
	lv_cont_set_fit4(ui->container_1, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_1, LV_CONT_STYLE_MAIN, &style0_container_1);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	ui->label_28 = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->label_28, "SDK version");
	lv_label_set_long_mode(ui->label_28, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_28, 26, 17);
	lv_obj_set_size(ui->label_28, 87, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_29 = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->label_29, "77dc3cda707b7a7b025da198eb27ac27504d1ebc");
	lv_label_set_long_mode(ui->label_29, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_29, 180, 17);
	lv_obj_set_size(ui->label_29, 356, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_30 = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->label_30, "OS version");
	lv_label_set_long_mode(ui->label_30, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_30, 27, 65);
	lv_obj_set_size(ui->label_30, 77, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_31 = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->label_31, "melis 2.0 @ 2018-11-26");
	lv_label_set_long_mode(ui->label_31, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_31, 371, 67);
	lv_obj_set_size(ui->label_31, 167, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_32 = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->label_32, "UI version");
	lv_label_set_long_mode(ui->label_32, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_32, 28, 114);
	lv_obj_set_size(ui->label_32, 70, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_33 = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->label_33, "V1.0.0");
	lv_label_set_long_mode(ui->label_33, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_33, 490, 115);
	lv_obj_set_size(ui->label_33, 45, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LINE
	lv_style_copy(&style0_line_19, &lv_style_pretty);
	style0_line_19.line.color = lv_color_hex(0xb1b1b1);
	style0_line_19.line.width = 1;

	ui->line_19 = lv_line_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->line_19, 28, 34);
	lv_obj_set_size(ui->line_19, 522, 17);
	static lv_point_t line_19_points[2] = {
		{0,15},
		{520,15},
		};
	lv_line_set_points(ui->line_19, line_19_points, 2);
	lv_line_set_style(ui->line_19, LV_LINE_STYLE_MAIN, &style0_line_19);
#endif // LV_USE_LINE

#ifdef LV_USE_LINE
	lv_style_copy(&style0_line_20, &lv_style_pretty);
	style0_line_20.line.color = lv_color_hex(0xb1b1b1);
	style0_line_20.line.width = 1;

	ui->line_20 = lv_line_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->line_20, 29, 84);
	lv_obj_set_size(ui->line_20, 522, 17);
	static lv_point_t line_20_points[2] = {
		{0,15},
		{520,15},
		};
	lv_line_set_points(ui->line_20, line_20_points, 2);
	lv_line_set_style(ui->line_20, LV_LINE_STYLE_MAIN, &style0_line_20);
#endif // LV_USE_LINE

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_3, &lv_style_pretty);
	style0_container_3.body.grad_color = lv_color_hex(0xffffff);
	style0_container_3.body.border.width = 1;

	ui->container_3 = lv_cont_create(ui->infomation_cont, NULL);
	lv_obj_set_pos(ui->container_3, 0, 173);
	lv_obj_set_size(ui->container_3, 550, 50);
	lv_cont_set_fit4(ui->container_3, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_3, LV_CONT_STYLE_MAIN, &style0_container_3);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	ui->label_34 = lv_label_create(ui->container_3, NULL);
	lv_label_set_text(ui->label_34, "Screen info");
	lv_label_set_long_mode(ui->label_34, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_34, 26, 18);
	lv_obj_set_size(ui->label_34, 82, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_35 = lv_label_create(ui->container_3, NULL);
	lv_label_set_text(ui->label_35, "800*480 @60fps");
	lv_label_set_long_mode(ui->label_35, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_35, 416, 19);
	lv_obj_set_size(ui->label_35, 120, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_language_cont, &lv_style_pretty);
	style0_language_cont.body.grad_color = lv_color_hex(0xffffff);
	style0_language_cont.body.radius = 10;
	style0_language_cont.body.border.width = 1;

	ui->language_cont = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->language_cont, 21, 247);
	lv_obj_set_size(ui->language_cont, 550, 225);
	lv_cont_set_fit4(ui->language_cont, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_obj_set_hidden(ui->language_cont,true);
	lv_cont_set_style(ui->language_cont, LV_CONT_STYLE_MAIN, &style0_language_cont);
#endif // LV_USE_CONT

#ifdef LV_USE_CB
	ui->check_box_1 = lv_cb_create(ui->language_cont, NULL);
	lv_obj_set_pos(ui->check_box_1, 85, 27);
	lv_obj_set_size(ui->check_box_1, 92, 29);
	lv_cb_set_text(ui->check_box_1, "Chinese");
	lv_cb_set_checked(ui->check_box_1, true);
#endif // LV_USE_CB

#ifdef LV_USE_CB
	ui->check_box_2 = lv_cb_create(ui->language_cont, NULL);
	lv_obj_set_pos(ui->check_box_2, 313, 27);
	lv_obj_set_size(ui->check_box_2, 86, 29);
	lv_cb_set_text(ui->check_box_2, "English");
	lv_cb_set_checked(ui->check_box_2, true);
#endif // LV_USE_CB

#ifdef LV_USE_CB
	ui->check_box_3 = lv_cb_create(ui->language_cont, NULL);
	lv_obj_set_pos(ui->check_box_3, 85, 143);
	lv_obj_set_size(ui->check_box_3, 74, 29);
	lv_cb_set_text(ui->check_box_3, "Egypt");
	lv_cb_set_checked(ui->check_box_3, true);
	lv_btn_set_state(ui->check_box_3, true);
#endif // LV_USE_CB

#ifdef LV_USE_CB
	ui->check_box_4 = lv_cb_create(ui->language_cont, NULL);
	lv_obj_set_pos(ui->check_box_4, 313, 85);
	lv_obj_set_size(ui->check_box_4, 79, 29);
	lv_cb_set_text(ui->check_box_4, "Japan");
	lv_cb_set_checked(ui->check_box_4, true);
	lv_btn_set_state(ui->check_box_4, true);
#endif // LV_USE_CB

#ifdef LV_USE_CB
	ui->check_box_5 = lv_cb_create(ui->language_cont, NULL);
	lv_obj_set_pos(ui->check_box_5, 85, 85);
	lv_obj_set_size(ui->check_box_5, 122, 29);
	lv_cb_set_text(ui->check_box_5, "South Korea");
	lv_cb_set_checked(ui->check_box_5, true);
	lv_btn_set_state(ui->check_box_5, true);
#endif // LV_USE_CB

#ifdef LV_USE_CB
	ui->check_box_6 = lv_cb_create(ui->language_cont, NULL);
	lv_obj_set_pos(ui->check_box_6, 313, 143);
	lv_obj_set_size(ui->check_box_6, 91, 29);
	lv_cb_set_text(ui->check_box_6, "Russian");
	lv_cb_set_checked(ui->check_box_6, true);
	lv_btn_set_state(ui->check_box_6, true);
#endif // LV_USE_CB

#ifdef LV_USE_CONT
	lv_style_copy(&style0_display_cont, &lv_style_pretty);
	style0_display_cont.body.main_color = lv_color_hex(0xf2f2f2);
	style0_display_cont.body.grad_color = lv_color_hex(0xf2f2f2);
	style0_display_cont.body.radius = 10;
	style0_display_cont.body.border.width = 0;

	ui->display_cont = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->display_cont, 0, 218);
	lv_obj_set_size(ui->display_cont, 561, 262);
	lv_cont_set_fit4(ui->display_cont, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_obj_set_hidden(ui->display_cont,true);
	lv_cont_set_style(ui->display_cont, LV_CONT_STYLE_MAIN, &style0_display_cont);
#endif // LV_USE_CONT

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_8, &lv_style_pretty);
	style0_container_8.body.grad_color = lv_color_hex(0xffffff);
	style0_container_8.body.radius = 10;
	style0_container_8.body.border.width = 1;

	ui->container_8 = lv_cont_create(ui->display_cont, NULL);
	lv_obj_set_pos(ui->container_8, 0, 0);
	lv_obj_set_size(ui->container_8, 550, 50);
	lv_cont_set_fit4(ui->container_8, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_8, LV_CONT_STYLE_MAIN, &style0_container_8);
#endif // LV_USE_CONT

#ifdef LV_USE_SLIDER
	lv_style_copy(&style1_slider_1, &lv_style_pretty_color);
	style1_slider_1.body.padding.top = 0;
	style1_slider_1.body.padding.bottom = 0;
	style1_slider_1.body.padding.left = 0;
	style1_slider_1.body.padding.right = 0;
	style1_slider_1.body.padding.inner = 0;

	lv_style_copy(&style2_slider_1, &lv_style_pretty);
	style2_slider_1.body.radius = 20;

	ui->slider_1 = lv_slider_create(ui->container_8, NULL);
	lv_obj_set_pos(ui->slider_1, 130, 13);
	lv_obj_set_size(ui->slider_1, 350, 25);
	lv_slider_set_range(ui->slider_1, 0, 100);
	lv_slider_set_knob_in(ui->slider_1, true);
	lv_slider_set_value(ui->slider_1, 0, LV_ANIM_OFF);
	lv_slider_set_style(ui->slider_1, LV_SLIDER_STYLE_INDIC, &style1_slider_1);
	lv_slider_set_style(ui->slider_1, LV_SLIDER_STYLE_KNOB, &style2_slider_1);
#endif // LV_USE_SLIDER

#ifdef LV_USE_LABEL
	ui->label_6 = lv_label_create(ui->container_8, NULL);
	lv_label_set_text(ui->label_6, "brightness");
	lv_label_set_long_mode(ui->label_6, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_6, 15, 16);
	lv_obj_set_size(ui->label_6, 75, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_8 = lv_label_create(ui->container_8, NULL);
	lv_label_set_text(ui->label_8, "50%");
	lv_label_set_long_mode(ui->label_8, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_8, 500, 16);
	lv_obj_set_size(ui->label_8, 30, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_9, &lv_style_pretty);
	style0_container_9.body.grad_color = lv_color_hex(0xffffff);
	style0_container_9.body.border.width = 1;

	ui->container_9 = lv_cont_create(ui->display_cont, NULL);
	lv_obj_set_pos(ui->container_9, 0, 60);
	lv_obj_set_size(ui->container_9, 550, 200);
	lv_cont_set_fit4(ui->container_9, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_9, LV_CONT_STYLE_MAIN, &style0_container_9);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	ui->label_7 = lv_label_create(ui->container_9, NULL);
	lv_label_set_text(ui->label_7, "enhance");
	lv_label_set_long_mode(ui->label_7, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_7, 17, 17);
	lv_obj_set_size(ui->label_7, 62, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LINE
	lv_style_copy(&style0_line_7, &lv_style_pretty);
	style0_line_7.line.color = lv_color_hex(0xb1b1b1);
	style0_line_7.line.width = 1;

	ui->line_7 = lv_line_create(ui->container_9, NULL);
	lv_obj_set_pos(ui->line_7, 18, 34);
	lv_obj_set_size(ui->line_7, 532, 17);
	static lv_point_t line_7_points[2] = {
		{0,15},
		{530,15},
		};
	lv_line_set_points(ui->line_7, line_7_points, 2);
	lv_line_set_style(ui->line_7, LV_LINE_STYLE_MAIN, &style0_line_7);
#endif // LV_USE_LINE

#ifdef LV_USE_SW
	lv_style_copy(&style0_switch_2, &lv_style_pretty);
	style0_switch_2.body.main_color = lv_color_hex(0x55aaff);
	style0_switch_2.body.grad_color = lv_color_hex(0x55aaff);
	style0_switch_2.body.radius = 40;
	style0_switch_2.body.border.width = 0;
	style0_switch_2.body.padding.top = 0;
	style0_switch_2.body.padding.bottom = 0;
	style0_switch_2.body.padding.left = 3;
	style0_switch_2.body.padding.right = 1;
	style0_switch_2.body.padding.inner = 0;

	lv_style_copy(&style1_switch_2, &lv_style_pretty_color);
	style1_switch_2.body.opa = 0;
	style1_switch_2.body.border.width = 0;

	lv_style_copy(&style2_switch_2, &lv_style_pretty);
	style2_switch_2.body.main_color = lv_color_hex(0xe8e8e8);
	style2_switch_2.body.grad_color = lv_color_hex(0xe8e8e8);
	style2_switch_2.body.radius = 40;
	style2_switch_2.body.border.width = 0;

	lv_style_copy(&style3_switch_2, &lv_style_pretty);
	style3_switch_2.body.main_color = lv_color_hex(0xe8e8e8);
	style3_switch_2.body.grad_color = lv_color_hex(0xe8e8e8);
	style3_switch_2.body.radius = 40;
	style3_switch_2.body.border.width = 0;

	ui->switch_2 = lv_sw_create(ui->container_9, NULL);
	lv_obj_set_pos(ui->switch_2, 487, 13);
	lv_obj_set_size(ui->switch_2, 50, 25);
	lv_sw_on(ui->switch_2, LV_ANIM_OFF);
	lv_sw_set_style(ui->switch_2, LV_SW_STYLE_BG, &style0_switch_2);
	lv_sw_set_style(ui->switch_2, LV_SW_STYLE_INDIC, &style1_switch_2);
	lv_sw_set_style(ui->switch_2, LV_SW_STYLE_KNOB_OFF, &style2_switch_2);
	lv_sw_set_style(ui->switch_2, LV_SW_STYLE_KNOB_ON, &style3_switch_2);
#endif // LV_USE_SW

#ifdef LV_USE_LABEL
	ui->label_9 = lv_label_create(ui->container_9, NULL);
	lv_label_set_text(ui->label_9, "saturation");
	lv_label_set_long_mode(ui->label_9, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_9, 16, 65);
	lv_obj_set_size(ui->label_9, 72, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_12 = lv_label_create(ui->container_9, NULL);
	lv_label_set_text(ui->label_12, "contrast");
	lv_label_set_long_mode(ui->label_12, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_12, 17, 114);
	lv_obj_set_size(ui->label_12, 58, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_13 = lv_label_create(ui->container_9, NULL);
	lv_label_set_text(ui->label_13, "hue");
	lv_label_set_long_mode(ui->label_13, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_13, 18, 166);
	lv_obj_set_size(ui->label_13, 27, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_SLIDER
	lv_style_copy(&style1_slider_2, &lv_style_pretty_color);
	style1_slider_2.body.padding.top = 0;
	style1_slider_2.body.padding.bottom = 0;
	style1_slider_2.body.padding.left = 0;
	style1_slider_2.body.padding.right = 0;
	style1_slider_2.body.padding.inner = 0;

	lv_style_copy(&style2_slider_2, &lv_style_pretty);
	style2_slider_2.body.radius = 10;

	ui->slider_2 = lv_slider_create(ui->container_9, NULL);
	lv_obj_set_pos(ui->slider_2, 131, 63);
	lv_obj_set_size(ui->slider_2, 350, 25);
	lv_slider_set_range(ui->slider_2, 0, 100);
	lv_slider_set_knob_in(ui->slider_2, true);
	lv_slider_set_value(ui->slider_2, 0, LV_ANIM_OFF);
	lv_slider_set_style(ui->slider_2, LV_SLIDER_STYLE_INDIC, &style1_slider_2);
	lv_slider_set_style(ui->slider_2, LV_SLIDER_STYLE_KNOB, &style2_slider_2);
#endif // LV_USE_SLIDER

#ifdef LV_USE_SLIDER
	lv_style_copy(&style1_slider_3, &lv_style_pretty_color);
	style1_slider_3.body.padding.top = 0;
	style1_slider_3.body.padding.bottom = 0;
	style1_slider_3.body.padding.left = 0;
	style1_slider_3.body.padding.right = 0;
	style1_slider_3.body.padding.inner = 0;

	lv_style_copy(&style2_slider_3, &lv_style_pretty);
	style2_slider_3.body.radius = 10;

	ui->slider_3 = lv_slider_create(ui->container_9, NULL);
	lv_obj_set_pos(ui->slider_3, 130, 112);
	lv_obj_set_size(ui->slider_3, 350, 25);
	lv_slider_set_range(ui->slider_3, 0, 100);
	lv_slider_set_knob_in(ui->slider_3, true);
	lv_slider_set_value(ui->slider_3, 0, LV_ANIM_OFF);
	lv_slider_set_style(ui->slider_3, LV_SLIDER_STYLE_INDIC, &style1_slider_3);
	lv_slider_set_style(ui->slider_3, LV_SLIDER_STYLE_KNOB, &style2_slider_3);
#endif // LV_USE_SLIDER

#ifdef LV_USE_SLIDER
	lv_style_copy(&style1_slider_4, &lv_style_pretty_color);
	style1_slider_4.body.padding.top = 0;
	style1_slider_4.body.padding.bottom = 0;
	style1_slider_4.body.padding.left = 0;
	style1_slider_4.body.padding.right = 0;
	style1_slider_4.body.padding.inner = 0;

	lv_style_copy(&style2_slider_4, &lv_style_pretty);
	style2_slider_4.body.radius = 10;

	ui->slider_4 = lv_slider_create(ui->container_9, NULL);
	lv_obj_set_pos(ui->slider_4, 130, 162);
	lv_obj_set_size(ui->slider_4, 350, 25);
	lv_slider_set_range(ui->slider_4, 0, 100);
	lv_slider_set_knob_in(ui->slider_4, true);
	lv_slider_set_value(ui->slider_4, 28, LV_ANIM_OFF);
	lv_slider_set_style(ui->slider_4, LV_SLIDER_STYLE_INDIC, &style1_slider_4);
	lv_slider_set_style(ui->slider_4, LV_SLIDER_STYLE_KNOB, &style2_slider_4);
#endif // LV_USE_SLIDER

#ifdef LV_USE_LINE
	lv_style_copy(&style0_line_17, &lv_style_pretty);
	style0_line_17.line.color = lv_color_hex(0xb1b1b1);
	style0_line_17.line.width = 1;

	ui->line_17 = lv_line_create(ui->container_9, NULL);
	lv_obj_set_pos(ui->line_17, 18, 84);
	lv_obj_set_size(ui->line_17, 532, 17);
	static lv_point_t line_17_points[2] = {
		{0,15},
		{530,15},
		};
	lv_line_set_points(ui->line_17, line_17_points, 2);
	lv_line_set_style(ui->line_17, LV_LINE_STYLE_MAIN, &style0_line_17);
#endif // LV_USE_LINE

#ifdef LV_USE_LINE
	lv_style_copy(&style0_line_18, &lv_style_pretty);
	style0_line_18.line.color = lv_color_hex(0xb1b1b1);
	style0_line_18.line.width = 1;

	ui->line_18 = lv_line_create(ui->container_9, NULL);
	lv_obj_set_pos(ui->line_18, 18, 134);
	lv_obj_set_size(ui->line_18, 532, 17);
	static lv_point_t line_18_points[2] = {
		{0,15},
		{530,15},
		};
	lv_line_set_points(ui->line_18, line_18_points, 2);
	lv_line_set_style(ui->line_18, LV_LINE_STYLE_MAIN, &style0_line_18);
#endif // LV_USE_LINE

#ifdef LV_USE_LABEL
	ui->label_25 = lv_label_create(ui->container_9, NULL);
	lv_label_set_text(ui->label_25, "20%");
	lv_label_set_long_mode(ui->label_25, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_25, 499, 65);
	lv_obj_set_size(ui->label_25, 30, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_26 = lv_label_create(ui->container_9, NULL);
	lv_label_set_text(ui->label_26, "20%");
	lv_label_set_long_mode(ui->label_26, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_26, 499, 115);
	lv_obj_set_size(ui->label_26, 30, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_27 = lv_label_create(ui->container_9, NULL);
	lv_label_set_text(ui->label_27, "20%");
	lv_label_set_long_mode(ui->label_27, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_27, 499, 167);
	lv_obj_set_size(ui->label_27, 30, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_general_cont, &lv_style_pretty);
	style0_general_cont.body.main_color = lv_color_hex(0xf2f2f2);
	style0_general_cont.body.grad_color = lv_color_hex(0xf2f2f2);
	style0_general_cont.body.radius = 10;
	style0_general_cont.body.border.width = 0;

	ui->general_cont = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->general_cont, 215, 184);
	lv_obj_set_size(ui->general_cont, 550, 296);
	lv_cont_set_fit4(ui->general_cont, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->general_cont, LV_CONT_STYLE_MAIN, &style0_general_cont);
#endif // LV_USE_CONT

#ifdef LV_USE_LINE
	lv_style_copy(&style0_line_5, &lv_style_pretty);
	style0_line_5.line.color = lv_color_hex(0xb1b1b1);
	style0_line_5.line.width = 1;

	ui->line_5 = lv_line_create(ui->cont, NULL);
	lv_obj_set_pos(ui->line_5, 200, 54);
	lv_obj_set_size(ui->line_5, 602, 17);
	static lv_point_t line_5_points[2] = {
		{0,15},
		{600,15},
		};
	lv_line_set_points(ui->line_5, line_5_points, 2);
	lv_line_set_style(ui->line_5, LV_LINE_STYLE_MAIN, &style0_line_5);
#endif // LV_USE_LINE

}

void setting_auto_ui_destory(setting_ui_t *ui)
{
	lv_obj_clean(ui->cont);
	free_image(image_1_home_return_png);
	free_image(list_1_1_setting_general_png);
	free_image(list_1_2_setting_display_png);
	free_image(list_1_3_setting_language_png);
	free_image(list_1_4_setting_information_png);
	free_image(list_1_5_setting_wallpaper_png);
	free_image(list_1_6_setting_password_png);
	free_image(list_1_7_setting_date_and_time_png);
}

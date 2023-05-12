/**********************
 *      includes
 **********************/
#include "ui_explorer.h"
#include "lvgl.h"
#include "ui_resource.h"
#include "common.h"


/**********************
 *       variables
 **********************/
static lv_style_t style_screen;
static lv_style_t style0_list_2;
static lv_style_t style1_list_2;
static lv_style_t style3_list_2;
static lv_style_t style4_list_2;
static lv_style_t style5_list_2;
static lv_style_t style6_list_2;
static lv_style_t style7_list_2;
static lv_style_t style0_medium_cont;
static lv_style_t style0_medium_list_1;
static lv_style_t style1_medium_list_1;
static lv_style_t style2_medium_list_1;
static lv_style_t style3_medium_list_1;
static lv_style_t style4_medium_list_1;
static lv_style_t style5_medium_list_1;
static lv_style_t style6_medium_list_1;
static lv_style_t style7_medium_list_1;
static lv_style_t style0_container_7;
static lv_style_t style0_container_3;
static lv_style_t style0_image_1;
static lv_style_t style0_label_5;
static lv_style_t style0_container_1;
static lv_style_t style0_container_5;
static lv_style_t style0_direct_cont;

/**********************
 *  images and fonts
 **********************/
static void *image_1_home_return = NULL;
static void *image_3_explorer_opendir = NULL;
static void *image_2_explorer_return = NULL;

/**********************
 *  functions
 **********************/
void explorer_auto_ui_create(explorer_ui_t *ui)
{
	lv_style_copy(&style_screen, &lv_style_scr);
	style_screen.body.main_color = lv_color_hex(0xf1f0f5);
	style_screen.body.grad_color = lv_color_hex(0xf1f0f5);
	lv_obj_set_style(ui->cont, &style_screen);

#ifdef LV_USE_LIST
	lv_style_copy(&style0_list_2, &lv_style_transp_fit);

	lv_style_copy(&style1_list_2, &lv_style_pretty);
	style1_list_2.body.main_color = lv_color_hex(0x6f6f6f);
	style1_list_2.body.grad_color = lv_color_hex(0x6f6f6f);
	style1_list_2.body.radius = 0;
	style1_list_2.body.border.width = 0;
	style1_list_2.body.padding.top = 0;
	style1_list_2.body.padding.bottom = 0;
	style1_list_2.body.padding.left = 0;
	style1_list_2.body.padding.right = 0;
	style1_list_2.body.padding.inner = 0;

	lv_style_copy(&style3_list_2, &lv_style_transp);
	style3_list_2.body.main_color = lv_color_hex(0x5596d8);
	style3_list_2.body.grad_color = lv_color_hex(0x5596d8);
	style3_list_2.body.radius = 0;
	style3_list_2.body.opa = 255;
	style3_list_2.body.border.color = lv_color_hex(0x000000);
	style3_list_2.body.border.width = 0;
	style3_list_2.body.border.part = 15;
	style3_list_2.body.border.opa = 255;
	style3_list_2.body.shadow.color = lv_color_hex(0x808080);
	style3_list_2.body.shadow.width = 0;
	style3_list_2.body.shadow.type = 1;
	style3_list_2.body.padding.top = 5;
	style3_list_2.body.padding.bottom = 5;
	style3_list_2.body.padding.left = 5;
	style3_list_2.body.padding.right = 5;
	style3_list_2.body.padding.inner = 5;
	style3_list_2.text.color = lv_color_hex(0xf0f0f0);
	style3_list_2.text.sel_color = lv_color_hex(0x5596d8);
	style3_list_2.text.font = &lv_font_roboto_16;
	style3_list_2.text.letter_space = 0;
	style3_list_2.text.line_space = 2;
	style3_list_2.text.opa = 255;
	style3_list_2.image.color = lv_color_hex(0xf0f0f0);
	style3_list_2.image.intense = 0;
	style3_list_2.image.opa = 255;

	lv_style_copy(&style4_list_2, &lv_style_btn_rel);
	style4_list_2.body.main_color = lv_color_hex(0xc4c4c7);
	style4_list_2.body.grad_color = lv_color_hex(0xc4c4c7);
	style4_list_2.body.radius = 0;
	style4_list_2.body.border.width = 0;
	style4_list_2.text.color = lv_color_hex(0x383834);
	style4_list_2.text.line_space = 2;

	lv_style_copy(&style5_list_2, &lv_style_btn_pr);
	style5_list_2.body.main_color = lv_color_hex(0xf47e4f);
	style5_list_2.body.grad_color = lv_color_hex(0xf47e4f);
	style5_list_2.body.radius = 0;
	style5_list_2.body.border.width = 0;
	style5_list_2.text.line_space = 2;

	lv_style_copy(&style6_list_2, &lv_style_btn_tgl_rel);
	style6_list_2.body.main_color = lv_color_hex(0xf47e4f);
	style6_list_2.body.grad_color = lv_color_hex(0xf47e4f);
	style6_list_2.body.radius = 0;
	style6_list_2.body.border.width = 0;
	style6_list_2.text.color = lv_color_hex(0xffffff);
	style6_list_2.text.line_space = 2;

	lv_style_copy(&style7_list_2, &lv_style_btn_tgl_pr);
	style7_list_2.body.main_color = lv_color_hex(0xf47e4f);
	style7_list_2.body.grad_color = lv_color_hex(0xf47e4f);
	style7_list_2.body.radius = 0;
	style7_list_2.text.line_space = 2;

	ui->list_2 = lv_list_create(ui->cont, NULL);
	lv_obj_set_pos(ui->list_2, 200, 94);
	lv_obj_set_size(ui->list_2, 600, 386);
	lv_list_set_single_mode(ui->list_2, true);
	lv_list_set_scroll_propagation(ui->list_2, false);
	lv_list_set_edge_flash(ui->list_2, true);
	lv_list_set_style(ui->list_2, LV_LIST_STYLE_BG, &style0_list_2);
	lv_list_set_style(ui->list_2, LV_LIST_STYLE_SCRL, &style1_list_2);
	lv_list_set_style(ui->list_2, LV_LIST_STYLE_EDGE_FLASH, &style3_list_2);
	lv_list_set_style(ui->list_2, LV_LIST_STYLE_BTN_REL, &style4_list_2);
	lv_list_set_style(ui->list_2, LV_LIST_STYLE_BTN_PR, &style5_list_2);
	lv_list_set_style(ui->list_2, LV_LIST_STYLE_BTN_TGL_REL, &style6_list_2);
	lv_list_set_style(ui->list_2, LV_LIST_STYLE_BTN_TGL_PR, &style7_list_2);
#endif // LV_USE_LIST

#ifdef LV_USE_CONT
	lv_style_copy(&style0_medium_cont, &lv_style_pretty);
	style0_medium_cont.body.main_color = lv_color_hex(0xe7e5e4);
	style0_medium_cont.body.grad_color = lv_color_hex(0xe7e5e4);
	style0_medium_cont.body.radius = 0;
	style0_medium_cont.body.border.color = lv_color_hex(0x000000);
	style0_medium_cont.body.border.part = 8;

	ui->medium_cont = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->medium_cont, 0, 54);
	lv_obj_set_size(ui->medium_cont, 200, 262);
	lv_cont_set_fit4(ui->medium_cont, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->medium_cont, LV_CONT_STYLE_MAIN, &style0_medium_cont);
#endif // LV_USE_CONT

#ifdef LV_USE_LIST
	lv_style_copy(&style0_medium_list_1, &lv_style_transp_fit);
	style0_medium_list_1.body.border.opa = 0;

	lv_style_copy(&style1_medium_list_1, &lv_style_pretty);
	style1_medium_list_1.body.radius = 0;
	style1_medium_list_1.body.padding.top = 0;
	style1_medium_list_1.body.padding.bottom = 0;
	style1_medium_list_1.body.padding.left = 0;
	style1_medium_list_1.body.padding.right = 0;
	style1_medium_list_1.body.padding.inner = 0;

	lv_style_copy(&style2_medium_list_1, &lv_style_pretty_color);
	style2_medium_list_1.body.border.opa = 0;

	lv_style_copy(&style3_medium_list_1, &lv_style_transp);
	style3_medium_list_1.body.main_color = lv_color_hex(0x5596d8);
	style3_medium_list_1.body.grad_color = lv_color_hex(0x5596d8);
	style3_medium_list_1.body.radius = 0;
	style3_medium_list_1.body.opa = 255;
	style3_medium_list_1.body.border.color = lv_color_hex(0x000000);
	style3_medium_list_1.body.border.width = 0;
	style3_medium_list_1.body.border.part = 15;
	style3_medium_list_1.body.border.opa = 255;
	style3_medium_list_1.body.shadow.color = lv_color_hex(0x808080);
	style3_medium_list_1.body.shadow.width = 0;
	style3_medium_list_1.body.shadow.type = 1;
	style3_medium_list_1.body.padding.top = 5;
	style3_medium_list_1.body.padding.bottom = 5;
	style3_medium_list_1.body.padding.left = 5;
	style3_medium_list_1.body.padding.right = 5;
	style3_medium_list_1.body.padding.inner = 5;
	style3_medium_list_1.text.color = lv_color_hex(0xf0f0f0);
	style3_medium_list_1.text.sel_color = lv_color_hex(0x5596d8);
	style3_medium_list_1.text.font = &lv_font_roboto_16;
	style3_medium_list_1.text.letter_space = 0;
	style3_medium_list_1.text.line_space = 2;
	style3_medium_list_1.text.opa = 255;
	style3_medium_list_1.image.color = lv_color_hex(0xf0f0f0);
	style3_medium_list_1.image.intense = 0;
	style3_medium_list_1.image.opa = 255;

	lv_style_copy(&style4_medium_list_1, &lv_style_btn_rel);
	style4_medium_list_1.body.main_color = lv_color_hex(0xc4c4c7);
	style4_medium_list_1.body.grad_color = lv_color_hex(0xc4c4c7);
	style4_medium_list_1.body.radius = 0;
	style4_medium_list_1.body.border.color = lv_color_hex(0x404040);
	style4_medium_list_1.body.border.part = 8;
	style4_medium_list_1.body.border.opa = 76;
	style4_medium_list_1.text.color = lv_color_hex(0x000000);
	style4_medium_list_1.text.line_space = 2;

	lv_style_copy(&style5_medium_list_1, &lv_style_btn_pr);
	style5_medium_list_1.body.main_color = lv_color_hex(0xeb703e);
	style5_medium_list_1.body.grad_color = lv_color_hex(0xeb703e);
	style5_medium_list_1.body.radius = 0;
	style5_medium_list_1.body.border.color = lv_color_hex(0xb0b0b0);
	style5_medium_list_1.body.border.part = 1;
	style5_medium_list_1.text.line_space = 2;

	lv_style_copy(&style6_medium_list_1, &lv_style_btn_tgl_rel);
	style6_medium_list_1.body.main_color = lv_color_hex(0xeb703e);
	style6_medium_list_1.body.grad_color = lv_color_hex(0xeb703e);
	style6_medium_list_1.body.radius = 0;
	style6_medium_list_1.body.border.color = lv_color_hex(0xb0b0b0);
	style6_medium_list_1.body.border.part = 9;
	style6_medium_list_1.text.line_space = 2;

	lv_style_copy(&style7_medium_list_1, &lv_style_btn_tgl_pr);
	style7_medium_list_1.body.main_color = lv_color_hex(0xeb703e);
	style7_medium_list_1.body.grad_color = lv_color_hex(0xeb703e);
	style7_medium_list_1.body.radius = 0;
	style7_medium_list_1.body.border.color = lv_color_hex(0xb0b0b0);
	style7_medium_list_1.body.border.part = 1;
	style7_medium_list_1.text.line_space = 2;

	ui->medium_list_1 = lv_list_create(ui->medium_cont, NULL);
	lv_obj_set_pos(ui->medium_list_1, 0, 40);
	lv_obj_set_size(ui->medium_list_1, 200, 222);
	lv_list_set_single_mode(ui->medium_list_1, true);
	lv_list_set_scroll_propagation(ui->medium_list_1, false);
	lv_list_set_edge_flash(ui->medium_list_1, false);
	lv_list_set_style(ui->medium_list_1, LV_LIST_STYLE_BG, &style0_medium_list_1);
	lv_list_set_style(ui->medium_list_1, LV_LIST_STYLE_SCRL, &style1_medium_list_1);
	lv_list_set_style(ui->medium_list_1, LV_LIST_STYLE_SB, &style2_medium_list_1);
	lv_list_set_style(ui->medium_list_1, LV_LIST_STYLE_EDGE_FLASH, &style3_medium_list_1);
	lv_list_set_style(ui->medium_list_1, LV_LIST_STYLE_BTN_REL, &style4_medium_list_1);
	lv_list_set_style(ui->medium_list_1, LV_LIST_STYLE_BTN_PR, &style5_medium_list_1);
	lv_list_set_style(ui->medium_list_1, LV_LIST_STYLE_BTN_TGL_REL, &style6_medium_list_1);
	lv_list_set_style(ui->medium_list_1, LV_LIST_STYLE_BTN_TGL_PR, &style7_medium_list_1);
#endif // LV_USE_LIST

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_7, &lv_style_pretty);
	style0_container_7.body.radius = 0;

	ui->container_7 = lv_cont_create(ui->medium_cont, NULL);
	lv_obj_set_pos(ui->container_7, 0, 0);
	lv_obj_set_size(ui->container_7, 200, 42);
	lv_cont_set_fit4(ui->container_7, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_7, LV_CONT_STYLE_MAIN, &style0_container_7);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	ui->label_2 = lv_label_create(ui->container_7, NULL);
	lv_label_set_text(ui->label_2, "Medium");
	lv_label_set_long_mode(ui->label_2, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_2, 70, 13);
	lv_obj_set_size(ui->label_2, 59, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_3, &lv_style_pretty);
	style0_container_3.body.main_color = lv_color_hex(0x53514a);
	style0_container_3.body.grad_color = lv_color_hex(0x53514a);
	style0_container_3.body.radius = 0;
	style0_container_3.body.border.color = lv_color_hex(0xa3a3a3);
	style0_container_3.body.border.part = 1;
	style0_container_3.body.border.opa = 255;

	ui->container_3 = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->container_3, 0, 0);
	lv_obj_set_size(ui->container_3, 800, 55);
	lv_cont_set_fit4(ui->container_3, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_3, LV_CONT_STYLE_MAIN, &style0_container_3);
#endif // LV_USE_CONT

#ifdef LV_USE_IMG
	lv_style_copy(&style0_image_1, &lv_style_plain);
	style0_image_1.image.color = lv_color_hex(0xffffff);
	style0_image_1.image.intense = 255;

	ui->image_1 = lv_img_create(ui->container_3, NULL);
	lv_obj_set_pos(ui->image_1, 9, 12);
	lv_obj_set_size(ui->image_1, 32, 32);
	image_1_home_return = (void *)parse_image_form_file(LV_IMAGE_PATH"home_return.png");
	lv_img_set_src(ui->image_1, image_1_home_return);

	lv_img_set_style(ui->image_1, LV_IMG_STYLE_MAIN, &style0_image_1);
#endif // LV_USE_IMG

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_5, &lv_style_transp);
	style0_label_5.text.color = lv_color_hex(0xffffff);
	style0_label_5.text.font = &lv_font_roboto_22;
	style0_label_5.text.line_space = 2;

	ui->label_5 = lv_label_create(ui->container_3, NULL);
	lv_label_set_text(ui->label_5, "Explorer");
	lv_label_set_long_mode(ui->label_5, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_5, 46, 15);
	lv_obj_set_size(ui->label_5, 80, 26);
	lv_label_set_style(ui->label_5, LV_LABEL_STYLE_MAIN, &style0_label_5);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_1, &lv_style_pretty);
	style0_container_1.body.main_color = lv_color_hex(0xe7e5e4);
	style0_container_1.body.grad_color = lv_color_hex(0xe7e5e4);
	style0_container_1.body.radius = 0;

	ui->container_1 = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->container_1, 0, 315);
	lv_obj_set_size(ui->container_1, 200, 165);
	lv_cont_set_fit4(ui->container_1, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_1, LV_CONT_STYLE_MAIN, &style0_container_1);
#endif // LV_USE_CONT

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_5, &lv_style_pretty);
	style0_container_5.body.radius = 0;

	ui->container_5 = lv_cont_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->container_5, 0, 0);
	lv_obj_set_size(ui->container_5, 200, 39);
	lv_cont_set_fit4(ui->container_5, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_5, LV_CONT_STYLE_MAIN, &style0_container_5);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	ui->label_1 = lv_label_create(ui->container_5, NULL);
	lv_label_set_text(ui->label_1, "Information");
	lv_label_set_long_mode(ui->label_1, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_1, 59, 7);
	lv_obj_set_size(ui->label_1, 83, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_4 = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->label_4, "File Size:");
	lv_label_set_long_mode(ui->label_4, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_4, 36, 67);
	lv_obj_set_size(ui->label_4, 65, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_7 = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->label_7, "Modified time:");
	lv_label_set_long_mode(ui->label_7, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_7, 42, 98);
	lv_obj_set_size(ui->label_7, 104, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_6 = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->label_6, "Text");
	lv_label_set_long_mode(ui->label_6, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_6, 118, 67);
	lv_obj_set_size(ui->label_6, 31, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_8 = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->label_8, "1970/1/1 08:00:00");
	lv_label_set_long_mode(ui->label_8, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_8, 32, 126);
	lv_obj_set_size(ui->label_8, 134, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_CONT
	lv_style_copy(&style0_direct_cont, &lv_style_pretty);
	style0_direct_cont.body.radius = 0;
	style0_direct_cont.body.border.part = 3;

	ui->direct_cont = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->direct_cont, 200, 53);
	lv_obj_set_size(ui->direct_cont, 600, 42);
	lv_cont_set_layout(ui->direct_cont, LV_LAYOUT_ROW_M);
	lv_cont_set_fit4(ui->direct_cont, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->direct_cont, LV_CONT_STYLE_MAIN, &style0_direct_cont);
#endif // LV_USE_CONT

#ifdef LV_USE_IMG
	ui->image_3 = lv_img_create(ui->direct_cont, NULL);
	lv_obj_set_pos(ui->image_3, 5, 5);
	lv_obj_set_size(ui->image_3, 32, 32);
	image_3_explorer_opendir = (void *)parse_image_form_file(LV_IMAGE_PATH"explorer_opendir.png");
	lv_img_set_src(ui->image_3, image_3_explorer_opendir);

#endif // LV_USE_IMG
}

void explorer_auto_ui_destory(explorer_ui_t *ui)
{
	lv_obj_clean(ui->cont);
	free_image(image_1_home_return);
	free_image(image_3_explorer_opendir);
	free_image(image_2_explorer_return);
}

/**********************
 *      includes
 **********************/
#include "ui_folder.h"
#include "lvgl.h"
#include "common.h"
#include "ui_resource.h"


/**********************
 *       variables
 **********************/
static lv_style_t style_screen;
static lv_style_t style0_container_1;
static lv_style_t style0_image_1;
static lv_style_t style0_label_1;
static lv_style_t style1_list_1;
static lv_style_t style4_list_1;
static lv_style_t style5_list_1;
static lv_style_t style6_list_1;
static lv_style_t style7_list_1;
static lv_style_t style0_direct_cont;
static lv_style_t style0_page_1;
static lv_style_t style1_page_1;

/**********************
 *  images and fonts
 **********************/
static void *image_1_home_return_png = NULL;
static void *image_3_explorer_return_png = NULL;
static void *image_2_explorer_fileinfo_png = NULL;
static void *image_4_explorer_opendir_png = NULL;

/**********************
 *  functions
 **********************/
void folder_auto_ui_create(folder_ui_t *ui)
{
	lv_style_copy(&style_screen, &lv_style_scr);
	style_screen.body.main_color = lv_color_hex(0xcbcbcb);
	style_screen.body.grad_color = lv_color_hex(0xcbcbcb);
	lv_obj_set_style(ui->cont, &style_screen);

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_1, &lv_style_pretty);
	style0_container_1.body.main_color = lv_color_hex(0x53514a);
	style0_container_1.body.grad_color = lv_color_hex(0x53514a);
	style0_container_1.body.radius = 0;
	style0_container_1.body.border.width = 0;

	ui->container_1 = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->container_1, 0, 0);
	lv_obj_set_size(ui->container_1, 800, 46);
	lv_cont_set_fit4(ui->container_1, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_1, LV_CONT_STYLE_MAIN, &style0_container_1);
#endif // LV_USE_CONT

#ifdef LV_USE_IMG
	lv_style_copy(&style0_image_1, &lv_style_plain);
	style0_image_1.image.color = lv_color_hex(0xffffff);
	style0_image_1.image.intense = 255;

	ui->image_1 = lv_img_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->image_1, 8, 9);
	lv_obj_set_size(ui->image_1, 32, 32);
	image_1_home_return_png = (void *)parse_image_form_file(LV_IMAGE_PATH"home_return.png");
	lv_img_set_src(ui->image_1, image_1_home_return_png);

	lv_img_set_style(ui->image_1, LV_IMG_STYLE_MAIN, &style0_image_1);
#endif // LV_USE_IMG

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_1, &lv_style_transp);
	style0_label_1.text.color = lv_color_hex(0xffffff);
	style0_label_1.text.font = &lv_font_roboto_22;
	style0_label_1.text.line_space = 2;

	ui->label_1 = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->label_1, "Explorer");
	lv_label_set_long_mode(ui->label_1, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_1, 42, 11);
	lv_obj_set_size(ui->label_1, 80, 26);
	lv_label_set_style(ui->label_1, LV_LABEL_STYLE_MAIN, &style0_label_1);
#endif // LV_USE_LABEL

#ifdef LV_USE_LIST
	lv_style_copy(&style1_list_1, &lv_style_pretty);
	style1_list_1.body.radius = 0;
	style1_list_1.body.border.width = 0;
	style1_list_1.body.padding.top = 0;
	style1_list_1.body.padding.bottom = 0;
	style1_list_1.body.padding.left = 0;
	style1_list_1.body.padding.right = 0;
	style1_list_1.body.padding.inner = 0;

	lv_style_copy(&style4_list_1, &lv_style_btn_rel);
	style4_list_1.body.main_color = lv_color_hex(0xc4c4c7);
	style4_list_1.body.grad_color = lv_color_hex(0xc4c4c7);
	style4_list_1.body.radius = 0;
	style4_list_1.body.border.width = 0;
	style4_list_1.text.color = lv_color_hex(0x000000);
	style4_list_1.text.line_space = 2;

	lv_style_copy(&style5_list_1, &lv_style_btn_pr);
	style5_list_1.body.main_color = lv_color_hex(0xf47e4f);
	style5_list_1.body.grad_color = lv_color_hex(0xf47e4f);
	style5_list_1.body.radius = 0;
	style5_list_1.body.border.width = 0;
	style5_list_1.text.color = lv_color_hex(0xffffff);
	style5_list_1.text.line_space = 2;

	lv_style_copy(&style6_list_1, &lv_style_btn_tgl_rel);
	style6_list_1.body.main_color = lv_color_hex(0xf47e4f);
	style6_list_1.body.grad_color = lv_color_hex(0xf47e4f);
	style6_list_1.body.radius = 0;
	style6_list_1.body.border.width = 0;
	style6_list_1.text.color = lv_color_hex(0xffffff);
	style6_list_1.text.line_space = 2;

	lv_style_copy(&style7_list_1, &lv_style_btn_tgl_pr);
	style7_list_1.body.main_color = lv_color_hex(0xf47e4f);
	style7_list_1.body.grad_color = lv_color_hex(0xf47e4f);
	style7_list_1.body.radius = 0;
	style7_list_1.text.color = lv_color_hex(0xffffff);
	style7_list_1.text.line_space = 2;

	ui->list_1 = lv_list_create(ui->cont, NULL);
	lv_obj_set_pos(ui->list_1, 0, 88);
	lv_obj_set_size(ui->list_1, 800, 345);
	lv_list_set_single_mode(ui->list_1, false);
	lv_list_set_scroll_propagation(ui->list_1, false);
	lv_list_set_edge_flash(ui->list_1, true);
	lv_list_set_style(ui->list_1, LV_LIST_STYLE_SCRL, &style1_list_1);
	lv_list_set_style(ui->list_1, LV_LIST_STYLE_BTN_REL, &style4_list_1);
	lv_list_set_style(ui->list_1, LV_LIST_STYLE_BTN_PR, &style5_list_1);
	lv_list_set_style(ui->list_1, LV_LIST_STYLE_BTN_TGL_REL, &style6_list_1);
	lv_list_set_style(ui->list_1, LV_LIST_STYLE_BTN_TGL_PR, &style7_list_1);
#endif // LV_USE_LIST

#ifdef LV_USE_CONT
	lv_style_copy(&style0_direct_cont, &lv_style_pretty);
	style0_direct_cont.body.radius = 0;
	style0_direct_cont.body.border.part = 1;

	ui->direct_cont = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->direct_cont, 0, 46);
	lv_obj_set_size(ui->direct_cont, 800, 43);
	lv_cont_set_fit4(ui->direct_cont, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->direct_cont, LV_CONT_STYLE_MAIN, &style0_direct_cont);
#endif // LV_USE_CONT

#ifdef LV_USE_IMG
	ui->image_3 = lv_img_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_3, 760, 52);
	lv_obj_set_size(ui->image_3, 32, 32);
	image_3_explorer_return_png = (void *)parse_image_form_file(LV_IMAGE_PATH"explorer_return.png");
	lv_img_set_src(ui->image_3, image_3_explorer_return_png);

#endif // LV_USE_IMG

#ifdef LV_USE_PAGE
	lv_style_copy(&style0_page_1, &lv_style_pretty_color);
	style0_page_1.body.main_color = lv_color_hex(0xffffff);
	style0_page_1.body.grad_color = lv_color_hex(0xffffff);
	style0_page_1.body.radius = 0;
	style0_page_1.body.border.width = 0;
	style0_page_1.body.padding.top = 0;
	style0_page_1.body.padding.bottom = 0;
	style0_page_1.body.padding.left = 0;
	style0_page_1.body.padding.right = 0;
	style0_page_1.body.padding.inner = 0;

	lv_style_copy(&style1_page_1, &lv_style_pretty);
	style1_page_1.body.grad_color = lv_color_hex(0xffffff);
	style1_page_1.body.radius = 0;
	style1_page_1.body.border.part = 2;
	style1_page_1.body.padding.top = 0;
	style1_page_1.body.padding.bottom = 0;
	style1_page_1.body.padding.left = 0;
	style1_page_1.body.padding.right = 0;
	style1_page_1.body.padding.inner = 0;

	ui->page_1 = lv_page_create(ui->cont, NULL);
	lv_obj_set_pos(ui->page_1, 0, 434);
	lv_obj_set_size(ui->page_1, 800, 46);
	lv_page_set_sb_mode(ui->page_1, LV_SB_MODE_AUTO);
	lv_page_set_edge_flash(ui->page_1, false);
	lv_page_set_scroll_propagation(ui->page_1, false);
	lv_page_set_style(ui->page_1, LV_PAGE_STYLE_BG, &style0_page_1);
	lv_page_set_style(ui->page_1, LV_PAGE_STYLE_SCRL, &style1_page_1);
#endif // LV_USE_PAGE

#ifdef LV_USE_LABEL
	ui->label_2 = lv_label_create(ui->page_1, NULL);
	lv_label_set_text(ui->label_2, "Filename:");
	lv_label_set_long_mode(ui->label_2, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_2, 56, 5);
	lv_obj_set_size(ui->label_2, 71, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_3 = lv_label_create(ui->page_1, NULL);
	lv_label_set_text(ui->label_3, "Filename.txt");
	lv_label_set_long_mode(ui->label_3, LV_LABEL_LONG_BREAK);
	lv_obj_set_pos(ui->label_3, 144, 6);
	lv_obj_set_size(ui->label_3, 624, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_4 = lv_label_create(ui->page_1, NULL);
	lv_label_set_text(ui->label_4, "Modified time:");
	lv_label_set_long_mode(ui->label_4, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_4, 55, 27);
	lv_obj_set_size(ui->label_4, 104, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_IMG
	ui->image_2 = lv_img_create(ui->page_1, NULL);
	lv_obj_set_pos(ui->image_2, 8, 8);
	lv_obj_set_size(ui->image_2, 32, 32);
	image_2_explorer_fileinfo_png = (void *)parse_image_form_file(LV_IMAGE_PATH"explorer_fileinfo.png");
	lv_img_set_src(ui->image_2, image_2_explorer_fileinfo_png);

#endif // LV_USE_IMG

#ifdef LV_USE_LABEL
	ui->label_5 = lv_label_create(ui->page_1, NULL);
	lv_label_set_text(ui->label_5, "2020/9/14 08:00:00");
	lv_label_set_long_mode(ui->label_5, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_5, 169, 27);
	lv_obj_set_size(ui->label_5, 143, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_6 = lv_label_create(ui->page_1, NULL);
	lv_label_set_text(ui->label_6, "Size:");
	lv_label_set_long_mode(ui->label_6, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_6, 364, 27);
	lv_obj_set_size(ui->label_6, 35, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	ui->label_7 = lv_label_create(ui->page_1, NULL);
	lv_label_set_text(ui->label_7, "0.0M");
	lv_label_set_long_mode(ui->label_7, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_7, 413, 27);
	lv_obj_set_size(ui->label_7, 36, 19);
#endif // LV_USE_LABEL

#ifdef LV_USE_IMG
	ui->image_4 = lv_img_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_4, 8, 51);
	lv_obj_set_size(ui->image_4, 32, 32);
	image_4_explorer_opendir_png = (void *)parse_image_form_file(LV_IMAGE_PATH"explorer_opendir.png");
	lv_img_set_src(ui->image_4, image_4_explorer_opendir_png);

#endif // LV_USE_IMG

}

void folder_auto_ui_destory(folder_ui_t *ui)
{
	lv_obj_clean(ui->cont);
	free_image(image_1_home_return_png);
	free_image(image_3_explorer_return_png);
	free_image(image_2_explorer_fileinfo_png);
	free_image(image_4_explorer_opendir_png);
}

/**********************
 *      includes
 **********************/
#include "ui_calendar.h"
#include "lvgl.h"
#include "common.h"
#include "ui_resource.h"


/**********************
 *       variables
 **********************/
static lv_style_t style_screen;
static lv_style_t style0_calendar_1;
static lv_style_t style1_calendar_1;
static lv_style_t style3_calendar_1;
static lv_style_t style4_calendar_1;
static lv_style_t style5_calendar_1;
static lv_style_t style6_calendar_1;
static lv_style_t style7_calendar_1;
static lv_style_t style0_container_1;
static lv_style_t style0_label_1;
static lv_style_t style0_image_1;
static lv_style_t style0_container_2;
static lv_style_t style0_label_2;
static lv_style_t style0_label_3;
static lv_style_t style0_label_4;
static lv_style_t style0_image_2;

/**********************
 *  images and fonts
 **********************/
static void *image_1_home_return_png = NULL;
static void *image_2_calendar_set_png = NULL;

/**********************
 *  functions
 **********************/
void calendar_auto_ui_create(calendar_ui_t *ui)
{
	lv_style_copy(&style_screen, &lv_style_scr);
	style_screen.body.main_color = lv_color_hex(0x00557f);
	style_screen.body.grad_color = lv_color_hex(0x00557f);
	lv_obj_set_style(ui->cont, &style_screen);

#ifdef LV_USE_CALENDAR
	lv_style_copy(&style0_calendar_1, &lv_style_pretty);
	style0_calendar_1.body.main_color = lv_color_hex(0x00906a);
	style0_calendar_1.body.grad_color = lv_color_hex(0xaaaa7f);
	style0_calendar_1.body.radius = 10;
	style0_calendar_1.body.padding.inner = 20;
	style0_calendar_1.text.line_space = 2;

	lv_style_copy(&style1_calendar_1, &lv_style_plain_color);
	style1_calendar_1.body.opa = 0;
	style1_calendar_1.body.padding.top = 20;
	style1_calendar_1.body.padding.bottom = 20;
	style1_calendar_1.body.padding.left = 20;
	style1_calendar_1.body.padding.right = 20;
	style1_calendar_1.text.line_space = 2;

	lv_style_copy(&style3_calendar_1, &lv_style_pretty);
	style3_calendar_1.text.color = lv_color_hex(0x000000);
	style3_calendar_1.text.font = &lv_font_roboto_28;
	style3_calendar_1.text.line_space = 2;

	lv_style_copy(&style4_calendar_1, &lv_style_plain_color);
	style4_calendar_1.text.color = lv_color_hex(0xaa0000);
	style4_calendar_1.text.line_space = 2;

	lv_style_copy(&style5_calendar_1, &lv_style_btn_ina);
	style5_calendar_1.text.color = lv_color_hex(0x555500);
	style5_calendar_1.text.line_space = 2;

	lv_style_copy(&style6_calendar_1, &lv_style_plain_color);
	style6_calendar_1.body.main_color = lv_color_hex(0x55aa7f);
	style6_calendar_1.body.grad_color = lv_color_hex(0x55aa7f);
	style6_calendar_1.text.color = lv_color_hex(0x55ff7f);
	style6_calendar_1.text.line_space = 2;

	lv_style_copy(&style7_calendar_1, &lv_style_pretty_color);
	style7_calendar_1.body.main_color = lv_color_hex(0x3e7c5c);
	style7_calendar_1.body.grad_color = lv_color_hex(0x438865);
	style7_calendar_1.text.color = lv_color_hex(0x00ffff);
	style7_calendar_1.text.line_space = 2;

	ui->calendar_1 = lv_calendar_create(ui->cont, NULL);
	lv_obj_set_pos(ui->calendar_1, 412, 104);
	lv_obj_set_size(ui->calendar_1, 357, 350);
	
	lv_calendar_date_t calendar_1_today_date;
	calendar_1_today_date.year  = 2018;
	calendar_1_today_date.month = 1;
	calendar_1_today_date.day   = 1;
	lv_calendar_set_today_date(ui->calendar_1,&calendar_1_today_date);
	
	lv_calendar_date_t calendar_1_show_date;
	calendar_1_show_date.year  = 2018;
	calendar_1_show_date.month = 1;
	calendar_1_show_date.day   = 1;
	lv_calendar_set_showed_date(ui->calendar_1,&calendar_1_show_date);
	
	static lv_calendar_date_t calendar_1_highlight_date;
	calendar_1_highlight_date.year  = 2018;
	calendar_1_highlight_date.month = 1;
	calendar_1_highlight_date.day   = 1;
	lv_calendar_set_highlighted_dates(ui->calendar_1,&calendar_1_highlight_date,1);
	
	lv_calendar_set_style(ui->calendar_1, LV_CALENDAR_STYLE_BG, &style0_calendar_1);
	lv_calendar_set_style(ui->calendar_1, LV_CALENDAR_STYLE_HEADER, &style1_calendar_1);
	lv_calendar_set_style(ui->calendar_1, LV_CALENDAR_STYLE_DAY_NAMES, &style3_calendar_1);
	lv_calendar_set_style(ui->calendar_1, LV_CALENDAR_STYLE_HIGHLIGHTED_DAYS, &style4_calendar_1);
	lv_calendar_set_style(ui->calendar_1, LV_CALENDAR_STYLE_INACTIVE_DAYS, &style5_calendar_1);
	lv_calendar_set_style(ui->calendar_1, LV_CALENDAR_STYLE_WEEK_BOX, &style6_calendar_1);
	lv_calendar_set_style(ui->calendar_1, LV_CALENDAR_STYLE_TODAY_BOX, &style7_calendar_1);
#endif // LV_USE_CALENDAR

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_1, &lv_style_pretty);
	style0_container_1.body.main_color = lv_color_hex(0x00aaff);
	style0_container_1.body.grad_color = lv_color_hex(0x0073a8);
	style0_container_1.body.radius = 40;
	style0_container_1.body.border.width = 0;

	ui->container_1 = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->container_1, 169, 13);
	lv_obj_set_size(ui->container_1, 470, 71);
	lv_cont_set_fit4(ui->container_1, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_1, LV_CONT_STYLE_MAIN, &style0_container_1);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_1, &lv_style_transp);
	style0_label_1.text.color = lv_color_hex(0xffaa00);
	style0_label_1.text.font = &microsoft_yahei_bold_28_4;
	style0_label_1.text.line_space = 2;

	ui->label_1 = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->label_1, "What do you want to do today?");
	lv_label_set_long_mode(ui->label_1, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_1, 13, 16);
	lv_obj_set_size(ui->label_1, 444, 37);
	lv_label_set_style(ui->label_1, LV_LABEL_STYLE_MAIN, &style0_label_1);
#endif // LV_USE_LABEL

#ifdef LV_USE_IMG
	lv_style_copy(&style0_image_1, &lv_style_plain);
	style0_image_1.image.color = lv_color_hex(0xffffff);
	style0_image_1.image.intense = 255;

	ui->image_1 = lv_img_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_1, 22, 14);
	lv_obj_set_size(ui->image_1, 32, 32);
	image_1_home_return_png = (void *)parse_image_form_file(LV_IMAGE_PATH"home_return.png");
	lv_img_set_src(ui->image_1, image_1_home_return_png);

	lv_img_set_style(ui->image_1, LV_IMG_STYLE_MAIN, &style0_image_1);
#endif // LV_USE_IMG

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_2, &lv_style_pretty);
	style0_container_2.body.main_color = lv_color_hex(0xaaaa7f);
	style0_container_2.body.grad_color = lv_color_hex(0x00aa7f);
	style0_container_2.body.radius = 10;

	ui->container_2 = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->container_2, 31, 106);
	lv_obj_set_size(ui->container_2, 352, 349);
	lv_cont_set_fit4(ui->container_2, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_2, LV_CONT_STYLE_MAIN, &style0_container_2);
#endif // LV_USE_CONT

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_2, &lv_style_transp);
	style0_label_2.body.main_color = lv_color_hex(0x005500);
	style0_label_2.body.shadow.color = lv_color_hex(0x005500);
	style0_label_2.text.color = lv_color_hex(0x000000);
	style0_label_2.text.font = &microsoft_yahei_bold_28_4;
	style0_label_2.text.line_space = 2;

	ui->label_2 = lv_label_create(ui->container_2, NULL);
	lv_label_set_text(ui->label_2, "2018年1月");
	lv_label_set_long_mode(ui->label_2, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_2, 108, 26);
	lv_obj_set_size(ui->label_2, 141, 37);
	lv_label_set_style(ui->label_2, LV_LABEL_STYLE_MAIN, &style0_label_2);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_3, &lv_style_transp);
	style0_label_3.text.font = &microsoft_yahei_bold_100_4;
	style0_label_3.text.line_space = 2;

	ui->label_3 = lv_label_create(ui->container_2, NULL);
	lv_label_set_text(ui->label_3, "1");
	lv_label_set_long_mode(ui->label_3, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_3, 147, 92);
	lv_obj_set_size(ui->label_3, 62, 132);
	lv_label_set_style(ui->label_3, LV_LABEL_STYLE_MAIN, &style0_label_3);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_4, &lv_style_transp);
	style0_label_4.text.font = &microsoft_yahei_bold_28_4;
	style0_label_4.text.line_space = 2;

	ui->label_4 = lv_label_create(ui->container_2, NULL);
	lv_label_set_text(ui->label_4, "星期五");
	lv_label_set_long_mode(ui->label_4, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->label_4, 136, 253);
	lv_obj_set_size(ui->label_4, 84, 37);
	lv_label_set_style(ui->label_4, LV_LABEL_STYLE_MAIN, &style0_label_4);
#endif // LV_USE_LABEL

#ifdef LV_USE_IMG
	lv_style_copy(&style0_image_2, &lv_style_plain);
	style0_image_2.image.color = lv_color_hex(0x00aa7f);
	style0_image_2.image.intense = 255;

	ui->image_2 = lv_img_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_2, 721, 14);
	lv_obj_set_size(ui->image_2, 64, 64);
	image_2_calendar_set_png = (void *)parse_image_form_file(LV_IMAGE_PATH"calendar_set.png");
	lv_img_set_src(ui->image_2, image_2_calendar_set_png);

	lv_img_set_style(ui->image_2, LV_IMG_STYLE_MAIN, &style0_image_2);
#endif // LV_USE_IMG

}

void calendar_auto_ui_destory(calendar_ui_t *ui)
{
	lv_obj_clean(ui->cont);
	free_image(image_1_home_return_png);
	free_image(image_2_calendar_set_png);
}

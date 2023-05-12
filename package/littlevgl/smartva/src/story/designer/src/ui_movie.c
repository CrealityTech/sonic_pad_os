/**********************
 *      includes
 **********************/
#include "ui_movie.h"
#include "lvgl.h"
#include "common.h"
#include "ui_resource.h"


/**********************
 *       variables
 **********************/
static lv_style_t style_screen;
static lv_style_t style0_media_list;
static lv_style_t style1_media_list;
static lv_style_t style2_media_list;
static lv_style_t style4_media_list;
static lv_style_t style5_media_list;
static lv_style_t style6_media_list;
static lv_style_t style7_media_list;
static lv_style_t style0_container_1;
static lv_style_t style0_progressbar;
static lv_style_t style1_progressbar;
static lv_style_t style2_progressbar;
static lv_style_t style0_volume_bar;
static lv_style_t style1_volume_bar;
static lv_style_t style2_volume_bar;
static lv_style_t style0_curr_time;
static lv_style_t style0_total_time;
static lv_style_t style0_file_name;
static lv_style_t style0_download;
static lv_style_t style0_label_1;
static lv_style_t style0_label_2;
static lv_style_t style0_order;
static lv_style_t style0_full_button;
static lv_style_t style1_full_button;
static lv_style_t style2_full_button;
static lv_style_t style3_full_button;

/**********************
 *  images and fonts
 **********************/
static void *image_4_movie_big_png = NULL;
static void *play_music_play_png_state0 = NULL;
static void *play_music_play_png_state1 = NULL;
static void *play_music_pause_png_state2 = NULL;
static void *play_music_pause_png_state3 = NULL;
static void *next_music_next_png_state0 = NULL;
static void *next_music_next_png_state1 = NULL;
static void *last_music_prev_png_state0 = NULL;
static void *last_music_prev_png_state1 = NULL;
static void *loop_music_cycle_png_state0 = NULL;
static void *loop_music_cycle_png_state1 = NULL;
static void *loop_music_loop_png_state2 = NULL;
static void *loop_music_loop_png_state3 = NULL;
static void *volume_music_vol_png_state0 = NULL;
static void *volume_music_vol_png_state2 = NULL;
static void *order_music_order_png_state0 = NULL;
static void *order_music_order_png_state1 = NULL;
static void *back_home_return_png_state0 = NULL;
static void *back_home_return_png_state3 = NULL;
static void *media_list_1_movie_item_png = NULL;
static void *media_list_2_movie_item_png = NULL;
static void *media_list_3_movie_item_png = NULL;
static void *media_list_4_movie_item_png = NULL;
static void *media_list_5_movie_item_png = NULL;
static void *media_list_6_movie_item_png = NULL;

/**********************
 *  functions
 **********************/
void movie_auto_ui_create(movie_ui_t *ui)
{
	lv_style_copy(&style_screen, &lv_style_scr);
	style_screen.body.main_color = lv_color_hex(0xff557f);
	style_screen.body.grad_color = lv_color_hex(0xff557f);
	lv_obj_set_style(ui->cont, &style_screen);

#ifdef LV_USE_LIST
	lv_style_copy(&style0_media_list, &lv_style_transp_fit);
	style0_media_list.body.main_color = lv_color_hex(0xff55ff);

	lv_style_copy(&style1_media_list, &lv_style_pretty);
	style1_media_list.body.main_color = lv_color_hex(0xff557f);
	style1_media_list.body.grad_color = lv_color_hex(0xff557f);
	style1_media_list.body.border.width = 0;

	lv_style_copy(&style2_media_list, &lv_style_pretty_color);
	style2_media_list.body.main_color = lv_color_hex(0xff557f);

	lv_style_copy(&style4_media_list, &lv_style_btn_rel);
	style4_media_list.body.main_color = lv_color_hex(0xffaaff);
	style4_media_list.body.grad_color = lv_color_hex(0xff55ff);
	style4_media_list.body.radius = 20;
	style4_media_list.text.line_space = 2;

	lv_style_copy(&style5_media_list, &lv_style_btn_pr);
	style5_media_list.body.main_color = lv_color_hex(0x55aaff);
	style5_media_list.body.grad_color = lv_color_hex(0x55aaff);
	style5_media_list.body.radius = 20;
	style5_media_list.text.line_space = 2;

	lv_style_copy(&style6_media_list, &lv_style_btn_tgl_rel);
	style6_media_list.body.main_color = lv_color_hex(0x55aaff);
	style6_media_list.body.grad_color = lv_color_hex(0x55aaff);
	style6_media_list.body.radius = 20;
	style6_media_list.text.line_space = 2;

	lv_style_copy(&style7_media_list, &lv_style_btn_tgl_pr);
	style7_media_list.body.main_color = lv_color_hex(0x55aaff);
	style7_media_list.body.grad_color = lv_color_hex(0x55aaff);
	style7_media_list.body.radius = 20;
	style7_media_list.text.line_space = 2;

	ui->media_list = lv_list_create(ui->cont, NULL);
	lv_obj_set_pos(ui->media_list, 382, 5);
	lv_obj_set_size(ui->media_list, 418, 417);
	
	media_list_1_movie_item_png = (void *)parse_image_form_file(LV_IMAGE_PATH"movie_item.png");
	lv_list_add_btn(ui->media_list, media_list_1_movie_item_png, "1");
	
	media_list_2_movie_item_png = (void *)parse_image_form_file(LV_IMAGE_PATH"movie_item.png");
	lv_list_add_btn(ui->media_list, media_list_2_movie_item_png, "2");
	
	media_list_3_movie_item_png = (void *)parse_image_form_file(LV_IMAGE_PATH"movie_item.png");
	lv_list_add_btn(ui->media_list, media_list_3_movie_item_png, "3");
	
	media_list_4_movie_item_png = (void *)parse_image_form_file(LV_IMAGE_PATH"movie_item.png");
	lv_list_add_btn(ui->media_list, media_list_4_movie_item_png, "4");
	
	media_list_5_movie_item_png = (void *)parse_image_form_file(LV_IMAGE_PATH"movie_item.png");
	lv_list_add_btn(ui->media_list, media_list_5_movie_item_png, "5");
	
	media_list_6_movie_item_png = (void *)parse_image_form_file(LV_IMAGE_PATH"movie_item.png");
	lv_list_add_btn(ui->media_list, media_list_6_movie_item_png, "6");
	lv_list_set_single_mode(ui->media_list, true);
	lv_list_set_scroll_propagation(ui->media_list, false);
	lv_list_set_edge_flash(ui->media_list, true);
	lv_list_set_style(ui->media_list, LV_LIST_STYLE_BG, &style0_media_list);
	lv_list_set_style(ui->media_list, LV_LIST_STYLE_SCRL, &style1_media_list);
	lv_list_set_style(ui->media_list, LV_LIST_STYLE_SB, &style2_media_list);
	lv_list_set_style(ui->media_list, LV_LIST_STYLE_BTN_REL, &style4_media_list);
	lv_list_set_style(ui->media_list, LV_LIST_STYLE_BTN_PR, &style5_media_list);
	lv_list_set_style(ui->media_list, LV_LIST_STYLE_BTN_TGL_REL, &style6_media_list);
	lv_list_set_style(ui->media_list, LV_LIST_STYLE_BTN_TGL_PR, &style7_media_list);
#endif // LV_USE_LIST

#ifdef LV_USE_IMG
	ui->image_4 = lv_img_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_4, 125, 166);
	lv_obj_set_size(ui->image_4, 128, 128);
	image_4_movie_big_png = (void *)parse_image_form_file(LV_IMAGE_PATH"movie_big.png");
	lv_img_set_src(ui->image_4, image_4_movie_big_png);

#endif // LV_USE_IMG

#ifdef LV_USE_CONT
	lv_style_copy(&style0_container_1, &lv_style_pretty);
	style0_container_1.body.radius = 0;

	ui->container_1 = lv_cont_create(ui->cont, NULL);
	lv_obj_set_pos(ui->container_1, 0, 425);
	lv_obj_set_size(ui->container_1, 800, 55);
	lv_cont_set_fit4(ui->container_1, LV_FIT_NONE, LV_FIT_NONE ,LV_FIT_NONE ,LV_FIT_NONE);
	lv_cont_set_style(ui->container_1, LV_CONT_STYLE_MAIN, &style0_container_1);
#endif // LV_USE_CONT

#ifdef LV_USE_IMGBTN
	ui->play = lv_imgbtn_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->play, 70, 3);
	lv_obj_set_size(ui->play, 48, 48);
	lv_imgbtn_set_state(ui->play, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->play, true);
	play_music_play_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_play.png");
	lv_imgbtn_set_src(ui->play, LV_BTN_STATE_REL, play_music_play_png_state0);
	play_music_play_png_state1 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_play.png");
	lv_imgbtn_set_src(ui->play, LV_BTN_STATE_PR, play_music_play_png_state1);
	play_music_pause_png_state2 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_pause.png");
	lv_imgbtn_set_src(ui->play, LV_BTN_STATE_TGL_REL, play_music_pause_png_state2);
	play_music_pause_png_state3 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_pause.png");
	lv_imgbtn_set_src(ui->play, LV_BTN_STATE_TGL_PR, play_music_pause_png_state3);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_SLIDER
	lv_style_copy(&style0_progressbar, &lv_style_pretty);

	lv_style_copy(&style1_progressbar, &lv_style_pretty_color);
	style1_progressbar.body.padding.top = 0;
	style1_progressbar.body.padding.bottom = 0;
	style1_progressbar.body.padding.left = 0;
	style1_progressbar.body.padding.right = 0;
	style1_progressbar.body.padding.inner = 0;

	lv_style_copy(&style2_progressbar, &lv_style_pretty);
	style2_progressbar.body.radius = 15;

	ui->progressbar = lv_slider_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->progressbar, 211, 14);
	lv_obj_set_size(ui->progressbar, 331, 20);
	lv_slider_set_range(ui->progressbar, 0, 1024);
	lv_slider_set_knob_in(ui->progressbar, false);
	lv_slider_set_value(ui->progressbar, 0, LV_ANIM_OFF);
	lv_slider_set_style(ui->progressbar, LV_SLIDER_STYLE_BG, &style0_progressbar);
	lv_slider_set_style(ui->progressbar, LV_SLIDER_STYLE_INDIC, &style1_progressbar);
	lv_slider_set_style(ui->progressbar, LV_SLIDER_STYLE_KNOB, &style2_progressbar);
#endif // LV_USE_SLIDER

#ifdef LV_USE_SLIDER
	lv_style_copy(&style0_volume_bar, &lv_style_pretty);

	lv_style_copy(&style1_volume_bar, &lv_style_pretty_color);
	style1_volume_bar.body.padding.top = 0;
	style1_volume_bar.body.padding.bottom = 0;
	style1_volume_bar.body.padding.left = 0;
	style1_volume_bar.body.padding.right = 0;
	style1_volume_bar.body.padding.inner = 0;

	lv_style_copy(&style2_volume_bar, &lv_style_pretty);
	style2_volume_bar.body.radius = 15;

	ui->volume_bar = lv_slider_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->volume_bar, 652, 16);
	lv_obj_set_size(ui->volume_bar, 136, 20);
	lv_slider_set_range(ui->volume_bar, 0, 40);
	lv_slider_set_knob_in(ui->volume_bar, false);
	lv_slider_set_value(ui->volume_bar, 30, LV_ANIM_OFF);
	lv_slider_set_style(ui->volume_bar, LV_SLIDER_STYLE_BG, &style0_volume_bar);
	lv_slider_set_style(ui->volume_bar, LV_SLIDER_STYLE_INDIC, &style1_volume_bar);
	lv_slider_set_style(ui->volume_bar, LV_SLIDER_STYLE_KNOB, &style2_volume_bar);
#endif // LV_USE_SLIDER

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_curr_time, &lv_style_transp);
	style0_curr_time.text.color = lv_color_hex(0x000000);
	style0_curr_time.text.line_space = 2;

	ui->curr_time = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->curr_time, "00:00:00");
	lv_label_set_long_mode(ui->curr_time, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->curr_time, 177, 35);
	lv_obj_set_size(ui->curr_time, 62, 19);
	lv_label_set_style(ui->curr_time, LV_LABEL_STYLE_MAIN, &style0_curr_time);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_total_time, &lv_style_transp);
	style0_total_time.text.color = lv_color_hex(0x000000);
	style0_total_time.text.line_space = 2;

	ui->total_time = lv_label_create(ui->container_1, NULL);
	lv_label_set_text(ui->total_time, "00:00:00");
	lv_label_set_long_mode(ui->total_time, LV_LABEL_LONG_EXPAND);
	lv_obj_set_pos(ui->total_time, 499, 32);
	lv_obj_set_size(ui->total_time, 62, 19);
	lv_label_set_style(ui->total_time, LV_LABEL_STYLE_MAIN, &style0_total_time);
#endif // LV_USE_LABEL

#ifdef LV_USE_IMGBTN
	ui->next = lv_imgbtn_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->next, 128, 3);
	lv_obj_set_size(ui->next, 48, 48);
	lv_imgbtn_set_state(ui->next, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->next, false);
	next_music_next_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_next.png");
	lv_imgbtn_set_src(ui->next, LV_BTN_STATE_REL, next_music_next_png_state0);
	next_music_next_png_state1 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_next.png");
	lv_imgbtn_set_src(ui->next, LV_BTN_STATE_PR, next_music_next_png_state1);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_IMGBTN
	ui->last = lv_imgbtn_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->last, 14, 4);
	lv_obj_set_size(ui->last, 48, 48);
	lv_imgbtn_set_state(ui->last, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->last, false);
	last_music_prev_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_prev.png");
	lv_imgbtn_set_src(ui->last, LV_BTN_STATE_REL, last_music_prev_png_state0);
	last_music_prev_png_state1 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_prev.png");
	lv_imgbtn_set_src(ui->last, LV_BTN_STATE_PR, last_music_prev_png_state1);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_IMGBTN
	ui->loop = lv_imgbtn_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->loop, 567, 11);
	lv_obj_set_size(ui->loop, 32, 32);
	lv_imgbtn_set_state(ui->loop, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->loop, true);
	loop_music_cycle_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_cycle.png");
	lv_imgbtn_set_src(ui->loop, LV_BTN_STATE_REL, loop_music_cycle_png_state0);
	loop_music_cycle_png_state1 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_cycle.png");
	lv_imgbtn_set_src(ui->loop, LV_BTN_STATE_PR, loop_music_cycle_png_state1);
	loop_music_loop_png_state2 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_loop.png");
	lv_imgbtn_set_src(ui->loop, LV_BTN_STATE_TGL_REL, loop_music_loop_png_state2);
	loop_music_loop_png_state3 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_loop.png");
	lv_imgbtn_set_src(ui->loop, LV_BTN_STATE_TGL_PR, loop_music_loop_png_state3);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_IMGBTN
	ui->volume = lv_imgbtn_create(ui->container_1, NULL);
	lv_obj_set_pos(ui->volume, 610, 11);
	lv_obj_set_size(ui->volume, 32, 32);
	lv_imgbtn_set_state(ui->volume, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->volume, true);
	volume_music_vol_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_vol.png");
	lv_imgbtn_set_src(ui->volume, LV_BTN_STATE_REL, volume_music_vol_png_state0);
	volume_music_vol_png_state2 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_vol.png");
	lv_imgbtn_set_src(ui->volume, LV_BTN_STATE_TGL_REL, volume_music_vol_png_state2);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_file_name, &lv_style_transp);

	ui->file_name = lv_label_create(ui->cont, NULL);
	lv_label_set_text(ui->file_name, "");
	lv_label_set_long_mode(ui->file_name, LV_LABEL_LONG_CROP);
	lv_obj_set_pos(ui->file_name, 275, 9);
	lv_obj_set_size(ui->file_name, 161, 43);
	lv_label_set_style(ui->file_name, LV_LABEL_STYLE_MAIN, &style0_file_name);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_download, &lv_style_transp);
	style0_download.text.color = lv_color_hex(0xffff7f);
	style0_download.text.font = &lv_font_roboto_28;
	style0_download.text.line_space = 2;

	ui->download = lv_label_create(ui->cont, NULL);
	lv_label_set_text(ui->download, "downloading...");
	lv_label_set_long_mode(ui->download, LV_LABEL_LONG_CROP);
	lv_obj_set_pos(ui->download, 127, 211);
	lv_obj_set_size(ui->download, 223, 38);
	lv_obj_set_hidden(ui->download,true);
	lv_label_set_style(ui->download, LV_LABEL_STYLE_MAIN, &style0_download);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_1, &lv_style_transp);
	style0_label_1.text.color = lv_color_hex(0xffff7f);
	style0_label_1.text.font = &lv_font_roboto_28;
	style0_label_1.text.line_space = 2;

	ui->label_1 = lv_label_create(ui->download, NULL);
	lv_label_set_text(ui->label_1, "download...");
	lv_label_set_long_mode(ui->label_1, LV_LABEL_LONG_CROP);
	lv_obj_set_pos(ui->label_1, 287, 209);
	lv_obj_set_size(ui->label_1, 153, 38);
	lv_label_set_style(ui->label_1, LV_LABEL_STYLE_MAIN, &style0_label_1);
#endif // LV_USE_LABEL

#ifdef LV_USE_LABEL
	lv_style_copy(&style0_label_2, &lv_style_transp);
	style0_label_2.text.color = lv_color_hex(0xffff7f);
	style0_label_2.text.font = &lv_font_roboto_28;
	style0_label_2.text.line_space = 2;

	ui->label_2 = lv_label_create(ui->label_1, NULL);
	lv_label_set_text(ui->label_2, "download...");
	lv_label_set_long_mode(ui->label_2, LV_LABEL_LONG_CROP);
	lv_obj_set_pos(ui->label_2, 287, 209);
	lv_obj_set_size(ui->label_2, 153, 38);
	lv_label_set_style(ui->label_2, LV_LABEL_STYLE_MAIN, &style0_label_2);
#endif // LV_USE_LABEL

#ifdef LV_USE_IMGBTN
	lv_style_copy(&style0_order, &lv_style_btn_rel);

	ui->order = lv_imgbtn_create(ui->cont, NULL);
	lv_obj_set_pos(ui->order, 768, 206);
	lv_obj_set_size(ui->order, 32, 32);
	lv_imgbtn_set_state(ui->order, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->order, false);
	order_music_order_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_order.png");
	lv_imgbtn_set_src(ui->order, LV_BTN_STATE_REL, order_music_order_png_state0);
	order_music_order_png_state1 = (void*)parse_image_form_file(LV_IMAGE_PATH"music_order.png");
	lv_imgbtn_set_src(ui->order, LV_BTN_STATE_PR, order_music_order_png_state1);
	lv_obj_set_hidden(ui->order,true);
	lv_imgbtn_set_style(ui->order, LV_IMGBTN_STYLE_REL, &style0_order);
#endif // LV_USE_IMGBTN

#ifdef LV_USE_BTN
	lv_style_copy(&style0_full_button, &lv_style_btn_rel);
	style0_full_button.body.main_color = lv_color_hex(0x000000);
	style0_full_button.body.grad_color = lv_color_hex(0x000000);
	style0_full_button.body.opa = 0;
	style0_full_button.body.border.opa = 0;

	lv_style_copy(&style1_full_button, &lv_style_btn_pr);
	style1_full_button.body.opa = 0;
	style1_full_button.body.border.opa = 0;

	lv_style_copy(&style2_full_button, &lv_style_btn_tgl_rel);
	style2_full_button.body.opa = 0;
	style2_full_button.body.border.opa = 0;

	lv_style_copy(&style3_full_button, &lv_style_btn_tgl_pr);
	style3_full_button.body.opa = 0;
	style3_full_button.body.border.opa = 0;

	ui->full_button = lv_btn_create(ui->cont, NULL);
	lv_obj_set_pos(ui->full_button, 1, 73);
	lv_obj_set_size(ui->full_button, 387, 346);
	lv_btn_set_style(ui->full_button, LV_BTN_STYLE_REL, &style0_full_button);
	lv_btn_set_style(ui->full_button, LV_BTN_STYLE_PR, &style1_full_button);
	lv_btn_set_style(ui->full_button, LV_BTN_STYLE_TGL_REL, &style2_full_button);
	lv_btn_set_style(ui->full_button, LV_BTN_STYLE_TGL_PR, &style3_full_button);
#endif // LV_USE_BTN

#ifdef LV_USE_IMGBTN
	ui->back = lv_imgbtn_create(ui->cont, NULL);
	lv_obj_set_pos(ui->back, 9, 7);
	lv_obj_set_size(ui->back, 32, 32);
	lv_imgbtn_set_state(ui->back, LV_BTN_STATE_REL);
	lv_imgbtn_set_toggle(ui->back, false);
	back_home_return_png_state0 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_return.png");
	lv_imgbtn_set_src(ui->back, LV_BTN_STATE_REL, back_home_return_png_state0);
	back_home_return_png_state3 = (void*)parse_image_form_file(LV_IMAGE_PATH"home_return.png");
	lv_imgbtn_set_src(ui->back, LV_BTN_STATE_TGL_PR, back_home_return_png_state3);
#endif // LV_USE_IMGBTN

}

void movie_auto_ui_destory(movie_ui_t *ui)
{
	lv_obj_clean(ui->cont);
	free_image(image_4_movie_big_png);
	free_image(play_music_play_png_state0);
	free_image(play_music_play_png_state1);
	free_image(play_music_pause_png_state2);
	free_image(play_music_pause_png_state3);
	free_image(next_music_next_png_state0);
	free_image(next_music_next_png_state1);
	free_image(last_music_prev_png_state0);
	free_image(last_music_prev_png_state1);
	free_image(loop_music_cycle_png_state0);
	free_image(loop_music_cycle_png_state1);
	free_image(loop_music_loop_png_state2);
	free_image(loop_music_loop_png_state3);
	free_image(volume_music_vol_png_state0);
	free_image(volume_music_vol_png_state2);
	free_image(order_music_order_png_state0);
	free_image(order_music_order_png_state1);
	free_image(back_home_return_png_state0);
	free_image(back_home_return_png_state3);
	free_image(media_list_1_movie_item_png);
	free_image(media_list_2_movie_item_png);
	free_image(media_list_3_movie_item_png);
	free_image(media_list_4_movie_item_png);
	free_image(media_list_5_movie_item_png);
	free_image(media_list_6_movie_item_png);
}

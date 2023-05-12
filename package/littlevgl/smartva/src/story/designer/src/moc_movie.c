/**********************
 *      includes
 **********************/
#include "moc_movie.h"
#include "ui_movie.h"
#include "lvgl.h"
#include "page.h"
#include "ui_resource.h"
#include "media_load_file.h"
#include "player_int.h"
#include "media_ui.h"
/**********************
 *       variables
 **********************/
typedef struct
{
	movie_ui_t ui;
} movie_para_t;
static movie_para_t para;

static player_ui_t * player_ui;

/**********************
 *  functions
 **********************/
typedef enum{
	NONE_STATUS,
	FULL_STATUS,
	BAR_STATUS,
	LIST_STATUS,
}ui_status_t;

ui_status_t ui_status = NONE_STATUS;

static void back_btn_event(lv_obj_t * btn, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED){
		destory_page(PAGE_MOVIE);
		create_page(PAGE_HOME);
	}
}

static void clean_screen(movie_ui_t *ui)
{
	// open tplayerdemo || willow					// 打开视频或则图片图层，pipe要是0

	lv_obj_t *scn = lv_scr_act();					// 清屏幕
	static lv_style_t scn_style;
	lv_style_copy(&scn_style, &lv_style_plain);
	scn_style.body.main_color.full = 0x00000000;
	scn_style.body.grad_color.full = 0x00000000;
	lv_obj_set_style(scn, &scn_style);
	
	static lv_style_t cont_style;
	lv_style_copy(&cont_style, &lv_style_plain);
	cont_style.body.main_color.full = 0x00000000;		// 清cont
	cont_style.body.grad_color.full = 0x00000000;
	lv_cont_set_style(ui->cont, LV_CONT_STYLE_MAIN, &cont_style);	
}

static void btn_player_event(lv_obj_t * btn, lv_event_t event)
{
	int index;
	int list_size;
	
	index = lv_list_get_btn_index(para.ui.media_list, btn);

	if (event == LV_EVENT_CLICKED){
		//printf("%s %d index = %d\n",__func__, __LINE__, index);
		media_play_file(index);
		lv_label_set_text(para.ui.file_name, player_ui->play_name);
		lv_btn_set_state(btn, LV_BTN_STATE_REL);
		lv_list_set_btn_selected(para.ui.media_list, btn);
		lv_btn_set_state(para.ui.play, LV_BTN_STYLE_TGL_REL);
		clean_screen(&para.ui);
		lv_obj_set_hidden(para.ui.media_list, true);
		lv_obj_set_hidden(para.ui.image_4, true);
		lv_obj_set_hidden(para.ui.order, false);
		ui_status = BAR_STATUS;
	}
}

static void btn_list_order_event(lv_obj_t * btn, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED){
		lv_obj_set_hidden(para.ui.media_list, false);
		lv_obj_set_hidden(para.ui.order, true);
		lv_obj_set_hidden(para.ui.file_name, true);
		tplayer_pause();
		lv_btn_set_state(para.ui.play, LV_BTN_STYLE_PR);
		ui_status = LIST_STATUS;
	}
}

static void btn_full_button_event(lv_obj_t * btn, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED){

		switch(ui_status){
			case FULL_STATUS:
				lv_obj_set_hidden(para.ui.media_list, true);
				lv_obj_set_hidden(para.ui.order, false);
				lv_obj_set_hidden(para.ui.file_name, false);
				lv_obj_set_hidden(para.ui.container_1, false);
				ui_status = BAR_STATUS;
				break;
			case LIST_STATUS:
				lv_obj_set_hidden(para.ui.media_list, true);
				lv_obj_set_hidden(para.ui.order, false);
				lv_obj_set_hidden(para.ui.file_name, false);
				lv_obj_set_hidden(para.ui.container_1, false);
				tplayer_play();
				lv_btn_set_state(para.ui.play, LV_BTN_STYLE_TGL_REL);
				ui_status = BAR_STATUS;
				break;
			case BAR_STATUS:
				lv_obj_set_hidden(para.ui.media_list, true);
				lv_obj_set_hidden(para.ui.order, true);
				lv_obj_set_hidden(para.ui.container_1, true);
				lv_obj_set_hidden(para.ui.file_name, true);
				ui_status = FULL_STATUS;
				break;
			default:
				break;
		}
	}
}

static void media_next_last_event(lv_obj_t * btn, lv_event_t event)
{
	int next_last = 0;
	lv_obj_t *  focus_btn;
	
	if(event == LV_EVENT_CLICKED){
		next_last = (btn == para.ui.next) ? 1 : -1;
		
		media_play_next_or_last(para.ui.media_list, next_last);
		//lv_list_focus(next_btn, LV_ANIM_OFF);
		lv_label_set_text(para.ui.file_name, player_ui->play_name);
		debug_info("play state %d", lv_btn_get_state(para.ui.play));
		if(lv_btn_get_state(para.ui.play) == LV_BTN_STYLE_TGL_REL){
			tplayer_play();
		}
	}
}

static void media_volume_event(lv_obj_t * btn, lv_event_t event)
{
	if(event != LV_EVENT_PRESSED){
		return;
	}
	
	if(lv_btn_get_state(btn) == LV_BTN_STYLE_PR){
		tplayer_volume(0);
	}
	
	if(lv_btn_get_state(btn) == LV_BTN_STYLE_TGL_PR){
		tplayer_volume(lv_bar_get_value(para.ui.volume_bar));
	}
}

static void *media_player_ui_update(void *param)
{
	lv_obj_t *  focus_btn;

	player_ui = (player_ui_t *)param;

	if(player_ui == NULL){
		return;
	}

	if(media_insert_play_enable()){
		media_play_file(media_get_play_file_index());
		lv_label_set_text(para.ui.file_name, player_ui->play_name);
		tplayer_seekto(player_ui->break_tag.offset);
		
		focus_btn = lv_list_get_next_btn(para.ui.media_list, NULL);
		for(int i=0; i < player_ui->play_index; i++){
			focus_btn = lv_list_get_next_btn(para.ui.media_list, focus_btn);
		}
		clean_screen(&para.ui);
		lv_btn_set_state(focus_btn, LV_BTN_STATE_REL);
		lv_list_set_btn_selected(para.ui.media_list, focus_btn);
		lv_btn_set_state(para.ui.play, LV_BTN_STYLE_TGL_REL);
		lv_obj_set_hidden(para.ui.media_list, true);
		lv_obj_set_hidden(para.ui.image_4, true);
		lv_obj_set_hidden(para.ui.order, false);
	}
	
	media_downloading(para.ui.download);

	if(media_list_loop(para.ui.media_list)){
		lv_label_set_text(para.ui.file_name, player_ui->play_name);
	}
	media_update_progressbar(para.ui.progressbar, para.ui.total_time, para.ui.curr_time);
}

static int create_movie(void)
{
	para.ui.cont = lv_cont_create(lv_scr_act(), NULL);
	lv_obj_set_size(para.ui.cont, LV_HOR_RES_MAX, LV_VER_RES_MAX);
	static lv_style_t cont_style;
	lv_style_copy(&cont_style, &lv_style_plain);
	cont_style.body.main_color = LV_COLOR_BLUE;
	cont_style.body.grad_color = LV_COLOR_BLUE;
	lv_cont_set_style(para.ui.cont, LV_CONT_STYLE_MAIN, &cont_style);
	lv_cont_set_layout(para.ui.cont, LV_LAYOUT_OFF);
	lv_cont_set_fit(para.ui.cont, LV_FIT_NONE);

	static lv_style_t back_btn_style;
	lv_style_copy(&back_btn_style, &lv_style_pretty);
	back_btn_style.text.font = &lv_font_roboto_28;

#if 0
	lv_obj_t *back_btn = lv_btn_create(para.ui.cont, NULL);
	lv_obj_align(back_btn, para.ui.cont, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_obj_t *back_lable = lv_label_create(back_btn, NULL);
	lv_label_set_text(back_lable, LV_SYMBOL_LEFT);
	lv_obj_set_event_cb(back_btn, back_btn_event);
	lv_btn_set_style(back_btn, LV_BTN_STYLE_REL, &back_btn_style);
	lv_btn_set_style(back_btn, LV_BTN_STYLE_PR, &back_btn_style);
#endif
	movie_auto_ui_create(&para.ui);
	
	lv_obj_set_event_cb(para.ui.back, back_btn_event);
	lv_obj_set_event_cb(para.ui.full_button, btn_full_button_event);
	lv_obj_set_event_cb(para.ui.order, btn_list_order_event);
	lv_obj_set_event_cb(para.ui.play, media_play_event);
	lv_obj_set_event_cb(para.ui.next, media_next_last_event);
	lv_obj_set_event_cb(para.ui.last, media_next_last_event);
	lv_obj_set_event_cb(para.ui.volume_bar, media_volume_bar_event);
	lv_obj_set_event_cb(para.ui.volume, media_volume_event);
	lv_obj_set_event_cb(para.ui.loop, media_loop_event);
	lv_obj_set_event_cb(para.ui.progressbar, media_progressbar_event);
	media_update_file_list(para.ui.media_list, RAT_MEDIA_TYPE_VIDEO, btn_player_event);	
	media_func_register(media_player_ui_update);
	
	return 0;
}


static int destory_movie(void)
{
	media_func_unregister();
	movie_auto_ui_destory(&para.ui);
	lv_obj_del(para.ui.cont);
	return 0;
}

static int show_movie(void)
{
	lv_obj_set_hidden(para.ui.cont, 0);

	return 0;
}

static int hide_movie(void)
{
	lv_obj_set_hidden(para.ui.cont, 1);

	return 0;
}

static page_interface_t page_movie = 
{
	.ops =
	{
		create_movie,
		destory_movie,
		show_movie,
		hide_movie,
	}, 
	.info =
	{
		.id         = PAGE_MOVIE, 
		.user_data  = NULL
	}
};

void REGISTER_PAGE_MOVIE(void)
{
	reg_page(&page_movie);
}

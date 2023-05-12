/**********************
 *      includes
 **********************/
#include "moc_music.h"
#include "ui_music.h"
#include "lvgl.h"
#include "page.h"
#include "ui_resource.h"
#include "media_load_file.h"
#include "player_int.h"
#include "media_ui.h"
#include "music_lrc.h"

/**********************
 *       variables
 **********************/
typedef struct
{
	music_ui_t ui;
} music_para_t;
static music_para_t para;

/**********************
 *  functions
 **********************/

static music_lrc_info lrc_info;
static player_ui_t * player_ui;

static int media_load_lrc(void)
{
	FILE *lrc = NULL;
	char lrc_name[FILE_NAME_LEN];
	char lrc_path[FILE_PATH_LEN];
	unsigned int curr_line = 0;
	char name[256];	
	char suffix[] = "lrc";
	char *file_name;
	char *path;
	int i;	
	int ret;

	memset(&lrc_info, 0, sizeof(lrc_info));
//match lrc name	
	file_name = player_ui->play_name;
	memset(lrc_name, 0, sizeof(lrc_name));
	memset(name, 0, sizeof(name));	
	for(i=0; i<sizeof(name); i++ ){
		if(file_name[i] == '.'){
			break;
		}
		name[i] = file_name[i];
	}
	if(i == FILE_NAME_LEN-1){
		goto END;
	}
	sprintf(lrc_name, "%s.%s", name, suffix);
	debug_info("match lrc name is %s", lrc_name);
	
//find lrc
	path =  media_get_disk_mount_point();

	memset(lrc_path, 0, sizeof(lrc_path));
	ret = find_music_lrc(path, lrc_name, lrc_path);

	if(ret < 0){
		debug_warn("don't find lrc:%s,lrc path =%s",lrc_name, lrc_path);
		goto END;
	}
//load lrc
	debug_info("open %s", lrc_path);
	lrc = fopen(lrc_path, "rb");
	if(!lrc){
		goto END;
	}
	fseek(lrc, 0, SEEK_END);
	lrc_info.lrc_size = ftell(lrc);
	fseek(lrc, 0, SEEK_SET);

	if(lrc_info.lrc_size <=0 ){
		debug_err("lrc size = %d", lrc_info.lrc_size);
		goto END;
	}
	
	lrc_info.lrc_cache = malloc(lrc_info.lrc_size);
	if(!lrc_info.lrc_cache){
		debug_err("lrc malloc failed!!!");
		goto END;
	}
	
	ret = fread(lrc_info.lrc_cache, 1, lrc_info.lrc_size, lrc);
	if(ret != lrc_info.lrc_size){
		debug_err("lrc read failed!!!");
		free(lrc_info.lrc_cache);
		goto END;
	}
	
	//debug_info("size = %d lrc_cache = %s", lrc_info.lrc_size, lrc_info.lrc_cache);

	lrc_info.start = parse_lrc_get_tag(lrc_info.lrc_cache, &lrc_info);
	if(!lrc_info.start){
		goto END;
	}
	lrc_info.end = lrc_info.lrc_cache + lrc_info.lrc_size;
	lrc_info.list_len = parse_lrc_get_time_list(&lrc_info);
	if(lrc_info.lrc_cache){
		free(lrc_info.lrc_cache);
	}

#if 0
//show lrc
	curr_line = parse_lrc_get_current_line(0, &lrc_info);
	lv_ta_add_text(para.ui.lrc, lrc_info.text_list[0]);
	
	for(i = 0; i < lrc_info.list_len; i++){
		curr_line = parse_lrc_get_next_line(curr_line, &lrc_info);
		lv_ta_add_text(para.ui.lrc, lrc_info.text_list[i]);
	}
#endif

	lv_obj_set_hidden(para.ui.lrc_no, true);
	
//	for(i = 0; i < lrc_info.list_len; i++){
//		debug_info("%d %d %s ",i, lrc_info.time_list[i], lrc_info.text_list[i]);
//	}

END:
	if(lrc)
		fclose(lrc);
	return -1;
}

static int media_update_lrc(int time)
{
#if	1
	unsigned int curr_line = 0;
	static unsigned int last_line = 0;

	curr_line = parse_lrc_get_current_line(time, &lrc_info);
	
	if(curr_line != last_line){
		last_line = curr_line;
		debug_info("time=%d %d: %s ",time, lrc_info.time_list[curr_line], lrc_info.text_list[curr_line]);
		lv_ta_set_text(para.ui.lrc, lrc_info.text_list[last_line]);
		//lv_ta_set_text(para.ui.lrc, lrc_info.text_list[curr_line]);
		//lv_ta_add_text(para.ui.lrc, lrc_info.text_list[last_line]);
	}
#endif
}


static void back_btn_event(lv_obj_t * btn, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{
		destory_page(PAGE_MUSIC);
		create_page(PAGE_HOME);
	}
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
		lv_obj_set_hidden(para.ui.lrc_no, false);
		media_load_lrc();
	}
}

static void media_next_last_event(lv_obj_t * btn, lv_event_t event)
{
	int next_last = 0;
	lv_obj_t *  focus_btn;
	
	if(event == LV_EVENT_CLICKED){
		next_last = (btn == para.ui.next) ? 1 : -1;
		media_play_next_or_last(para.ui.media_list, next_last);
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

static void media_player_ui_update(void *param)
{
	lv_obj_t *  focus_btn;
	char text[256];
	MediaInfo *musicInfo;

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
		focus_btn = lv_list_get_next_btn(para.ui.media_list, NULL);
		lv_btn_set_state(focus_btn, LV_BTN_STATE_REL);
		lv_list_set_btn_selected(para.ui.media_list, focus_btn);
		media_load_lrc();
	}

	if(media_list_loop(para.ui.media_list)){
		lv_label_set_text(para.ui.file_name, player_ui->play_name);
	}

	media_downloading(para.ui.download);
	
	media_update_progressbar(para.ui.progressbar, para.ui.total_time, para.ui.curr_time);

	if(tplayer_IsPlaying() && lrc_info.list_len){
		media_update_lrc(player_ui->play_time);
	}
}

static int create_music(void)
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
	music_auto_ui_create(&para.ui);
	
	lv_obj_set_event_cb(para.ui.back, back_btn_event);
	lv_obj_set_event_cb(para.ui.play, media_play_event);
	lv_obj_set_event_cb(para.ui.next, media_next_last_event);
	lv_obj_set_event_cb(para.ui.last, media_next_last_event);
	lv_obj_set_event_cb(para.ui.volume_bar, media_volume_bar_event);
	lv_obj_set_event_cb(para.ui.volume, media_volume_event);
	lv_obj_set_event_cb(para.ui.loop, media_loop_event);
	lv_obj_set_event_cb(para.ui.progressbar, media_progressbar_event);
	
	media_update_file_list(para.ui.media_list, RAT_MEDIA_TYPE_AUDIO, btn_player_event);	
	media_func_register(media_player_ui_update);
	return 0;
}

static int destory_music(void)
{
	media_func_unregister();
	music_auto_ui_destory(&para.ui);
	lv_obj_del(para.ui.cont);
	return 0;
}

static int show_music(void)
{
	lv_obj_set_hidden(para.ui.cont, 0);

	return 0;
}

static int hide_music(void)
{
	lv_obj_set_hidden(para.ui.cont, 1);

	return 0;
}

static page_interface_t page_music = 
{
	.ops =
	{
		create_music,
		destory_music,
		show_music,
		hide_music,
	}, 
	.info =
	{
		.id         = PAGE_MUSIC, 
		.user_data  = NULL
	}
};

void REGISTER_PAGE_MUSIC(void)
{
	reg_page(&page_music);
}

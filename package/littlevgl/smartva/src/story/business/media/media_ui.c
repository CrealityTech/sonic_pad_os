#include <pthread.h>
#include "player_int.h"
#include "media_load_file.h"
#include "page.h"
#include "media_ui.h"
#include "debug.h"
#include "DiskManager.h"

#define		LOCK
#ifdef LOCK

#define	MUTEX_LOCK(mtx)			if(pthread_mutex_lock(&mtx)){\
									printf("%s:%d mutex lock fail\n",__func__,__LINE__);\
								}
#define	MUTEX_UNLOCK(mtx)		if(pthread_mutex_unlock(&mtx)){\
									printf("%s:%d mutex unlock fail\n",__func__,__LINE__);\
								}
#else
#define MUTEX_LOCK(mtx)
#define MUTEX_UNLOCK(mtx)
#endif

static player_ui_t *player_ui;
static RegisterInfo_t RegisterInfo;
static DiskInfo_t Disk_HotPlugMsg;
static DiskInfo_t Disk_HotPlugTf;

static int media_set_play_param(void)
{
	float scaleWidth;
	float scaleHeigh;
	MediaInfo *videoInfo = NULL;
	TplayerVideoScaleDownType ScaleDownTypeW,ScaleDownTypeH ;

	player_ui->play_index = media_get_play_file_index();
	player_ui->play_path = media_get_file_path(player_ui->play_index);
	player_ui->play_name = media_get_path_to_name(player_ui->play_path);

	if(player_ui->play_path == NULL){
		debug_err("playing file path error");
		return 0;
	}

	debug_info("start playing %s\n", player_ui->play_path);
	if(tplayer_play_url(player_ui->play_path) < 0){
		return 0;
	}
	tplayer_setdisplayrect(0, 0, 800, 480);


#if 0
	videoInfo = tplayer_getmediainfo();
	if(videoInfo == NULL){
		gui_error("videoInfo error\n");
		return -1;
	}
	if(!videoInfo->pVideoStreamInfo){
		return 0;
	}
	gui_debug("videoInfo:  width = %d,height = %d %u...............................", videoInfo->pVideoStreamInfo->nWidth,videoInfo->pVideoStreamInfo->nHeight,videoInfo->nDurationMs);

	scaleWidth = (float)((float)videoInfo->pVideoStreamInfo->nWidth / (float)MAXWIDTH);
	scaleHeigh = (float)((float)videoInfo->pVideoStreamInfo->nHeight / (float)MAXHEIGHT);
	gui_debug("videoInfo:  width = %f,height = %f", scaleWidth,scaleHeigh);
	if(scaleWidth <= 1){
		ScaleDownTypeW = TPLAYER_VIDEO_SCALE_DOWN_1;
	}else if(scaleWidth <= 2){
		ScaleDownTypeW = TPLAYER_VIDEO_SCALE_DOWN_2;
	}else if(scaleWidth <= 4){
		ScaleDownTypeW = TPLAYER_VIDEO_SCALE_DOWN_4;
	}else{
		ScaleDownTypeW = TPLAYER_VIDEO_SCALE_DOWN_8;
	}

	if(scaleHeigh <= 1){
		ScaleDownTypeH = TPLAYER_VIDEO_SCALE_DOWN_1;
	}else if(scaleHeigh <= 2){
		ScaleDownTypeH = TPLAYER_VIDEO_SCALE_DOWN_2;
	}else if(scaleHeigh <= 4){
		ScaleDownTypeH = TPLAYER_VIDEO_SCALE_DOWN_4;
	}else{
		ScaleDownTypeH = TPLAYER_VIDEO_SCALE_DOWN_8;
	}

	tplayer_setscaledown(ScaleDownTypeW, ScaleDownTypeH);
	gui_debug("videoInfo:  ScaleDownTypeW = %d,ScaleDownTypeH = %d", ScaleDownTypeW, ScaleDownTypeH);
#endif

	return 1;
}


int media_play_next_or_last(const lv_obj_t * list, int next_last)
{
	lv_obj_t *  focus_btn;

	media_set_play_file_index(media_get_play_file_index() + next_last);
	player_ui->play_index = media_get_play_file_index();

	media_set_play_param();

	focus_btn = lv_list_get_next_btn(list, NULL);
	for(int i=0; i < player_ui->play_index; i++){
		focus_btn = lv_list_get_next_btn(list, focus_btn);
	}

	lv_list_set_btn_selected(list, focus_btn);

	if(next_last == 1){
		lv_list_up(list);
	}else{
		lv_list_down(list);
	}

	return 0;
}

void media_play_event(lv_obj_t * btn, lv_event_t event)
{
	if(event != LV_EVENT_PRESSED){
		return;
	}
	debug_info("play state %d", lv_btn_get_state(btn));
	if(lv_btn_get_state(btn) == LV_BTN_STYLE_PR){
		tplayer_play();
	}

	if(lv_btn_get_state(btn) == LV_BTN_STYLE_TGL_PR){
		tplayer_pause();
	}
}

void media_loop_event(lv_obj_t * btn, lv_event_t event)
{
	if(event != LV_EVENT_PRESSED){
		return;
	}

	debug_info("loop state %d", lv_btn_get_state(btn));
	if(lv_btn_get_state(btn) == LV_BTN_STYLE_TGL_PR){
		debug_info("single loop");
		tplayer_setlooping(1);
		player_ui->list_loop = 0;
	}

	if(lv_btn_get_state(btn) == LV_BTN_STYLE_PR){
		debug_info("list loop");
		tplayer_setlooping(0);
		player_ui->list_loop = 1;
	}

}

void media_volume_bar_event(lv_obj_t * btn, lv_event_t event)
{

	if(event == LV_EVENT_CLICKED){
		int volume = lv_bar_get_value(btn);
		tplayer_volume(volume);
	}
}

void media_progressbar_event(lv_obj_t * btn, lv_event_t event)
{
	int duration;
	int value;
	int seekvalue;

	if(event == LV_EVENT_CLICKED){
		value = lv_bar_get_value(btn);
		tplayer_getduration(&duration);
		if(duration <= 0 || !tplayer_IsPlaying){
			return;
		}
		seekvalue = value * duration / lv_bar_get_max_value(btn);
		tplayer_seekto(seekvalue);
	}
}

static void media_breakpoint_record(int offset)
{
	player_ui->break_tag.fileType = player_ui->media_list->media_type;
	player_ui->break_tag.offset = offset;
	player_ui->break_tag.index = media_get_play_file_index();
	memset(player_ui->break_tag.filename, 0, sizeof(player_ui->break_tag.filename));
	strcpy(player_ui->break_tag.filename, player_ui->play_name);
}

static int media_breakpoint_play(void)
{
	int i;
	char *path;
	
	if(media_get_file_total_num() == 0){
		goto NOTFIND;
	}

	debug_info("break_filename = %s", player_ui->break_tag.filename);
	path = media_get_file_path(player_ui->break_tag.index);
	if(!strcmp(player_ui->break_tag.filename, media_get_path_to_name(path))){
		media_set_play_file_index(player_ui->break_tag.index);
		goto FOUND;
	}
//find play file;
	path = media_get_file_path(0);
	for(i = 0; i < media_get_file_total_num(); i++){
		if(!strcmp(player_ui->break_tag.filename, media_get_path_to_name(path))){
			player_ui->break_tag.index = i;
			media_set_play_file_index(player_ui->break_tag.index);
			goto FOUND;
		}
		path = media_get_next_file_path();
	}
NOTFIND:
//don't find play file
	player_ui->break_tag.offset = 0;
	media_set_play_file_index(0);
	return 0;
FOUND:
	return 1;
}

void media_update_progressbar(lv_obj_t * btn, lv_obj_t *t_lable, lv_obj_t *c_lable)
{
	int duration;
	int curr_time = 0;
	int barValue;
	int hours, minutes, second;
	char t_time[64] = {0};
	char c_time[64] = {0};

	if(!tplayer_prepared() || !tplayer_IsPlaying()){
		goto end;
	}

	if(player_ui->tplayer->mpstaus != DEFAULT_STATUS){
		goto end;
	}
	if(!player_ui->tplayer->mMediaInfo){
		goto end;
	}

	if(tplayer_getcompletestate()){
		if(lv_bar_get_value(btn) < lv_bar_get_max_value(btn)){
			lv_bar_set_value(btn, lv_bar_get_max_value(btn), LV_ANIM_OFF);
		}
		goto end;
	}

	duration = player_ui->tplayer->mMediaInfo->nDurationMs;
	if(duration > 0){

		tplayer_getcurrentpos(&curr_time);
		media_breakpoint_record(curr_time);
		player_ui->play_time = curr_time;
		barValue = lv_bar_get_max_value(btn) * curr_time / duration;
		lv_bar_set_value(btn, barValue, LV_ANIM_OFF);

		second = duration / 1000;
		minutes = second / 60;
		hours = minutes / 60;
		memset(t_time, 0, sizeof(t_time));
		sprintf(t_time, "%d:%d:%d", hours%24, minutes % 60, second % 60);
		lv_label_set_text(t_lable, t_time);
		second = curr_time / 1000;
		minutes = second / 60;
		hours = minutes / 60;
		memset(c_time, 0, sizeof(c_time));
		sprintf(c_time, "%d:%d:%d", hours%24, minutes % 60, second % 60);
		lv_label_set_text(c_lable, c_time);
	}
end:
	return;
}

void media_downloading(lv_obj_t * donwnload)
{
	static int count = 0;
	bool hidden_en = true;

	if((!tplayer_prepared() || !tplayer_IsPlaying()) && !tplayer_getcompletestate()){
		hidden_en = false;
		count++;
		if(count == 10){
			lv_label_set_text(donwnload, "downloading .");
		}else if(count == 20){
			lv_label_set_text(donwnload, "downloading . .");
		}else if(count == 30){
			lv_label_set_text(donwnload, "downloading . . .");
			count = 0;
		}
	}
//	if(lv_obj_get_hidden(donwnload) != hidden_en){
	//	lv_obj_set_hidden(donwnload, hidden_flag);
	//}
}

int media_list_loop(const lv_obj_t * list)
{
	static int flag = 0;
	if(!tplayer_getcompletestate()){
		flag = 0;
	}
	if(tplayer_getcompletestate() && player_ui->list_loop && !flag){
		media_play_next_or_last(list, 1);
		tplayer_play();
		flag = 1;
		return 0;
	}
	return -1;
}

void media_play_file(int index)
{
	char *path;
	
	media_set_play_file_index(index);
	if(tplayer_prepared() && player_ui->play_index == media_get_play_file_index()){
		tplayer_play();//continue play current file
	}else{
	//to change play file
		if(media_set_play_param()){
			tplayer_play();
		}
	}
}

static char defaultMountPoint[]="/tmp/";
	
char *media_get_disk_mount_point(void)
{
	if(player_ui->insert_vaild[0]){
		return Disk_HotPlugMsg.MountPoint;
	}
	if(player_ui->insert_vaild[1]){
		return Disk_HotPlugTf.MountPoint;
	}
	
	return defaultMountPoint;
}

int media_get_usb_status(void)
{
	return player_ui->insert_vaild[0];
}

int media_get_tf_status(void)
{
	return player_ui->insert_vaild[1];
}

int media_update_file_list(lv_obj_t * list, rat_media_type_t fileType, lv_event_cb_t event_cb)
{
	int file_num;
	char file_name[512];
	lv_obj_t *firstBtn;
	lv_obj_t *nextBtn;
	char *path;
	void *img_src;
	int list_size;
	int i;

	player_ui->media_list = media_load_file(fileType, media_get_disk_mount_point());
	
	if(!player_ui->media_list){
		goto end;
	}
	if(player_ui->media_list->total_num == 0){
		goto end;
	}

	file_num = player_ui->media_list->total_num;
	firstBtn = lv_list_get_next_btn(list, NULL);
	img_src = lv_img_get_src(lv_list_get_btn_img(firstBtn));
	nextBtn = firstBtn;
	list_size = lv_list_get_size(list);
	path = media_get_file_path(0);
	for(i = 0; i < file_num; i++){
		sprintf(file_name, "%d.%s",i+1, media_get_path_to_name(path));
		if(i < list_size){
			lv_label_set_text(lv_list_get_btn_label(nextBtn), file_name);
			nextBtn = lv_list_get_next_btn(list, nextBtn);
		}else{
			lv_list_add_btn(list, img_src, file_name);
		}
		path = media_get_next_file_path();
	}
	media_get_file_path(media_get_play_file_index());//iit will index of searching  set as current play index
	
	nextBtn = firstBtn;
	for(i = 0; i < file_num; i++){
		lv_obj_set_event_cb(nextBtn, event_cb);
		nextBtn = lv_list_get_next_btn(list, nextBtn);
	}
end:
	return file_num;
}


static void media_disk_insert(DiskInfo_t *DiskInfo)
{
	int DiskType = 0;

	if(DiskInfo->operate == MEDIA_PLUGOUT){
		goto end;
	}
	
	DiskType = DiskInfo->MediaType == MEDIA_SD_CARD ? 1 : 0;
	if(player_ui->disk_check[DiskType]){
		goto end;
	}
//find breakpoint
	player_ui->disk_check[DiskType] = 1;

	if(player_ui->break_tag.fileType == RAT_MEDIA_TYPE_VIDEO ||
		player_ui->break_tag.fileType == RAT_MEDIA_TYPE_AUDIO){
		player_ui->media_list = media_load_file(player_ui->break_tag.fileType, DiskInfo->MountPoint);
		if(media_breakpoint_play() && player_ui->media_list){
			debug_info("play breakpoint fileType=%d, pos=%d\n", player_ui->break_tag.fileType, player_ui->break_tag.offset);
			player_ui->disk_use = DiskInfo->MediaType;
			player_ui->insert_vaild[DiskType] = 1;
			player_ui->play_enable = 1;
			destory_page(current_page());
			if(player_ui->break_tag.fileType == RAT_MEDIA_TYPE_VIDEO)
				create_page(PAGE_MOVIE);
			else{
				create_page(PAGE_MUSIC);
			}
			goto end;
		}
	}

//play video when don't find breakpoint
	player_ui->media_list = media_load_file(RAT_MEDIA_TYPE_VIDEO, DiskInfo->MountPoint);
	if(player_ui->media_list->total_num > 0){
		player_ui->disk_use = DiskInfo->MediaType;
		player_ui->insert_vaild[DiskType] = 1;
		player_ui->play_enable = 1;
		media_set_play_file_index(0);
		destory_page(current_page());
		create_page(PAGE_MOVIE);
		goto end;
	}
	
//play music when don't find video file
	player_ui->media_list = media_load_file(RAT_MEDIA_TYPE_AUDIO, DiskInfo->MountPoint);
	if(player_ui->media_list->total_num > 0){
		player_ui->disk_use = DiskInfo->MediaType;
		player_ui->insert_vaild[DiskType] = 1;
		player_ui->play_enable = 1;
		media_set_play_file_index(0);
		destory_page(current_page());
		create_page(PAGE_MUSIC);
		goto end;
	}

end:
	return;
}
//disk is pulled out
static void media_disk_pullout(DiskInfo_t *DiskInfo)
{
	int DiskType = 0;

	DiskType = DiskInfo->MediaType == MEDIA_USB_MASSSTORAGE ? 0 : 1;
	
	if(DiskInfo->operate == MEDIA_PLUGOUT){
		player_ui->insert_vaild[DiskType] = 0;
		player_ui->disk_check[DiskType] = 0;
	}
	
	if(player_ui->insert_vaild[DiskType]){
		goto end;
	}
//it will use another disk
	if(player_ui->disk_use == DiskInfo->MediaType){
		if(player_ui->insert_vaild[!DiskType]){
			player_ui->disk_check[!DiskType] = 0;
			goto end;
		}
	}
//disk is pulled out, the page will go to home
	if(!player_ui->insert_vaild[0] && !player_ui->insert_vaild[1] && !player_ui->online_mode){
		if(current_page() == PAGE_MOVIE ||
			current_page() == PAGE_MUSIC ){
			destory_page(current_page());
			create_page(PAGE_HOME);
			goto end;
		}
	}
end:
	return;
}

int media_insert_play_enable(void)
{
	if(player_ui->play_enable){
		player_ui->play_enable = 0;
		return 1;
	}
	return 0;
}

static void media_ui_player_event(void* pUserData, int msg, int param0, void* param1)
{
	player_context_t* pPlayer = (player_context_t*)pUserData;
	switch(msg){
		case TPLAYER_NOTIFY_PLAYBACK_COMPLETE:
			break;
		default:
			break;
	}
}
static void *media_ui_task(struct _lv_task_t *param)
{
	MUTEX_LOCK(player_ui->mutex);
	media_disk_insert(&Disk_HotPlugMsg);
	media_disk_pullout(&Disk_HotPlugMsg);
	media_disk_insert(&Disk_HotPlugTf);
	media_disk_pullout(&Disk_HotPlugTf);
	MUTEX_UNLOCK(player_ui->mutex);
	if(player_ui->fun){
		player_ui->fun(param->user_data);
	}
}

void disk_manager_callback(DiskInfo_t *DiskInfo)
{
	MUTEX_LOCK(player_ui->mutex);
	if(DiskInfo->MediaType == MEDIA_USB_MASSSTORAGE){
		memset(&Disk_HotPlugMsg, 0x00, sizeof(DiskInfo_t));
		memcpy(&Disk_HotPlugMsg, DiskInfo, sizeof(DiskInfo_t));
	}
	
	if(DiskInfo->MediaType == MEDIA_SD_CARD){
		memset(&Disk_HotPlugTf, 0x00, sizeof(DiskInfo_t));
		memcpy(&Disk_HotPlugTf, DiskInfo, sizeof(DiskInfo_t));
	}
	if(DiskInfo->operate == MEDIA_PLUGOUT){
		media_unload_file();
	}
	MUTEX_UNLOCK(player_ui->mutex);
}

void media_ui_pthread_create(media_task_func fun)
{
	int result;

	player_ui = (player_ui_t *)malloc(sizeof(player_ui_t));
	if(player_ui == NULL){
		debug_err("player_ui malloc failed!\r\n");
		goto end;
	}

	memset(player_ui, 0, sizeof(player_ui_t));

	player_ui->tplayer = tplayer_pthread_create();

	player_ui->online_mode = 0;

	if(!player_ui->tplayer){
		debug_err("tplayer create failed!\r\n");
		goto end;
	}
#ifdef LOCK
	result = pthread_mutex_init(&player_ui->mutex, NULL);
	if(result != 0){
		debug_err("pthread mutex init failed!\r\n");
		goto end;
	}
#endif
	Disk_HotPlugMsg.MediaType = MEDIA_USB_MASSSTORAGE;
	Disk_HotPlugMsg.operate = MEDIA_PLUGOUT;
	Disk_HotPlugTf.MediaType = MEDIA_SD_CARD;	
	Disk_HotPlugTf.operate = MEDIA_PLUGOUT;
	RegisterInfo.CallBackFunction = disk_manager_callback;
	strcpy(RegisterInfo.Cur_Win, "media_ui");
	DiskManager_Register(&RegisterInfo);
	player_ui->lv_task = lv_task_create(media_ui_task, LV_INDEV_DEF_READ_PERIOD, LV_TASK_PRIO_MID, player_ui);
	if(!player_ui->lv_task){
		debug_err("lv task create failed!\r\n");
		goto end;
	}

end:
	return;
}

void media_ui_pthread_destroy(void)
{
	lv_task_del(player_ui->lv_task);

#ifdef LOCK
	pthread_mutex_destroy(&player_ui->mutex);
#endif
	DiskManager_UnRegister(&RegisterInfo);

	tplayer_pthread_destory(player_ui->tplayer);
	free(player_ui);
	player_ui = NULL;
}

void media_func_register(media_task_func fun)
{
	tplayer_init();
	tplayer_set_callback(player_ui, media_ui_player_event);
	player_ui->fun = fun;
}

void media_func_unregister(void)
{
	tplayer_set_callback(player_ui, NULL);
	player_ui->fun = NULL;
	tplayer_exit();
	media_unload_file();
}

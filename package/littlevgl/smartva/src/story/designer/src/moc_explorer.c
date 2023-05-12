/**********************
 *      includes
 **********************/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "moc_explorer.h"
#include "ui_explorer.h"
#include "lvgl.h"
#include "page.h"
#include "ui_resource.h"
#include "rat_common.h"
#include "DiskManager.h"

/**********************
 *       variables
 **********************/ 
typedef struct
{
	rat_ctrl_t rat_ctrl;	
	explorer_list_t *explorer_list;
	lv_obj_t *backImageBtn;
	explorer_ui_t ui;
} explorer_para_t;
static explorer_para_t Explorer_Para;
static void *backImageBuff = NULL;
static void *explorer_src[EXPLORER_IMAGE_NUM_MAX] = {0};
static lv_obj_t *label_dir_base = NULL;
static lv_task_t *auto_load_task = NULL;
static lv_task_t *HotPlug_Process_task = NULL;
static RegisterInfo_t RegisterInfo;
static pthread_mutex_t Explorer_HotPlug_Mutex;

static void Explorer_FileList_Update(void);
static void Explorer_FileList_event(lv_obj_t * btn, lv_event_t event);
static void Explorer_AutoLoad_Task(lv_task_cb_t *task);
static int Explorer_TypeAll_Update_Filelist(file_list_t* cur_list, int start, int end);

/**********************
 *  functions
 **********************/ 
static bool __is_in_obj(lv_obj_t * obj, __u32 x, __u32 y)
{
	lv_area_t  obj_area;
	lv_obj_get_coords(obj, &obj_area);
	
	if (x > obj_area.x2 || x < obj_area.x1 || y > obj_area.y2 || y < obj_area.y1)
	{
		return false;
	}
	else
	{
		return true;
	} 
}
static void back_btn_event(lv_obj_t * btn, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		destory_page(PAGE_EXPLORER);
		create_page(PAGE_HOME);
	}
}
static void Filesize2str(__u32 size, char *str) {
	// 小于1 k
	if (size < 1024) {
		sprintf(str, "%d B",size);
	}
	// 小于 1 M
	else if ( size < (1024*1024)) {
		sprintf(str, "%d K",size/1024);
	}
	// 小于 1 G
	else if (size < (1024*1024*1024)) {
		sprintf(str, "%d.%d%d M",size/(1024*1024), ((size%(1024*1024))/1024)*1000/1024/100,((size%(1024*1024))/1024)*1000/1024%100/10);	//保留两位小数
	}
	else {
		sprintf(str, "%d.%d%d G",size/(1024*1024*1024),(size%(1024*1024*1024))/(1024*1024)*1000/1024/100,(size%(1024*1024*1024))/(1024*1024)*1000/1024%100/10);
	}
}
static __s32 mtime_to_time_string(time_t *mtime, char *string) {
	struct tm *p = NULL;
	__s32 Index = 0;
	
	p = gmtime(mtime);
	
	string[Index++] = (1900 + p->tm_year)/1000 + '0';
	string[Index++] = (1900 + p->tm_year)/100%10 + '0';
	string[Index++] = (1900 + p->tm_year)/10%10 + '0';
	string[Index++] = (1900 + p->tm_year)%10 + '0';

	string[Index++] = '-';	
	string[Index++] = p->tm_mon/10 + '0';
	string[Index++] = p->tm_mon%10 + '0';

	string[Index++] = '-';
	string[Index++] = p->tm_mday/10 + '0';
	string[Index++] = p->tm_mday%10 + '0';

	string[Index++] = ' ';
	string[Index++] = p->tm_hour / 10 + '0';
	string[Index++] = p->tm_hour % 10 + '0';
	string[Index++] = ':';
	string[Index++] = p->tm_min / 10 + '0';
	string[Index++] = p->tm_min % 10 + '0';
	string[Index++] = ':';
	string[Index++] = p->tm_sec / 10 + '0';
	string[Index++] = p->tm_sec % 10 + '0';
	string[Index++] = '\0';
}
static void Explorer_Return_Home_Event(lv_obj_t * btn, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{
		create_page(PAGE_HOME);
		destory_page(PAGE_EXPLORER);
	}
}

static void Explorer_Update_Directory(const char *FilePath)
{
	lv_label_set_text(label_dir_base, FilePath);
}

static void Explorer_Release_CurFileList(void)
{
	file_list_t *Temp = NULL;
	file_list_t *Child = Explorer_Para.explorer_list->top_file_list->child;
	while (Child != NULL) {
		Temp = Child;
		Child = Child->child;
		delete_file_list_nod(Temp);
	}
	Explorer_Para.explorer_list->top_file_list->child = NULL;
}

static void Explorer_Update_FileInfo(const char *FilePath)
{
	int ret = 0;
	char str[50] = {0};
	struct stat file_stat;
	
	memset(&file_stat, 0x00, sizeof(struct stat));
	stat(FilePath, &file_stat);
	Filesize2str(file_stat.st_size, str);	
	lv_label_set_text(Explorer_Para.ui.label_6, str);

	memset(str, 0x00, sizeof(str));
	mtime_to_time_string(&file_stat.st_mtime, str);
	lv_label_set_text(Explorer_Para.ui.label_8, str);
}
static void Explorer_Back_Btn_Event(lv_obj_t * btn, lv_event_t event)
{
	if (event == LV_EVENT_RELEASED) {
		unsigned int Index = 0, list_btn_num = lv_list_get_size(Explorer_Para.ui.list_2), start = 0, end = 0;

//		printf("%s %d %s \n",__FILE__,__LINE__,__func__);
		if (Explorer_Para.explorer_list->cur_file_list->parent != Explorer_Para.explorer_list->top_file_list) {
			struct file_list_s  *parent = Explorer_Para.explorer_list->cur_file_list->parent;
//			delete_file_list_nod(Explorer_Para.explorer_list->cur_file_list);
			Explorer_Para.explorer_list->cur_file_list = parent;
			Explorer_Para.explorer_list->CurIsEmpty = 0;
		} else if (Explorer_Para.explorer_list->CurIsEmpty == 1){
			printf("do nothing\n");
		} else {
			printf("this top dir\n");
		}
		
		if (auto_load_task != NULL) {
			lv_task_del(auto_load_task);
			auto_load_task = NULL;
		}
		lv_list_clean(Explorer_Para.ui.list_2);
		start = lv_list_get_size(Explorer_Para.ui.list_2);
		end = ((start + EXPLORER_LIST_TASKLOAD_ITEM_NUM) > Explorer_Para.explorer_list->cur_file_list->total)?
				Explorer_Para.explorer_list->cur_file_list->total:(start + EXPLORER_LIST_TASKLOAD_ITEM_NUM);
		Explorer_TypeAll_Update_Filelist(Explorer_Para.explorer_list->cur_file_list, start, end);
		Explorer_Update_Directory(Explorer_Para.explorer_list->cur_file_list->file_path);
		if (end < Explorer_Para.explorer_list->cur_file_list->total) {
			auto_load_task = lv_task_create((void *)Explorer_AutoLoad_Task,100,LV_TASK_PRIO_MID,NULL);
		}
	}
}
static __s32 Explorer_Add_FileListItem(rat_media_type_t type, const char *FileName){
	lv_obj_t *img_type = NULL;
	lv_obj_t *btn_temp = NULL;
	lv_obj_t *label_name = NULL;
	lv_obj_t *img_more = NULL;

//	printf("%s %d %s type:%d\n", __FILE__, __LINE__, __func__, type);
	switch(type) {
		case RAT_MEDIA_TYPE_PIC: 
			if (explorer_src[EXPLORER_IMAGE_PICTURE] == NULL) {
				explorer_src[EXPLORER_IMAGE_PICTURE] = parse_image_form_file(EXPLORER_IMAGE_PICTURE_PATH);
			}
			btn_temp = lv_list_add_btn(Explorer_Para.ui.list_2,explorer_src[EXPLORER_IMAGE_PICTURE], FileName);
			break;
		case RAT_MEDIA_TYPE_AUDIO:
			if (explorer_src[EXPLORER_IMAGE_AUDIO] == NULL) {
				explorer_src[EXPLORER_IMAGE_AUDIO] = parse_image_form_file(EXPLORER_IMAGE_AUDIO_PATH);
			}
			btn_temp = lv_list_add_btn(Explorer_Para.ui.list_2,explorer_src[EXPLORER_IMAGE_AUDIO], FileName);
			break;
		case RAT_MEDIA_TYPE_VIDEO:
			if (explorer_src[EXPLORER_IMAGE_VIDEO] == NULL) {
				explorer_src[EXPLORER_IMAGE_VIDEO] = parse_image_form_file(EXPLORER_IMAGE_VIDEO_PATH);
			}
			btn_temp = lv_list_add_btn(Explorer_Para.ui.list_2,explorer_src[EXPLORER_IMAGE_VIDEO], FileName);
			break;
		case RAT_MEDIA_TYPE_FIRMWARE:
			if (explorer_src[EXPLORER_IMAGE_FIRMWARE] == NULL) {
				explorer_src[EXPLORER_IMAGE_FIRMWARE] = parse_image_form_file(EXPLORER_IMAGE_FIRMWARE_PATH);
			}
			btn_temp = lv_list_add_btn(Explorer_Para.ui.list_2,explorer_src[EXPLORER_IMAGE_FIRMWARE], FileName);
			
			break;
		case RAT_MEDIA_TYPE_EBOOK:
			if (explorer_src[EXPLORER_IMAGE_TEXT] == NULL) {
				explorer_src[EXPLORER_IMAGE_TEXT] = parse_image_form_file(EXPLORER_IMAGE_TEXT_PATH);
			}
			btn_temp = lv_list_add_btn(Explorer_Para.ui.list_2,explorer_src[EXPLORER_IMAGE_TEXT], FileName);							
			break;
		case RAT_MEDIA_TYPE_UNKNOWN:
			if (explorer_src[EXPLORER_IMAGE_UNKOWN] == NULL) {
				explorer_src[EXPLORER_IMAGE_UNKOWN] = parse_image_form_file(EXPLORER_IMAGE_UNKOWN_PATH);
			}
			btn_temp = lv_list_add_btn(Explorer_Para.ui.list_2,explorer_src[EXPLORER_IMAGE_UNKOWN], FileName);
			break;
		case RAT_MEDIA_TYPE_FOLDER:
			btn_temp = lv_list_add_btn(Explorer_Para.ui.list_2,NULL,NULL);
			/*关闭自动布局，改为手动布局*/
			lv_btn_set_layout(btn_temp, LV_LAYOUT_OFF);
			img_type = lv_img_create(btn_temp,NULL);
			if (explorer_src[EXPLORER_IMAGE_FOLDER] == NULL) {
				explorer_src[EXPLORER_IMAGE_FOLDER] = parse_image_form_file(EXPLORER_IMAGE_FOLDER_PATH);
			}
			lv_img_set_src(img_type,explorer_src[EXPLORER_IMAGE_FOLDER]);
			lv_obj_align(img_type,NULL,LV_ALIGN_IN_LEFT_MID,24,0);

			img_more = lv_img_create(btn_temp,NULL);
			if (explorer_src[EXPLORER_IMAGE_MORE] == NULL) {
				explorer_src[EXPLORER_IMAGE_MORE] = parse_image_form_file(EXPLORER_IMAGE_MORE_PATH);
			}
			lv_img_set_src(img_more,explorer_src[EXPLORER_IMAGE_MORE]);
			lv_obj_align(img_more,img_type,LV_ALIGN_OUT_RIGHT_MID,510,0);//以类型图标作为对齐目标

			label_name = lv_label_create(btn_temp,NULL);
			lv_label_set_text(label_name, FileName);
			lv_obj_align(label_name,img_type,LV_ALIGN_OUT_RIGHT_MID,12,0);								
			break;
		default:
		break;
	}
//	printf("%s %d %s\n", __FILE__, __LINE__, __func__);
	if (btn_temp) {
		lv_obj_set_event_cb(btn_temp,Explorer_FileList_event);
	} else {
		printf("%s %d %s btn_temp null\n", __FILE__, __LINE__, __func__);
	}
}
static int Explorer_TypeAll_Update_Filelist(file_list_t* cur_list, int start, int end)
{
	unsigned int i = 0;
	file_item_t *file_item = NULL;	
	
	end = (end > cur_list->total)?cur_list->total:end;
	/*此处只加载2个页面的元素，后续用线程加载剩余元素或检测滑动动态加载*/
	for (i = start; i < end; i++) {
		file_item = get_file_list_item(cur_list, i);
		if (file_item != NULL) {
			rat_media_type_t type;
			
			type = get_file_list_item_file_type(file_item); 
			Explorer_Add_FileListItem(type, file_item->name);
		} else {
			printf("%s %d file_item == NULL\n", __FILE__, __LINE__);
		}
		
	}
	lv_label_set_text(Explorer_Para.ui.label_6,"");
	lv_label_set_text(Explorer_Para.ui.label_8,"");	
}

static void Explorer_FileList_event(lv_obj_t * btn, lv_event_t event)
{
	if (event == LV_EVENT_SHORT_CLICKED) {
		lv_indev_t * indev = lv_indev_get_act();
		/*过滤list在顶端不能触发滑动事件的情况，增加判断点击位置是否在元素上*/
		if (__is_in_obj(btn,indev->proc.types.pointer.act_point.x,indev->proc.types.pointer.act_point.y)) {
			unsigned int Index = lv_list_get_btn_index(Explorer_Para.ui.list_2, btn);

			if (Explorer_Para.rat_ctrl.media_type == RAT_MEDIA_TYPE_ALL) {
				file_item_t *sel_item = get_file_list_item(Explorer_Para.explorer_list->cur_file_list, Index);
				if (sel_item->fatdirattr == FSYS_ATTR_DIRECTORY) {					
					Explorer_Para.explorer_list->file_item = sel_item;
					Explorer_FileList_Update();
				}
				else//点击的是文件，就更新文件信息
				{				
					char FilePath[RAT_MAX_FULL_PATH_LEN] = {0};
					sprintf(FilePath, "%s/%s", Explorer_Para.explorer_list->cur_file_list->file_path, sel_item->name);
					Explorer_Update_FileInfo(FilePath);
					// do something,switch app
				}
			}else {		
				int ret = 0;
				rat_entry_t ItemInfo;
				
				memset(&ItemInfo,0,sizeof(rat_entry_t));
				ret = rat_get_item_info_by_index(Explorer_Para.rat_ctrl.handle, Index, &ItemInfo); 
				if (ret == EPDK_FAIL) {
					printf("get file information form rat fail!!");
					return ;
				}
				Explorer_Update_Directory(ItemInfo.Path);
				Explorer_Update_FileInfo(ItemInfo.Path);
				// do something,switch app
			}
			
		}
	}
	return ;
}
static void Explorer_AutoLoad_Task(lv_task_cb_t *task)
{
	if (Explorer_Para.rat_ctrl.media_type == RAT_MEDIA_TYPE_ALL) {
		file_item_t *file_item = NULL;
		file_list_t* cur_list = Explorer_Para.explorer_list->cur_file_list;
		rat_media_type_t file_type;
		unsigned int Index = 0, list_btn_num = lv_list_get_size(Explorer_Para.ui.list_2), start = 0, end = 0;

		start = list_btn_num;
		end = ((list_btn_num + EXPLORER_LIST_TASKLOAD_ITEM_NUM) > cur_list->total)?cur_list->total:(list_btn_num + EXPLORER_LIST_TASKLOAD_ITEM_NUM);
		Explorer_TypeAll_Update_Filelist(cur_list, start, end);
		if (end == cur_list->total) {
			lv_task_del(auto_load_task);
			auto_load_task = NULL;	
		}
	} else {
		if (!rat_is_cursor_end(Explorer_Para.rat_ctrl.handle)) {
			__u32 Index = lv_list_get_size(Explorer_Para.ui.list_2);
			rat_move_cursor_forward(Explorer_Para.rat_ctrl.handle, EXPLORER_LIST_TASKLOAD_ITEM_NUM);
			Explorer_Para.rat_ctrl.total = rat_get_cur_scan_cnt(Explorer_Para.rat_ctrl.handle);
//			printf("%s %d %s Explorer_Para.rat_ctrl.total:%d\n",__FILE__,__LINE__,__func__, Explorer_Para.rat_ctrl.total);
			for (; Index < Explorer_Para.rat_ctrl.total; Index++) {
				int ret = 0;
				rat_entry_t ItemInfo;
				extern char * SLIB_strchrlast(char * pstr, char srch_char);
				
				memset(&ItemInfo,0,sizeof(rat_entry_t));	
				ret = rat_get_item_info_by_index(Explorer_Para.rat_ctrl.handle, Index, &ItemInfo); 
				if (ret == EPDK_FAIL) {
					printf("get file information form rat fail!!");
					return ;
				}
				if (Index == 0) {
					Explorer_Update_Directory(ItemInfo.Path);
					Explorer_Update_FileInfo(ItemInfo.Path);
				}
				printf("%s %d %s SLIB_strchrlast(ItemInfo.Path, '/') + 1:%s\n",__FILE__,__LINE__,__func__, SLIB_strchrlast(ItemInfo.Path, '/') + 1);
				Explorer_Add_FileListItem(Explorer_Para.rat_ctrl.media_type, SLIB_strchrlast(ItemInfo.Path, '/') + 1);
			}
		}else {
			lv_task_del(auto_load_task);
			auto_load_task = NULL;	
		}		
	}
}

static void Explorer_FileList_Update(void) {
	if (Explorer_Para.rat_ctrl.media_type == RAT_MEDIA_TYPE_ALL) {
		file_list_t *FileList = NULL;
		char  file_path[RAT_MAX_FULL_PATH_LEN] = {0};

		/*打开第一个设备，进入第一级目录*/		
		if (strlen(Explorer_Para.explorer_list->cur_file_list->file_path) != 0) {
			sprintf(file_path, "%s/", Explorer_Para.explorer_list->cur_file_list->file_path);
		}
		strcat(file_path, Explorer_Para.explorer_list->file_item->name);
		FileList = new_file_list_nod(file_path, NULL);
		if (FileList != NULL) {	
			unsigned int start = 0, end = 0;
			
			FileList->parent = Explorer_Para.explorer_list->cur_file_list;
			Explorer_Para.explorer_list->cur_file_list->child = FileList;
			Explorer_Para.explorer_list->cur_file_list = FileList;

			if (auto_load_task != NULL) {
				lv_task_del(auto_load_task);
				auto_load_task = NULL;
			}
			lv_list_clean(Explorer_Para.ui.list_2);
			start = lv_list_get_size(Explorer_Para.ui.list_2);
			end = ((start + EXPLORER_LIST_TASKLOAD_ITEM_NUM) > Explorer_Para.explorer_list->cur_file_list->total) ?
						Explorer_Para.explorer_list->cur_file_list->total:(start + EXPLORER_LIST_TASKLOAD_ITEM_NUM);
			Explorer_TypeAll_Update_Filelist(Explorer_Para.explorer_list->cur_file_list, start, end);
			//开始预加载任务
			if (end < Explorer_Para.explorer_list->cur_file_list->total) {
				auto_load_task = lv_task_create((void *)Explorer_AutoLoad_Task,100,LV_TASK_PRIO_MID,NULL);
			}
		} else {
			lv_list_clean(Explorer_Para.ui.list_2);
			Explorer_Para.explorer_list->CurIsEmpty = 1;
		}		
		Explorer_Update_Directory(file_path);
	} else {
		__u32 Index = 0;
		
		strcpy(Explorer_Para.rat_ctrl.SearchPath, Explorer_Para.explorer_list->file_item->name);
		Explorer_Para.rat_ctrl.handle = rat_open(Explorer_Para.rat_ctrl.SearchPath, Explorer_Para.rat_ctrl.media_type, EXPLORER_LIST_PERLOAD_ITEM_NUM); //全部搜索	
		Explorer_Para.rat_ctrl.total = rat_get_cur_scan_cnt(Explorer_Para.rat_ctrl.handle);
		lv_list_clean(Explorer_Para.ui.list_2);				
		for (Index = 0; Index < Explorer_Para.rat_ctrl.total; Index++) {
			int ret = 0;
			rat_entry_t ItemInfo;
			extern char * SLIB_strchrlast(char * pstr, char srch_char);
			
			memset(&ItemInfo,0,sizeof(rat_entry_t));	
			ret = rat_get_item_info_by_index(Explorer_Para.rat_ctrl.handle, Index, &ItemInfo); 
			if (ret == EPDK_FAIL) {
				printf("get file information form rat fail!!");
				return ;
			}
			if (Index == 0) {
				Explorer_Update_Directory(ItemInfo.Path);
				Explorer_Update_FileInfo(ItemInfo.Path);
			}
			Explorer_Add_FileListItem(Explorer_Para.rat_ctrl.media_type, SLIB_strchrlast(ItemInfo.Path, '/') + 1);
		}	
		if (!rat_is_cursor_end(Explorer_Para.rat_ctrl.handle)) {
			auto_load_task = lv_task_create((void *)Explorer_AutoLoad_Task,100,LV_TASK_PRIO_MID,NULL);
		}
	}
}
static void Explorer_MediumList_Update(void) {
	if (Explorer_Para.rat_ctrl.media_type == RAT_MEDIA_TYPE_ALL) {
		Explorer_Release_CurFileList();
	}else {
		if (Explorer_Para.rat_ctrl.handle) {
			rat_close(Explorer_Para.rat_ctrl.handle);
			Explorer_Para.rat_ctrl.handle = 0;
		}		
	}	
	Explorer_FileList_Update();
}

static void Explorer_MediumList_Event(lv_obj_t * btn, lv_event_t event)
{
	if (event == LV_EVENT_SHORT_CLICKED) {
		lv_indev_t * indev = lv_indev_get_act();
		/*过滤list在顶端不能触发滑动事件的情况，增加判断点击位置是否在元素上*/
		if(__is_in_obj(btn, indev->proc.types.pointer.act_point.x, indev->proc.types.pointer.act_point.y))
		{
			unsigned int CurItem = 0, CurFocusItem = 0;
			lv_obj_t *CurFocus_btn_obj = NULL;
			
			if (auto_load_task) {
				lv_task_del(auto_load_task);
				auto_load_task = NULL;
			}
			CurFocus_btn_obj = lv_list_get_btn_selected(Explorer_Para.ui.medium_list_1);
			CurFocusItem = lv_list_get_btn_index(Explorer_Para.ui.medium_list_1, CurFocus_btn_obj);
			CurItem = lv_list_get_btn_index(Explorer_Para.ui.medium_list_1, btn);
			if (CurFocusItem == CurItem) {
				printf("not need to update\n");
				return ;
			}
			lv_list_set_btn_selected(Explorer_Para.ui.medium_list_1, btn);
			Explorer_Para.explorer_list->file_item = get_file_list_item(Explorer_Para.explorer_list->top_file_list, CurItem);
			Explorer_MediumList_Update();
		}
	}
}
static void Explorer_MediumList_AddItem(unsigned int FocusItem, unsigned int Index) {
	file_item_t *file_item = NULL; 
	rat_media_type_t type;
	lv_obj_t *btn_tmp = NULL;

	file_item = get_file_list_item(Explorer_Para.explorer_list->top_file_list, Index);//在node中搜索信息
	type = get_file_list_item_file_type(file_item);
	switch(type) {
		case RAT_MEDIA_TYPE_SD_DEVICE: 
			if (explorer_src[EXPLORER_IMAGE_SD] == NULL) {
				explorer_src[EXPLORER_IMAGE_SD] = parse_image_form_file(EXPLORER_IMAGE_SD_PATH);
			}
			btn_tmp = lv_list_add_btn(Explorer_Para.ui.medium_list_1,explorer_src[EXPLORER_IMAGE_SD], "SD");	
			break;
		case RAT_MEDIA_TYPE_USB_DEVICE:
			if (explorer_src[EXPLORER_IMAGE_USB] == NULL) {
				explorer_src[EXPLORER_IMAGE_USB] = parse_image_form_file(EXPLORER_IMAGE_USB_PATH);
			}
			btn_tmp = lv_list_add_btn(Explorer_Para.ui.medium_list_1,explorer_src[EXPLORER_IMAGE_USB], "USB");
			break;
		case RAT_MEDIA_TYPE_LOCAL_DEVICE:
			if (explorer_src[EXPLORER_IMAGE_SD] == NULL) {
				explorer_src[EXPLORER_IMAGE_SD] = parse_image_form_file(EXPLORER_IMAGE_SD_PATH);
			}
			btn_tmp = lv_list_add_btn(Explorer_Para.ui.medium_list_1,explorer_src[EXPLORER_IMAGE_SD], "LocalDisk");
			break;
		default:
		break;
	}
	if (FocusItem == Index) {
		lv_obj_t *btn_obj = NULL;
		
		btn_obj = lv_list_get_btn_selected(Explorer_Para.ui.medium_list_1);
		if (btn_obj) {
			lv_btn_set_state(btn_obj, LV_BTN_STATE_REL);
		}		
		Explorer_Para.explorer_list->file_item = get_file_list_item(Explorer_Para.explorer_list->top_file_list, FocusItem);
		lv_list_set_btn_selected(Explorer_Para.ui.medium_list_1, btn_tmp);
	}
	lv_obj_set_event_cb(btn_tmp, Explorer_MediumList_Event);
	Explorer_Para.explorer_list->file_item = get_file_list_item(Explorer_Para.explorer_list->top_file_list, FocusItem);
}
static void Explorer_MediumList_DelItem(unsigned int FocusItem, unsigned int Index) {
	lv_list_remove(Explorer_Para.ui.medium_list_1, Index);	
	Explorer_Para.explorer_list->file_item = get_file_list_item(Explorer_Para.explorer_list->top_file_list, FocusItem);	
}

static void Explorer_MediumList_Init(char *MountPoint) {
	unsigned int Index = 0, FocusItem = -1;

	Explorer_Para.explorer_list = (explorer_list_t*)malloc(sizeof(explorer_list_t));
	memset(Explorer_Para.explorer_list,0,sizeof(explorer_list_t));
	/*Left list*/
	Explorer_Para.explorer_list->top_file_list = new_file_root_list_nod(NULL);
	if (MountPoint) {
		FocusItem = FileList_GetItemNum_ByMountPoint(Explorer_Para.explorer_list->top_file_list, MountPoint);
	} else {
		FocusItem = 0;
	}
	if (Explorer_Para.explorer_list->top_file_list) {
		for (Index = 0; Index < Explorer_Para.explorer_list->top_file_list->total; Index++) {
			Explorer_MediumList_AddItem(FocusItem, Index);
		}
		Explorer_Para.explorer_list->cur_file_list = Explorer_Para.explorer_list->top_file_list;
	}
	Explorer_FileList_Update();	
	return ;
}
DiskInfo_t Disk_HotPlugMsg;
unsigned char Disk_Msg_Enable = 0;
static void Explorer_DiskHotPlugCallBack(DiskInfo_t *DiskInfo) {
	pthread_mutex_lock(&Explorer_HotPlug_Mutex);
	if (Disk_Msg_Enable == 0) {
		memset(&Disk_HotPlugMsg, 0x00, sizeof(DiskInfo_t));
		memcpy(&Disk_HotPlugMsg, DiskInfo, sizeof(DiskInfo_t));
		Disk_Msg_Enable = 1;
	}
	pthread_mutex_unlock(&Explorer_HotPlug_Mutex);
}
static void Explorer_HotPlug_Process_Task(lv_task_cb_t *task) {	
	pthread_mutex_lock(&Explorer_HotPlug_Mutex);
	if (Disk_HotPlugMsg.operate == MEDIA_PLUGIN && Disk_Msg_Enable) {
		FileList_AddFileNod_To_RootList(Explorer_Para.explorer_list->top_file_list, &Disk_HotPlugMsg);
		Explorer_MediumList_AddItem(Explorer_Para.explorer_list->top_file_list->total - 1, Explorer_Para.explorer_list->top_file_list->total - 1);
		Explorer_MediumList_Update();
		Disk_Msg_Enable = 0;
		memset(&Disk_HotPlugMsg, 0x00, sizeof(DiskInfo_t));
	} else if (Disk_HotPlugMsg.operate == MEDIA_PLUGOUT && Disk_Msg_Enable) {
		lv_obj_t *CurFocus_btn_obj = NULL, *NewFocus_btn_obj = NULL;
		int ItemNum = 0, FocusItem = 0, CurFocusItem = 0;
		
		ItemNum = FileList_GetItemNum_ByMountPoint(Explorer_Para.explorer_list->top_file_list, Disk_HotPlugMsg.MountPoint);
		FileList_DelFileNod_To_RootList(Explorer_Para.explorer_list->top_file_list, &Disk_HotPlugMsg);
		CurFocus_btn_obj = lv_list_get_btn_selected(Explorer_Para.ui.medium_list_1);
		CurFocusItem = lv_list_get_btn_index(Explorer_Para.ui.medium_list_1, CurFocus_btn_obj);
		if (ItemNum == CurFocusItem) {
			if (CurFocusItem == 0) {
				NewFocus_btn_obj = lv_list_get_next_btn(Explorer_Para.ui.medium_list_1, CurFocus_btn_obj);
			} else {
				NewFocus_btn_obj = lv_list_get_prev_btn(Explorer_Para.ui.medium_list_1, CurFocus_btn_obj);
			}
			if (NewFocus_btn_obj) {
				lv_list_set_btn_selected(Explorer_Para.ui.medium_list_1, NewFocus_btn_obj);
			} else {
				printf("not disk to explorer will return to home");
				create_page(PAGE_HOME);
				destory_page(PAGE_EXPLORER);
				Disk_Msg_Enable = 0;
				memset(&Disk_HotPlugMsg, 0x00, sizeof(DiskInfo_t));
				pthread_mutex_unlock(&Explorer_HotPlug_Mutex);
				return ;
			}
		} 
		Explorer_MediumList_DelItem(FocusItem, ItemNum);	
		Explorer_MediumList_Update();
		Disk_Msg_Enable = 0;
		memset(&Disk_HotPlugMsg, 0x00, sizeof(DiskInfo_t));
	}
	pthread_mutex_unlock(&Explorer_HotPlug_Mutex);
}

static int create_explorer(void)
{
//	rat_media_type_t Type = RAT_MEDIA_TYPE_EBOOK;
	rat_media_type_t Type = RAT_MEDIA_TYPE_ALL;

	memset(&Explorer_Para, 0x00, sizeof(explorer_para_t));
	Explorer_Para.ui.cont = lv_cont_create(lv_scr_act(), NULL);
	lv_obj_set_size(Explorer_Para.ui.cont, LV_HOR_RES_MAX, LV_VER_RES_MAX);
	static lv_style_t cont_style;
	lv_style_copy(&cont_style, &lv_style_plain);
	cont_style.body.main_color = LV_COLOR_BLUE;
	cont_style.body.grad_color = LV_COLOR_BLUE;
	lv_cont_set_style(Explorer_Para.ui.cont, LV_CONT_STYLE_MAIN, &cont_style);
	lv_cont_set_layout(Explorer_Para.ui.cont, LV_LAYOUT_OFF);
	lv_cont_set_fit(Explorer_Para.ui.cont, LV_FIT_NONE);

	static lv_style_t back_btn_style;
	lv_style_copy(&back_btn_style, &lv_style_pretty);
	back_btn_style.text.font = &lv_font_roboto_28;
	lv_obj_t *back_btn = lv_btn_create(Explorer_Para.ui.cont, NULL);
	lv_obj_align(back_btn, Explorer_Para.ui.cont, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_obj_t *back_lable = lv_label_create(back_btn, NULL);
	lv_label_set_text(back_lable, LV_SYMBOL_LEFT);
	lv_obj_set_event_cb(back_btn, back_btn_event);
	lv_btn_set_style(back_btn, LV_BTN_STYLE_REL, &back_btn_style);
	lv_btn_set_style(back_btn, LV_BTN_STYLE_PR, &back_btn_style);

	explorer_auto_ui_create(&Explorer_Para.ui);
	lv_list_set_sb_mode(Explorer_Para.ui.list_2, LV_SB_MODE_AUTO);
	
	//return imgbtn 
	lv_obj_set_click(Explorer_Para.ui.image_1, true);
	lv_obj_set_ext_click_area(Explorer_Para.ui.image_1, 27,27,27,27);
	lv_obj_set_event_cb(Explorer_Para.ui.image_1, Explorer_Return_Home_Event);
		
	label_dir_base = lv_label_create(Explorer_Para.ui.direct_cont,NULL);
	lv_label_set_long_mode(label_dir_base, LV_LABEL_LONG_SROLL_CIRC);
	lv_obj_set_pos(label_dir_base, 42, 10);
	lv_obj_set_size(label_dir_base, 519, 22);
	//back imgbtn	
	if (Type == RAT_MEDIA_TYPE_ALL) {
#ifdef LV_USE_IMG
		Explorer_Para.backImageBtn = lv_img_create(Explorer_Para.ui.cont, NULL);
		lv_obj_set_pos(Explorer_Para.backImageBtn, 763, 59);
		lv_obj_set_size(Explorer_Para.backImageBtn, 32, 32);
		backImageBuff = (void *)parse_image_form_file(LV_IMAGE_PATH"explorer_return.png");
		lv_img_set_src(Explorer_Para.backImageBtn, backImageBuff);
		lv_obj_set_click(Explorer_Para.backImageBtn, true);
		lv_obj_set_ext_click_area(Explorer_Para.backImageBtn, 27,27,27,27);
		lv_obj_set_event_cb(Explorer_Para.backImageBtn, Explorer_Back_Btn_Event);
#endif // LV_USE_IMG
	}	
	Explorer_Para.rat_ctrl.media_type = Type;
	strcpy(RegisterInfo.Cur_Win, "explorer");
	RegisterInfo.CallBackFunction = Explorer_DiskHotPlugCallBack;
	DiskManager_Register(&RegisterInfo);
	pthread_mutex_init(&Explorer_HotPlug_Mutex,NULL);
	if (DiskManager_GetDiskNum() == 0) {
		printf("not disk to explorer will return to home\n");
		create_page(PAGE_HOME);
		destory_page(PAGE_EXPLORER);
		Disk_Msg_Enable = 0;
		memset(&Disk_HotPlugMsg, 0x00, sizeof(DiskInfo_t));
		return 0;
	}
	Explorer_MediumList_Init(NULL);
	HotPlug_Process_task = lv_task_create((void *)Explorer_HotPlug_Process_Task, 10,LV_TASK_PRIO_MID,NULL);	
	return 0;
}

static int destory_explorer(void)
{
	__s32 Index = 0;
	
	DiskManager_UnRegister(&RegisterInfo);
	explorer_auto_ui_destory(&Explorer_Para.ui);
	lv_obj_del(Explorer_Para.ui.cont);
	if (Explorer_Para.backImageBtn) {
		lv_obj_del(Explorer_Para.backImageBtn);
	}
	if (label_dir_base) {
		lv_obj_del(label_dir_base);
	}
	if (auto_load_task) {
		lv_task_del(auto_load_task);
		auto_load_task = NULL;
	}

	if (HotPlug_Process_task) {
		lv_task_del(HotPlug_Process_task);
		HotPlug_Process_task = NULL;
	}

	if (Explorer_Para.explorer_list != NULL)
	{
		if (Explorer_Para.explorer_list->top_file_list != NULL) {
			delete_file_list_chain(Explorer_Para.explorer_list->top_file_list);
		}
		Explorer_Para.explorer_list->cur_file_list = NULL;
		Explorer_Para.explorer_list->top_file_list = NULL;
		free(Explorer_Para.explorer_list);
	}
	if (Explorer_Para.rat_ctrl.handle) {
		rat_close(Explorer_Para.rat_ctrl.handle);
		Explorer_Para.rat_ctrl.handle = 0;
	}
	
	if (backImageBuff) {
		free_image(backImageBuff);
		backImageBuff = NULL;
	}
	for (Index = 0; Index < EXPLORER_IMAGE_NUM_MAX; Index++) {
		if (explorer_src[Index]) {
			free_image(explorer_src[Index]);
			explorer_src[Index] = NULL;
		}
	}
	
	Disk_Msg_Enable = 0;
	memset(&Disk_HotPlugMsg, 0x00, sizeof(DiskInfo_t));	
    pthread_mutex_destroy(&Explorer_HotPlug_Mutex);
	return 0;
}

static int show_explorer(void)
{
	lv_obj_set_hidden(Explorer_Para.ui.cont, 0);

	return 0;
}

static int hide_explorer(void)
{
	lv_obj_set_hidden(Explorer_Para.ui.cont, 1);

	return 0;
}

static page_interface_t page_explorer = 
{
	.ops =
	{
		create_explorer,
		destory_explorer,
		show_explorer,
		hide_explorer,
	}, 
	.info =
	{
		.id         = PAGE_EXPLORER, 
		.user_data  = NULL
	}
};

void REGISTER_PAGE_EXPLORER(void)
{
	reg_page(&page_explorer);
}

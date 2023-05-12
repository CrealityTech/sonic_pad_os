/**********************
 *      includes
 **********************/
#include "moc_home.h"
#include "ui_home.h"
#include "lvgl.h"
#include "page.h"
#include "ui_resource.h"
#include <stdio.h>
#include "media_ui.h"
#include "debug.h"

/**********************
 *       variables
 **********************/
typedef struct
{
	home_ui_t ui;
} home_para_t;
static home_para_t para;


/**********************
 *  functions
 **********************/
static void back_btn_event(lv_obj_t * btn, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{
		destory_page(PAGE_HOME);
		create_page(PAGE_HOME);
	}
}

static void btn_usb_event(lv_obj_t * btn, lv_event_t event)
{
	static bool host_device = 0;
	if (event == LV_EVENT_CLICKED)
	{
        if(!host_device){
                system("cat /sys/devices/soc/usbc0/usb_host");
               // usleep(5000000);
              //  system("mount /dev/sda /mnt/SDCARD/");
        }else{
               // system("umount /mnt/SDCARD/");
                system("cat /sys/devices/soc/usbc0/usb_device");
        }
        host_device = !host_device;
	}
}

static void page_change_event(lv_obj_t * btn, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{
		if(btn == para.ui.image_button_1)
		{
#if 0
			destory_page(PAGE_HOME);
			create_page(PAGE_MOVIE);
			printf("[L=%d, F=%s]:\n", __LINE__, __FUNCTION__);
#else
			if(media_get_usb_status() || media_get_tf_status()){
				destory_page(PAGE_HOME);
				create_page(PAGE_MOVIE);
				printf("[L=%d, F=%s]:\n", __LINE__, __FUNCTION__);
			}else{
				debug_warn("usb don't insert");
				//lv_obj_set_hidden(para.ui.label_disk, false);
				//lv_task_handler();
				//usleep(1000000);
				//lv_obj_set_hidden(para.ui.label_disk, true);
			}
#endif
		}
		else if(btn == para.ui.image_button_2)
		{
#if 0
			destory_page(PAGE_HOME);
			create_page(PAGE_MUSIC);
			printf("[L=%d, F=%s]:\n", __LINE__, __FUNCTION__);
#else
			if(media_get_usb_status() || media_get_tf_status()){
				destory_page(PAGE_HOME);
				create_page(PAGE_MUSIC);
				printf("[L=%d, F=%s]:\n", __LINE__, __FUNCTION__);
			}else{
				debug_warn("usb don't insert");
				//lv_obj_set_hidden(para.ui.label_disk, false);
				//lv_task_handler();
				//usleep(1000000);
				//lv_obj_set_hidden(para.ui.label_disk, true);				
			}
#endif

		}
		else if(btn == para.ui.image_button_3)
		{
			destory_page(PAGE_HOME);
			create_page(PAGE_CALENDAR);
			printf("[L=%d, F=%s]:\n", __LINE__, __FUNCTION__);
		}
		else if(btn == para.ui.image_button_4)
		{
			//destory_page(PAGE_HOME);
			//create_page(PAGE_EXAMPLE);
			printf("[L=%d, F=%s]:\n", __LINE__, __FUNCTION__);
		}
		else if(btn == para.ui.image_button_5)
		{
			destory_page(PAGE_HOME);
			create_page(PAGE_PHOTO);
			printf("[L=%d, F=%s]:\n", __LINE__, __FUNCTION__);
		}
		else if(btn == para.ui.image_button_6)
		{
			destory_page(PAGE_HOME);
			create_page(PAGE_CALCULATOR);
			printf("[L=%d, F=%s]:\n", __LINE__, __FUNCTION__);
		}
		else if(btn == para.ui.image_button_7)
		{
			destory_page(PAGE_HOME);
			create_page(PAGE_EXPLORER);
			printf("[L=%d, F=%s]:\n", __LINE__, __FUNCTION__);
		}
		else if(btn == para.ui.image_button_8)
		{
			//destory_page(PAGE_HOME);
			//create_page(PAGE_SETTING);
			btn_usb_event(btn, event);
			printf("[L=%d, F=%s]:\n", __LINE__, __FUNCTION__);
		}
	}
}

static int create_home(void)
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
	lv_obj_t *back_btn = lv_btn_create(para.ui.cont, NULL);
	lv_obj_align(back_btn, para.ui.cont, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_obj_t *back_lable = lv_label_create(back_btn, NULL);
	lv_label_set_text(back_lable, LV_SYMBOL_LEFT);
	lv_obj_set_event_cb(back_btn, back_btn_event);
	lv_btn_set_style(back_btn, LV_BTN_STYLE_REL, &back_btn_style);
	lv_btn_set_style(back_btn, LV_BTN_STYLE_PR, &back_btn_style);

	home_auto_ui_create(&para.ui);
	lv_obj_set_event_cb(para.ui.image_button_1, page_change_event);
	lv_obj_set_event_cb(para.ui.image_button_2, page_change_event);
	lv_obj_set_event_cb(para.ui.image_button_3, page_change_event);
	lv_obj_set_event_cb(para.ui.image_button_4, page_change_event);
	lv_obj_set_event_cb(para.ui.image_button_5, page_change_event);
	lv_obj_set_event_cb(para.ui.image_button_6, page_change_event);
	lv_obj_set_event_cb(para.ui.image_button_7, page_change_event);
	lv_obj_set_event_cb(para.ui.image_button_8, page_change_event);

	return 0;
}

static int destory_home(void)
{

	home_auto_ui_destory(&para.ui);
	lv_obj_del(para.ui.cont);
	return 0;
}

static int show_home(void)
{
	lv_obj_set_hidden(para.ui.cont, 0);

	return 0;
}

static int hide_home(void)
{
	lv_obj_set_hidden(para.ui.cont, 1);

	return 0;
}

static page_interface_t page_home = 
{
	.ops =
	{
		create_home,
		destory_home,
		show_home,
		hide_home,
	}, 
	.info =
	{
		.id         = PAGE_HOME, 
		.user_data  = NULL
	}
};

void REGISTER_PAGE_HOME(void)
{
	reg_page(&page_home);
}

#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/sunxi_ctpdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "page.h"
#include "va_business.h"
#include "DiskManager.h"
#include "rat.h"

#define DISP_BUF_SIZE (480 * LV_HOR_RES_MAX)

pthread_t tick_id;

extern int evdev_root_x;
extern int evdev_root_y;
extern int evdev_button;

void *tick_thread(void * data)
{
    (void)data;

    while(1) {
        usleep(10000);   /*Sleep for 5 millisecond*/
        lv_tick_inc(10); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return 0;
}

/**
 * Print the memory usage periodically
 * @param param
 */
static void memory_monitor(lv_task_t * param)
{
    (void) param; /*Unused*/

    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    printf("used: %6d (%3d %%), frag: %3d %%, biggest free: %6d\n", (int)mon.total_size - mon.free_size,
            mon.used_pct,
            mon.frag_pct,
            (int)mon.free_biggest_size);

}

static void hal_init(void)
{
	int ret;
	/*Linux frame buffer device init*/
    fbdev_init();
    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];
    /*Initialize a descriptor for the buffer*/
    static lv_disp_buf_t disp_buf;
    lv_disp_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);
	//printf("L:%d F=%s: buf=%x\n", __LINE__, __FUNCTION__, buf);

    /*Initialize and register a display driver*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer   = &disp_buf;
    disp_drv.flush_cb = fbdev_flush;
    lv_disp_drv_register(&disp_drv);

	ctpdev_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = ctpdev_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_t * mouse_indev = lv_indev_drv_register(&indev_drv);

	ret = pthread_create(&tick_id, NULL, tick_thread, NULL);
	if (ret == -1)
	{
		printf("create thread fail\n");
		return ;
	}
	printf("tick_id=%d\n", tick_id);

    /* Optional:
     * Create a memory monitor task which prints the memory usage in periodically.*/
    //lv_task_create(memory_monitor, 3000, LV_TASK_PRIO_MID, NULL);
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}

lv_task_t *main_task;
//LV_IMG_DECLARE(img_1_800x480)
//LV_IMG_DECLARE(img_2_800x480)

#if 0
// home
typedef struct
{
	uint8_t id;
	lv_obj_t *cont;
	lv_obj_t *image_1;
	lv_obj_t *button_1;
} home_ui_t;

typedef struct
{
	home_ui_t ui;
} home_para_t;
static home_para_t para;

void home_auto_ui_create(home_ui_t *ui)
{

/*
#ifdef LV_USE_IMG
	static lv_style_t style0_image_1;

	lv_style_copy(&style0_image_1, &lv_style_plain);

	ui->image_1 = lv_img_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_1, 0, 0);
	lv_obj_set_size(ui->image_1, 800, 480);
	lv_img_set_src(ui->image_1, &img_1_800x480);

	lv_img_set_style(ui->image_1, LV_IMG_STYLE_MAIN, &style0_image_1);
#endif // LV_USE_IMG
*/

#ifdef LV_USE_BTN
	ui->button_1 = lv_btn_create(ui->cont, NULL);
	lv_obj_set_pos(ui->button_1, 10, 10);
	lv_obj_set_size(ui->button_1, 780, 460);
#endif // LV_USE_BTN

}

void home_auto_ui_destory(home_ui_t *ui)
{
	lv_obj_clean(ui->cont);
}

static void back_btn_event(lv_obj_t * btn, lv_event_t event)
{
	static int flag = 0;
	if (event == LV_EVENT_CLICKED)
	{
		printf("L:%d F=%s:\n", __LINE__, __FUNCTION__);

		flag = !flag;
		if(flag == 0)
		{
		}
		else
		{
		}

	}
}

int create_home(void)
{
	para.ui.cont = lv_cont_create(lv_scr_act(), NULL);
	lv_obj_set_size(para.ui.cont, LV_HOR_RES_MAX, LV_VER_RES_MAX);
	static lv_style_t cont_style;
	lv_style_copy(&cont_style, &lv_style_plain);
	cont_style.body.main_color = LV_COLOR_RED;
	cont_style.body.grad_color = LV_COLOR_RED;
	lv_cont_set_style(para.ui.cont, LV_CONT_STYLE_MAIN, &cont_style);
	lv_cont_set_layout(para.ui.cont, LV_LAYOUT_OFF);
	lv_cont_set_fit(para.ui.cont, LV_FIT_NONE);

	static lv_style_t back_btn_style;
	lv_style_copy(&back_btn_style, &lv_style_pretty);
	lv_obj_t *back_btn = lv_btn_create(para.ui.cont, NULL);
	lv_obj_align(back_btn, para.ui.cont, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_obj_set_event_cb(back_btn, back_btn_event);
	lv_btn_set_style(back_btn, LV_BTN_STYLE_REL, &back_btn_style);
	lv_btn_set_style(back_btn, LV_BTN_STYLE_PR, &back_btn_style);

	home_auto_ui_create(&para.ui);

	lv_obj_move_foreground(back_btn);
	return 1;
}

int destory_home(void)
{
	home_auto_ui_destory(&para.ui);
	lv_obj_del(para.ui.cont);

	return 1;
}


typedef struct
{
	uint8_t id;
	lv_obj_t *cont;
	lv_obj_t *image_1;
} music_ui_t;

typedef struct
{
	music_ui_t ui;
} music_para_t;
static music_para_t music_para;

void music_auto_ui_create(music_ui_t *ui)
{

#if 0
#ifdef LV_USE_IMG
	static lv_style_t style0_image_1;
	lv_style_copy(&style0_image_1, &lv_style_plain);

	ui->image_1 = lv_img_create(ui->cont, NULL);
	lv_obj_set_pos(ui->image_1, 0, 0);
	lv_obj_set_size(ui->image_1, 800, 480);
	lv_img_set_src(ui->image_1, &img_2_800x480);

	lv_img_set_style(ui->image_1, LV_IMG_STYLE_MAIN, &style0_image_1);
#endif // LV_USE_IMG
#endif


}

void music_auto_ui_destory(music_ui_t *ui)
{
	lv_obj_clean(ui->cont);
}

static void music_back_btn_event(lv_obj_t * btn, lv_event_t event)
{
	static int flag = 0;
	if (event == LV_EVENT_CLICKED)
	{
		printf("L:%d F=%s:\n", __LINE__, __FUNCTION__);

		flag = !flag;
		if(flag == 0)
		{

		}
		else
		{

		}

	}
}

int create_music(void)
{
	music_para.ui.cont = lv_cont_create(lv_scr_act(), NULL);
	lv_obj_set_size(music_para.ui.cont, LV_HOR_RES_MAX, LV_VER_RES_MAX);
	static lv_style_t cont_style;
	lv_style_copy(&cont_style, &lv_style_plain);
	cont_style.body.main_color = LV_COLOR_BLUE;
	cont_style.body.grad_color = LV_COLOR_BLUE;
	lv_cont_set_style(music_para.ui.cont, LV_CONT_STYLE_MAIN, &cont_style);
	lv_cont_set_layout(music_para.ui.cont, LV_LAYOUT_OFF);
	lv_cont_set_fit(music_para.ui.cont, LV_FIT_NONE);

	static lv_style_t back_btn_style;
	lv_style_copy(&back_btn_style, &lv_style_pretty);
	lv_obj_t *back_btn = lv_btn_create(music_para.ui.cont, NULL);
	lv_obj_align(back_btn, music_para.ui.cont, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_obj_set_event_cb(back_btn, music_back_btn_event);
	lv_btn_set_style(back_btn, LV_BTN_STYLE_REL, &back_btn_style);
	lv_btn_set_style(back_btn, LV_BTN_STYLE_PR, &back_btn_style);

	music_auto_ui_create(&music_para.ui);

	lv_obj_move_foreground(back_btn);
	return 1;
}

int destory_music(void)
{
	music_auto_ui_destory(&music_para.ui);
	lv_obj_del(music_para.ui.cont);

	return 1;
}
#endif

void lv_main_task(lv_task_t * task)
{
	static int counter;
	static int flag = 0;


	counter++;
	//if(counter % 100 == 1)
	{
		printf("L:%d F=%s: counter=%d\n", __LINE__, __FUNCTION__, counter);
	}

	flag = !flag;
	printf("L:%d F=%s: flag=%d\n", __LINE__, __FUNCTION__, flag);
	if(flag == 0)
	{
		//destory_music();
		//create_home();
	}
	else
	{
		//destory_home();
		//create_music();
	}

}

int main(void)
{
	//printf("L:%d, F=%s, main...\n", __LINE__, __FUNCTION__);
    /*LittlevGL init*/
//	DiskInfo_t DeviceInfo;

    lv_init();

	/* hal init */
	hal_init();

	//lv_obj_t * scr = lv_disp_get_scr_act(NULL);
    //lv_obj_t * btn1 = lv_btn_create(scr, NULL);
	//lv_obj_set_pos(btn1, 0, 0);
	//lv_obj_set_size(btn1, 400, 240);

	//lv_tutorial_hello_world();
	//lv_tutorial_animations();
	//demo_create();

	//create_home();
	//indev->driver.read_task = lv_task_create(lv_indev_read_task, LV_INDEV_DEF_READ_PERIOD, LV_TASK_PRIO_MID, indev);
	//main_task = lv_task_create(lv_main_task, 2000, LV_TASK_PRIO_MID, NULL);

	page_init();
	REG_PAGE(PAGE_HOME);
    REG_PAGE(PAGE_MUSIC);
    REG_PAGE(PAGE_MOVIE);
	REG_PAGE(PAGE_CALCULATOR);
    REG_PAGE(PAGE_CALENDAR)	;
    REG_PAGE(PAGE_EXPLORER);
    REG_PAGE(PAGE_FOLDER);
	REG_PAGE(PAGE_PHOTO) ;
    REG_PAGE(PAGE_SETTING);
	REG_PAGE(PAGE_EXAMPLE);
	DiskManager_Init();
	rat_init();
	
//	memset(&DeviceInfo, 0x00, sizeof(DeviceInfo));
//	strcpy(DeviceInfo.DeviceName, "/dev/mmcblk0p1");
//	strcpy(DeviceInfo.MountPoint, "/mnt/UDISK");
//	DeviceInfo.MediaType = MEDIA_SD_CARD;
//	DeviceInfo.Major = 179;
//	DeviceInfo.Minor = 1;
//	DiskManager_Register_StaticDisk(&DeviceInfo);
	create_page(PAGE_HOME);
	
	media_ui_pthread_create(NULL);
    /*Handle LitlevGL tasks (tickless mode)*/
	int start;
	int end;
	int inter;
    while(1)
	{
		start = lv_tick_get();
        lv_task_handler();
		end = lv_tick_get();

		#if 1
		/* Adjust the sleep time according to the main task processing time */
		inter = end - start;
		if (inter >= 40) {
			;
		}
		else if(inter >= 20) {
			usleep(5000);
		}
		else {
			usleep(10000);
		}
		#endif
    }
	media_ui_pthread_destroy();
	rat_deinit();
	pthread_join(tick_id, NULL);
	va_power_power_off();
    return 0;
}

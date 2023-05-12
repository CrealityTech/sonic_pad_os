#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/sunxi_ctpdev.h"
#include "lv_examples/lv_examples.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define DISP_BUF_SIZE (480 * LV_HOR_RES_MAX)

pthread_t tick_id;

void *tick_thread(void * data)
{
    (void)data;

    while(1) {
        usleep(10000);   /*Sleep for 10 millisecond*/
        lv_tick_inc(10); /*Tell LittelvGL that 10 milliseconds were elapsed*/
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

int main(void)
{
	//printf("L:%d, F=%s, main...\n", __LINE__, __FUNCTION__);
    /*LittlevGL init*/
    lv_init();

	/* hal init */
	hal_init();

    /*Create a Demo*/
    //lv_demo_widgets();
	//lv_tutorial_hello_world();
	//lv_tutorial_objects();
	demo_create();

    /*Handle LitlevGL tasks (tickless mode)*/
    while(1)
	{
        lv_task_handler();
        usleep(10000);
    }

	pthread_join(tick_id, NULL);

    return 0;
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

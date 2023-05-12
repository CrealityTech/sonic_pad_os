/**
 * @file: sunxi_ctpdev.h
 * @autor: huangyixiu
 * @url: huangyixiu@allwinnertech.com
 */
#ifndef SUNXI_CTPDEV_H
#define SUNXI_CTPDEV_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../../lv_drv_conf.h"
#endif
#endif

#if USE_SUNXI_CTPDEV

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/**
 * Initialize the ctpdev interface
 */
void ctpdev_init(void);

/**
 * unitialize the ctpdev interface
 */
void ctpdev_uninit(void);

/**
 * Get the current position and state of the evdev
 * @param data store the evdev data here
 * @return false: because the points are not buffered, so no more data to be read
 */
bool ctpdev_read(lv_indev_drv_t * drv, lv_indev_data_t * data);



#endif /* USE_SUNXI_CTPDEV */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SUNXI_CTPDEV_H */

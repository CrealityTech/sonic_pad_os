#ifndef _VA_DISPLAY_H_
#define _VA_DISPLAY_H_

#include <stdio.h>
#include <stdbool.h>
#include "sunxi_display_v2.h"

int va_display_lcd_onoff(bool onoff);
int va_display_lcd_backlight_onoff(bool onoff);
int va_display_lcd_set_brightness(int val);
int va_display_get_lcd_rect(disp_rectsz *rect);

#endif

#ifndef __WIN_INCLUDE_H__
#define __WIN_INCLUDE_H__

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <sys/time.h>

//#define GUI_DEBUG
#define GUI_WARN
#define GUI_ERROR

#ifdef GUI_DEBUG
#define gui_debug(fmt, ...) \
	do {fprintf(stdout, "[%s %d]dbg: ", __FILE__, __LINE__); \
		fprintf(stdout, fmt, ##__VA_ARGS__); } while (0)

/*
	do {struct timespec ts; \
		clock_gettime(CLOCK_MONOTONIC, &ts);\
		fprintf(stdout, "[%s %d <%d.%d>]dbg: ", __FILE__, __LINE__, ts.tv_sec,ts.tv_nsec); \
		fprintf(stdout, fmt, ##__VA_ARGS__); } while (0)
		*/
#else
#define gui_debug(fmt, ...) \
	do {;} while (0)
#endif

#ifdef GUI_WARN
#define gui_warn(fmt, ...) \
	do {fprintf(stdout, "[%s %d]wrn: ", __FILE__, __LINE__); \
		fprintf(stdout, fmt, ##__VA_ARGS__); } while (0)
#else
#define gui_warn(fmt, ...) \
	do {;} while (0)
#endif

#ifdef GUI_ERROR
#define gui_error(fmt, ...) \
	do {fprintf(stdout, "[%s %d]err: ", __FILE__, __LINE__); \
		fprintf(stdout, fmt, ##__VA_ARGS__); } while (0)
#else
#define gui_error(fmt, ...) \
	do {;} while (0)
#endif


#define HOMEPAGE "homepage"
#define MUSICLIST "musiclist"
#define PHOTOLIST "photolist"
#define PHOTOPLAYLIST "photoplaylist"
#define VIDEOLIST "videolist"
#define PLAYERLIST "playerlist"
#define SETTINGLIST "settinglist"
#define MPLAYERLIST "mplayerlist"

#define USER_TIMER_ID	101
#define USER_MSG_WIN_CHANGE		0x801
#define MAX_TEXT_SIZE	64

typedef void (*pButtonFun)(int index, DWORD param);

typedef struct BmpType
{
	GAL_Rect sRect;		// bmp show rect
	BITMAP sBmp;		// bmp src;
}BmpType;

typedef struct TextType
{
	char pText[MAX_TEXT_SIZE];	// text string
	GAL_Rect sRect;		// text show rect
	gal_pixel sColor;	// text color
	PLOGFONT sFont;		// test font
}TextType;

typedef struct ButtonType
{
	GAL_Rect sRect;			// active rect
	BITMAP onBmp;		// focus bmp
	BITMAP offBmp;		// unfocus bmp
	int onStus;			// bfocus
	TextType sText;		// text type
	pButtonFun fun;		// callback function
}ButtonType;


HWND GetMonitorHwnd(void);
void unloadBitMap(BITMAP *bmp);

#endif

#ifndef __SLIDEBOXVIEW_H__
#define __SLIDEBOXVIEW_H__

#include "middle_inc.h"
#include "winInclude.h"

#define SLIDEBOX_VIEW "slidebox"

typedef struct widgetData{
	int bx;
	int by;
	int bw;
	int bh;
	int index;
	BITMAP bmp;

	char text[64];
	RECT textRect;

	void *data;
	pButtonFun fun;
}widgetData;

typedef struct slideBoxView{
	int x;
	int y;
	int w;
	int h;
	BITMAP winbg;

	int widgetNum;
	int showNum;
	int widgetGap;
	HWND hwnd;
	PLOGFONT textFt;
	gal_pixel textColor;

	int btDownFlag;
	int oldX;
	int moveSenstive;
	int moveFlag;
	int moveGap;
	int curWidgetID;		//current widget ID in the middle screen
	int moveDirection;		//widgets' move direction after button up
	int turnPoint;			//record the coordinate when slide direction changed

	widgetData *widgets;
}slideBoxView;

#define SLIDEBOXTIMERID 0x0701

extern BOOL RegisterslideBoxView(void);
extern void UnregisterslideBoxView(void);
extern void slideBoxViewDeinit(slideBoxView *slideBox);
#endif

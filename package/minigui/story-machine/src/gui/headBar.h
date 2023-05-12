#ifndef __HEADBAR_H__
#define __HEADBAR_H__
#include "middle_inc.h"
#include "winInclude.h"

#define HEADBAR_VIEW "headbarview"

typedef struct {
	BITMAP onBmp;
	BITMAP offBmp;
	int onStus;
	int sltItem;
	char text[64];
	pButtonFun fun;
}HeadbarButtonData;

typedef struct headbarItem{
	char text[64];
	HeadbarButtonData *btn;
}headbarItem;

enum itemId{
	HEADBAR_LEFT,
	HEADBAR_RIGHT
};

typedef struct headBarView{
	int x;
	int y;
	int width;
	int height;

	int gapwbx;	//gap of widget and boundry in x
	int gapwby;	//gap of widget and boundry in y
	int textY;	//middleItem Y position

	BITMAP headBarBg;
	HWND parenthwnd;
	HWND handler;
	gal_pixel textColor;

	void *data;
	PLOGFONT textFt;

	headbarItem *leftItem;
	headbarItem *middleItem;
	headbarItem *rightItem;
}headBarView;

extern BOOL RegisterHeadBarView(void);
extern void UnregisterHeadBarView(void);
extern void HeadBarDataDeinit(headBarView *headBar);

#endif

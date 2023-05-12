#ifndef LISTBOX_VIEW_H_
#define LISTBOX_VIEW_H_

#include "middle_inc.h"
#include "winInclude.h"
#include <string.h>
#include <unistd.h>

#define NAME_MAX_SIZE 128
#define LISTBOX_VIEW "ListboxView"

// item mode 1
#define RITEM_NONE		0x0
#define RITEM_LTEXT	0x01
#define RITEM_RTEXT	0x02
#define RITEM_BMP		0x03
#define RITEM_BUTTON	0x04

// item mode 2
#define ITEM_NONE		0x0
#define ITEM_LINE		0x01
#define ITEM_BMP		0x02

typedef struct {
	BITMAP onBmp;
	BITMAP offBmp;
	int onStus;
	int sltItem;
	char text[NAME_MAX_SIZE];
	pButtonFun fun;
}ButtonData;

typedef struct {
	int rType;
	char lT[NAME_MAX_SIZE];
	char rT[NAME_MAX_SIZE];
	BITMAP rB;
	ButtonData *btn;

}ItemData;

typedef struct {
	HWND sfHwnd;
	HWND parentHwnd;

	BITMAP winBg;

	PLOGFONT lTextFt[100];
	gal_pixel lTextColor;
	PLOGFONT rTextFt[100];
	gal_pixel rTextColor;

	// disp rect
	int x;
	int ht;
	int y;
	int vt;

	// hfp ix len
	int hfp;
	int ix;

	// iy vgap len
	int iy;
	int vgap;

	// item define
	int itType;
	BITMAP ltbBg;
	gal_pixel lineColor;
	ItemData *itDt;
	int itSz;
	int Index;
	int showSz;
	int lthGap;

	//  not need init
	int moveGap;
	BOOL isSlidText;
	int oldY;
	int moveNum;
	int moveFlag;
}ListboxView;

BOOL RegisterListboxView(void);
void UnregisterListboxView(void);
extern void ListBoxDataDeinit(ListboxView *listboxView);

#endif

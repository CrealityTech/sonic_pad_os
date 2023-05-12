#ifndef __RECTBOX_H__
#define __RECTBOX_H__
#include "middle_inc.h"
#include "winInclude.h"

#define RECTBOX "rectbox"

typedef struct RectBox
{
	GAL_Rect sRect;	// RectBox rect
	BmpType sBg;	// RectBox background
	HWND hwnd;

	int nTextNum;
	TextType *pText;

	int nBmpNum;
	BmpType *pBmp;

	int nBtnNum;
	ButtonType *pBtn;

	int moveGapX;
	int moveGapY;
	int oldY;
	int oldX;
	int moveFlag;
	int moveNum;
	pButtonFun mvFun;

}RectBox;

BOOL RegisterRectBox(void);
void UnregisterRectBox(void);
void RectBoxDataUninit(RectBox *rectBox);

#endif

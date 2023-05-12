#ifndef __HEADBARA_H__
#define __HEADBARA_H__
#include "middle_inc.h"
#include "winInclude.h"

#define HEADBARA "headbarA"

typedef struct HeadBarA
{
	HWND hwnd;
	GAL_Rect sRect;	// headbar rect;
	BmpType sBg;	// headbar background

	int nTextNum;
	TextType *pText;

	int nBmpNum;
	BmpType *pBmp;

	int nBtnNum;
	ButtonType *pBtn;

}HeadBarA;

BOOL RegisterHeadBarA(void);
void UnregisterHeadBarA(void);
void HeadBarADataUninit(HeadBarA *pHbar);

#endif

#include "headBar.h"
#include "middle_inc.h"

static int HeadBarViewProc(HWND hwnd, int message, WPARAM wParam,LPARAM lParam)
{
	int newX = 0;
	int newY = 0;
	static headBarView *headbar = NULL;
	headbar = (headBarView*)GetWindowAdditionalData(hwnd);

	switch (message)
	{
	case MSG_PAINT:
	{
		int res;
		int a, b;
		SIZE size;
		HDC hdc = BeginPaint(hwnd);
		gui_debug("MSG_PAINT\n");

		if(headbar->headBarBg.bmBits){
			FillBoxWithBitmap(hdc, 0, 0, headbar->width, headbar->height,  &headbar->headBarBg);
		}

		if(headbar->leftItem != NULL && headbar->leftItem->btn != NULL){
			if(headbar->leftItem->btn->onStus == 0 && headbar->leftItem->btn->onBmp.bmBits){
				FillBoxWithBitmap(hdc, headbar->gapwbx, headbar->gapwby, headbar->leftItem->btn->onBmp.bmWidth,
					headbar->leftItem->btn->onBmp.bmHeight,  &headbar->leftItem->btn->onBmp);
			}else{
				if(headbar->leftItem->btn->offBmp.bmBits){
					FillBoxWithBitmap(hdc, headbar->gapwbx, headbar->gapwby, headbar->leftItem->btn->offBmp.bmWidth,
						headbar->leftItem->btn->offBmp.bmHeight,  &headbar->leftItem->btn->offBmp);
				}
			}
		}
		if(headbar->middleItem != NULL){
//			SetTextColor(hdc, RGB2Pixel(HDC_SCREEN, 255, 179, 9));
			SetTextColor(hdc, headbar->textColor);
			SelectFont(hdc, headbar->textFt);
			SetBkMode(hdc, BM_TRANSPARENT);

			GetTextExtent(hdc, headbar->middleItem->text, -1, &size);
			TextOut(hdc, (headbar->width - size.cx)/2 ,headbar->textY, headbar->middleItem->text);
		}
		if(headbar->rightItem != NULL && headbar->rightItem->btn != NULL){
			if(headbar->rightItem->btn->onStus == 0 && headbar->rightItem->btn->onBmp.bmBits){
				FillBoxWithBitmap(hdc, headbar->width - headbar->gapwbx - headbar->rightItem->btn->onBmp.bmWidth,
					headbar->gapwby, headbar->rightItem->btn->onBmp.bmWidth,
					headbar->rightItem->btn->onBmp.bmHeight,  &headbar->rightItem->btn->onBmp);
			}else{
				if(headbar->rightItem->btn->offBmp.bmBits){
					FillBoxWithBitmap(hdc, headbar->width - headbar->gapwbx - headbar->rightItem->btn->offBmp.bmWidth,
						headbar->gapwby, headbar->rightItem->btn->offBmp.bmWidth,
						headbar->rightItem->btn->offBmp.bmHeight,  &headbar->rightItem->btn->offBmp);
				}
			}
		}
		EndPaint(hwnd, hdc);

		return 0;
	}
	case MSG_LBUTTONDOWN:
	{
		newX = LOWORD(lParam);
		newY = HIWORD(lParam);
		gui_debug("MSG_LBUTTONDOWN####################\n");
		gui_debug("newX=%d, newY=%d\n", newX, newY);

//		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
	case MSG_LBUTTONUP:
	{
		gui_debug("MSG_LBUTTONUP#####################\n");
		newX = LOWORD(lParam);
		newY = HIWORD(lParam);
		gui_debug("newX=%d, newY=%d\n", newX, newY);

		if(headbar->leftItem != NULL){
			if(headbar->leftItem->btn->onStus == 0){
				if((newX >  headbar->gapwbx && newX < headbar->gapwbx + headbar->leftItem->btn->onBmp.bmWidth) &&
					(newY > headbar->gapwby && newY < headbar->gapwby + headbar->leftItem->btn->onBmp.bmHeight)){
					//TODO : LEFT BUTTON PRESSED
					gui_debug("LEFT BUTTON PRESSED\n");
					headbar->leftItem->btn->fun(HEADBAR_LEFT, (DWORD)headbar);
				}
			}else{
				if((newX >	headbar->gapwbx && newX < headbar->gapwbx + headbar->leftItem->btn->offBmp.bmWidth) &&
					(newY > headbar->gapwby && newY < headbar->gapwby + headbar->leftItem->btn->offBmp.bmHeight)){
					//TODO : LEFT BUTTON PRESSED
					gui_debug("LEFT BUTTON PRESSED\n");
					headbar->leftItem->btn->fun(HEADBAR_LEFT, (DWORD)headbar);
				}
			}
		}
		if(headbar->rightItem != NULL){
			if(headbar->rightItem->btn->onStus == 0){
				if((newX > headbar->width - headbar->gapwbx - headbar->rightItem->btn->onBmp.bmWidth && newX < headbar->width - headbar->gapwbx) &&
					(newY > headbar->gapwby && newY < headbar->gapwby + headbar->rightItem->btn->onBmp.bmHeight)){
					//TODL: RIGHT BUTTON PRESSED
					gui_debug("RIGHT BUTTON PRESSED\n");
					headbar->leftItem->btn->fun(HEADBAR_RIGHT, (DWORD)headbar);
				}
			}else{
				if((newX > headbar->width - headbar->gapwbx - headbar->rightItem->btn->offBmp.bmWidth && newX < headbar->width - headbar->gapwbx) &&
					(newY > headbar->gapwby && newY < headbar->gapwby + headbar->rightItem->btn->offBmp.bmHeight)){
					//TODL: RIGHT BUTTON PRESSED
					gui_debug("RIGHT BUTTON PRESSED\n");
					headbar->leftItem->btn->fun(HEADBAR_RIGHT, (DWORD)headbar);
				}
			}
		}

		//SetTimer(hwnd, LISTBOX_TIMER_ID, 1);
		break;
	}
	default:
		break;
	}

	return DefaultControlProc(hwnd, message, wParam, lParam);
}

BOOL RegisterHeadBarView(void)
{
	WNDCLASS MyClass;
	MyClass.spClassName = HEADBAR_VIEW;
	MyClass.dwStyle = WS_NONE;
	MyClass.dwExStyle = WS_EX_NONE;
	MyClass.hCursor = 0;
	MyClass.iBkColor = PIXEL_lightgray;
	MyClass.WinProc = HeadBarViewProc;

	return RegisterWindowClass(&MyClass);
}

void UnregisterHeadBarView(void)
{
	UnregisterWindowClass(HEADBAR_VIEW);
}

void HeadBarDataDeinit(headBarView *headBar)
{
	if(headBar){
		if(headBar->leftItem != NULL){
			if(headBar->leftItem->btn != NULL){
				unloadBitMap(&headBar->leftItem->btn->offBmp);
				unloadBitMap(&headBar->leftItem->btn->onBmp);
				free(headBar->leftItem->btn);
				headBar->leftItem->btn = NULL;
			}
			free(headBar->leftItem);
			headBar->leftItem = NULL;
		}
		if(headBar->rightItem != NULL){
			if(headBar->rightItem->btn != NULL){
				unloadBitMap(&headBar->rightItem->btn->offBmp);
				unloadBitMap(&headBar->rightItem->btn->onBmp);
				free(headBar->rightItem->btn);
				headBar->rightItem->btn = NULL;
			}
			free(headBar->rightItem);
			headBar->rightItem = NULL;
		}
		if(headBar->middleItem != NULL){
			if(headBar->middleItem->btn != NULL){
				unloadBitMap(&headBar->middleItem->btn->offBmp);
				unloadBitMap(&headBar->middleItem->btn->onBmp);
				free(headBar->middleItem->btn);
				headBar->middleItem->btn = NULL;
			}
			free(headBar->middleItem);
			headBar->middleItem = NULL;
		}
		free(headBar);
	}
}

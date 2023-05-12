#include "slideBoxView.h"
#include <sys/time.h>

static int getCurMiddleItemID(slideBoxView* slidebox)
{
	int num = 0, tmpNum = 0, i = 0;
	unsigned int min = -1;
	for(i = 0; i < slidebox->widgetNum; i++){
		tmpNum = abs(slidebox->widgets[i].bx + slidebox->widgets[i].bw/2 - slidebox->w/2);
		min = tmpNum > min? min: tmpNum;
		num = tmpNum > min? num: i;
	}
	return num;
}

static void slideBoxCycleShow(slideBoxView* slidebox)
{
	int i;
	for(i = 0; i < slidebox->widgetNum; i++){
		if(slidebox->widgets[i].bx >= 0 &&	slidebox->widgets[i].bx < 800){
			if(i == 0){
				slidebox->widgets[slidebox->widgetNum-1].bx = slidebox->widgets[i].bx - slidebox->widgetGap;
				slidebox->widgets[slidebox->widgetNum-1].textRect.left = slidebox->widgets[i].textRect.left - slidebox->widgetGap;
				slidebox->widgets[slidebox->widgetNum-1].textRect.right = slidebox->widgets[i].textRect.right - slidebox->widgetGap;
			}else{
				slidebox->widgets[(i-1)].bx = slidebox->widgets[i].bx - slidebox->widgetGap;
				slidebox->widgets[(i-1)].textRect.left = slidebox->widgets[i].textRect.left - slidebox->widgetGap;
				slidebox->widgets[(i-1)].textRect.right = slidebox->widgets[i].textRect.right - slidebox->widgetGap;
			}
		}

		if(slidebox->widgets[i].bx + slidebox->widgetGap <= 800 &&	slidebox->widgets[i].bx > 0){
			slidebox->widgets[(i+1)%slidebox->widgetNum].bx = slidebox->widgets[i%slidebox->widgetNum].bx + slidebox->widgetGap;
			slidebox->widgets[(i+1)%slidebox->widgetNum].textRect.left = slidebox->widgets[i%slidebox->widgetNum].textRect.left + slidebox->widgetGap;
			slidebox->widgets[(i+1)%slidebox->widgetNum].textRect.right = slidebox->widgets[i%slidebox->widgetNum].textRect.right + slidebox->widgetGap;
		}
	}
}

#define AUTOMOVESPEED 100
#define SLIDETHRESHOLD 30

static int slideBoxViewProc(HWND hwnd, int message, WPARAM wParam,LPARAM lParam)
{
	int i;
	int newX = 0;
	int newY = 0;
	static int moveTimes = 0;
	slideBoxView *slidebox = NULL;
	slidebox = (slideBoxView*)GetWindowAdditionalData(hwnd);

	switch (message)
	{
	case MSG_CREATE:
	{
		if(slidebox->moveSenstive == 0){
			slidebox->moveSenstive = 1;
		}
		return 0;
	}
	case MSG_PAINT:
	{
		int i ;
		int res;
		int a, b;
		SIZE size;
		HDC hdc;

		long long total_time;
		struct timeval start, end;
		gettimeofday(&start, NULL);

		gui_debug("=================MSG_PAINT_2==========================\n");
		hdc = BeginPaint(hwnd);
		for(i = 0; i < slidebox->widgetNum; i++){

			if(strlen(slidebox->widgets[i].text) > 0){
				SetTextColor(hdc, slidebox->textColor);
				SelectFont(hdc, slidebox->textFt);
				SetBkMode(hdc, BM_TRANSPARENT);
			}

			if(slidebox->widgets[i].bmp.bmBits){
				FillBoxWithBitmap(hdc, slidebox->widgets[i].bx, slidebox->widgets[i].by,
					slidebox->widgets[i].bmp.bmWidth, slidebox->widgets[i].bmp.bmHeight, &slidebox->widgets[i].bmp);
			}

			if(strlen(slidebox->widgets[i].text) > 0){
				DrawText(hdc, slidebox->widgets[i].text, -1, &slidebox->widgets[i].textRect,
						 DT_NOCLIP | DT_CENTER | DT_WORDBREAK);
			}
		}

		EndPaint(hwnd, hdc);

		gettimeofday(&end, NULL);
		total_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
		total_time /= 1000;
		gui_debug("time %lld ms\n", total_time);

		return 0;
	}

	case MSG_LBUTTONDOWN:
	{
		newX = LOWORD(lParam);
		newY = HIWORD(lParam);
		gui_debug("MSG_LBUTTONDOWN\n");
		gui_debug("newX=%d, newY=%d\n", newX, newY);

		slidebox->btDownFlag = 1;
		slidebox->moveFlag = 0;
		slidebox->oldX = newX;
		slidebox->moveGap = 0;
		slidebox->turnPoint = newX;
		slidebox->moveDirection = 0;

		slidebox->curWidgetID = getCurMiddleItemID(slidebox);	//record widget id before slide
		break;
	}
	case MSG_LBUTTONUP:
	{
		newX = LOWORD(lParam);
		newY = HIWORD(lParam);
		slidebox->btDownFlag = 0;
		int i;

		gui_debug("MSG_LBUTTONDOWN\n");
		gui_debug("newX=%d, newY=%d\n", newX, newY);

		if(slidebox->widgetNum == 0){
			break;
		}
		if(slidebox->moveFlag == 0){
			gui_debug("==== MSG_LBUTTONUP ====\n");
			for(i = 0; i < slidebox->widgetNum; i++){
				if(newY >= slidebox->widgets[i].by && newY <= slidebox->widgets[i].by+slidebox->widgets[i].bh){
					if(newX >= slidebox->widgets[i].bx && newX <= slidebox->widgets[i].bx+slidebox->widgets[i].bw){
						slidebox->widgets[i].fun(i, (DWORD)slidebox);
					}
				}
			}
		}else{
			int tmpNum = getCurMiddleItemID(slidebox);		//get widget id after slide
			if(abs(newX - slidebox->turnPoint) < SLIDETHRESHOLD){
				slidebox->moveDirection = 0;
			}
			switch(slidebox->moveDirection){
				case 0:
					slidebox->curWidgetID = tmpNum;
					break;
				case 1:		//1: i-- right slide ;
					if(slidebox->curWidgetID == tmpNum){	//short slide
						slidebox->curWidgetID = slidebox->curWidgetID > 0? slidebox->curWidgetID-1: slidebox->widgetNum-1;
					}else{									//long slide
						slidebox->curWidgetID = tmpNum;
					}
					break;
				case -1:	//-1: i++ left slide
					if(slidebox->curWidgetID == tmpNum){
						slidebox->curWidgetID = (slidebox->curWidgetID + 1) % slidebox->widgetNum;
					}else{
						slidebox->curWidgetID = tmpNum;
					}
					break;
			}
		SetTimer(hwnd, SLIDEBOXTIMERID, 1);
		}
		break;
	}
	case MSG_TIMER: {
		gui_debug("** MSG_TIMER CUR ID: [%d] **\n",slidebox->curWidgetID);
		int tmpMove = 0;
		if(slidebox->widgetNum == 0){
			KillTimer(hwnd, SLIDEBOXTIMERID);
			break;
		}
		int tmpGap = slidebox->widgets[slidebox->curWidgetID].bx +
			slidebox->widgets[slidebox->curWidgetID].bw/2 - slidebox->w/2;
		gui_debug("X: [%d], W/2: [%d], MID: [%d]\n", slidebox->widgets[slidebox->curWidgetID].bx,
			slidebox->widgets[slidebox->curWidgetID].bw/2, slidebox->w/2);
		if(abs(tmpGap) < AUTOMOVESPEED){
			tmpMove = tmpGap;
		}else{
			tmpMove = tmpGap > 0? AUTOMOVESPEED : -AUTOMOVESPEED;
		}
		gui_debug("cur gap: [%d], Move: [%d]\n", tmpGap, tmpMove);

		if(tmpGap != 0){
			for(i = 0; i < slidebox->widgetNum; i++){
				slidebox->widgets[i].bx -= tmpMove;
				slidebox->widgets[i].textRect.left -= tmpMove;
				slidebox->widgets[i].textRect.right -= tmpMove;
			}
			slideBoxCycleShow(slidebox);
			InvalidateRect(hwnd, NULL, TRUE);

			KillTimer(hwnd, SLIDEBOXTIMERID);
		}else{
			KillTimer(hwnd, SLIDEBOXTIMERID);
		}
		break;
	}
	case MSG_MOUSEMOVE:
	{
		newX = LOWORD(lParam);
		newY = HIWORD(lParam);
		int tmpDirection = 0;

		if(slidebox->btDownFlag == 1){
			gui_debug("MSG_MOUSEMOVE\n");
			slidebox->moveFlag = 1;
			moveTimes++;

			slidebox->moveGap += newX - slidebox->oldX;
			if(abs(slidebox->moveGap) < 2){
				break;
			}
			tmpDirection = slidebox->moveGap > 0? 1: -1;	//1: i--; -1: i++
			gui_debug("tmpDirection: [%d], moveGap: [%d], moveDirection: [%d]\n", tmpDirection, slidebox->moveGap, slidebox->moveDirection);
			if(tmpDirection != slidebox->moveDirection){
				slidebox->turnPoint = slidebox->oldX;
				slidebox->moveDirection = tmpDirection;
			}
			slidebox->oldX = newX;

			if(moveTimes%slidebox->moveSenstive == 0){
				for(i = 0; i < slidebox->widgetNum; i++){
					slidebox->widgets[i].bx += slidebox->moveGap;
					slidebox->widgets[i].textRect.left += slidebox->moveGap;
					slidebox->widgets[i].textRect.right += slidebox->moveGap;
				}
				slideBoxCycleShow(slidebox);
				slidebox->moveGap = 0;
				InvalidateRect(hwnd, NULL, TRUE);
			}

		}
		break;
	}

	default:
		break;
	}

	return DefaultControlProc(hwnd, message, wParam, lParam);
}


BOOL RegisterslideBoxView(void)
{
	WNDCLASS MyClass;
	MyClass.spClassName = SLIDEBOX_VIEW;
	MyClass.dwStyle = WS_NONE;
	MyClass.dwExStyle = WS_EX_NONE;
	MyClass.hCursor = 0;
	MyClass.iBkColor = PIXEL_lightgray;
	MyClass.WinProc = slideBoxViewProc;

	return RegisterWindowClass(&MyClass);
}

void UnregisterslideBoxView(void)
{
	UnregisterWindowClass(SLIDEBOX_VIEW);
}

void slideBoxViewDeinit(slideBoxView *slideBox)
{
	if(slideBox != NULL){
		if(slideBox->widgets != NULL){
			int i;
			for(i = 0; i < slideBox->widgetNum; i++){
				if(NULL != &slideBox->widgets[i].bmp){
					unloadBitMap(&slideBox->widgets[i].bmp);
				}
			}
			free(slideBox->widgets);
			slideBox->widgets = NULL;
		}
		if(NULL != &slideBox->winbg){
			unloadBitMap(&slideBox->winbg);
		}
		free(slideBox);
		//slideBox = NULL;
	}
}

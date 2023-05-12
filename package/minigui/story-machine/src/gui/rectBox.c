#include "rectBox.h"
#include "middle_inc.h"

void RectBoxDataUninit(RectBox *rectBox)
{
	int i;
	if(rectBox)
	{
		for(i=0; i<rectBox->nBtnNum; i++)
		{
			if(rectBox->pBtn[i].onBmp.bmBits)
			{
				unloadBitMap(&rectBox->pBtn[i].onBmp);
			}
			if(rectBox->pBtn[i].offBmp.bmBits)
			{
				unloadBitMap(&rectBox->pBtn[i].offBmp);
			}
		}
		if(rectBox->pBtn)
		{
			free(rectBox->pBtn);
			rectBox->pBtn = NULL;
		}

		for(i=0; i<rectBox->nBmpNum; i++)
		{
			if(rectBox->pBmp[i].sBmp.bmBits)
			{
				unloadBitMap(&rectBox->pBmp[i].sBmp);
			}
		}
		if(rectBox->pBmp)
		{
			free(rectBox->pBmp);
			rectBox->pBmp = NULL;
		}

		if(rectBox->pText)
		{
			free(rectBox->pText);
			rectBox->pText = NULL;
		}

		if(rectBox->sBg.sBmp.bmBits)
		{
			unloadBitMap(&rectBox->sBg.sBmp);
		}

		free(rectBox);
		//rectBox = NULL;
	}
}

static int RectBoxProc(HWND hwnd, int message, WPARAM wParam,LPARAM lParam)
{
	int i;
	int newX = 0;
	int newY = 0;
	RectBox *rectBox = (RectBox*)GetWindowAdditionalData(hwnd);
	switch (message)
	{
		case MSG_LBUTTONDOWN:
		{
			newX = LOWORD(lParam);
			newY = HIWORD(lParam);
			rectBox->oldY = newY;
			rectBox->oldX = newX;
			rectBox->moveFlag = 0;
			rectBox->moveNum = 0;
			rectBox->moveGapX = 0;
			rectBox->moveGapY = 0;
			if(rectBox->mvFun)
				rectBox->mvFun(message, rectBox);
			gui_debug("MSG_LBUTTONDOWN\n");
			gui_debug("newX=%d, newY=%d\n", newX, newY);
			break;
		}

		case MSG_LBUTTONUP:
		{
			newX = LOWORD(lParam);
			newY = HIWORD(lParam);
			gui_debug("MSG_LBUTTONUP\n");
			gui_debug("newX=%d, newY=%d\n", newX, newY);

			if(rectBox->mvFun)
				rectBox->mvFun(message, rectBox);
			rectBox->moveFlag = 0;
			for(i=0; i<rectBox->nBtnNum; i++)
			{
				if( (newX >= rectBox->pBtn[i].sRect.x) && (newX <= (rectBox->pBtn[i].sRect.x+rectBox->pBtn[i].sRect.w))
				 && (newY >= rectBox->pBtn[i].sRect.y) && (newY <= (rectBox->pBtn[i].sRect.y+rectBox->pBtn[i].sRect.h)))
				{
					if(rectBox->pBtn[i].fun)
					{
						gui_debug("active i=%d\n", i);
						if(rectBox->pBtn[i].onStus == 1)
						{
							rectBox->pBtn[i].onStus = 0;
						}
						else if(rectBox->pBtn[i].onStus == 0)
						{
							rectBox->pBtn[i].onStus = 1;
						}
						rectBox->pBtn[i].fun(i, (DWORD)rectBox);
					}
				}
			}

			InvalidateRect(hwnd, NULL, TRUE);

			break;
		}

		case MSG_PAINT:
		{
			HDC hdc = BeginPaint(hwnd);
			//gui_debug("MSG_PAINT\n");


			// draw hbar background
			if(rectBox->sBg.sBmp.bmBits)
			{
				FillBoxWithBitmap(hdc, rectBox->sBg.sRect.x, rectBox->sBg.sRect.y,
					rectBox->sBg.sRect.w, rectBox->sBg.sRect.h,  &rectBox->sBg.sBmp);
			}

			for(i=0; i<rectBox->nBmpNum; i++)
			{
				if(rectBox->pBmp[i].sBmp.bmBits)
				{
					FillBoxWithBitmap(hdc, rectBox->pBmp[i].sRect.x, rectBox->pBmp[i].sRect.y,
						rectBox->pBmp[i].sRect.w, rectBox->pBmp[i].sRect.h,  &rectBox->pBmp[i].sBmp);
				}
			}

			for(i=0; i<rectBox->nTextNum; i++)
			{
				if(rectBox->pText[i].pText)
				{
					SetBkMode(hdc, BM_TRANSPARENT);
					SetTextColor(hdc, rectBox->pText[i].sColor);
					SelectFont(hdc, rectBox->pText[i].sFont);
					TextOut(hdc, rectBox->pText[i].sRect.x, rectBox->pText[i].sRect.y, rectBox->pText[i].pText);
				}
			}

			for(i=0; i<rectBox->nBtnNum; i++)
			{
				if(rectBox->pBtn)
				{
					if(rectBox->pBtn[i].onStus==1 && rectBox->pBtn[i].onBmp.bmBits)
					{
						FillBoxWithBitmap(hdc, rectBox->pBtn[i].sRect.x, rectBox->pBtn[i].sRect.y, rectBox->pBtn[i].sRect.w,
						rectBox->pBtn[i].sRect.h,  &rectBox->pBtn[i].onBmp);
					}
					else if(rectBox->pBtn[i].onStus==0 && rectBox->pBtn[i].offBmp.bmBits)
					{
						FillBoxWithBitmap(hdc, rectBox->pBtn[i].sRect.x, rectBox->pBtn[i].sRect.y, rectBox->pBtn[i].sRect.w,
						rectBox->pBtn[i].sRect.h,  &rectBox->pBtn[i].offBmp);
					}
				}
			}

			EndPaint(hwnd, hdc);
			return 0;
		}
		case MSG_MOUSEMOVE:
			newX = LOWORD(lParam);
			newY = HIWORD(lParam);
			if(newY > (rectBox->sRect.h - 60)){
				break;
			}
			rectBox->moveNum++;
			if(rectBox->moveNum > 2){
				rectBox->moveNum = 0;
				gui_debug("MSG_MOUSEMOVE %d %d %d %d \n",newX,newY,rectBox->oldX,rectBox->oldY);
				rectBox->moveGapX = newX - rectBox->oldX;
				rectBox->moveGapY = newY - rectBox->oldY;
				rectBox->moveFlag = 1;
				if(rectBox->mvFun)
					rectBox->mvFun(message, rectBox);
				rectBox->oldY = newY;
				rectBox->oldX = newX;
			}

			break;
		default:
			break;
	}

	return DefaultControlProc(hwnd, message, wParam, lParam);
}

BOOL RegisterRectBox(void)
{
	WNDCLASS MyClass;
	MyClass.spClassName = RECTBOX;
	MyClass.dwStyle = WS_NONE;
	MyClass.dwExStyle = WS_EX_NONE;
	MyClass.hCursor = 0;
	MyClass.iBkColor = PIXEL_lightgray;
	MyClass.WinProc = RectBoxProc;

	return RegisterWindowClass(&MyClass);
}

void UnregisterRectBox(void)
{
	UnregisterWindowClass(RECTBOX);
}

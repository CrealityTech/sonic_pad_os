#include "headBarA.h"
#include "middle_inc.h"

static int HeadBarAProc(HWND hwnd, int message, WPARAM wParam,LPARAM lParam)
{
	int i;
	int newX = 0;
	int newY = 0;
	HeadBarA *pHbar = (HeadBarA*)GetWindowAdditionalData(hwnd);
	switch (message)
	{
		case MSG_LBUTTONDOWN:
		{
			newX = LOWORD(lParam);
			newY = HIWORD(lParam);
			gui_debug("MSG_LBUTTONDOWN\n");
			gui_debug("newX=%d, newY=%d\n", newX, newY);

			for(i=0; i<pHbar->nBtnNum; i++)
			{
				if( (newX>pHbar->pBtn[i].sRect.x) && (newX<(pHbar->pBtn[i].sRect.x+pHbar->pBtn[i].sRect.w))
				 && (newY>pHbar->pBtn[i].sRect.y) && (newY<(pHbar->pBtn[i].sRect.y+pHbar->pBtn[i].sRect.h)))
				{
					pHbar->pBtn[i].onStus = 1;
				}
			}
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}

		case MSG_LBUTTONUP:
		{
			newX = LOWORD(lParam);
			newY = HIWORD(lParam);
			gui_debug("MSG_LBUTTONUP\n");
			gui_debug("newX=%d, newY=%d\n", newX, newY);

			for(i=0; i<pHbar->nBtnNum; i++)
			{
				if( (newX >= pHbar->pBtn[i].sRect.x) && (newX <= (pHbar->pBtn[i].sRect.x+pHbar->pBtn[i].sRect.w))
				 && (newY >= pHbar->pBtn[i].sRect.y) && (newY <= (pHbar->pBtn[i].sRect.y+pHbar->pBtn[i].sRect.h)))
				{
					if(pHbar->pBtn[i].fun)
					{
						gui_debug("active i=%d\n", i);
						pHbar->pBtn[i].fun(i, (DWORD)pHbar);
					}
				}
			}

			for(i=0; i<pHbar->nBtnNum; i++)
			{
				pHbar->pBtn[i].onStus = 0;
			}
			InvalidateRect(hwnd, NULL, TRUE);

			break;
		}

		case MSG_PAINT:
		{
			HDC hdc = BeginPaint(hwnd);
			gui_debug("MSG_PAINT\n");

			// draw hbar background
			if(pHbar == NULL){

				gui_warn("pHbar == NULL\n");
				return 0;
			}
			if(pHbar->sBg.sBmp.bmBits)
			{
				FillBoxWithBitmap(hdc, pHbar->sBg.sRect.x, pHbar->sBg.sRect.y,
					pHbar->sBg.sRect.w, pHbar->sBg.sRect.h,  &pHbar->sBg.sBmp);
			}
			for(i=0; i<pHbar->nBmpNum; i++)
			{
				if(pHbar->pBmp[i].sBmp.bmBits)
				{
					FillBoxWithBitmap(hdc, pHbar->pBmp[i].sRect.x, pHbar->pBmp[i].sRect.y,
						pHbar->pBmp[i].sRect.w, pHbar->pBmp[i].sRect.h,  &pHbar->pBmp[i].sBmp);
				}
			}
			for(i=0; i<pHbar->nTextNum; i++)
			{
				if(pHbar->pText[i].pText)
				{

					SetBkMode(hdc, BM_TRANSPARENT);
					SetTextColor(hdc, pHbar->pText[i].sColor);
					SelectFont(hdc, pHbar->pText[i].sFont);
					TextOut(hdc, pHbar->pText[i].sRect.x, pHbar->pText[i].sRect.y, pHbar->pText[i].pText);
				}
			}
			for(i=0; i<pHbar->nBtnNum; i++)
			{
				if(pHbar->pBtn)
				{
					if(pHbar->pBtn[i].onStus==1 && pHbar->pBtn[i].onBmp.bmBits)
					{
						FillBoxWithBitmap(hdc, pHbar->pBtn[i].sRect.x, pHbar->pBtn[i].sRect.y, pHbar->pBtn[i].sRect.w,
						pHbar->pBtn[i].sRect.h,  &pHbar->pBtn[i].onBmp);
					}
					else if(pHbar->pBtn[i].onStus==0 && pHbar->pBtn[i].offBmp.bmBits)
					{
						FillBoxWithBitmap(hdc, pHbar->pBtn[i].sRect.x, pHbar->pBtn[i].sRect.y, pHbar->pBtn[i].sRect.w,
						pHbar->pBtn[i].sRect.h,  &pHbar->pBtn[i].offBmp);
					}
				}
			}

			EndPaint(hwnd, hdc);
			return 0;
		}
		default:
			break;
	}

	return DefaultControlProc(hwnd, message, wParam, lParam);
}

void HeadBarADataUninit(HeadBarA *pHbar)
{
	int i;
	if(pHbar)
	{
		for(i=0; i<pHbar->nBtnNum; i++)
		{
			if(pHbar->pBtn[i].onBmp.bmBits)
			{
				unloadBitMap(&pHbar->pBtn[i].onBmp);
			}
			if(pHbar->pBtn[i].offBmp.bmBits)
			{
				unloadBitMap(&pHbar->pBtn[i].offBmp);
			}
		}
		if(pHbar->pBtn)
		{
			free(pHbar->pBtn);
			pHbar->pBtn = NULL;
		}

		for(i=0; i<pHbar->nBmpNum; i++)
		{
			if(pHbar->pBmp[i].sBmp.bmBits)
			{
				unloadBitMap(&pHbar->pBmp[i].sBmp);
			}
		}
		if(pHbar->pBmp)
		{
			free(pHbar->pBmp);
			pHbar->pBmp = NULL;
		}

		if(pHbar->pText)
		{
			free(pHbar->pText);
			pHbar->pText = NULL;
		}

		if(pHbar->sBg.sBmp.bmBits)
		{
			unloadBitMap(&pHbar->sBg.sBmp);
		}

		free(pHbar);
	}
}


BOOL RegisterHeadBarA(void)
{
	WNDCLASS MyClass;
	MyClass.spClassName = HEADBARA;
	MyClass.dwStyle = WS_NONE;
	MyClass.dwExStyle = WS_EX_NONE;
	MyClass.hCursor = 0;
	MyClass.iBkColor = PIXEL_lightgray;
	MyClass.WinProc = HeadBarAProc;

	return RegisterWindowClass(&MyClass);
}

void UnregisterHeadBarA(void)
{
	UnregisterWindowClass(HEADBARA);
}

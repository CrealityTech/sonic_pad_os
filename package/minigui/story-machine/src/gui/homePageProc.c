#include "homePageProc.h"
#include "winLinklist.h"
#include "slideBoxView.h"
#include "middle_inc.h"
#include "headBarA.h"
#include "rectBox.h"
#include "winMonitor.h"

#define USER_TIMER_ID	101

winNode homepageNode =
{
	HOMEPAGE,
	0,
	homepageProcStart,
	NULL,
	NULL,
};

static void ButtonCallbackFun(int index, DWORD param)
{
	HeadBarA *pHbar = (HeadBarA*)param;

	gui_debug("index=%d, sRect.w=%d\n", index, pHbar->sBg.sRect.w);
}

static void slideBoxItemFunction(int index, DWORD param)
{
	gui_debug("ID: [%d]\n", index);
	switch (index)
	{
		case 0:
			SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)HOMEPAGE, (LPARAM)VIDEOLIST);
			break;
		case 1:
			SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)HOMEPAGE, (LPARAM)MUSICLIST);
			break;
		case 2:
			SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)HOMEPAGE, (LPARAM)PHOTOLIST);
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)HOMEPAGE, (LPARAM)SETTINGLIST);
			break;
		case 6:
			break;
		case 7:
			break;
	}
}

static int mainWindowProc(HWND hwnd, int nMessage, WPARAM wParam, LPARAM lParam)
{
	HeadBarA *pHbar = NULL;
	slideBoxView *slidebox = NULL;
	HDC hdc;

	switch(nMessage)
	{
		case MSG_CREATE:
			gui_debug("MSG_CREATE\n");
			pHbar = HeadBarAFunctionSet(ButtonCallbackFun);
			if(pHbar)
			{
				gui_debug("MSG_TIMER %p\n", pHbar);
				CreateWindowEx(HEADBARA, "",
				WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT,
				1, pHbar->sRect.x, pHbar->sRect.y, pHbar->sRect.w, pHbar->sRect.h,
				hwnd, (DWORD)pHbar);
			}
			//rectBox = RectBoxDataInit(rectBox);
			slidebox = slideBoxItemFunctionSet(slideBoxItemFunction);

			if(slidebox)
			{
				slidebox->hwnd = CreateWindowEx(SLIDEBOX_VIEW, "",
				WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT, 0, slidebox->x, slidebox->y,
				slidebox->w, slidebox->h, hwnd, (DWORD)slidebox);
			}

			SetTimer(hwnd, USER_TIMER_ID, 3);		// 10ms
			break;

		case MSG_TIMER: {
			gui_debug("MSG_TIMER\n");
			KillTimer(hwnd, USER_TIMER_ID);

			#if 0
			if(rectBox)
			{
				CreateWindowEx(RECTBOX, "",
				WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT,
				1, rectBox->sRect.x , rectBox->sRect.y, rectBox->sRect.w, rectBox->sRect.h,
				hwnd, (DWORD)rectBox);
			}
			#endif

			tplayer_pause();
			tplayer_stop();

			break;
		}
		case MSG_ERASEBKGND: {
			int newX = LOWORD(lParam);
			int newY = HIWORD(lParam);
			gui_debug("MSG_ERASEBKGND\n");

			hdc = (HDC) wParam;
			//hdc = BeginPaint(hwnd);
			if(winbackward.bmBits)
			{
				FillBoxWithBitmap(hdc, 0, 0, 800, 480, &winbackward);
			}

			return 0;
		}

		case MSG_LBUTTONDOWN: {
			gui_debug("MSG_LBUTTONDOWN\n");
			break;
		}

		case MSG_LBUTTONUP: {
			gui_debug("MSG_LBUTTONUP\n");
			break;
		}

		case MSG_DESTROY:
			gui_debug("MSG_DESTROY\n");
			break;
		case MSG_CLOSE:
			gui_debug("MSG_CLOSE\n");
			DestroyAllControls(hwnd);
			DestroyMainWindow(hwnd);
			break;
	}

	return DefaultMainWinProc(hwnd, nMessage, wParam, lParam);
}

int homepageProcStart(HWND parent, void *data)
{
	MSG Msg;
	HWND hMainWnd;
	MAINWINCREATE CreateInfo;

	CreateInfo.dwStyle = WS_NONE;		//WS_CAPTION | WS_BORDER | WS_VISIBLE;
	CreateInfo.dwExStyle = WS_EX_AUTOSECONDARYDC;
	CreateInfo.spCaption = "";
	CreateInfo.hMenu = 0;
	CreateInfo.hCursor =  GetSystemCursor(IDC_ARROW);
	CreateInfo.hIcon = 0;
	CreateInfo.MainWindowProc = mainWindowProc;
	CreateInfo.lx = 0;
	CreateInfo.ty = 0;
	CreateInfo.rx = g_rcScr.right;
	CreateInfo.by = g_rcScr.bottom;
	CreateInfo.iBkColor = PIXEL_lightgray;
	CreateInfo.dwAddData = 0;
	CreateInfo.dwReserved = 0;
	CreateInfo.hHosting = parent;//HWND_DESKTOP;
	ShowCursor(0);	// hide the cursor

	gui_debug("the home win create\n");
	hMainWnd = CreateMainWindow (&CreateInfo);
	if (hMainWnd == HWND_INVALID)
	{
		return -1;
	}
	ShowWindow (hMainWnd, SW_SHOWNORMAL);
	homepageNode.owner = hMainWnd;
	gui_debug("the home win create finish\n");

	while (GetMessage(&Msg, hMainWnd)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	gui_debug("the home win create end\n");

	MainWindowThreadCleanup (hMainWnd);
}

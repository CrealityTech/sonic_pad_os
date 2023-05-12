#include "photo_list.h"
#include "listbox_view.h"
#include "winLinklist.h"
#include "headBar.h"
#include "headBarA.h"
#include "read_file_list.h"
#include "winMonitor.h"
#include "rectBox.h"

typedef struct photobg_t
{
	BITMAP last;
	BITMAP curr;
	BITMAP next;
	int finished;
}photobg_t;

photobg_t photobg;

static RectBox *RectBoxView;
static ListboxView *listboxView = NULL;
static HeadBarA *pHbar = NULL;
extern int PhotoFileNum;

int photoPlayListStart(HWND parent, void *data);

winNode photoPlayListNode =
{
	PHOTOPLAYLIST,
	0,
	photoPlayListStart,
	NULL,
	NULL,
};

static void ButtonCallbackFun(int index, DWORD param)
{
	HeadBarA *pHbar = (HeadBarA*)param;

	gui_debug("index=%d, sRect.w=%d\n", index, pHbar->sBg.sRect.w);

	SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)PHOTOPLAYLIST, (LPARAM)HOMEPAGE);
}

static int recBoxFun(int index, DWORD param)
{
	SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)PHOTOPLAYLIST, (LPARAM)PHOTOLIST);
}

static RectBox *RectBoxDataInit(RectBox *rectBox)
{
	int err_code;

	rectBox = (RectBox*) malloc(sizeof(RectBox));
	if(rectBox == NULL){
		gui_error("HeadBarA malloc error\n");
		return NULL;
	}
	memset(rectBox, 0, sizeof(RectBox));

	rectBox->sRect.x = 10;
	rectBox->sRect.y = 70;
	rectBox->sRect.w = 60;
	rectBox->sRect.h = 60;

	rectBox->nBtnNum = 1;
	rectBox->pBtn = (ButtonType *) malloc(sizeof(ButtonType) * rectBox->nBtnNum);
	if(rectBox->pBtn == NULL){
		gui_error("malloc failed\n");
		goto ERR;
	}
	memset(rectBox->pBtn, 0, (sizeof(ButtonType) * rectBox->nBtnNum));

	rectBox->pBtn[0].onStus = 0;
	rectBox->pBtn[0].fun = recBoxFun;
	rectBox->pBtn[0].sRect.x = 0;
	rectBox->pBtn[0].sRect.y = 0;
	rectBox->pBtn[0].sRect.w = 54;
	rectBox->pBtn[0].sRect.h = 54;

	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[0].onBmp, PHOTO_PATH"back.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[0].offBmp, PHOTO_PATH"back.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}

	return rectBox;

ERR:
	RectBoxDataUninit(rectBox);
	return NULL;
}

int photo_prev_load(int NextLast, BITMAP *loadmap)
{
	int index;
	int err_code = -1;

	index = select_index + NextLast;
	index = index >= PhotoFileNum ? 0 : index;
	index = index < 0 ? PhotoFileNum - 1 : index;
	gui_debug("select_index = %d, index =%d NextLast %d\n",select_index ,index,NextLast );
	PlayerFilePath = mediaFilePath[index];

	err_code = LoadBitmapFromFile(HDC_SCREEN, loadmap, PlayerFilePath);

	return err_code;
}

void photoPlayer(HDC hdc, int NextLast)
{
	int err_code;


	if(NextLast != 0){
		goto PREV_LOAD;
	}

	if(PlayerFilePath == NULL){
		gui_warn("PlayerFilePath don't found\n");
		goto END;
	}

	err_code = photo_prev_load(0, &photobg.curr);//load last photo
	if (err_code != ERR_BMP_OK){
		gui_warn("load bitmap failed\n");
		goto END;
	}

	if(photobg.curr.bmBits){
		FillBoxWithBitmap(hdc, 100, 100, 600, 280, &photobg.curr);
	}
	goto END;

PREV_LOAD:

	if(NextLast == 1){
		unloadBitMap(&photobg.last);
		photobg.last = photobg.curr;
		photobg.curr = photobg.next;
	}else{
		unloadBitMap(&photobg.next);
		photobg.next = photobg.curr;
		photobg.curr = photobg.last;
	}
	if(winbackward.bmBits){
		FillBoxWithBitmap(hdc, 0, 0, 800, 480, &winbackward);
	}
	if(photobg.curr.bmBits){
		FillBoxWithBitmap(hdc, 100, 100, 600, 280, &photobg.curr);
	}
END:
	return;
}

int GetPhotoLastNextIndex(HDC hdc, int NextLast)
{

	select_index += NextLast;

	if(select_index >= PhotoFileNum)
		select_index = 0;
	if(select_index < 0){
		select_index = PhotoFileNum - 1;
	}

	gui_debug("index=%d PhotoFileNum= %d\n", select_index, PhotoFileNum);

	PlayerFilePath = mediaFilePath[select_index];

	if(PlayerFilePath == NULL){
		gui_warn("PlayerFilePath don't found\n");
		return -1;
	}

	photoPlayer(hdc, NextLast);

	return 0;
}

static int photoPlayListStartProc(HWND hwnd, int nMessage, WPARAM wParam, LPARAM lParam)
{
	int err_code;
	int newX = LOWORD(lParam);
	int newY = HIWORD(lParam);
	static int newX_old = 0;
	static int newY_old = 0;
	static int nextLast = 0;
	switch(nMessage)
	{
		case MSG_CREATE:
		{
			gui_debug("MSG_CREATE\n");

			pHbar = HeadBarAFunctionSet(ButtonCallbackFun);

			pHbar->hwnd = CreateWindowEx(HEADBARA, "",
			WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT,
			1, pHbar->sRect.x, pHbar->sRect.y, pHbar->sRect.w, pHbar->sRect.h,
			hwnd, (DWORD)pHbar);
			nextLast = 0;
			RectBoxView = RectBoxDataInit(RectBoxView);
			RectBoxView->hwnd = CreateWindowEx(RECTBOX, "",
			WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT,
			4, RectBoxView->sRect.x , RectBoxView->sRect.y, RectBoxView->sRect.w, RectBoxView->sRect.h,
			hwnd, (DWORD)RectBoxView);

			SetTimer(hwnd, USER_TIMER_ID, 1);
			return 0;
		}
		case MSG_TIMER:
		{
			gui_debug("MSG_TIMER\n");
			KillTimer(hwnd, USER_TIMER_ID);
			if(nextLast == 0){
				err_code = photo_prev_load(-1, &photobg.last);//load last photo
				if (err_code != ERR_BMP_OK){
					gui_warn("load bitmap failed\n");
				}
				err_code = photo_prev_load(1, &photobg.next);//load next photo
				if (err_code != ERR_BMP_OK){
					gui_warn("load bitmap failed\n");
				}
			}else{

				if(nextLast == 1){
					err_code = photo_prev_load(nextLast, &photobg.next);
				}else{
					err_code = photo_prev_load(nextLast, &photobg.last);
				}
				if (err_code != ERR_BMP_OK){
					gui_warn("load bitmap failed\n");
				}
			}
			photobg.finished = 1;
			gui_debug("photobg.finished = %d\n", photobg.finished);
			return 0;
		}
		case MSG_ERASEBKGND: {
			int newX = LOWORD(lParam);
			int newY = HIWORD(lParam);
			gui_debug("MSG_ERASEBKGND\n");
			HDC hdc = (HDC) wParam;
			if(winbackward.bmBits){
				FillBoxWithBitmap(hdc, 0, 0, 800, 480, &winbackward);
			}
			photoPlayer(hdc, 0);
			return 0;
		}
		case MSG_LBUTTONDOWN:
		{
			newX_old = newX;
			newY_old = newY;
			break;
		}
		case MSG_LBUTTONUP:
		{
			gui_debug("MSG_LBUTTONUP newX = %d newX_old = %d\n", newX, newX_old);

			HDC hdc = (HDC) wParam;
			if(newY < 100 || newX < 100 || newX > 700){
				break;
			}
			//wait photo load finished
			if(!photobg.finished){
				return 0;
			}

			if((newX - newX_old) > 3){
				nextLast = 1;
				GetPhotoLastNextIndex(hdc, nextLast);
				SetTimer(hwnd, USER_TIMER_ID, 1);
				photobg.finished = 0;
			}else if((newX - newX_old) < 3){
				nextLast = -1;
				GetPhotoLastNextIndex(hdc, nextLast);
				photobg.finished = 0;
				SetTimer(hwnd, USER_TIMER_ID, 1);
			}

			return 0;
		}
		case MSG_DESTROY:
			gui_debug("MSG_DESTROY\n");
			break;
		case MSG_CLOSE:
		{
			gui_debug("MSG_CLOSE\n");
			RectBoxDataUninit(RectBoxView);
			unloadBitMap(&photobg.last);
			unloadBitMap(&photobg.curr);
			unloadBitMap(&photobg.next);
			DestroyAllControls(hwnd);
			DestroyMainWindow(hwnd);
			break;
		}
	}

	return DefaultMainWinProc(hwnd, nMessage, wParam, lParam);
}

int photoPlayListStart(HWND parent, void *data)
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
	CreateInfo.MainWindowProc = photoPlayListStartProc;
	CreateInfo.lx = 0;
	CreateInfo.ty = 0;
	CreateInfo.rx = g_rcScr.right;
	CreateInfo.by = g_rcScr.bottom;
	CreateInfo.iBkColor = PIXEL_lightgray;
	CreateInfo.dwAddData = 0;
	CreateInfo.dwReserved = 0;
	CreateInfo.hHosting = parent;
	ShowCursor(0);	// hide the cursor

	gui_debug("win create\n");
	hMainWnd = CreateMainWindow (&CreateInfo);
	if (hMainWnd == HWND_INVALID)
	{
		return -1;
	}
	ShowWindow (hMainWnd, SW_SHOWNORMAL);
	photoPlayListNode.owner = hMainWnd;
	gui_debug("win finish\n");

	while (GetMessage(&Msg, hMainWnd)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	gui_debug("win end\n");
	MainWindowThreadCleanup (hMainWnd);
}

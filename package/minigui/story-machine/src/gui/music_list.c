#include "music_list.h"
#include "listbox_view.h"
#include "winLinklist.h"
#include "headBar.h"
#include "headBarA.h"
#include "read_file_list.h"
#include "func_uevent.h"
#include "rectBox.h"
#include "winMonitor.h"

static ListboxView *listboxView = NULL;
static headBarView *headbarView = NULL;
static HeadBarA *pHbar = NULL;
static RectBox *RectBoxView;

static int select_index;
static int FileNum;

int MusicListStart(HWND parent, void *data);

winNode musicListNode =
{
	MUSICLIST,
	0,
	MusicListStart,
	NULL,
	NULL,
};

static void ButtonItemFunction(int index, DWORD param)
{
	if(HEADBAR_RIGHT == index){
		gui_debug("head bar start\n");
	}else{
		gui_debug("HEADBAR_LEFT\n");
		SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)MUSICLIST, (LPARAM)HOMEPAGE);
	}
}

static void ButtonCallbackFun(int index, DWORD param)
{
	HeadBarA *pHbar = (HeadBarA*)param;

	gui_debug("index=%d, sRect.w=%d\n", index, pHbar->sBg.sRect.w);
	tplayer_pause();
	PlayerFilePath = NULL;
	SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)MUSICLIST, (LPARAM)HOMEPAGE);
}

static int TplayAudio(int index, DWORD param)
{
	ListboxView *boxview = (ListboxView*)param;
	if(index < 0){
		return 0;
	}
	PlayerFilePath = mediaFilePath[index];
	PlayerFileName = mediaFileName[index];
	select_index = index;
	SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)MUSICLIST, (LPARAM)MPLAYERLIST);
	return 0;
}

int GetMusicNextLastIndex(int NextLast)
{
	select_index += NextLast;
	if(select_index >= FileNum)
		select_index = 0;
	if(select_index < 0){
		select_index = FileNum - 1;
	}
	gui_debug("index=%d FileNum= %d\n", select_index, FileNum);

	PlayerFilePath = mediaFilePath[select_index];
	PlayerFileName = mediaFileName[select_index];

	tplayer_pause();
	tplayer_stop();
	tplayer_play_url(PlayerFilePath);
	if(tplayer_play() < 0){
		return -1;
	}
	return 0;
}

static ListboxView* ListboxDataInit(ListboxView *listboxView)
{
	int i;
	int err_code;
	char pBuf[NAME_MAX_SIZE];

	FileNum = getFilePath(AUDIO_FILE_TYPE);
	if(FileNum <= 0){
		return NULL;
	}

	listboxView = (ListboxView*) malloc(sizeof(ListboxView));
	if (NULL == listboxView) {
		gui_warn("malloc ListboxView data error\n");
		return NULL;
	}
	memset((void *) listboxView, 0, sizeof(ListboxView));

#if 0

	err_code = LoadBitmapFromFile(HDC_SCREEN, &listboxView->ltbBg, PHOTO_PATH"music_item.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_warn("load %s bitmap failed\n", PHOTO_PATH"music_item.png");
	}
#endif

	listboxView->itType = ITEM_NONE;
	listboxView->lTextColor = COLOR_lightwhite;
	listboxView->rTextColor = COLOR_lightwhite;

	listboxView->x = 70;
	listboxView->y = 60;
	listboxView->ht = 800-200;
	listboxView->vt = 480-70;

	listboxView->ix = 500;
	listboxView->hfp = 90;

	listboxView->iy = 40;
	listboxView->vgap = 10;
	listboxView->itSz = FileNum;
	listboxView->Index = 0;
	listboxView->showSz = FileNum < 8 ? FileNum : 8;
	listboxView->lthGap = 10;
	listboxView->moveGap = 0;

	gui_debug("music file num %d %d\n", listboxView->itSz,listboxView->showSz );

	listboxView->itDt = (ItemData*) malloc(sizeof(ItemData)*listboxView->itSz);
	if(listboxView->itDt == NULL){
		goto ERR;
	}
	memset((void *) listboxView->itDt, 0, sizeof(ItemData)*listboxView->itSz);
	for(i=0; i<listboxView->itSz; i++)
	{
		listboxView->itDt[i].rType = RITEM_LTEXT;
		listboxView->itDt[i].rType |= RITEM_BUTTON;
		listboxView->itDt[i].btn = (ButtonData *)calloc(1, sizeof(ButtonData));
		listboxView->itDt[i].btn->fun = TplayAudio;
	}

	for(i = 0; i < listboxView->itSz; i++){
		char FileName[512];
		sprintf(FileName, "%d.%s",i+1, mediaFileName[i]);
		memcpy(listboxView->itDt[i].lT, FileName, strlen(FileName));
		if(mediaFileLang[i] == ENCODE_GB2312){
			gui_debug("music file num %d %d \n", listboxView->itSz, ENCODE_GB2312);
			listboxView->lTextFt[i] = GetLogFont(ID_FONT_SIMSUN_GB_25);
			listboxView->rTextFt[i] = GetLogFont(ID_FONT_SIMSUN_GB_25);
		}else{
			listboxView->lTextFt[i] = GetLogFont(ID_FONT_SIMSUN_25);
			listboxView->rTextFt[i] = GetLogFont(ID_FONT_SIMSUN_25);
		}
	}

	return listboxView;
ERR:
ListBoxDataDeinit(listboxView);
return NULL;

}

static int recBoxFun(int index, DWORD param)
{
	if(!PlayerFilePath){
		PlayerFilePath = mediaFilePath[0];
		PlayerFileName = mediaFileName[0];
	}
	if(PlayerFilePath[0] != 0)
		SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)MUSICLIST, (LPARAM)MPLAYERLIST);
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

	rectBox->sRect.x = 730;
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

	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[0].onBmp, PHOTO_PATH"back1.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[0].offBmp, PHOTO_PATH"back1.png");
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

static int MusicListStartProc(HWND hwnd, int nMessage, WPARAM wParam, LPARAM lParam)
{
	int err_code;
	static BITMAP bmpBg;
	static HWND rectBox_hwnd;
	static HWND pHbar_hwnd;
	int newX;
	int newY;

	switch(nMessage)
	{
		case MSG_CREATE:
		{
			gui_debug("MSG_CREATE\n");

			//headbarView = HeadBarDataInit(headbarView);
			pHbar = HeadBarAFunctionSet(ButtonCallbackFun);
			pHbar_hwnd = CreateWindowEx(HEADBARA, "",
			WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT,
			0, pHbar->sRect.x, pHbar->sRect.y, pHbar->sRect.w, pHbar->sRect.h,
			hwnd, (DWORD)pHbar);

			SetTimer(hwnd, USER_TIMER_ID, 1);
			break;
		}
		case MSG_TIMER:
		{
			KillTimer(hwnd, USER_TIMER_ID);

			gui_debug("MSG_TIMER\n");

			RectBoxView = RectBoxDataInit(RectBoxView);
			RectBoxView->hwnd = CreateWindowEx(RECTBOX, "",
			WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT,
			4, RectBoxView->sRect.x , RectBoxView->sRect.y, RectBoxView->sRect.w, RectBoxView->sRect.h,
			hwnd, (DWORD)RectBoxView);

			listboxView = ListboxDataInit(listboxView);
			if(!listboxView){
				return 0;
			}
			listboxView->parentHwnd = hwnd;
			listboxView->sfHwnd= CreateWindowEx(LISTBOX_VIEW, "",
				WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT,
				2, listboxView->x, listboxView->y, listboxView->ht, listboxView->vt,
				hwnd, (DWORD)listboxView);
			SetWindowFont(listboxView->sfHwnd, GetLogFont(ID_FONT_SIMSUN_25));

			return 0;
		}
		case MSG_ERASEBKGND: {
			gui_debug("MSG_ERASEBKGND\n");

			HDC hdc = (HDC) wParam;
			if(winbackward.bmBits)
			{
				FillBoxWithBitmap(hdc, 0, 0, 800, 480, &winbackward);
			}

			return 0;
		}
		case MSG_LBUTTONDOWN:
		{
			newX = LOWORD(lParam);
			newY = HIWORD(lParam);

			break;
		}
		case MSG_LBUTTONUP:
		{
			gui_debug("MSG_LBUTTONDOWN\n");
			break;
		}
		case MSG_DESTROY:
			gui_debug("MSG_DESTROY\n");
			return 0;
		case MSG_CLOSE:
			gui_debug("MSG_CLOSE\n");
			ListBoxDataDeinit(listboxView);
			RectBoxDataUninit(RectBoxView);
			DestroyAllControls(hwnd);
			DestroyMainWindow(hwnd);
			break;
	}

	return DefaultMainWinProc(hwnd, nMessage, wParam, lParam);
}

int MusicListStart(HWND parent, void *data)
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
	CreateInfo.MainWindowProc = MusicListStartProc;
	CreateInfo.lx = 0;
	CreateInfo.ty = 0;
	CreateInfo.rx = g_rcScr.right;
	CreateInfo.by = g_rcScr.bottom;
	CreateInfo.iBkColor = PIXEL_lightgray;
	CreateInfo.dwAddData = 0;
	CreateInfo.dwReserved = 0;
	CreateInfo.hHosting = parent;
	ShowCursor(0);	// hide the cursor

	gui_debug("MusicList win create\n");
	hMainWnd = CreateMainWindow (&CreateInfo);
	if (hMainWnd == HWND_INVALID)
	{
		return -1;
	}
	ShowWindow (hMainWnd, SW_SHOWNORMAL);
	musicListNode.owner = hMainWnd;
	gui_debug("MusicList win finish\n");

	while (GetMessage(&Msg, hMainWnd)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	gui_debug("MusicList win end\n");
	MainWindowThreadCleanup (hMainWnd);
}

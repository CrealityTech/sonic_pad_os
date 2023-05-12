#include "video_list.h"
#include "listbox_view.h"
#include "winLinklist.h"
#include "headBar.h"
#include "headBarA.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "read_file_list.h"
#include "func_uevent.h"
#include "winMonitor.h"
#include "rectBox.h"

static ListboxView *listboxView = NULL;
static HeadBarA *pHbar = NULL;
static RectBox *RectBoxView;

int VideoListStart(HWND parent, void *data);

winNode videoListNode =
{
	VIDEOLIST,
	0,
	VideoListStart,
	NULL,
	NULL,
};

static void ButtonItemFunction(int index, DWORD param)
{
	if(HEADBAR_RIGHT == index){
		gui_debug("head bar start\n");
	}else{
		gui_debug("HEADBAR_LEFT\n");
		SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)VIDEOLIST, (LPARAM)HOMEPAGE);
	}
}

static void ButtonCallbackFun(int index, DWORD param)
{
	HeadBarA *pHbar = (HeadBarA*)param;

	gui_debug("index=%d, sRect.w=%d\n", index, pHbar->sBg.sRect.w);
	PlayerFilePath = NULL;
	SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)VIDEOLIST, (LPARAM)HOMEPAGE);
}

static int recBoxFun(int index, DWORD param)
{
	if(!PlayerFilePath){
		PlayerFilePath = mediaFilePath[0];
		PlayerFileName = mediaFileName[0];
	}
	if(PlayerFilePath[0] != 0)
		SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)VIDEOLIST, (LPARAM)PLAYERLIST);
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

static headBarView *HeadBarDataInit(headBarView *headBar)
{
	int err_code;
	headBar = (headBarView *)calloc(1, sizeof(headBarView));
	if(headBar == NULL){
		gui_warn("HeadBarDataInit malloc error\n");
		return NULL;
	}
	headBar->x = 0;
	headBar->y = 0;
	headBar->height = 120;
	headBar->width = 800;
	headBar->textY = 50;

	headBar->gapwbx = 40;
	headBar->gapwby = 30;
	headBar->textFt = GetLogFont(ID_FONT_SIMSUN_50);

	//err_code = LoadBitmapFromFile(HDC_SCREEN, &headBar->headBarBg, PHOTO_PATH"hbar_bg.png");
	headBar->leftItem = (headbarItem *)calloc(1, sizeof(headbarItem));
	if(headBar->leftItem == NULL){
		goto ERR;
	}
//	headBar->rightItem = (headbarItem *)calloc(1, sizeof(headbarItem));
//	if(headBar->rightItem == NULL){
//		goto ERR;
//	}
	headBar->middleItem = (headbarItem *)calloc(1, sizeof(headbarItem));
	if(headBar->middleItem == NULL){
		goto ERR;
	}

	headBar->leftItem->btn = (ButtonData *)calloc(1, sizeof(ButtonData));
	if(headBar->leftItem->btn == NULL){
		goto ERR;
	}
	headBar->leftItem->btn->fun = ButtonItemFunction;
	headBar->leftItem->btn->onStus = 0;
	err_code = LoadBitmapFromFile(HDC_SCREEN, &headBar->leftItem->btn->onBmp, PHOTO_PATH"fanhui.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_warn("load %s bitmap failed\n", PHOTO_PATH"fanhui.png");
	}

	memcpy(headBar->middleItem->text, getLabel(1), strlen(getLabel(1)));

	return headBar;
ERR:
	HeadBarDataDeinit(headBar);
	return NULL;
}

int StartPlayVideo(void)
{
	float scaleWidth;
	float scaleHeigh;
	MediaInfo *videoInfo = NULL;
	TplayerVideoScaleDownType ScaleDownTypeW,ScaleDownTypeH ;

	if(PlayerFilePath == NULL){
		gui_error("playing file path error");
		return -1;
	}

	gui_debug("start playing %s", PlayerFilePath);

	tplayer_setdisplayrect(0, 0, 800, 480);
	if(tplayer_play_url(PlayerFilePath) < 0){
		return -1;
	}
#if 1
	videoInfo = tplayer_getmediainfo();
	if(videoInfo == NULL){
		gui_error("videoInfo error\n");
		return -1;
	}
	if(!videoInfo->pVideoStreamInfo){
		return 0;
	}
	gui_debug("videoInfo:  width = %d,height = %d %u...............................", videoInfo->pVideoStreamInfo->nWidth,videoInfo->pVideoStreamInfo->nHeight,videoInfo->nDurationMs);

	scaleWidth = (float)((float)videoInfo->pVideoStreamInfo->nWidth / (float)MAXWIDTH);
	scaleHeigh = (float)((float)videoInfo->pVideoStreamInfo->nHeight / (float)MAXHEIGHT);
	gui_debug("videoInfo:  width = %f,height = %f", scaleWidth,scaleHeigh);
	if(scaleWidth <= 1){
		ScaleDownTypeW = TPLAYER_VIDEO_SCALE_DOWN_1;
	}else if(scaleWidth <= 2){
		ScaleDownTypeW = TPLAYER_VIDEO_SCALE_DOWN_2;
	}else if(scaleWidth <= 4){
		ScaleDownTypeW = TPLAYER_VIDEO_SCALE_DOWN_4;
	}else{
		ScaleDownTypeW = TPLAYER_VIDEO_SCALE_DOWN_8;
	}

	if(scaleHeigh <= 1){
		ScaleDownTypeH = TPLAYER_VIDEO_SCALE_DOWN_1;
	}else if(scaleHeigh <= 2){
		ScaleDownTypeH = TPLAYER_VIDEO_SCALE_DOWN_2;
	}else if(scaleHeigh <= 4){
		ScaleDownTypeH = TPLAYER_VIDEO_SCALE_DOWN_4;
	}else{
		ScaleDownTypeH = TPLAYER_VIDEO_SCALE_DOWN_8;
	}

	tplayer_setscaledown(ScaleDownTypeW, ScaleDownTypeH);
	gui_debug("videoInfo:  ScaleDownTypeW = %d,ScaleDownTypeH = %d", ScaleDownTypeW, ScaleDownTypeH);
#endif

	return 0;
}

static int select_index;
static int FileNum;
static void TplayVideo(int index, DWORD param)
{
	ListboxView *boxview = (ListboxView*)param;
	if(index < 0){
		return;
	}

	gui_debug("index=%d\n", index);
	PlayerFilePath = mediaFilePath[index];
	PlayerFileName = mediaFileName[index];

	select_index = index;

	if(PlayerFilePath[0] != 0){
		SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)VIDEOLIST, (LPARAM)PLAYERLIST);
	}
}

int GetVideoNextLastIndex(int NextLast)
{
	select_index += NextLast;

	if(select_index >= FileNum)
		select_index = 0;
	if(select_index < 0){
		select_index = FileNum - 1;
	}

	PlayerFilePath = mediaFilePath[select_index];
	PlayerFileName = mediaFileName[select_index];

	tplayer_pause();
	tplayer_stop();
	if(!StartPlayVideo());
		tplayer_play();

	gui_debug("index=%d FileNum= %d %s\n", select_index, FileNum,PlayerFileName);
	return 0;
}

static ListboxView* ListboxDataInit(ListboxView *listboxView)
{
	int i;
	int err_code;
	char pBuf[NAME_MAX_SIZE];


	FileNum = getFilePath(VIDEO_FILE_TYPE) + 1;
	char netvideo[] = "http://bbhlt.shoujiduoduo.com/bb/video/first/20000373.mp4";
	memcpy(mediaFileName[FileNum-1], "net example:20000373.mp4", strlen("net example:20000373.mp4"));
	memcpy(mediaFilePath[FileNum-1], netvideo, strlen(netvideo));

	if(FileNum <= 0){
		return NULL;
	}

	listboxView = (ListboxView*) malloc(sizeof(ListboxView));
	if (NULL == listboxView) {
		gui_warn("malloc ListboxView data error\n");
		return NULL;
	}
	memset((void *) listboxView, 0, sizeof(ListboxView));
	/*
	err_code = LoadBitmapFromFile(HDC_SCREEN, &listboxView->ltbBg, PHOTO_PATH"music_item.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_warn("load %s bitmap failed\n", PHOTO_PATH"music_item.png");
	}*/

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

	gui_debug("Video file num %d\n", listboxView->itSz);

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
		listboxView->itDt[i].btn->fun = TplayVideo;
	}

	for(i = 0; i < listboxView->itSz; i++){
		char FileName[512];
		sprintf(FileName, "%d.%s",i+1, mediaFileName[i]);
		memcpy(listboxView->itDt[i].lT, FileName, strlen(FileName));
		listboxView->lTextFt[i] = GetLogFont(ID_FONT_SIMSUN_25);
		listboxView->rTextFt[i] = GetLogFont(ID_FONT_SIMSUN_25);

	}
	return listboxView;
ERR:
	ListBoxDataDeinit(listboxView);
	return NULL;
}

static int VideoListStartProc(HWND hwnd, int nMessage, WPARAM wParam, LPARAM lParam)
{
	switch(nMessage)
	{
		case MSG_CREATE:
		{
			gui_debug("MSG_CREATE\n");

			pHbar = HeadBarAFunctionSet(ButtonCallbackFun);
			if(pHbar){
				CreateWindowEx(HEADBARA, "",
				WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT,
				1, pHbar->sRect.x, pHbar->sRect.y, pHbar->sRect.w, pHbar->sRect.h,
				hwnd, (DWORD)pHbar);
			}
			SetTimer(hwnd, USER_TIMER_ID, 1);
			return 0;
		}
		case MSG_TIMER:
		{
			gui_debug("MSG_TIMER\n");
			KillTimer(hwnd, USER_TIMER_ID);

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
				0, listboxView->x, listboxView->y, listboxView->ht, listboxView->vt,
				hwnd, (DWORD)listboxView);
			SetWindowFont(listboxView->sfHwnd, GetLogFont(ID_FONT_SIMSUN_25));

			break;
		}
		case MSG_ERASEBKGND: {
			int newX = LOWORD(lParam);
			int newY = HIWORD(lParam);
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
			gui_debug("MSG_LBUTTONDOWN\n");
			break;
		}
		case MSG_LBUTTONUP:
		{
			gui_debug("MSG_LBUTTONDOWN\n");
			//SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)MUSICLIST, (LPARAM)HOMEPAGE);
			break;
		}
		case MSG_DESTROY:
			gui_debug("MSG_DESTROY\n");

			break;
		case MSG_CLOSE:
			gui_debug("MSG_CLOSE\n");
			DestroyAllControls(hwnd);
			ListBoxDataDeinit(listboxView);
			DestroyMainWindow(hwnd);
			break;
	}

	return DefaultMainWinProc(hwnd, nMessage, wParam, lParam);
}

int VideoListStart(HWND parent, void *data)
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
	CreateInfo.MainWindowProc = VideoListStartProc;
	CreateInfo.lx = 0;
	CreateInfo.ty = 0;
	CreateInfo.rx = g_rcScr.right;
	CreateInfo.by = g_rcScr.bottom;
	CreateInfo.iBkColor = PIXEL_lightgray;
	CreateInfo.dwAddData = 0;
	CreateInfo.dwReserved = 0;
	CreateInfo.hHosting = parent;
	ShowCursor(0);	// hide the cursor

	gui_debug("VideoList win create\n");
	hMainWnd = CreateMainWindow (&CreateInfo);
	if (hMainWnd == HWND_INVALID)
	{
		return -1;
	}
	ShowWindow (hMainWnd, SW_SHOWNORMAL);
	videoListNode.owner = hMainWnd;
	gui_debug("VideoList win finish\n");

	while (GetMessage(&Msg, hMainWnd)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	gui_debug("VideoList win end\n");
	MainWindowThreadCleanup (hMainWnd);
}

#include "music_list.h"
#include "listbox_view.h"
#include "winLinklist.h"
#include "headBar.h"
#include "headBarA.h"
#include "read_file_list.h"
#include "func_uevent.h"
#include "rectBox.h"
#include "winMonitor.h"

static headBarView *headbarView = NULL;
static RectBox *RectBoxView;
static HeadBarA *pHbar = NULL;
static int volume = 30;
static char *CurrPlayerFile = NULL;

int MPlayerListStart(HWND parent, void *data);

winNode MPlayerListNode =
{
	MPLAYERLIST,
	0,
	MPlayerListStart,
	NULL,
	NULL,
};

static void ButtonItemFunction(int index, DWORD param)
{
	if(HEADBAR_RIGHT == index){
		gui_debug("head bar start\n");
	}else{
		gui_debug("HEADBAR_LEFT\n");
		SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)MPLAYERLIST, (LPARAM)HOMEPAGE);
	}
}

static void ButtonCallbackFun(int index, DWORD param)
{
	HeadBarA *pHbar = (HeadBarA*)param;

	gui_debug("index=%d, sRect.w=%d\n", index, pHbar->sBg.sRect.w);
	tplayer_pause();
	PlayerFilePath = NULL;
	CurrPlayerFile = NULL;
	SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)MPLAYERLIST, (LPARAM)HOMEPAGE);
}

static void MPlayerUpdateText(void)
{
	char conent[512];
	HDC hdc;

	MediaInfo *musicInfo;

	musicInfo = tplayer_getmediainfo();

	memset(RectBoxView->pText[0].pText, 0, sizeof(RectBoxView->pText[0].pText));
	memcpy(RectBoxView->pText[0].pText, PlayerFileName, strlen(PlayerFileName));

	memset(conent, '\0', sizeof(conent));
	sprintf(conent,"%s %s	%s	%s", musicInfo->Id3Info->title,
		musicInfo->Id3Info->artist,
		musicInfo->Id3Info->albumArtist,
		musicInfo->Id3Info->author);
	memcpy(RectBoxView->pText[1].pText, conent, strlen(conent));

	InvalidateRect(RectBoxView->hwnd, NULL, TRUE);
}

static void TplayMusicControl(int index, DWORD param)
{
	RectBox *boxview = (RectBox*)param;

	switch(index){
		case 0:
			//gui_debug("index %d %d\n",index, boxview->pBtn[0].onStus);
			GetMusicNextLastIndex(-1);
			MPlayerUpdateText();
			break;
		case 1:
			if(boxview->pBtn[1].onStus == 0){
				tplayer_play();
			}else{
				tplayer_pause();
			}
			break;
		case 2:
			GetMusicNextLastIndex(1);
			MPlayerUpdateText();
			break;
		case 3:
			if(boxview->pBtn[3].onStus == 0)
				tplayer_setlooping(true);
			else
				tplayer_setlooping(false);

			break;
		case 4:
			SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)MPLAYERLIST, (LPARAM)MUSICLIST);
			break;
		default:
			break;
	}

}

static void TplayAudio(int message, DWORD param)
{
	RectBox *boxview = (RectBox*)param;
	static int duration;
	static int currentpos;
	static int set_flag = 0;
#define VOLUME_MAX  40

	switch(message){
		case MSG_LBUTTONDOWN:
			if(tplayer_IsPlaying()){
				tplayer_getduration(&duration);
				tplayer_getcurrentpos(&currentpos);
			}
			set_flag = 0;
			break;
		case MSG_LBUTTONUP:
			if(tplayer_IsPlaying() && boxview->moveFlag && set_flag)
				tplayer_seekto((int)currentpos);
			break;
		case MSG_MOUSEMOVE:
				if(boxview->moveFlag){
					gui_debug("moveGapX=%d, moveGapY=%d\n", boxview->moveGapX,boxview->moveGapY);
					if(abs(boxview->moveGapY) > 3 && boxview->oldX < 400){
						volume -= (int)(boxview->moveGapY * VOLUME_MAX / boxview->sRect.h );

						if(volume < 0){
							volume = 0;
						}

						if(volume > VOLUME_MAX){
							volume = VOLUME_MAX;
						}
						gui_debug("volume= %d\n",(int)volume);
						char str_v[64]={0};
						sprintf(str_v, "%d", (int)volume);
						memset(pHbar->pText[2].pText, 0, sizeof(RectBoxView->pText[2].pText));
						memcpy(pHbar->pText[2].pText, str_v, strlen(str_v));
						InvalidateRect(pHbar->hwnd, NULL, TRUE);
						tplayer_volume((int)volume);

					}

					if(duration == 0){
						return;
					}
					if(abs(boxview->moveGapX) > 20 && boxview->oldY > 100){

						currentpos += (int)(boxview->moveGapX * duration / boxview->sRect.w );
						if(currentpos < 0){
							currentpos = 0;
						}

						if(currentpos > duration){
							currentpos = duration;
						}

						set_flag = 1;
						gui_debug("currentpos= %d, duration= %d\n", currentpos, duration);
						SendDlgItemMessage (GetMonitorHwnd(), 1, PBM_SETPOS, currentpos, 0L);

						/*
						int posser = boxview->pBmp[0].sRect.w * currentpos / duration;
						boxview->pBmp[1].sRect.w = posser;
						boxview->pBmp[2].sRect.x = boxview->pBmp[1].sRect.x + posser;
						SendMessage (GetMonitorHwnd(), MSG_PAINT, 0, 0);
						*/
					}

					return;
				}
			break;
	}


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

	rectBox->sRect.x = 0;
	rectBox->sRect.y = 70;
	rectBox->sRect.w = 800;
	rectBox->sRect.h = 410;

	rectBox->nBtnNum = 5;
	rectBox->pBtn = (ButtonType *) malloc(sizeof(ButtonType) * rectBox->nBtnNum);
	if(rectBox->pBtn == NULL){
		gui_error("malloc failed\n");
		goto ERR;
	}
	memset(rectBox->pBtn, 0, (sizeof(ButtonType) * rectBox->nBtnNum));

	rectBox->pBtn[0].onStus = 0;
	rectBox->pBtn[0].fun = TplayMusicControl;
	rectBox->pBtn[0].sRect.x = 0;
	rectBox->pBtn[0].sRect.y = 350;
	rectBox->pBtn[0].sRect.w = 54;
	rectBox->pBtn[0].sRect.h = 54;

	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[0].onBmp, PHOTO_PATH"last.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[0].offBmp, PHOTO_PATH"last.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}

	rectBox->pBtn[1].onStus = 0;
	rectBox->pBtn[1].fun = TplayMusicControl;
	rectBox->pBtn[1].sRect.x = 60;
	rectBox->pBtn[1].sRect.y = 350;
	rectBox->pBtn[1].sRect.w = 54;
	rectBox->pBtn[1].sRect.h = 54;

	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[1].onBmp, PHOTO_PATH"pause.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[1].offBmp, PHOTO_PATH"play.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}

	rectBox->pBtn[2].onStus = 0;
	rectBox->pBtn[2].fun = TplayMusicControl;
	rectBox->pBtn[2].sRect.x = 60 + 60;
	rectBox->pBtn[2].sRect.y = 350;
	rectBox->pBtn[2].sRect.w = 54;
	rectBox->pBtn[2].sRect.h = 54;

	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[2].onBmp, PHOTO_PATH"next.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[2].offBmp, PHOTO_PATH"next.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}

	rectBox->pBtn[3].onStus = 0;
	rectBox->pBtn[3].fun = TplayMusicControl;
	rectBox->pBtn[3].sRect.x = 60 + 60 + 60;
	rectBox->pBtn[3].sRect.y = 350;
	rectBox->pBtn[3].sRect.w = 54;
	rectBox->pBtn[3].sRect.h = 54;

	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[3].onBmp, PHOTO_PATH"listloop.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[3].offBmp, PHOTO_PATH"singleloop.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}

	rectBox->pBtn[4].onStus = 0;
	rectBox->pBtn[4].fun = TplayMusicControl;
	rectBox->pBtn[4].sRect.x = 20;
	rectBox->pBtn[4].sRect.y = 0;
	rectBox->pBtn[4].sRect.w = 54;
	rectBox->pBtn[4].sRect.h = 54;

	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[4].onBmp, PHOTO_PATH"back.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBtn[4].offBmp, PHOTO_PATH"back.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	/*

	rectBox->nBmpNum = 1;
	rectBox->pBmp = (ButtonType *) malloc(sizeof(ButtonType) * rectBox->nBmpNum);
	if(rectBox->pBmp == NULL){
		gui_error("malloc failed\n");
		goto ERR;
	}
	memset(rectBox->pBmp, 0, (sizeof(ButtonType) * rectBox->nBmpNum));

	rectBox->pBmp[0].sRect.x = 0;
	rectBox->pBmp[0].sRect.y = 345;
	rectBox->pBmp[0].sRect.w = 800;
	rectBox->pBmp[0].sRect.h = 60;
	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBmp[0].sBmp, PHOTO_PATH"progressbar.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}

	rectBox->pBmp[1].sRect.x = 240;
	rectBox->pBmp[1].sRect.y = 360;
	rectBox->pBmp[1].sRect.w = 2;
	rectBox->pBmp[1].sRect.h = 30;
	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBmp[1].sBmp, PHOTO_PATH"progressbar_b.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	rectBox->pBmp[2].sRect.x = 240;
	rectBox->pBmp[2].sRect.y = 350;
	rectBox->pBmp[2].sRect.w = 15;
	rectBox->pBmp[2].sRect.h = 50;
	err_code = LoadBitmapFromFile(HDC_SCREEN, &rectBox->pBmp[2].sBmp, PHOTO_PATH"blue1.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}

*/
	rectBox->mvFun = TplayAudio;

	rectBox->nTextNum = 3;
	rectBox->pText = (ButtonType *) malloc(sizeof(ButtonType) * rectBox->nTextNum);
	if(rectBox->pText == NULL){
		gui_error("malloc failed\n");
		goto ERR;
	}
	memset(rectBox->pText, 0, (sizeof(ButtonType) * rectBox->nTextNum));
	rectBox->pText[0].sRect.x = 250;
	rectBox->pText[0].sRect.y = 0;
	rectBox->pText[0].sRect.w = 200;
	rectBox->pText[0].sRect.h = 40;
	rectBox->pText[0].sColor = COLOR_lightwhite;
	rectBox->pText[0].sFont = GetLogFont(ID_FONT_SIMSUN_30);

	rectBox->pText[1].sRect.x = 250;
	rectBox->pText[1].sRect.y = 40;
	rectBox->pText[1].sRect.w = 200;
	rectBox->pText[1].sRect.h = 30;
	rectBox->pText[1].sColor = COLOR_lightwhite;
	rectBox->pText[1].sFont = GetLogFont(ID_FONT_SIMSUN_20);

	rectBox->pText[2].sRect.x = 100;
	rectBox->pText[2].sRect.y = 80;
	rectBox->pText[2].sRect.w = 300;
	rectBox->pText[2].sRect.h = 210;
	rectBox->pText[2].sColor = COLOR_lightwhite;
	rectBox->pText[2].sFont = GetLogFont(ID_FONT_SIMSUN_20);

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

static int MPlayerListStartProc(HWND hwnd, int nMessage, WPARAM wParam, LPARAM lParam)
{
	int err_code;
	int newX;
	int newY;
	HDC hdc;
	int duration;
	int currpos;
	static int status = 0;

	switch(nMessage)
	{
		case MSG_CREATE:
		{
			gui_debug("MSG_CREATE\n");

			pHbar = HeadBarAFunctionSet(ButtonCallbackFun);
			RectBoxView = RectBoxDataInit(RectBoxView);
			SetTimer(hwnd, USER_TIMER_ID, 1);
			status = 0;
			break;
		}
		case MSG_TIMER:
		{
			switch(status){
				case 0:
					RectBoxView->hwnd = CreateWindowEx(RECTBOX, "",
					WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT,
					4, RectBoxView->sRect.x , RectBoxView->sRect.y, RectBoxView->sRect.w, RectBoxView->sRect.h,
					hwnd, (DWORD)RectBoxView);

					pHbar->hwnd = CreateWindowEx(HEADBARA, "",
					WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT,
					0, pHbar->sRect.x, pHbar->sRect.y, pHbar->sRect.w, pHbar->sRect.h,
					hwnd, (DWORD)pHbar);

					CreateWindowEx("progressbar", "",
								WS_VISIBLE,WS_EX_USEPARENTCURSOR,
								1, 240, 440, 500, 20, hwnd, 0);
					status++;
					SetTimer(hwnd, USER_TIMER_ID, 1);
					return 0;
				case 1:
					if(!PlayerFilePath){
						return 0;
					}
					if(PlayerFilePath == CurrPlayerFile){
						if(!tplayer_IsPlaying){
							tplayer_play();
						}
						MPlayerUpdateText();
						status++;
						break;
					}

					tplayer_pause();
					tplayer_stop();
					tplayer_play_url(PlayerFilePath);
					tplayer_play();
					tplayer_setlooping(true);
					MPlayerUpdateText();
					CurrPlayerFile = PlayerFilePath;
					status++;
					break;
				default:
					if(tplayer_IsPlaying() && tplayer_getduration(&duration) == 0){
						SendDlgItemMessage (hwnd, 1, PBM_SETRANGE, 0, duration);
						tplayer_getcurrentpos(&currpos);
						SendDlgItemMessage (hwnd, 1, PBM_SETPOS, currpos, 0L);
						/*
						tplayer_getcurrentpos(&currpos);
						int posser = RectBoxView->pBmp[0].sRect.w * currpos / duration;
						RectBoxView->pBmp[1].sRect.w = posser;
						RectBoxView->pBmp[2].sRect.x = RectBoxView->pBmp[1].sRect.x + posser;
						SendMessage (RectBoxView->hwnd, MSG_PAINT, 0, 0);
						*/
					}

					if(tplayer_getcompletestate()){
						SendDlgItemMessage (hwnd, 1, PBM_SETPOS, duration, 0L);
						if(RectBoxView->pBtn[3].onStus){
							GetMusicNextLastIndex(1);
							MPlayerUpdateText();
						}
					}
					break;
			}
			SetTimer(hwnd, USER_TIMER_ID, 5);
			return 0;
		}
		case MSG_ERASEBKGND: {
			gui_debug("MSG_ERASEBKGND\n");

			hdc = (HDC) wParam;
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

			KillTimer(hwnd, USER_TIMER_ID);
			RectBoxDataUninit(RectBoxView);
			DestroyAllControls(hwnd);
			DestroyMainWindow(hwnd);
			break;
	}

	return DefaultMainWinProc(hwnd, nMessage, wParam, lParam);
}

int MPlayerListStart(HWND parent, void *data)
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
	CreateInfo.MainWindowProc = MPlayerListStartProc;
	CreateInfo.lx = 0;
	CreateInfo.ty = 0;
	CreateInfo.rx = g_rcScr.right;
	CreateInfo.by = g_rcScr.bottom;
	CreateInfo.iBkColor = PIXEL_lightgray;
	CreateInfo.dwAddData = 0;
	CreateInfo.dwReserved = 0;
	CreateInfo.hHosting = parent;
	ShowCursor(0);	// hide the cursor

	gui_debug("MPlayerList win create\n");
	hMainWnd = CreateMainWindow (&CreateInfo);
	if (hMainWnd == HWND_INVALID)
	{
		return -1;
	}
	ShowWindow (hMainWnd, SW_SHOWNORMAL);
	MPlayerListNode.owner = hMainWnd;

	while (GetMessage(&Msg, hMainWnd)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	gui_debug("MPlayerList win end\n");
	MainWindowThreadCleanup (hMainWnd);
}

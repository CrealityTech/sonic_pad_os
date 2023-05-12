#include "video_list.h"
#include "listbox_view.h"
#include "winLinklist.h"
#include "headBar.h"
#include "headBarA.h"
#include "wifi.h"
#include "winMonitor.h"

static ListboxView *listboxView = NULL;
static headBarView *headbarView = NULL;
static HeadBarA *pHbar = NULL;
static struct wifi_info *wifi_scan_info;

struct wifi_info *wifi_info_list;

int SettingListStart(HWND parent, void *data);

winNode settingListNode =
{
	SETTINGLIST,
	0,
	SettingListStart,
	NULL,
	NULL,
};

static void ButtonItemFunction(int index, DWORD param)
{
	if(HEADBAR_RIGHT == index){
		gui_debug("head bar start\n");
	}else{
		gui_debug("HEADBAR_LEFT\n");
		SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)SETTINGLIST, (LPARAM)HOMEPAGE);
	}
}

static void ButtonCallbackFun(int index, DWORD param)
{
	HeadBarA *pHbar = (HeadBarA*)param;

	gui_debug("index=%d, sRect.w=%d\n", index, pHbar->sBg.sRect.w);

	SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)SETTINGLIST, (LPARAM)HOMEPAGE);
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

static void WifiConnect(int index, DWORD param)
{
	ListboxView *boxview = (ListboxView*)param;
	char passwd[] = "Aw.guest6688";
	char cmd[256] = {0};

	if(boxview->moveFlag){
		return;
	}
	gui_debug("index=%d\n", index);

	sprintf(cmd, "%s %s %s","wifi_connect_ap_test", wifi_info_list[index].ssid, passwd);
	system(cmd);
}

static ListboxView* ListboxDataInit(ListboxView *listboxView)
{
	int i;
	int err_code;
	char pBuf[NAME_MAX_SIZE];
	int total_num;

	wifi_info_list = GetWifiInfo(&total_num);
	if(wifi_info_list == NULL){
		return NULL;
	}

	listboxView = (ListboxView*) malloc(sizeof(ListboxView));
	if (NULL == listboxView) {
		gui_warn("malloc ListboxView data error\n");
		return NULL;
	}
	memset((void *) listboxView, 0, sizeof(ListboxView));

	err_code = LoadBitmapFromFile(HDC_SCREEN, &listboxView->ltbBg, PHOTO_PATH"music_item.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_warn("load %s bitmap failed\n", PHOTO_PATH"music_item.png");
	}

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

	listboxView->itSz = total_num;
	listboxView->Index = 0;
	listboxView->showSz = total_num < 8 ? total_num : 8;
	listboxView->lthGap = 10;
	listboxView->moveGap = 0;

	gui_debug("wifi num %d\n", listboxView->itSz);

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
		listboxView->itDt[i].btn->fun = WifiConnect;
	}

	for(i = 0; i < listboxView->itSz; i++){
		memcpy(listboxView->itDt[i].lT, wifi_info_list[i].ssid, strlen(wifi_info_list[i].ssid));
		listboxView->lTextFt[i] = GetLogFont(ID_FONT_SIMSUN_25);
		listboxView->rTextFt[i] = GetLogFont(ID_FONT_SIMSUN_25);
	}

	return listboxView;
ERR:
ListBoxDataDeinit(listboxView);
return NULL;

}


static int SettingListStartProc(HWND hwnd, int nMessage, WPARAM wParam, LPARAM lParam)
{
	struct wifi_info *wifi_info_list;

	switch(nMessage)
	{
		case MSG_CREATE:
		{
			gui_debug("MSG_CREATE\n");

			//headbarView = HeadBarDataInit(headbarView);

			pHbar = HeadBarAFunctionSet(ButtonCallbackFun);
			CreateWindowEx(HEADBARA, "",
			WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT,
			1, pHbar->sRect.x, pHbar->sRect.y, pHbar->sRect.w, pHbar->sRect.h,
			hwnd, (DWORD)pHbar);
			SetTimer(hwnd, USER_TIMER_ID, 1);
			return 0;
		}
		case MSG_TIMER:
		{
			gui_debug("MSG_TIMER\n");
			KillTimer(hwnd, USER_TIMER_ID);
			listboxView = ListboxDataInit(listboxView);
			if(listboxView){
				listboxView->parentHwnd = hwnd;
				listboxView->sfHwnd= CreateWindowEx(LISTBOX_VIEW, "",
					WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT,
					0, listboxView->x, listboxView->y, listboxView->ht, listboxView->vt,
					hwnd, (DWORD)listboxView);
				SetWindowFont(listboxView->sfHwnd, GetLogFont(ID_FONT_SIMSUN_25));
			}


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
			ListBoxDataDeinit(listboxView);
			listboxView = NULL;

			DestroyAllControls(hwnd);
			DestroyMainWindow(hwnd);
			break;
	}

	return DefaultMainWinProc(hwnd, nMessage, wParam, lParam);
}

int SettingListStart(HWND parent, void *data)
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
	CreateInfo.MainWindowProc = SettingListStartProc;
	CreateInfo.lx = 0;
	CreateInfo.ty = 0;
	CreateInfo.rx = g_rcScr.right;
	CreateInfo.by = g_rcScr.bottom;
	CreateInfo.iBkColor = PIXEL_lightgray;
	CreateInfo.dwAddData = 0;
	CreateInfo.dwReserved = 0;
	CreateInfo.hHosting = parent;
	ShowCursor(0);	// hide the cursor

	gui_debug("SettingList win create\n");
	hMainWnd = CreateMainWindow (&CreateInfo);
	if (hMainWnd == HWND_INVALID)
	{
		return -1;
	}
	ShowWindow (hMainWnd, SW_SHOWNORMAL);
	settingListNode.owner = hMainWnd;
	gui_debug("SettingList win finish\n");

	while (GetMessage(&Msg, hMainWnd)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	gui_debug("SettingList win end\n");
	MainWindowThreadCleanup (hMainWnd);
}

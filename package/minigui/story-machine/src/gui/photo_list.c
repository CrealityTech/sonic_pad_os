#include "photo_list.h"
#include "winLinklist.h"
#include "headBar.h"
#include "read_file_list.h"
#include "winMonitor.h"
#include "rectBox.h"
#include "listbox_view.h"
#include "headBarA.h"

static ListboxView *listboxView = NULL;
static HeadBarA *pHbar = NULL;
static RectBox *RectBoxView;

int PhotoFileNum;
int select_index;

int PhotoListStart(HWND parent, void *data);

winNode photoListNode =
{
	PHOTOLIST,
	0,
	PhotoListStart,
	NULL,
	NULL,
};

static void ButtonCallbackFun(int index, DWORD param)
{
	HeadBarA *pHbar = (HeadBarA*)param;

	gui_debug("index=%d, sRect.w=%d\n", index, pHbar->sBg.sRect.w);

	SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)PHOTOLIST, (LPARAM)HOMEPAGE);
}

static int recBoxFun(int index, DWORD param)
{
	if(!PlayerFilePath){
		PlayerFilePath = mediaFilePath[0];
		PlayerFileName = mediaFileName[0];
	}
	if(PlayerFilePath[0] != 0)
		SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)PHOTOLIST, (LPARAM)PHOTOPLAYLIST);
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

static void photoPlay(int index, DWORD param)
{
	ListboxView *boxview = (ListboxView*)param;
	int err_code;
	if(index < 0){
		return;
	}
	gui_debug("index = %d\n", index);

	PlayerFilePath = mediaFilePath[index];
	if(PlayerFilePath[0] == 0){

		gui_error("don't found %s\n",PlayerFilePath);
		return;
	}
	select_index = index;
	SendMessage(GetMonitorHwnd(), USER_MSG_WIN_CHANGE, (WPARAM)PHOTOLIST, (LPARAM)PHOTOPLAYLIST);
}

static ListboxView* ListboxDataInit(ListboxView *listboxView)
{
	int i;
	int err_code;
	char pBuf[NAME_MAX_SIZE];

	PhotoFileNum = getFilePath(IMAGE_FILE_TYPE);
	if(PhotoFileNum <= 0){
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

	listboxView->itType = ITEM_BMP;
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

	listboxView->itSz = PhotoFileNum;
	listboxView->Index = 0;
	listboxView->showSz = PhotoFileNum < 8 ? PhotoFileNum : 8;
	listboxView->lthGap = 10;
	listboxView->moveGap = 0;

	gui_debug("photo file num %d\n", listboxView->itSz);

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
		listboxView->itDt[i].btn->fun = photoPlay;
	}

	for(i = 0; i < listboxView->itSz; i++){
		char FileName[512];
		sprintf(FileName, "%d.%s",i+1, mediaFileName[i]);
		memcpy(listboxView->itDt[i].lT, FileName, strlen(FileName));

		listboxView->lTextFt[i] = GetLogFont(ID_FONT_SIMSUN_20);
		listboxView->rTextFt[i] = GetLogFont(ID_FONT_SIMSUN_20);
	}

	return listboxView;
ERR:
ListBoxDataDeinit(listboxView);
return NULL;

}


static int PhotoListStartProc(HWND hwnd, int nMessage, WPARAM wParam, LPARAM lParam)
{
	int err_code;

	switch(nMessage)
	{
		case MSG_CREATE:
		{
			gui_debug("MSG_CREATE\n");

			pHbar = HeadBarAFunctionSet(ButtonCallbackFun);
			CreateWindowEx(HEADBARA, "",
			WS_CHILD | WS_VISIBLE | SS_NOTIFY, WS_EX_TRANSPARENT,
			1, pHbar->sRect.x, pHbar->sRect.y, pHbar->sRect.w, pHbar->sRect.h,
			hwnd, (DWORD)pHbar);

			RectBoxView = RectBoxDataInit(RectBoxView);
			SetTimer(hwnd, USER_TIMER_ID, 1);

			return 0;
		}
		case MSG_TIMER:
		{
			gui_debug("MSG_TIMER\n");
			KillTimer(hwnd, USER_TIMER_ID);
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
				10, listboxView->x, listboxView->y, listboxView->ht, listboxView->vt,
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
			break;
		}
		case MSG_DESTROY:
			gui_debug("MSG_DESTROY\n");
			break;
		case MSG_CLOSE:
		{
			gui_debug("MSG_CLOSE\n");
			RectBoxDataUninit(RectBoxView);
			ListBoxDataDeinit(listboxView);
			DestroyAllControls(hwnd);
			DestroyMainWindow(hwnd);
			break;
		}
	}

	return DefaultMainWinProc(hwnd, nMessage, wParam, lParam);
}

int PhotoListStart(HWND parent, void *data)
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
	CreateInfo.MainWindowProc = PhotoListStartProc;
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
	photoListNode.owner = hMainWnd;
	gui_debug("win finish\n");

	while (GetMessage(&Msg, hMainWnd)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	gui_debug("win end\n");
	MainWindowThreadCleanup (hMainWnd);
}

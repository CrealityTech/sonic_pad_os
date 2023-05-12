#include "winMonitor.h"
#include "homePageProc.h"
#include "winLinklist.h"
#include "func_uevent.h"
#include "slideBoxView.h"
#include "headBarA.h"
#include "rectBox.h"
#include "listbox_view.h"
#include "headBar.h"

db_list_t *WinLinklistHead = NULL;
extern winNode homepageNode;
extern winNode musicListNode;
extern winNode photoListNode;
extern winNode videoListNode;
extern winNode PlayerListNode;
extern winNode settingListNode;
extern winNode photoPlayListNode;
extern winNode MPlayerListNode;

HWND hMonitorHwnd = NULL;

static HeadBarA *pHbar = NULL;
static slideBoxView *slidebox = NULL;
BITMAP winbackward;

slideBoxView *slideBoxViewInit(slideBoxView *slideBox);
HeadBarA *HeadBarADataInit(HeadBarA *pHbar);

static int monitorProc(HWND hwnd, int nMessage, WPARAM wParam, LPARAM lParam)
{
	switch(nMessage)
	{
		case MSG_CREATE:
			gui_debug("MSG_CREATE\n");
			hMonitorHwnd = hwnd;
			homepageProcStart(hwnd, NULL);
			break;

		case USER_MSG_WIN_CHANGE: {
			winNode *oldTmp = NULL;
			winNode *newTmp = NULL;

			gui_debug("{ wParam: %s, lParam: %s}\n", (char *)wParam, (char *)lParam);

			oldTmp = (winNode *)winLinklistFind(WinLinklistHead, (char *)wParam);
			newTmp = (winNode *)winLinklistFind(WinLinklistHead, (char *)lParam);

			if(oldTmp && oldTmp->owner)
			{
				PostMessage(oldTmp->owner, MSG_CLOSE, 0, 0);	// not use SendMessage, in order to change win disp normal
				//SendMessage(oldTmp->owner, MSG_CLOSE, 0, 0);
			}

			if(newTmp)
			{
				if(oldTmp && oldTmp->data)
				{
					newTmp->winStart(hwnd, oldTmp->data);
				}
				else
				{
					newTmp->winStart(hwnd, NULL);
				}
			}

			break;
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
			return 0;
		case MSG_CLOSE:
		{
			gui_debug("MSG_CLOSE\n");
			DestroyAllControls(hwnd);
			DestroyMainWindow(hwnd);
			PostQuitMessage(hwnd);
			return 0;
		}
	}

	return DefaultMainWinProc(hwnd, nMessage, wParam, lParam);
}

void winMonitorStart(void)
{
    MSG Msg;
	HWND hMainWnd;
    MAINWINCREATE CreateInfo;
	int err_code;

	RegisterRectBox();
	RegisterHeadBarA();
	RegisterHeadBarView();
	RegisterslideBoxView();
	RegisterListboxView();

	DoubleBufferEnable(FALSE);

    CreateInfo.dwStyle = WS_NONE;
    CreateInfo.dwExStyle = WS_EX_AUTOSECONDARYDC;
    CreateInfo.spCaption = "story-machine";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor =  0;
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = monitorProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;
    CreateInfo.iBkColor = PIXEL_blue;
    CreateInfo.dwAddData = 0;
    CreateInfo.dwReserved = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
	ShowCursor(0);	// hide the cursor

	slidebox = slideBoxViewInit(slidebox);
	pHbar = HeadBarADataInit(pHbar);

	err_code = LoadBitmapFromFile(HDC_SCREEN, &winbackward, PHOTO_PATH"home_bg.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_warn("load bitmap failed\n");
	}

	gui_debug("the monitor win create\n");
    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
    {
        return;
    }
	ShowWindow (hMainWnd, SW_SHOWNORMAL);
	gui_debug("the monitor win create finish\n");

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

	gui_debug("the monitor win create end\n");
	UnregisterslideBoxView();
	UnregisterHeadBarView();
	UnregisterListboxView();
	UnregisterHeadBarA();
	UnregisterRectBox();
	HeadBarADataUninit(pHbar);
	slideBoxViewDeinit(slidebox);
	unloadBitMap(&winbackward);

    MainWindowThreadCleanup (hMainWnd);
}


HWND GetMonitorHwnd(void)
{
	//gui_debug("hMonitorHwnd=0x%x\n", hMonitorHwnd);
	return hMonitorHwnd;
}

void GuiWinAdd()
{
	WinLinklistHead = winLinklistCreate(WinLinklistHead);
	winLinklistAdd(WinLinklistHead, (void *)&homepageNode);
	winLinklistAdd(WinLinklistHead, (void *)&musicListNode);
	winLinklistAdd(WinLinklistHead, (void *)&photoListNode);
	winLinklistAdd(WinLinklistHead, (void *)&videoListNode);
	winLinklistAdd(WinLinklistHead, (void *)&PlayerListNode);
	winLinklistAdd(WinLinklistHead, (void *)&settingListNode);
	winLinklistAdd(WinLinklistHead, (void *)&photoPlayListNode);
	winLinklistAdd(WinLinklistHead, (void *)&MPlayerListNode);
	return 0;
}
//»ñÈ¡µ±Ç°µÄÁ÷Á¿£¬²ÎÊýÎª½«»ñÈ¡µ½µÄÁ÷Á¿±£´æµÄÎ»ÖÃ

#define WAIT_SECOND 1
long int getCurrentDownloadRates(long int *rate_ppp0, long int *rate_wlan0)
{
	FILE * net_dev_file;	//ÎÄ¼þÖ¸Õë
	char buffer[1024];	//ÎÄ¼þÖÐµÄÄÚÈÝÔÝ´æÔÚ×Ö·û»º³åÇøÀ
	char buffer2[1024];
	size_t bytes_read;	//Êµ¼Ê¶ÁÈ¡µÄÄÚÈÝ´óÐ¡
	char * match;	 //ÓÃÒÔ±£´æËùÆ¥Åä×Ö·û´®¼°Ö®ºóµÄÄÚÈÝ

	net_dev_file=fopen("/proc/net/dev", "r");
	if ( net_dev_file == NULL ){
		printf("open file /proc/net/dev/ error!\n");
		return -1;
	}

	bytes_read = fread(buffer, 1, sizeof(buffer), net_dev_file);//½«ÎÄ¼þÖÐµÄ1024¸ö×Ö·û´óÐ¡µÄÊý¾Ý±£´æµ½bufferÀï
	fclose(net_dev_file);

	//Èç¹ûÎÄ¼þÄÚÈÝ´óÐ¡Îª£°£¬Ã»ÓÐÊý¾ÝÔòÍË³ö
	if (bytes_read == 0 ){
		return -1;
	}

	buffer[bytes_read] = '\0';
	memcpy(buffer2, buffer, sizeof(buffer));

	match = strstr(buffer, "ppp0:");
	if (match){
		sscanf(match, "ppp0:%ld", rate_ppp0);
	}

	match = strstr(buffer2, "wlan0:");
	if (match){
		sscanf(match, "wlan0:%ld", rate_wlan0);
	}
	return 0;
}

static inline float get_diff_time(struct timespec * start , bool update)
{
        float dt;
        struct timespec now;

        clock_gettime(CLOCK_MONOTONIC,&now);
        dt = (float)(now.tv_sec  - start->tv_sec);
        dt += (float)(now.tv_nsec - start->tv_nsec) * 1e-9;

        if(update == true){
                start->tv_sec = now.tv_sec;
                start->tv_nsec = now.tv_nsec;
        }

        return dt;
}

func_callback_t getNetworkRates(void *param)
{
	long int ppp0_start = 0;	//±£´æ¿ªÊ¼Ê±µÄÁ÷Á¿¼ÆÊý
	long int ppp0_end = 0;	//±£´æ½á¹ûÊ±µÄÁ÷Á¿¼ÆÊý
	long int wlan0_start = 0;
	long int wlan0_end = 0;
	float total_time;
	struct timeval time_start, time_end;
	char name[2][5]={"wlan","ppp0"};
	int qcq_flag = 1;
	while(1){
		if(access("/dev/qcqmi0", R_OK) < 0){
			qcq_flag = 1;
		}
		//must pppd again after close wlan0
		if(!access("/dev/qcqmi0", R_OK) && qcq_flag){
			system("/etc/ppp/peers/quectel-ppp-kill &");
			system("/etc/ppp/peers/quectel-pppd.sh &");
			qcq_flag = 0;
		}


		gettimeofday(&time_start, NULL);
		getCurrentDownloadRates(&ppp0_start, &wlan0_start);
		sleep(WAIT_SECOND);
		getCurrentDownloadRates(&ppp0_end, &wlan0_end);
		gettimeofday(&time_end, NULL);
		total_time = (time_end.tv_sec - time_start.tv_sec) * 1000000 + (time_end.tv_usec - time_start.tv_usec);
		total_time /= 1000000;
		//printf("network test\n");
		if(ppp0_end - ppp0_start > 0){
			printf("%s download is :[%ld] - [%ld] = %.2lf KBytes/s time = %.2f\n",
			name[1], ppp0_end, ppp0_start, (float)(ppp0_end-ppp0_start)/total_time/1024,total_time);//´òÓ¡½á¹û
		}
		if(wlan0_end - wlan0_start > 0){
			printf("%s download is :[%ld] - [%ld] = %.2lf KBytes/s time = %.2f\n",
			name[0], wlan0_end, wlan0_start, (float)(wlan0_end-wlan0_start)/total_time/1024,total_time);//´òÓ¡½á¹û
		}

	}
	return 0;
}

int MiniGUIMain(int argc, const char* argv[])
{
	gui_debug("the gui main enter\n");

	InitLogFont();

	int i = 0;
	gui_debug("#############_1\n");
	InitLangAndLable();


	//while(i++<10)
	{
		LANGUAGE lang = getLanguage();
		gui_debug("lang=%d, %s, %s\n", lang, getLabel(2), getLabel(1));

	}
	func_uevent_init();
	func_register_callback(getNetworkRates);
	post_func_sem(NULL);
	//aw_wifi_open();
	tplayer_init(TPLAYER_VIDEO_ROTATE_DEGREE_0);

	GuiWinAdd();
	winMonitorStart();

	UninitLangAndLable();
	UninitLogFont();

    return 0;
}

slideBoxView *slideBoxViewInit(slideBoxView *slideBox)
{
	int i;
	slideBox = (slideBoxView *)calloc(1, sizeof(slideBoxView));
	if(slideBox == NULL){
		gui_debug("slideBox CALLOC FAILED\n");
		return NULL;
	}
	memset(slideBox, 0, sizeof(slideBoxView));

	slideBox->widgetNum = 6;
	slideBox->x = 0;
	slideBox->y = 180;
	slideBox->w = 800;
	slideBox->h = 200;

	slideBox->moveSenstive = 2;
	slideBox->widgetGap = 200;
	slideBox->textFt = GetLogFont(ID_FONT_SIMSUN_20);
	slideBox->textColor = COLOR_black;

	//slideBox->widgets = (widgetData *)calloc(slideBox->widgetNum, sizeof(widgetData));
	slideBox->widgets  = (widgetData*) malloc(sizeof(widgetData)*slideBox->widgetNum);
	gui_debug("###################_1\n");
	gui_debug("slideBox->widgets=0x%x\n", (unsigned int)slideBox->widgets);

	if(slideBox->widgets == NULL){
		gui_debug("slideBox CALLOC FAILED\n");
		goto ERR;
	}
	memset(slideBox->widgets, 0, sizeof(widgetData)*slideBox->widgetNum);

	LoadBitmapFromFile(HDC_SCREEN, &slideBox->widgets[0].bmp, PHOTO_PATH"home_movie_uf.png");
	LoadBitmapFromFile(HDC_SCREEN, &slideBox->widgets[1].bmp, PHOTO_PATH"home_music_uf.png");
	LoadBitmapFromFile(HDC_SCREEN, &slideBox->widgets[2].bmp, PHOTO_PATH"home_picture_uf.png");
	LoadBitmapFromFile(HDC_SCREEN, &slideBox->widgets[3].bmp, PHOTO_PATH"home_record_uf.png");
	LoadBitmapFromFile(HDC_SCREEN, &slideBox->widgets[4].bmp, PHOTO_PATH"home_talk_uf.png");
	LoadBitmapFromFile(HDC_SCREEN, &slideBox->widgets[5].bmp, PHOTO_PATH"home_setting_uf.png");

	#if 1
	memcpy(slideBox->widgets[0].text, getLabel(LANG_LABEL_HOME+1), strlen(getLabel(LANG_LABEL_HOME+1)));
	memcpy(slideBox->widgets[1].text, getLabel(LANG_LABEL_HOME+2), strlen(getLabel(LANG_LABEL_HOME+2)));
	memcpy(slideBox->widgets[2].text, getLabel(LANG_LABEL_HOME+3), strlen(getLabel(LANG_LABEL_HOME+3)));
	memcpy(slideBox->widgets[3].text, getLabel(LANG_LABEL_HOME+4), strlen(getLabel(LANG_LABEL_HOME+4)));
	memcpy(slideBox->widgets[4].text, getLabel(LANG_LABEL_HOME+5), strlen(getLabel(LANG_LABEL_HOME+5)));
	memcpy(slideBox->widgets[5].text, getLabel(LANG_LABEL_HOME+6), strlen(getLabel(LANG_LABEL_HOME+6)));
	#endif

	for(i = 0; i < slideBox->widgetNum; i++){
		slideBox->widgets[i].bx = slideBox->x + slideBox->widgetGap*i - slideBox->widgets[i].bmp.bmWidth/2;
		slideBox->widgets[i].by = 0;
		slideBox->widgets[i].bw = slideBox->widgets[i].bmp.bmWidth;
		slideBox->widgets[i].bh = slideBox->widgets[i].bmp.bmHeight;
		slideBox->widgets[i].fun = NULL;
		slideBox->widgets[i].textRect.top = slideBox->widgets[i].by + slideBox->widgets[i].bh-30;
		slideBox->widgets[i].textRect.left = slideBox->widgets[i].bx;
		slideBox->widgets[i].textRect.bottom = slideBox->widgets[i].textRect.top+50;
		slideBox->widgets[i].textRect.right = slideBox->widgets[i].textRect.left+slideBox->widgets[i].bw;
	}

	return slideBox;
ERR:
	slideBoxViewDeinit(slideBox);
	return NULL;
}

slideBoxView *slideBoxItemFunctionSet(pButtonFun fun)
{
	int i;
	for(i = 0; i < slidebox->widgetNum; i++){
		slidebox->widgets[i].bx = slidebox->x + slidebox->widgetGap*i - slidebox->widgets[i].bmp.bmWidth/2;
		slidebox->widgets[i].by = 0;
		slidebox->widgets[i].bw = slidebox->widgets[i].bmp.bmWidth;
		slidebox->widgets[i].bh = slidebox->widgets[i].bmp.bmHeight;
		slidebox->widgets[i].fun = fun;
		slidebox->widgets[i].textRect.top = slidebox->widgets[i].by + slidebox->widgets[i].bh-30;
		slidebox->widgets[i].textRect.left = slidebox->widgets[i].bx;
		slidebox->widgets[i].textRect.bottom = slidebox->widgets[i].textRect.top+50;
		slidebox->widgets[i].textRect.right = slidebox->widgets[i].textRect.left+slidebox->widgets[i].bw;
	}
	return slidebox;
}

HeadBarA *HeadBarADataInit(HeadBarA *pHbar)
{
	int err_code;

	pHbar = (HeadBarA*) malloc(sizeof(HeadBarA));
	if(pHbar == NULL){
		gui_error("HeadBarA malloc error\n");
		return NULL;
	}
	memset(pHbar, 0, sizeof(HeadBarA));

	pHbar->sRect.x = 0;
	pHbar->sRect.y = 0;
	pHbar->sRect.w = 800;
	pHbar->sRect.h = 46;

	err_code = LoadBitmapFromFile(HDC_SCREEN, &pHbar->sBg.sBmp, PHOTO_PATH"hbar_bg.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	pHbar->sBg.sRect.x = 0;
	pHbar->sBg.sRect.y = 0;
	pHbar->sBg.sRect.w = pHbar->sBg.sBmp.bmWidth;
	pHbar->sBg.sRect.h = pHbar->sBg.sBmp.bmHeight;


	pHbar->nTextNum = 3;
	pHbar->pText = (TextType *) malloc(sizeof(TextType) * pHbar->nTextNum);
	if(pHbar->pText == NULL){
		gui_error("malloc failed\n");
		goto ERR;
	}
	memset(pHbar->pText, 0, (sizeof(TextType) * pHbar->nTextNum));

	memcpy(pHbar->pText[0].pText, "09:00", strlen("09:00"));
	pHbar->pText[0].sColor = PIXEL_black;
	pHbar->pText[0].sFont = GetLogFont(ID_FONT_SIMSUN_20);
	pHbar->pText[0].sRect.x = 20;
	pHbar->pText[0].sRect.y = 15;
	pHbar->pText[0].sRect.w = 46;
	pHbar->pText[0].sRect.h = 30;


	memcpy(pHbar->pText[1].pText, "9", strlen("9"));
	pHbar->pText[1].sColor = PIXEL_black;
	pHbar->pText[1].sFont = GetLogFont(ID_FONT_SIMSUN_20);
	pHbar->pText[1].sRect.x = 534;
	pHbar->pText[1].sRect.y = 15;
	pHbar->pText[1].sRect.w = 46;
	pHbar->pText[1].sRect.h = 30;


	memcpy(pHbar->pText[2].pText, "30", strlen("30"));
	pHbar->pText[2].sColor = PIXEL_black;
	pHbar->pText[2].sFont = GetLogFont(ID_FONT_SIMSUN_20);
	pHbar->pText[2].sRect.x = 630;
	pHbar->pText[2].sRect.y = 15;
	pHbar->pText[2].sRect.w = 46;
	pHbar->pText[2].sRect.h = 30;


	pHbar->nBmpNum = 4;
	pHbar->pBmp = (BmpType *) malloc(sizeof(BmpType) * pHbar->nBmpNum);
	if(pHbar->pBmp == NULL){
		gui_error("malloc failed\n");
		goto ERR;
	}
	memset(pHbar->pBmp, 0, (sizeof(BmpType) * pHbar->nBmpNum));

	err_code = LoadBitmapFromFile(HDC_SCREEN, &pHbar->pBmp[0].sBmp, PHOTO_PATH"BRIGHT.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	pHbar->pBmp[0].sRect.x = 482;
	pHbar->pBmp[0].sRect.y = 0;
	pHbar->pBmp[0].sRect.w = 46;
	pHbar->pBmp[0].sRect.h = 46;

	err_code = LoadBitmapFromFile(HDC_SCREEN, &pHbar->pBmp[1].sBmp, PHOTO_PATH"VOLUME.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	pHbar->pBmp[1].sRect.x = 578;
	pHbar->pBmp[1].sRect.y = 0;
	pHbar->pBmp[1].sRect.w = 46;
	pHbar->pBmp[1].sRect.h = 46;

	#if 0
	err_code = LoadBitmapFromFile(HDC_SCREEN, &pHbar->pBmp[2].sBmp, PHOTO_PATH"hbar_wifi_signal_0.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	pHbar->pBmp[2].sRect.x = 500;
	pHbar->pBmp[2].sRect.y = 0;
	pHbar->pBmp[2].sRect.w = 46;
	pHbar->pBmp[2].sRect.h = 46;
	#endif

	err_code = LoadBitmapFromFile(HDC_SCREEN, &pHbar->pBmp[2].sBmp, PHOTO_PATH"ENERGY_3.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	pHbar->pBmp[2].sRect.x = 674;
	pHbar->pBmp[2].sRect.y = 0;
	pHbar->pBmp[2].sRect.w = 46;
	pHbar->pBmp[2].sRect.h = 46;

	pHbar->nBtnNum = 1;
	pHbar->pBtn = (ButtonType *) malloc(sizeof(ButtonType) * pHbar->nBtnNum);
	if(pHbar->pBtn == NULL){
		gui_error("malloc failed\n");
		goto ERR;
	}
	memset(pHbar->pBtn, 0, (sizeof(ButtonType) * pHbar->nBtnNum));

	pHbar->pBtn[0].onStus = 0;
	pHbar->pBtn[0].fun = NULL;
	pHbar->pBtn[0].sRect.x = 730;
	pHbar->pBtn[0].sRect.y = 0;
	pHbar->pBtn[0].sRect.w = 70;
	pHbar->pBtn[0].sRect.h = 46;

	err_code = LoadBitmapFromFile(HDC_SCREEN, &pHbar->pBtn[0].onBmp, PHOTO_PATH"hbar_return_fs.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	err_code = LoadBitmapFromFile(HDC_SCREEN, &pHbar->pBtn[0].offBmp, PHOTO_PATH"hbar_return_ufs.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}

	#if 0
	pHbar->pBtn[1].onStus = 0;
	pHbar->pBtn[1].fun = ButtonCallbackFun;
	pHbar->pBtn[1].sRect.x = 330;
	pHbar->pBtn[1].sRect.y = 0;
	pHbar->pBtn[1].sRect.w = 70;
	pHbar->pBtn[1].sRect.h = 46;

	err_code = LoadBitmapFromFile(HDC_SCREEN, &pHbar->pBtn[1].onBmp, PHOTO_PATH"hbar_return_fs.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	err_code = LoadBitmapFromFile(HDC_SCREEN, &pHbar->pBtn[1].offBmp, PHOTO_PATH"hbar_return_ufs.png");
	if (err_code != ERR_BMP_OK)
	{
		gui_error("loadbitmap failed\n");
		goto ERR;
	}
	#endif

	return pHbar;

	ERR:
		HeadBarADataUninit(pHbar);
		return NULL;
}

HeadBarA *HeadBarAFunctionSet(pButtonFun fun)
{
	pHbar->pBtn[0].fun = fun;
	return pHbar;
}

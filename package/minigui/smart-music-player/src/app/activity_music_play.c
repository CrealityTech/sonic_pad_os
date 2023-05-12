/**
 * Project Name: smart-musicplayer
 * File Name: activity_music_play.c
 * Date: 2019-08-19 19:01
 * Author: anruliu
 * Description: Music play window
 * Copyright (c) 2019, Allwinnertech All Rights Reserved.
 */

#include "resource.h"
#include "datafactory.h"

extern HWND ActivityMusicSpectrum(HWND hosting);

static HWND controlButotnViewHwnd;

static void my_notify_proc(HWND hwnd, LINT id, int nc, DWORD add_data) {

    sm_debug("music play button clike hwnd=%p id=%ld\n", hwnd, id);

    ControlButotnData *controlButotnData = NULL;
    controlButotnData = (ControlButotnData*) GetWindowAdditionalData(
            controlButotnViewHwnd);

    if (nc == STN_CLICKED) {
        switch (id) {
        case 0:
            list_get_pre_node(media_list);
            StartMusicPlay();

            if (!controlButotnData->buttonData[1].selectStatus) {
                controlButotnData->buttonData[1].selectStatus = 1;
                UpdateWindow(controlButotnViewHwnd, FALSE);
            }
            break;
        case 1:
            if (!tplayer_getplaying())
                tplayer_play();
            else
                tplayer_pause();
            break;
        case 2:
            list_get_next_node(media_list);
            StartMusicPlay();

            if (!controlButotnData->buttonData[1].selectStatus) {
                controlButotnData->buttonData[1].selectStatus = 1;
                UpdateWindow(controlButotnViewHwnd, FALSE);
            }
            break;
        }
    }
}

static void my_notify_proc1(HWND hwnd, LINT id, int nc, DWORD add_data) {

    sm_debug("music play buttons clike hwnd=%p id=%ld\n", hwnd, id);

    static int volume;

    if (nc == STN_CLICKED) {
        switch (id) {
        case 0:
            volume = tplayer_getvolumn();
            if (volume <= 0)
                break;
            volume = volume - 2;
            tplayer_setvolumn(volume);
            sm_debug("current volume is %d\n", volume);
            break;
        case 1:
            ActivityMusicSpectrum(GetParent(hwnd));
            break;
        case 2:
            volume = tplayer_getvolumn();
            if (volume < 0 || volume >= 40)
                break;
            volume = volume + 2;
            tplayer_setvolumn(volume);
            sm_debug("current volume is %d\n", volume);
            break;
        }
    }
}

static LRESULT ActivityMusicPlayProc(HWND hWnd, UINT nMessage, WPARAM wParam,
        LPARAM lParam) {

    static HWND curTimeHwnd;
    static HWND totalTimeHwnd;

    switch (nMessage) {
    case MSG_CREATE: {
        RECT rect;
        int x, x1, y;
        char totalTime[8];
        char curTime[8];
        GetClientRect(hWnd, &rect);

        HeadbarViewInit(hWnd, 1, 0);
        controlButotnViewHwnd = ControlButotnViewInit(hWnd, 0, my_notify_proc);
        ControlButotnsViewInit(hWnd, my_notify_proc1);
        ProgressViewInit(hWnd);

        GetPlayTime(curTime, totalTime);

        if (rect.right > 240) {
            x = 17;
            x1 = 244;
            y = 440;
        } else {
            x = 14;
            x1 = 190;
            y = 293;
        }

        curTimeHwnd = CreateWindowEx(CTRL_STATIC, curTime,
        WS_CHILD | SS_NOTIFY | SS_SIMPLE | WS_VISIBLE,
        WS_EX_TRANSPARENT, 0, x, y, 60, 30, hWnd, 0);
        SetWindowFont(curTimeHwnd, getLogFont(ID_FONT_TIMES_14));
        SetWindowElementAttr(curTimeHwnd, WE_FGC_WINDOW,
                RGB2Pixel(HDC_SCREEN, 152, 152, 152));

        totalTimeHwnd = CreateWindowEx(CTRL_STATIC, totalTime,
        WS_CHILD | SS_NOTIFY | SS_SIMPLE | WS_VISIBLE,
        WS_EX_TRANSPARENT, 0, x1, y, 60, 30, hWnd, 0);
        SetWindowFont(totalTimeHwnd, getLogFont(ID_FONT_TIMES_14));
        SetWindowElementAttr(totalTimeHwnd, WE_FGC_WINDOW,
                RGB2Pixel(HDC_SCREEN, 152, 152, 152));

        SetTimer(hWnd, ID_TIMER_MUSIC_TIME_UPDATE, 100);

        if (tplayer_getplaying()) {
            ControlButotnData *controlButotnData = NULL;
            controlButotnData = (ControlButotnData*) GetWindowAdditionalData(
                    controlButotnViewHwnd);
            controlButotnData->buttonData[1].selectStatus = 1;
        }
        break;
    }
    case MSG_TIMER: {
        /* Update song playback time */
        char totalTime[8];
        char curTime[8];
        GetPlayTime(curTime, totalTime);
        SetWindowText(curTimeHwnd, curTime);
        SetWindowText(totalTimeHwnd, totalTime);
        break;
    }
    case MSG_PAINT: {
        HDC hdc = BeginPaint(hWnd);
        RECT rect;
        GetClientRect(hWnd, &rect);
        SetBrushColor(hdc, RGB2Pixel(HDC_SCREEN, 204, 40, 71));

        if (rect.right > 240) {
            FillBox(hdc, 0, 0, rect.right, 297);
        } else {
            FillBox(hdc, 0, 0, rect.right, 198);
        }

        EndPaint(hWnd, hdc);
        return 0;
    }
    case MSG_ACTIVITY_MUSIC_LIST_CLOSE: {
        ControlButotnData *controlButotnData = NULL;
        controlButotnData = (ControlButotnData*) GetWindowAdditionalData(
                controlButotnViewHwnd);

        if (!tplayer_getplaying())
            controlButotnData->buttonData[1].selectStatus = 0;
        else
            controlButotnData->buttonData[1].selectStatus = 1;
        UpdateWindow(controlButotnViewHwnd, FALSE);
        break;
    }
    case MSG_DESTROY:
        sm_debug("MSG_DESTROY\n");
        if (IsTimerInstalled(hWnd, ID_TIMER_MUSIC_TIME_UPDATE))
            KillTimer(hWnd, ID_TIMER_MUSIC_TIME_UPDATE);
        DestroyAllControls(hWnd);
        return 0;
    case MSG_CLOSE:
        DestroyMainWindow(hWnd);
        MainWindowThreadCleanup(hWnd);
        return 0;
    }
    return DefaultMainWinProc(hWnd, nMessage, wParam, lParam);
}

HWND ActivityMusicPlay(HWND hosting) {

    MSG Msg;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , SMART_MUSIC_PLAYER , 0 , 0);
#endif

    smRect rect;
    getResRect(ID_SCREEN, &rect);

    CreateInfo.dwStyle = WS_NONE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = ActivityMusicPlayProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = rect.w;
    CreateInfo.by = rect.h;
    CreateInfo.iBkColor = PIXEL_black;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = hosting;

    HWND hMainWnd = CreateMainWindow(&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return HWND_INVALID;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    return hMainWnd;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif


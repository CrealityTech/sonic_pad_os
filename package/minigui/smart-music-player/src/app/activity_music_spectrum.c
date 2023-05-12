/*
 * activity_music_spectrum.c
 *
 * Show music spectrum window
 *
 *  Created on: 2020/6/12
 *      Author: anruliu
 */

#include "resource.h"
#include "datafactory.h"
#include <mgplus/mgplus.h>
#include "math.h"

#define PI 3.1415926535

static HPATH path;
static RECT rect;

/*
 * @param pr The real part of n sampled inputs, returns the discrete fft
 * @param pi The imaginary part of n sampled inputs
 * @param n Sampling points
 * @param k n=2^k
 * @param fr Return n real parts of discrete fft
 * @param fi Return the n imaginary parts of the discrete fft
 */
static void kfft(pr, pi, n, k, fr, fi)
    int n, k;double pr[], pi[], fr[], fi[]; {
    int it, m, is, i, j, nv, l0;
    double p, q, s, vr, vi, poddr, poddi;
    /* Assign pr[0] and pi[0] to fr[] and fi[] cyclically */
    for (it = 0; it <= n - 1; it++) {
        m = it;
        is = 0;
        for (i = 0; i <= k - 1; i++) {
            j = m / 2;
            is = 2 * is + (m - 2 * j);
            m = j;
        }
        fr[it] = pr[is];
        fi[it] = pi[is];
    }
    pr[0] = 1.0;
    pi[0] = 0.0;
    p = 6.283185306 / (1.0 * n);
    /* Express w=e^-j2pi/n with Euler's formula */
    pr[1] = cos(p);
    pi[1] = -sin(p);

    /* Calculate pr[] */
    for (i = 2; i <= n - 1; i++) {
        p = pr[i - 1] * pr[1];
        q = pi[i - 1] * pi[1];
        s = (pr[i - 1] + pi[i - 1]) * (pr[1] + pi[1]);
        pr[i] = p - q;
        pi[i] = s - p - q;
    }
    for (it = 0; it <= n - 2; it = it + 2) {
        vr = fr[it];
        vi = fi[it];
        fr[it] = vr + fr[it + 1];
        fi[it] = vi + fi[it + 1];
        fr[it + 1] = vr - fr[it + 1];
        fi[it + 1] = vi - fi[it + 1];
    }
    m = n / 2;
    nv = 2;
    /* Butterfly operation */
    for (l0 = k - 2; l0 >= 0; l0--) {
        m = m / 2;
        nv = 2 * nv;
        for (it = 0; it <= (m - 1) * nv; it = it + nv)
            for (j = 0; j <= (nv / 2) - 1; j++) {
                p = pr[m * j] * fr[it + j + nv / 2];
                q = pi[m * j] * fi[it + j + nv / 2];
                s = pr[m * j] + pi[m * j];
                s = s * (fr[it + j + nv / 2] + fi[it + j + nv / 2]);
                poddr = p - q;
                poddi = s - p - q;
                fr[it + j + nv / 2] = fr[it + j] - poddr;
                fi[it + j + nv / 2] = fi[it + j] - poddi;
                fr[it + j] = fr[it + j] + poddr;
                fi[it + j] = fi[it + j] + poddi;
            }
    }
    for (i = 0; i <= n - 1; i++) {
        /* Amplitude calculation */
        pr[i] = sqrt(fr[i] * fr[i] + fi[i] * fi[i]);
    }
    return;
}

static void runFft(char buf[], int size, HPATH path) {
    int i, j, k;
    double pr[size], pi[size], fr[size], fi[size], t[size];
    for (i = 0; i < size; i++) {
        /* Generate input signal */
        //t[i] = i * 0.001;
        t[i] = buf[i] * 0.001;
        pr[i] = 1.2 + 2.7 * cos(2 * PI * 33 * t[i])
                + 5 * cos(2 * PI * 200 * t[i] + PI / 2);
        //pr[i] = buf[i];
        pi[i] = 0.0;
    }

    kfft(pr, pi, size, 7, fr, fi);

    k = 0;
    for (i = (size / 2 - 17); i < (size / 2 + 16); i++) {
        //sm_debug("%d\t%lf\n", i, pr[i]);
        pr[i] = pr[i] * 2;
        if (pr[i] > 255)
            pr[i] = 255;
        if (rect.right > 240)
            MGPlusPathAddRectangle(path, 10 * k + 2, 90 + 255 - pr[i], 6, pr[i]);
        else
            MGPlusPathAddRectangle(path, 8 * k + 2, 55 + 255 - pr[i], 4, pr[i]);
        k++;
    }

    isAudioPcmDataCopy = FALSE;
}

/* The program did not create the thread */
static void *FtfThreadProc(void *arg) {
    isActivityMusicSpectrumOpen = TRUE;
    isAudioPcmDataCopy = FALSE;

    while (1) {
        if (!isActivityMusicSpectrumOpen) {
            sm_error("ftf thread end\n");
            pthread_exit((void*) 1);
        }
        pthread_mutex_lock(&spectrumMutex);

        if (!isActivityMusicSpectrumOpen) {
            pthread_mutex_unlock(&spectrumMutex);
            usleep(16 * 1000);/* Delay 16ms */
            continue;
        } else if (isAudioPcmDataCopy) {
            runFft(g_AudioPcmData, PCM_DATA_SIZE, path);
            /* InvalidateRect no wait for the update to complete the UI before returning */
            InvalidateRect((HWND) arg, NULL, TRUE);
            /* UpdateWindow will wait for the update to complete the UI before returning */
            /* UpdateWindow((HWND) arg, TRUE); */
        }

        pthread_mutex_unlock(&spectrumMutex);
        usleep(60 * 1000);/* Delay 60ms */
    }
    sm_error("Ftf thread end fail\n");
    return (void*) 0;
}

static LRESULT ActivityMusicSpectrumProc(HWND hWnd, UINT nMessage,
        WPARAM wParam, LPARAM lParam) {

    static BITMAP backgroundBmp;
    static HBRUSH brush;
    static pthread_t threadID;

    switch (nMessage) {
    case MSG_CREATE: {
        int ret;
        RECT arect;
        GetClientRect(hWnd, &rect);
        HeadbarViewInit(hWnd, 0, 8);

        setCurrentIconValue(ID_BACKGROUND_BMP, 0);
        getResBmp(ID_BACKGROUND_BMP, BMPTYPE_BASE, &backgroundBmp);

        pthread_mutex_init(&spectrumMutex, NULL);
        memset((void *) g_AudioPcmData, 0, PCM_DATA_SIZE * sizeof(char));

        /* Create path selection odd-even winding rules */
        path = MGPlusPathCreate(MP_PATH_FILL_MODE_WINDING);
        brush = MGPlusBrushCreate(MP_BRUSH_TYPE_LINEARGRADIENT);
        MGPlusSetLinearGradientBrushMode(brush,
                MP_LINEAR_GRADIENT_MODE_VERTICAL);

        if (rect.right > 240) {
            arect.left = 0;
            arect.top = 90;
            arect.right = 320;
            arect.bottom = 344;
        } else {
            arect.left = 0;
            arect.top = 55;
            arect.right = 240;
            arect.bottom = 310;
        }
        MGPlusSetLinearGradientBrushRect(brush, &arect);

        /* Set gradient color */
        ARGB color[2] = { 0xFFFF0000, 0xFF0000FF };
        MGPlusSetLinearGradientBrushColors(brush, color, 2);

        ret = pthread_create(&threadID, NULL, FtfThreadProc, (void *) hWnd);
        if (ret == -1) {
            sm_error("create ftf thread fail\n");
        }
        break;
    }
    case MSG_PAINT: {
        HDC hdc = BeginPaint(hWnd);
        HGRAPHICS graphics = MGPlusGraphicCreateWithoutCanvas(hdc);

        MGPlusFillPath(graphics, brush, path);

        MGPlusPathReset(path);
        MGPlusGraphicDelete(graphics);

        EndPaint(hWnd, hdc);
        return 0;
    }
    case MSG_ERASEBKGND: {
        HDC hdc = (HDC) wParam;
        const RECT* clip = (const RECT*) lParam;
        BOOL fGetDC = FALSE;
        RECT rcTemp;
        if (hdc == 0) {
            hdc = GetClientDC(hWnd);
            fGetDC = TRUE;
        }
        if (clip) {
            rcTemp = *clip;
            ScreenToClient(hWnd, &rcTemp.left, &rcTemp.top);
            ScreenToClient(hWnd, &rcTemp.right, &rcTemp.bottom);
            IncludeClipRect(hdc, &rcTemp);
        }
        FillBoxWithBitmap(hdc, 0, 0, 0, 0, &backgroundBmp);
        if (fGetDC)
            ReleaseDC(hdc);
        return 0;
    }
    case MSG_DESTROY:
        sm_debug("MSG_DESTROY\n");

        isActivityMusicSpectrumOpen = FALSE;
        pthread_join(threadID, NULL);
        threadID = 0;

        pthread_mutex_destroy(&spectrumMutex);

        MGPlusPathDelete(path);
        MGPlusBrushDelete(brush);

        unloadBitMap(&backgroundBmp);
        DestroyAllControls(hWnd);

        return 0;
    case MSG_CLOSE:
        DestroyMainWindow(hWnd);
        MainWindowThreadCleanup(hWnd);
        return 0;
    }
    return DefaultMainWinProc(hWnd, nMessage, wParam, lParam);
}

HWND ActivityMusicSpectrum(HWND hosting) {

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
    CreateInfo.MainWindowProc = ActivityMusicSpectrumProc;
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


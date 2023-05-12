#ifndef _PLAYER_INT_H_
#define _PLAYER_INT_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <errno.h>
#include <sys/select.h>
#include <allwinner/tplayer.h>
#include "dbList.h"

typedef enum
{
	DEFAULAT_CMD = 0,
	INIT_CMD = 1,
	EXIT_CMD = 2,
	PREPARE_CMD = 3,
	PLAY_CMD = 4,
	PAUSE_CMD = 5,
	STOP_CMD = 6,
	SEEK_TO = 7,
	SETTING,
}playerCmd;

enum playerSetParam
{
	SET_VOLUME,
	SET_LOOP,
	SET_SCALE,
	SET_DISPLAY,
	SET_SPEED,
	SET_ROTATE,
};

typedef enum
{
	DEFAULT_STATUS,
	INIT_STATUS,
	EXIT_STATUS,
	PREPARE_STATUS,
	PAUSE_STATUS,
	STOP_STATUS,
	PLAY_STATUS,
	SEEKTO_STATUS,
	SETTING_STATUS,
}playerStatus;

#define QUEUE_MAX_SIZE    10

typedef struct PLAYER_CONTEXT_T
{
    TPlayer*          mTPlayer;
    int               mSeekable;
    int               mError;
    int               mVideoFrameNum;
    bool              mPreparedFlag;
    bool              Preparing;
	bool			  mCompleteFlag;
    MediaInfo*		  mMediaInfo;
	playerStatus	  mpstaus;
	pthread_t		  id;
	pthread_mutex_t   mutex;
    sem_t             mPreparedSem;
	TPlayerNotifyCallback callback;
	db_list_t*		  queue_head;
}player_context_t;


struct player_param{
	player_context_t *tplayer;
	playerCmd cmd;
	int param[10];
};

int tplayer_init(void);
int tplayer_exit(void);
int tplayer_play_url(const char *path);
int tplayer_play(void);
int tplayer_pause(void);
int tplayer_prepared(void);
int tplayer_IsPlaying(void);
int tplayer_seekto(int nSeekTimeMs);
int tplayer_stop(void);
int tplayer_volume(int volume);
int tplayer_setspeed(TplayerPlaySpeedType nSpeed);
int tplayer_setlooping( bool bLoop);
int tplayer_setscaledown(TplayerVideoScaleDownType nHorizonScaleDown, TplayerVideoScaleDownType nVerticalScaleDown);
int tplayer_setrotate(TplayerVideoRotateType rotateDegree);
int tplayer_getduration(int* msec);
int tplayer_getcurrentpos(int* msec);
int tplayer_getcompletestate(void);
int tplayer_setdisplayrect(int x, int y, unsigned int width, unsigned int height);
void media_player_uevent(void *param);
player_context_t *tplayer_pthread_create(void);
void tplayer_pthread_destory(player_context_t *tplayer);
void tplayer_set_callback(player_context_t* pPlayer, TPlayerNotifyCallback callback);

#endif

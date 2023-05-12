#include "player_int.h"
#include "debug.h"
#include "dbList.h"

#define CEDARX_UNUSE(param) (void)param
#define ISNULL(x) if(!x){return -1;}

#define		LOCK

#ifdef LOCK
#define	MUTEX_LOCK(mtx)			if(pthread_mutex_lock(&mtx)){\
									printf("%s:%d mutex lock fail\n",__func__,__LINE__);\
								}
#define	MUTEX_UNLOCK(mtx)		if(pthread_mutex_unlock(&mtx)){\
									printf("%s:%d mutex unlock fail\n",__func__,__LINE__);\
								}
#else
#define MUTEX_LOCK(mtx)
#define MUTEX_UNLOCK(mtx)
#endif
void tplayer_set_callback(player_context_t* pPlayer, TPlayerNotifyCallback callback)
{
	pPlayer->callback = callback;
}

//* a callback for tplayer.
static int CallbackForTPlayer(void* pUserData, int msg, int param0, void* param1)
{
    player_context_t* pPlayer = (player_context_t*)pUserData;

    CEDARX_UNUSE(param1);

	if(pPlayer->callback){
		pPlayer->callback(pUserData, msg, param0, param1);
	}

    switch(msg){
    case TPLAYER_NOTIFY_PREPARED:
    {
        printf("TPLAYER_NOTIFY_PREPARED,has prepared.\n");
		pPlayer->mPreparedFlag = 1;
        sem_post(&pPlayer->mPreparedSem);
        break;
    }
    case TPLAYER_NOTIFY_PLAYBACK_COMPLETE:
    {
        printf("TPLAYER_NOTIFY_PLAYBACK_COMPLETE\n");
        pPlayer->mCompleteFlag = 1;
        break;
    }
    case TPLAYER_NOTIFY_SEEK_COMPLETE:
    {
        printf("TPLAYER_NOTIFY_SEEK_COMPLETE>>>>info: seek ok.\n");
        break;
    }
    case TPLAYER_NOTIFY_MEDIA_ERROR:
    {
        switch (param0)
        {
            case TPLAYER_MEDIA_ERROR_UNKNOWN:
            {
                printf("erro type:TPLAYER_MEDIA_ERROR_UNKNOWN\n");
                break;
            }
            case TPLAYER_MEDIA_ERROR_UNSUPPORTED:
            {
                printf("erro type:TPLAYER_MEDIA_ERROR_UNSUPPORTED\n");
                break;
            }
            case TPLAYER_MEDIA_ERROR_IO:
            {
                printf("erro type:TPLAYER_MEDIA_ERROR_IO\n");
                break;
            }
        }
		pPlayer->mError = 1;
		pPlayer->mPreparedFlag = 0;
        sem_post(&pPlayer->mPreparedSem);
        printf("error: open media source fail.\n");
        break;
    }
    case TPLAYER_NOTIFY_NOT_SEEKABLE:
    {
        pPlayer->mSeekable = 0;
        printf("info: media source is unseekable.\n");
        break;
    }
	case TPLAYER_NOTIFY_BUFFER_START:
    {
        printf("have no enough data to play\n");
        break;
    }
	case TPLAYER_NOTIFY_BUFFER_END:
    {
        printf("have enough data to play again\n");
        break;
    }
    case TPLAYER_NOTIFY_VIDEO_FRAME:
    {
        //printf("get the decoded video frame\n");
        break;
    }
    case TPLAYER_NOTIFY_AUDIO_FRAME:
    {
        //printf("get the decoded audio frame\n");
        break;
    }
    case TPLAYER_NOTIFY_SUBTITLE_FRAME:
    {
        //printf("get the decoded subtitle frame\n");
        break;
    }
    default:
    {
        printf("warning: unknown callback from Tinaplayer.\n");
        break;
    }
    }
    return 0;
}

static int tplayer_queue(void *param)
{
	struct player_param *p = (struct player_param *)param;
	player_context_t *tplayer = p->tplayer;
	playerCmd cmd = p->cmd;

	if(tplayer->mTPlayer == NULL && cmd != INIT_CMD){
		goto end;
	}

	switch(cmd){
		case INIT_CMD:
			if(tplayer->mTPlayer != NULL){
				debug_warn("tplayer is already created\n");
				goto end;
			}
			tplayer->mTPlayer = TPlayerCreate(CEDARX_PLAYER);
			if(tplayer->mTPlayer == NULL){
				debug_err("can not create tplayer, quit.\n");
				goto end;
			}

			tplayer->mError = 0;
			tplayer->mSeekable = 1;
			tplayer->mPreparedFlag = 0;
			tplayer->mMediaInfo = NULL;
			tplayer->mCompleteFlag = 0;

			TPlayerSetNotifyCallback(tplayer->mTPlayer,CallbackForTPlayer, (void*)tplayer);
			TPlayerReset(tplayer->mTPlayer);
			TPlayerSetDebugFlag(tplayer->mTPlayer, 0);
			debug_info("init finished\n");
			break;
		case EXIT_CMD:

			debug_info("");
			TPlayerStop(tplayer->mTPlayer);
			TPlayerReset(tplayer->mTPlayer);

			debug_info("");
			TPlayerDestroy(tplayer->mTPlayer);
			tplayer->mTPlayer = NULL;
			tplayer->mPreparedFlag = 0;
			tplayer->mCompleteFlag = 0;
			debug_info("exit finished\n");
			break;
		case PREPARE_CMD:

			TPlayerStop(tplayer->mTPlayer);
			TPlayerReset(tplayer->mTPlayer);

			tplayer->mPreparedFlag = 0;
			tplayer->mCompleteFlag = 0;
			if(TPlayerSetDataSource(tplayer->mTPlayer, (const char *)p->param[0], (CdxKeyedVectorT *)p->param[1])){
				 goto end;
			}else{
				debug_warn("setDataSource end\n");
			}

			if(TPlayerPrepareAsync(tplayer->mTPlayer) != 0){
				debug_warn("TPlayerPrepareAsync() return fail.\n");

			}

			sem_wait(&tplayer->mPreparedSem);
			if(!tplayer->mPreparedFlag){
				debug_warn("TPlayerPrepareAsync() return fail.\n");
				goto end;
			}
			tplayer->mMediaInfo = TPlayerGetMediaInfo(tplayer->mTPlayer);
			debug_info("prepared finished\n");
			break;
		case PAUSE_CMD:
			if(!TPlayerIsPlaying(tplayer->mTPlayer)){
				debug_warn("not playing!\n");
				goto end;
			}
			TPlayerPause(tplayer->mTPlayer);
			debug_info("pause finished\n");

			break;
		case STOP_CMD:
			tplayer->mPreparedFlag = 0;
			TPlayerStop(tplayer->mTPlayer);

			debug_info("stop finished\n");
			break;
		case PLAY_CMD:
			if(!tplayer->mPreparedFlag){
				debug_warn("not prepared!\n");
				goto end;
			}

			if(TPlayerIsPlaying(tplayer->mTPlayer)){
				debug_warn("already palying!\n");
				goto end;
			}
			tplayer->mCompleteFlag = 0;
			TPlayerStart(tplayer->mTPlayer);

			debug_info("play finished\n");
			break;
		case SEEK_TO:
			if(!tplayer->mPreparedFlag){
				debug_warn("not prepared!\n");
				goto end;
			}
			TPlayerSeekTo(tplayer->mTPlayer, (int)p->param[0]);
			break;
		case SETTING:
			switch((int)p->param[0]){
				case SET_LOOP: TPlayerSetLooping(tplayer->mTPlayer, (bool)p->param[1]);	break;
				case SET_SCALE: TPlayerSetScaleDownRatio(tplayer->mTPlayer,
					(int)p->param[1], (int)p->param[2]);
					break;
				case SET_DISPLAY: TPlayerSetDisplayRect(tplayer->mTPlayer,
					(int)p->param[1], (int)p->param[2],
					(unsigned int)p->param[3], (unsigned int)p->param[4]);
					break;
				case SET_ROTATE: TPlayerSetRotate(tplayer->mTPlayer, (TplayerVideoRotateType)p->param[1]); break;
				case SET_VOLUME: TPlayerSetVolume(tplayer->mTPlayer, (int)p->param[1]); break;
				case SET_SPEED: TPlayerSetSpeed(tplayer->mTPlayer, (TplayerPlaySpeedType)p->param[1]); break;
				default: break;
			}
			break;
		default:
			break;
	}
end:
	return 0;
}


static void tplayer_add_queue(struct player_param *param)
{
	int queue_size = 0;

	MUTEX_LOCK(param->tplayer->mutex);
	__db_list_put_tail(param->tplayer->queue_head, param);
	queue_size = param->tplayer->queue_head->limit_size;
	MUTEX_UNLOCK(param->tplayer->mutex);
//if queue too many, it will wait.
#if 0
	while(queue_size > QUEUE_MAX_SIZE){
		debug_warn("queue too many, please wait...");
		usleep(10000);
		MUTEX_LOCK(param->tplayer->mutex);
		queue_size = param->tplayer->queue_head->limit_size;
		MUTEX_UNLOCK(param->tplayer->mutex);
	}
#endif
}

static void *tplayer_pthread(void *arg)
{
	player_context_t *tplayer = (player_context_t *)arg;
	struct player_param *queue;
	int empty;

	while(1){

		MUTEX_LOCK(tplayer->mutex);
		empty = is_list_empty(tplayer->queue_head);
		MUTEX_UNLOCK(tplayer->mutex);

		if(empty){
			usleep(100000);
			continue;
		}

		MUTEX_LOCK(tplayer->mutex);
		queue = (struct player_param *)__db_list_pop(tplayer->queue_head);
		MUTEX_UNLOCK(tplayer->mutex);

		debug_info("cmd = %d\n", queue->cmd);
		tplayer_queue(queue);
		if(queue->cmd == PREPARE_CMD){
			tplayer->Preparing = 0;
		}
		tplayer->mpstaus = DEFAULT_STATUS;
		free(queue);
	}
    pthread_exit(NULL);
    return NULL;
}

static player_context_t *player_context = NULL;

player_context_t *tplayer_pthread_create(void)
{
	player_context_t *tplayer;
	int result;

	tplayer = (player_context_t *)malloc(sizeof(player_context_t));

	if(tplayer == NULL){
		debug_err("tplayer failed");
		goto end;
	}

	memset(tplayer, 0, sizeof(player_context_t));

	tplayer->queue_head = db_list_create();

	sem_init(&tplayer->mPreparedSem, 0, 0);
#ifdef	LOCK
	result = pthread_mutex_init(&tplayer->mutex, NULL);
	if(result != 0){
			debug_err("pthread mutex init failed!\r\n");
			goto end;
	}

#endif
	result = pthread_create(&tplayer->id, NULL, tplayer_pthread, (void*)tplayer);
	if(result != 0){
		debug_err("pthread create fail!\r\n");
		free(tplayer);
		tplayer = NULL;
		goto end;
	}
end:
	player_context = tplayer;
	return tplayer;
}

void tplayer_pthread_destory(player_context_t *tplayer)
{
	if(!tplayer){
		return;
	}

	pthread_cancel(tplayer->id);
	pthread_join(tplayer->id, NULL);
#ifdef	LOCK
	pthread_mutex_destroy(&tplayer->mutex);
#endif
	__db_list_destory(tplayer->queue_head);
	sem_destroy(&tplayer->mPreparedSem);
	free(tplayer);
	tplayer = NULL;
}


CdxKeyedVectorT http_pHeaders;

char Headers_key[256] = { 0 };
char Headers_val[256] = { 0 };

int tplayer_init(void)
{
	struct player_param *param;

	ISNULL(player_context);
	param = (struct player_param *)malloc(sizeof(struct player_param));

	debug_info("%p\n", param);
	if(!param){
		debug_err("malloc failed!!!");
		return -1;
	}

	param->tplayer = player_context;
	param->cmd = INIT_CMD;
	tplayer_add_queue(param);
	param->tplayer->mpstaus = INIT_STATUS;
	return 0;
}

int tplayer_exit(void)
{
	struct player_param *param;

	ISNULL(player_context);
	param = (struct player_param *)malloc(sizeof(struct player_param));
	debug_info("%p\n", param);
	if(!param){
		debug_err("malloc failed!!!");
		return -1;
	}

	param->tplayer = player_context;
	param->cmd = EXIT_CMD;
	tplayer_add_queue(param);
	param->tplayer->mpstaus = EXIT_STATUS;
	return 0;
}

int tplayer_play_url(const char *path)
{
	struct player_param *param;

	ISNULL(player_context);
	param = (struct player_param *)malloc(sizeof(struct player_param));

	debug_info("%p\n", param);
	if(!param){
		debug_err("malloc failed!!!");
		return -1;
	}

	param->tplayer = player_context;
	param->cmd = PREPARE_CMD;
	param->param[0] = (unsigned int)path;

#if 1
	if(param->tplayer->Preparing){
		debug_err("this is invaild, last player is preparing. please wait...");
		free(param);
		return -1;
	}
#endif

	param->tplayer->mpstaus = PREPARE_STATUS;
	param->tplayer->Preparing = 1;

#if 0
	strcpy(Headers_key,"Referer");
	strcpy(Headers_val,"www.gtzdb.com");
	http_pHeaders.item->key = Headers_key;
	http_pHeaders.item->val = Headers_val;
	http_pHeaders.size = 1;
	http_pHeaders.index = 0;
	param->param[1] = (unsigned int)&http_pHeaders;
#else
	param->param[1] = 0;
#endif

	tplayer_add_queue(param);
	return 0;
}

int tplayer_play(void)
{
	struct player_param *param;

	ISNULL(player_context);
	param = (struct player_param *)malloc(sizeof(struct player_param));
	if(!param){
		debug_err("malloc failed!!!");
		return -1;
	}

	param->tplayer = player_context;
	param->cmd = PLAY_CMD;
	tplayer_add_queue(param);
	param->tplayer->mpstaus = PLAY_STATUS;
	return 0;
}

int tplayer_pause(void)
{
	struct player_param *param;

	ISNULL(player_context);
	param = (struct player_param *)malloc(sizeof(struct player_param));
	debug_info("%p\n", param);

	if(!param){
		debug_err("malloc failed!!!");
		return -1;
	}

	param->tplayer = player_context;
	param->cmd = PAUSE_CMD;
	tplayer_add_queue(param);
	param->tplayer->mpstaus = PAUSE_STATUS;

	return 0;
}

int tplayer_seekto(int nSeekTimeMs)
{
	struct player_param *param;


	ISNULL(player_context);
	param = (struct player_param *)malloc(sizeof(struct player_param));
	if(!param){
		debug_err("malloc failed!!!");
		return -1;
	}

	param->tplayer = player_context;
	param->cmd = SEEK_TO;
	param->param[0] = nSeekTimeMs;
	tplayer_add_queue(param);
	param->tplayer->mpstaus = SEEKTO_STATUS;
	//sem_wait(&player_context.cmdSem);
	return 0;
}

int tplayer_stop(void)
{
	struct player_param *param;

	ISNULL(player_context);
	param = (struct player_param *)malloc(sizeof(struct player_param));
	debug_info("%p\n", param);

	if(!param){
		debug_err("malloc failed!!!");
		return -1;
	}

	param->tplayer = player_context;
	param->cmd = STOP_CMD;
	tplayer_add_queue(param);
	param->tplayer->mpstaus = STOP_STATUS;
	return 0;
}

int tplayer_volume(int volume)
{
	struct player_param *param;

	ISNULL(player_context);
	param = (struct player_param *)malloc(sizeof(struct player_param));
	if(!param){
		debug_err("malloc failed!!!");
		return -1;
	}

	param->tplayer = player_context;
	param->cmd = SETTING;
	param->param[0] = SET_VOLUME;
	param->param[1] = volume;
	tplayer_add_queue(param);
	param->tplayer->mpstaus = SETTING_STATUS;
	return 0;
}

int tplayer_setlooping(bool bLoop)
{
	struct player_param *param;

	ISNULL(player_context);
	param = (struct player_param *)malloc(sizeof(struct player_param));
	if(!param){
		debug_err("malloc failed!!!");
		return -1;
	}

	param->tplayer = player_context;
	param->cmd = SETTING;
	param->param[0] = SET_LOOP;
	param->param[1] = bLoop;
	tplayer_add_queue(param);
	param->tplayer->mpstaus = SETTING_STATUS;
	return 0;
}

int tplayer_setscaledown(TplayerVideoScaleDownType nHorizonScaleDown, TplayerVideoScaleDownType nVerticalScaleDown)
{
	struct player_param *param;

	ISNULL(player_context);
	param = (struct player_param *)malloc(sizeof(struct player_param));
	if(!param){
		debug_err("malloc failed!!!");
		return -1;
	}

	param->tplayer = player_context;
	param->cmd = SETTING;
	param->param[0] = SET_SCALE;
	param->param[1] = nHorizonScaleDown;
	param->param[2] = nVerticalScaleDown;
	tplayer_add_queue(param);
	param->tplayer->mpstaus = SETTING_STATUS;
	return 0;
}

int tplayer_setdisplayrect(int x, int y, unsigned int width, unsigned int height)
{
	struct player_param *param;

	ISNULL(player_context);
	param = (struct player_param *)malloc(sizeof(struct player_param));
	if(!param){
		debug_err("malloc failed!!!");
		return -1;
	}

	param->tplayer = player_context;
	param->cmd = SETTING;
	param->param[0] = SET_DISPLAY;
	param->param[1] = x;
	param->param[2] = y;
	param->param[3] = width;
	param->param[4] = height;
	tplayer_add_queue(param);
	param->tplayer->mpstaus = SETTING_STATUS;
	return 0;
}

int tplayer_setspeed(TplayerPlaySpeedType nSpeed)
{
	struct player_param *param;

	ISNULL(player_context);
	param = (struct player_param *)malloc(sizeof(struct player_param));
	if(!param){
		debug_err("malloc failed!!!");
		return -1;
	}

	param->tplayer = player_context;
	param->cmd = SETTING;
	param->param[0] = SET_SPEED;
	param->param[1] = nSpeed;
	tplayer_add_queue(param);
	param->tplayer->mpstaus = SETTING_STATUS;
	return 0;
}

int tplayer_setrotate(TplayerVideoRotateType rotateDegree)
{
	struct player_param *param;

	ISNULL(player_context);
	param = (struct player_param *)malloc(sizeof(struct player_param));
	if(!param){
		debug_err("malloc failed!!!");
		return -1;
	}

	param->tplayer = player_context;
	param->cmd = SETTING;
	param->param[0] = SET_ROTATE;
	param->param[1] = rotateDegree;
	tplayer_add_queue(param);
	param->tplayer->mpstaus = SETTING_STATUS;
	return 0;
}

int tplayer_prepared(void)
{
	ISNULL(player_context);
	return player_context->mPreparedFlag;
}

int tplayer_IsPlaying(void)
{
	ISNULL(player_context);
	ISNULL(player_context->mTPlayer);
	return TPlayerIsPlaying(player_context->mTPlayer);
}

int tplayer_getduration(int* msec)
{
	ISNULL(player_context);
	ISNULL(player_context->mTPlayer);
	if(!player_context->mPreparedFlag){
		debug_info("not prepared!\n");
		return 0;
	}
	return TPlayerGetDuration(player_context->mTPlayer, msec);
}

int tplayer_getcurrentpos(int* msec)
{
	ISNULL(player_context);
	ISNULL(player_context->mTPlayer);
	if(!player_context->mPreparedFlag){
		debug_info("not prepared!\n");
		return 0;
	}
	return TPlayerGetCurrentPosition(player_context->mTPlayer, msec);
}

int tplayer_getcompletestate(void)
{
	if(!player_context){
		return 0;
	}
	if(!player_context->mPreparedFlag){
		return 0;
	}
	return  player_context->mCompleteFlag;
}

#ifndef __VIDEO_LIST_H__
#define __VIDEO_LIST_H__

#include "middle_inc.h"
#include "winInclude.h"

enum video_uevent
{
	VIDEO_INIT,
	VIDEO_URL,
	VIDEO_PLAY,
	VIDEO_PAUSE,
	VIDEO_EXIT,

};

int GetVideoNextIndex(void);
int StartPlayVideo(void);

#define MAXWIDTH  1280
#define MAXHEIGHT 720

#endif

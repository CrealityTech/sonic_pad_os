#ifndef __SST_LOG_H__
#define __SST_LOG_H__

#if defined(ANDROID_STYLE)
#include <android/log.h>
#include <cutils/log.h>

#undef LOG_TAG
#define LOG_TAG "sst_storage"

#define SST_DBG_ERR(fmt, args...) ALOGE(fmt, ## args)
#define SST_DBG_INFO(fmt, args...) ALOGD(fmt, ## args)
#else

#define SST_DBG_ERR(fmt, args...) printf(fmt, ## args)

#if SST_DEBUG_ON
#define SST_DBG_INFO(fmt, args...) printf(fmt, ## args)
#else
#define SST_DBG_INFO(fmt, ...)  ({})
#endif
#endif

#endif /* end of include guard: __SST_LOG_H__ */

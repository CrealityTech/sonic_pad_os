#ifndef  _CRMUTEX_H
#define _CRMUTEX_H


/********

    跨平台

    USING_OS_LINUX
    USING_OS_WINDOWS
    USING_OS_MOC



***************/

#ifndef USING_OS_LINUX
#define USING_OS_LINUX
#endif


#ifdef USING_OS_LINUX

#include <pthread.h>
typedef pthread_mutex_t CrPthreadMutex_t;
typedef pthread_mutexattr_t CrPthreadMutexAttr_t;

#elif USING_OS_RTTHREAD

#include <pthread.h>
typedef pthread_mutex_t CrPthreadMutex_t;
typedef pthread_mutexattr_t CrPthreadMutexAttr_t;

#endif

#ifdef __cplusplus
extern "C" {
#endif


unsigned int CrPthreadMutexInit(CrPthreadMutex_t  *Mutex, const CrPthreadMutexAttr_t *MutexAttr);
unsigned int CrPthreadMutexLock(CrPthreadMutex_t  *Mutex);
unsigned int CrPthreadMutexUnLock(CrPthreadMutex_t *Mutex);
unsigned int CrPthreadMutexDestroy(CrPthreadMutex_t *Mutex);


#ifdef __cplusplus
}
#endif



#endif
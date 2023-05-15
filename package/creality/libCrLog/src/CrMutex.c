
#include "CrMutex.h"




inline unsigned int CrPthreadMutexInit(CrPthreadMutex_t  *Mutex, const CrPthreadMutexAttr_t *MutexAttr)
{
#ifdef USING_OS_LINUX

    return pthread_mutex_init(Mutex, MutexAttr);

#endif
}

unsigned int CrPthreadMutexLock(CrPthreadMutex_t  *Mutex)
{
#ifdef USING_OS_LINUX

    return pthread_mutex_lock(Mutex);

#endif
}

inline unsigned int CrPthreadMutexUnLock(CrPthreadMutex_t *Mutex)
{
#ifdef USING_OS_LINUX

    return pthread_mutex_unlock(Mutex);

#endif
}

inline unsigned int CrPthreadMutexDestroy(CrPthreadMutex_t *Mutex)
{
#ifdef USING_OS_LINUX

    return pthread_mutex_destroy(Mutex);

#endif
}



















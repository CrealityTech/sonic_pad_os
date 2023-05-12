#include <stdio.h>
#ifdef CONF_OMX_ENABLE_EXTERN_MEM
#include <ion_mem_alloc.h>

#define OMX_GetMemAdapterOpsS() \
        GetMemAdapterOpsS()

#define OMX_MemPalloc(memops, nSize)         \
        SunxiMemPalloc(memops, nSize)

#define OMX_MemPfree(memops, pMem)          \
        SunxiMemPfree(memops, pMem)

#define OMX_MemPallocSecure(memops, nSize) \
        SunxiMemPallocSecure(memops, nSize)

#define OMX_MemOpen(memops)           \
        SunxiMemOpen(memops)

#define OMX_MemClose(memops)          \
        SunxiMemClose(memops)

#define OMX_MemFlushCache(memops, pMem, nSize)  \
        SunxiMemFlushCache(memops, pMem, nSize)

#define OMX_MemTotalSize(memops)   \
        SunxiMemTotalSize(memops)

#define OMX_MemCopy(memops, pMemDst, pMemSrc, nSize) \
        SunxiMemCopy(memops, pMemDst, pMemSrc, nSize)

#define OMX_MemRead(memops, pMemDst, pMemSrc, nSize) \
        SunxiMemRead(memops, pMemDst, pMemSrc, nSize)

#define OMX_MemWrite(memops, pMemDst, pMemSrc, nSize) \
        SunxiMemWrite(memops, pMemDst, pMemSrc, nSize)

#define OMX_MemGetPhysicAddress(memops, virt) \
        SunxiMemGetPhysicAddressCpu(memops, virt)

#define OMX_MemGetVirtualAddress(memops, phy) \
        SunxiMemGetVirtualAddressCpu(memops, phy)

#define OMX_MemSetup(memops) \
        SunxiMemSetup(memops)

#define OMX_MemShutdown(memops) \
        SunxiMemShutdown(memops)

#define OMX_MemGetHandleByVirtualAddr(memops, virt) \
        SunxiMemGetBufferFd(memops, virt)


#define OMX_MemGetVirtualAddrByHandle(memops, int) \
        SunxiMemGetViraddrByFd(memops, int)

#else
#include "memoryAdapter.h"
#include "vdecoder.h"

#define OMX_GetMemAdapterOpsS() \
    MemAdapterGetOpsS()

#define OMX_MemPalloc(memops, nSize)         \
    CdcMemPalloc(memops, nSize, NULL, NULL)

#define OMX_MemPfree(memops, pMem)          \
    CdcMemPfree(memops, pMem, NULL, NULL)

#define OMX_MemPallocSecure(memops, nSize) \
    CdcMemPallocSecure(memops, nSize, NULL, NULL)

#define OMX_MemOpen(memops)           \
    CdcMemOpen(memops)

#define OMX_MemClose(memops)          \
    CdcMemClose(memops)

#define OMX_MemFlushCache(memops, pMem, nSize)  \
    CdcMemFlushCache(memops, pMem, nSize)

#define OMX_MemTotalSize(memops)   \
    CdcMemTotalSize(memops)

#define OMX_MemCopy(memops, pMemDst, pMemSrc, nSize) \
    CdcMemCopy(memops, pMemDst, pMemSrc, nSize)

#define OMX_MemRead(memops, pMemDst, pMemSrc, nSize) \
    CdcMemRead(memops, pMemDst, pMemSrc, nSize)

#define OMX_MemWrite(memops, pMemDst, pMemSrc, nSize) \
    CdcMemWrite(memops, pMemDst, pMemSrc, nSize)

#define OMX_MemGetPhysicAddress(memops, virt) \
    CdcMemGetPhysicAddressCpu(memops, virt)

#define OMX_MemGetVirtualAddress(memops, phy) \
    CdcMemGetVirtualAddressCpu(memops, phy)

#define OMX_MemSetup(memops) \
    CdcMemSetup(memops)

#define OMX_MemShutdown(memops) \
    CdcMemShutdown(memops)

#define OMX_MemGetHandleByVirtualAddr(memops, virt) \
    CdcMemPallocGetFdByVirtualAddr(memops, virt)

#define OMX_MemGetVirtualAddrByHandle(memops, int) \
    CdcMemPallocGetVirtualAddrByFd(memops, int)

#endif



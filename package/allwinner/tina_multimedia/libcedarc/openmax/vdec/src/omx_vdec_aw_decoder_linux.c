/*
 * =====================================================================================
 *
 *       Filename:  omx_vdec_aw_decoder_linux.c
 *
 *    Description: 1. hardware decode with AW decoder
 *                        2. using copy process
 *
 *        Version:  1.0
 *        Created:  08/02/2018 04:18:11 PM
 *       Revision:  none
 *
 *         Author:  Gan Qiuye
 *        Company:
 *
 * =====================================================================================
 */

#define LOG_TAG "omx_vdec_aw"
#include "log.h"
#include "omx_vdec_decoder.h"
#include "vdecoder.h"
#include "omx_vdec_port.h"
#include "omx_vdec_config.h"
#include "OMX_Video.h"
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include "memoryAdapter.h"
#include "veAdapter.h"
#include "veInterface.h"
#include "CdcUtil.h"
#include "omx_mutex.h"
#include "omx_sem.h"
#include "omxMM.h"
//#include <ion/ion.h>

#define USE_ALIGN_SIZE 1


typedef struct TranBufferInfo
{
    void* pAddr;
    int  nWidth;
    int  nHeight;
}TranBufferInfo;

typedef struct OMXOutputBufferInfoA {
    OMX_BUFFERHEADERTYPE* pHeadBufInfo;
    char*                 pBufPhyAddr;
    char*                 pBufVirAddr;
    VideoPicture*         pVideoPicture;
    OutBufferStatus       mStatus;
    OMX_BOOL              mUseDecoderBufferToSetEosFlag;
    int                   nVirAddrSize;
    int                   nBufFd;
 }OMXOutputBufferInfoA;

typedef struct OmxAwDecoderContext
{
    OmxDecoder             base;
    VideoDecoder*          m_decoder;
    VideoStreamInfo        mStreamInfo;
    VConfig                mVideoConfig;
    OMX_S32                mGpuAlignStride;
    #ifdef CONF_OMX_ENABLE_EXTERN_MEM
    struct SunxiMemOpsS *decMemOps;
    #else
    struct ScMemOpsS *decMemOps;
    #endif
    int                    mIonFd;

    OMX_CONFIG_RECTTYPE    mVideoRect; //* for display crop
    OMX_U32                mCodecSpecificDataLen;
    OMX_U8                 mCodecSpecificData[CODEC_SPECIFIC_DATA_LENGTH];
    OMXOutputBufferInfoA   mOutputBufferInfoArr[OMX_MAX_VIDEO_BUFFER_NUM];
    DecoderCallback        callback;
    void*                  pUserData;
    OMX_U32                mSetToDecoderBufferNum;
    OMX_U32                mNeedReSetToDecoderBufferNum;
    OMX_U32                mOutBufferNumDecoderNeed;
    OMX_S32                mExtraOutBufferNum;
    OMX_BOOL               bHadGetVideoFbmBufInfoFlag;
    OMX_BOOL               bFlushFlag;
    OMX_U32                nExtendFlag;

    OMX_BOOL               bInputEosFlag;
    OMX_BOOL               bIsFirstGetOffset;

    AwOmxVdecPort*         pInPort;
    AwOmxVdecPort*         pOutPort;

    OmxMutexHandle         awMutexHandler;
    OmxSemHandle           mSemOutBuffer;
    OmxSemHandle           mSemInData;
    OmxSemHandle           mSemValidPic;

    VideoPicture*          pPicture;

    OMX_BOOL               bUseZeroCopyBuffer;
    OMX_U32                nPhyOffset;
    OMX_U32                mPicNum;
    OMX_U32                mUseMetaDataOutBuffers;
    OMX_U32                nOffset;
    OMX_U32                nFdOffset;

}OmxAwDecoderContext;

//#if(ENABLE_SAVE_PICTURE)
static void savePictureForDebug(OmxAwDecoderContext *pCtx,VideoPicture* pVideoPicture)
{
    pCtx->mPicNum++;
    if(pCtx->mPicNum <= 300 && pCtx->mPicNum >=250)
    {
        char  path[1024] = {0};
        FILE* fpStream   = NULL;
        int   len = 0;
        OMX_PARAM_PORTDEFINITIONTYPE* outDef = getPortDef(pCtx->pOutPort);
        int   width = outDef->format.video.nFrameWidth;
        int   height = outDef->format.video.nFrameHeight;
        logd("save picture: w[%d],h[%d],pVirBuf[%p]",
              width, height, pVideoPicture->pData0);

        sprintf (path,"/data/camera/pic%d.dat",(int)pCtx->mPicNum);
        fpStream = fopen(path, "wb");
        if(pVideoPicture->bEnableAfbcFlag)
            len = pVideoPicture->nAfbcSize;
        else
            len  = (width*height)*3/2;
        if(fpStream != NULL)
        {
            fwrite(pVideoPicture->pData0,1,len, fpStream);
            fclose(fpStream);
        }
        else
        {
            logd("++the fpStream is null when save picture");
        }
    }

    return;
}
//#endif

static OMX_U32 liGetStreamFormat(OMX_VIDEO_CODINGTYPE videoCoding)
{
    switch(videoCoding)
    {
        case OMX_VIDEO_CodingMJPEG:
            return VIDEO_CODEC_FORMAT_MJPEG;//mjpeg
        case OMX_VIDEO_CodingMPEG1:
            return VIDEO_CODEC_FORMAT_MPEG1;//mpeg1
        case OMX_VIDEO_CodingMPEG2:
            return VIDEO_CODEC_FORMAT_MPEG2;//mpeg2
        case OMX_VIDEO_CodingMPEG4:
            return VIDEO_CODEC_FORMAT_XVID;//xvid-mpeg4
        case OMX_VIDEO_CodingMSMPEG4V1:
            return VIDEO_CODEC_FORMAT_MSMPEG4V1;//mpeg4v1
        case OMX_VIDEO_CodingMSMPEG4V2:
            return VIDEO_CODEC_FORMAT_MSMPEG4V2;//mpeg4v2
        case OMX_VIDEO_CodingMSMPEG4V3:
            return VIDEO_CODEC_FORMAT_DIVX3;//divx3-mpeg4v3
        case OMX_VIDEO_CodingDIVX4:
            return VIDEO_CODEC_FORMAT_DIVX4;//divx4
        case OMX_VIDEO_CodingRX:
            return VIDEO_CODEC_FORMAT_RX;//rx
        case OMX_VIDEO_CodingAVS:
            return VIDEO_CODEC_FORMAT_AVS;//avs
        case OMX_VIDEO_CodingDIVX:
            return VIDEO_CODEC_FORMAT_DIVX5;//divx5
        case OMX_VIDEO_CodingXVID:
            return VIDEO_CODEC_FORMAT_XVID;//xvid
        case OMX_VIDEO_CodingH263:
            return VIDEO_CODEC_FORMAT_H263;//h263
        case OMX_VIDEO_CodingS263:
            return VIDEO_CODEC_FORMAT_SORENSSON_H263;//sh263-sorensson
        case OMX_VIDEO_CodingRXG2:
            return VIDEO_CODEC_FORMAT_RXG2;//rxg2
        case OMX_VIDEO_CodingWMV1:
            return VIDEO_CODEC_FORMAT_WMV1;//wmv1
        case OMX_VIDEO_CodingWMV2:
            return VIDEO_CODEC_FORMAT_WMV2;//wmv2
        case OMX_VIDEO_CodingWMV:
            return VIDEO_CODEC_FORMAT_WMV3;//wmv3
        case OMX_VIDEO_CodingVP6:
            return VIDEO_CODEC_FORMAT_VP6;//vp6
        case OMX_VIDEO_CodingVP8:
            return VIDEO_CODEC_FORMAT_VP8;//vp8
        case OMX_VIDEO_CodingVP9:
            return VIDEO_CODEC_FORMAT_VP9;//vp9
        case OMX_VIDEO_CodingAVC:
            return VIDEO_CODEC_FORMAT_H264;//h264-avc
        case OMX_VIDEO_CodingHEVC:
            return VIDEO_CODEC_FORMAT_H265;//h265-hevc
        default:
        {
            loge("unsupported OMX this format:%d", videoCoding);
            break;
        }
    }
    return VIDEO_CODEC_FORMAT_UNKNOWN;
}

void liGetStremInfo(OmxAwDecoderContext *pCtx)
{
    OMX_PARAM_PORTDEFINITIONTYPE* inDef = getPortDef(pCtx->pInPort);
    OMX_VIDEO_PARAM_PORTFORMATTYPE* inFormatType = getPortFormatType(pCtx->pInPort);

    pCtx->mStreamInfo.eCodecFormat = liGetStreamFormat(inFormatType->eCompressionFormat);
    pCtx->mStreamInfo.nWidth  = inDef->format.video.nFrameWidth;
    pCtx->mStreamInfo.nHeight = inDef->format.video.nFrameHeight;
}

static int liGetVideoFbmBufInfo(OmxAwDecoderContext *pCtx)
{
    OmxAcquireMutex(pCtx->awMutexHandler);
    FbmBufInfo* pFbmBufInfo =  GetVideoFbmBufInfo(pCtx->m_decoder);
    OmxReleaseMutex(pCtx->awMutexHandler);

    if(pFbmBufInfo != NULL)
    {
        logd("video buffer info: nWidth[%d],nHeight[%d],"
            "nBufferCount[%d],ePixelFormat[%d]",
              pFbmBufInfo->nBufWidth,pFbmBufInfo->nBufHeight,
              pFbmBufInfo->nBufNum,pFbmBufInfo->ePixelFormat);
        logd("video buffer info: nAlignValue[%d],"
             "bProgressiveFlag[%d],bIsSoftDecoderFlag[%d]",
              pFbmBufInfo->nAlignValue,pFbmBufInfo->bProgressiveFlag,
              pFbmBufInfo->bIsSoftDecoderFlag);

        if(pFbmBufInfo->nBufNum > (OMX_MAX_VIDEO_BUFFER_NUM - 2))
            pFbmBufInfo->nBufNum = OMX_MAX_VIDEO_BUFFER_NUM - 2;

        pCtx->mOutBufferNumDecoderNeed  = pFbmBufInfo->nBufNum;

        OMX_U32 nCurExtendFlag = 0;

        #ifdef CONF_SURPPORT_METADATA_BUFFER
        if(pFbmBufInfo->b10bitVideoFlag == 1)
            nCurExtendFlag |= AW_VIDEO_10BIT_FLAG;

        if(pFbmBufInfo->bAfbcModeFlag == 1)
            nCurExtendFlag |= AW_VIDEO_AFBC_FLAG;

        if(pFbmBufInfo->bHdrVideoFlag == 1)
            nCurExtendFlag |= AW_VIDEO_HDR_FLAG;
        #endif

        logd("**** nExtendFlag = %lx", nCurExtendFlag);
        OMX_PARAM_PORTDEFINITIONTYPE* inDef  = getPortDef(pCtx->pInPort);
        OMX_PARAM_PORTDEFINITIONTYPE* outDef = getPortDef(pCtx->pOutPort);

        OMX_BOOL bBufferNumMatchFlag = OMX_FALSE;
        //* if init-buffer-num is equal to or more than real-buffer-num,
        //* not callback the event "OMX_EventPortSettingsChanged" to avoid remalloc buffer
        //* which cost some time
        if(outDef->nBufferCountActual >= (OMX_U32)pFbmBufInfo->nBufNum)
            bBufferNumMatchFlag = OMX_TRUE;
        logd("*** bBufferNumMatchFlag = %d",bBufferNumMatchFlag);
        OMX_U32 nInitWidht  =  outDef->format.video.nFrameWidth;
        OMX_U32 nInitHeight =  outDef->format.video.nFrameHeight;
        if(bBufferNumMatchFlag
            && nInitWidht  == (OMX_U32)pFbmBufInfo->nBufWidth
            && nInitHeight == (OMX_U32)pFbmBufInfo->nBufHeight
            && nCurExtendFlag == pCtx->nExtendFlag)
        {
            logd("buffer match!!!!");
            return 0;
        }
        logd("************** port setting change ************");
        logd("****  buffer num: %lu, %d; w&h: %lu, %lu, %d, %d",
              outDef->nBufferCountActual,
              pFbmBufInfo->nBufNum,
              nInitWidht, nInitHeight,
              pFbmBufInfo->nBufWidth,
              pFbmBufInfo->nBufHeight);

        if(inDef->format.video.nFrameWidth   == WIDTH_DEFAULT ||
            inDef->format.video.nFrameHeight == HEIGHT_DEFAULT)
        {
            inDef->format.video.nFrameWidth  = pFbmBufInfo->nBufWidth;
            inDef->format.video.nFrameHeight = pFbmBufInfo->nBufHeight;
        }

        //*ACodec will add the mExtraOutBufferNum to nBufferCountActual,
        //*so we decrease it here
        //*if here not - pCtx->mExtraOutBufferNum, it will keep in omx

        OMX_U32 num = pFbmBufInfo->nBufNum/* - pCtx->mExtraOutBufferNum*/;
        outDef->nBufferCountMin    = num;
        outDef->nBufferCountActual = num;
        outDef->format.video.nFrameWidth  = pFbmBufInfo->nBufWidth;
        outDef->format.video.nFrameHeight = pFbmBufInfo->nBufHeight;
        outDef->nBufferSize = pFbmBufInfo->nBufWidth*pFbmBufInfo->nBufHeight*3/2;
        pCtx->nExtendFlag = nCurExtendFlag;

        pCtx->mVideoRect.nLeft   = pFbmBufInfo->nLeftOffset;
        pCtx->mVideoRect.nTop    = pFbmBufInfo->nTopOffset;
        pCtx->mVideoRect.nWidth  = \
                        pFbmBufInfo->nRightOffset - pFbmBufInfo->nLeftOffset;
        pCtx->mVideoRect.nHeight = \
                        pFbmBufInfo->nBottomOffset - pFbmBufInfo->nTopOffset;
        pCtx->callback(pCtx->pUserData, AW_OMX_CB_NOTIFY_RECT, &(pCtx->mVideoRect));
        pCtx->callback(pCtx->pUserData, AW_OMX_CB_PORT_CHANGE, NULL);
    }
    else
    {
        logv("pFbmBufInfo = NULL, m_decoder(%p)", pCtx->m_decoder);
        return -1;
    }

    return 0;
}

static OMX_BOOL isToInitBuffer(OmxAwDecoderContext *pCtx,
                                 OMX_BUFFERHEADERTYPE* pOutBufHdr,
                                 OMX_U32* index)
{
    OMX_U32 i;
    OMX_BOOL ret = OMX_TRUE;
    for(i=0; i< OMX_MAX_VIDEO_BUFFER_NUM; i++)
    {
        OMX_BUFFERHEADERTYPE* tmpBufHdr = \
            pCtx->mOutputBufferInfoArr[i].pHeadBufInfo;
        if(pOutBufHdr == tmpBufHdr)
        {
            ret = OMX_FALSE;
            break;
        }
        if(tmpBufHdr == NULL)
        {
            break;
        }
    }
    //if i>= OMX_MAX_VIDEO_BUFFER_NUM ,
    //something is wrong or the OMX_MAX_VIDEO_BUFFER_NUM is not enough
    *index = i;
    return ret;
}
static int initBufferInfo(OmxAwDecoderContext *pCtx,
                                        OMX_BUFFERHEADERTYPE*   pOutBufHdr,
                                        int *nBufFd)
{
    OMX_U32 i;
    char* nPhyAddress = NULL;
    char* nVirAddress = NULL;
    for(i = 0;i<getPortAllocSize(pCtx->pOutPort);i++)
    {
        if(pCtx->mOutputBufferInfoArr[i].pHeadBufInfo == NULL)
            break;
    }
    if(pCtx->mUseMetaDataOutBuffers == META_DATA_IN_BUFFER)
    {
    	OmxPrivateBuffer sOmxOutPrivateBuffer;
    	memcpy(&sOmxOutPrivateBuffer, pOutBufHdr->pBuffer, sizeof(OmxPrivateBuffer));
    	nVirAddress = (char*)OMX_MemGetVirtualAddrByHandle(pCtx->decMemOps,
            sOmxOutPrivateBuffer.nShareBufFd);
	*nBufFd = sOmxOutPrivateBuffer.nShareBufFd;
    }
    else if(pCtx->mUseMetaDataOutBuffers == META_AND_RAW_DATA_IN_BUFFER)
    {
        OmxPrivateBuffer sOmxOutPrivateBuffer;
        memcpy(&sOmxOutPrivateBuffer.nShareBufFd, pOutBufHdr->pBuffer+pCtx->nFdOffset, sizeof(int));
        logv("share buf fd: %d.", sOmxOutPrivateBuffer.nShareBufFd);
        nVirAddress = (char*)OMX_MemGetVirtualAddrByHandle(pCtx->decMemOps,
            sOmxOutPrivateBuffer.nShareBufFd);
        *nBufFd = sOmxOutPrivateBuffer.nShareBufFd;
    }
    else
    {
        nVirAddress = (char*)pOutBufHdr->pBuffer;
        *nBufFd = -1;
    }

    nPhyAddress = (char*)OMX_MemGetPhysicAddress(pCtx->decMemOps, nVirAddress) - pCtx->nPhyOffset;

    logd("Mode [%ld], nVirAddress [%p], pOutBufHdr [%p], nPhyAddress [%p].",
        pCtx->mUseMetaDataOutBuffers, nVirAddress, pOutBufHdr, nPhyAddress);
    pCtx->mOutputBufferInfoArr[i].nVirAddrSize = pOutBufHdr->nAllocLen;
    pCtx->mOutputBufferInfoArr[i].nBufFd   = *nBufFd;
    pCtx->mOutputBufferInfoArr[i].pBufVirAddr = nVirAddress;
    pCtx->mOutputBufferInfoArr[i].pBufPhyAddr  = (char*)nPhyAddress;
    pCtx->mOutputBufferInfoArr[i].pHeadBufInfo = pOutBufHdr;
    return 0;
}

static int requestOutputBuffer(OmxAwDecoderContext *pCtx,
                                    VideoPicture* pPicBufInfo,
                                    OMX_BUFFERHEADERTYPE*   pOutBufHdr,
                                    OMX_BOOL bInitBufferFlag)
{
    OMX_U32 i;
    int mYsize;
    int nBufFd;
    int ret = 0;
    if(pOutBufHdr == NULL)
    {
        loge(" the pOutBufHdr is null when request OutPut Buffer");
        return -1;
    }

    //* init the buffer

    if(bInitBufferFlag)
    {
        ret = initBufferInfo(pCtx, pOutBufHdr, &nBufFd);
        if(ret == -1)
            return ret;
    }
    OMX_U32 num = getPortAllocSize(pCtx->pOutPort);
    for(i = 0;i<num ;i++)
    {
        if(pCtx->mOutputBufferInfoArr[i].pHeadBufInfo == pOutBufHdr)
            break;
    }

    OMX_ASSERT(i!=num);

    OMX_PARAM_PORTDEFINITIONTYPE* outDef = getPortDef(pCtx->pOutPort);

    //* set the buffer address
    mYsize = outDef->format.video.nFrameWidth * outDef->format.video.nFrameHeight;
    pPicBufInfo->pData0      = pCtx->mOutputBufferInfoArr[i].pBufVirAddr;
    pPicBufInfo->pData1      = pPicBufInfo->pData0 + mYsize;
    pPicBufInfo->phyYBufAddr = \
        (uintptr_t)pCtx->mOutputBufferInfoArr[i].pBufPhyAddr;
    pPicBufInfo->phyCBufAddr = pPicBufInfo->phyYBufAddr + mYsize;
    pPicBufInfo->nBufId      = i;
    pPicBufInfo->nBufFd      =  pCtx->mOutputBufferInfoArr[i].nBufFd;
    pPicBufInfo->nWidth      =  outDef->format.video.nFrameWidth;
    pPicBufInfo->nHeight     =  outDef->format.video.nFrameHeight;
    pPicBufInfo->nLineStride =  outDef->format.video.nFrameWidth;
    pCtx->mOutputBufferInfoArr[i].mStatus = OWNED_BY_DECODER;

    return 0;
}

#if ENABLE_SHOW_BUFINFO_STATUS
static void showBufferInfoArrStatus(OmxAwDecoderContext *pCtx)
{
    int i;
    logd("***********show status begin*********");
    for(i = 0; i < OMX_MAX_VIDEO_BUFFER_NUM; i++)
    {
        if(pCtx->mOutputBufferInfoArr[i].mStatus == OWNED_BY_NONE)
        {
            logd("i:%d~%d,pHeadBufInfo:      0x0, pic:       0x0, "
                 "status: OWNED_BY_NONE",
                i, OMX_MAX_VIDEO_BUFFER_NUM - 1);
            break;
        }
        logd("i:%2d, pHeadBufInfo:%10p, pic:%10p, status: %s, phyYBufAddr: %lx",
            i,
            pCtx->mOutputBufferInfoArr[i].pHeadBufInfo,
            pCtx->mOutputBufferInfoArr[i].pVideoPicture,
            statusToString(pCtx->mOutputBufferInfoArr[i].mStatus),
            (uintptr_t)pCtx->mOutputBufferInfoArr[i].pBufPhyAddr);
    }
    logd("***********show status end***********");
}
#endif

static int returnInitBuffer(OmxAwDecoderContext *pCtx,
                            OMX_BUFFERHEADERTYPE* pOutBufHdr,
                            OMX_U32 index)
{
    int mRet = -1;
    VideoPicture mVideoPicture;
    VideoPicture* pVideoPicture = NULL;
    memset(&mVideoPicture, 0, sizeof(VideoPicture));
    if(pCtx->mSetToDecoderBufferNum >= pCtx->mOutBufferNumDecoderNeed)
    {
        pCtx->mOutputBufferInfoArr[index].pHeadBufInfo = pOutBufHdr;
        pCtx->mOutputBufferInfoArr[index].mStatus      = OWNED_BY_US;
#if ENABLE_SHOW_BUFINFO_STATUS
        showBufferInfoArrStatus(pCtx);
#endif
        return 0;
    }
    else
    {
        mRet = requestOutputBuffer(pCtx, &mVideoPicture, pOutBufHdr, OMX_TRUE);
        if(mRet == 0)
        {
            pCtx->mSetToDecoderBufferNum++;
            if(pCtx->mNeedReSetToDecoderBufferNum > 0)
            {
                if(RequestReleasePicture(pCtx->m_decoder) != NULL)
                {
                    pCtx->mNeedReSetToDecoderBufferNum--;
                    pVideoPicture = ReturnRelasePicture(pCtx->m_decoder,
                                                        &mVideoPicture, 0);
                }
                else
                {
                    logw("** can not reqeust release picture");
                    return -1;
                }
            }
            else
            {
                pVideoPicture = SetVideoFbmBufAddress(pCtx->m_decoder,
                                                      &mVideoPicture, 0);
                logd("*** call SetVideoFbmBufAddress: pVideoPicture(%p)",
                     pVideoPicture);
            }
            OmxTryPostSem(pCtx->mSemOutBuffer);
            pCtx->mOutputBufferInfoArr[mVideoPicture.nBufId].pVideoPicture = pVideoPicture;

#if ENABLE_SHOW_BUFINFO_STATUS
            showBufferInfoArrStatus(pCtx);
#endif
            return 0;
        }
        else
            return mRet;
    }
}

static int returnArrangedBuffer(OmxAwDecoderContext *pCtx,
                                OMX_BUFFERHEADERTYPE* pOutBufHdr,
                                OMX_U32 index)
{
    int mRet = -1;
    VideoPicture mVideoPicture;
    VideoPicture* pVideoPicture = NULL;
    memset(&mVideoPicture, 0, sizeof(VideoPicture));

    mRet = requestOutputBuffer(pCtx, &mVideoPicture, pOutBufHdr, OMX_FALSE);
    if(mRet == 0)
    {
        //* we should not return to decoder again if it had been used to set eos to render
        if(pCtx->mOutputBufferInfoArr[index].mUseDecoderBufferToSetEosFlag)
        {
            pCtx->mOutputBufferInfoArr[index].mUseDecoderBufferToSetEosFlag = OMX_FALSE;
            return 0;
        }
        pVideoPicture = pCtx->mOutputBufferInfoArr[index].pVideoPicture;
        ReturnPicture(pCtx->m_decoder, pVideoPicture);
        OmxTryPostSem(pCtx->mSemOutBuffer);
        return 0;
    }
    else
        return mRet;
 }

static int returnBufferToDecdoer(OmxAwDecoderContext *pCtx)
{
    OMX_U32 index = 0xff;
    OMX_BUFFERHEADERTYPE*  pOutBufHdr = NULL;
    pOutBufHdr = doRequestPortBuffer(pCtx->pOutPort);
    if(pOutBufHdr != NULL)
    {
        if(isToInitBuffer(pCtx, pOutBufHdr, &index))
        {
            return returnInitBuffer(pCtx, pOutBufHdr, index);
        }
        else
        {
            return returnArrangedBuffer(pCtx, pOutBufHdr, index);
        }
    }
    else
        return -1;
}

static void freeOutputBufferInfoArr(OmxAwDecoderContext *pCtx)
{
    memset(&pCtx->mOutputBufferInfoArr, 0,
             sizeof(OMXOutputBufferInfoA)*OMX_MAX_VIDEO_BUFFER_NUM);
}

static void standbyOutBufferArr(OmxAwDecoderContext *pCtx)
{
    int i;
    OMX_BOOL        out_buffer_flag   = OMX_FALSE;

    for(i = 0; i < OMX_MAX_VIDEO_BUFFER_NUM; i++)
    {
        OutBufferStatus out_buffer_status = pCtx->mOutputBufferInfoArr[i].mStatus;
        out_buffer_flag = \
            pCtx->mOutputBufferInfoArr[i].mUseDecoderBufferToSetEosFlag;
        if(out_buffer_status == OWNED_BY_DECODER)
        {
            logd("fillBufferDone: i[%d],pHeadBufInfo[%p],mStatus[%s]",
                 i,
                 pCtx->mOutputBufferInfoArr[i].pHeadBufInfo,
                 statusToString(out_buffer_status));

            pCtx->callback(pCtx->pUserData, AW_OMX_CB_FILL_BUFFER_DONE,
                            pCtx->mOutputBufferInfoArr[i].pHeadBufInfo);

            pCtx->mOutputBufferInfoArr[i].mStatus = OWNED_BY_RENDER;
        }
        else if(out_buffer_status == OWNED_BY_RENDER
                && !out_buffer_flag && pCtx->bFlushFlag)
        {
            if(pCtx->mOutputBufferInfoArr[i].pVideoPicture != NULL)
            {

                logw("** return pic when flush,i[%d],pPic[%p]",
                     i,pCtx->mOutputBufferInfoArr[i].pVideoPicture);
                ReturnPicture(pCtx->m_decoder,
                              pCtx->mOutputBufferInfoArr[i].pVideoPicture);
                OmxTryPostSem(pCtx->mSemOutBuffer);
            }
        }
        else if(out_buffer_status == OWNED_BY_US)
        {
            logd("** fillbufferDone owned_by_us, i[%d]", i);
            pCtx->callback(pCtx->pUserData, AW_OMX_CB_FILL_BUFFER_DONE,
                            pCtx->mOutputBufferInfoArr[i].pHeadBufInfo);
        }
    }

    freeOutputBufferInfoArr(pCtx);
}

static int liDealWithInitData(OmxAwDecoderContext *pCtx)
{
    OMX_BUFFERHEADERTYPE* pInBufHdr = doRequestPortBuffer(pCtx->pInPort);

    if(pInBufHdr == NULL)
    {
        logd("fatal error! pInBufHdr is NULL, check code!");
        return -1;
    }

    if(pInBufHdr->nFlags & OMX_BUFFERFLAG_CODECCONFIG)
    {
        OMX_ASSERT((pInBufHdr->nFilledLen + pCtx->mCodecSpecificDataLen) <=
                    CODEC_SPECIFIC_DATA_LENGTH);

        OMX_U8* pInBuffer = pInBufHdr->pBuffer;
        memcpy(pCtx->mCodecSpecificData + pCtx->mCodecSpecificDataLen,
               pInBuffer,
               pInBufHdr->nFilledLen);

        pCtx->mCodecSpecificDataLen += pInBufHdr->nFilledLen;
        pCtx->callback(pCtx->pUserData, AW_OMX_CB_EMPTY_BUFFER_DONE, (void*)pInBufHdr);
    }
    else
    {
        logd("++++++++++++++++pCtx->mCodecSpecificDataLen[%d]",(int)pCtx->mCodecSpecificDataLen);
        if(pCtx->mCodecSpecificDataLen > 0)
        {
            if(pCtx->mStreamInfo.pCodecSpecificData)
                free(pCtx->mStreamInfo.pCodecSpecificData);
            pCtx->mStreamInfo.nCodecSpecificDataLen = pCtx->mCodecSpecificDataLen;
            pCtx->mStreamInfo.pCodecSpecificData    = (char*)malloc(pCtx->mCodecSpecificDataLen);
            memset(pCtx->mStreamInfo.pCodecSpecificData, 0, pCtx->mCodecSpecificDataLen);
            memcpy(pCtx->mStreamInfo.pCodecSpecificData,
                   pCtx->mCodecSpecificData, pCtx->mCodecSpecificDataLen);
        }
        else
        {
            pCtx->mStreamInfo.pCodecSpecificData      = NULL;
            pCtx->mStreamInfo.nCodecSpecificDataLen = 0;
        }
        returnPortBuffer(pCtx->pInPort);
        return 0;
    }
    return -1;
}

static int liCheckResolutionChange(OmxAwDecoderContext *pCtx, VideoPicture* picture)
{
    int width_align  = 0;
    int height_align = 0;
    OMX_CONFIG_RECTTYPE mCurVideoRect;
    memset(&mCurVideoRect, 0, sizeof(OMX_CONFIG_RECTTYPE));

    if(pCtx->bIsFirstGetOffset)
    {
        pCtx->mVideoRect.nLeft   = picture->nLeftOffset;
        pCtx->mVideoRect.nTop    = picture->nTopOffset;
        pCtx->mVideoRect.nWidth  = picture->nRightOffset - picture->nLeftOffset;
        pCtx->mVideoRect.nHeight = picture->nBottomOffset - picture->nTopOffset;
        logd("gqy********l:%ld, t:%ld, w:%ld, h:%ld",
            pCtx->mVideoRect.nLeft,
            pCtx->mVideoRect.nTop,
            pCtx->mVideoRect.nWidth,
            pCtx->mVideoRect.nHeight);
        pCtx->bIsFirstGetOffset = OMX_FALSE;
    }

    mCurVideoRect.nLeft   = picture->nLeftOffset;
    mCurVideoRect.nTop    = picture->nTopOffset;
    mCurVideoRect.nWidth  = picture->nRightOffset  - picture->nLeftOffset;
    mCurVideoRect.nHeight = picture->nBottomOffset - picture->nTopOffset;
#if USE_ALIGN_SIZE
    width_align  = picture->nWidth;
    height_align = picture->nHeight;

#else
    width_align  = picture->nRightOffset  - picture->nLeftOffset;
    height_align = picture->nBottomOffset - picture->nTopOffset;
    if(width_align <= 0 || height_align <= 0)
    {
        width_align  = picture->nWidth;
        height_align = picture->nHeight;
    }
#endif
    //* make the height to 2 align
   // height_align = (height_align + 1) & ~1;
    OMX_PARAM_PORTDEFINITIONTYPE* outDef = getPortDef(pCtx->pOutPort);

    if((OMX_U32)width_align != outDef->format.video.nFrameWidth
        || (OMX_U32)height_align  != outDef->format.video.nFrameHeight
        || (mCurVideoRect.nLeft   != pCtx->mVideoRect.nLeft)
        || (mCurVideoRect.nTop    != pCtx->mVideoRect.nTop)
        || (mCurVideoRect.nWidth  != pCtx->mVideoRect.nWidth)
        || (mCurVideoRect.nHeight != pCtx->mVideoRect.nHeight))
    {
        logw(" port setting changed -- old info : widht = %d, height = %d, "
               "mVideoRect: %d, %d, %d, %d ",
                (int)outDef->format.video.nFrameWidth,
                (int)outDef->format.video.nFrameHeight,
                (int)pCtx->mVideoRect.nTop,(int)pCtx->mVideoRect.nLeft,
                (int)pCtx->mVideoRect.nWidth,(int)pCtx->mVideoRect.nHeight);
        logw(" port setting changed -- new info : widht = %d, height = %d, "
               "mVideoRect: %d, %d, %d, %d ",
                (int)width_align, (int)height_align,
                (int)mCurVideoRect.nTop,(int)mCurVideoRect.nLeft,
                (int)mCurVideoRect.nWidth,(int)mCurVideoRect.nHeight);

        memcpy(&pCtx->mVideoRect, &mCurVideoRect, sizeof(OMX_CONFIG_RECTTYPE));
        outDef->format.video.nFrameHeight = height_align;
        outDef->format.video.nFrameWidth  = width_align;
        outDef->format.video.nStride      = width_align;
        outDef->format.video.nSliceHeight = height_align;

        outDef->nBufferSize = height_align*width_align *3/2;
        pCtx->callback(pCtx->pUserData, AW_OMX_CB_NOTIFY_RECT, &(pCtx->mVideoRect));
        pCtx->callback(pCtx->pUserData, AW_OMX_CB_PORT_CHANGE, NULL);
        return 0;
    }
    return -1;
}
/*
static void AlignTransformYV12ToYUV420(unsigned char* dstPtr,
                                                 unsigned char* srcPtr,
                                                 int w, int h)
{
    int yPlaneSz = w*h;
    int uvPlaneSz = yPlaneSz/4;
    //copy y
    memcpy(dstPtr, srcPtr, yPlaneSz);

    //copy u
    unsigned char* uSrcPtr = srcPtr + yPlaneSz*5/4;
    unsigned char* uDstPtr = dstPtr + yPlaneSz;
    memcpy(uDstPtr, uSrcPtr, uvPlaneSz);

    //copy v
    unsigned char* vSrcPtr = srcPtr  + yPlaneSz;
    unsigned char* vDstPtr = uDstPtr + uvPlaneSz;
    memcpy(vDstPtr, vSrcPtr, uvPlaneSz);
}*/

//nFlag, 1: transfrom yv12 to yuv420. 0:just copy y/u/v plane.
static void AlignTransformYUVPlane(unsigned char* dstPtr,
                                                 unsigned char* srcPtr,
                                                 int w, int h, int nFlag)
{
    int ratio1;
    int ratio2;
    //* if nFlag not 1, just copy y/u/v plane. if 1, it means transform yv12 to yuv420.
    if(nFlag == 1)
    {
        ratio1 = 125;
        ratio2 = 100;
    }
    else
    {
        ratio1 = 100;
        ratio2 = 125;
    }

    int yPlaneSz = w*h;
    int uvPlaneSz = yPlaneSz/4;
    //copy y
    memcpy(dstPtr, srcPtr, yPlaneSz);

    //copy the next plane
    unsigned char* uSrcPtr = srcPtr + yPlaneSz*ratio1/100;
    unsigned char* uDstPtr = dstPtr + yPlaneSz;
    memcpy(uDstPtr, uSrcPtr, uvPlaneSz);

    //*copy the third plane
    unsigned char* vSrcPtr = srcPtr  + yPlaneSz*ratio2/100;
    unsigned char* vDstPtr = uDstPtr + uvPlaneSz;
    memcpy(vDstPtr, vSrcPtr, uvPlaneSz);
}

//nFlag, 1: transfrom yv12 to yuv420. 0:just copy y/u/v plane.
static void TransformYUVPlane(VideoPicture* pPicture,
                                  TranBufferInfo* pTranBufferInfo, int nFlag)
{
    int i;
    int nPicRealWidth;
    int nPicRealHeight;
    int nSrcBufWidth;
    int nSrcBufHeight;
    int nDstBufWidth;
    int nDstBufHeight;
    int nCopyDataWidth;
    int nCopyDataHeight;
    unsigned char* dstPtr;
    unsigned char* srcPtr;
    int ratio1;
    int ratio2;
    dstPtr      = (unsigned char*)pTranBufferInfo->pAddr;
    srcPtr      = (unsigned char*)pPicture->pData0;

    nPicRealWidth  = pPicture->nRightOffset  - pPicture->nLeftOffset;
    nPicRealHeight = pPicture->nBottomOffset - pPicture->nTopOffset;

    //* if nFlag not 1, just copy y/u/v plane. if 1, it means transform yv12 to yuv420.
    if(nFlag == 1)
    {
        ratio1 = 125;
        ratio2 = 100;
    }
    else
    {
        ratio1 = 100;
        ratio2 = 125;
    }

    //* if the uOffset is not right, we should not use them to compute width and height
    if(nPicRealWidth <= 0 || nPicRealHeight <= 0)
    {
        nPicRealWidth  = pPicture->nWidth;
        nPicRealHeight = pPicture->nHeight;
    }

    nSrcBufWidth  = (pPicture->nWidth + 15) & ~15;
    nSrcBufHeight = (pPicture->nHeight + 15) & ~15;

    //* On chip-1673, the gpu is 32 align ,but here is not copy to gpu, so also 16 align.
    //* On other chip, gpu buffer is 16 align.
    //nDstBufWidth = (pTranBufferInfo->nWidth+ 15)&~15;
    nDstBufWidth  = pTranBufferInfo->nWidth;

    nDstBufHeight = pTranBufferInfo->nHeight;

    nCopyDataWidth  = nPicRealWidth;
    nCopyDataHeight = nPicRealHeight;

    logv("nPicRealWidth & H = %d, %d, nSrcBufWidth & H = %d, %d,"
         "nDstBufWidth & H = %d, %d,  nCopyDataWidth & H = %d, %d",
          nPicRealWidth,nPicRealHeight,
          nSrcBufWidth,nSrcBufHeight,
          nDstBufWidth,nDstBufHeight,
          nCopyDataWidth,nCopyDataHeight);

    //*copy y
    for(i=0; i < nCopyDataHeight; i++)
    {
        memcpy(dstPtr, srcPtr, nCopyDataWidth);
        dstPtr += nDstBufWidth;
        srcPtr += nSrcBufWidth;
    }

    //*copy u
    srcPtr = ((unsigned char*)pPicture->pData0) + nSrcBufWidth*nSrcBufHeight*ratio1/100;
    nCopyDataWidth  = (nCopyDataWidth+1)/2;
    nCopyDataHeight = (nCopyDataHeight+1)/2;

    for(i=0; i < nCopyDataHeight; i++)
    {
        memcpy(dstPtr, srcPtr, nCopyDataWidth);
        dstPtr += nDstBufWidth/2;
        srcPtr += nSrcBufWidth/2;
    }

    //*copy v
    srcPtr = ((unsigned char*)pPicture->pData0) + nSrcBufWidth*nSrcBufHeight*ratio2/100;
    for(i=0; i<nCopyDataHeight; i++)
    {
        memcpy(dstPtr, srcPtr, nCopyDataWidth);
        dstPtr += nDstBufWidth/2;    //a31 gpu, uv is half of y
        srcPtr += nSrcBufWidth/2;
    }

    return;
}

static void liReopenVideoEngine(OmxAwDecoderContext *pCtx)
{
    logd("***ReopenVideoEngine!");

    if(pCtx->mStreamInfo.pCodecSpecificData != NULL)
    {
        free(pCtx->mStreamInfo.pCodecSpecificData);
        pCtx->mStreamInfo.pCodecSpecificData  = NULL;
        pCtx->mStreamInfo.nCodecSpecificDataLen = 0;
    }

    OmxAcquireMutex(pCtx->awMutexHandler);
    ReopenVideoEngine(pCtx->m_decoder, &pCtx->mVideoConfig, &(pCtx->mStreamInfo));
    OmxReleaseMutex(pCtx->awMutexHandler);

    return ;
}

static OMX_BUFFERHEADERTYPE* liDrainZeroCopy(OmxAwDecoderContext *pCtx)
{
    OMX_U32      i = 0;
    VideoPicture*           pPicture     = NULL;
    OMX_BUFFERHEADERTYPE*   pOutBufHdr  = NULL;

    pPicture = RequestPicture(pCtx->m_decoder, 0);
    if(pPicture == NULL)
    {
        logw("the pPicture is null when request displayer picture!");
        return NULL;
    }
    logv("*** picture %p,  picture info: w(%d),h(%d),offset,t(%d),b(%d),l(%d),r(%d)",
         pPicture,
         pPicture->nWidth,pPicture->nHeight,
         pPicture->nTopOffset,pPicture->nBottomOffset,
         pPicture->nLeftOffset,pPicture->nRightOffset);

//#if(ENABLE_SAVE_PICTURE)
    if(getenv("OMX_VDEC_SAVE_PIC"))
        savePictureForDebug(pCtx, pPicture);
//#endif
    OMX_MemFlushCache(pCtx->decMemOps, (void*)pPicture->pData0,
                 pPicture->nLineStride * pPicture->nHeight*3/2);

    OMX_U32 num = getPortAllocSize(pCtx->pOutPort);
    for(i = 0; i<num; i++)
    {
        if(pCtx->mOutputBufferInfoArr[i].pVideoPicture == pPicture)
            break;
    }

    OMX_ASSERT(i!=num);

    //* we should return buffer to decoder if it was used as set eos to render
    if(pCtx->mOutputBufferInfoArr[i].mUseDecoderBufferToSetEosFlag)
    {
        logw("detect the buffer had been used to set eos to render,\
              here, we not callback again!");
        pCtx->mOutputBufferInfoArr[i].mStatus = OWNED_BY_RENDER;
        pCtx->mOutputBufferInfoArr[i].mUseDecoderBufferToSetEosFlag = OMX_FALSE;
        return NULL;
    }

    pOutBufHdr =  pCtx->mOutputBufferInfoArr[i].pHeadBufInfo;

    pCtx->mOutputBufferInfoArr[i].mStatus = OWNED_BY_RENDER;

    pOutBufHdr->nTimeStamp = pPicture->nPts;
    pOutBufHdr->nOffset    = 0;
    OMX_PARAM_PORTDEFINITIONTYPE* outDef = getPortDef(pCtx->pOutPort);

    pOutBufHdr->nFilledLen = (outDef->format.video.nFrameWidth *
                              outDef->format.video.nFrameHeight) * 3 / 2;
    return pOutBufHdr;

}

static OMX_BUFFERHEADERTYPE* liDrainCopy(OmxAwDecoderContext *pCtx)
{
    VideoPicture*           pPicture     = NULL;
    OMX_BUFFERHEADERTYPE*   pOutBufHdr  = NULL;
    TranBufferInfo          mTranBufferInfo;
    memset(&mTranBufferInfo, 0 ,sizeof(TranBufferInfo));

    int validPicNum = ValidPictureNum(pCtx->m_decoder, 0);
    if(validPicNum <= 0)
    {
        return NULL;
    }
    OmxAcquireMutex(pCtx->awMutexHandler);
    pPicture = NextPictureInfo(pCtx->m_decoder,0);
    OmxReleaseMutex(pCtx->awMutexHandler);
    if(pPicture == NULL)
        return NULL;

    if(liCheckResolutionChange(pCtx, pPicture) == 0)
    {
        return NULL;
    }
    pOutBufHdr = doRequestPortBuffer(pCtx->pOutPort);
    if(pOutBufHdr == NULL)
    {
        return NULL;
    }

    OmxAcquireMutex(pCtx->awMutexHandler);
    pCtx->pPicture = RequestPicture(pCtx->m_decoder, 0);
    OmxReleaseMutex(pCtx->awMutexHandler);
    if(getenv("OMX_VDEC_SAVE_PIC"))
        savePictureForDebug(pCtx, pPicture);
    logv("*** get picture[%p]",pCtx->pPicture);
    if(pCtx->pPicture == NULL)
    {
        logw("the pPicture is null when request displayer picture!");
        return NULL;
    }
    logv("*** picture info: w(%d),h(%d),offset,t(%d),b(%d),l(%d),r(%d)",
         pCtx->pPicture->nWidth,    pCtx->pPicture->nHeight,
         pCtx->pPicture->nTopOffset,pCtx->pPicture->nBottomOffset,
         pCtx->pPicture->nLeftOffset,pCtx->pPicture->nRightOffset);
    OMX_MemFlushCache(pCtx->decMemOps, (void*)pCtx->pPicture->pData0,
             pCtx->pPicture->nLineStride * pCtx->pPicture->nHeight*3/2);
    OMX_PARAM_PORTDEFINITIONTYPE* outDef = getPortDef(pCtx->pOutPort);
#if USE_ALIGN_SIZE
    if(pCtx->mVideoConfig.eOutputPixelFormat == PIXEL_FORMAT_YV12 &&
        COLOR_FORMAT_DEFAULT == OMX_COLOR_FormatYUV420Planar)
    {
        AlignTransformYUVPlane((unsigned char*)pOutBufHdr->pBuffer,
                                (unsigned char*)pCtx->pPicture->pData0,
                                outDef->format.video.nFrameWidth,
                                outDef->format.video.nFrameHeight, 1);
    }
    else
    {
        //just copy y/u/v for other cases including 4:2:0 format nv21, nv12, yuv420, yvu420.
        AlignTransformYUVPlane((unsigned char*)pOutBufHdr->pBuffer,
                                (unsigned char*)pCtx->pPicture->pData0,
                                outDef->format.video.nFrameWidth,
                                outDef->format.video.nFrameHeight, 0);
    }
#else
    mTranBufferInfo.pAddr   = (char*)*pOutBufHdr->pBuffer;
    mTranBufferInfo.nWidth  = outDef->format.video.nFrameWidth;
    mTranBufferInfo.nHeight = outDef->format.video.nFrameHeight;
    if(pCtx->mVideoConfig.eOutputPixelFormat == PIXEL_FORMAT_YV12 &&
        COLOR_FORMAT_DEFAULT == OMX_COLOR_FormatYUV420Planar)
    {
        TransformYUVPlane(pCtx->pPicture, &mTranBufferInfo, 1);  // YUV420 planar
    }
    else
    {
        TransformYUVPlane(pCtx->pPicture, &mTranBufferInfo, 0);
    }
#endif
    pOutBufHdr->nTimeStamp = pCtx->pPicture->nPts;
    pOutBufHdr->nOffset    = 0;
    pOutBufHdr->nFilledLen = (outDef->format.video.nFrameWidth *
                              outDef->format.video.nFrameHeight) * 3 / 2;

    return pOutBufHdr;

}
static int __liCallback(OmxDecoder* pDec, DecoderCallback callback,
                                    void* pUserData)
{
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;
    pCtx->callback  = callback;
    pCtx->pUserData = pUserData;
    return 0;
}
static OMX_ERRORTYPE __liGetExtPara(OmxDecoder* pDec,
                                       AW_VIDEO_EXTENSIONS_INDEXTYPE eParamIndex,
                                       OMX_PTR pParamData)
{
    CEDARC_UNUSE(pDec);
    CEDARC_UNUSE(pParamData);
    CEDARC_UNUSE(eParamIndex);
    logw("get_parameter: unknown param %08x\n", eParamIndex);
    return OMX_ErrorUnsupportedIndex;
}

static OMX_ERRORTYPE __liSetExtPara(OmxDecoder* pDec,
                                       AW_VIDEO_EXTENSIONS_INDEXTYPE eParamIndex,
                                       OMX_PTR pParamData)
{
    CEDARC_UNUSE(pDec);
    CEDARC_UNUSE(pParamData);
    logw("Setparameter: unknown param %d\n", eParamIndex);
    return OMX_ErrorUnsupportedIndex;
}

static OMX_ERRORTYPE __liGetExtConfig(OmxDecoder* pDec,
                                       AW_VIDEO_EXTENSIONS_INDEXTYPE eConfigIndex,
                                       OMX_PTR pConfigData)
{
    CEDARC_UNUSE(pDec);
    CEDARC_UNUSE(pConfigData);
    logw("Setparameter: unknown param %d\n", eConfigIndex);
    return OMX_ErrorUnsupportedIndex;
}

static OMX_ERRORTYPE __liSetExtConfig(OmxDecoder* pDec,
                                       AW_VIDEO_EXTENSIONS_INDEXTYPE eConfigIndex,
                                       OMX_PTR pConfigData)
{
    CEDARC_UNUSE(pDec);
    CEDARC_UNUSE(pConfigData);
    logw("Setparameter: unknown param %d\n", eConfigIndex);
    return OMX_ErrorUnsupportedIndex;
}

static int __liPrepare(OmxDecoder* pDec)
{
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;
    int ret = -1;
    if(-1 == liDealWithInitData(pCtx))
        return -1;
    logd("decoder prepare");
    liGetStremInfo(pCtx);

    OmxAcquireMutex(pCtx->awMutexHandler);
    //*if mdecoder had closed before, we should create it
    if(pCtx->m_decoder==NULL)
    {
        AddVDPlugin();
        pCtx->m_decoder = CreateVideoDecoder();
    }
    if(pCtx->bUseZeroCopyBuffer== OMX_TRUE)
        pCtx->mVideoConfig.bGpuBufValid = 1;

    pCtx->mVideoConfig.nAlignStride       = pCtx->mGpuAlignStride;

    pCtx->mVideoConfig.eOutputPixelFormat = PIXEL_FORMAT_SET;//* Used to be YV12.

#if (ENABLE_SCALEDOWN_WHEN_RESOLUTION_MOER_THAN_1080P)
    if (pCtx->mStreamInfo.nWidth > 1920
        && pCtx->mStreamInfo.nHeight > 1088)
    {
        pCtx->mVideoConfig.bScaleDownEn = 1;
        pCtx->mVideoConfig.nHorizonScaleDownRatio = 1;
        pCtx->mVideoConfig.nVerticalScaleDownRatio = 1;
    }
#endif

    if(pCtx->mStreamInfo.eCodecFormat == VIDEO_CODEC_FORMAT_WMV3)
    {
        logd("*** pCtx->mStreamInfo.bIsFramePackage to 1 when it is vc1");
        pCtx->mStreamInfo.bIsFramePackage = 1;
    }


    pCtx->mVideoConfig.bDispErrorFrame = 1;

    pCtx->mVideoConfig.nDeInterlaceHoldingFrameBufferNum = 0;
    pCtx->mVideoConfig.nDisplayHoldingFrameBufferNum  = 0;
    pCtx->mVideoConfig.nRotateHoldingFrameBufferNum \
                         = NUM_OF_PICTURES_KEEPPED_BY_ROTATE;
    pCtx->mVideoConfig.nDecodeSmoothFrameBufferNum \
                         = NUM_OF_PICTURES_FOR_EXTRA_SMOOTH;

    pCtx->mVideoConfig.memops = MemAdapterGetOpsS();
    //pCtx->decMemOps  = pCtx->mVideoConfig.memops;
    //CdcMemOpen(pCtx->decMemOps);
    pCtx->mStreamInfo.bIsFramePackage = 1;
    ret = InitializeVideoDecoder(pCtx->m_decoder,
                                 &(pCtx->mStreamInfo),
                                 &pCtx->mVideoConfig);
    if(ret != 0)
    {
        DestroyVideoDecoder(pCtx->m_decoder);
        pCtx->m_decoder           = NULL;
        pCtx->callback(pCtx->pUserData, AW_OMX_CB_EVENT_ERROR, NULL);
        loge("Idle transition failed, set_vstream_info() return fail.\n");
        OmxReleaseMutex(pCtx->awMutexHandler);
        return ret;
    }
    OmxReleaseMutex(pCtx->awMutexHandler);
    OmxTryPostSem(pCtx->mSemInData);
    OmxTryPostSem(pCtx->mSemOutBuffer);
    return ret;
}

static int __liSubmit(OmxDecoder* pDec, OMX_BUFFERHEADERTYPE* pInBufHdr)
{
    logv("OmxCopyInputDataToDecoder()");

    char* pBuf0;
    char* pBuf1;
    int   size0;
    int   size1;
    int   require_size;
    unsigned char*   pData;
    VideoStreamDataInfo DataInfo;
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;

    memset(&DataInfo, 0, sizeof(VideoStreamDataInfo));

    require_size = pInBufHdr->nFilledLen;

    OmxAcquireMutex(pCtx->awMutexHandler);
    if(0 != RequestVideoStreamBuffer(pCtx->m_decoder, require_size,
                                     &pBuf0, &size0, &pBuf1, &size1,0))
    {
        logv("req vbs fail! maybe vbs buffer is full! require_size[%d]",
             require_size);
        OmxReleaseMutex(pCtx->awMutexHandler);
        return -1;
    }

    if(require_size != (size0 + size1))
    {
        OmxReleaseMutex(pCtx->awMutexHandler);
        logw(" the requestSize[%d] is not equal to needSize[%d]!",(size0+size1),require_size);
        return -1;
    }

    DataInfo.nLength      = require_size;
    DataInfo.bIsFirstPart = 1;
    DataInfo.bIsLastPart  = 1;
    DataInfo.pData        = pBuf0;
    if(pInBufHdr->nTimeStamp >= 0)
    {
        DataInfo.nPts   = pInBufHdr->nTimeStamp;
        DataInfo.bValid = 1;
    }
    else
    {
        DataInfo.nPts   = -1;
        DataInfo.bValid = 0;
    }

    pData = pInBufHdr->pBuffer + pInBufHdr->nOffset;
    if(require_size <= size0)
    {
        memcpy(pBuf0, pData, require_size);
    }
    else
    {
        memcpy(pBuf0, pData, size0);
        pData += size0;
        memcpy(pBuf1, pData, require_size - size0);
    }
    SubmitVideoStreamData(pCtx->m_decoder, &DataInfo,0);

    OmxReleaseMutex(pCtx->awMutexHandler);

    return 0 ;
}

static inline void __liDecode(OmxDecoder* pDec)
{
    int decodeResult;
    int validPicNum = 0;

    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;

    OmxAcquireMutex(pCtx->awMutexHandler);
    decodeResult = DecodeVideoStream(pCtx->m_decoder,0,0,0,0);
    logv("***decodeResult = %d",decodeResult);
    OmxReleaseMutex(pCtx->awMutexHandler);

    if(decodeResult == VDECODE_RESULT_KEYFRAME_DECODED ||
       decodeResult == VDECODE_RESULT_FRAME_DECODED ||
       decodeResult == VDECODE_RESULT_OK)
    {
        OmxTryPostSem(pCtx->mSemValidPic);
    }
    else if(decodeResult == VDECODE_RESULT_NO_FRAME_BUFFER)
    {
        OmxTimedWaitSem(pCtx->mSemOutBuffer, 20);
    }
    else if(decodeResult == VDECODE_RESULT_NO_BITSTREAM ||
            decodeResult == VDECODE_RESULT_CONTINUE)
    {
        if(pCtx->bInputEosFlag)
        {
            //TODO: If eos,  then never reset it again.
            //pCtx->bInputEosFlag = OMX_FALSE;

            //*set eos to decoder ,decoder should flush all fbm
            int mRet = 0;
            int mDecodeCount = 0;
            while(mRet != VDECODE_RESULT_NO_BITSTREAM)
            {
                OmxAcquireMutex(pCtx->awMutexHandler);
                mRet = DecodeVideoStream(pCtx->m_decoder,1,0,0,0);
                OmxReleaseMutex(pCtx->awMutexHandler);
                if(mRet == VDECODE_RESULT_RESOLUTION_CHANGE)
                    goto resolution_change;
                usleep(5*1000);
                mDecodeCount++;
                if(mDecodeCount > 1000)
                {
                    logw(" decoder timeOut when set eos to decoder!");
                    break;
                }
            }

            pCtx->callback(pCtx->pUserData, AW_OMX_CB_NOTIFY_EOS, NULL);
            OmxTimedWaitSem(pCtx->mSemInData, 20);
        }
        else
        {
            OmxTimedWaitSem(pCtx->mSemInData, 20);
        }
    }
    else if(decodeResult == VDECODE_RESULT_RESOLUTION_CHANGE)
    {
    resolution_change:
        validPicNum = ValidPictureNum(pCtx->m_decoder, 0);
        pCtx->callback(pCtx->pUserData, AW_OMX_CB_FLUSH_ALL_PIC, &validPicNum);
        liReopenVideoEngine(pCtx);
        pCtx->callback(pCtx->pUserData, AW_OMX_CB_FININSH_REOPEN_VE, NULL);
    }
    else if(decodeResult < 0)
    {
        logw("decode fatal error[%d]", decodeResult);
        pCtx->callback(pCtx->pUserData, AW_OMX_CB_EVENT_ERROR, NULL);
    }
    else
    {
        logd("decode ret[%d], ignore? why?", decodeResult);
    }
    return ;
}


static OMX_BUFFERHEADERTYPE* __liDrain(OmxDecoder* pDec)
{
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;
    int validPicNum = ValidPictureNum(pCtx->m_decoder, 0);

    if(validPicNum <= 0)
    {
        //pCtx->callback(pCtx->pUserData, AW_OMX_CB_CONTINUE_SUBMIT, NULL);
        if(pCtx->bHadGetVideoFbmBufInfoFlag)
            OmxTimedWaitSem(pCtx->mSemValidPic, 2);
        return NULL;
    }
    if(pCtx->bUseZeroCopyBuffer)
        return liDrainZeroCopy(pCtx);
    else
        return liDrainCopy(pCtx);
}

static void __liStandbyBuffer(OmxDecoder* pDec)
{
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;\
    if(pCtx->bUseZeroCopyBuffer)
   {
        pCtx->mNeedReSetToDecoderBufferNum = 0;
        pCtx->mSetToDecoderBufferNum = 0;
        return standbyOutBufferArr(pCtx);
    }
    else
        return ;

    freeOutputBufferInfoArr(pCtx);
}

static int __liReturnBuffer(OmxDecoder* pDec)
{
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;
    if(pCtx->bUseZeroCopyBuffer)
    {
        if(!pCtx->bHadGetVideoFbmBufInfoFlag)
        {
            if(0 == liGetVideoFbmBufInfo(pCtx))
            {
                pCtx->bHadGetVideoFbmBufInfoFlag = OMX_TRUE;
                return 0;
            }
            else
            {
                pCtx->callback(pCtx->pUserData, AW_OMX_CB_CONTINUE_SUBMIT, NULL);
                if(pCtx->bInputEosFlag) //for cts test
                {
                    pCtx->callback(pCtx->pUserData, AW_OMX_CB_NOTIFY_EOS, NULL);
                }
            }
            return -1;
        }
        return returnBufferToDecdoer(pCtx);
    }
    else
    {
        if(pCtx->pPicture != NULL)
        {
            OmxAcquireMutex(pCtx->awMutexHandler);
            ReturnPicture(pCtx->m_decoder, pCtx->pPicture);
            pCtx->pPicture = NULL;
            OmxReleaseMutex(pCtx->awMutexHandler);
            OmxTryPostSem(pCtx->mSemOutBuffer);
        }
        return 0;
    }
    //return 0;
}

static inline void __liFlush(OmxDecoder* pDec)
{
    logd("decoder flush");
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;

    //* we should reset the mInputEosFlag when flush vdecoder
    pCtx->bInputEosFlag = OMX_FALSE;

    OmxTryPostSem(pCtx->mSemInData);
    OmxTryPostSem(pCtx->mSemOutBuffer);
    OmxAcquireMutex(pCtx->awMutexHandler);
    if(pCtx->m_decoder)
    {
        ResetVideoDecoder(pCtx->m_decoder);
    }
    else
    {
        logw(" fatal error, m_decoder is not malloc when flush all ports!");
    }
    if(pCtx->bUseZeroCopyBuffer)
    {
        pCtx->bFlushFlag = OMX_TRUE;
        standbyOutBufferArr(pCtx);
        pCtx->bFlushFlag = OMX_FALSE;
        pCtx->mNeedReSetToDecoderBufferNum += pCtx->mSetToDecoderBufferNum;
        pCtx->mSetToDecoderBufferNum = 0;
        SetVideoFbmBufRelease(pCtx->m_decoder);
    }
    OmxReleaseMutex(pCtx->awMutexHandler);
}

static  void __liClose(OmxDecoder* pDec)
{
    logd("decoder close");
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;
    OmxTryPostSem(pCtx->mSemInData);
    OmxTryPostSem(pCtx->mSemOutBuffer);
    OmxReleaseMutex(pCtx->awMutexHandler);
    if(pCtx->m_decoder != NULL)
    {
        DestroyVideoDecoder(pCtx->m_decoder);
        pCtx->m_decoder           = NULL;
    }
    pCtx->mCodecSpecificDataLen = 0;
    memset(pCtx->mCodecSpecificData, 0 , CODEC_SPECIFIC_DATA_LENGTH);
    pCtx->mNeedReSetToDecoderBufferNum = 0;
    pCtx->bHadGetVideoFbmBufInfoFlag   = OMX_FALSE;
    OmxReleaseMutex(pCtx->awMutexHandler);
}

static void __liSetInputEos(OmxDecoder* pDec, OMX_BOOL bEos)
{
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;
    pCtx->bInputEosFlag = bEos;
}

static int __liSetOutputEos(OmxDecoder* pDec)
{
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;
    OMX_U32 size = getPortValidSize(pCtx->pOutPort);
    logv("*** OutBufList.nSizeOfList = %lu",size);

    if(size > 0)
    {
        while (getPortValidSize(pCtx->pOutPort) > 0)
        {
            OMX_BUFFERHEADERTYPE* pOutBufHdr = doRequestPortBuffer(pCtx->pOutPort);

            if(pOutBufHdr==NULL)
                continue;

            if (pOutBufHdr->nFilledLen != 0)
            {
                pCtx->callback(pCtx->pUserData, AW_OMX_CB_FILL_BUFFER_DONE,
                               (void*)pOutBufHdr);
                pOutBufHdr = NULL;
            }
            else
            {
                logd("++++ set output eos(normal)");
                pOutBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
                pCtx->callback(pCtx->pUserData, AW_OMX_CB_FILL_BUFFER_DONE,
                               (void*)pOutBufHdr);
                pOutBufHdr = NULL;

                break;
            }
        }
    }
    return 0;
}

static void __liGetFormat(OmxDecoder* pDec)
{
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;
    pCtx->mVideoConfig.eOutputPixelFormat = PIXEL_FORMAT_SET;
}

static inline void __liSetExtBufNum(OmxDecoder* pDec, OMX_S32 num)
{
    CEDARC_UNUSE(pDec);
    CEDARC_UNUSE(num);
}

static OMX_U8* __liAllocatePortBuffer(OmxDecoder* pDec, AwOmxVdecPort* port, OMX_U32 size)
{
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;
    OMX_U8* pBuffer = NULL;
    char* nPhyaddress = NULL;
    OmxPrivateBuffer sOmxOutPrivateBuffer;
    sOmxOutPrivateBuffer.nID = 0x5A45524F43F45059;

    if(!isInputPort(port) && pCtx->bUseZeroCopyBuffer)
    {
        if(pCtx->mUseMetaDataOutBuffers == META_DATA_IN_BUFFER)
        {
            OMX_U8* pDecBuffer = (OMX_U8*) OMX_MemPalloc (pCtx->decMemOps, size);
            pBuffer = (OMX_U8*) malloc (size);
            if(!pBuffer || !pDecBuffer)
            {
                if(pBuffer)
                {
                    free(pBuffer);
                }
                if(pDecBuffer)
                {
                    OMX_MemPfree(pCtx->decMemOps, pDecBuffer);
                }
                return NULL;
            }
            sOmxOutPrivateBuffer.nShareBufFd = OMX_MemGetHandleByVirtualAddr(pCtx->decMemOps, pDecBuffer);
            if(pCtx->mIonFd > 0)
            {
                nPhyaddress = (char*)OMX_MemGetPhysicAddress(pCtx->decMemOps, pDecBuffer);
                nPhyaddress -= pCtx->nPhyOffset;
            }
            else
            {
                logd("the ion fd is wrong : fd = %d",(int)pCtx->mIonFd);
                return NULL;
            }
            if(pCtx->m_decoder)
            {
                FbmBufInfo* pFbmBufInfo =  GetVideoFbmBufInfo(pCtx->m_decoder);
                sOmxOutPrivateBuffer.pAddrPhyY = (unsigned char*)nPhyaddress;
                sOmxOutPrivateBuffer.pAddrPhyC = (unsigned char*)nPhyaddress + pFbmBufInfo->nBufWidth*pFbmBufInfo->nBufHeight;
                sOmxOutPrivateBuffer.pAddrVirY = pDecBuffer;
                sOmxOutPrivateBuffer.pAddrVirC = pDecBuffer + pFbmBufInfo->nBufWidth*pFbmBufInfo->nBufHeight;
                sOmxOutPrivateBuffer.bEnableCrop = 1;
                sOmxOutPrivateBuffer.nLeft   = pFbmBufInfo->nLeftOffset;
                sOmxOutPrivateBuffer.nTop    = pFbmBufInfo->nTopOffset;
                sOmxOutPrivateBuffer.nWidth  = \
                    pFbmBufInfo->nRightOffset - pFbmBufInfo->nLeftOffset;
                sOmxOutPrivateBuffer.nHeight = \
                    pFbmBufInfo->nBottomOffset - pFbmBufInfo->nTopOffset;
                logv("sOmxOutPrivateBuffer Crop info. left %d, top %d, width %d, height %d.",
                    sOmxOutPrivateBuffer.nLeft, sOmxOutPrivateBuffer.nTop,
                    sOmxOutPrivateBuffer.nWidth, sOmxOutPrivateBuffer.nHeight);
            }
            memcpy(pBuffer, &sOmxOutPrivateBuffer, sizeof(OmxPrivateBuffer));
            logd("META_DATA_IN_BUFFER. allocate buffer. share_fd: 0x%x. phyaddress: %p.",
                sOmxOutPrivateBuffer.nShareBufFd, nPhyaddress);
        }
        else if(pCtx->mUseMetaDataOutBuffers == META_AND_RAW_DATA_IN_BUFFER)
        {
            OMX_PARAM_PORTDEFINITIONTYPE* outDef = getPortDef(pCtx->pOutPort);
            pBuffer = OMX_MemPalloc(pCtx->decMemOps, size + 32);
            if(!pBuffer)
            {
                return NULL;
            }
            sOmxOutPrivateBuffer.nShareBufFd = OMX_MemGetHandleByVirtualAddr(pCtx->decMemOps, pBuffer);
            if(pCtx->mIonFd > 0)
            {
                nPhyaddress = (char*)OMX_MemGetPhysicAddress(pCtx->decMemOps, pBuffer);
                nPhyaddress -= pCtx->nPhyOffset;
            }
            else
            {
                logd("the ion fd is wrong : fd = %d",(int)pCtx->mIonFd);
                return NULL;
            }
            if(pCtx->m_decoder)
            {
                FbmBufInfo* pFbmBufInfo =  GetVideoFbmBufInfo(pCtx->m_decoder);
                sOmxOutPrivateBuffer.pAddrPhyY = (unsigned char*)nPhyaddress;
                sOmxOutPrivateBuffer.pAddrPhyC = (unsigned char*)nPhyaddress + pFbmBufInfo->nBufWidth*pFbmBufInfo->nBufHeight;
                sOmxOutPrivateBuffer.pAddrVirY = pBuffer;
                sOmxOutPrivateBuffer.pAddrVirC = pBuffer + pFbmBufInfo->nBufWidth*pFbmBufInfo->nBufHeight;
                sOmxOutPrivateBuffer.bEnableCrop = 1;
                sOmxOutPrivateBuffer.nLeft   = pFbmBufInfo->nLeftOffset;
                sOmxOutPrivateBuffer.nTop    = pFbmBufInfo->nTopOffset;
                sOmxOutPrivateBuffer.nWidth  = \
                        pFbmBufInfo->nRightOffset - pFbmBufInfo->nLeftOffset;
                sOmxOutPrivateBuffer.nHeight = \
                        pFbmBufInfo->nBottomOffset - pFbmBufInfo->nTopOffset;
                logv("sOmxOutPrivateBuffer Crop info. left %d, top %d, width %d, height %d.",
                    sOmxOutPrivateBuffer.nLeft, sOmxOutPrivateBuffer.nTop,
                    sOmxOutPrivateBuffer.nWidth, sOmxOutPrivateBuffer.nHeight);

            }
            pCtx->nFdOffset = outDef->format.video.nFrameWidth * outDef->format.video.nFrameHeight * 3/2;
            memcpy(pBuffer+pCtx->nFdOffset, &sOmxOutPrivateBuffer.nShareBufFd, sizeof(int));
            logd("META_AND_RAW_DATA_IN_BUFFER. allocate buffer. share_fd: 0x%x. phyaddress: %p.",
                sOmxOutPrivateBuffer.nShareBufFd, nPhyaddress);
        }
        else
        {
            pBuffer = (OMX_U8*)OMX_MemPalloc(pCtx->decMemOps, size);
        }
    }
    else
    {
        pBuffer = (OMX_U8*)malloc(size);
    }
    return pBuffer;
}

static void __liFreePortBuffer(OmxDecoder* pDec, AwOmxVdecPort* port, OMX_U8* pBuffer)
{
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;
    if(!isInputPort(port) && pCtx->bUseZeroCopyBuffer)
    {
        if(pCtx->mUseMetaDataOutBuffers == META_DATA_IN_BUFFER)
        {
            OmxPrivateBuffer sOmxOutPrivateBuffer;
            memcpy(&sOmxOutPrivateBuffer, pBuffer, sizeof(OmxPrivateBuffer));
            OMX_U8* nVirAddress = (OMX_U8*)OMX_MemGetVirtualAddrByHandle(pCtx->decMemOps,
                sOmxOutPrivateBuffer.nShareBufFd);
            logv("META_DATA_IN_BUFFER. free ion buffer 0x%p. pBuffer 0x%p.", nVirAddress, pBuffer);
            OMX_MemPfree(pCtx->decMemOps, nVirAddress);
            free(pBuffer);
        }
        else if(pCtx->mUseMetaDataOutBuffers == META_AND_RAW_DATA_IN_BUFFER)
        {
            OmxPrivateBuffer sOmxOutPrivateBuffer;
            memcpy(&sOmxOutPrivateBuffer.nShareBufFd, pBuffer+pCtx->nFdOffset, sizeof(int));
            logv("META_AND_RAW_DATA_IN_BUFFER. free sOmxOutPrivateBuffer.nShareBufFd: %d.",
            sOmxOutPrivateBuffer.nShareBufFd);
            OMX_MemPfree(pCtx->decMemOps, pBuffer);
        }
        else
        {
            OMX_MemPfree(pCtx->decMemOps, pBuffer);
        }
    }
    else
    {
        free(pBuffer);
    }
    //pBuffer = NULL;
    return ;
}

static OmxDecoderOpsT mAwDecoderOps =
{
    .getExtPara   =   __liGetExtPara,
    .setExtPara   =   __liSetExtPara,
    .getExtConfig =   __liGetExtConfig,
    .setExtConfig =   __liSetExtConfig,
    .prepare      =   __liPrepare,
    .close        =   __liClose,
    .flush        =   __liFlush,
    .decode       =   __liDecode,
    .submit       =   __liSubmit,
    .drain        =   __liDrain,
    .callback     =   __liCallback,
    .setInputEos  =   __liSetInputEos,
    .setOutputEos =   __liSetOutputEos,
    .standbyBuf   =   __liStandbyBuffer,
    .returnBuf    =   __liReturnBuffer,
    .getFormat    =   __liGetFormat,
    .setExtBufNum =   __liSetExtBufNum,
    .allocPortBuf =   __liAllocatePortBuffer,
    .freePortBuf  =   __liFreePortBuffer,
};


OmxDecoder* OmxDecoderCreate(AwOmxVdecPort* in, AwOmxVdecPort* out, OMX_BOOL bIsSecureVideoFlag)
{
    logv("OmxDecoderCreate.");
    OmxAwDecoderContext* pCtx;
    CEDARC_UNUSE(bIsSecureVideoFlag);
    pCtx = (OmxAwDecoderContext*)malloc(sizeof(OmxAwDecoderContext));
    if(pCtx == NULL)     {
        loge("malloc memory fail.");
        return NULL;
    }
    memset(pCtx, 0, sizeof(OmxAwDecoderContext));
    pCtx->pInPort = in;
    pCtx->pOutPort = out;
    pCtx->bIsFirstGetOffset = OMX_TRUE;
    pCtx->mUseMetaDataOutBuffers = META_DATA_IN_BUFFER;
    if(getenv("META_RAW_ZERO_COPY_MODE"))
        pCtx->mUseMetaDataOutBuffers = META_AND_RAW_DATA_IN_BUFFER;
    if(getenv("META_ZERO_COPY_MODE"))
        pCtx->mUseMetaDataOutBuffers = META_DATA_IN_BUFFER;
    if(getenv("RAW_ZERO_COPY_MODE"))
        pCtx->mUseMetaDataOutBuffers = RAW_DATA_IN_BUFFER;

    pCtx->nOffset = 0;

#ifndef CONF_OMX_USE_ZERO_COPY
    pCtx->bUseZeroCopyBuffer = OMX_FALSE;
#else
    pCtx->bUseZeroCopyBuffer = OMX_TRUE;
#endif
    if(getenv("NONE_ZERO_COPY_MODE"))
        pCtx->bUseZeroCopyBuffer = OMX_FALSE;
    else
        pCtx->bUseZeroCopyBuffer = OMX_TRUE;
    logd("UseZeroCopyBuffer [%d], ZeroCopyMode [%lu].", pCtx->bUseZeroCopyBuffer,
        pCtx->mUseMetaDataOutBuffers);

    pCtx->mGpuAlignStride = 16;

    OmxCreateMutex(&pCtx->awMutexHandler, OMX_TRUE);
    pCtx->mSemInData    = OmxCreateSem("InDataSem",    0, 0, OMX_FALSE);
    pCtx->mSemOutBuffer = OmxCreateSem("OutBufferSem", 0, 0, OMX_FALSE);
    pCtx->mSemValidPic  = OmxCreateSem("ValidPicSem",  0, 0, OMX_FALSE);
    //* get the phy offset
    int type = VE_OPS_TYPE_NORMAL;
    VeOpsS* veOps = GetVeOpsS(type);
    if(veOps == NULL)
    {
        loge("get ve ops failed");
    }
    else
    {
        VeConfig mVeConfig;
        memset(&mVeConfig, 0, sizeof(VeConfig));
        mVeConfig.nDecoderFlag = 1;

        void* pVeopsSelf = CdcVeInit(veOps,&mVeConfig);
        if(pVeopsSelf == NULL)
        {
            loge("init ve ops failed");
            CdcVeRelease(veOps, pVeopsSelf);
            pCtx->nPhyOffset = 0x40000000;
        }
        else
        {
            pCtx->nPhyOffset = CdcVeGetPhyOffset(veOps, pVeopsSelf);
            CdcVeRelease(veOps, pVeopsSelf);
        }
        logd("** nPhyOffset = %lx",pCtx->nPhyOffset);
    }

    pCtx->base.ops = &mAwDecoderOps;

    pCtx->decMemOps = OMX_GetMemAdapterOpsS();
    OMX_MemOpen(pCtx->decMemOps);
    pCtx->mIonFd = -1;
    pCtx->mIonFd = CdcIonOpen();
    if(pCtx->mIonFd <= 0)
        loge("***************open ion fail %d****************", (int)pCtx->mIonFd);
    return (OmxDecoder*)&pCtx->base;
}

void OmxDestroyDecoder(OmxDecoder* pDec)
{
    OmxAwDecoderContext *pCtx = (OmxAwDecoderContext*)pDec;

    if(pCtx->m_decoder != NULL)
    {
        DestroyVideoDecoder(pCtx->m_decoder);
        pCtx->m_decoder = NULL;
    }

    OmxDestroyMutex(&pCtx->awMutexHandler);
    OmxDestroySem(&pCtx->mSemInData);
    OmxDestroySem(&pCtx->mSemOutBuffer);
    OmxDestroySem(&pCtx->mSemValidPic);

    if(pCtx->mStreamInfo.pCodecSpecificData)
        free(pCtx->mStreamInfo.pCodecSpecificData);

    if(pCtx->decMemOps)
    {
        OMX_MemClose(pCtx->decMemOps);
    }
    if(pCtx->mIonFd > 0)
    {
        logd("close ion");
        CdcIonClose(pCtx->mIonFd);
    }
    free(pCtx);
    pCtx = NULL;
}

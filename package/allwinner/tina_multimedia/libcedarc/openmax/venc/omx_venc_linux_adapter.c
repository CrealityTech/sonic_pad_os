 /*
  * =====================================================================================
  *
  *      Copyright (c) 2008-2018 Allwinner Technology Co. Ltd.
  *      All rights reserved.
  *
  *       Filename:  omx_venc_linux_adapter.c
  *
  *    Description:
  *
  *        Version:  1.0
  *        Created:  2018/08
  *       Revision:  none
  *       Compiler:  gcc
  *
  *         Author:  SWC-PPD
  *      Company: Allwinner Technology Co. Ltd.
  *
  *       History :
  *        Author : AL3
  *        Date   : 2013/05/05
  *    Comment : complete the design code
  *
  * =====================================================================================
  */

#include "omx_venc_adapter.h"
OMX_ERRORTYPE getDefaultParameter(AwOmxVenc* impl, OMX_IN OMX_INDEXTYPE eParamIndex,
                                           OMX_IN OMX_PTR pParamData)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;

    switch ((OMX_INDEXEXTTYPE)eParamIndex)
    {
        case OMX_IndexParamNalStreamFormat:
        {
            OMX_NALSTREAMFORMATTYPE* pComponentParam =
            (OMX_NALSTREAMFORMATTYPE*)(pParamData);
            memcpy(pComponentParam, &impl->m_avcNaluFormat, sizeof(OMX_NALSTREAMFORMATTYPE));
            logv("get_parameter: VideoEncodeCustomParamextendedAVCNaluFormat. Nalu format %s.",
                pComponentParam->eNaluFormat== OMX_NaluFormatFourByteInterleaveLength ?
                "Enable" : "Disable");
            break;
        }

        default:
        {
            eError = OMX_ErrorUnsupportedIndex;
        }
        break;
    }
    return eError;
}
OMX_ERRORTYPE setDefaultParameter(AwOmxVenc* impl, OMX_IN OMX_INDEXTYPE eParamIndex,
                                           OMX_IN OMX_PTR pParamData)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    switch ((OMX_INDEXEXTTYPE)eParamIndex)
    {
        case OMX_IndexParamNalStreamFormat:
        {
            OMX_NALSTREAMFORMATTYPE* pComponentParam =
            (OMX_NALSTREAMFORMATTYPE*)(pParamData);
            if(pComponentParam->eNaluFormat != OMX_NaluFormatStartCodes &&
                pComponentParam->eNaluFormat != OMX_NaluFormatFourByteInterleaveLength)
            {
                eError =OMX_ErrorUnsupportedSetting;
                logw("set_parameter: omx only support OMX_NaluFormatStartCodes"
                    " and OMX_NaluFormatFourByteInterleaveLength");
                break;
            }

            logv("set_parameter: VideoEncodeCustomParamextendedAVCNaluFormat. Nalu format %s.",
                pComponentParam->eNaluFormat== OMX_NaluFormatFourByteInterleaveLength ?
                "Enable" : "Disable");
            memcpy(&impl->m_avcNaluFormat, pComponentParam, sizeof(OMX_NALSTREAMFORMATTYPE));
            break;
        }

        default:
        {
            eError = OMX_ErrorUnsupportedIndex;
        }
        break;
    }
    return eError;
}

int parseEncInputBuffer(AwOmxVenc *impl,
                                      long long aw_buf_id,
                                      int share_fd,
                                      unsigned long *phy_addr)
{
    int i;
    int ret;
    aw_ion_user_handle_t ion_handle;

    for(i=0; i<NUM_MAX_IN_BUFFERS; i++)
    {
        //parse a new share fd
        if(-1 == impl->mInputBufInfo[i].nAwBufId)
        {
            //get ion buffer fd
            logd("aw buf id: %p. share_fd: 0x%x.",(void*)aw_buf_id, share_fd);
            impl->mInputBufInfo[i].nAwBufId = aw_buf_id;
            //get ion buffer handle
            ret = CdcIonImport(impl->mIonFd, share_fd, &ion_handle);
            if(ret < 0)
            {
                loge("use CdcIonImport get ion_handle error\n");
                return -1;
            }
            impl->mInputBufInfo[i].handle_ion = ion_handle;
            impl->mInputBufInfo[i].nShareFd = share_fd;

            if(impl->mIonFd > 0)
                ret = CdcIonGetFd(impl->mIonFd, (uintptr_t)ion_handle);
            else
                loge("ion not open.");
            if(ret < 0)
            {
                loge("use CdcIonImport get ion_handle error\n");
                return -1;
            }
            impl->mInputBufInfo[i].nBufFd = ret;

            //get phy address
            if(CdcIonGetMemType() == MEMORY_IOMMU)//TODO: Not test yet!!!!
            {
                struct user_iommu_param sIommuBuf;
                sIommuBuf.fd = impl->mInputBufInfo[i].nBufFd;
                VideoEncoderGetVeIommuAddr(impl->m_encoder, &sIommuBuf);
                impl->mInputBufInfo[i].nBufPhyAddr = (unsigned long)sIommuBuf.iommu_addr;
            }
            else
            {
                impl->mInputBufInfo[i].nBufPhyAddr = CdcIonGetPhyAdr(impl->mIonFd,
                                                        (uintptr_t)ion_handle);
            }
            break;
        }
        //get already parsed share fd's index
        else if(aw_buf_id == impl->mInputBufInfo[i].nAwBufId)
        {
            break;
        }
    }
    if(NUM_MAX_IN_BUFFERS == i)
    {
        loge("the omx_venc inputBuffer num is bigger than NUM_MAX_IN_BUFFERS[%d]\n",
                                                    NUM_MAX_IN_BUFFERS);
        return -1;
    }

    *phy_addr = impl->mInputBufInfo[i].nBufPhyAddr - impl->mPhyOffset;
    logv("mInputBufInfo[%d], nShareFd [%d], nBufPhyAddr [%lx], handle [%lx], nBufFd [%d]",
                                            i,
                                            impl->mInputBufInfo[i].nShareFd,
                                            impl->mInputBufInfo[i].nBufPhyAddr,
                                            (long)impl->mInputBufInfo[i].handle_ion,
                                            impl->mInputBufInfo[i].nBufFd);
    return 0;
}

void getAndAddInputBuffer(AwOmxVenc* impl, OMX_BUFFERHEADERTYPE* pInBufHdr,
                                    VencInputBuffer* sInputBuffer)
{
    OmxPrivateBuffer sOmxInPrivateBuffer;
    unsigned long phyaddress = 0;
    OMX_U32 nFdOffset = 0;

    memset(&sOmxInPrivateBuffer, 0, sizeof(OmxPrivateBuffer));
    if(impl->m_useMetaDataInBufferMode == META_AND_RAW_DATA_IN_BUFFER)
    {
        nFdOffset = impl->m_sInPortDefType.format.video.nFrameHeight *
            impl->m_sInPortDefType.format.video.nFrameWidth * 3 / 2;
        memcpy(&sOmxInPrivateBuffer.nShareBufFd, pInBufHdr->pBuffer + pInBufHdr->nOffset + nFdOffset,
            sizeof(int));
        parseEncInputBuffer(impl, (long long)sOmxInPrivateBuffer.nShareBufFd,
        sOmxInPrivateBuffer.nShareBufFd, &phyaddress);
    }
    else if(impl->m_useMetaDataInBufferMode == META_DATA_IN_BUFFER)
    {
        memcpy(&sOmxInPrivateBuffer, pInBufHdr->pBuffer + pInBufHdr->nOffset,
            sizeof(OmxPrivateBuffer));
        parseEncInputBuffer(impl, (long long)sOmxInPrivateBuffer.nShareBufFd,
        sOmxInPrivateBuffer.nShareBufFd, &phyaddress);
    }
    else if(impl->m_useMetaDataInBufferMode == RAW_DATA_IN_BUFFER)//Ony okay for copy mode. Since pBuffer addr change.
    {
        OMX_U8* pVirAddr =  pInBufHdr->pBuffer;
        OMX_U32 nPhyOffset = 0x40000000;
        sInputBuffer->pAddrVirY = (unsigned char*) pVirAddr;
        sInputBuffer->pAddrVirC = sInputBuffer->pAddrVirY +
                impl->m_sInPortDefType.format.video.nStride *
                impl->m_sInPortDefType.format.video.nFrameHeight;
        phyaddress = (unsigned long)CdcMemGetPhysicAddressCpu(impl->memops, pVirAddr);
        phyaddress -= nPhyOffset;
    }
    else
    {
        loge("invalid useMetaDataInBuffer mode [%ld].", impl->m_useMetaDataInBufferMode);
    }

    sInputBuffer->pAddrPhyY = (unsigned char *)phyaddress;
    sInputBuffer->pAddrPhyC = sInputBuffer->pAddrPhyY +
            impl->m_sInPortDefType.format.video.nStride *
            impl->m_sInPortDefType.format.video.nFrameHeight;

    logv("Mode: %ld. pBuffer: %p. offset: %ld. Fd offset: %ld. share fd: %d. phyYBufAddr : %p.",
        impl->m_useMetaDataInBufferMode, pInBufHdr->pBuffer, pInBufHdr->nOffset, nFdOffset,
        sOmxInPrivateBuffer.nShareBufFd, sInputBuffer->pAddrPhyY);

    //* clear flag
    sInputBuffer->nFlag = 0;
    sInputBuffer->nPts = pInBufHdr->nTimeStamp;
    sInputBuffer->nID = (unsigned long)pInBufHdr;

    if (pInBufHdr->nFlags & OMX_BUFFERFLAG_EOS)
    {
        sInputBuffer->nFlag |= VENC_BUFFERFLAG_EOS;
    }

    if(sOmxInPrivateBuffer.bEnableCrop)
    {
        sInputBuffer->bEnableCorp = 1;
        sInputBuffer->sCropInfo.nLeft = sOmxInPrivateBuffer.nLeft;
        sInputBuffer->sCropInfo.nWidth = sOmxInPrivateBuffer.nWidth;
        sInputBuffer->sCropInfo.nTop = sOmxInPrivateBuffer.nTop;
        sInputBuffer->sCropInfo.nHeight = sOmxInPrivateBuffer.nHeight;
        logv("Enbale crop:%d, Left %d, Width %d, Top %d, Height %d.",
            sInputBuffer->bEnableCorp,
            sInputBuffer->sCropInfo.nLeft, sInputBuffer->sCropInfo.nWidth,
            sInputBuffer->sCropInfo.nTop, sInputBuffer->sCropInfo.nHeight);
    }
    if (impl->mVideoExtParams.bEnableCropping)
    {
        if (impl->mVideoExtParams.ui16CropLeft ||
             impl->mVideoExtParams.ui16CropTop)
        {
            sInputBuffer->bEnableCorp = 1;
            sInputBuffer->sCropInfo.nLeft =
                impl->mVideoExtParams.ui16CropLeft;
            sInputBuffer->sCropInfo.nWidth =
                impl->m_sOutPortDefType.format.video.nFrameWidth -
                impl->mVideoExtParams.ui16CropLeft -
                impl->mVideoExtParams.ui16CropRight;
            sInputBuffer->sCropInfo.nTop =
                impl->mVideoExtParams.ui16CropTop;
            sInputBuffer->sCropInfo.nHeight =
                impl->m_sOutPortDefType.format.video.nFrameHeight -
                impl->mVideoExtParams.ui16CropTop -
                impl->mVideoExtParams.ui16CropBottom;
        }
    }

    int result = AddOneInputBuffer(impl->m_encoder, sInputBuffer);
    if (result!=0)
    {
        impl->mInputBufferStep = OMX_VENC_STEP_ADD_BUFFER_TO_ENC;
    }
    else
    {
        impl->mInputBufferStep = OMX_VENC_STEP_GET_INPUTBUFFER;
    }
}

void getInputBufferFromBufHdr (AwOmxVenc* impl, OMX_BUFFERHEADERTYPE* pInBufHdr,
                                                                VencInputBuffer* sInputBuffer)
{
    logv("function %s. line %d.", __FUNCTION__, __LINE__);
    if (impl->mVideoExtParams.bEnableCropping)
    {
        if (impl->mVideoExtParams.ui16CropLeft ||
            impl->mVideoExtParams.ui16CropTop)
        {
            sInputBuffer->bEnableCorp = 1;
            sInputBuffer->sCropInfo.nLeft =
                impl->mVideoExtParams.ui16CropLeft;
            sInputBuffer->sCropInfo.nWidth =
                impl->m_sOutPortDefType.format.video.nFrameWidth -
                impl->mVideoExtParams.ui16CropLeft -
                impl->mVideoExtParams.ui16CropRight;
            sInputBuffer->sCropInfo.nTop =
                impl->mVideoExtParams.ui16CropTop;
            sInputBuffer->sCropInfo.nHeight =
                impl->m_sOutPortDefType.format.video.nFrameHeight -
                impl->mVideoExtParams.ui16CropTop -
                impl->mVideoExtParams.ui16CropBottom;
        }
    }
    memcpy(sInputBuffer->pAddrVirY,
           pInBufHdr->pBuffer + pInBufHdr->nOffset, impl->mSizeY);
    memcpy(sInputBuffer->pAddrVirC,
           pInBufHdr->pBuffer + pInBufHdr->nOffset + impl->mSizeY, impl->mSizeC);
    impl->mSizeY = 0;
    impl->mSizeC = 0;

}

void getInputBufferFromBufHdrWithoutCrop(AwOmxVenc* impl, OMX_BUFFERHEADERTYPE* pInBufHdr,
                                                                VencInputBuffer* sInputBuffer)
{
    logv("function %s. line %d.", __FUNCTION__, __LINE__);
    memcpy(sInputBuffer->pAddrVirY,
           pInBufHdr->pBuffer + pInBufHdr->nOffset, impl->mSizeY);
    memcpy(sInputBuffer->pAddrVirC,
           pInBufHdr->pBuffer + pInBufHdr->nOffset + impl->mSizeY, impl->mSizeC);
    impl->mSizeY = 0;
    impl->mSizeC = 0;
}

void determineVencColorFormat(AwOmxVenc* impl)
{
    switch (impl->m_sInPortFormatType.eColorFormat)
    {
        case OMX_COLOR_FormatYUV420SemiPlanar:
        {
            logd("color format: VENC_PIXEL_YUV420SP/NV12");
            impl->m_vencColorFormat = VENC_PIXEL_YUV420SP;
            break;
        }
        case OMX_COLOR_FormatYVU420Planar:
        {
            logd("color format: VENC_PIXEL_YVU420P/YV12");
            impl->m_vencColorFormat = VENC_PIXEL_YVU420P;
            break;
        }
        case OMX_COLOR_FormatYVU420SemiPlanar:
        {
            logd("color format: VENC_PIXEL_YVU420SP/NV21");
            impl->m_vencColorFormat = VENC_PIXEL_YVU420SP;
            break;
        }
        case OMX_COLOR_FormatYUV420Planar:
        {
            logd("color format: VENC_PIXEL_YUV420P/I420");
            impl->m_vencColorFormat = VENC_PIXEL_YUV420P;
            break;
        }
        default:
        {
            logw("determine none format 0x%x!!!!", impl->m_sInPortFormatType.eColorFormat);
            break;
        }

    }
}

void determineVencBufStrideIfNecessary(AwOmxVenc* impl, VencBaseConfig* pBaseConfig)
{
    CEDARC_UNUSE(impl);
    CEDARC_UNUSE(pBaseConfig);
    //do nothing.
}

int deparseEncInputBuffer(int nIonFd,
                                VideoEncoder *pEncoder,
                                OMXInputBufferInfoT *pInputBufInfo)
{
    OMX_S32 i;
    int ret;
    for(i=0; i<NUM_MAX_IN_BUFFERS; i++)
    {
        OMXInputBufferInfoT *p;
        p = pInputBufInfo + i;

        if(p->nAwBufId != -1)
        {
            if(CdcIonGetMemType() == MEMORY_IOMMU)
            {
                struct user_iommu_param sIommuBuf;
                sIommuBuf.fd = p->nBufFd;
                VideoEncoderFreeVeIommuAddr(pEncoder, &sIommuBuf);
             }

            //close buf fd
            if(p->nBufFd != -1)
            {
                ret = CdcIonClose(p->nBufFd);
                if(ret < 0)
                {
                    loge("CdcIonClose close buf fd error\n");
                    return -1;
                }
            }

            //free ion handle
            if(p->handle_ion)
            {
                ret = CdcIonFree(nIonFd, p->handle_ion);
                if(ret < 0)
                {
                    loge("CdcIonFree free ion_handle error\n");
                    return -1;
                }
            }

            p->nShareFd = -1;
        }
    }
    return 0;
}

void updateOmxDebugFlag(AwOmxVenc* impl)
{
    char* envchar = getenv("DEBUG_TYPE");
    logd("DEBUG_TYPE is %s.", envchar);
    if(envchar)
    {
        char* debugType = "SAVE_BIT";
        if(!strncasecmp(envchar, debugType, 8))
            impl->bSaveStreamFlag = OMX_TRUE;
        debugType = "SAVE_INPUT";
        if(!strncasecmp(envchar, debugType, 10))
            impl->bSaveInputDataFlag = OMX_TRUE;
        debugType = "OPEN_STATISTICS";
        if(!strncasecmp(envchar, debugType, 15))
            impl->bOpenStatisticFlag = OMX_TRUE;
        debugType = "SHOW_FRAMESIZE";
        if(!strncasecmp(envchar, debugType, 14))
            impl->bShowFrameSizeFlag = OMX_TRUE;
    }

    #if SAVE_BITSTREAM
    impl->bSaveStreamFlag = OMX_TRUE;
    #endif
    #if OPEN_STATISTICS
    impl->bOpenStatisticFlag = OMX_TRUE;
    #endif
    #if PRINTF_FRAME_SIZE
    impl->bShowFrameSizeFlag = OMX_TRUE;
    #endif
    #if SAVE_INPUTDATA
    impl->bSaveInputDataFlag = OMX_TRUE;
    #endif
}

OMX_U32 getFrameRateFps(OMX_U32 nFramerate)
{
    return nFramerate;
}

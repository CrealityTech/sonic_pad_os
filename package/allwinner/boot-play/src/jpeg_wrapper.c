/*
 * @Author: your name
 * @Date: 2021-07-05 17:53:10
 * @LastEditTime: 2021-07-06 15:49:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /package/allwinner/boot-play/src/jpeg_wrapper.c
 */
/*
 * Copyright (c) 2008-2016 Allwinner Technology Co. Ltd.
 * All rights reserved.
 *
 * File : xmetademo.c
 * Description : xmetademo
 * History :
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <errno.h>
#include "jpeg_wrapper.h"
#include "log.h"

/**
 * @description: 
 * @param {struct JpegDecoderWrapper_t} *DecoderWrapper
 * @return {*} 0: success; -1: error
 */
int JpegDecoderInit(struct JpegDecoderWrapper_t *DecoderWrapper){
    DecoderWrapper->JpegDecoder =  JpegDecoderCreate();
    if(NULL == DecoderWrapper->JpegDecoder)
    {
        printf("create jpegdecoder failed\n");
        return -1;
    }
    return 0;
}

/**
 * @description: 
 * @param {char} *Path
 * @param {struct JpegDecoderWrapper_t} *DecoderWrapper
 * @return {*}
 */
int JpegDecompress(char *Path, struct JpegDecoderWrapper_t *DecoderWrapper){

    JpegDecoderSetDataSource(DecoderWrapper->JpegDecoder, Path,
                                DecoderWrapper->ScaleRatio, DecoderWrapper->OutputDataTpe);
    DecoderWrapper->ImgFrame = JpegDecoderGetFrame(DecoderWrapper->JpegDecoder);
    if(NULL == DecoderWrapper->ImgFrame){
        printf("[%s] %s %d JpegDecoderGetFrame fail\n", __FILE__, __func__, __LINE__);
        return -1;
    }
    return 0;
}

/**
 * @description: 
 * @param {struct JpegDecoderWrapper_t} *DecoderWrapper
 * @return {*}
 */
int JpegDecoderUnInit(struct JpegDecoderWrapper_t *DecoderWrapper){
    if(DecoderWrapper->JpegDecoder){
        JpegDecoderDestory(DecoderWrapper->JpegDecoder);
        DecoderWrapper->JpegDecoder = NULL;
    }
}
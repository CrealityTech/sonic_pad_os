/*
 * @Author: your name
 * @Date: 2021-07-05 11:32:13
 * @LastEditTime: 2021-07-06 16:00:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /package/allwinner/boot-play/src/de_display.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <linux/fb.h>
#include <linux/kernel.h>
#include "de_display.h"

int de_display_init(struct dedisplay *de)
{
	VoutRect rect;
	
	de->mDispOutPort = CreateVideoOutport(0);
	
    if(de->mDispOutPort == NULL){
		printf("CreateVideoOutport ERR");
		return -1;
	}

	rect.x = 0;
	rect.y = 0;
	rect.width = de->width;
	rect.height = de->height;
	
	de->mDispOutPort->init(de->mDispOutPort, 0, 0, &rect);
	
	rect.x = 0;
	rect.y = 0;
	rect.width = de->mDispOutPort->getScreenWidth(de->mDispOutPort);
	rect.height = de->mDispOutPort->getScreenHeight(de->mDispOutPort);
    de->mDispOutPort->setRect(de->mDispOutPort,&rect);
	
	de->pMemops = GetMemAdapterOpsS();
    SunxiMemOpen(de->pMemops);
	
	de->pVirBuf = (char*)SunxiMemPalloc(de->pMemops, de->width * de->height * 4);
	de->pPhyBuf = (char*)SunxiMemGetPhysicAddressCpu(de->pMemops, de->pVirBuf);

    memset((void*)de->pVirBuf, 0x0, de->width * de->height * 4);
	
	return 0;
}

void de_disp_display(struct dedisplay *de, void *buf, unsigned int len, VideoPixelFormat format){

    videoParam vparam;
    renderBuf rBuf;

	if(!buf && len < 0)
		return;

    memcpy(de->pVirBuf, buf, len);

    SunxiMemFlushCache(de->pMemops, (void*)(de->pVirBuf), de->width * de->height * 4);

    vparam.srcInfo.crop_x = 0;
	vparam.srcInfo.crop_y = 0;
	vparam.srcInfo.crop_w = de->width;
	vparam.srcInfo.crop_h = de->height;

	vparam.srcInfo.w = de->width;
	vparam.srcInfo.h = de->height;
	vparam.srcInfo.color_space = VIDEO_BT601;
    vparam.srcInfo.format = format;
    
	switch(format){
        case VIDEO_PIXEL_FORMAT_NV21 :
            rBuf.y_phaddr = (unsigned long)(de->pPhyBuf);
            rBuf.v_phaddr = (unsigned long)(de->pPhyBuf + de->width * de->height);
            rBuf.u_phaddr = (unsigned long)(de->pPhyBuf + de->width * de->height);
            break;
        case VIDEO_PIXEL_FORMAT_YV12 : 
            rBuf.y_phaddr = (unsigned long)de->pPhyBuf;
            rBuf.u_phaddr = (unsigned long)(de->pPhyBuf + de->width * de->height);
            rBuf.v_phaddr = (unsigned long)(de->pPhyBuf + de->width * de->height * 5 / 4);
            break;
	case VIDEO_PIXEL_FORMAT_YUV_PLANER_420 :
		rBuf.y_phaddr = (unsigned long)de->pPhyBuf;
		rBuf.u_phaddr = (unsigned long)(de->pPhyBuf + de->width * de->height);
		rBuf.v_phaddr = (unsigned long)(de->pPhyBuf + de->width * de->height * 5 / 4);
		break;
    }
	rBuf.isExtPhy = VIDEO_USE_EXTERN_ION_BUF;
	
    de->mDispOutPort->queueToDisplay(de->mDispOutPort, vparam.srcInfo.w*vparam.srcInfo.h*4, &vparam, &rBuf);
	de->mDispOutPort->SetZorder(de->mDispOutPort, VIDEO_ZORDER_MIDDLE);
	de->mDispOutPort->setEnable(de->mDispOutPort, 1);
}

void de_disp_uninit(struct dedisplay *de){

	if(de->mDispOutPort)
    	de->mDispOutPort->setEnable(de->mDispOutPort, 0);
	if(de->pMemops && de->pVirBuf){
		SunxiMemPfree(de->pMemops, de->pVirBuf);
		SunxiMemClose(de->pMemops);
		de->pMemops = NULL;
		de->pVirBuf = NULL;
	}

	if(de->mDispOutPort){
		de->mDispOutPort->deinit(de->mDispOutPort);
		DestroyVideoOutport(de->mDispOutPort);
		de->mDispOutPort =NULL;
	}
}
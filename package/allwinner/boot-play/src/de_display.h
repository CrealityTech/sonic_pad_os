#ifndef __DE_DISPLAY_H__
#define __DE_DISPLAY_H__

#include <ion_mem_alloc.h>
#include <videoOutPort.h>

struct dedisplay
{
    dispOutPort *mDispOutPort;
    struct SunxiMemOpsS *pMemops;
    char* pPhyBuf;
    char* pVirBuf;
    unsigned int width;
    unsigned int height;
};

int de_display_init(struct dedisplay *de);
void de_disp_display(struct dedisplay *de, void *buf, unsigned int len, VideoPixelFormat format);
void de_disp_uninit(struct dedisplay *de);

#endif

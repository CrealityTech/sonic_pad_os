#ifndef __JPEG_WRAPPER_H__
#define __JPEG_WRAPPER_H__

#include "jpegdecode.h"

struct JpegDecoderWrapper_t
{
    JpegDecoder* JpegDecoder;
    JpegDecodeScaleDownRatio ScaleRatio;
    JpegDecodeOutputDataType OutputDataTpe;
    ImgFrame* ImgFrame;
};

int JpegDecoderInit(struct JpegDecoderWrapper_t *DecoderWrapper);

int JpegDecompress(char *Path, struct JpegDecoderWrapper_t *DecoderWrapper);

int JpegDecoderUnInit(struct JpegDecoderWrapper_t *DecoderWrapper);


#endif

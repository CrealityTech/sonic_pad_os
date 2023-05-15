#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

#include "cameraInfo.h"

#define CLEAR(x) memset(&(x), 0, sizeof(x))

enum io_method {
        IO_METHOD_READ,
        IO_METHOD_MMAP,
        IO_METHOD_USERPTR,
};

struct buffer {
        void   *start;
        size_t  length;
};

//这里的fd是用于打开vidio的句柄
static int              fd = -1;
static enum io_method   io = IO_METHOD_MMAP;

static void errno_exit(const char *s)
{
        fprintf(stderr, "Error: %s error %d, %s\n", s, errno, strerror(errno));
        exit(EXIT_FAILURE);
}

static int xioctl(int fh, int request, void *arg)
{
        int r;

        do {
                r = ioctl(fh, request, arg);
        } while (-1 == r && EINTR == errno);

        return r;
}

static int enum_frame_intervals(int dev, __u32 pixfmt, __u32 width, __u32 height)
{
    int ret;
    struct v4l2_frmivalenum fival;

    memset(&fival, 0, sizeof(fival));
    fival.index = 0;
    fival.pixel_format = pixfmt;
    fival.width = width;
    fival.height = height;
    while ((ret = ioctl(dev, VIDIOC_ENUM_FRAMEINTERVALS, &fival)) == 0) {
        if (fival.type == V4L2_FRMIVAL_TYPE_DISCRETE) {
            printf("\"fps\":%u}", fival.discrete.denominator);
        } else if (fival.type == V4L2_FRMIVAL_TYPE_CONTINUOUS) {
            printf("\nError: it won't support the type \"V4L2_FRMIVAL_TYPE_CONTINUOUS\".\n");
            return -1;
        } else if (fival.type == V4L2_FRMIVAL_TYPE_STEPWISE) {
            printf("\nError: it won't support the type \"V4L2_FRMIVAL_TYPE_STEPWISE\".\n");
            return -1;
        }
        fival.index++;
    }
    printf("\n");
    if (ret != 0 && errno != EINVAL) {
        printf("ERROR enumerating frame intervals: %d\n", errno);
        return errno;
    }

    return 0;
}

static int enum_frame_sizes(int dev, __u32 pixfmt)
{
    int ret;
    struct v4l2_frmsizeenum fsize;

    memset(&fsize, 0, sizeof(fsize));
    fsize.index = 0;
    fsize.pixel_format = pixfmt;
    while ((ret = ioctl(dev, VIDIOC_ENUM_FRAMESIZES, &fsize)) == 0) {
        if (fsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
            printf("{\"width\":%u,\"height\":%u,",
                   fsize.discrete.width, fsize.discrete.height);
            ret = enum_frame_intervals(dev, pixfmt,
                                       fsize.discrete.width, fsize.discrete.height);
            if (ret != 0)
            {
                printf("Error: Unable to enumerate frame sizes.\n");
                break;
            }
        } else if (fsize.type == V4L2_FRMSIZE_TYPE_CONTINUOUS) {
            printf("Error: it won't support the type \"V4L2_FRMSIZE_TYPE_CONTINUOUS\".\n");
            break;
        } else if (fsize.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
            printf("Error: it won't support the type \"V4L2_FRMSIZE_TYPE_STEPWISE\".\n");
            break;
        }
        fsize.index++;
    }
    if (ret != 0 && errno != EINVAL) {
        printf("ERROR enumerating frame sizes: %d\n", errno);
        return errno;
    }

    return 0;
}

static int enum_frame_formats()
{
    int ret;
    struct v4l2_fmtdesc fmt;

    memset(&fmt, 0, sizeof(fmt));
    fmt.index = 0;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while ((ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmt)) == 0) {
        fmt.index++;
        printf("{\"pixelformat\":'%c%c%c%c',\"description\":'%s'}\n",
               fmt.pixelformat & 0xFF, (fmt.pixelformat >> 8) & 0xFF,
               (fmt.pixelformat >> 16) & 0xFF, (fmt.pixelformat >> 24) & 0xFF,
               fmt.description);
        printf("support pixels:\n");
        ret = enum_frame_sizes(fd, fmt.pixelformat);
        if (ret != 0)
            printf("  Unable to enumerate frame sizes.\n");
    }
    if (errno != EINVAL) {
        printf("ERROR enumerating frame formats: %d\n", errno);
        return errno;
    }

    return 0;
}

void init_device(char *devname)
{
        struct v4l2_capability cap;

        fd = open(devname, O_RDWR /* required */ | O_NONBLOCK, 0);

        if (-1 == fd) {
                fprintf(stderr, "Error: Cannot open '%s': %d, %s\n",
                         devname, errno, strerror(errno));
                exit(EXIT_FAILURE);
        }
        
        //输出视频帧格式，例如宽高等数据
        enum_frame_formats();

        //查询设备功能
        if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "Error: %s is no V4L2 device\n",
                                 devname);
                        exit(EXIT_FAILURE);
                } else {
                        errno_exit("VIDIOC_QUERYCAP");
                }
        }

        //判断设备是否支持单个平面
        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
                fprintf(stderr, "Error: %s is no video capture device\n",
                         devname);
                exit(EXIT_FAILURE);
        }

        switch (io) {
        case IO_METHOD_READ:
                //设备支持读写操作
                if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
                        fprintf(stderr, "Error: %s does not support read i/o\n",
                                 devname);
                        exit(EXIT_FAILURE);
                }
                break;

        case IO_METHOD_MMAP:
        case IO_METHOD_USERPTR:
                //设备支持stream流模式
                if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
                        fprintf(stderr, "Error: %s does not support streaming i/o\n",
                                 devname);
                        exit(EXIT_FAILURE);
                }
                break;
        }
}

void uninit_device(void)
{

}
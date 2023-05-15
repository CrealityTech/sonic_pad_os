#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <getopt.h>             /* getopt_long() */
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "cameraInfo.h"

char *dev_name = "/dev/video0";

int main(int argc, char** argv)
{
    if(argc >= 2)
        dev_name = argv[1];
    init_device(dev_name);

    uninit_device();

    return 0;
}

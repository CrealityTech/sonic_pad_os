#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include "sunxi_display_v2.h"


#define DISP_PATH "/dev/disp"
#define SCREEN_ID 0

static void parse_args(int argc, char *argv[])
{
    static struct option longOptions[] = {
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };

    for (;;)
    {
        int optidx = 0;
        int c;

        c = getopt_long(argc, argv, "h", longOptions, &optidx);
        if (-1 == c)
            break;

        switch (c)
        {
            case 'h':
                printf("\nUSAGE: lcd_display [value]\n \
                        0: off \n \
                        1: on  \n");
            default:
                exit(EXIT_FAILURE);
        }
    }
}


int main(int argc, char **argv)
{
    //printf("lcd display argc:%d, first string:%s \n", argc, argv[0]);

    if(argc != 2)
    {
        printf("param is invalid, please use the help command to view the usage method :lcd_display -h \n");
        return -1;
    }

    parse_args(argc, argv);

    int disp_fd = -1, ret = -1;

    unsigned long param[4] = {0};
    param[0] = (unsigned long)SCREEN_ID;
    param[1] = (unsigned long)strtol(argv[1], NULL, 0);

    disp_fd = open(DISP_PATH, O_RDWR);
    if (disp_fd == -1)
    {
        printf("open display device faild ( %s )\n", strerror(errno));
        goto err;
    }

    ret = ioctl(disp_fd, DISP_DEVICE_SWITCH, param);
    if (ret == -1) {
        printf("ioctl failed.( %s )\n", strerror(errno));
        goto err;
    }

    close(disp_fd);
    return 0;

err:
    close(disp_fd);
    return -1;

}



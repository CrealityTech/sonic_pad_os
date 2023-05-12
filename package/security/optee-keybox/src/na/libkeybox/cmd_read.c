
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#define CMDLINE_FILE_PATH ("/proc/cmdline")

int main(void)
{
    int ret;
    int fd;
    char cmdline[1024];

    memset(cmdline, 0, 1024);

    fd = open(CMDLINE_FILE_PATH, O_RDONLY);
    if (!fd) {
        printf("Failed to open %s", CMDLINE_FILE_PATH);
        return -1;
    }

    ret = read(fd, cmdline, 1024);
    if (ret < 0 || ret > 1024) {
        printf("Cmd line read fail: ret=%d, %s\n", ret, strerror(errno));
        close(fd);
        return -1;
    }
	printf("cmdline:%s\n",cmdline);
    close(fd);
	return 0;
}

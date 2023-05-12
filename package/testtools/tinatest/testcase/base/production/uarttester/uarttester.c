#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <linux/serial.h>

#include "mjson.h"
#include "interact.h"

#define UART_TESTCASE_PATH	"/base/production/uarttester/"
#define UART_PORT		UART_TESTCASE_PATH"uart_port"
#define UART_BAUD		UART_TESTCASE_PATH"uart_baud"
#define TEST_CLCLE		UART_TESTCASE_PATH"test_cycles"
#define TEST_BYTES		UART_TESTCASE_PATH"test_bytes_per_cycle"


/* uart收发字符串，以TRANS_BUF_FILL填充 */
#define TRANS_BUF_FILL		"abcd"

enum parameter_type {
        PT_PROGRAM_NAME = 0,
        PT_DEV_PORT,
        PT_DEV_BAUDRATE,
        PT_CYCLE,
        PT_DATA_SIZE,
        PT_NUM,
};

void usage(void)
{
        printf("You should input as: \n");
        printf("\t uart_test [uart_port] [baudrate] [cycle count] [bytes_per_cycle]\n");
	printf("\t example: use uart3 send and receive 100 cycle, trans 100 bytes every cycle:\n");
	printf("\t\t\"./uart 3 115200 100 1024\"\n");

	printf("\t one board: tx <--------> rx\n");
	printf("\t two board: \n");
	printf("\t            tx <--------> rx\n");
	printf("\t            rx <--------> tx\n");
}

int open_dev(char *name)
{
        int fd = open(name, O_RDWR);
        if (-1 == fd)
                printf("Can't Open(%s)!", name);

        return fd;
}

/**
*@brief  设置串口通信速率
*@param  fd     类型 int  打开串口的文件句柄
*@param  speed  类型 int  串口速度
*@return  void
*/
int set_speed(int fd, int speed){
        int   i;
        int   status;
        struct termios  Opt = {0};
        int speed_arr[] = { B230400, B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300 };
        int name_arr[] = { 230400, 115200, 38400,  19200,  9600,  4800,	2400,  1200,  300, 38400 };

        tcgetattr(fd, &Opt);

        for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++) {
                if  (speed == name_arr[i])
                        break;
        }

        if (i == sizeof(speed_arr) / sizeof(int))
                return -1;

        tcflush(fd, TCIOFLUSH);
        cfsetispeed(&Opt, speed_arr[i]);
        cfsetospeed(&Opt, speed_arr[i]);

        Opt.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
        Opt.c_oflag  &= ~OPOST;   /*Output*/

        status = tcsetattr(fd, TCSANOW, &Opt);
        if  (status != 0) {
                printf("tcsetattr error");
                return -1;
        }
        tcflush(fd, TCIOFLUSH);
        return 0;
}

/**
*@brief   设置串口数据位，停止位和效验位
*@param  fd     类型  int  打开的串口文件句柄
*@param  databits 类型  int 数据位   取值 为 7 或者8
*@param  stopbits 类型  int 停止位   取值为 1 或者2
*@param  parity  类型  int  效验类型 取值为N,E,O,,S
*/
int set_parity(int fd,int databits,int stopbits,int parity)
{
        struct termios options;

        if ( tcgetattr( fd,&options)  !=  0) {
                perror("SetupSerial 1");
                return -1;
        }
        options.c_cflag &= ~CSIZE;

        switch (databits) /*设置数据位数*/
        {
                case 7:
                        options.c_cflag |= CS7;
                        break;
                case 8:
                        options.c_cflag |= CS8;
                        break;
                default:
                        fprintf(stderr,"Unsupported data size\n");
                        return -1;
        }

        switch (parity)
        {
                case 'n':
                case 'N':
                        options.c_cflag &= ~PARENB;
                        options.c_iflag &= ~INPCK;
                        break;
                case 'o':
                case 'O':
                        options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
                        options.c_iflag |= INPCK;
                        break;
                case 'e':
                case 'E':
                        options.c_cflag |= PARENB;
                        options.c_cflag &= ~PARODD;   /* 转换为偶效验*/
                        options.c_iflag |= INPCK;
                        break;
                case 'S':
                case 's':  /*as no parity*/
                        options.c_cflag &= ~PARENB;
                        options.c_cflag &= ~CSTOPB;break;
                default:
                        fprintf(stderr,"Unsupported parity\n");
                        return -1;
        }

        /* 设置停止位*/
        switch (stopbits)
        {
                case 1:
                        options.c_cflag &= ~CSTOPB;
                        break;
                case 2:
                        options.c_cflag |= CSTOPB;
                        break;
                default:
                        fprintf(stderr,"Unsupported stop bits\n");
                        return -1;
        }

        /* Set input parity option */
        if (parity != 'n')
                options.c_iflag |= INPCK;
        tcflush(fd,TCIFLUSH);
        options.c_cc[VTIME] = 150;
        options.c_cc[VMIN] = 0;
        if (tcsetattr(fd,TCSANOW,&options) != 0)
        {
                perror("SetupSerial 3");
                return -1;
        }
        return 0;
}

int main(int argc, char **argv)
{
        int i = 0, temp = 0, ret = -1;
        int fd = 0;
        int cnt = 0, err_cnt = 0, read_size = 0, write_size = 0;

        int baudrate;
        int data_size;
        char *data_buf = NULL;
        char *receive_buf = NULL;
	char uart_port[16] = {0};
        char buf[256];

        fd_set rd_fdset;
        struct timeval dly_tm;          // delay time in select()

	if (argc == 1) {
		/* used tinatest default config. */
#if 1
		int port;
		port = mjson_fetch_int(UART_PORT);
		sprintf(uart_port, "/dev/ttyS%d", port);

		baudrate = mjson_fetch_int(UART_BAUD);
		cnt  = mjson_fetch_int(TEST_CLCLE);
		data_size = mjson_fetch_int(TEST_BYTES);
#endif

	} else if (argc > 1 && argc != PT_NUM) {
                usage();
                return -1;
        } else {
		int port;
		sscanf(argv[PT_DEV_PORT], "%d", &port);
		sprintf(uart_port, "/dev/ttyS%d", port);

		sscanf(argv[PT_DEV_BAUDRATE], "%d", &baudrate);

		sscanf(argv[PT_CYCLE], "%d", &cnt);

		sscanf(argv[PT_DATA_SIZE], "%d", &data_size);

	}
	printf("uart_port: %s.\n", uart_port);
	printf("baudrate: %d.\n", baudrate);
	printf("test cycle: %d.\n", cnt);
	printf("bytes_per_cycle: %d.\n", data_size);

        /* transfer buffer init */
        data_buf = malloc(data_size + 1);
        if(NULL == data_buf) {
                printf("malloc for data_buf error\n");
                return -1;
        }
        memset(data_buf, 0 , data_size);
        for(i = 0 ; i < data_size; i = i + 4) {
                if(data_size < i+4)
                        break;
                strcpy(data_buf+i, TRANS_BUF_FILL);
        }
        data_buf[data_size] = '\0';

        receive_buf = malloc(data_size + 1);
        if(NULL == receive_buf) {
                printf("malloc for receive_buf error\n");
                return -1;
        }
        memset(receive_buf, 0 , data_size + 1);

        /* serial init */
        fd = open_dev(uart_port);
        if (fd < 0)
                return -1;

        if(set_speed(fd, baudrate) < 0) {
                printf("set baudrate error\n");
                return -1;
        }

        if (set_parity(fd,8,1,'N') <0) {
                printf("set parity Error\n");
                return -1;
        }

        /* begin transmit and receive test */

        /* the first time transmit, do not use select */
        /* or it will always printf "wait data timeout", until other board run uart-test */
        tcflush(fd,TCIOFLUSH);
        write_size = write(fd, data_buf, data_size);
        printf("transmit %d bytes.\n", write_size);

        i = 0;
        memset(receive_buf, 0, data_size + 1);
        while (read_size != data_size) {
                memset(buf, 0, sizeof(buf));
                temp = read(fd, buf, 255);
                if(temp <= 0)
                        break;
                read_size += temp;
                strcat(receive_buf, buf);
        }
        i++;
        tcflush(fd,TCIFLUSH);
        receive_buf[read_size+1] = '\0';
	printf("recv %d bytes\n", read_size);
        read_size = 0;

        if(!strcmp(receive_buf, data_buf)) {
                tcflush(fd, TCOFLUSH);
                write_size = write(fd, data_buf, data_size);
                printf("transmits %d byte.\n", write_size);
        }
        else {
                err_cnt++;
                printf("\n%d times : receive data error.\n", i);
        }

        /* the second time start transmit */
        while (i < cnt) {
                i++;
                FD_ZERO(&rd_fdset);
                FD_SET(fd, &rd_fdset);
                dly_tm.tv_sec  = 1;
                dly_tm.tv_usec = 0;

                ret = select(fd+1, &rd_fdset, NULL, NULL, &dly_tm);
                /* wait timeout */
                if (ret == 0) {
                        err_cnt++;
                        printf("\n%d times : wait data timeout.\n", i);
                        goto begin_next_test;
                }

                /* select error */
                if (ret < 0) {
                        err_cnt++;
                        printf("select(%s) return %d. [%d]: %s \n", argv[PT_DEV_PORT], ret, errno, strerror(errno));
                        goto begin_next_test;

                }

                memset(receive_buf, 0, data_size + 1);
                while (read_size != data_size) {
                        memset(buf, 0, sizeof(buf));
                        temp = read(fd, buf, 255);
                        if(temp <= 0)
                                break;
                        read_size += temp;
                        strcat(receive_buf, buf);
                }
                tcflush(fd,TCIFLUSH);
                receive_buf[read_size+1] = '\0';
		printf("recv %d bytes\n", read_size);
                read_size = 0;

                if(!strcmp(receive_buf,data_buf)) {
                        goto begin_next_test;
                }
                else {
                        err_cnt++;
                        printf("\n%d times : receive data error.\n", i);
                        goto begin_next_test;

                }

begin_next_test:
                tcflush(fd,TCOFLUSH);
                write_size = write(fd, data_buf, data_size);
                printf("transmit %d bytes.\n", write_size);

        }
        printf("\n\nResult:\n");
        printf("\ttest %d times, transmit %d every time.\n", i, data_size);
        printf("\t%d times success, %d times fail.\n",i - err_cnt, err_cnt);

        close(fd);

	if (err_cnt)
		return -1;

        return 0;
}

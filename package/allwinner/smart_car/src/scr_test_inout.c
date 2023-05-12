#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <linux/poll.h>
#include "sunxi-scr-user.h"


int main(int argc, char **argv)
{
	int i = 0;
	static int scr_fd;
	struct scr_card_para card_param;
	struct smc_atr_para smc_atr_para;
	struct smc_pps_para smc_pps_para;
	struct scr_atr scr_atr_des;
	struct scr_wr_data wr_data;

	uint8_t dataSend[128]={0x0};
	uint8_t rx_buf[128]={0};
	int ret=0;
	int rx_len=0;
	uint8_t sw1 = 0;
	uint8_t sw2 = 0;
	int time_us=0, test_num=1;
	int det=0;
	unsigned int start_time=0, end_time=0;
	int try_times = 5;

	fd_set rds;
	struct pollfd scr_pollfd;

	scr_fd = open("/dev/smartcard", O_RDWR);
	if (scr_fd < 0){
		printf("vedic:open fail!\n");
		return -1;
	}

	scr_pollfd.fd = scr_fd;
	scr_pollfd.events = POLLIN|POLLOUT;
	scr_pollfd.revents = 0;

	while(1){
		if (-1 == poll(&scr_pollfd, 1, -1)) {
			printf("poll fail!\n");
			return -1;
		}

		if (scr_pollfd.revents & POLLIN) {
			printf("card in!\n");
#if 1
			if(!ioctl(scr_fd, SCR_IOCGSTATUS, &det)){
				if (0 == det) {
					printf("not really card in\n");
					return -1;
				}
			} else {
				printf(">>>>>> SCR_IOCGSTATUS fail!\n");
				return -1;
			}
			
			if(ioctl(scr_fd, SCR_IOCRESET, NULL)){
				printf("SCR_IOCRESET fail!\n");
				return -1;
			}

			if(!ioctl(scr_fd, SCR_IOCGATR, &scr_atr_des)){
				for(i=0;i<scr_atr_des.atr_len;i++)
					printf("atr_data[%d]=0x%02x\n", i, scr_atr_des.atr_data[i]);
			} else {
				printf(">>>>>> SCR_IOCGATR fail!\n");
				return -1;
			}

			dataSend[0]=0xe5;
			dataSend[1]=0x04;
			dataSend[2]=0x00;
			dataSend[3]=0x00;
			dataSend[4]=0x04;
			
			memset(rx_buf, 0, 128);
			sw1=sw2=0;
			wr_data.cmd_buf = dataSend;
			wr_data.cmd_len = 5;
			wr_data.rtn_data = rx_buf;
			wr_data.rtn_len = &rx_len;
			wr_data.psw1 = &sw1;
			wr_data.psw2 = &sw2;
			ret = ioctl(scr_fd, SCR_IOCWRDATA, &wr_data);
			for(i = 0; i < rx_len; i++)
				printf("rx_buf[%d] = %d\n", i, rx_buf[i]);

			printf("sw1=0x%x, sw2=0x%x\n", sw1, sw2);
#endif
		} else if (scr_pollfd.revents & POLLOUT) {
			printf("card out!\n");
		} else {
			printf("card invalid!\n");
		}
	}
}

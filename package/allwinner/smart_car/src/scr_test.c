#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include "sunxi-scr-user.h"

int main(int argc, char **argv)
{
	int i = 0;
	static int scr_fd;
	struct scr_card_para card_param;
	struct scr_atr scr_atr_des;
	uint8_t dataSend[128]={0x0};
	uint8_t rx_buf[128]={0};
	int rx_len=0;
	int det=0;

	scr_fd = open("/dev/smartcard", O_RDWR);

	if (scr_fd < 0){
		printf("smartcard: open fail!\n");
		return -1;
	}

	if(!ioctl(scr_fd, SCR_IOCGSTATUS, &det)){
		printf("SCR_IOCGSTATUS ok!\n");
		printf("Detect status = %d\n", det);
	} else {
		printf("SCR_IOCGSTATUS fail!\n");
		return -1;
	}

	if(ioctl(scr_fd, SCR_IOCRESET, NULL)){
		printf("SCR_IOCRESET fail!\n");
		return -1;
	} else
		printf("SCR_IOCRESET ok!\n");

	if(!ioctl(scr_fd, SCR_IOCGATR, &scr_atr_des)){
		printf("SCR_IOCGATR ok!\n");
		for(i = 0; i<scr_atr_des.atr_len; i++)
			printf("atr_data[%d]=0x%02x\n", i, scr_atr_des.atr_data[i]);
	} else {
		printf("SCR_IOCGATR fail!\n");
		return -1;
	}

	if(!ioctl(scr_fd, SCR_IOCGPARA, &card_param)){
		printf("SCR_IOCGPARA ok!\n");
		printf("card_param.d=%d,card_param.f=%d,card_param.freq=%d,card_param.recv_no_parity=%d\n\n",card_param.d, card_param.f, card_param.freq, card_param.recv_no_parity);
	} else {
		printf("SCR_IOCGPARA fail!\n");
		return -1;
	}

	card_param.recv_no_parity = 0;
	if(ioctl(scr_fd, SCR_IOCSPARA, &card_param)){
		printf("SCR_IOCSPARA fail!\n");
		return -1;
	} else
		printf("SCR_IOCSPARA ok!\n");


	if(!ioctl(scr_fd, SCR_IOCGPARA, &card_param)){
		printf("SCR_IOCGPARA ok!\n");
		printf("\ncard_param.d=%d,card_param.f=%d,card_param.freq=%d,card_param.recv_no_parity=%d\n\n",card_param.d, card_param.f, card_param.freq, card_param.recv_no_parity);
	} else {
		printf("SCR_IOCGPARA fail!\n");
		return -1;
	}

	dataSend[0]=0xe5;
	dataSend[1]=0x04;
	dataSend[2]=0x0;
	dataSend[3]=0x0;
	dataSend[4]=0x04;

	rx_len = write(scr_fd, &dataSend, 5);
	printf("write: %d\n", rx_len);

	rx_len = read(scr_fd, rx_buf, 64);
	printf("read: %d\n", rx_len);
	for(i = 0; i < rx_len; i++)
		printf("rx_buf[%d]=0x%02x\n", i, rx_buf[i]);

	close(scr_fd);
	printf("test pass\n");

	return 0;
}

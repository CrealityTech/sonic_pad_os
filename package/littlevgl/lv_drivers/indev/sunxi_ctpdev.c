/**
 * @file: sunxi_ctpdev.c
 * @autor: huangyixiu
 * @url: huangyixiu@allwinnertech.com
 */
/*********************
 *      INCLUDES
 *********************/
#include "sunxi_ctpdev.h"
#if USE_SUNXI_CTPDEV != 0

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

/*********************
 *      DEFINES
 *********************/
#define CTP_DEV_TEST	0		/* simulation */
#define CTP_DEV_SYNC	1		/* sync , else async*/

int ctpdev_fd;
int ctpdev_root_x;
int ctpdev_root_y;
int ctpdev_button;
pthread_t ctpdev_pthread_id;
pthread_mutex_t ctpdev_mutex;

void *ctpdev_thread(void * data)
{
#if CTP_DEV_TEST
    (void)data;

    while(1) {
		static int nCnt = 0;

		nCnt++;
		//printf("#####L:%d, F=%s, t=%d\n", __LINE__, __FUNCTION__, lv_tick_get());
		if(nCnt<10)
		{
			pthread_mutex_lock(&ctpdev_mutex);
			ctpdev_root_x = 700;
			ctpdev_root_y = 340;
			ctpdev_button = 0;
			pthread_mutex_unlock(&ctpdev_mutex);
		}
		else if (nCnt == 10)
		{
			//printf("L:%d, F=%s, t=%d\n", __LINE__, __FUNCTION__, lv_tick_get());
		}
		else
		{
			pthread_mutex_lock(&ctpdev_mutex);
			ctpdev_root_x -= 10;
			if(ctpdev_root_x <= 100)
			{
				//printf("L:%d, F=%s, t=%d\n", __LINE__, __FUNCTION__, lv_tick_get());
				nCnt = 0;
			}

			ctpdev_root_y = 340;
			ctpdev_button = 1;
			pthread_mutex_unlock(&ctpdev_mutex);
		}

		usleep(10000);   /*Sleep for 10 millisecond*/
    }

#elif CTP_DEV_SYNC
	(void)data;
	int ret;
	fd_set readfd;
	struct timeval timeout;
	struct input_event in;

	while(1)
	{
	      timeout.tv_sec=5;
	      timeout.tv_usec=0;

	      FD_ZERO(&readfd);
	      FD_SET(ctpdev_fd,&readfd);
	      ret=select(ctpdev_fd+1,&readfd,NULL,NULL,&timeout);
	      if (ret == -1)
	      {
	          printf("select error");
	      }
		  else if (ret == 0)
	      {
	          printf("time out\n");
	      }
	      else if (ret > 0)
	      {
	          if(FD_ISSET(ctpdev_fd,&readfd))
	          {
			  FD_CLR(ctpdev_fd, &readfd);
	              read(ctpdev_fd, &in, sizeof(in));
				  //printf("input info:(%d, %d, %d)\n", in.code, in.type, in.value);
				  if(in.type == EV_ABS)
				  {
						pthread_mutex_lock(&ctpdev_mutex);
						if(in.code == ABS_MT_POSITION_X)
							ctpdev_root_x = in.value;
			            else if(in.code == ABS_MT_POSITION_Y)
							ctpdev_root_y = in.value;
						else if(in.code == ABS_PRESSURE)
						{
							if(in.value == 0)
							{
								ctpdev_button = LV_INDEV_STATE_REL;
							}
							else
							{
								ctpdev_button = LV_INDEV_STATE_PR;
							}
						}
						pthread_mutex_unlock(&ctpdev_mutex);
			      }
		     }
	      }

		  //printf("piont:%d, %d, state=%d\n", ctpdev_root_x, ctpdev_root_y, ctpdev_button);
	}
#else
	(void)data;
    while(1)
	{
		struct input_event in;

		//printf("###L:%d, F=%s\n", __LINE__, __FUNCTION__);

	    while(read(ctpdev_fd, &in, sizeof(struct input_event)) > 0)
		{
			//printf("L:%d, F=%s, in:(%d, %d, %d)\n", __LINE__, __FUNCTION__, in.type, in.code, in.value);

			if(in.type==EV_SYN && in.code==SYN_REPORT && in.value==0)
			{
				break;
			}

			if(in.type == EV_ABS)
			{
				pthread_mutex_lock(&ctpdev_mutex);
				if(in.code == ABS_MT_POSITION_X)
					ctpdev_root_x = in.value;
	            else if(in.code == ABS_MT_POSITION_Y)
					ctpdev_root_y = in.value;
				else if(in.code == ABS_PRESSURE)
				{
					if(in.value == 0)
					{
						ctpdev_button = LV_INDEV_STATE_REL;
					}
					else
					{
						ctpdev_button = LV_INDEV_STATE_PR;
					}
				}
				pthread_mutex_unlock(&ctpdev_mutex);
	        }

	    }

		usleep(15000);

		printf("piont:%d, %d, state=%d\n", ctpdev_root_x, ctpdev_root_y, ctpdev_button);

    }
#endif
}

/**
 * Initialize the ctpdev interface
 */
void ctpdev_init(void)
{
	ctpdev_fd = -1;
    ctpdev_fd = open(CTPDEV_NAME,  O_RDONLY | O_NONBLOCK);
    if(ctpdev_fd == -1) {
        perror("unable open evdev interface:");
        return;
    }

    ctpdev_root_x = 0;
    ctpdev_root_y = 0;
    ctpdev_button = LV_INDEV_STATE_REL;
	pthread_mutex_init (&ctpdev_mutex, NULL);


	int ret = pthread_create(&ctpdev_pthread_id, NULL, ctpdev_thread, NULL);
	if (ret == -1)
	{
		printf("create thread fail\n");
		return ;
	}
	printf("ctpdev_pthread_id=%d\n", ctpdev_pthread_id);
}

/**
 * uninitialize the ctpdev interface
 */
void ctpdev_uninit(void)
{
	pthread_join(ctpdev_pthread_id, NULL);
	pthread_mutex_destroy(&ctpdev_mutex);

	ctpdev_root_x = 0;
    ctpdev_root_y = 0;
    ctpdev_button = LV_INDEV_STATE_REL;

	if (ctpdev_fd != -1)
	{
		close(ctpdev_fd);
	}
}

/**
 * Get the current position and state of the evdev
 * @param data store the evdev data here
 * @return false: because the points are not buffered, so no more data to be read
 */
bool ctpdev_read(lv_indev_drv_t * drv, lv_indev_data_t * data)
{
	//printf("#####L:%d, F=%s, t=%d\n", __LINE__, __FUNCTION__, lv_tick_get());

	pthread_mutex_lock(&ctpdev_mutex);
    data->point.x = ctpdev_root_x;
    data->point.y = ctpdev_root_y;
    data->state = ctpdev_button;
	pthread_mutex_unlock(&ctpdev_mutex);

	return false;
}

#endif

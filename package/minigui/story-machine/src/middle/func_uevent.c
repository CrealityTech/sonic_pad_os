#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include<semaphore.h>//for Semaphore
#include<sys/sem.h>
#include<sys/shm.h>//for shared memory
#include<sys/ipc.h>
#include<sys/types.h>
#include "wifi.h"
#include "player_int.h"
#include "func_uevent.h"
#include "listbox_view.h"

sem_t sem_id;

static func_callback_t	func_callback = NULL;

void *func_param;

void post_func_sem(void *param)
{
	func_param = param;
	sem_post(&sem_id);
}

static int *func_uevent_check(void *arg)
{
	int id = 0;

	sem_init(&sem_id, 0, 1);

	aw_wifi_open();

	while(1){
		sem_wait(&sem_id);
		if(func_callback){
			//printf("%s \n",__func__);
			func_callback(func_param);
		}
	}
	sem_destroy(&sem_id);
}


void func_uevent_init()
{
	pthread_t media_uevent;
	int result;

	result = pthread_create(&media_uevent,NULL,func_uevent_check,NULL);
	if(result != 0){
		printf("pthrad creat fail!\r\n");
		exit(EXIT_FAILURE);
	}
}

void func_register_callback(func_callback_t callback)
{
	func_callback = callback;
}

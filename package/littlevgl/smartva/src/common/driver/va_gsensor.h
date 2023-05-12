#ifndef __VA_GSENSOR_H__
#define __VA_GSENSOR_H__

#include <sys/types.h>

typedef void (*va_gsensor_callback_fun)(void);

typedef struct
{
	int bThreadRun;
	pthread_t threadID;
	va_gsensor_callback_fun gsensorFun;
	int gsensorHdle;
}va_GsensorDataType;

int va_gsensor_init(va_gsensor_callback_fun fun);
int va_gsensor_uninit(va_gsensor_callback_fun fun);
int va_gsensor_threshold_set(int threshold);
int va_gsensor_threshold_get(int *threshold);

#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include <sys/time.h>

uint64_t start_time_sec;
uint64_t start_time_usec;

void os_delay_us(uint32_t t)
{
	usleep(t);
}

void os_delay(uint32_t t)
{
	uint64_t ti;
	ti = (uint64_t) t *1000;

	usleep(ti);
}

uint32_t os_sys_time_get()
{
	uint64_t time_in_mill;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	if ((uint64_t)tv.tv_usec < start_time_usec) {
		tv.tv_usec += 1000000;
		tv.tv_sec--;
	}
	time_in_mill = (tv.tv_sec - start_time_sec) * 1000 +
	    (tv.tv_usec - start_time_usec) / 1000;
	return (uint32_t) time_in_mill;
}

uint32_t os_sys_time_get_us()
{
	uint64_t time_in_micro;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	if ((uint64_t)tv.tv_usec < start_time_usec) {
		tv.tv_usec += 1000000;
		tv.tv_sec--;
	}
	time_in_micro = (tv.tv_sec - start_time_sec) * 1000000 +
	    (tv.tv_usec - start_time_usec);
	return (uint32_t) time_in_micro;
}

void os_sys_time_init()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	start_time_sec = tv.tv_sec;
	start_time_usec = tv.tv_usec;
}

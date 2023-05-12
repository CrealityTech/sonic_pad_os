#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include <sys/time.h>

bool os_mutex_create(void **p_mutex)
{
	*p_mutex = NULL;
	return true;
}

#ifndef __DEBUG__H
#define __DEBUG__H

#include <stdio.h>

#define DEBUG_ERR
#define DEBUG_WARN
#define DEBUG_INFO

#ifdef DEBUG_ERR
#define debug_err(fmt,...) printf("err:<%s:%d>"fmt"\n",__func__, __LINE__, ##__VA_ARGS__)
#else
#define debug_err(fmt,...) 
#endif

#ifdef DEBUG_WARN
#define debug_warn(fmt,...) printf("warn:<%s:%d>"fmt"\n",__func__, __LINE__, ##__VA_ARGS__)
#else
#define debug_warn(fmt,...) 

#endif

#ifdef DEBUG_INFO
#define debug_info(fmt,...) printf("info:<%s:%d>"fmt"\n",__func__, __LINE__, ##__VA_ARGS__)
#else
#define debug_info(fmt,...) 
#endif

#endif

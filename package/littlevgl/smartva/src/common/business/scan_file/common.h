#ifndef __COMMON_H__
#define __COMMON_H__
typedef unsigned int        __hdle;
typedef unsigned long long  __u64;
typedef unsigned int		__u32;
typedef unsigned short		__u16;
typedef unsigned char		__u8;
typedef signed int			__s32;
typedef signed short		__s16;
typedef signed char 		__s8;
typedef signed char 		__bool;
#define EPDK_OK			(0)
#define EPDK_FAIL		(-1)
#define EPDK_TRUE       (1)
#define EPDK_FALSE      (0)
#ifndef NULL
#define NULL ((void *)0)
#endif
#define  RAT_ALIGN(val, align)		(((val) + ((align) - 1)) & ~((align) - 1))
#endif

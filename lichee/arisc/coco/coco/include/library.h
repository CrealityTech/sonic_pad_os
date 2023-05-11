/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                               Basic C Library
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : library.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-4-26
* Descript: the basic library fucntions.
* Update  : date                auther      ver     notes
*           2012-4-26 17:20:05  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __LIBRARY_H__
#define __LIBRARY_H__


#include "../library/cpus_math.h"

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

//basic string functions
extern size_t strlen(const char *s);
extern char *strcpy(char *dest, const char *src);
extern char *strncpy(char *dest, const char *src, size_t n);
extern char *strcat(char *dest, const char *src);
extern char *strncat(char *dest, const char *src, size_t n);
extern int   strcmp(const char *s1, const char *s2);
extern int   strncmp(const char *s1, const char *s2, size_t n);
extern char *strchr(const char *s, int c);
extern char *strrchr(const char *s, int c);
extern int sprintf(char * buf, char * fmt, ... );

//basic mem functions
extern void *memcpy(void *dest, const void *src, size_t n);
extern int   memcmp(const void *s1, const void *s2, size_t n);
extern void *memchr(const void *s, int c, size_t n);
extern void *memset(void *d, int c, size_t n);

//integer to string
extern char *itoa(int value, char *string, int radix);
extern char *utoa(unsigned int value, char *string, int radix);
void hexdump(const char* name, char* base, u32 len);

extern unsigned int hstr2int(const char *str, unsigned int len);
extern unsigned int dstr2int(const char *str, unsigned int len);
#endif  //__LIBRARY_H__

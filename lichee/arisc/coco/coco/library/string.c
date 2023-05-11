/*
	string.h -- String manipulation
	Implements (some) of the standard string routines
	Copyright (C) 2002 Richard Herveille, rherveille@opencores.org

	This file is part of OpenRISC 1000 Reference Platform Monitor (ORPmon)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <stddef.h>
//#include "string.h"
#include <stdarg.h>
#include "ibase.h"

/* Basic string functions */

/*
  s t r l e n

  returns number of characters in s (not including terminating null character)
*/
size_t strlen(const char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		;/* nothing */

	return sc - s;
}

/*
  s t r c p y

  Copy 'src' to 'dest'. Strings may not overlap.
*/
char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		;/* nothing */

	return tmp;
}

#ifdef BOOT_USED
char *strncpy(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	while (count) {
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
		count--;
	}
	return dest;
}
#endif
//char *strcat(char *dest, const char *src)
//{
//  char *tmp = dest;
//
//  while (*dest)
//      dest++;
//  while ((*dest++ = *src++) != '\0')
//      ;
//
//  return tmp;
//}

char *strncat(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	if (count) {
		while (*dest)
			dest++;
		while ((*dest++ = *src++) != 0) {
			if (--count == 0) {
				*dest = '\0';
				break;
			}
		}
	}
	return tmp;
}

int strcmp(const char *cs, const char *ct)
{
	unsigned char *c1 = (unsigned char *)cs;
	unsigned char *c2 = (unsigned char *)ct;

	while (1) {
		if ((*c1++) != (*c2++))
			return ((*c1) < (*c2)) ? -1 : 1;
		if (!(*c1))
			break;
	}
	return 0;
}

//int strncmp(const char *cs, const char *ct, size_t count)
//{
//  unsigned char c1, c2;
//
//  while (count) {
//      c1 = *cs++;
//      c2 = *ct++;
//      if (c1 != c2)
//          return c1 < c2 ? -1 : 1;
//      if (!c1)
//          break;
//      count--;
//  }
//  return 0;
//}

/* Basic mem functions */
void *memcpy(void *dest, const void *src, size_t n)
{
	char *cs;
	char *cd;

	/* check if 'src' and 'dest' are on LONG boundaries */
	if (0x03 & ((unsigned long)dest | (unsigned long)src)) { /* only for 32 archtecture plarform */
	//if (1) {
		/* no, do a byte-wide copy */
		cs = (char *)src;
		cd = (char *)dest;

		while (n--)
			*cd++ = *cs++;
	} else {
		/* yes, speed up copy process */
		/* copy as many LONGs as possible */
		long *ls = (long *)src;
		long *ld = (long *)dest;

		size_t cnt = n >> 2;
		while (cnt--)
			*ld++ = *ls++;

		/* finally copy the remaining bytes */
		cs = (char *)(src + (n & ~0x03));
		cd = (char *)(dest + (n & ~0x03));

		cnt = n & 0x03;
		while (cnt--)
			*cd++ = *cs++;
	}

	return dest;
}

//int memcmp(const void *s1, const void *s2, size_t n)
//{
//      char *p1 = (void *)s1;
//      char *p2 = (void *)s2;
//
//      while ((*p1 == *p2) && n--) {
//          p1++;
//          p2++;
//      }
//
//      return *p1 - *p2;
//}

void *memset(void *s, int c, size_t n)
{
	char *p = s;

	while (n--)
		*p++ = c;

	return s;
}

#define STRVAL_SIZE     32

int sprintf(char * buf, char * fmt, ... )
{
	va_list ap;
	char strval[STRVAL_SIZE];
	char *p;
	int nval;
	//char *pval;
	unsigned int i, index=0;

	va_start(ap, fmt);

	for (p=fmt; *p; p++) {
		for (i=0; i<STRVAL_SIZE; i++)  /* clear strval[] */
			strval[i] = 0;

		if (*p!='%') {
			buf[index++] = *p;
			continue;
		}

		p++;

		switch (*p) {
			case 'd':
				nval = va_arg(ap, int);
				itoa(nval, strval, 10);
				//LOG("str:%s\n", strval);
			break;
			default:
			break;
/*
			case 'u':
				nval = va_arg(ap, unsigned int);
			break;

			case 'x':
				nval = va_arg(ap, int);
				itoa(nval, strval, 16);
			break;

			case 'c':
				nval = va_arg(ap, int);
				strval[0] = nval;
				strval[1] = 0;
			break;

			case 's':
				pval = va_arg(ap, char *);
				for (i=0; (pval[i]!='\0') && (i<STRVAL_SIZE); i++)
					strval[i] = pval[i];
			break;

			case 'S':
				pval = va_arg(ap, char *);
				for (i=0; pgm8(pval[i])!='\0' && i<STRVAL_SIZE; i++)
					strval[i] = pgm8(pval[i]);
			break;
*/
			}

			for (i=0; strval[i]!='\0' && i<STRVAL_SIZE; i++)
				buf[index++] = strval[i];
        }

        buf[index]='\0';

        va_end(ap);

        return (int)index;
}

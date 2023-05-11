/*
*********************************************************************************************************
*                                                AR100 System
*                                     AR100 Software System Develop Kits
*                                               Debugger Module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : debugger.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-4-25
* Descript: debugger public header.
* Update  : date                auther      ver     notes
*           2012-4-25 16:19:40  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

/* extern u32 debug_level; */

#ifdef CFG_DEBUGGER_PRINTF
/*
*********************************************************************************************************
*                                           INITIALIZE DEBUGGER
*
* Description:  initialize debugger.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize debugger succeeded, others if failed.
*********************************************************************************************************
*/
s32 debugger_init(void);

/*
*********************************************************************************************************
*                                           EXIT DEBUGGER
*
* Description:  exit debugger.
*
* Arguments  :  none.
*
* Returns    :  OK if exit debugger succeeded, others if failed.
*********************************************************************************************************
*/
s32 debugger_exit(void);

/*
*********************************************************************************************************
*                                           PUT A CHARSET
*
* Description:  put out a charset.
*
* Arguments  :  ch  : the charset which we want to put out.
*
* Returns    :  OK if put out charset succeeded, others if failed.
*********************************************************************************************************
*/
s32 debugger_putc(char ch);

/*
*********************************************************************************************************
*                                           GET A CHARSET
*
* Description:  get a charset.
*
* Arguments  :  none.
*
* Returns    :  the charset we read from uart.
*********************************************************************************************************
*/
u32 debugger_get(char *buf);
extern s32 debugger_puts(char *string);

/*
*********************************************************************************************************
*                                           FORMATTED PRINTF
*
* Description:  print out a formatted string, similar to ANSI-C function printf().
*               This function can support and only support the following conversion specifiers:
*               %d  signed decimal integer.
*               %u  unsigned decimal integer.
*               %x  unsigned hexadecimal integer, using hex digits 0x.
*               %c  single character.
*               %s  character string.
*
* Arguments  :  format  : format control.
*               ...     : arguments.
*
* Returns    :  the number of characters printed out.
*
* Note       :  the usage refer to ANSI-C function printf().
*********************************************************************************************************
*/
s32 debugger_printf(u32 level, const char *format, ...);

/*
*********************************************************************************************************
*                                           SET DEBUG LEVEL
*
* Description:  set debug level.
*
* Arguments  :  para level.
*
* Returns    :  OK if set the debug level succeeded.
*********************************************************************************************************
*/
s32 set_debug_level(u32 level);

#else
static inline s32 debugger_init(void)
{
	return -ENOSYS;
}

static inline s32 debugger_exit(void)
{
	return -ENOSYS;
}

static inline s32 debugger_putc(char ch)
{
	return -ENOSYS;
}

static inline u32 debugger_get(char *buf)
{
	return -ENOSYS;
}

static inline s32 debugger_puts(char *string)
{
	return -ENOSYS;
}

static inline s32 debugger_printf(u32 level, const char *format, ...)
{
	return -ENOSYS;
}

static inline s32 vprintk(u32 level, const char *format, va_list args)
{
	return -ENOSYS;
}

static inline s32 set_debug_level(u32 level)
{
	return -ENOSYS;
}

#endif

#endif /* __DEBUGGER_H__ */

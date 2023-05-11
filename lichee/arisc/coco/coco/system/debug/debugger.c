/*
*********************************************************************************************************
*                                                AR100 System
*                                     AR100 Software System Develop Kits
*                                               Debugger Module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : debugger.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-3
* Descript: debugger module.
* Update  : date                auther      ver     notes
*           2012-5-3 17:16:47   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "debugger_i.h"

uart_device *debug_uart;

u32 debug_level = 1;

volatile unsigned int print_timeflg = 1;
#define DEBUG_BUFFER_SIZE (128) /* debug buffer size */
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

static s32 print_current_time(void);
static s32 print_align(char *string, s32 len, s32 align);

s32 debugger_init(void)
{
	debug_uart = query_uart();

	debug_uart->dev.handler = getcmd;
	install_isr(debug_uart->dev.irq_no, debug_uart->dev.handler, (void *)(debug_uart));

#ifdef CFG_SHELL_USED
	interrupt_enable(debug_uart->dev.irq_no);
#endif
//	UART_IRQ_Request(debug_uart.UARTx,getcmd,NULL);
//	install_isr(INTC_R_UART_IRQ, getcmd, NULL);
//	interrupt_enable(debug_uart->dev.irq_no);

	return OK;
}

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
s32 debugger_exit(void)
{
	//...

	return OK;
}

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
s32 debugger_putc(char ch)
{
	uart_putc(debug_uart,ch);

	return OK;
}

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
u32 debugger_get(char *buf)
{
	return uart_get(debug_uart,buf);
}

/*
*********************************************************************************************************
*                                           PUT A STRING
*
* Description:  put out a string.
*
* Arguments  :  string  : the string which we want to put out.
*
* Returns    :  OK if put out string succeeded, others if failed.
*********************************************************************************************************
*/
s32 debugger_puts(char *string)
{
	uart_puts(debug_uart,string);

	return OK;
}

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
char debugger_buffer[DEBUG_BUFFER_SIZE];
s32 debugger_printf(u32 level, const char *format, ...)
{
	va_list  args;
	s32 ret;

	va_start(args, format);

	ret = vprintk(level,format,args);

	va_end(args);

	return ret;
	/*
	va_list  args;
	char     string[16];    //align by cpu word
	char    *pdest;
	char    *psrc;
	s32      align;
	s32      len = 0;
	s32      cpsr;

	if(level & (0xf0 >> (debug_level +1)))
	{
		cpsr = cpu_disable_int();
		//dump current timestemp
		if (print_timeflg)
		print_current_time();
		pdest = debugger_buffer;
		va_start(args, format);
		while(*format)
		{
			if(*format == '%')
			{
				++format;
				if (('0' < (*format)) && ((*format) <= '9'))
				{
					//we just suport wide from 1 to 9.
					align = *format - '0';
					++format;
				}
				else
				{
					align = 0;
				}
				switch(*format)
				{
					case 'd':
					{
						//int
						itoa(va_arg(args, int), string, 10);
						len = strlen(string);
						len += print_align(string, len, align);
						strcpy(pdest, string);
						pdest += len;
						break;
					}
					case 'x':
					case 'p':
					{
						//hex
						utoa(va_arg(args, unsigned long), string, 16);
						len = strlen(string);
						len += print_align(string, len, align);
						strcpy(pdest, string);
						pdest += len;
						break;
					}
					case 'u':
					{
						//unsigned int
						utoa(va_arg(args, unsigned long), string, 10);
						len = strlen(string);
						len += print_align(string, len, align);
						strcpy(pdest, string);
						pdest += len;
						break;
					}
					case 'c':
					{
						//charset, aligned by cpu word
						*pdest = (char)va_arg(args, int);
						break;
					}
					case 's':
					{
						//string
						psrc = va_arg(args, char *);
						strcpy(pdest, psrc);
						pdest += strlen(psrc);
						break;
					}
					default :
					{
						//no-conversion
						*pdest++ = '%';
						*pdest++ = *format;
					}
				}
			}
			else
			{
				*pdest++ = *format;
			}
			//parse next token
			++format;
		}
		va_end(args);

		if (*(pdest - 1) == '\n')
			print_timeflg = 1;
		else
			print_timeflg = 0;
		//must end with '\0'
		*pdest = '\0';
		pdest++;
		debugger_puts(debugger_buffer);

		cpu_enable_int(cpsr);

		return (pdest - debugger_buffer);
	}
	return OK;
	*/
}


s32 vprintk(u32 level, const char *format,va_list args)
{
	char     string[16];    //align by cpu word
	char    *pdest;
	char    *psrc;
	s32      align;
	s32      len = 0;
	s32      cpsr;

	if(level & (0xf0 >> (debug_level +1)))
	{
		cpsr = cpu_disable_int();
		//dump current timestemp
		if (print_timeflg)
		print_current_time();
		pdest = debugger_buffer;
		while(*format)
		{
			if(*format == '%')
			{
				++format;
				if (('0' < (*format)) && ((*format) <= '9'))
				{
					//we just suport wide from 1 to 9.
					align = *format - '0';
					++format;
				}
				else
				{
					align = 0;
				}
				switch(*format)
				{
					case 'd':
					{
						//int
						itoa(va_arg(args, int), string, 10);
						len = strlen(string);
						len += print_align(string, len, align);
						strcpy(pdest, string);
						pdest += len;
						break;
					}
					case 'x':
					case 'p':
					{
						//hex
						utoa(va_arg(args, unsigned long), string, 16);
						len = strlen(string);
						len += print_align(string, len, align);
						strcpy(pdest, string);
						pdest += len;
						break;
					}
					case 'u':
					{
						//unsigned int
						utoa(va_arg(args, unsigned long), string, 10);
						len = strlen(string);
						len += print_align(string, len, align);
						strcpy(pdest, string);
						pdest += len;
						break;
					}
					case 'c':
					{
						//charset, aligned by cpu word
						*pdest = (char)va_arg(args, int);
						break;
					}
					case 's':
					{
						//string
						psrc = va_arg(args, char *);
						strcpy(pdest, psrc);
						pdest += strlen(psrc);
						break;
					}
					default :
					{
						//no-conversion
						*pdest++ = '%';
						*pdest++ = *format;
					}
				}
			}
			else
			{
				*pdest++ = *format;
			}
			//parse next token
			++format;
		}
		if (*(pdest - 1) == '\n')
			print_timeflg = 1;
		else
			print_timeflg = 0;
		//must end with '\0'
		*pdest = '\0';
		pdest++;
		debugger_puts(debugger_buffer);

		cpu_enable_int(cpsr);

		return (pdest - debugger_buffer);
	}
	return OK;
}

static s32 print_align(char *string, s32 len, s32 align)
{
	//fill with space ' ' when align request,
	//the max align length is 16 byte.
	char fill_ch[] = "                ";
	if (len < align)
	{
		//fill at right
		strncat(string, fill_ch, align - len);
		return align - len;
	}
	//not fill anything
	return 0;
}

/*
*********************************************************************************************************
*                                           print current time
*
* Description:  print current time.
*
* Arguments  :  none.
*
* Returns    :  OK if print current time succeeded, others if failed.
*********************************************************************************************************
*/
static s32 print_current_time(void)
{
	char time[12];
	u32  millisec = 0;
	u32  sec;
	u32  div;
	u32  i;

	//convert current ticks to millisecond.
	millisec = current_time_tick() * (1000 / TICK_PER_SEC);

	//time print format : [secs.ms] s,
	//example           : [0001.00] s.
	time[0]  = '[';
	time[5]  = '.';
	time[8]  = ']';
	time[9]  = ' ';
	time[10] = '\0';

	//second
	sec = millisec / 1000;
	sec = sec % 10000;
	div = 1000;
	for (i = 1; i <= 4; i++)
	{
		time[i] = ((u8)(sec / div)) + '0';
		sec = sec % div;
		div = div / 10;
	}
	//millisecond
	millisec = millisec % 1000;
	div = 100;
	for (i = 6; i <= 7; i++)
	{
		time[i] = ((u8)(millisec / div)) + '0';
		millisec = millisec % div;
		div = div / 10;
	}
	//dump time buffer
	debugger_puts(time);

	return OK;
}

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
s32 set_debug_level(u32 level)
{
	LOG("debug_mask from %d to %d\n", debug_level, level);
	debug_level  = level;

	return OK;
}

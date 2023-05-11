#ifndef __UART_I_H__
#define __UART_I_H__

#include "ibase.h"
#include "driver.h"



struct uart_ops {
	s32 (*putc)(uart_device *uart, char ch);
	s32 (*get)(uart_device *uart, char *buf);
	s32 (*puts)(uart_device *uart, const char *string);
	s32 (*set_baudrate)(uart_device *uart, u32 baud_rate);
	s32 (*get_baudrate)(uart_device *uart);
	s32 (*set_lock)(uart_device *uart,u32 lock);
	s32 (*suspend_early)(uart_device *uart);
	s32 (*resume_early)(uart_device *uart);
};



#endif



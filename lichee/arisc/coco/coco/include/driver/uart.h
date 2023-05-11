#ifndef __UART_H__
#define __UART_H__

#include "ibase.h"
#include "driver.h"

typedef struct {
	arisc_device dev;
	arisc_driver *dri;
	const struct uart_ops *ops;
	u32 baud_rate;
	u32 flow_ctrl;
	u32 flow_cts_count;
	UART_TypedDef UARTx;
	pin_typedef *uart_tx;
	pin_typedef *uart_rx;
	s32 (*clkchangecb)(u32 command, void *parg);
}uart_device;

uart_device *query_uart(void);
s32 uart_exit(uart_device *dev);
s32 uart_putc(uart_device *dev, char ch);
s32 uart_get(uart_device *dev, char *buf);
s32 uart_puts(uart_device *dev, const char *string);
s32 uart_set_baudrate(uart_device *dev, u32 baud_rate);
s32 uart_get_baudrate(uart_device *dev);
s32 uart_set_lock(uart_device *uart,u32 lock);
s32 uart_init(void);


#endif


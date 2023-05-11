/*
 * include/drivers/uart.h
 *
 * Copyright (C) 2012-2016 AllWinnertech Ltd.
 * Author: Sunny <Sunny@allwinnertech.com>
 *
 */
#ifndef __UART_H__
#define __UART_H__

extern s32 uart_init(void);
extern s32 uart_exit(void);
extern s32 uart_putc(char ch);
extern u32 uart_get(char *buf);
extern s32 uart_puts(const char *string);
extern s32 uart_set_baudrate(u32 rate);
extern u32 uart_get_baudrate(void);
extern s32 uart_clkchangecb(u32 command, u32 freq);

#endif  /* __UART_H__ */

#ifndef _DATA_UART_H_
#define _DATA_UART_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
/****************************************************************************************************************
* exported functions other .c files may use all defined here.
****************************************************************************************************************/
/**
 * @defgroup Data_Uart_Exported_Functions Data Uart Exported Functions
 * @brief
 * @{
 */

void data_uart_print_enable(bool enable);
extern void data_uart_debug(char *fmt, ...);
extern void data_uart_dump(uint8_t *pbuffer, uint32_t len);
extern uint32_t data_uart_send_string(const uint8_t *data, uint32_t len);
/** @} */
/** @} */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif

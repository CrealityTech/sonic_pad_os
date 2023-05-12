#include "data_uart.h"

#define SPRINTF_BUF_LEN     1024

static bool data_uart_flag = 1;
static char data_buf[SPRINTF_BUF_LEN];

void data_uart_print_enable(bool enable)
{
	data_uart_flag = enable;
	return;
}

void data_uart_debug(char *fmt, ...)
{
	if(data_uart_flag)
	{
		va_list args;
		int n;
		va_start(args, fmt);

		n = vsprintf(data_buf, fmt, args);

		va_end(args);

		if(n>=SPRINTF_BUF_LEN)
		{
			data_buf[SPRINTF_BUF_LEN-1] = '\0';
		}
		else
		{
			data_buf[n] = '\0';
		}
		printf("%s",data_buf);
	}
}

void data_uart_dump(uint8_t *pbuffer, uint32_t len)
{
	char data[2];
	data[1]= '\0';
	if (data_uart_flag)
    {
        printf("0");
        printf("x");
		uint32_t loop = 0;
        for (loop = 0; loop < len; loop++)
        {
            data[0] = "0123456789ABCDEF"[pbuffer[loop] >> 4];
            printf("%s",data);
            data[0] = "0123456789ABCDEF"[pbuffer[loop] & 0x0f];
            printf("%s", data);
        }
        printf("\r");
        printf("\n");
    }
}

uint32_t data_uart_send_string(const uint8_t *data, uint32_t len)
{
	uint32_t i = 0;
	for (i = 0; i < len; ++i)
    {
        printf("%c ", (char)data[i]);
    }
    printf("\r");
    printf("\n");
    return len;
}
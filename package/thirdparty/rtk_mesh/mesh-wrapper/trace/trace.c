/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <trace.h>
#include <os_sched.h>
//#include <os_sync.h>
//#include <otp.h>

//#include "patch_platform.h"
//#include "pingpong_buffer.h"
//#include "log_uart_dma.h"
//#include "rtl876x_uart.h"
//#include "rtl876x_tim.h"

//#include <platform_os.h>

#define SPRINTF_BUFFER_LEN     1024

#define LOG_MESSAGE_SYNC_CODE         0x7E
#define MAX_LOG_MESSAGE_LEN           128
#define MAX_LOG_MESSAGE_LEN_FOR_SNOOP 252
#define MAX_ARGUMENT_SIZE             20

#define GET_TYPE(info)              (uint8_t)((info) >> 24)
#define GET_SUBTYPE(info)           (uint8_t)((info) >> 16)
#define GET_MODULE(info)            (uint8_t)((info) >> 8)
#define GET_LEVEL(info)             (uint8_t)((info) >> 0)

#ifdef FOR_SIMULATION_WITH_FW_LOG
#undef FOR_SIMULATION
#endif

static char data_buffer[SPRINTF_BUFFER_LEN];


uint8_t log_seq_num = 0;
uint8_t snoop_seq_num = 0;
bool is_log_init = false;
uint64_t trace_mask_init[LEVEL_NUM]= {0xFFFFFFFFFFFFFFFF,
						      0x0000000000000000,
							  0x0000000000000000,
							  0x0000000000000000};
static uint64_t *trace_mask = trace_mask_init;

static FILE* mesh_log_fp;

static char *level_prompt[4]={"!!! ","*!! ","**! ","*** "};
static char *module_prompt[64]={
	"[PATCH]",      //0
	"[OS]",         //1
	"[OSIF]",       //2
	"[BOOT]",       //3
	"[DLPS]",       //4
	"[AES]",        //5
	"[FS]",	        //6
	"[7]",	        //7
	"[8]",	        //8
	"[9]",	        //9
	"[10]",	        //10
	"[11]",	        //11

	"[KEYSCAN]",    //12
	"[QDECODE]",
	"[IR]",
	"[3DG]",
	"[ADC]",
	"[GDMA]",
	"[I2C]",
	"[RTC]",
	"[SPI]",
	"[TIMER ]",
	"[UART]",
	"[FLASH]",
	"[GPIO]",
	"[PINMUX]",
	"[PWM]",
	"[USB]",
	"[SDIO]",
	"[CHARGER]",
	"[DSP]",
	"[EFUSE]",    //31

	"[32]",       //32
	"[33]",       //33
	"[34]",       //34
	"[35]",       //35
	"[36]",       //36
	"[37]",       //37
	"[38]",       //38
	"[39]",       //39
	"[40]",       //40
	"[41]",       //41
	"[42]",       //42
	"[43]",       //43
	"[44]",       //44
	"[45]",       //45
	"[46]",       //46
	"[47]",       //47

	"[APP]",       //48
	"[DFU]",       //49
	"[RFCOMM]",       //50
	"[PROFILE]",       //51
	"[PROTOCOL]",       //52
	"[GAP]",       //53
	"[BTE]",       //54
	"[BTIF]",       //55
	"[GATT]",       //56
	"[SMP]",       //57
	"[SDP]",       //58
	"[L2CAP]",       //59
	"[HCI]",       //60
	"[SNOOP]",       //61
	"[UPPERSTACK]",       //62
	"[LOWERSTACK]",       //63
};

static void time_stamp(void)
{
	uint32_t time = os_sys_time_get();
	if(mesh_log_fp != NULL)
	{
		fprintf(mesh_log_fp,"[-0x%10dms-] ",time);
	}
}

void log_module_trace_init(uint64_t mask[LEVEL_NUM])
{
	memset(trace_mask_init,0,LEVEL_NUM*sizeof(uint64_t));
	if (mask != NULL)
    {
        uint8_t i;
        for (i = 0; i < LEVEL_NUM; i++)
        {
            trace_mask_init[i] = mask[i];
        }
    }

    trace_mask = &trace_mask_init[0];

    mesh_log_fp = fopen("mesh_log.txt","a");
}

bool log_module_trace_set(T_MODULE_ID module_id, uint8_t trace_level, bool set)
{
    if (module_id >= MODULE_NUM || trace_level >= LEVEL_NUM)
    {
        return false;
    }

    if (set == true)
    {
        trace_mask[trace_level] |= (uint64_t)1 << module_id;
    }
    else
    {
        trace_mask[trace_level] &= ~((uint64_t)1 << module_id);
    }

    return true;
}

uint64_t *log_module_trace_get(void )
{
	return trace_mask;
}

bool log_module_bitmap_trace_set(uint64_t module_bitmap, uint8_t trace_level, bool set)
{
    if (set == true)
    {
        trace_mask[trace_level] |= module_bitmap;
    }
    else
    {
        trace_mask[trace_level] &= ~module_bitmap;
    }

    return true;
}

void log_disable(void)
{
	memset(trace_mask,0,LEVEL_NUM*sizeof(uint64_t));
	if(mesh_log_fp != NULL)
	{
		fclose(mesh_log_fp);
	}
}
void log_print(const char* pFuncName, T_LOG_TYPE type, T_LOG_SUBTYPE subtype, T_MODULE_ID module, uint8_t level, char *fmt, ...)
{
#ifdef MESH_ANDROID
	if(trace_mask[level]&((uint64_t)1<<module))
	{
		va_list args;
		int n = 0;
		int count = 0;
		char *pbuf = data_buffer;
		if(type == TYPE_MESH )
		{
			n = sprintf(pbuf,"%s ","[MESH]2.00*");
			pbuf += n;
			count += n;

			n = sprintf(pbuf,"%s ",level_prompt[level] );
			pbuf += n;
			count += n;
		}
		else
		{
			n = sprintf(pbuf,"%s ","[MESH]2.00");
			pbuf += n;
			count += n;

			n = sprintf(pbuf,"%s ",module_prompt[module] );
			pbuf += n;
			count += n;

			n = sprintf(pbuf,"%s ",level_prompt[level] );
			pbuf += n;
			count += n;
		}


		n = sprintf(pbuf,"%s ",pFuncName);
		count += n;

		va_start(args, fmt);
		n = vsprintf(pbuf, fmt, args);
		count += n;
		va_end(args);

		if(count>=SPRINTF_BUFFER_LEN )
		{
			data_buffer[SPRINTF_BUFFER_LEN-1] = '\0';
		}
		else
		{
			data_buffer[count] = '\0';
		}
		ALOGD("%s", data_buffer);
	}

  return;
#endif

	if(((mesh_log_fp != NULL)&&trace_mask[level]&((uint64_t)1<<module))
		||((mesh_log_fp != NULL)&&(type == TYPE_MESH)))
	{
		time_stamp();

		va_list args;
		int n = 0;
		int count = 0;
		char *pbuf = data_buffer;
		if(type == TYPE_MESH )
		{
			n = sprintf(pbuf,"%s ","[MESH]2.00*");
			pbuf += n;
			count += n;

			n = sprintf(pbuf,"%s ",level_prompt[level] );
			pbuf += n;
			count += n;
		}
		else
		{
			n = sprintf(pbuf,"%s ","[MESH]2.00");
			pbuf += n;
			count += n;

			n = sprintf(pbuf,"%s ",module_prompt[module] );
			pbuf += n;
			count += n;

			n = sprintf(pbuf,"%s ",level_prompt[level] );
			pbuf += n;
			count += n;
		}


		n = sprintf(pbuf,"%s ",pFuncName);
		count += n;

		va_start(args, fmt);
		n = vsprintf(pbuf, fmt, args);
		count += n;
		va_end(args);

		if(count>=SPRINTF_BUFFER_LEN )
		{
			data_buffer[SPRINTF_BUFFER_LEN-1] = '\0';
		}
		else
		{
			data_buffer[count] = '\0';
		}
		fprintf(mesh_log_fp,"%s\n", data_buffer);
		if(ftell(mesh_log_fp) >= (4*1024*1024) )
		{
			fclose(mesh_log_fp);
			rename("mesh_log.txt","mesh_log0.txt");
			mesh_log_fp = fopen("mesh_log.txt","a");
			fseek(mesh_log_fp,0,SEEK_SET);
		}
	}
}

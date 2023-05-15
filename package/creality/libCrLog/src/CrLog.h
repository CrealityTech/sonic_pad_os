#ifndef _CRLOG_H
#define _CRLOG_H

#include <string.h>
#include <stdio.h>

#define RET_LOG_OK      0
#define RET_LOG_ERR     (-1)

enum LOG_LEVEL
{
    LOG_DEFAULT = 0,
    LOG_DEBUG = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3 
};

#ifdef __cplusplus
extern "C" {
#endif

/*
void SetLogLevel(int level);
int GetLogLevel(void);
用于设置、获取当前的log打印级别
*/
void SetLogLevel(int level);
int GetLogLevel(void);

/*
函数说明：
    初始化log的存储路径、名称及log级别
参数:
    LogPath:指定log存放路径
    FileName:指定log存放名称，最终名称为FileName+系统时间
    LogLevel:指定log打印级别
返回值：
    0:为创建成功
    -1:创建失败
*/
unsigned int CrLogOpen(char *LogPath, char *FileName , int LogLevel); 
/*
函数说明：
    按照指定的log级别来存储buffer
参数:
    loglevel:按照什么log级别来存储
    Buff:将要写入的buffer
    size:写入的buffer长度
返回值：
    写入的个数 
*/
unsigned int CrLogWrite(int loglevel , char *Buff, unsigned int Size);
/*
函数说明：
    关闭log日志句柄
*/
unsigned int CrLogClose();

/*
多线程情况下的锁
*/
unsigned int CrLogLock();
unsigned int CrLogUnLock();

#define CRLOG_BUFF_SIZE 512
static char g_CrLogBuff[CRLOG_BUFF_SIZE];

#define CrLog(loglevel , xx, arg...)   do                                                                         \
        {                                                                                                       \
            if(loglevel >= GetLogLevel())                                                                          \
            {                                                                                                   \
                CrLogLock();                                                                                    \
                snprintf(g_CrLogBuff, CRLOG_BUFF_SIZE,"<%s>(%s):%d-" xx, __FILE__ , __func__ , __LINE__ , ##arg);     \
                CrLogWrite(loglevel , g_CrLogBuff,  strlen(g_CrLogBuff));                                                      \
                CrLogUnLock();                                                                                  \
            }                                                                                                   \
        } while(0)

#define LOG_PRINTF(loglevel, format, arguments...)     do{     \
    if(loglevel >= GetLogLevel())           \
    {                                   \
        switch(loglevel)               \
        {                               \
            case LOG_DEBUG:             \
                printf("[DEBUG] ");     \
                break;                  \
            case LOG_WARNING:           \
                printf("[WARNING] ");   \
                break;                  \
            case LOG_ERROR:             \
                printf("[ERROR] ");     \
                break;                  \
            default:                    \
                printf("[WARNING] ");   \
                break;                  \
        }                               \
        printf("%s %d : " , __FUNCTION__,__LINE__); \
        printf(format, ##arguments);    \
        printf("\n");                   \
    }                                   \
}while(0)

#ifdef __cplusplus
}
#endif

#endif
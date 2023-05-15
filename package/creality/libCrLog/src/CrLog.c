#include "CrLog.h"
#include "CrMutex.h"

#ifdef USING_OS_LINUX

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

extern int errno;

typedef struct stat Stat_t;

#endif

#define MAX_SIZE 200
#define PERMISSION (S_IRWXG | S_IRWXO | S_IRWXU)

static int g_FileFd = -1;

static CrPthreadMutex_t MutexLog;
static unsigned int g_BoolUsingMutexFlag = 0; //是否使用锁的标志

#define MAXFILECOUNT 4  //一个类型文件的最大个数
#define MAXFILELENGTH (10*1024*1024)
#define CRLOG_PATH_LENGTH   80

//LogName(日志文件的名称)
static unsigned char g_FileName[20] = "FileLog";

struct CrLogAttr_t
{
    unsigned int UsingMutex;             //定义是否上锁
    unsigned int OutPutPathFlag;           //定义输出的路径，如果为0，则表示当前路径
    unsigned int mLogLevel;              //定义输出的log级别
    unsigned char  FilePath[CRLOG_PATH_LENGTH];  //文件路径(最后一个是字符'/')
};

static struct CrLogAttr_t   g_LogAttr;   //保存属性的接口

/*
对log文件进行判断大小、增删文件等操作函数
*/
static short IsChangedFile(unsigned int Fd, int FileMaxSize);
static short CatFileName(unsigned char* FileName, const unsigned char* Path, const unsigned char* LogName); 
static short CountLogFile(const char* Path, const char* Filter);
static short GetMinModificationTimeFileName(char* Name, const char* Path, const char* Filter);
static void DeleteFileOverTheLimit(const char* Path, const char* Filter);

/*
对log文件进行读写操作函数
*/
unsigned int CrLogIoctl();
static unsigned int CrFileWrite(unsigned int Fd, const char *Buff, const unsigned int Size);
static unsigned int CrFileOpen(const char* Name, unsigned int  access, unsigned int mode);
static unsigned int CrFileClose(unsigned int Fd);

/*
对loglevel进行配置的操作函数
*/
#define LOG_LEVEL_SAVE_FILE     "./log_level(0-def;1-dbg;2-warn;3-err).txt"
static int g_LogLevel = 2;  //默认告警级别是warning
static unsigned char *g_LogLevelString[4] = {"----" , "DEBUG" , "WARNING" , "ERROR"};
static unsigned int SaveLogLevelToFile(int LogLevel);
static unsigned int GetLogLevelFromFile(int *LogLevel);



void SetLogLevel(int level)
{
    g_LogLevel = level;
}

int GetLogLevel(void)
{
    return g_LogLevel;
}

//用于记录当前运行程序的log级别到指定文件中
unsigned int SaveLogLevelToFile(int LogLevel)
{
    int Fd = 0;
    if(access(LOG_LEVEL_SAVE_FILE , F_OK) != 0)
        Fd = open(LOG_LEVEL_SAVE_FILE , O_RDWR | O_CREAT | O_TRUNC , PERMISSION);
    else
        Fd = open(LOG_LEVEL_SAVE_FILE , O_RDWR , PERMISSION);

    if(Fd < 0)
    {
        LOG_PRINTF(LOG_ERROR , "open %s failed." , LOG_LEVEL_SAVE_FILE);
        return RET_LOG_ERR;
    }

    char buf[10] = {0};
    
    switch (LogLevel)
    {
        case LOG_DEBUG:
        case LOG_WARNING:
        case LOG_ERROR:
            snprintf(buf , sizeof(buf) , "%d" , LogLevel);
            break;
        default:
            //默认使用LOG_WARNING等级
            snprintf(buf , sizeof(buf) , "%d" , LOG_WARNING);
            break;
    }

    //此处不需要判断返回值
    write(Fd , buf , strlen(buf));

    close(Fd);

    return RET_LOG_OK;
}

//用于从指定的文件中获取当前的log级别
unsigned int GetLogLevelFromFile(int *LogLevel)
{
    int Fd = 0;
    if(access(LOG_LEVEL_SAVE_FILE , F_OK) == 0)
    {
        Fd = open(LOG_LEVEL_SAVE_FILE , O_RDWR , PERMISSION);
    }
    else
    {
        LOG_PRINTF(LOG_DEBUG , "the %s is null.can't get the log level.[ignore]" , LOG_LEVEL_SAVE_FILE);
        goto GET_FILE_ERR;
    }

    if(Fd < 0)
    {
        //默认使用LOG_WARNING
        LOG_PRINTF(LOG_ERROR , "open %s failed.Fd = %d." , LOG_LEVEL_SAVE_FILE , Fd);
        goto GET_FILE_ERR;
    }

    char buf[10] = {0};
    int ret = read(Fd , buf , sizeof(buf));
    if(ret < 0)
    {
        LOG_PRINTF(LOG_ERROR , "read %s failed.ret = %d." , LOG_LEVEL_SAVE_FILE , ret);
        goto GET_FILE_ERR;
    }
    *LogLevel = atoi(buf);

    if(*LogLevel > LOG_ERROR || *LogLevel < LOG_DEBUG)
    {
        LOG_PRINTF(LOG_ERROR , "the LogLevel num (%d) is error." , *LogLevel);
        goto GET_FILE_ERR;
    }

    return RET_LOG_OK;

GET_FILE_ERR:
    *LogLevel = LOG_WARNING;
    return RET_LOG_ERR;
}

static unsigned int CrLogInit(struct CrLogAttr_t * Attr) //是否使用锁，锁只有不同的线程之间才使用
{
    //to do ：打开各个Fd;
    if ( !Attr )
        return RET_LOG_ERR;

    CrPthreadMutexInit(&MutexLog, NULL);
    g_BoolUsingMutexFlag = Attr->UsingMutex;

    char Name[MAX_SIZE] = {0};
    unsigned char   TempFilePath[CRLOG_PATH_LENGTH] = {0};
    int  SFileFd = 0;

    strcpy(TempFilePath, Attr->FilePath);
    DeleteFileOverTheLimit(TempFilePath, g_FileName);
    CatFileName(Name, TempFilePath, g_FileName);
    SFileFd = CrFileOpen(Name, O_RDWR | O_CREAT, PERMISSION);

    //printf("File file Name : %s and fd : %d\n", Name, SFileFd);

    if ( SFileFd < 0 )
    {
        CrFileClose(SFileFd);
        return RET_LOG_ERR;
    }

    if ( SFileFd >= 0 )
    {
        CrFileClose(g_FileFd);
        g_FileFd = SFileFd;
        strcpy(Attr->FilePath, TempFilePath);
    }

    return RET_LOG_OK;
}

unsigned int CrLogIoctl()
{
    if (g_LogAttr.OutPutPathFlag == 0)
    {
        g_LogAttr.UsingMutex = 1;
        g_LogAttr.OutPutPathFlag = 0;
        strcpy(g_LogAttr.FilePath, "./");
    }

    return CrLogInit(&g_LogAttr);
}


unsigned int CrLogOpen(char *LogPath, char *FileName , int LogLevel)
{
#ifdef MAKEFILE_SET_LOG_LEVEL
    LOG_PRINTF(LOG_DEBUG , "makefile set log level = %d." , MAKEFILE_SET_LOG_LEVEL);
#endif
    if(LogLevel > LOG_ERROR || LogLevel < LOG_DEFAULT)
    {
        LOG_PRINTF(LOG_ERROR , "the log level (%d) is error!" , LogLevel);
    }
    //从loglevel中获取log级别
    if(GetLogLevelFromFile(&g_LogAttr.mLogLevel) == RET_LOG_ERR)
    {
        //如果获取不到log级别，则按照输入来作为log级别
//如果Makefile中有配置log级别，则先初始化为makefile中的log级别
#ifdef MAKEFILE_SET_LOG_LEVEL
        SetLogLevel(MAKEFILE_SET_LOG_LEVEL);
#endif
        if(LogLevel != LOG_DEFAULT)
            g_LogAttr.mLogLevel = LogLevel;
        else
            g_LogAttr.mLogLevel = g_LogLevel;
        SetLogLevel(g_LogAttr.mLogLevel);
    }
    else
    {
        //如果获取到了，则以配置文件的log级别为准，而不是以代码的；因为代码是无法在运行中更改的，所以配置文件优先;
        SetLogLevel(g_LogAttr.mLogLevel);
    }
    LOG_PRINTF(LOG_DEBUG , "the log level is %d(%d)." , g_LogAttr.mLogLevel , g_LogLevel);
    SaveLogLevelToFile(g_LogAttr.mLogLevel);
    //先判断文件名是否为空，如果为空，则使用默认名称
    if(FileName != NULL)
    {
        strncpy(g_FileName , FileName , sizeof(g_FileName));
    }
    LOG_PRINTF(LOG_DEBUG , "now,the log file name is %s." , g_FileName);

    if (LogPath == NULL)
    {
        g_LogAttr.OutPutPathFlag = 0;
        return CrLogIoctl();
    }
    g_LogAttr.OutPutPathFlag = 1;
    g_LogAttr.UsingMutex = 1;

    CrPthreadMutexInit(&MutexLog, NULL);

    char Name[MAX_SIZE] = {0};
    unsigned char   TempFilePath[CRLOG_PATH_LENGTH] = {0};
    int  SFileFd = 0;
    
    g_BoolUsingMutexFlag = g_LogAttr.UsingMutex;

    strcpy(TempFilePath, LogPath);
    DeleteFileOverTheLimit(TempFilePath, g_FileName);
    CatFileName(Name, TempFilePath, g_FileName);
    SFileFd = CrFileOpen(Name, O_RDWR | O_CREAT, PERMISSION);

    LOG_PRINTF(LOG_DEBUG , "File file Name : %s and fd : %d\n", Name, SFileFd);

    if ( SFileFd < 0 )
    {
        CrFileClose(SFileFd);
        return RET_LOG_ERR;
    }
    else
    {
        close(g_FileFd);
        g_FileFd = SFileFd;
        strcpy(g_LogAttr.FilePath, TempFilePath);
    }

    return RET_LOG_OK;
}

unsigned int CrLogWrite(int loglevel , char *Buff, unsigned int Size)
{
    //to do 根据Fd的值，大于0的全部写入
     //如果是写文件，注意检查文件大小与文件个数，如果+size超过大小，则更换文件编写，如果超过文件个数，则自动删除最旧一个
    if(Buff == NULL)
        return 0;
    if ( g_FileFd >= 0 )
    {
        unsigned char LogBuf[1024] = {0};
        time_t now = time(NULL);
        struct tm *CurTime = localtime(&now);
        // localtime_r(&now , &CurTime);

        snprintf(LogBuf , 1024 ,"[%s]-[%d/%d/%d-%d:%d:%d]-%s\r\n",                                   \
                g_LogLevelString[loglevel] ,                         \
                CurTime->tm_year + 1900, CurTime->tm_mon + 1, CurTime->tm_mday , \
                CurTime->tm_hour , CurTime->tm_min , CurTime->tm_sec ,\
                Buff);

        CrFileWrite(g_FileFd, LogBuf, strlen(LogBuf));
        if (IsChangedFile(g_FileFd, MAXFILELENGTH))
        {
                CrFileClose(g_FileFd);
                LOG_PRINTF(LOG_DEBUG , "File log should change log file");
                char Name[MAX_SIZE] = {0};
                CatFileName(Name, g_LogAttr.FilePath, g_FileName);
                int FileFd = CrFileOpen(Name, O_RDWR | O_CREAT, PERMISSION);
                if (FileFd < 0)
                {
                   LOG_PRINTF(LOG_ERROR , "open current : %s  failed, error : %s", Name, strerror(errno));
                   CrFileClose(FileFd);
                }
                else
                {
                   //删除文件
                   DeleteFileOverTheLimit(g_LogAttr.FilePath, g_FileName);
                   g_FileFd = FileFd;
                }
        }
        return Size;
    }
    return 0;
}

unsigned int CrLogClose()
{
    //to do：close 所有Fd
    CrFileClose(g_FileFd);

    CrPthreadMutexDestroy(&MutexLog);
    
    return 0;
}

unsigned int CrLogLock()
{
    if (g_BoolUsingMutexFlag)
    {
       return CrPthreadMutexLock(&MutexLog);
    }

    return 1;
}

unsigned int CrLogUnLock()
{
    if (g_BoolUsingMutexFlag)
    {
        return CrPthreadMutexUnLock(&MutexLog);
    }

    return 1;
}

unsigned int CrFileWrite(unsigned int Fd, const char *Buff, const unsigned int Size)
{
    if (Fd < 0)
    {
        return -1;
    }

    unsigned int U32Len = write(Fd, Buff, Size);

    return U32Len;
}

unsigned int CrFileOpen(const char* Name, unsigned int  access, unsigned int mode)
{
    if (!Name)
        return -1;

    unsigned int Fd = open(Name, access, mode);

    return Fd;
}

unsigned int CrFileClose(unsigned int Fd)
{
    close(Fd);
    //重置句柄
    Fd = -1;
    return RET_LOG_OK;
}

//检查文件是否更换
short IsChangedFile(unsigned int Fd, int FileMaxSize)
{
    int Len = lseek(Fd, 0, SEEK_END);
    short Changed = 0;
    if ( Len >= FileMaxSize )
        Changed = 1;

    return Changed;
}


//拼接文件名
/*
*   文件的全名是由文件路径+文件名+文件下标+文件类型组成
*   FileName 输出缓冲区，
*   path 日志文件路径
*   LogName 这个是固定的名称，在文件中有定义
*   最后加上时间的年月日时分秒
*/
short CatFileName(unsigned char* FileName, const unsigned char* Path, const unsigned char* LogName)
{
    if ( !FileName || !Path || !LogName )
        return -1;

    time_t CurTime = time(0);
    struct tm Time;
    localtime_r(&CurTime, &Time);

    char Name[100] = {0};
    char Buff[40] = {0};
    strcpy(Name, Path);
    sprintf(Buff ,"%s_%04d%02d%02d_%02d%02d%02d.txt", LogName, Time.tm_year + 1900, Time.tm_mon + 1, Time.tm_mday, Time.tm_hour, Time.tm_min, Time.tm_sec);
    strcat(Name, Buff);
    strcpy(FileName, Name);

    return 0;
}


//统计文件夹下的文件个数
static short CountLogFile(const char* Path, const char* Filter)
{
    if ( !Path )
        return -1;

    errno = 0;
    DIR *PathDir;
    PathDir = opendir(Path); /* 打开目录 多线程操作，没加锁的状态会导致路径是对的，但是报无效参数的错误*/
    if ( PathDir == NULL )
    {
        closedir(PathDir);
        //printf("CountLogFile ++++ opendir : %s , errNo = %d, error : %s\n", Path, errno, strerror(errno));
        return -1;
    }
    
    errno = 0;
    struct dirent * Ptr;
    int Total = 0;
    while ( ( Ptr = readdir(PathDir) ) != NULL )
    {
        //顺序读取每一个目bai录项；
        //跳过“du..”和“.”两个目录
        if ( strcmp(Ptr->d_name,".") == 0 || strcmp(Ptr->d_name,"..") == 0 )
        {
            continue;
        }

#if 0
........//printf("%s%s/n",root,ptr->d_name);
........//如果是目录，则递归调用 get_file_count函数
........if(ptr->d_type == DT_DIR)
........{
........sprintf(path,"%s%s/",root,ptr->d_name);
........//printf("%s/n",path);
........total += get_file_count(path);
........}
#endif

        if ( Ptr->d_type == DT_REG )
        {
            if ( !Filter ) //无筛选的内容
            {
                Total++;
                continue;
            }

            if ( strstr(Ptr->d_name, Filter) != NULL ) //有筛选的内容且匹配
            {
                Total++;
            }
            //printf("%s%s\n",root,ptr->d_name);
        }
    }

    
    closedir(PathDir);

    if ( errno != 0 )
    {
        Total = -1;
    }

    return Total;
}


/********
 fstat(int fd,struct stat *)接收的已open的文件描述符



stat(char *filename,struct stat *)接收的路径名， 需要注意的是 能处理符号链接，但处理的是符号链接指向的文件。


lstat(char *filename,struct stat *)接收的路径名  ，需要注意的是，也能能处理符号链接，但处理的是符号链接本身（自身）文件。 


***********/

// 获取修改时间最早的那个文件ming
static short GetMinModificationTimeFileName(char* Name, const char* Path, const char* Filter)
{
    if ( !Name || !Path  )
        return -1;

    DIR *PathDir;
    PathDir = opendir(Path); /* 打开目录*/
    if ( PathDir == NULL )
    {
        closedir(PathDir);
        //printf("opendir : %s , errNo = %d , error : %s\n", Path, errno, strerror(errno));
        return -1;
    }

    unsigned long long MinTime = 0;
    char MinPath[MAX_SIZE] = {0};
    struct dirent * Ptr;

    errno = 0;

    while ( ( Ptr = readdir(PathDir) ) != NULL )
    {
        //顺序读取每一个目bai录项；
        //跳过“du..”和“.”两个目录
        if ( strcmp(Ptr->d_name,".") == 0 || strcmp(Ptr->d_name,"..") == 0 )
        {
            continue;
        }

        if ( Ptr->d_type == DT_REG )
        {
            if ( Filter ) //有筛选的内容
            {
                if( strstr(Ptr->d_name, Filter) == NULL ) //不匹配
                    continue;
            }

            //获取文件的时间
            char FilePath[100] = {0};
            strcpy(FilePath, Path);
            strcat(FilePath, Ptr->d_name);
            Stat_t FileStat;
            if( stat(FilePath, &FileStat) < 0 )
            {
                continue;
            }

            if ( MinTime == 0 )
            {
                MinTime = FileStat.st_mtime;
                strcpy(MinPath, FilePath);
            }
            else
            {
                if( MinTime > FileStat.st_mtime )
                {
                    MinTime = FileStat.st_mtime;
                    strcpy(MinPath, FilePath);
                }
            }

        }
    }

    closedir(PathDir);
    
    if ( errno != 0 )
    {
        return -1;
    }
    
    strcpy(Name, MinPath);

    return 0;
}

// 删除文件夹下超过文件个数限制的文件
static void DeleteFileOverTheLimit(const char* Path, const char* Filter)
{
    if ( !Path )
        return;
    int Count = CountLogFile(Path, Filter);

    if (Count >= MAXFILECOUNT)
    {
        for (Count; Count > MAXFILECOUNT; Count--)
        {
            char Name[200] = {0};
            if ( GetMinModificationTimeFileName(Name, Path, Filter) >= 0 )
            {
                remove(Name);
            }
        }
    }

    return;
}











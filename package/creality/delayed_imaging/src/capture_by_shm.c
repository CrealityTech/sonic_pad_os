#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

enum DelayImagingRes{
    DELAY_IMAGING_START = 1,
    DELAY_IMAGING_SUCCESS = 0,
    DELAY_IMAGING_READ_FRAME_FAIL = -1,
    DELAY_IMAGING_ENCODE_H264_FAIL = -2,
    DELAY_IMAGING_ENCODE_ONE_H264_FAIL = -3,
    DELAY_IMAGING_MESSAGE_EXCETION = -4,
    DELAY_IMAGING_OTHER_EXCETION = -5,
};
#define DELAY_IMAGING_TIME      (1500)  //ms

#define PID_FILE        "/tmp/delayed_imaging.pid"

pid_t pid;

char *p = NULL;
int semID = 0;
union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};

int getkey(void)
{
    struct sembuf sops;
    sops.sem_num =0;
    sops.sem_op  =-1;       //负值，获取使用权
    sops.sem_flg = SEM_UNDO;
    return semop(semID, &sops,1);
}

int putkey(void)
{
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op  = 1;       //正直，释放
    sops.sem_flg = SEM_UNDO;
    return semop(semID, &sops,1);
}

int getCaptureFlag(void)
{
    int Flag = 0;
    if(getkey() < 0)
    {
        return -1;
    }
    memcpy(&Flag , (char *)p , sizeof(int));

    while(putkey() < 0)
    {
        usleep(1000);  
    }
    return Flag;
}

int setCaptureFlag(int num)
{
    int Flag = num;
    if(getkey() < 0)
    {
        return -1;
    }
    memcpy(p , &Flag , sizeof(int));

    while(putkey() < 0)
    {
        usleep(1000);  
    }
    return Flag;
}

int checkThePid(void)
{
    //check the pid running
    FILE *fp = NULL;
    int count = 0;
    char buf[100] = {0};
    memset(buf , 0 , sizeof(buf));
    sprintf(buf , "ps | grep %d | grep -v grep | wc -l" , pid);
    if((fp = popen(buf , "r")) == NULL)
    {
        return -1;
    }

    if(fgets(buf , sizeof(buf) , fp) != NULL)
    {
        count = atoi(buf);
        if(count < 1)
        {
            return -1;
        }
    }

    fclose(fp);
    return 0;
}


struct capture_t {
    int captureFlag;
};

int main()
{
    char buf[100] = {0};
    int fd = open(PID_FILE , O_RDONLY);
    if(fd < 0)
    {
        printf("%d", DELAY_IMAGING_OTHER_EXCETION);
        return -1;
    }
    read(fd , buf , sizeof(buf));
    pid = atoi(buf);
    close(fd);

    if(checkThePid() == 0)
    {
        // printf("the pid is alive...!\n");
    }
    //---------------------

    key_t keyID = ftok(PID_FILE , pid);
    if(keyID == -1)
    {
        printf("%d", DELAY_IMAGING_OTHER_EXCETION);
        return -1;
    }

    semID = semget(keyID , 1 , IPC_CREAT | 0666);
    if(semID == -1)
    {
        printf("%d", DELAY_IMAGING_OTHER_EXCETION);
        return -1;
    }

    int shmid = shmget(keyID , sizeof(int) , IPC_CREAT | 0666);
    if(shmid == -1)
    {
        printf("%d", DELAY_IMAGING_OTHER_EXCETION);
        return -1;
    }
    p = shmat(shmid,0, 0);
    if(p == (void *)-1)
    {
        printf("%d", DELAY_IMAGING_OTHER_EXCETION);
        return -1;
    }

    int captureFlag = 1;
    int saveCaptureFlag = 0;
    int captureCount = 0;

    if(checkThePid() != 0)
    {
        printf("%d", DELAY_IMAGING_OTHER_EXCETION);
        return -1;
    }

    // 设置开始抓拍
    setCaptureFlag(captureFlag);
    // 等待抓拍结果
    do{
        if (++captureCount > (DELAY_IMAGING_TIME/10))
        {
            captureFlag = DELAY_IMAGING_MESSAGE_EXCETION;
            break;
        }
        usleep(10000);
        captureFlag = getCaptureFlag();
    } while (captureFlag == DELAY_IMAGING_START);
    // 复位共享内存数据
    saveCaptureFlag = captureFlag;
    captureFlag = DELAY_IMAGING_SUCCESS;
    setCaptureFlag(captureFlag);

    usleep(1000);

    shmdt(p);

    printf("%d", saveCaptureFlag);
    return saveCaptureFlag;
}

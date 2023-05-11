/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                              hwmsgbox module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : error.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-4-27
* Descript: the error number of ar100 software.
* Update  : date                auther      ver     notes
*           2012-4-27 16:17:36  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __ERROR_H__
#define __ERROR_H__

/* standard define */
#define	EPERM		 1	/* Operation not permitted */
#define	ENOENT		 2	/* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EINTR		 4	/* Interrupted system call */
#define	EIO		 5	/* I/O error */
#define	ENXIO		 6	/* No such device or address */
#define	E2BIG		 7	/* Argument list too long */
#define	ENOEXEC		 8	/* Exec format error */
#define	EBADF		 9	/* Bad file number */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
#define	ENOTBLK		15	/* Block device required */
#define	EBUSY		16	/* Device or resource busy */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Cross-device link */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR		20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* File table overflow */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Not a typewriter */
#define	ETXTBSY		26	/* Text file busy */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Illegal seek */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Math argument out of domain of func */
#define	ERANGE		34	/* Math result not representable */
/* cpus define */
#define ETIMEOUT        35
#define EFAIL           36
#define EEMPTY          37
#define EFULL           38

/* standby progress record */
/* macro record */
#define REC_SSTANDBY     (0xf1f10000)
#define REC_NSTANDBY     (0xf2f20000)
#define REC_ESTANDBY     (0xf3f30000)
#define REC_FAKEPOWEROFF (0xf4f40000)
#define REC_TSTANDBY     (0xf5f50000)
/* micro record */
#define REC_COPY_DONE     (0x0000)
#define REC_ENTER         (0x1000)
#define REC_BEFORE_INIT   (0x2000)
#define REC_ENTER_INIT    (0x3000)
#define REC_AFTER_INIT    (0x4000)
#define REC_WAIT_WAKEUP   (0x5000)
#define REC_BEFORE_EXIT   (0x6000)
#define REC_ENTER_EXIT    (0x7000)
#define REC_AFTER_EXIT    (0x8000)
#define REC_DRAM_DBG      (0x9000)
#define REC_SHUTDOWN      (0xa000)
#define REC_HOTPULG       (0xb000)
#define REC_CPUX_RESUME   (0xc000)
#define REC_DEBUG         (0xd000)
#define REC_CPUS_LOOP     (0xe000)
#define REC_CPUS_WAKEUP   (0xf000)

#endif /* __ERROR_H__ */

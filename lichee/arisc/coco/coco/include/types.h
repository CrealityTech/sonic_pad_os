/*
*********************************************************************************************************
*                                                AR100 System
*                                     AR100 Software System Develop Kits
*                                               Basic Data Types
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : types.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-3-23
* Descript: general data types defines.
* Update  : date                auther      ver     notes
*           2012-3-23 13:53:43  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __TYPES_H__
#define __TYPES_H__

/*
 * return value defines
 */
#define OK      (0)
#define FAIL    (-1)
#define TRUE    (1)
#define FALSE   (0)

/*
 * general data type defines
 */
typedef void *          HANDLE;
typedef unsigned long long  u64;
typedef unsigned int        u32;
typedef unsigned short      u16;
typedef unsigned char       u8;
typedef signed long long    s64;
typedef signed int          s32;
typedef signed short        s16;
typedef signed char         s8;
typedef signed char         bool;

/*
 * general function pointer defines
 */
typedef s32  (* __pCBK_t)(void *p_arg); //call-back
typedef s32  (* __pISR_t)(void *p_arg,u32 intno); //ISR
typedef s32  (* __pNotifier_t)(u32 message, void *aux);//notifer call-back
typedef s32  (* __pCPUExceptionHandler)(void);       //cpu exception handler pointer
typedef s32  (* ar100_cb_t)(void *arg);
typedef s32  (* long_jump_fn)(void *arg);
typedef s32  (* mem_long_jump_fn)(u32 arg);
typedef s32 (*bvprintk)(u32 level, const char *format, va_list args);
typedef s32 (*ioctl__)(u32 devid, u32 requset, va_list args);
typedef s32 (*syscall__)(u32 callid, va_list args);
typedef void (*cococall)(void);

/*
 * Register read and write operations
 */
#define readb(reg)              (*(volatile u8  *)(reg))
#define readw(reg)              (*(volatile u16 *)(reg))
#define readl(reg)              (*(volatile u32 *)(reg))

#define writeb(val, reg)        (*(volatile u8  *)(reg) = (val))
#define writew(val, reg)        (*(volatile u16 *)(reg) = (val))
#define writel(val, reg)        (*(volatile u32 *)(reg) = (val))


/*
 * Basic bit operations
 */
#define SET_REG_BIT(val, pos)               ((val) |= 1U<<(pos))
#define CLEAR_REG_BIT(val, pos)             ((val) &= ~(1U<<(pos)))
#define TEST_REG_BIT(val, pos)              ((val) & (1U<<(pos)))
#define BITS_REG_MOD(len)                   ((1U<<(len)) - 1)
#define BITS_REG_MASK(len, pos)             (~(BITS_REG_MOD(len)<<(pos)))
#define CLEAR_REG_BITS(val, pos, len)       (val) &= BITS_REG_MASK(len, pos)
#define SET_REG_BITS(val, pos, len, _val)   (val) = ((val) & BITS_REG_MASK(len, pos)) | (((_val) & BITS_REG_MOD(len))<<(pos))
#define TEST_REG_BITS(val, pos, len, _val)  ((val) & (~BITS_REG_MASK(len, pos))) == ((_val)<<(pos))
#define GET_REG_BITS_VAL(val, pos, len)     (((val)>>(pos)) & BITS_REG_MOD(len))

#define BITS_PER_LONG       32
#define DIV_ROUND_UP(n,d)   (((n) + (d) - 1) / (d))
#define BITS_TO_LONGS(nr)   DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))


/*
 * misc operations
 */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


#endif  /* __TYPES_H__ */

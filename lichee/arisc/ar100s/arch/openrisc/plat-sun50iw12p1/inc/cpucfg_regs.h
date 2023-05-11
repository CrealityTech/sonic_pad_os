/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                cpucfg module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : cpucfg_regs.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-7
* Descript: cpu config register defines.
* Update  : date                auther      ver     notes
*           2012-5-7 18:54:47   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __CPUCFG_REGS_H__
#define __CPUCFG_REGS_H__

/* TIMESTAMP REGS */
#define CNT_LOW_REG             (TS_STAT_REG_BASE + 0x0000)
#define CNT_HIGH_REG            (TS_STAT_REG_BASE + 0x0004)
#define CNT_CTRL_REG            (TS_CTRL_REG_BASE + 0x0000)
#define CNT_LOW_REG_SET         (TS_CTRL_REG_BASE + 0x0008)
#define CNT_HIGH_REG_SET        (TS_CTRL_REG_BASE + 0x000C)
#define CNT_FREQID_REG          (TS_CTRL_REG_BASE + 0x0020)

/* CPUSCFG REGS */
#define CPUS_SPACE_LE_CTRL_REG  (R_CPUCFG_REG_BASE + 0x000C)
#define CPUS_SPACE_START_REG(n) (R_CPUCFG_REG_BASE + 0x0010 + n * 8)
#define CPUS_SPACE_END_REG(n)   (R_CPUCFG_REG_BASE + 0x0014 + n * 8)

#define C0_CPUX_POWEROFF_GATING_REG  (R_CPUCFG_REG_BASE + 0x0044)

#define SUNXI_CLUSTER_PWRON_RESET(cluster)        (R_CPUCFG_REG_BASE + 0x40 + (cluster<<2))
#define SUNXI_CLUSTER_PWROFF_GATING(cluster)      (R_CPUCFG_REG_BASE + 0x44 + (cluster<<2))
#define SUNXI_CPU_PWR_CLAMP(cluster, cpu)         (R_CPUCFG_REG_BASE + 0x50 + (cluster<<4) + (cpu<<2))

#define SUNXI_CPU_SYS_RESET                       (R_CPUCFG_REG_BASE + 0x0A0)
#define SUNXI_IRQ_FIQ_MASK			  (R_CPUCFG_REG_BASE + 0x10c)

#endif /* __CPUCFG_REGS_H__ */

/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : task_handle.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:34
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __TASK_HANDLE_H__
#define __TASK_HANDLE_H__



//the base of messages
#define MESSAGE_BASE            (0x10)

#define PSCI_CPU_OPS		(MESSAGE_BASE + 0x00)
#define ARISC_WAKEUP_ALREADY	(MESSAGE_BASE + 0x04)
#define SYS_OP_REQ              (MESSAGE_BASE + 0x14)     /* cpu operations         (ac327 to arisc)          */
#define ARISC_CLEAR_WAKEUP_SRC	(MESSAGE_BASE + 0x15)
#define ARISC_SET_WAKEUP_SRC	(MESSAGE_BASE + 0x16)

#define PMU_OPS_HDLE		(MESSAGE_BASE + 0x40)
#define RSB_OPS_HDLE		(MESSAGE_BASE + 0x70)

#define ARISC_DVFS_OPS		(MESSAGE_BASE + 0x40)
#define CPUS_LOOP_BACK		(MESSAGE_BASE + 0x51)


/* rsb commands */
#define RSB_READ_BLOCK_DATA         (MESSAGE_BASE + 0x70)  /* rsb read block data        (ac327 to arisc)      */
#define RSB_WRITE_BLOCK_DATA        (MESSAGE_BASE + 0x71)  /* rsb write block data       (ac327 to arisc)      */
#define RSB_BITS_OPS_SYNC           (MESSAGE_BASE + 0x72)  /* rsb clear bits sync        (ac327 to arisc)      */
#define RSB_SET_RTSADDR             (MESSAGE_BASE + 0x74)  /* rsb set runtime slave addr (ac327 to arisc)      */

/* pmu commands */
#define AXP_DISABLE_IRQ             (MESSAGE_BASE + 0x41)  /* disable axp irq of ar200                         */
#define AXP_ENABLE_IRQ              (MESSAGE_BASE + 0x42)  /* enable axp irq of ar200                          */
#define SET_PMU_VOLT                (MESSAGE_BASE + 0x45)  /* set pmu voltage (ac327 to arisc)                 */
#define GET_PMU_VOLT                (MESSAGE_BASE + 0x46)  /* get pmu voltage (ac327 to arisc)                 */
#define CLR_NMI_STATUS              (MESSAGE_BASE + 0x4a)  /* clear nmi status (ac327 to arisc)                */
#define SET_NMI_TRIGGER             (MESSAGE_BASE + 0x4b)  /* set nmi tigger (ac327 to arisc)                  */
#define SET_PMU_VOLT_STA            (MESSAGE_BASE + 0x4c)  /* set pmu voltage state(ac327 to arisc)            */
#define GET_PMU_VOLT_STA            (MESSAGE_BASE + 0x4d)  /* get pmu voltage state(ac327 to arisc)            */

//set debug level commands
#define SET_DRAM_CRC_PARAS          (MESSAGE_BASE + 0x54)  /* config dram crc parameters (ac327 to ar100)      */

#define CPUS_STARTUP_NOTIFY	(MESSAGE_BASE + 0x80)
#define CPUS_SYSTEM_DEBUG	(MESSAGE_BASE + 0x85)

#define TASK_GID_DEFAULT	0
#define TASK_GID_PSCI		21
#define TASK_GID_SYSBEBUG	5
#define TASK_GID_DAEMON		20

#define TASK_GID_PMU		31
#define TASK_GID_RSB		32


#endif


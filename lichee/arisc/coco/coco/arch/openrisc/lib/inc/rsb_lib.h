#ifndef __RSB_HAL_H__
#define __RSB_HAL_H__

//#include "rsb_platform.h"
#include <sys/errno.h>

typedef struct {
	u32 RSB_RuntimeAddr;
	u32 RSB_DevAddr;
	u32 RSB_TransRate;
} RSB_InitConfig;

#define RSB_TOVER_INT           (1U << 0)
#define RSB_TERR_INT            (1U << 1)
#define RSB_LBSY_INT            (1U << 2)

/* RSB SHIFT */
#define RSB_RTSADDR_SHIFT       (16)//runtime slave address shift
#define RSB_SADDR_SHIFT         (0)//Slave Address shift

/* RSB command */
#define RSB_CMD_BYTE_WRITE      (0x4E)//(0x27)//Byte write
#define RSB_CMD_HWORD_WRITE     (0x59)//(0x2c)//Half word write
#define RSB_CMD_WORD_WRITE      (0x63)//(0x31)//Word write
#define RSB_CMD_BYTE_READ       (0x8B)//(0x45)//Byte read
#define RSB_CMD_HWORD_READ      (0x9C)//(0x4e)//Half word read
#define RSB_CMD_WORD_READ       (0xA6)//(0x53)//Word read
#define RSB_CMD_SET_RTSADDR     (0xE8)//(0x74)//Set Run-time Address

/* RSB Device Slave Address */
#define RSB_SADDR1              (0x3A3) //(0x01d1)AXP22x(AW1636)
#define RSB_SADDR2              (0x4E6) //(0x0273)
#define RSB_SADDR3              (0x745) //(0x03a2)AXP15x(AW1657)
#define RSB_SADDR4              (0x9CC) //(0x04e6)
#define RSB_SADDR5              (0xA6F) //(0x537)
#define RSB_SADDR6              (0xD2A) //(0x0695)
#define RSB_SADDR7              (0xE89) //(0x0744)
#define RSB_SADDR8              (0x103B)//(0x081d)
#define RSB_SADDR9              (0x1398)//(0x09cc)
#define RSB_SADDR10             (0x14DD)//(0x0a6e)
#define RSB_SADDR11             (0x177E)//(0x0bbf)


/* RSB run time address */
//#define RSB_RTSADDR1  (0x17)//(0x0B)
#define RSB_RTSADDR2    (0x2d)//(0x16)
#define RSB_RTSADDR3    (0x3A)//(0x1d)
#define RSB_RTSADDR4    (0x4E)//0x27
#define RSB_RTSADDR5    (0x59)//0x2c
#define RSB_RTSADDR6    (0x63)//0x31
#define RSB_RTSADDR7    (0x74)//0x3a
#define RSB_RTSADDR8    (0x8B)//0x45
#define RSB_RTSADDR9    (0x9C)//0x4e
#define RSB_RTSADDR10   (0xA6)//(0x53)
#define RSB_RTSADDR11   (0xB1)//(0x58)
#define RSB_RTSADDR12   (0xC5)//(0x62)
#define RSB_RTSADDR13   (0xD2)//(0x69)
#define RSB_RTSADDR14   (0xE8)//(0x74)
#define RSB_RTSADDR15   (0xFF)//(0x7f)

#define RSB_INVALID_RTSADDR (RSB_RTSADDR0)
#define RSB_RTSADDR0  (0x00)

#define RSB_SCK                 		(3000000)		/* the rsb SCK freq(3M) */
#define RSB_SCK_FREQ                 	(3000000)		/* the rsb SCK freq(3M) */
#define RSB_SDAODLY             		(1)				/* SDA output delay */
#define RSB_TRANS_BYTE_MAX              (4)             /* the max number of RSB transfer byte */

#define TWI_CLOCK_FREQ                  (400 * 1000)    /* the twi source clock freq */

//rsb or twi pmu mode bit field
#define RSB_PMU_INIT            (1U << 31)
#define NTWI_PMU_INIT           (0U << 31)

extern void RSB_DefInit(RSB_InitConfig *config);
void RSB_Reg_Init(RSB_TypedDef *RSBx, u32 reg_base);
extern s32 RSB_Config(RSB_TypedDef *RSBx, RSB_InitConfig *config);
extern s32 RSB_Wait_State(RSB_TypedDef *RSBx);

#ifdef CFG_RSB_BYTE_OP
extern s32 RSB_Read_Byte(RSB_TypedDef *RSBx, u32 devaddr, u32 regaddr,
			 u32 *data);
extern s32 RSB_Write_Byte(RSB_TypedDef *RSBx, u32 devaddr, u32 regaddr,
			  u32 *data);
#else
static inline s32 RSB_Read_Byte(RSB_TypedDef *RSBx, u32 devaddr, u32 regaddr,
				u32 *data)
{
	return -ENOSYS;
}
static inline s32 RSB_Write_Byte(RSB_TypedDef *RSBx, u32 devaddr, u32 regaddr,
				 u32 *data)
{
	return -ENOSYS;
}

#endif

#ifdef CFG_RSB_HALDWORD_OP
extern s32 RSB_Read_Hword(RSB_TypedDef *RSBx, u32 devaddr, u32 regaddr,
			  u32 *data);
extern s32 RSB_Write_Hword(RSB_TypedDef *RSBx, u32 devaddr, u32 regaddr,
			   u32 data);
#else
static inline s32 RSB_Read_Hword(RSB_TypedDef *RSBx, u32 devaddr, u32 regaddr,
				 u32 *data)
{
	return -ENOSYS;
}
static inline s32 RSB_Write_Hword(RSB_TypedDef *RSBx, u32 devaddr, u32 regaddr,
				  u32 data)
{
	return -ENOSYS;
}
#endif

#ifdef CFG_RSB_WORD_OP
extern s32 RSB_Read_Word(RSB_TypedDef *RSBx, u32 devaddr, u32 regaddr,
			 u32 *data);
extern s32 RSB_Write_Word(RSB_TypedDef *RSBx, u32 devaddr, u32 regaddr,
			  u32 data);
#else
static inline s32 RSB_Read_Word(RSB_TypedDef *RSBx, u32 devaddr, u32 regaddr,
				u32 *data)
{
	return -ENOSYS;
}

static inline s32 RSB_Write_Word(RSB_TypedDef *RSBx, u32 devaddr, u32 regaddr,
				 u32 data)
{
	return -ENOSYS;
}
#endif

extern void RSB_Soft_Reset(RSB_TypedDef *RSBx);
extern s32 RSB_Set_PMU_Mode(RSB_TypedDef *RSBx, u32 slave_addr, u32 regaddr,
			    u32 data);
extern s32 RSB_Set_Runtime_Addr(RSB_TypedDef *RSBx, u32 addr, u32 rtaddr);
extern s32 RSB_Set_Clk(RSB_TypedDef *RSBx, u32 clk);

#endif


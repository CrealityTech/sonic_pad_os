#ifndef __PIN_LIB_H__
#define __PIN_LIB_H__

#include "lib_inc.h"


#define PL_NUM 11
#define PM_NUM 5

//pin controller register list
#define PIN_REG_CFG(n, i)           ((volatile u32 *)(R_PIO_REG_BASE + ((n)-1)*0x24 + ((i)>>3)*0x4 + 0x00))
#define PIN_REG_DLEVEL(n, i)        ((volatile u32 *)(R_PIO_REG_BASE + ((n)-1)*0x24 + ((i)>>4)*0x4 + 0x14))
#define PIN_REG_PULL(n, i)          ((volatile u32 *)(R_PIO_REG_BASE + ((n)-1)*0x24 + ((i)>>4)*0x4 + 0x1C))
#define PIN_REG_DATA(n)             ((volatile u32 *)(R_PIO_REG_BASE + ((n)-1)*0x24 + 0x10))

#define PIN_REG_CFG_VALUE(n, i)     readl(R_PIO_REG_BASE + ((n)-1)*0x24 + ((i)>>3)*0x4 + 0x00)
#define PIN_REG_DLEVEL_VALUE(n, i)  readl(R_PIO_REG_BASE + ((n)-1)*0x24 + ((i)>>4)*0x4 + 0x14)
#define PIN_REG_PULL_VALUE(n, i)    readl(R_PIO_REG_BASE + ((n)-1)*0x24 + ((i)>>4)*0x4 + 0x1C)
#define PIN_REG_DATA_VALUE(n)       readl(R_PIO_REG_BASE + ((n)-1)*0x24 + 0x10)

#define PIN_REG_INT_CFG(n, i)       ((volatile u32 *)(R_PIO_REG_BASE + (n-1)*0x20 + ((i)>>3)*0x4 + 0x200))
#define PIN_REG_INT_CTL(n)          ((volatile u32 *)(R_PIO_REG_BASE + (n-1)*0x20 + 0x210))
#define PIN_REG_INT_STAT(n)         ((volatile u32 *)(R_PIO_REG_BASE + (n-1)*0x20 + 0x214))
#define PIN_REG_INT_DEBOUNCE(n)     ((volatile u32 *)(R_PIO_REG_BASE + (n-1)*0x20 + 0x218))

#define	PIN_VERSION_REG			0x0370



s32 Pin_Clk_Switch(u32 pin_grp, u32 source);
s32 Pin_Set_Multi_Sel(u32 pin_grp, u32 pin_num, u32 multi_sel);
s32 Pin_Set_Pull(u32 pin_grp, u32 pin_num, u32 pull);
s32 Pin_Set_Drive(u32 pin_grp, u32 pin_num, u32 drive);
u32 Pin_Grp_Int_Status(u32 pin_grp);
s32 Pin_Write_Data(u32 pin_grp, u32 pin_num, u32 data);
u32 Pin_Read_Data(u32 pin_grp, u32 pin_num);

s32 Exti_Set_Int_Trigger_Mode(u32 eint_grp, u32 eint_num, u32 mode);
s32 Exti_Disable_Int(u32 eint_grp, u32 eint_num);
u32 Exti_Query_Pending(u32 eint_grp, u32 eint_num);
u32 Exti_Query_Grp_Pending(u32 eint_grp);
s32 Exti_Clear_Pending(u32 eint_grp, u32 eint_num);
s32 Exti_Enable_Int(u32 eint_grp, u32 eint_num);
s32 Exti_Set_Int_Debounce(u32 eint_grp, u32 eint_pre, u32 eint_clk);
s32 Exti_Standby_Init(u32 gpio_enable_bitmap);
s32 Exti_Standby_Exit(void);



#endif

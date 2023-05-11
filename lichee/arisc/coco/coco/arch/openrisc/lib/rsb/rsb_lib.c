
#include "lib_inc.h"

void RSB_DefInit(RSB_InitConfig *config)
{
	RSB_InitConfig *rsb_config = config;

	rsb_config->RSB_TransRate	= RSB_SCK_FREQ;
}

void RSB_Reg_Init(RSB_TypedDef *RSBx, u32 reg_base)
{
	RSBx->control			= (rsb_ctrl_t *)(reg_base + RSB_CTRL);
	RSBx->clk_ctrl			= (rsb_ccr_t	*)(reg_base + RSB_CCR);
	RSBx->irq_enable		= (rsb_inte_t *)(reg_base + RSB_INTE);
	RSBx->status			= (rsb_stat_t *)(reg_base + RSB_STAT);
	RSBx->address           = (rsb_ar_t *)(reg_base + RSB_AR);
	RSBx->data              = (rsb_data_t *)(reg_base + RSB_DATA);
	RSBx->line_ctrl         = (rsb_lcr_t *)(reg_base + RSB_LCR);
	RSBx->dev_mode_ctrl     = (rsb_dmcr_t *)(reg_base + RSB_DMCR);
	RSBx->command			= (rsb_cmd_t *)(reg_base + RSB_CMD);
	RSBx->dev_address       = (rsb_dar_t *)(reg_base + RSB_DAR);
	RSBx->arbitration       = (rsb_arbiter_t *)(reg_base + RSB_ARBITER);
	RSBx->version           = (rsb_ver_t *)(reg_base + RSB_VER);
}

s32 RSB_Config(RSB_TypedDef *RSBx,RSB_InitConfig *config)
{

	RSB_Set_Clk(RSBx,config->RSB_TransRate);

	return OK;
}

s32 RSB_Set_Clk(RSB_TypedDef *RSBx,u32 clk)
{
	u32 sda_odly;

	u32 rval;

	u32 src_clk = Get_APBS2_Freq();

	u32 div = (src_clk + clk) / clk / 2 - 1;

	if (div > 0xff)
		ERR("rsb div overflow");

	div &= 0xff;
        sda_odly = ((div >> 1) - 1) >> 1;
        if (sda_odly < 1)
                sda_odly = 1;
        else if (sda_odly > 7)
                sda_odly = 7;
	rval = (div & 0xff) | ((sda_odly & 0x7) << 8);

	RSBx->clk_ctrl->reg = rval;

	return OK;

}

s32 RSB_Wait_State(RSB_TypedDef *RSBx)
{
	s32  ret = FAIL;
	u32  stat;
	while (1)
	{
		stat = RSBx->status->reg;
		if (stat & RSB_LBSY_INT)
		{
			//transfer error
			ERR("loading busy\n");
			ret = -EBUSY;
			break;
		}
		if (stat & RSB_TERR_INT)
		{
			//transfer error
			ERR("rsb trans err[%x]\n", stat);
			ret = -EFAIL;
			break;
		}
		if (stat & RSB_TOVER_INT)
		{
			//transfer complete
			ret = OK;
			break;
		}
	}

	//clear state flag
	RSBx->status->reg = stat;

	return ret;
}

#ifdef CFG_RSB_BYTE_OP
s32 RSB_Read_Byte(RSB_TypedDef *RSBx, u32 devaddr, u32 regaddr, u32 *data)
{
#ifdef CFG_HWCPUIDLE_USED
	while(RSBx->arbitration->bits.hw_busy_set)
		;
	RSBx->arbitration->bits.sw_busy_set = 1;
#endif

	RSBx->dev_address->bits.rta = devaddr;
	RSBx->address->reg = regaddr;

	RSBx->command->reg = RSB_CMD_BYTE_READ;

	RSBx->control->bits.start_trans = 1;

	if (RSB_Wait_State(RSBx) != OK)
	{
		ERR("rsb read failed\n");
		ERR("read devaddr:%x, regaddr:%x\n", devaddr, regaddr);
		return -EFAIL;
	}

	*data = (RSBx->data->reg) & 0xff;

#ifdef CFG_HWCPUIDLE_USED
	RSBx->arbitration->bits.sw_busy_set = 0;
#endif

	return OK;
}

s32 RSB_Write_Byte(RSB_TypedDef *RSBx,u32 devaddr,u32 regaddr,u32 *data)
{
#ifdef CFG_HWCPUIDLE_USED
	while(RSBx->arbitration->bits.hw_busy_set)
		;
	RSBx->arbitration->bits.sw_busy_set = 1;
#endif

	RSBx->dev_address->bits.rta = devaddr;
	RSBx->address->reg = regaddr;

	RSBx->data->reg = *data;
	RSBx->command->reg = RSB_CMD_BYTE_WRITE;
	RSBx->control->bits.start_trans = 1;

	if (RSB_Wait_State(RSBx) != OK)
	{
		ERR("rsb read failed\n");
		ERR("read devaddr:%x, regaddr:%x\n", devaddr, regaddr);
		return -EFAIL;
	}

#ifdef CFG_HWCPUIDLE_USED
	RSBx->arbitration->bits.sw_busy_set = 0;
#endif

	return OK;
}
#endif

#ifdef CFG_RSB_HALDWORD_OP
s32 RSB_Read_Hword(RSB_TypedDef *RSBx,u32 devaddr,u32 regaddr,u32 *data)
{
#ifdef CFG_HWCPUIDLE_USED
	while(RSBx->arbitration->bits.hw_busy_set)
		;
	RSBx->arbitration->bits.sw_busy_set = 1;
#endif

	RSBx->dev_address->bits.rta = devaddr;
	RSBx->address->reg = regaddr;

	RSBx->command->reg = RSB_CMD_HWORD_READ;

	RSBx->control->bits.start_trans = 1;

	if (RSB_Wait_State(RSBx) != OK)
	{
		ERR("rsb read failed\n");
		ERR("read devaddr:%x, regaddr:%x\n", devaddr, regaddr);
		return -EFAIL;
	}

	*data = (RSBx->data->reg) & 0xffff;

#ifdef CFG_HWCPUIDLE_USED
	RSBx->arbitration->bits.sw_busy_set = 0;
#endif

	return OK;
}

s32 RSB_Write_Hword(RSB_TypedDef *RSBx,u32 devaddr,u32 regaddr, u32 data)
{
#ifdef CFG_HWCPUIDLE_USED
	while(RSBx->arbitration->bits.hw_busy_set)
		;
	RSBx->arbitration->bits.sw_busy_set = 1;
#endif

	RSBx->dev_address->bits.rta = devaddr;
	RSBx->address->reg = regaddr;

	RSBx->data.reg = data;
	RSBx->command->reg = RSB_CMD_HWORD_WRITE;
	RSBx->control->bits.start_trans = 1;

	if (RSB_Wait_State(RSBx) != OK)
	{
		ERR("rsb read failed\n");
		ERR("read devaddr:%x, regaddr:%x\n", devaddr, regaddr);
		return -EFAIL;
	}

#ifdef CFG_HWCPUIDLE_USED
	RSBx->arbitration->bits.sw_busy_set = 0;
#endif

	return OK;
}
#endif

#ifdef CFG_RSB_WORD_OP
s32 RSB_Read_Word(RSB_TypedDef *RSBx,u32 devaddr,u32 regaddr,u32 *data)
{
#ifdef CFG_HWCPUIDLE_USED
	while(RSBx->arbitration->bits.hw_busy_set)
		;
	RSBx->arbitration->bits.sw_busy_set = 1;
#endif

	RSBx->dev_address->bits.rta = devaddr;
	RSBx->address->reg = regaddr;

	RSBx->command->reg = RSB_CMD_WORD_READ;

	RSBx->control->bits.start_trans = 1;

	if (RSB_Wait_State(RSBx) != OK)
	{
		ERR("rsb read failed\n");
		ERR("read devaddr:%x, regaddr:%x\n", devaddr, regaddr);
		return -EFAIL;
	}

	*data = (RSBx->data->reg) & 0xffff;

#ifdef CFG_HWCPUIDLE_USED
	RSBx->arbitration->bits.sw_busy_set = 0;
#endif

	return OK;
}

s32 RSB_Write_Word(RSB_TypedDef *RSBx,u32 devaddr,u32 regaddr,u32 data)
{
#ifdef CFG_HWCPUIDLE_USED
	while(RSBx->arbitration->bits.hw_busy_set)
		;
	RSBx->arbitration->bits.sw_busy_set = 1;
#endif

	RSBx->dev_address->bits.rta = devaddr;
	RSBx->address->reg = regaddr;

	RSBx->data.reg = data;
	RSBx->command->reg = RSB_CMD_WORD_WRITE;
	RSBx->control->bits.start_trans = 1;

	if (RSB_Wait_State(RSBx) != OK)
	{
		ERR("rsb read failed\n");
		ERR("read devaddr:%x, regaddr:%x\n", devaddr, regaddr);
		return -EFAIL;
	}

#ifdef CFG_HWCPUIDLE_USED
	RSBx->arbitration->bits.sw_busy_set = 0;
#endif

	return OK;
}
#endif

void RSB_Soft_Reset(RSB_TypedDef *RSBx)
{
	RSBx->control->bits.soft_reset = 1;
}

s32 RSB_Set_PMU_Mode(RSB_TypedDef *RSBx, u32 slave_addr, u32 regaddr, u32 data)
{
	RSBx->dev_mode_ctrl->reg = RSB_PMU_INIT | (slave_addr << 1) | (regaddr << 8) | (data << 16);

	while(RSBx->dev_mode_ctrl->bits.device_mode_start)
		;

	if (RSB_Wait_State(RSBx) != OK)
	{
		ERR("rsb read failed\n");
		ERR("slave_addr:%x, regaddr:%x\n", slave_addr, regaddr);
		return -EFAIL;
	}

	return OK;
}

s32 RSB_Set_Runtime_Addr(RSB_TypedDef *RSBx, u32 saddr ,u32 rtsaddr)
{
	RSBx->dev_address->reg = \
		(saddr & 0xffff) | ((rtsaddr & 0xff) << 16);
	RSBx->command->reg = RSB_CMD_SET_RTSADDR;

	RSBx->control->bits.start_trans = 1;

	if (RSB_Wait_State(RSBx) != OK)
	{
		ERR("rsb read failed\n");
		ERR("saddr:%x, rtsaddr:%x\n", saddr, rtsaddr);
		return -EFAIL;
	}

	return OK;
}


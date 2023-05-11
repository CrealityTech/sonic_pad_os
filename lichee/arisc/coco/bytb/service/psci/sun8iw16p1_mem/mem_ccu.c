#include "includes.h"
#include "mem.h"

struct system_state *system_back = (struct system_state *)ARISC_USE_DRAM_DATA_PBASE;

#define CCM_REG_BASE system_back->ccm_back.ccm_reg_base
#define CCM_REG_BACK system_back->ccm_back.ccm_reg_back

static int mem_restore_pll(unsigned int bk_pll_ctl,void *base_pll_ctl)
{
	unsigned int factor = 0;
	unsigned int bk = bk_pll_ctl;
	unsigned int *base = (unsigned int *)base_pll_ctl;
	unsigned int i,j = 0;

	if(bk >> 31) {
		do{
			bk = bk_pll_ctl;
			factor = (bk >> 8) & 0xff;
			bk &= ~(0xff << 8);
			bk |= (0x50 << 8);
			writel(bk,(unsigned int)base);
			__time_udelay(10);
			bk &= ~(0xff << 8);
			bk |= (factor << 8);
			bk |= (0x1 << 29);
			writel(bk,(unsigned int)base);
			i = 0;
			while(((readl((unsigned int)base) >> 28)&0x1) != 1)  {
				__time_udelay(100);
				i++;
				if( i>= 50) {
					bk &= ~(0x1 << 31);
					writel(bk,(unsigned int)base);
					break;
				}
			}
			if(i < 50)
				break;
			j++ ;
		}while(j < 5);
		if(j == 5) {
			while(((readl((unsigned int)base) >> 28)&0x1) != 1)
				;
		}
	}
	else
		writel(bk,(unsigned int)base);

	return OK;
}

int mem_ccu_save(void)
{
	ASSERT(&CCM_REG_BASE->PllLockCtrl != SUNXI_CCM_PEND);

	system_back->ccm_back.ccm_reg_base = (__ccmu_reg_list_t *)(SUNXI_CCM_PBASE);

	//ctrl:
	CCM_REG_BACK.PllCpuxCtrl       = CCM_REG_BASE->PllCpuxCtrl;
//	CCM_REG_BACK.PllDdrCtrl        = CCM_REG_BASE->PllDdrCtrl;
	CCM_REG_BACK.PllPeri0Ctrl      = CCM_REG_BASE->PllPeri0Ctrl;
	CCM_REG_BACK.PllPeri1Ctrl      = CCM_REG_BASE->PllPeri1Ctrl;
	CCM_REG_BACK.PllGpuCtrl        = CCM_REG_BASE->PllGpuCtrl;
	CCM_REG_BACK.PllVideo0Ctrl     = CCM_REG_BASE->PllVideo0Ctrl;
	CCM_REG_BACK.PllVideo1Ctrl     = CCM_REG_BASE->PllVideo1Ctrl;
	CCM_REG_BACK.PllVeCtrl         = CCM_REG_BASE->PllVeCtrl;
	CCM_REG_BACK.PllDeCtrl         = CCM_REG_BASE->PllDeCtrl;
	CCM_REG_BACK.PllAudioCtrl      = CCM_REG_BASE->PllAudioCtrl;
	CCM_REG_BACK.Pll24mCtrl        = CCM_REG_BASE->Pll24mCtrl;

	//pat:
//	CCM_REG_BACK.PllDdrPatCtrl     = CCM_REG_BASE->PllDdrPatCtrl;
	CCM_REG_BACK.PllPeri0Pat0Ctrl  = CCM_REG_BASE->PllPeri0Pat0Ctrl;
	CCM_REG_BACK.PllPeri0Pat1Ctrl  = CCM_REG_BASE->PllPeri0Pat1Ctrl;
	CCM_REG_BACK.PllPeri1Pat0Ctrl  = CCM_REG_BASE->PllPeri1Pat0Ctrl;
	CCM_REG_BACK.PllPeri1Pat1Ctrl  = CCM_REG_BASE->PllPeri1Pat1Ctrl;
	CCM_REG_BACK.PllGpu0Pat0Ctrl   = CCM_REG_BASE->PllGpu0Pat0Ctrl;
	CCM_REG_BACK.PllGpu0Pat1Ctrl   = CCM_REG_BASE->PllGpu0Pat1Ctrl;
	CCM_REG_BACK.PllVideo0Pat0Ctrl = CCM_REG_BASE->PllVideo0Pat0Ctrl;
	CCM_REG_BACK.PllVideo0Pat1Ctrl = CCM_REG_BASE->PllVideo0Pat1Ctrl;
	CCM_REG_BACK.PllVideo1Pat0Ctrl = CCM_REG_BASE->PllVideo1Pat0Ctrl;
	CCM_REG_BACK.PllVideo1Pat1Ctrl = CCM_REG_BASE->PllVideo1Pat1Ctrl;
	CCM_REG_BACK.PllVePat0Ctrl     = CCM_REG_BASE->PllVePat0Ctrl;
	CCM_REG_BACK.PllVePat1Ctrl     = CCM_REG_BASE->PllVePat1Ctrl;
	CCM_REG_BACK.PllDePat0Ctrl     = CCM_REG_BASE->PllDePat0Ctrl;
	CCM_REG_BACK.PllDePat1Ctrl     = CCM_REG_BASE->PllDePat1Ctrl;
	CCM_REG_BACK.PllAudioPat0Ctrl  = CCM_REG_BASE->PllAudioPat0Ctrl;
	CCM_REG_BACK.PllAudioPat1Ctrl  = CCM_REG_BASE->PllAudioPat1Ctrl;

	//bias:
	CCM_REG_BACK.PllCpuxBias       = CCM_REG_BASE->PllCpuxBias;
//	CCM_REG_BACK.PllDdrBias        = CCM_REG_BASE->PllDdrBias;
	CCM_REG_BACK.PllPeri0Bias      = CCM_REG_BASE->PllPeri0Bias;
	CCM_REG_BACK.PllPeri1Bias      = CCM_REG_BASE->PllPeri1Bias;
	CCM_REG_BACK.PllGpu0Bias       = CCM_REG_BASE->PllGpu0Bias;
	CCM_REG_BACK.PllVideo0Bias     = CCM_REG_BASE->PllVideo0Bias;
	CCM_REG_BACK.PllVideo1Bias     = CCM_REG_BASE->PllVideo1Bias;
	CCM_REG_BACK.PllVeBias         = CCM_REG_BASE->PllVeBias;
	CCM_REG_BACK.PllDeBias         = CCM_REG_BASE->PllDeBias;
	CCM_REG_BACK.PllAudioBias      = CCM_REG_BASE->PllAudioBias;
	CCM_REG_BACK.Pll24mBias        = CCM_REG_BASE->Pll24mBias;

	//tun
	CCM_REG_BACK.PllCpuxTun        = CCM_REG_BASE->PllCpuxTun;

	//cfg
	CCM_REG_BACK.C0CpuxAxiCfg      = CCM_REG_BASE->C0CpuxAxiCfg;
	CCM_REG_BACK.PsiAhb1Ahb2Cfg    = CCM_REG_BASE->PsiAhb1Ahb2Cfg;
	CCM_REG_BACK.Ahb3Cfg           = CCM_REG_BASE->Ahb3Cfg;
	CCM_REG_BACK.Apb1Cfg           = CCM_REG_BASE->Apb1Cfg;
	CCM_REG_BACK.Apb2Cfg           = CCM_REG_BASE->Apb2Cfg;
//	CCM_REG_BACK.MbusCfg           = CCM_REG_BASE->MbusCfg;

	//accelerator
	CCM_REG_BACK.DeClk             = CCM_REG_BASE->DeClk;
	CCM_REG_BACK.DeBgr             = CCM_REG_BASE->DeBgr;

	CCM_REG_BACK.EinkEngineClk     = CCM_REG_BASE->EinkEngineClk;
	CCM_REG_BACK.EinkBgr           = CCM_REG_BASE->EinkBgr;
	CCM_REG_BACK.EdmaClk           = CCM_REG_BASE->EdmaClk;
	CCM_REG_BACK.G2dClk            = CCM_REG_BASE->G2dClk;
	CCM_REG_BACK.G2dBgr            = CCM_REG_BASE->G2dBgr;
	CCM_REG_BACK.GpuCoreClk        = CCM_REG_BASE->GpuCoreClk;
	CCM_REG_BACK.GpuBgr            = CCM_REG_BASE->GpuBgr;
	CCM_REG_BACK.CeClk             = CCM_REG_BASE->CeClk;
	CCM_REG_BACK.CeBgr             = CCM_REG_BASE->CeBgr;
	CCM_REG_BACK.VeClk             = CCM_REG_BASE->VeClk;
	CCM_REG_BACK.VeBgr             = CCM_REG_BASE->VeBgr;
	CCM_REG_BACK.EmceClk           = CCM_REG_BASE->EmceClk;
	CCM_REG_BACK.EmceBgr           = CCM_REG_BASE->EmceBgr;

	//sys resourc
	CCM_REG_BACK.DmaBgr            = CCM_REG_BASE->DmaBgr;
	CCM_REG_BACK.MsgboxBgr         = CCM_REG_BASE->MsgboxBgr;
	CCM_REG_BACK.SpinlockBgr       = CCM_REG_BASE->SpinlockBgr;
	CCM_REG_BACK.HstimerBgr        = CCM_REG_BASE->HstimerBgr;
	CCM_REG_BACK.AvsClk            = CCM_REG_BASE->AvsClk;
	CCM_REG_BACK.DbgsysBgr         = CCM_REG_BASE->DbgsysBgr;
	CCM_REG_BACK.PsiBgr            = CCM_REG_BASE->PsiBgr;
	CCM_REG_BACK.PwmBgr            = CCM_REG_BASE->PwmBgr;

	//store media
	CCM_REG_BACK.IommuBgr          = CCM_REG_BASE->IommuBgr;
//	CCM_REG_BACK.DramClk           = CCM_REG_BASE->DramClk;
//	CCM_REG_BACK.MbusMatClkGating  = CCM_REG_BASE->MbusMatClkGating;
//	CCM_REG_BACK.DramBgr           = CCM_REG_BASE->DramBgr;
	CCM_REG_BACK.Nand00Clk         = CCM_REG_BASE->Nand00Clk;
	CCM_REG_BACK.Nand01Clk         = CCM_REG_BASE->Nand01Clk;
	CCM_REG_BACK.NandBgr           = CCM_REG_BASE->NandBgr;
	CCM_REG_BACK.Smhc0Clk          = CCM_REG_BASE->Smhc0Clk;
	CCM_REG_BACK.Smhc1Clk          = CCM_REG_BASE->Smhc1Clk;
	CCM_REG_BACK.Smhc2Clk          = CCM_REG_BASE->Smhc2Clk;
	CCM_REG_BACK.SmhcBgr           = CCM_REG_BASE->SmhcBgr;
	CCM_REG_BACK.UartBgr           = CCM_REG_BASE->UartBgr;
	CCM_REG_BACK.TwiBgr            = CCM_REG_BASE->TwiBgr;
	CCM_REG_BACK.Spi0Clk           = CCM_REG_BASE->Spi0Clk;
	CCM_REG_BACK.Spi1Clk           = CCM_REG_BASE->Spi1Clk;
	CCM_REG_BACK.SpiBgr            = CCM_REG_BASE->SpiBgr;
	CCM_REG_BACK.GpadcBgr          = CCM_REG_BASE->GpadcBgr;
	CCM_REG_BACK.ThsBgr            = CCM_REG_BASE->ThsBgr;

	CCM_REG_BACK.I2sPcm0Clk        = CCM_REG_BASE->I2sPcm0Clk;
	CCM_REG_BACK.I2sPcm1Clk        = CCM_REG_BASE->I2sPcm1Clk;
	CCM_REG_BACK.I2sPcmBgr         = CCM_REG_BASE->I2sPcmBgr;
	CCM_REG_BACK.DmicClk           = CCM_REG_BASE->DmicClk;
	CCM_REG_BACK.DmicBgr           = CCM_REG_BASE->DmicBgr;
	CCM_REG_BACK.AudioCodec1xClk   = CCM_REG_BASE->AudioCodec1xClk;
	CCM_REG_BACK.AudioCodec4xClk   = CCM_REG_BASE->AudioCodec4xClk;
	CCM_REG_BACK.AudioCodecBgr     = CCM_REG_BASE->AudioCodecBgr;
	CCM_REG_BACK.Usb0Clk           = CCM_REG_BASE->Usb0Clk;
	CCM_REG_BACK.Usb1Clk           = CCM_REG_BASE->Usb1Clk;
	CCM_REG_BACK.UsbBgr            = CCM_REG_BASE->UsbBgr;

	CCM_REG_BACK.MipiDsiDphy0HsClk = CCM_REG_BASE->MipiDsiDphy0HsClk;
	CCM_REG_BACK.MipiDsiHost0Clk   = CCM_REG_BASE->MipiDsiHost0Clk;
	CCM_REG_BACK.MipiBgr           = CCM_REG_BASE->MipiBgr;
	CCM_REG_BACK.DisplayIfTopBgr   = CCM_REG_BASE->DisplayIfTopBgr;
	CCM_REG_BACK.TconLcd0Clk       = CCM_REG_BASE->TconLcd0Clk;
	CCM_REG_BACK.TconLcdBgr        = CCM_REG_BASE->TconLcdBgr;
	CCM_REG_BACK.LvdsBgr           = CCM_REG_BASE->LvdsBgr;
	CCM_REG_BACK.Csi0MiscClk       = CCM_REG_BASE->Csi0MiscClk;
	CCM_REG_BACK.Csi0TopClk        = CCM_REG_BASE->Csi0TopClk;
	CCM_REG_BACK.Csi00MstClk       = CCM_REG_BASE->Csi00MstClk;
	CCM_REG_BACK.CsiBgr            = CCM_REG_BASE->CsiBgr;
	CCM_REG_BACK.CcmuSecSwitch     = CCM_REG_BASE->CcmuSecSwitch;
	CCM_REG_BACK.PllLockDbgCtrl    = CCM_REG_BASE->PllLockDbgCtrl;
	CCM_REG_BACK.PllCpuxHwFm       = CCM_REG_BASE->PllCpuxHwFm;
	CCM_REG_BACK.ModSpeClk         = CCM_REG_BASE->ModSpeClk;
	CCM_REG_BACK.HoscOutputCtrl    = CCM_REG_BASE->HoscOutputCtrl;
	CCM_REG_BACK.CcmuVersion       = CCM_REG_BASE->CcmuVersion;

	return 0;
}

int mem_ccu_restore(void)
{
	//bias:
/*
	CCM_REG_BASE->PllCpuxBias      = CCM_REG_BACK.PllCpuxBias;
//	CCM_REG_BASE->PllDdrBias       = CCM_REG_BACK.PllDdrBias;
	CCM_REG_BASE->PllPeri0Bias     = CCM_REG_BACK.PllPeri0Bias;
	CCM_REG_BASE->PllPeri1Bias     = CCM_REG_BACK.PllPeri1Bias;
	CCM_REG_BASE->PllGpu0Bias      = CCM_REG_BACK.PllGpu0Bias;
	CCM_REG_BASE->PllVideo0Bias    = CCM_REG_BACK.PllVideo0Bias;
	CCM_REG_BASE->PllVideo1Bias    = CCM_REG_BACK.PllVideo1Bias;
	CCM_REG_BASE->PllVeBias        = CCM_REG_BACK.PllVeBias;
	CCM_REG_BASE->PllDeBias        = CCM_REG_BACK.PllDeBias;
	CCM_REG_BASE->PllAudioBias     = CCM_REG_BACK.PllAudioBias;
	CCM_REG_BASE->Pll24mBias       = CCM_REG_BACK.Pll24mBias;
*/
	mem_restore_pll((unsigned int)(CCM_REG_BACK.PllCpuxCtrl.dwval),(void *)&(CCM_REG_BASE->PllCpuxCtrl));
//	mem_restore_pll((unsigned int)(CCM_REG_BACK.PllDdrCtrl),(void *)&(CCM_REG_BASE->PllDdrCtrl));
	mem_restore_pll((unsigned int)(CCM_REG_BACK.PllPeri0Ctrl),(void *)&(CCM_REG_BASE->PllPeri0Ctrl));
	mem_restore_pll((unsigned int)(CCM_REG_BACK.PllPeri1Ctrl),(void *)&(CCM_REG_BASE->PllPeri1Ctrl));
	mem_restore_pll((unsigned int)(CCM_REG_BACK.PllGpuCtrl),(void *)&(CCM_REG_BASE->PllGpuCtrl));
	mem_restore_pll((unsigned int)(CCM_REG_BACK.PllVideo0Ctrl),(void *)&(CCM_REG_BASE->PllVideo0Ctrl));
	mem_restore_pll((unsigned int)(CCM_REG_BACK.PllVideo1Ctrl),(void *)&(CCM_REG_BASE->PllVideo1Ctrl));
	mem_restore_pll((unsigned int)(CCM_REG_BACK.PllVeCtrl),(void *)&(CCM_REG_BASE->PllVeCtrl));
	mem_restore_pll((unsigned int)(CCM_REG_BACK.PllDeCtrl),(void *)&(CCM_REG_BASE->PllDeCtrl));
	mem_restore_pll((unsigned int)(CCM_REG_BACK.PllAudioCtrl),(void *)&(CCM_REG_BASE->PllAudioCtrl));
	mem_restore_pll((unsigned int)(CCM_REG_BACK.Pll24mCtrl),(void *)&(CCM_REG_BASE->Pll24mCtrl));
	//tune&pattern:

	CCM_REG_BASE->PllCpuxTun       = CCM_REG_BACK.PllCpuxTun;
//	CCM_REG_BASE->PllDdrPatCtrl    = CCM_REG_BACK.PllDdrPatCtrl;
	CCM_REG_BASE->PllPeri0Pat0Ctrl = CCM_REG_BACK.PllPeri0Pat0Ctrl;
	CCM_REG_BASE->PllPeri0Pat1Ctrl = CCM_REG_BACK.PllPeri0Pat1Ctrl;
	CCM_REG_BASE->PllPeri1Pat0Ctrl = CCM_REG_BACK.PllPeri1Pat0Ctrl;
	CCM_REG_BASE->PllPeri1Pat1Ctrl = CCM_REG_BACK.PllPeri1Pat1Ctrl;
	CCM_REG_BASE->PllGpu0Pat0Ctrl  = CCM_REG_BACK.PllGpu0Pat0Ctrl;
	CCM_REG_BASE->PllGpu0Pat1Ctrl  = CCM_REG_BACK.PllGpu0Pat1Ctrl;
	CCM_REG_BASE->PllVideo0Pat0Ctrl = CCM_REG_BACK.PllVideo0Pat0Ctrl;
	CCM_REG_BASE->PllVideo0Pat1Ctrl = CCM_REG_BACK.PllVideo0Pat1Ctrl;
	CCM_REG_BASE->PllVideo1Pat0Ctrl = CCM_REG_BACK.PllVideo1Pat0Ctrl;
	CCM_REG_BASE->PllVideo1Pat1Ctrl = CCM_REG_BACK.PllVideo1Pat1Ctrl;
	CCM_REG_BASE->PllVePat0Ctrl    = CCM_REG_BACK.PllVePat0Ctrl;
	CCM_REG_BASE->PllVePat1Ctrl    = CCM_REG_BACK.PllVePat1Ctrl;
	CCM_REG_BASE->PllDePat0Ctrl    = CCM_REG_BACK.PllDePat0Ctrl;
	CCM_REG_BASE->PllDePat1Ctrl    = CCM_REG_BACK.PllDePat1Ctrl;
	CCM_REG_BASE->PllAudioPat0Ctrl = CCM_REG_BACK.PllAudioPat0Ctrl;
	CCM_REG_BASE->PllAudioPat1Ctrl = CCM_REG_BACK.PllAudioPat1Ctrl;

	//ctlr:
	CCM_REG_BASE->PllCpuxBias      = CCM_REG_BACK.PllCpuxBias;
//	CCM_REG_BASE->PllDdrBias       = CCM_REG_BACK.PllDdrBias;
	CCM_REG_BASE->PllPeri0Bias     = CCM_REG_BACK.PllPeri0Bias;
	CCM_REG_BASE->PllPeri1Bias     = CCM_REG_BACK.PllPeri1Bias;
	CCM_REG_BASE->PllGpu0Bias      = CCM_REG_BACK.PllGpu0Bias;
	CCM_REG_BASE->PllVideo0Bias    = CCM_REG_BACK.PllVideo0Bias;
	CCM_REG_BASE->PllVideo1Bias    = CCM_REG_BACK.PllVideo1Bias;
	CCM_REG_BASE->PllVeBias        = CCM_REG_BACK.PllVeBias;
	CCM_REG_BASE->PllDeBias        = CCM_REG_BACK.PllDeBias;
	CCM_REG_BASE->PllAudioBias     = CCM_REG_BACK.PllAudioBias;
	CCM_REG_BASE->Pll24mBias       = CCM_REG_BACK.Pll24mBias;

			//&& ((((u32)CCM_REG_BASE->PllGpuCtl >> 28) & 0x1) == 1) 
	__time_udelay(2);
/*
	//notice: be care to cpu, axi restore sequence!.
	value = (u32)CCM_REG_BASE->C0CpuxAxiCfg.dwval;
	value &= ~0x3;
	value |= (CCM_REG_BACK.C0CpuxAxiCfg.dwval & 0x3);
	CCM_REG_BASE->C0CpuxAxiCfg.dwval = value;
	mem_long_jump((mem_long_jump_fn)time_udelay, 2);
	value = CCM_REG_BASE->C0CpuxAxiCfg.dwval;
	value &= ~(0x3 << 24);
	value |= (CCM_REG_BACK.C0CpuxAxiCfg.dwval & (0x0 << 24));
	CCM_REG_BASE->C0CpuxAxiCfg.dwval = value;
	mem_long_jump((mem_long_jump_fn)time_udelay, 2);
	value &= ~(0x3 << 8);
	value |= (CCM_REG_BACK.C0CpuxAxiCfg.dwval & (0x3 << 8));
	CCM_REG_BASE->C0CpuxAxiCfg.dwval = value;
	mem_long_jump((mem_long_jump_fn)time_udelay, 2);

	//set ahb1 and apb1
	//increase freq, set div first
	value = CCM_REG_BASE->PsiAhb1Ahb2Cfg.dwval;
	value &= ~0x3ff;
	value |= (CCM_REG_BACK.PsiAhb1Ahb2Cfg.dwval & 0x3ff);
	CCM_REG_BASE->PsiAhb1Ahb2Cfg.dwval = value;
	mem_long_jump((mem_long_jump_fn)time_udelay, 2);
	//set ahb1 src
	value &= ~(0x3 << 24);
	value |=  CCM_REG_BACK.PsiAhb1Ahb2Cfg.dwval & (0x0 << 24);
	CCM_REG_BASE->PsiAhb1Ahb2Cfg.dwval = value;
	mem_long_jump((mem_long_jump_fn)time_udelay, 2);

	//apb1
	value = CCM_REG_BASE->Apb1Cfg.dwval;
	value &= ~0x3ff;
	value |= (CCM_REG_BACK.Apb1Cfg.dwval & 0x3ff);
	CCM_REG_BASE->Apb1Cfg.dwval = value;
	mem_long_jump((mem_long_jump_fn)time_udelay, 2);
	//set apb1 src
	value &= ~(0x3 << 24);
	value |=  CCM_REG_BACK.Apb1Cfg.dwval & (0x3 << 24);
	CCM_REG_BASE->Apb1Cfg.dwval = value;
	mem_long_jump((mem_long_jump_fn)time_udelay, 2);


	//set apb2 div
	value = CCM_REG_BASE->Apb2Cfg.dwval;
	value &=~0x3ff;
	value |= CCM_REG_BACK.Apb2Cfg.dwval & 0x3ff;
	CCM_REG_BASE->Apb2Cfg.dwval = value;
	mem_long_jump((mem_long_jump_fn)time_udelay, 2);
	value &= ~(0x3 << 24);
	value |= CCM_REG_BACK.Apb2Cfg.dwval & (0x3 << 24);
	CCM_REG_BASE->Apb2Cfg.dwval = value;
	mem_long_jump((mem_long_jump_fn)time_udelay, 2);

	//ahb3
	value = CCM_REG_BASE->Ahb3Cfg.dwval;
	value &=~0x3ff;
	value |= CCM_REG_BACK.Ahb3Cfg.dwval & 0x3ff;
	CCM_REG_BASE->Ahb3Cfg.dwval = value;
	mem_long_jump((mem_long_jump_fn)time_udelay, 2);
	value &= ~(0x3 << 24);
	value |= CCM_REG_BACK.Ahb3Cfg.dwval & (0x0 << 24);
	CCM_REG_BASE->Ahb3Cfg.dwval = value;
	mem_long_jump((mem_long_jump_fn)time_udelay, 2);
*/
	//CCM_REG_BASE->SysClkDiv		= CCM_REG_BACK.SysClkDiv;
	//CCM_REG_BASE->Ahb1Div			= CCM_REG_BACK.Ahb1Div;
	//mem_long_jump((mem_long_jump_fn)time_udelay, 1);
	//CCM_REG_BASE->Apb2Div			= CCM_REG_BACK.Apb2Div;

	__time_udelay(10);
	CCM_REG_BASE->DeClk            = CCM_REG_BACK.DeClk;
	CCM_REG_BASE->DeBgr            = CCM_REG_BACK.DeBgr;
	CCM_REG_BASE->EinkEngineClk    = CCM_REG_BACK.EinkEngineClk;
	CCM_REG_BASE->EinkBgr          = CCM_REG_BACK.EinkBgr;
	CCM_REG_BASE->EdmaClk          = CCM_REG_BACK.EdmaClk;
	CCM_REG_BASE->G2dClk           = CCM_REG_BACK.G2dClk;
	CCM_REG_BASE->G2dBgr           = CCM_REG_BACK.G2dBgr;
	CCM_REG_BASE->GpuCoreClk       = CCM_REG_BACK.GpuCoreClk;
	CCM_REG_BASE->GpuBgr           = CCM_REG_BACK.GpuBgr;
	CCM_REG_BASE->CeClk            = CCM_REG_BACK.CeClk;
	CCM_REG_BASE->CeBgr            = CCM_REG_BACK.CeBgr;
	CCM_REG_BASE->VeClk            = CCM_REG_BACK.VeClk;
	CCM_REG_BASE->VeBgr            = CCM_REG_BACK.VeBgr;
	CCM_REG_BASE->EmceClk          = CCM_REG_BACK.EmceClk;
	CCM_REG_BASE->EmceBgr          = CCM_REG_BACK.EmceBgr;
	CCM_REG_BASE->DmaBgr           = CCM_REG_BACK.DmaBgr;
	CCM_REG_BASE->MsgboxBgr        = CCM_REG_BACK.MsgboxBgr;
	CCM_REG_BASE->SpinlockBgr      = CCM_REG_BACK.SpinlockBgr;
	CCM_REG_BASE->HstimerBgr       = CCM_REG_BACK.HstimerBgr;
	CCM_REG_BASE->AvsClk           = CCM_REG_BACK.AvsClk;
	CCM_REG_BASE->DbgsysBgr        = CCM_REG_BACK.DbgsysBgr;
	CCM_REG_BASE->PsiBgr           = CCM_REG_BACK.PsiBgr;
	CCM_REG_BASE->PwmBgr           = CCM_REG_BACK.PwmBgr;

	//store                                   media
	CCM_REG_BASE->IommuBgr         = CCM_REG_BACK.IommuBgr;
//	CCM_REG_BASE->DramClk          = CCM_REG_BACK.DramClk;
//	CCM_REG_BASE->MbusMatClkGating = CCM_REG_BACK.MbusMatClkGating;
//	CCM_REG_BASE->DramBgr          = CCM_REG_BACK.DramBgr;
	CCM_REG_BASE->Nand00Clk        = CCM_REG_BACK.Nand00Clk;
	CCM_REG_BASE->Nand01Clk        = CCM_REG_BACK.Nand01Clk;
	CCM_REG_BASE->NandBgr          = CCM_REG_BACK.NandBgr;
	CCM_REG_BASE->Smhc0Clk         = CCM_REG_BACK.Smhc0Clk;
	CCM_REG_BASE->Smhc1Clk         = CCM_REG_BACK.Smhc1Clk;
	CCM_REG_BASE->Smhc2Clk         = CCM_REG_BACK.Smhc2Clk;
	CCM_REG_BASE->SmhcBgr          = CCM_REG_BACK.SmhcBgr;
	CCM_REG_BASE->UartBgr          = CCM_REG_BACK.UartBgr;
	CCM_REG_BASE->TwiBgr           = CCM_REG_BACK.TwiBgr;
	CCM_REG_BASE->Spi0Clk          = CCM_REG_BACK.Spi0Clk;
	CCM_REG_BASE->Spi1Clk          = CCM_REG_BACK.Spi1Clk;
	CCM_REG_BASE->SpiBgr           = CCM_REG_BACK.SpiBgr;
	CCM_REG_BASE->GpadcBgr         = CCM_REG_BACK.GpadcBgr;
	CCM_REG_BASE->ThsBgr           = CCM_REG_BACK.ThsBgr;
	CCM_REG_BASE->I2sPcm0Clk       = CCM_REG_BACK.I2sPcm0Clk;
	CCM_REG_BASE->I2sPcm1Clk       = CCM_REG_BACK.I2sPcm1Clk;
	CCM_REG_BASE->I2sPcmBgr        = CCM_REG_BACK.I2sPcmBgr;
	CCM_REG_BASE->DmicClk          = CCM_REG_BACK.DmicClk;
	CCM_REG_BASE->DmicBgr          = CCM_REG_BACK.DmicBgr;
	CCM_REG_BASE->AudioCodec1xClk  = CCM_REG_BACK.AudioCodec1xClk;
	CCM_REG_BASE->AudioCodec4xClk  = CCM_REG_BACK.AudioCodec4xClk;
	CCM_REG_BASE->AudioCodecBgr    = CCM_REG_BACK.AudioCodecBgr;
	CCM_REG_BASE->Usb0Clk          = CCM_REG_BACK.Usb0Clk;
	CCM_REG_BASE->Usb1Clk          = CCM_REG_BACK.Usb1Clk;
	CCM_REG_BASE->UsbBgr           = CCM_REG_BACK.UsbBgr;
	CCM_REG_BASE->MipiDsiDphy0HsClk = CCM_REG_BACK.MipiDsiDphy0HsClk;
	CCM_REG_BASE->MipiDsiHost0Clk  = CCM_REG_BACK.MipiDsiHost0Clk;
	CCM_REG_BASE->MipiBgr          = CCM_REG_BACK.MipiBgr;
	CCM_REG_BASE->DisplayIfTopBgr  = CCM_REG_BACK.DisplayIfTopBgr;
	CCM_REG_BASE->TconLcd0Clk      = CCM_REG_BACK.TconLcd0Clk;
	CCM_REG_BASE->TconLcdBgr       = CCM_REG_BACK.TconLcdBgr;
	CCM_REG_BASE->LvdsBgr          = CCM_REG_BACK.LvdsBgr;
	CCM_REG_BASE->Csi0MiscClk      = CCM_REG_BACK.Csi0MiscClk;
	CCM_REG_BASE->Csi0TopClk       = CCM_REG_BACK.Csi0TopClk;
	CCM_REG_BASE->Csi00MstClk      = CCM_REG_BACK.Csi00MstClk;
	CCM_REG_BASE->CsiBgr           = CCM_REG_BACK.CsiBgr;
	CCM_REG_BASE->CcmuSecSwitch    = CCM_REG_BACK.CcmuSecSwitch;
//	CCM_REG_BASE->PllLockDbgCtrl   = CCM_REG_BACK.PllLockDbgCtrl;
	CCM_REG_BASE->PllCpuxHwFm      = CCM_REG_BACK.PllCpuxHwFm;
	CCM_REG_BASE->ModSpeClk        = CCM_REG_BACK.ModSpeClk;
	CCM_REG_BASE->HoscOutputCtrl   = CCM_REG_BACK.HoscOutputCtrl;

	__time_udelay(10);

	CCM_REG_BASE->PllLockDbgCtrl	= CCM_REG_BACK.PllLockDbgCtrl;
	CCM_REG_BASE->CcmuVersion	= CCM_REG_BACK.CcmuVersion;

	__time_udelay(1);

	return 0;
}

void cpux_ccmu_restore(void)
{
	volatile u32 value;

	value = (u32)CCM_REG_BASE->C0CpuxAxiCfg.dwval;
	value &= ~0x3;
	value |= (CCM_REG_BACK.C0CpuxAxiCfg.dwval & 0x3);
	CCM_REG_BASE->C0CpuxAxiCfg.dwval = value;
	__time_udelay(2);
	value = CCM_REG_BASE->C0CpuxAxiCfg.dwval;
	value &= ~(0x7 << 24);
	value |= (CCM_REG_BACK.C0CpuxAxiCfg.dwval & (0x7 << 24));
	CCM_REG_BASE->C0CpuxAxiCfg.dwval = value;
	__time_udelay(2);
	value &= ~(0x3 << 8);
	value |= (CCM_REG_BACK.C0CpuxAxiCfg.dwval & (0x3 << 8));
	CCM_REG_BASE->C0CpuxAxiCfg.dwval = value;
	__time_udelay(2);

	//set ahb1 and apb1
	//increase freq, set div first
	value = CCM_REG_BASE->PsiAhb1Ahb2Cfg.dwval;
	value &= ~0x3ff;
	value |= (CCM_REG_BACK.PsiAhb1Ahb2Cfg.dwval & 0x3ff);
	CCM_REG_BASE->PsiAhb1Ahb2Cfg.dwval = value;
	__time_udelay(2);
	//set ahb1 src
	value &= ~(0x3 << 24);
	value |=  CCM_REG_BACK.PsiAhb1Ahb2Cfg.dwval & (0x3 << 24);
	CCM_REG_BASE->PsiAhb1Ahb2Cfg.dwval = value;
	__time_udelay(2);

	//apb1
	value = CCM_REG_BASE->Apb1Cfg.dwval;
	value &= ~0x3ff;
	value |= (CCM_REG_BACK.Apb1Cfg.dwval & 0x3ff);
	CCM_REG_BASE->Apb1Cfg.dwval = value;
	__time_udelay(2);
	//set apb1 src
	value &= ~(0x3 << 24);
	value |=  CCM_REG_BACK.Apb1Cfg.dwval & (0x3 << 24);
	CCM_REG_BASE->Apb1Cfg.dwval = value;
	__time_udelay(2);


	//set apb2 div
	value = CCM_REG_BASE->Apb2Cfg.dwval;
	value &=~0x3ff;
	value |= CCM_REG_BACK.Apb2Cfg.dwval & 0x3ff;
	CCM_REG_BASE->Apb2Cfg.dwval = value;
	__time_udelay(2);
	value &= ~(0x3 << 24);
	value |= CCM_REG_BACK.Apb2Cfg.dwval & (0x3 << 24);
	CCM_REG_BASE->Apb2Cfg.dwval = value;
	__time_udelay(2);

	//ahb3
	value = CCM_REG_BASE->Ahb3Cfg.dwval;
	value &=~0x3ff;
	value |= CCM_REG_BACK.Ahb3Cfg.dwval & 0x3ff;
	CCM_REG_BASE->Ahb3Cfg.dwval = value;
	__time_udelay(2);
	value &= ~(0x3 << 24);
	value |= CCM_REG_BACK.Ahb3Cfg.dwval & (0x3 << 24);
	CCM_REG_BASE->Ahb3Cfg.dwval = value;
	__time_udelay(2);
}

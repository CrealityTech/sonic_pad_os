#ifndef __MEM_CCU_H__
#define __MEM_CCU_H__


#define CCM_REG_LENGTH		((0xFF0+0x4)>>2)

typedef union{
	unsigned int dwval;
	struct
	{
		u32 enable:1;		//bit31, 0-disable, 1-enable, (24Mhz*N*K)/(M)
		u32 reserved0:1;	//bit30, reserved
		u32 lock_en:1;		//bit29, 0-disable lock, 1-enable lock
		u32 lock_st:1;		//bit28, 0-unlocked, 1-locked(PLL has been stable)
		u32 reserved1:1;	//bit27, reserved
		u32 lock_time:3;	//bit24, lock time:freq scaling step
		u32 pll_input_sel:1; //bit23, 0-PLL_24M, 1-HOSC
		u32 reserved2:5;	//bit18, reserved
		u32 pll_out_ext_divp:2; //bit16, PLL Output external divider P
		u32 factor_n:8;		//bit8,  PLL1 Factor_N
		u32 reserved3:6;	//bit2,  reserved
		u32 factor_m:2;		//bit0,  PLL1 Factor_M
	} bits;
} __ccmu_pll_cpux_reg0000_t;

#define AC327_CLKSRC_LOSC   (0)
#define AC327_CLKSRC_HOSC   (1)
#define AC327_CLKSRC_RC		(2)
#define AC327_CLKSRC_PLL1   (3)

typedef union{
	unsigned int dwval;
	struct
	{
		unsigned int   reserved2:6;       //bit26,  reserved
		unsigned int   CpuClkSrc:2;       //bit24, AXI1 clock divide ratio, 000-1, 001-2, 010-3, 011-4
		unsigned int   reserved1:14;        //bit10, reserved
		unsigned int   CpuApbFactorN:2;     //bit8, CPU0/1/2/3 clock source select, 0-HOSC, 1-PLL_C0CPUX
		unsigned int   reserved0:6;        //bit2,  reserved
		unsigned int   FactorM:2;        //bit0,  AXI0 clock divide ratio, 000-1, 001-2, 010-3, 011-4
	} bits;
}__ccmu_sysclk_ratio_reg0500_t;

#define AHB1_CLKSRC_LOSC    (0)
#define AHB1_CLKSRC_HOSC    (1)
#define AHB1_CLKSRC_RC     (2)
#define AHB1_CLKSRC_PLL6    (3)
typedef union{
	unsigned int dwval;
	struct
	{
		unsigned int   reserved2:6;      //bit26, reserved
		unsigned int   ClkSrcSel:2;      //bit24, ahb1 ahb2 clock source select, 00-LOSC, 01-OSC24M, 10/11-PLL6/ahb1_pre_div
		unsigned int   reserved1:14;     //bit8,  reserved
		unsigned int   FactorN:2;        //bit6,  pll facotrN
		unsigned int   reserved0:6;      //bit4,  reserved
		unsigned int   FactorM:2;        //bit0,  pll facotrM,tht range is from 1 to 4,M = FactorM+1.
	} bits;
}__ccmu_ahb1_ahb2_ratio_reg0510_t;

typedef union{
	unsigned int dwval;
	struct
	{
		unsigned int   reserved2:6;        //bit26, reserved
		unsigned int   ClkSrc:2;           //bit24, clock source select, 00-LOSC, 01-OSC24M, 10/11-PLL6
		unsigned int   reserved1:14;       //bit10, reserved
		unsigned int   FactorN:2;          //bit8,  pll facotrN
		unsigned int   reserved:6;         //bit2,  reserved
		unsigned int   FactortM:2;         //bit0,  pll facotrM,tht range is from 1 to 4,M = FactorM+1.
	} bits;
} __ccmu_bus_ratio_reg_t;

#define APB2_CLKSRC_LOSC    (0)
#define APB2_CLKSRC_HOSC    (1)
#define APB2_CLKSRC_PSI		(2)
#define APB2_CLKSRC_PLL6    (3)
#define APB1_CLKSRC_LOSC    (0)
#define APB1_CLKSRC_HOSC    (1)
#define APB1_CLKSRC_PSI		(2)
#define APB1_CLKSRC_PLL6    (3)
#define AHB3_CLKSRC_LOSC    (0)
#define AHB3_CLKSRC_HOSC    (1)
#define AHB3_CLKSRC_PSI		(2)
#define AHB3_CLKSRC_PLL6    (3)
#define	__ccmu_apb1_ratio_reg0520_t __ccmu_bus_ratio_reg_t
#define	__ccmu_apb2_ratio_reg0524_t __ccmu_bus_ratio_reg_t
#define	__ccmu_ahb3_ratio_reg051c_t __ccmu_bus_ratio_reg_t

/*
typedef struct __CCMU_PLLLOCK_REG0200
{
	unsigned int   reserved:16;        //bit16, reserved
	unsigned int   LockTime:16;        //bit0,  PLL lock time, based on us
} __ccmu_plllock_reg0200_t;
*/

typedef struct __CCMU_REG_LIST
{
	volatile __ccmu_pll_cpux_reg0000_t				PllCpuxCtrl;               //0x0000, pll cpu0
	volatile unsigned int						reserved4[3];
	volatile unsigned int						PllDdrCtrl;
	volatile unsigned int						reserved14[3];
	volatile unsigned int						PllPeri0Ctrl;
	volatile unsigned int						reserved24;
	volatile unsigned int						PllPeri1Ctrl;
	volatile unsigned int						reserved2c;
	volatile unsigned int						PllGpuCtrl;
	volatile unsigned int						reserved34[3];
	volatile unsigned int						PllVideo0Ctrl;
	volatile unsigned int						reserved44;
	volatile unsigned int						PllVideo1Ctrl;
	volatile unsigned int						reserved4c[3];
	volatile unsigned int						PllVeCtrl;
	volatile unsigned int						reserved5c;
	volatile unsigned int						PllDeCtrl;
	volatile unsigned int						reserved64[5];
	volatile unsigned int						PllAudioCtrl;
	volatile unsigned int						reserved7c[15];
	volatile unsigned int						Pll24mCtrl;
	volatile unsigned int						reservedbc[21];
	volatile unsigned int						PllDdrPatCtrl;
	volatile unsigned int						reserved114[3];
	volatile unsigned int						PllPeri0Pat0Ctrl;
	volatile unsigned int						PllPeri0Pat1Ctrl;
	volatile unsigned int						PllPeri1Pat0Ctrl;
	volatile unsigned int						PllPeri1Pat1Ctrl;
	volatile unsigned int						PllGpu0Pat0Ctrl;
	volatile unsigned int						PllGpu0Pat1Ctrl;
	volatile unsigned int						reserved138[2];
	volatile unsigned int						PllVideo0Pat0Ctrl;
	volatile unsigned int						PllVideo0Pat1Ctrl;
	volatile unsigned int						PllVideo1Pat0Ctrl;
	volatile unsigned int						PllVideo1Pat1Ctrl;
	volatile unsigned int						reserved150[2];
	volatile unsigned int						PllVePat0Ctrl;
	volatile unsigned int						PllVePat1Ctrl;
	volatile unsigned int						PllDePat0Ctrl;
	volatile unsigned int						PllDePat1Ctrl;
	volatile unsigned int						reserved168[4];
	volatile unsigned int						PllAudioPat0Ctrl;
	volatile unsigned int						PllAudioPat1Ctrl;
	volatile unsigned int						reserved180[96];
	volatile unsigned int						PllCpuxBias;
	volatile unsigned int						reserved304[3];
	volatile unsigned int						PllDdrBias;
	volatile unsigned int						reserved314[3];
	volatile unsigned int						PllPeri0Bias;
	volatile unsigned int						reserved324;
	volatile unsigned int						PllPeri1Bias;
	volatile unsigned int						reserved32c;
	volatile unsigned int						PllGpu0Bias;
	volatile unsigned int						reserved334[3];
	volatile unsigned int						PllVideo0Bias;
	volatile unsigned int						reserved344;
	volatile unsigned int						PllVideo1Bias;
	volatile unsigned int						reserved34c[3];
	volatile unsigned int						PllVeBias;
	volatile unsigned int						reserved35c;
	volatile unsigned int						PllDeBias;
	volatile unsigned int						reserved364[5];
	volatile unsigned int						PllAudioBias;
	volatile unsigned int						reserved37c[15];
	volatile unsigned int						Pll24mBias;
	volatile unsigned int						reserved3bc[17];
	volatile unsigned int						PllCpuxTun;
	volatile unsigned int						reserved404[63];
	volatile __ccmu_sysclk_ratio_reg0500_t				C0CpuxAxiCfg;
	volatile unsigned int						reserved504[3];
	volatile __ccmu_ahb1_ahb2_ratio_reg0510_t			PsiAhb1Ahb2Cfg;
	volatile unsigned int						reserved514[2];
	volatile __ccmu_ahb3_ratio_reg051c_t				Ahb3Cfg;
	volatile __ccmu_apb1_ratio_reg0520_t				Apb1Cfg;
	volatile __ccmu_apb2_ratio_reg0524_t				Apb2Cfg;
	volatile unsigned int 						reserved528[6];
	volatile unsigned int 						MbusCfg;
	volatile unsigned int 						reserved544[47];
	volatile unsigned int 						DeClk;
	volatile unsigned int 						reserved604[2];
	volatile unsigned int 						DeBgr;
	volatile unsigned int 						EinkEngineClk;
	volatile unsigned int 						reserved614[2];
	volatile unsigned int 						EinkBgr;
	volatile unsigned int 						reserved620[4];
	volatile unsigned int 						G2dClk;
	volatile unsigned int 						reserved634[2];
	volatile unsigned int 						G2dBgr;
	volatile unsigned int 						EdmaClk;
	volatile unsigned int 						reserved644[11];
	volatile unsigned int 						GpuCoreClk;
	volatile unsigned int 						reserved674[2];
	volatile unsigned int 						GpuBgr;
	volatile unsigned int 						CeClk;
	volatile unsigned int 						reserved684[2];
	volatile unsigned int 						CeBgr;
	volatile unsigned int 						VeClk;
	volatile unsigned int 						reserved694[2];
	volatile unsigned int 						VeBgr;
	volatile unsigned int 						reserved6a0[4];
	volatile unsigned int 						EmceClk;
	volatile unsigned int 						reserved6b4[2];
	volatile unsigned int 						EmceBgr;
	volatile unsigned int 						reserved6c0[19];
	volatile unsigned int 						DmaBgr;
	volatile unsigned int 						reserved710[3];
	volatile unsigned int 						MsgboxBgr;
	volatile unsigned int 						reserved720[3];
	volatile unsigned int 						SpinlockBgr;
	volatile unsigned int 						reserved730[3];
	volatile unsigned int 						HstimerBgr;
	volatile unsigned int 						AvsClk;
	volatile unsigned int 						reserved744[18];
	volatile unsigned int 						DbgsysBgr;
	volatile unsigned int 						reserved790[3];
	volatile unsigned int 						PsiBgr;
	volatile unsigned int 						reserved7a0[3];
	volatile unsigned int 						PwmBgr;
	volatile unsigned int 						reserved7b0[3];
	volatile unsigned int 						IommuBgr;
	volatile unsigned int 						reserved7c0[16];
	volatile unsigned int 						DramClk;
	volatile unsigned int 						MbusMatClkGating;
	volatile unsigned int 						reserved808;
	volatile unsigned int 						DramBgr;
	volatile unsigned int 						Nand00Clk;
	volatile unsigned int 						Nand01Clk;
	volatile unsigned int 						reserved818[5];
	volatile unsigned int 						NandBgr;
	volatile unsigned int 						Smhc0Clk;
	volatile unsigned int 						Smhc1Clk;
	volatile unsigned int 						Smhc2Clk;
	volatile unsigned int 						reserved83c[4];
	volatile unsigned int 						SmhcBgr;
	volatile unsigned int 						reserved850[47];
	volatile unsigned int 						UartBgr;
	volatile unsigned int 						reserved910[3];
	volatile unsigned int 						TwiBgr;
	volatile unsigned int 						reserved920[8];
	volatile unsigned int 						Spi0Clk;
	volatile unsigned int 						Spi1Clk;
	volatile unsigned int 						reserved948[9];
	volatile unsigned int 						SpiBgr;
	volatile unsigned int 						reserved970[31];
	volatile unsigned int 						GpadcBgr;
	volatile unsigned int 						reserved9f0[3];
	volatile unsigned int 						ThsBgr;
	volatile unsigned int 						reserveda00[4];
	volatile unsigned int 						I2sPcm0Clk;
	volatile unsigned int 						I2sPcm1Clk;
	volatile unsigned int 						reserveda18;
	volatile unsigned int 						I2sPcmBgr;
	volatile unsigned int 						reserveda20[8];
	volatile unsigned int 						DmicClk;
	volatile unsigned int 						reserveda44[2];
	volatile unsigned int 						DmicBgr;
	volatile unsigned int 						AudioCodec1xClk;
	volatile unsigned int 						AudioCodec4xClk;
	volatile unsigned int 						reserveda58;
	volatile unsigned int 						AudioCodecBgr;
	volatile unsigned int 						reserveda60[4];
	volatile unsigned int 						Usb0Clk;
	volatile unsigned int 						Usb1Clk;
	volatile unsigned int 						reserveda78[5];
	volatile unsigned int 						UsbBgr;
	volatile unsigned int 						reserveda90[36];
	volatile unsigned int 						MipiDsiDphy0HsClk;
	volatile unsigned int 						MipiDsiHost0Clk;
	volatile unsigned int 						reservedb28[9];
	volatile unsigned int 						MipiBgr;
	volatile unsigned int 						reservedb50[3];
	volatile unsigned int 						DisplayIfTopBgr;
	volatile unsigned int 						TconLcd0Clk;
	volatile unsigned int 						reservedb64[6];
	volatile unsigned int 						TconLcdBgr;
	volatile unsigned int 						reservedb80[11];
	volatile unsigned int 						LvdsBgr;
	volatile unsigned int 						reservedbb0[20];
	volatile unsigned int 						Csi0MiscClk;
	volatile unsigned int 						Csi0TopClk;
	volatile unsigned int 						Csi00MstClk;
	volatile unsigned int 						reservedc0c[8];
	volatile unsigned int 						CsiBgr;
	volatile unsigned int 						reservedc30[180];
	volatile unsigned int 						CcmuSecSwitch;
	volatile unsigned int 						PllLockDbgCtrl;
	volatile unsigned int 						reservedf08[6];
	volatile unsigned int 						PllCpuxHwFm;
	volatile unsigned int 						reservedf24[3];
	volatile unsigned int 						ModSpeClk;
	volatile unsigned int 						reservedf34[3];
	volatile unsigned int 						HoscOutputCtrl;
	volatile unsigned int 						reservedf44[43];
	volatile unsigned int 						CcmuVersion;
} __ccmu_reg_list_t;

struct ccm_state{
	__ccmu_reg_list_t *ccm_reg_base;
	__ccmu_reg_list_t ccm_reg_back;

};


int mem_ccu_save(void);
int mem_ccu_restore(void);
void cpux_ccmu_restore(void);

#endif


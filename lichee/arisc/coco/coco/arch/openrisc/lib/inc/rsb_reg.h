#ifndef __RSB_REG_H__
#define __RSB_REG_H__

#define R_RSB_REG_BASE		(0x07083000)
#define RSB_REG_BASE		R_RSB_REG_BASE

#define RSB_CTRL		(0x00)
#define	RSB_CCR			(0x04)
#define	RSB_INTE		(0x08)
#define	RSB_STAT		(0x0c)
#define	RSB_AR			(0x10)
#define	RSB_DATA		(0x1c)
#define	RSB_LCR			(0x24)
#define	RSB_DMCR		(0x28)
#define	RSB_CMD			(0x2c)
#define	RSB_DAR			(0x30)
#define	RSB_ARBITER		(0x34)
#define	RSB_VER			(0x300)

typedef union {
	u32 reg;
	struct {
		u32 reserved1:24;
		u32 start_trans:1;
		u32 abort_trans:1;
		u32 reserved0:4;
		u32 global_int_enb:1;
		u32 soft_reset:1;
	}bits;
}rsb_ctrl_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved0:21;
		u32 cd_odly:3;
		u32 ck_div:8;
	}bits;
}rsb_ccr_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved0:29;
		u32 load_bsy_enb:1;
		u32 trans_err_enb:1;
		u32 trans_over_enb:1;
	}bits;
}rsb_inte_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved2:15;
		u32 trans_err_ack:1;
		u32 reserved1:4;
		u32 trans_err_data:4;
		u32 reserved0:5;
		u32 load_bsy:1;
		u32 trans_err:1;
		u32 trans_over:1;
	}bits;
}rsb_stat_t;

typedef struct {
	u32 reg;
}rsb_ar_t;

typedef struct {
	u32 reg;
}rsb_data_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved0:26;
		u32 ck_state:1;
		u32 cd_state:1;
		u32 ck_ctl:1;
		u32 ck_ctl_en:1;
		u32 cd_ctl:1;
		u32 cd_ctl_en:1;
	}bits;
}rsb_lcr_t;

typedef union {
	u32 reg;
	struct {
		u32 device_mode_start:1;
		u32 reserved0:7;
		u32 device_mode_data:24;
	}bits;
}rsb_dmcr_t;

typedef struct {
	u32 reg;
}rsb_cmd_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved0:8;
		u32 rta:8;
		u32 da:16;
	}bits;
}rsb_dar_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved0:29;
		u32 sh_priority:1;
		u32 sw_busy_set:1;
		u32 hw_busy_set:1;
	}bits;
}rsb_arbiter_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved1:8;
		u32 ver_small:8;
		u32 reserved0:8;
		u32 ver_big:8;
	}bits;
}rsb_ver_t;

typedef volatile struct module_rsb {
	rsb_ctrl_t		*control;
	rsb_ccr_t		*clk_ctrl;
	rsb_inte_t		*irq_enable;
	rsb_stat_t		*status;
	rsb_ar_t		*address;
	rsb_data_t		*data;
	rsb_lcr_t		*line_ctrl;
	rsb_dmcr_t		*dev_mode_ctrl;
	rsb_cmd_t		*command;
	rsb_dar_t		*dev_address;
	rsb_arbiter_t	*arbitration;
	rsb_ver_t		*version;
	__pCBK_t				phandler[8];
	void					*parg[8];
} RSB_TypedDef;

typedef struct R_RSB {
	RSB_TypedDef	RSB;
	u32		irq_no;
	__pISR_t	phandler;
} R_RSB;



#endif



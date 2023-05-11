/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                 rsb module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : rsb.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-18
* Descript: rsb controller public header.
* Update  : date                auther      ver     notes
*           2012-5-18 9:55:00   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __RSB_H__
#define __RSB_H__

#define IOCTL_RSB_READ		0x001
#define IOCTL_RSB_WRITE		0x002


typedef struct {
	arisc_device dev;
	arisc_driver *dri;
	const struct rsb_ops *ops;
	RSB_TypedDef RSBx;
	pin_typedef *rsb_sck;
	pin_typedef *rsb_sda;
	u32 rsb_rate;
	s32 (*clkchangecb)(u32 command, void *parg);
}rsb_device;

/* rsb transfer data type */
typedef enum rsb_datatype {
	RSB_DATA_TYPE_BYTE  = 1,
	RSB_DATA_TYPE_HWORD = 2,
	RSB_DATA_TYPE_WORD  = 4
} rsb_datatype_e;

typedef struct rsb_paras {
	u32 len;
	u32 datatype;
	u32 msgattr;
	u32 devaddr;
	u8 *regaddr;
	u32 *data;
} rsb_paras_t;


/*
*********************************************************************************************************
*                                       INITIALIZE RSB
*
* Description:  initialize rsb module.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize rsb succeeded, others if failed.
*********************************************************************************************************
*/
void rsb_init(void);


/*
*********************************************************************************************************
*                                       EXIT RSB
*
* Description:  exit rsb module.
*
* Arguments  :  none.
*
* Returns    :  OK if exit rsb succeeded, others if failed.
*********************************************************************************************************
*/
s32 rsb_exit(rsb_device *dev);


/*
*********************************************************************************************************
*                                           READ DATA BY RSB
*
* Description:  read data by rsb.
*
* Arguments  :  devaddr : devices address, size is specified by len.
*               regaddr : registers address, size is specified by len.
*               data    : the pointer of data, size is specified by datatype.
*               datatype: the length of data.
*
* Returns    :  OK if read data succeeded, others if failed.
*********************************************************************************************************
*/
s32 rsb_read_block(rsb_device *dev, rsb_paras_t *paras);


/*
*********************************************************************************************************
*                                           WRITE DATA BY RSB
*
* Description:  write data by rsb.
*
* Arguments  :  devaddr : devices address, size is specified by len.
*               regaddr : registers address, size is specified by len.
*               data    : the pointer of data, size is specified by len.
*               datatype: the length of data.
*
* Returns    :  OK if write data succeeded, others if failed.
*********************************************************************************************************
*/
s32 rsb_write_block(rsb_device *dev, rsb_paras_t *paras);


rsb_device *query_rsb(void);
s32 rsb_set_run_time_addr(rsb_device *dev, u32 saddr ,u32 rtsaddr);
void rsb_hw_init(rsb_device *dev, RSB_InitConfig *config);
void rsb_pin_init(rsb_device *dev);
s32 rsb_set_rate(rsb_device *dev, u32 rate);
s32 rsb_set_lock(rsb_device *dev,u32 lock);

#endif  //__RSB_H__

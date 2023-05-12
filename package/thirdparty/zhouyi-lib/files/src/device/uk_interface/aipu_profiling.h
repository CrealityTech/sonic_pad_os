/**********************************************************************************
 * This file is CONFIDENTIAL and any use by you is subject to the terms of the
 * agreement between you and Arm China or the terms of the agreement between you
 * and the party authorised by Arm China to disclose this file to you.
 * The confidential and proprietary information contained in this file
 * may only be used by a person authorised under and to the extent permitted
 * by a subsisting licensing agreement from Arm China.
 *
 *        (C) Copyright 2020 Arm Technology (China) Co. Ltd.
 *                    All rights reserved.
 *
 * This entire notice must be reproduced on all copies of this file and copies of
 * this file may only be made by a person if such person is permitted to do so
 * under the terms of a subsisting license agreement from Arm China.
 *
 *********************************************************************************/

/**
 * @file aipu_profiling.h
 * UMD & KMD interface header of AIPU job profiling info.
 */

#ifndef _AIPU_PROFILING_H_
#define _AIPU_PROFILING_H_

#include <linux/types.h>
#ifdef __KERNEL__
#include "../aipu_private.h"
#endif

typedef struct profiling_data {
	struct timeval start_pending;
	struct timeval start_scheduled;
	struct timeval start_run;
	struct timeval end;
    __u32  rdata_tot_msb;
    __u32  rdata_tot_lsb;
    __u32  wdata_tot_msb;
    __u32  wdata_tot_lsb;
    __u32  tot_cycle_msb;
    __u32  tot_cycle_lsb;
    __u32  id_latency_max_msb;
    __u32  id_latency_max_lsb;
    __u32  id_latency_single;
    __u32  all_latency_total_msb;
    __u32  all_latency_total_lsb;
    __u32  all_rdata_total_msb;
    __u32  all_rdata_total_lsb;
    __u32  all_ar_handshake_msb;
    __u32  all_ar_handshake_lsb;
    __u32  max_outstand;
} profiling_data_t;

typedef struct req_pdata {
	__u32 job_id;
	profiling_data_t data;
    __u32 errcode;
} req_pdata_t;

#ifdef __KERNEL__
/*
 * @brief init a profiling data struct
 *
 * @param pdata: pdata to be initialized
 *
 * @return AIPU_ERRCODE_NO_ERROR if successful; others if failed;
 */
int init_profiling_data(profiling_data_t *pdata);
#endif

#endif //_AIPU_PROFILING_H_
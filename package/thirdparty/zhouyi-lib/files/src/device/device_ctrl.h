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
 * @file  device_ctrl.h
 * @brief AIPU User Mode Driver (UMD) device control module header
 */

#ifndef _DEVICE_CTRL_H_
#define _DEVICE_CTRL_H_

#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <signal.h>
#include <pthread.h>
#include "standard_api_v2.0.0.h"
#include "graph/graph_info.h"
#include "graph/buffer_desc.h"
#include "graph/job_desc.h"
#include "uk_interface/aipu_capability.h"
#include "uk_interface/aipu_profiling.h"
#include "uk_interface/aipu_thread_status.h"
#include "uk_interface/aipu_signal.h"
#include "arch/aipu_arch.h"

#define FNAME_LEN 2048
#define OPT_LEN   2148
#define CMD_MEN   8000
#define MAX_SCHED_JOB_NUM 1000

namespace AIRT
{
typedef struct output_desc {
    char fname[FNAME_LEN];
    uint32_t id;
    uint32_t size;
    void* va;
    uint32_t pa;
} output_file_desc_t;

typedef enum {
    SIM_FILE_TYPE_CODE,
    SIM_FILE_TYPE_RODATA,
    SIM_FILE_TYPE_DATA,
    SIM_FILE_TYPE_OUTPUT,
} simfile_type_t;

typedef struct simulation {
    uint32_t graph_id;
    char code_fname[FNAME_LEN];
    char rodata_fname[FNAME_LEN];
    char data_fname[FNAME_LEN];
    buffer_desc_t rodata;
    buffer_desc_t data;
    std::vector<output_file_desc_t> outputs;
    char odata_fname[FNAME_LEN];
    void* argv;
    uint32_t odata_whole_pa;
    void*    odata_whole_va;
    uint32_t odata_whole_size;
} simulation_res_t;

typedef struct sched_graph {
    uint32_t id;
    void* argv;
    uint32_t tid; /* for debug usage */
} sched_graph_t;

typedef struct ring_buf {
    sched_graph_t graphs[MAX_SCHED_JOB_NUM];
    volatile sig_atomic_t head;
    sig_atomic_t tail;
} ring_buf_t;

typedef void (*umd_signal_handler)(void* _this, int sig_num);

class DeviceCtrl
{
private:
    int fd;
    uint32_t host_aipu_shm_offset;
    struct sigaction new_sa;
    static pthread_mutex_t glock;
    static ring_buf_t sched;
    static umd_signal_handler handler;

#if (defined ARM_LINUX) && (ARM_LINUX==1)
private:
    uint32_t aipu_arch;
    uint32_t aipu_version;
    uint32_t aipu_hw_config;
#else
private:
    /* used ony for simulation */
    std::string simulator;
    std::string cfg_file_dir;
    std::string output_dir;
    uint32_t simulation_malloc_top;
    char simulation_cmd[CMD_MEN];
    static std::map<uint32_t, simulation_res_t> graphs;
    static std::map<uint32_t, aipu_arch_t> aipu_arch;

private:
    aipu_status_t create_simulation_input_file(uint32_t graph_id, const buffer_desc_t& desc,
        char* fname, const char* interfix, uint32_t load_size);
    uint32_t get_sim_data_size(const pbuf_alloc_templ_t& pbuf_templ,
        const tbuf_alloc_templ_t& tbuf_templ) const;
    aipu_status_t update_z1_simulation_sys_cfg(uint32_t arch, FILE *fp) const;
    aipu_status_t update_simulation_rtcfg_z1_arch(simulation_res_t& sim, const dev_config_t& config,
        FILE *fp, char* cfg_fname);
    aipu_status_t update_simulation_rtcfg_z2_arch(simulation_res_t& sim, const dev_config_t& config,
        FILE *fp, char* cfg_fname);
    aipu_status_t update_simulation_rtcfg(uint32_t graph_id, const dev_config_t& config);
    static void init_aipu_arch();

public:
    aipu_status_t config_simulation(const aipu_simulation_config_t* config);
    aipu_status_t simulation_alloc_data_buffer(uint32_t graph_id, const pbuf_alloc_templ_t& pbuf_templ,
        pbuf_info_t& pbuf, const tbuf_alloc_templ_t& tbuf_templ, tbuf_info_t& tbuf);
    aipu_status_t simulation_free_data_buffer(uint32_t graph_id);
    aipu_status_t simulation_set_io_info(uint32_t graph_id, const iobuf_info_t& iobuf);
#endif

public:
    aipu_status_t init(umd_signal_handler sig_handler);
    aipu_status_t deinit();
    bool match_target_dev(uint32_t arch, uint32_t version, uint32_t hw_config) const;
    void unload_graph(uint32_t graph_id);
    aipu_status_t malloc_buf(uint32_t size, uint32_t align, buffer_desc_t* buf);
    void load_buffer(volatile void* dest, const void* src, uint32_t bytes);
    aipu_status_t free_buf(const buffer_desc_t* buf);
    aipu_status_t alloc_inst_buffer(uint32_t graph_id, const pbuf_alloc_templ_t& pbuf_templ,
        buffer_desc_t& ibuf_desc);
    aipu_status_t free_inst_buffer(buffer_desc_t& ibuf_desc);
    aipu_status_t load_inst_buffer(uint32_t graph_id, const void* src, uint32_t size,
        buffer_desc_t& ibuf_desc);
    uint32_t get_shm_offset() const;
    aipu_status_t schedule_job_on_aipu(uint32_t graph_id, uint32_t job_id, const dev_config_t& config, void* job_ctx);
    /* TBD: to be removed! */
    static void aipu_interrupt_hdl(int sig_num, siginfo_t* info, void* context);
    aipu_status_t get_dev_status(uint32_t *value) const;

public:
    DeviceCtrl();
    ~DeviceCtrl();
    DeviceCtrl(const DeviceCtrl& ctrl) = delete;
    DeviceCtrl& operator=(const DeviceCtrl& ctrl) = delete;
};
}

#endif //_DEVICE_CTRL_H_

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
 * @file  graph.h
 * @brief AIPU User Mode Driver (UMD) graph module header
 */

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <map>
#include <deque>
#include "standard_api_v2.0.0.h"
#include "device/device_ctrl.h"
#include "graph_def.h"
#include "graph_info.h"
#include "graph_desc_inner.h"
#include "buffer_desc.h"
#include "job_desc.h"

namespace AIRT
{
void signal_handler(void* _this, int sig_num);

class Graph
{
private:
    DeviceCtrl& ctrl;
    bool map_flag;
    void*    gbin;
    uint32_t gbin_size;
    uint32_t entry;
    uint32_t arch;
    uint32_t hw_version;
    uint32_t hw_config;

private:
    tbuf_alloc_templ_t tbuf_templ;
    std::vector<io_tensor_desc_t> inputs;
    std::vector<io_tensor_desc_t> outputs;
    std::vector<io_tensor_desc_t> dumps;

private:
    std::vector<param_map_load_desc_t> param_map;

private:
    aipu_graph_desc_inner_t gdesc;

private:
    /* graph buffer info */
    pbuf_info_t pbuf;
    std::map<uint32_t, tbuf_info_t>  tbufs;
    std::map<uint32_t, iobuf_info_t> iobufs;

private:
    std::map<uint32_t, job_desc_t> built; /**< jobs created but not scheduled */
    std::deque<job_desc_t> sched;         /**< jobs scheduled and running */
    std::map<uint32_t, job_desc_t> end;   /**< jobs excution end */

private:
    void create_tensor_info(const std::vector<io_tensor_desc_t>& desc, aipu_tensor_info_inner_t& info);
    void destroy_tensor_info(aipu_tensor_info_inner_t& info);
    void create_graph_desc(const graph_info_t& info);
    void destroy_graph_desc();
    void create_iobuf_info(const tbuf_info_t& tbuf, const std::vector<io_tensor_desc_t>& io_tensor_desc,
        aipu_tensor_buffer_inner_t& iobuf);
    void destroy_iobuf_info(aipu_tensor_buffer_inner_t& iobuf);
    bool is_timeout(struct timeval sched_time, int32_t time_out);
    job_desc_t* get_sched_job_ptr(uint32_t job_id);
    const job_desc_t* get_sched_job_ptr_const(uint32_t job_id) const;
    std::deque<job_desc_t>::iterator get_sched_job_iter(uint32_t job_id);
    void dump_job_buffers(const job_desc_t& job, const char* interfix);

public:
    static uint32_t handle2graph_id(uint32_t buf_handle);
    static uint32_t job_id2graph_id(uint32_t job_id);

public:
    uint32_t create_buf_handle() const;
    uint32_t create_job_id() const;
    bool is_unload_ok() const;
    void get_graph_desc(aipu_graph_desc_t* gdesc_user) const;
    bool is_buffer_exist(uint32_t handle) const;
    bool is_buffer_free(uint32_t handle) const;
    bool is_job_exist(uint32_t job_id) const;
    bool is_job_built(uint32_t job_id) const;
    bool is_job_scheduled(uint32_t job_id) const;
    bool is_job_end(uint32_t job_id) const;
    aipu_status_t get_debug_info(uint32_t job_id, aipu_debug_info_t* info) const;

public:
    aipu_status_t load(const graph_info_t& info, bool _map_flag);
    aipu_status_t unload();
    aipu_status_t alloc_thread_buffer(aipu_buffer_alloc_info_t* info);
    aipu_status_t free_thread_buffer(uint32_t handle);
    aipu_status_t build_new_job(uint32_t handle, uint32_t* job_id);
    aipu_status_t flush_job(uint32_t job_id);
    aipu_status_t wait_for_job_end(uint32_t job_id, int32_t time_out,
        aipu_job_status_t* status);
    aipu_status_t clean_job(uint32_t job_id);
    aipu_status_t set_dump_options(uint32_t job_id, uint32_t flag, const char* dir);
    friend void signal_handler(void* _this, int sig_num);

public:
    Graph(uint32_t _id, DeviceCtrl& _ctrl);
    ~Graph();
    Graph(const Graph& graph) = delete;
    Graph& operator=(const Graph& graph) = delete;
};
}

#endif /* _GRAPH_H_ */
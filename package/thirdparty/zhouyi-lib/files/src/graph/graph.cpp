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
 * @file  graph.cpp
 * @brief AIPU User Mode Driver (UMD) graph module implementation
 */

#include <string.h>
#include <sys/mman.h>
#include "graph.h"
#include "util/helper.h"
#include "util/log.h"

AIRT::Graph::Graph(uint32_t _id, DeviceCtrl& _ctrl): ctrl(_ctrl)
{
    gdesc.id = _id;
    map_flag = false;
    gbin = nullptr;
    gbin_size = 0;
    entry = 0;
}

AIRT::Graph::~Graph()
{
}

uint32_t AIRT::Graph::handle2graph_id(uint32_t buf_handle)
{
    return buf_handle>>16;
}

uint32_t AIRT::Graph::job_id2graph_id(uint32_t job_id)
{
    return job_id>>16;
}

uint32_t AIRT::Graph::create_buf_handle() const
{
    uint32_t id_candidate = gdesc.id << 16;
    while (tbufs.count(id_candidate) == 1)
    {
        id_candidate++;
    }
    return id_candidate;
}

uint32_t AIRT::Graph::create_job_id() const
{
    uint32_t id_candidate = gdesc.id << 16;
    while (is_job_exist(id_candidate))
    {
        id_candidate++;
    }
    return id_candidate;
}

bool AIRT::Graph::is_unload_ok() const
{
    return sched.empty();
}

void AIRT::Graph::get_graph_desc(aipu_graph_desc_t* gdesc_user) const
{
    memcpy(gdesc_user, &gdesc, sizeof(aipu_graph_desc_t));
}

bool AIRT::Graph::is_buffer_exist(uint32_t handle) const
{
    return tbufs.count(handle);
}

bool AIRT::Graph::is_buffer_free(uint32_t handle) const
{
    bool ret = false;
    std::map<uint32_t, tbuf_info_t>::const_iterator iter = tbufs.find(handle);
    if (iter != tbufs.end())
    {
        ret = iter->second.is_free;
    }
    return ret;
}

bool AIRT::Graph::is_job_exist(uint32_t job_id) const
{
    return (is_job_built(job_id) || is_job_scheduled(job_id) || is_job_end(job_id));
}

bool AIRT::Graph::is_job_built(uint32_t job_id) const
{
    return built.count(job_id);
}

bool AIRT::Graph::is_job_scheduled(uint32_t job_id) const
{
    bool ret = false;
    std::deque<job_desc_t>::const_iterator iter;
    for (iter = sched.begin(); iter != sched.end(); iter++)
    {
        if (iter->id == job_id)
        {
            ret = true;
            break;
        }
    }
    return ret;
}

bool AIRT::Graph::is_job_end(uint32_t job_id) const
{
    return end.count(job_id);
}

job_desc_t* AIRT::Graph::get_sched_job_ptr(uint32_t job_id)
{
    job_desc_t* job = nullptr;
    for (uint32_t i = 0; i < sched.size(); i++)
    {
        if (sched[i].id == job_id)
        {
            job = &sched[i];
            break;
        }
    }
    return job;
}

const job_desc_t* AIRT::Graph::get_sched_job_ptr_const(uint32_t job_id) const
{
    const job_desc_t* job = nullptr;
    for (uint32_t i = 0; i < sched.size(); i++)
    {
        if (sched[i].id == job_id)
        {
            job = &sched[i];
            break;
        }
    }
    return job;
}

std::deque<job_desc_t>::iterator AIRT::Graph::get_sched_job_iter(uint32_t job_id)
{
    std::deque<job_desc_t>::iterator job;
    for (job = sched.begin(); job != sched.end(); job++)
    {
        if (job->id == job_id)
        {
            break;
        }
    }
    return job;
}

aipu_status_t AIRT::Graph::get_debug_info(uint32_t job_id, aipu_debug_info_t* info) const
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    std::map<uint32_t, job_desc_t>::const_iterator tbuf_iter;
    const job_desc_t* job = nullptr;

    if (is_job_end(job_id))
    {
        tbuf_iter = end.find(job_id);
        job = &tbuf_iter->second;
    }
    else if (is_job_scheduled(job_id))
    {
        job = get_sched_job_ptr_const(job_id);
    }
    else if (is_job_exist(job_id))
    {
        tbuf_iter = built.find(job_id);
        job = &tbuf_iter->second;
    }
    else
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
        goto finish;
    }

    *info = job->config.code;

finish:
    return ret;
}

void AIRT::Graph::create_tensor_info(const std::vector<io_tensor_desc_t>& desc, aipu_tensor_info_inner_t& info)
{
    info.number = desc.size();
    if (info.number != 0)
    {
        info.desc = new aipu_tensor_desc_t[info.number];
        for (uint32_t i = 0; i < info.number; i++)
        {
            info.desc[i].id = desc[i].id;
            info.desc[i].size = desc[i].size;
            info.desc[i].fmt = desc[i].fmt;
        }
    }
    else
    {
        info.desc = nullptr;
    }
}

void AIRT::Graph::destroy_tensor_info(aipu_tensor_info_inner_t& info)
{
    info.number = 0;
    if (info.desc != nullptr)
    {
        delete[] info.desc;
        info.desc = nullptr;
    }
}

void AIRT::Graph::create_graph_desc(const graph_info_t& info)
{
    gdesc.graph_version = info.version;
    gdesc.building_tool_major = BUILD_MAJOR(info.build_version);
    gdesc.building_tool_minor = BUILD_MINOR(info.build_version);
    gdesc.build_version = BUILD_NUMBER(info.build_version);
    create_tensor_info(info.inputs, gdesc.inputs);
    create_tensor_info(info.outputs, gdesc.outputs);
    create_tensor_info(info.dumps, gdesc.dumps);
}

void AIRT::Graph::destroy_graph_desc()
{
    gdesc.graph_version = 0;
    gdesc.building_tool_major = 0;
    gdesc.building_tool_minor = 0;
    gdesc.build_version = 0;
    destroy_tensor_info(gdesc.inputs);
    destroy_tensor_info(gdesc.outputs);
    destroy_tensor_info(gdesc.dumps);
}

aipu_status_t AIRT::Graph::load(const graph_info_t& info, bool _map_flag)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    tbuf_info_t  tbuf;
#if (defined X86_LINUX) && (X86_LINUX==1)
    iobuf_info_t iobuf;
    uint32_t handle = create_buf_handle();
#else
    buffer_desc_t buf;
#endif

    map_flag = _map_flag;
    if (map_flag)
    {
        gbin = info.gbin;
        gbin_size = info.gbin_size;
    }
    arch = AIPU_ARCH(info.device);
    hw_version = AIPU_VERSION(info.device);
    hw_config = AIPU_CONFIG(info.device);
    entry = info.entry;
    tbuf_templ = info.tbuf_templ;
    inputs = info.inputs;
    outputs = info.outputs;
    dumps = info.dumps;
    param_map = info.param_map;
    create_graph_desc(info);

    /* alloc and load instruction buffer */
    ret = ctrl.alloc_inst_buffer(gdesc.id, info.pbuf_templ, pbuf.inst);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }
    ret = ctrl.load_inst_buffer(gdesc.id, info.pbuf_templ.inst_src, info.pbuf_templ.inst_size, pbuf.inst);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }

#if (defined X86_LINUX) && (X86_LINUX==1)
    ret = ctrl.simulation_alloc_data_buffer(gdesc.id, info.pbuf_templ, pbuf, info.tbuf_templ, tbuf);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }
    tbuf.handle = handle;
    tbuf.is_free = true;
    tbufs[handle] = tbuf;
    /* calculate io buffer address */
    create_iobuf_info(tbuf, inputs, iobuf.inputs);
    create_iobuf_info(tbuf, outputs, iobuf.outputs);
    create_iobuf_info(tbuf, dumps, iobuf.dumps);
    ret = ctrl.simulation_set_io_info(gdesc.id, iobuf);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }
    iobufs[handle] = iobuf;
#else
    for (uint32_t i = 0; i < info.pbuf_templ.static_sections.size(); i++)
    {
        ret = ctrl.malloc_buf(info.pbuf_templ.static_sections[i].size, 1, &buf);
        if (AIPU_STATUS_SUCCESS != ret)
        {
            goto finish;
        }
        pbuf.static_buf.push_back(buf);
    }
#endif

    for (uint32_t i = 0; i < info.pbuf_templ.static_sections.size(); i++)
    {
        ctrl.load_buffer(pbuf.static_buf[i].va, info.pbuf_templ.static_sections[i].load_src,
            info.pbuf_templ.static_sections[i].size);
    }

finish:
    return ret;
}

aipu_status_t AIRT::Graph::unload()
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    std::map<uint32_t, tbuf_info_t>::iterator  tbuf_iter;
    std::map<uint32_t, iobuf_info_t>::iterator iobuf_iter;

    built.clear();
    sched.clear();
    end.clear();
    param_map.clear();
    inputs.clear();
    outputs.clear();
    dumps.clear();

    if (nullptr != pbuf.inst.va)
    {
        ret = ctrl.free_inst_buffer(pbuf.inst);
        if (AIPU_STATUS_SUCCESS != ret)
        {
            goto finish;
        }
        pbuf.inst.pa = 0;
        pbuf.inst.va = nullptr;
        pbuf.inst.size = 0;
    }

#if (defined ARM_LINUX) && (ARM_LINUX==1)
    for (uint32_t i = 0; i < pbuf.static_buf.size(); i++)
    {
        ret = ctrl.free_buf(&pbuf.static_buf[i]);
        if (AIPU_STATUS_SUCCESS != ret)
        {
            goto finish;
        }
    }
    pbuf.static_buf.clear();
#endif

    for (tbuf_iter = tbufs.begin(); tbuf_iter != tbufs.end(); tbuf_iter++)
    {
        ret = free_thread_buffer(tbuf_iter->first);
        if (AIPU_STATUS_SUCCESS != ret)
        {
            goto finish;
        }
    }
    tbufs.clear();

    if (map_flag && (nullptr != gbin))
    {
        munmap(gbin, gbin_size);
        gbin = nullptr;
        gbin_size = 0;
    }
    destroy_graph_desc();

    ctrl.unload_graph(gdesc.id);

finish:
    return ret;
}

void AIRT::Graph::create_iobuf_info(const tbuf_info_t& tbuf, const std::vector<io_tensor_desc_t>& io_tensor_desc,
    aipu_tensor_buffer_inner_t& iobuf)
{
    iobuf.number = io_tensor_desc.size();

    if (iobuf.number != 0)
    {
        iobuf.tensors = new aipu_buffer_t[iobuf.number];
        iobuf.pa = new uint32_t[iobuf.number];
        for (uint32_t i = 0; i < iobuf.number; i++)
        {
            uint32_t sec_iter = io_tensor_desc[i].ref_section_iter;
            iobuf.tensors[i].id = io_tensor_desc[i].id;
            iobuf.tensors[i].size = io_tensor_desc[i].size;
            iobuf.tensors[i].va = (void*)((unsigned long)tbuf.reuse_buf[sec_iter].va +
                io_tensor_desc[i].offset_in_section);
            iobuf.pa[i] = tbuf.reuse_buf[sec_iter].pa + io_tensor_desc[i].offset_in_section;
        }
    }
    else
    {
        iobuf.tensors = nullptr;
        iobuf.pa = nullptr;
    }
}

void AIRT::Graph::destroy_iobuf_info(aipu_tensor_buffer_inner_t& iobuf)
{
    if (iobuf.number != 0)
    {
        iobuf.number = 0;
        if (iobuf.tensors != nullptr)
        {
            delete[] iobuf.tensors;
            iobuf.tensors = nullptr;
        }
        if (iobuf.pa != nullptr)
        {
            delete[] iobuf.pa;
            iobuf.pa = nullptr;
        }
    }
}

aipu_status_t AIRT::Graph::alloc_thread_buffer(aipu_buffer_alloc_info_t* info)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    uint32_t handle = 0;
    tbuf_info_t  tbuf;
    std::map<uint32_t, iobuf_info_t>::const_iterator iobuf_iter;
#if (defined ARM_LINUX) && (ARM_LINUX==1)
    buffer_desc_t buf;
    iobuf_info_t iobuf;
#endif

    if (nullptr == info)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

#if (defined X86_LINUX) && (X86_LINUX==1)
    /* has been allocated; just get handle here */
    for (iobuf_iter = iobufs.begin(); iobuf_iter != iobufs.end(); iobuf_iter++)
    {
        handle = iobuf_iter->first;
    }
#else
    /* stack */
    ret = ctrl.malloc_buf(tbuf_templ.stack_size, 1, &tbuf.stack);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }
    /* rodata */
    ret = ctrl.malloc_buf(tbuf_templ.rodata_size, 1, &tbuf.rodata);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }
    /* reuse buffers */
    for (uint32_t i = 0; i < tbuf_templ.reuse_sections.size(); i++)
    {
        ret = ctrl.malloc_buf(tbuf_templ.reuse_sections[i].size, 1, &buf);
        if (AIPU_STATUS_SUCCESS != ret)
        {
            goto finish;
        }
        tbuf.reuse_buf.push_back(buf);
    }

    handle = create_buf_handle();
    tbuf.handle = handle;
    tbuf.is_free = true;
    tbufs[handle] = tbuf;

    create_iobuf_info(tbuf, inputs, iobuf.inputs);
    create_iobuf_info(tbuf, outputs, iobuf.outputs);
    create_iobuf_info(tbuf, dumps, iobuf.dumps);
    iobufs[handle] = iobuf;
#endif

    /* success */
    info->handle = handle;
    info->inputs.number   = iobufs[handle].inputs.number;
    info->inputs.tensors  = iobufs[handle].inputs.tensors;
    info->outputs.number  = iobufs[handle].outputs.number;
    info->outputs.tensors = iobufs[handle].outputs.tensors;
    info->dumps.number    = iobufs[handle].dumps.number;
    info->dumps.tensors   = iobufs[handle].dumps.tensors;

finish:
    return ret;
}

aipu_status_t AIRT::Graph::free_thread_buffer(uint32_t handle)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    std::map<uint32_t, tbuf_info_t>::iterator  tbuf_iter  = tbufs.find(handle);
    std::map<uint32_t, iobuf_info_t>::iterator iobuf_iter = iobufs.find(handle);

    if ((tbufs.end() == tbuf_iter) || (iobufs.end() == iobuf_iter))
    {
        ret = AIPU_STATUS_ERROR_INVALID_HANDLE;
        goto finish;
    }

    if (!tbuf_iter->second.is_free)
    {
        ret = AIPU_STATUS_ERROR_BUSY_HANDLE;
        goto finish;
    }

    destroy_iobuf_info(iobuf_iter->second.inputs);
    destroy_iobuf_info(iobuf_iter->second.outputs);
    destroy_iobuf_info(iobuf_iter->second.dumps);
    iobufs.erase(iobuf_iter->first);

    /* rodata */
    ret = ctrl.free_buf(&tbuf_iter->second.rodata);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }

#if (defined ARM_LINUX) && (ARM_LINUX==1)
    /* stack */
    ret = ctrl.free_buf(&tbuf_iter->second.stack);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }

    /* reuse buffers */
    for (uint32_t i = 0; i < tbuf_iter->second.reuse_buf.size(); i++)
    {
        ret = ctrl.free_buf(&tbuf_iter->second.reuse_buf[i]);
        if (AIPU_STATUS_SUCCESS != ret)
        {
            goto finish;
        }
    }
#else
    ret = ctrl.simulation_free_data_buffer(gdesc.id);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }
#endif
    tbufs.erase(tbuf_iter->first);

finish:
    return ret;
}

aipu_status_t AIRT::Graph::build_new_job(uint32_t handle, uint32_t* job_id)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    job_desc_t job;
    std::map<uint32_t, tbuf_info_t>::iterator  tbuf_iter  = tbufs.find(handle);
    std::map<uint32_t, iobuf_info_t>::iterator iobuf_iter = iobufs.find(handle);
    uint32_t offset = ctrl.get_shm_offset();

    if ((tbufs.end() == tbuf_iter) || (iobufs.end() == iobuf_iter))
    {
        ret = AIPU_STATUS_ERROR_INVALID_HANDLE;
        goto finish;
    }

    if (!tbuf_iter->second.is_free)
    {
        ret = AIPU_STATUS_ERROR_BUSY_HANDLE;
        goto finish;
    }

    tbuf_iter->second.is_free = false;

    /* load rodata buffer */
    ctrl.load_buffer(tbufs[handle].rodata.va, tbuf_templ.rodata_src, tbuf_templ.rodata_size);
    for (uint32_t i = 0; i < param_map.size(); i++)
    {
        volatile uint32_t* dest = (uint32_t*)((unsigned long)tbufs[handle].rodata.va +
            param_map[i].offset_in_map);
        uint32_t ref_iter = param_map[i].ref_section_iter;
        uint32_t sec_offset = param_map[i].sub_section_offset;
        uint32_t sub_sec_pa = 0;
        if (param_map[i].offset_in_map >= tbufs[handle].rodata.size)
        {
            ret = AIPU_STATUS_ERROR_INVALID_SIZE;
            goto finish;
        }

        if (param_map[i].load_type == PARAM_MAP_LOAD_TYPE_REUSE)
        {
            if (ref_iter >= tbufs[handle].reuse_buf.size())
            {
                ret = AIPU_STATUS_ERROR_INVALID_SIZE;
                goto finish;
            }
            sub_sec_pa = tbufs[handle].reuse_buf[ref_iter].pa + sec_offset - ctrl.get_shm_offset();
        }
        else if (param_map[i].load_type == PARAM_MAP_LOAD_TYPE_STATIC)
        {
            if (ref_iter >= pbuf.static_buf.size())
            {
                ret = AIPU_STATUS_ERROR_INVALID_SIZE;
                goto finish;
            }
            sub_sec_pa = pbuf.static_buf[ref_iter].pa + sec_offset - ctrl.get_shm_offset();
        }
        *dest = ((sub_sec_pa & param_map[i].addr_mask) | (*dest & (~param_map[i].addr_mask)));
    }

    job.id = create_job_id();
    job.state = JOB_STATE_BUILT;
    job.buf_handle = handle;
    job.dump_options = 0;
    job.config.arch = arch;
    job.config.hw_version = hw_version;
    job.config.hw_config =hw_config;
    job.config.code.instruction_base_pa = pbuf.inst.pa - offset;
    job.config.code.start_pc_pa = job.config.code.instruction_base_pa + entry;
    job.config.code.interrupt_pc_pa = job.config.code.instruction_base_pa + 0x10;
    job.config.rodata_base = tbuf_iter->second.rodata.pa - offset;
    job.config.stack_base = tbuf_iter->second.stack.pa - offset;
    built[job.id] = job;

    /* success */
    *job_id = job.id;

finish:
    return ret;
}

void AIRT::Graph::dump_job_buffers(const job_desc_t& job, const char* interfix)
{
    char fname[4096];
    std::map<uint32_t, tbuf_info_t>::const_iterator tbuf_iter = tbufs.find(job.buf_handle);
    std::map<uint32_t, iobuf_info_t>::const_iterator iobuf_iter = iobufs.find(job.buf_handle);

    if ((tbufs.end() == tbuf_iter) || (iobufs.end() == iobuf_iter))
    {
        LOG(LOG_WARN, "thread buffer for job 0x%x not found!", job.id);
        return;
    }

    if (job.dump_options & AIPU_DUMP_INST)
    {
        snprintf(fname, sizeof(fname), "%s/Graph0x%x_Job0x%x_%s_Code_Section_Base0x%x_Size0x%x.bin",
            job.dump_dir.c_str(), gdesc.id, job.id, interfix,
            pbuf.inst.pa - ctrl.get_shm_offset(), pbuf.inst.real_size);
        umd_dump_file_helper(fname, (void*)pbuf.inst.va, pbuf.inst.real_size);
    }

    if (job.dump_options & AIPU_DUMP_RO)
    {
        snprintf(fname, sizeof(fname), "%s/Graph0x%x_Job0x%x_%s_Rodata_Section_Base0x%x_Size0x%x.bin",
            job.dump_dir.c_str(), gdesc.id, job.id, interfix,
            tbuf_iter->second.rodata.pa - ctrl.get_shm_offset(), tbuf_iter->second.rodata.real_size);
        umd_dump_file_helper(fname, (void*)tbuf_iter->second.rodata.va, tbuf_iter->second.rodata.real_size);
    }

    if (job.dump_options & AIPU_DUMP_STACK)
    {
        snprintf(fname, sizeof(fname), "%s/Graph0x%x_Job0x%x_%s_Stack_Section_Base0x%x_Size0x%x.bin",
            job.dump_dir.c_str(), gdesc.id, job.id, interfix,
            tbuf_iter->second.stack.pa - ctrl.get_shm_offset(), tbuf_iter->second.stack.real_size);
        umd_dump_file_helper(fname, (void*)tbuf_iter->second.stack.va, tbuf_iter->second.stack.real_size);
    }

    if (job.dump_options & AIPU_DUMP_STATIC_TENSOR)
    {
        for (uint32_t i = 0; i < pbuf.static_buf.size(); i++)
        {
            snprintf(fname, sizeof(fname), "%s/Graph0x%x_Job0x%x_%s_Static_Section%u_Base0x%x_Size0x%x.bin",
                job.dump_dir.c_str(), gdesc.id, job.id, interfix, i,
                pbuf.static_buf[i].pa - ctrl.get_shm_offset(), pbuf.static_buf[i].real_size);
            umd_dump_file_helper(fname, (void*)pbuf.static_buf[i].va, pbuf.static_buf[i].real_size);
        }
    }

    if (job.dump_options & AIPU_DUMP_REUSE_TENSOR)
    {
        for (uint32_t i = 0; i < tbuf_iter->second.reuse_buf.size(); i++)
        {
            snprintf(fname, sizeof(fname), "%s/Graph0x%x_Job0x%x_%s_Reuse_Section%u_Base0x%x_Size0x%x.bin",
                job.dump_dir.c_str(), gdesc.id, job.id, interfix, i,
                tbuf_iter->second.reuse_buf[i].pa - ctrl.get_shm_offset(),
                tbuf_iter->second.reuse_buf[i].real_size);
            umd_dump_file_helper(fname, (void*)tbuf_iter->second.reuse_buf[i].va,
                tbuf_iter->second.reuse_buf[i].real_size);
        }
    }

    /* only dump output/intermediate tensor afret running done */
    if ((job.dump_options & AIPU_DUMP_OUT_TENSOR) && (interfix[0] == 'A'))
    {
        for (uint32_t i = 0; i < iobuf_iter->second.outputs.number; i++)
        {
            snprintf(fname, sizeof(fname), "%s/Graph0x%x_Job0x%x_%s_OutTensor%u_Base0x%x_Size0x%x.bin",
                job.dump_dir.c_str(), gdesc.id, job.id, interfix, i,
                iobuf_iter->second.outputs.pa[i] - ctrl.get_shm_offset(),
                iobuf_iter->second.outputs.tensors[i].size);
            umd_dump_file_helper(fname, (void*)iobuf_iter->second.outputs.tensors[i].va,
                iobuf_iter->second.outputs.tensors[i].size);
        }
    }

    if ((job.dump_options & AIPU_DUMP_INTER_TENSOR) && (interfix[0] == 'A'))
    {
        for (uint32_t i = 0; i < iobuf_iter->second.dumps.number; i++)
        {
            snprintf(fname, sizeof(fname), "%s/Graph0x%x_Job0x%x_%s_InterTensor%u_Base0x%x_Size0x%x.bin",
                job.dump_dir.c_str(), gdesc.id, job.id, interfix, i,
                iobuf_iter->second.dumps.pa[i] - ctrl.get_shm_offset(),
                iobuf_iter->second.dumps.tensors[i].size);
            umd_dump_file_helper(fname, (void*)iobuf_iter->second.dumps.tensors[i].va,
                iobuf_iter->second.dumps.tensors[i].size);
        }
    }
}

aipu_status_t AIRT::Graph::flush_job(uint32_t job_id)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    std::map<uint32_t, job_desc_t>::iterator iter = built.find(job_id);

    if (!is_job_exist(job_id))
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
        goto finish;
    }

    if (built.end() == iter)
    {
        ret = AIPU_STATUS_ERROR_JOB_SCHED;
        goto finish;
    }

    if (iter->second.dump_options & AIPU_DUMP_BEFORE_RUN)
    {
        dump_job_buffers(iter->second, "Before_Run");
    }

    iter->second.state = JOB_STATE_SCHED;
    sched.push_back(iter->second);

    ret = ctrl.schedule_job_on_aipu(gdesc.id, job_id, iter->second.config, (void*)this);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        get_sched_job_ptr(job_id)->state = JOB_STATE_NO_STATE;
        goto finish;
    }

    built.erase(job_id);
    gettimeofday(&get_sched_job_ptr(job_id)->sched_time, NULL);

finish:
    return ret;
}

bool AIRT::Graph::is_timeout(struct timeval sched_time, int32_t time_out)
{
    struct timeval curr;
    long timeuse = 0;
    if (time_out <= 0)
    {
        return false;
    }
    else
    {
        gettimeofday(&curr, NULL);
        timeuse =1000 * (curr.tv_sec - sched_time.tv_sec) +
            (long)(curr.tv_usec/1000 - sched_time.tv_usec/1000);
        return (timeuse > (uint32_t)time_out);
    }
}

aipu_status_t AIRT::Graph::wait_for_job_end(uint32_t job_id, int32_t time_out,
    aipu_job_status_t* status)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    job_desc_t* job = get_sched_job_ptr(job_id);
    std::map<uint32_t, job_desc_t>::iterator iter = end.find(job_id);

    if (!is_job_exist(job_id))
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
        goto finish;
    }

    if (nullptr == job)
    {
        if (end.end() == iter)
        {
            ret = AIPU_STATUS_ERROR_JOB_NOT_SCHED;
            goto finish;
        }
        else
        {
            /* job has done */
            job = &iter->second;
        }
    }
    else
    {
        /* job still running */
        while ((job->state != JOB_STATE_DONE) &&
        (job->state != JOB_STATE_EXCEPTION))
        {
            if (is_timeout(job->sched_time, time_out))
            {
                ret = AIPU_STATUS_ERROR_JOB_TIMEOUT;
                job->state = JOB_STATE_NO_STATE;
                break;
            }
        }

        end[job_id] = *job;
        sched.erase(get_sched_job_iter(job_id));
    }

    if (end[job_id].state == JOB_STATE_EXCEPTION)
    {
        ret = AIPU_STATUS_ERROR_JOB_EXCEPTION;
    }
    *status = (aipu_job_status_t)end[job_id].state;

    if (end[job_id].dump_options & AIPU_DUMP_AFTER_RUN)
    {
        dump_job_buffers(end[job_id], "After_Run");
    }

finish:
    return ret;
}

aipu_status_t AIRT::Graph::clean_job(uint32_t job_id)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    std::map<uint32_t, job_desc_t>::iterator  job_iter = end.find(job_id);
    std::map<uint32_t, tbuf_info_t>::iterator tbuf_iter;

    if (end.end() == job_iter)
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_END;
        goto finish;
    }

    tbuf_iter = tbufs.find(job_iter->second.buf_handle);
    if (tbuf_iter == tbufs.end())
    {
        ret = AIPU_STATUS_ERROR_INVALID_HANDLE;
        goto finish;
    }

    tbuf_iter->second.is_free = true;
    end.erase(job_id);

finish:
    return ret;
}

aipu_status_t AIRT::Graph::set_dump_options(uint32_t job_id, uint32_t flag, const char* dir)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    std::map<uint32_t, job_desc_t>::iterator job_iter = built.find(job_id);

    if (built.end() == job_iter)
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
        goto finish;
    }

    /**
     * invalid options:
     * 1. use config >= AIPU_DUMP_MAX;
     * 2. do not set dump timing info;
     * 3. set dump output before running;
     * 4. do not set dump what type of data;
     */
    if ((flag >= AIPU_DUMP_MAX) ||
        (!(flag & (AIPU_DUMP_BEFORE_RUN | AIPU_DUMP_AFTER_RUN))) ||
        (flag == (AIPU_DUMP_BEFORE_RUN | AIPU_DUMP_OUT_TENSOR)) ||
        (!(flag & (AIPU_DUMP_BEFORE_RUN - 1))))
    {
        ret = AIPU_STATUS_ERROR_INVALID_OPTIONS;
        goto finish;
    }

    /* success */
    job_iter->second.dump_options = flag;
    job_iter->second.dump_dir = dir;

finish:
    return ret;
}

void AIRT::signal_handler(void* _this, int sig_num)
{
    job_state_t state = JOB_STATE_NO_STATE;

    if (AIPU_SIG_DONE == sig_num)
    {
        state = JOB_STATE_DONE;
    }
    else if (AIPU_SIG_EXCEPTION == sig_num)
    {
        state = JOB_STATE_EXCEPTION;
    }

    for (uint32_t i = 0; i < ((Graph*)_this)->sched.size(); i++)
    {
        if (((Graph*)_this)->sched[i].state == JOB_STATE_SCHED)
        {
            ((Graph*)_this)->sched[i].state = state;
            break;
        }
    }
}
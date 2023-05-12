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
 * @file  context.cpp
 * @brief AIPU User Mode Driver (UMD) context module implementation
 */

#include <unistd.h>
#include <string.h>
#include "context.h"
#include "graph/graph.h"
#include "util/log.h"
#include "util/debug.h"
#include "util/helper.h"

AIRT::MainContext::MainContext(): ctrl()
{
}

AIRT::MainContext::~MainContext()
{
}

bool AIRT::MainContext::is_deinit_ok() const
{
    bool ret = true;
    GraphTable::const_iterator iter;
    for (iter = graphs.begin(); iter != graphs.end(); iter++)
    {
        if (!iter->second->is_unload_ok())
        {
            ret = false;
            break;
        }
    }
    return ret;
}

void AIRT::MainContext::print_graph_header_info(const bin_hdr_t& header) const
{
#if (defined PRINT_GRAPH_HDR_PARSING_INFO) && (PRINT_GRAPH_HDR_PARSING_INFO == 1)
    LOG(LOG_DEFAULT, "Target device: arch %u, version %u, configuration: %u",
        AIPU_ARCH(header.device),
        AIPU_VERSION(header.device),
        AIPU_CONFIG(header.device));
    LOG(LOG_DEFAULT, "Graph version: %u", header.version);
    LOG(LOG_DEFAULT, "Building tool version: major %u, minor %u, build number: %u",
        BUILD_MAJOR(header.build_version),
        BUILD_MINOR(header.build_version),
        BUILD_NUMBER(header.build_version));
    LOG(LOG_DEFAULT, "Graph fsize: %u", header.filesize);
    LOG(LOG_DEFAULT, "Entry: 0x%x", header.entry);
    LOG(LOG_DEFAULT, "Text section: offset 0x%x, size 0x%x", header.text_offset, header.text_size);
    LOG(LOG_DEFAULT, "Rodata section: offset 0x%x, size 0x%x", header.rodata_offset, header.rodata_size);
    LOG(LOG_DEFAULT, "DCR section: offset 0x%x, size 0x%x", header.dcr_offset, header.dcr_size);
    LOG(LOG_DEFAULT, "Data section: offset 0x%x, size 0x%x", header.data_offset, header.data_size);
    LOG(LOG_DEFAULT, "BSS section: offset 0x%x, size 0x%x", header.bss_offset, header.bss_size);
    LOG(LOG_DEFAULT, "Misc section: offset 0x%x, size 0x%x", header.misc_offset, header.misc_size);
#endif
}

aipu_status_t AIRT::MainContext::parse_graph_header(const void* graph, uint32_t size,
    graph_info_t& info) const
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    bin_hdr_t header;

    memcpy(&header, graph, BIN_HDR_SIZE);
    print_graph_header_info(header);

    if (strcmp(header.ident, MAGIC) != 0)
    {
        ret = AIPU_STATUS_ERROR_INVALID_GBIN;
        goto finish;
    }

    if (size < header.filesize)
    {
        ret = AIPU_STATUS_ERROR_INVALID_GBIN;
        goto finish;
    }

    if (AIPU_LOADABLE_GRAPH_V0003 != GVERSION_MINOR(header.version))
    {
        ret = AIPU_STATUS_ERROR_GVERSION_UNSUPPORTED;
        goto finish;
    }

    if (header.hdrsize != BIN_HDR_SIZE)
    {
        ret = AIPU_STATUS_ERROR_INVALID_GBIN;
        goto finish;
    }

    if (header.entry >= header.text_size)
    {
        ret = AIPU_STATUS_ERROR_INVALID_GBIN;
        goto finish;
    }

    if ((header.text_offset < BIN_HDR_SIZE) ||
        ((header.text_offset + header.text_size) > header.rodata_offset) ||
        (header.text_size == 0))
    {
        ret = AIPU_STATUS_ERROR_INVALID_GBIN;
        goto finish;
    }

    if ((header.dcr_offset != 0) || (header.dcr_size != 0))
    {
        ret = AIPU_STATUS_ERROR_INVALID_GBIN;
        goto finish;
    }

    if (((header.rodata_offset + header.rodata_size) > header.data_offset) ||
        (header.rodata_size == 0))
    {
        ret = AIPU_STATUS_ERROR_INVALID_GBIN;
        goto finish;
    }

    if (((header.data_offset + header.data_size) > header.bss_offset) ||
        (header.data_size == 0))
    {
        ret = AIPU_STATUS_ERROR_INVALID_GBIN;
        goto finish;
    }

    if (((header.bss_offset + header.bss_size) > header.filesize) ||
        (header.bss_size == 0))
    {
        ret = AIPU_STATUS_ERROR_INVALID_GBIN;
        goto finish;
    }

    /* success */
    info.gbin = (void*)graph;
    info.gbin_size = size;
    info.device = header.device;
    info.version = header.version;
    info.build_version = header.build_version;
    info.entry = header.entry;
    info.bss_src = (void*)((unsigned long)graph + header.bss_offset);
    info.pbuf_templ.inst_src = (void*)((unsigned long)graph + header.text_offset);
    info.pbuf_templ.inst_size = header.text_size;
    info.pbuf_templ.data_src = (void*)((unsigned long)graph + header.data_offset);
    info.pbuf_templ.data_size = header.data_size;
    info.tbuf_templ.rodata_src = (void*)((unsigned long)graph + header.rodata_offset);
    info.tbuf_templ.rodata_size = header.rodata_size;

finish:
    return ret;
}

template<typename sub_section_desc_type>
aipu_status_t AIRT::MainContext::fill_io_tensor_desc_inner(uint32_t reuse_sec_iter,
    uint32_t sub_sec_iter, const sub_section_desc_type& sub_section_load, graph_info_t& info) const
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    io_tensor_desc_t io_desc;

    io_desc.id = sub_section_load.id;
    io_desc.ref_section_iter = reuse_sec_iter;
    io_desc.offset_in_section = sub_section_load.offset_in_section_exec;
    io_desc.size = sub_section_load.size;
    io_desc.fmt.data_type = (aipu_data_type_t)sub_section_load.dtype;
    io_desc.fmt.layout = (aipu_tensor_layout_t)sub_section_load.layout;
    io_desc.fmt.shape.N = sub_section_load.shape[0];
    io_desc.fmt.shape.H = sub_section_load.shape[1];
    io_desc.fmt.shape.W = sub_section_load.shape[2];
    io_desc.fmt.shape.C = sub_section_load.shape[3];

    if (SECTION_TYPE_INPUT == sub_section_load.type)
    {
        info.inputs.push_back(io_desc);
    }
    else if (SECTION_TYPE_OUTPUT == sub_section_load.type)
    {
        info.outputs.push_back(io_desc);
    }
    else if (SECTION_TYPE_DUMP == sub_section_load.type)
    {
        info.dumps.push_back(io_desc);
    }

    return ret;
}

template<typename bss_hdr_type, typename static_section_desc_type,
   typename reuse_section_desc_type, typename sub_section_desc_type>
aipu_status_t AIRT::MainContext::parse_bss_section(graph_info_t& info) const
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    bss_hdr_type bss_header;
    static_section_desc_type static_desc_load;
    reuse_section_desc_type  reuse_desc_load;
    sub_section_desc_type    sub_desc_load;
    void* desc_load_addr  = nullptr;
    section_desc_t section_ir;
    param_map_load_desc_t param;

    /* bss sections (stack/static/reuse descriptors) */
    memcpy(&bss_header, info.bss_src, sizeof(bss_hdr_type));
    if ((bss_header.stack_size == 0) ||
        (bss_header.static_section_desc_cnt == 0) ||
        (bss_header.reuse_section_desc_cnt == 0))
    {
        ret = AIPU_STATUS_ERROR_INVALID_GBIN;
        goto finish;
    }

    /* get stack section descriptions */
    info.tbuf_templ.stack_size        = bss_header.stack_size;
    info.tbuf_templ.stack_align_bytes = bss_header.stack_align_bytes;

    /* static sections (weight/bias) in bss */
    desc_load_addr = (void*)((unsigned long)info.bss_src + sizeof(bss_hdr_type));
    for (uint32_t static_sec_iter = 0; static_sec_iter < bss_header.static_section_desc_cnt; static_sec_iter++)
    {
        section_ir.init();
        memcpy(&static_desc_load, desc_load_addr, sizeof(static_section_desc_type));

        /* update sub-section-desc info. */
        desc_load_addr = (void*)((unsigned long)desc_load_addr + sizeof(static_section_desc_type));
        for (uint32_t sub_sec_iter = 0; sub_sec_iter < static_desc_load.sub_section_cnt; sub_sec_iter++)
        {
            sub_section_desc_t sub_desc_ir;
            memcpy(&sub_desc_load, desc_load_addr, sizeof(sub_section_desc_type));

            /* get subsection info. */
            sub_desc_ir.offset_in_section = sub_desc_load.offset_in_section_exec;
            section_ir.sub_sections.push_back(sub_desc_ir);

            /* update parameter map element */
            desc_load_addr = (void*)((unsigned long)desc_load_addr + sizeof(sub_section_desc_type));
            for (uint32_t ro_entry_iter = 0; ro_entry_iter < sub_desc_load.offset_in_ro_cnt; ro_entry_iter++)
            {
                uint32_t offset_in_ro = *(uint32_t*)desc_load_addr;
                param.init(offset_in_ro, PARAM_MAP_LOAD_TYPE_STATIC, static_sec_iter, sub_sec_iter,
                    sub_desc_load.offset_in_section_exec, sub_desc_load.addr_mask);
                info.param_map.push_back(param);
                desc_load_addr = (void*)((unsigned long)desc_load_addr + sizeof(uint32_t));
            }
        }

        /* update section descriptor */
        section_ir.size = static_desc_load.size;
        section_ir.load_src = (void*)((unsigned long)info.pbuf_templ.data_src + static_desc_load.offset_in_file);
        info.pbuf_templ.static_sections.push_back(section_ir);
    }

    /* reuse sections (input/output/intermediate) in bss */
    for (uint32_t reuse_sec_iter = 0; reuse_sec_iter < bss_header.reuse_section_desc_cnt; reuse_sec_iter++)
    {
        section_ir.init();
        memcpy(&reuse_desc_load, desc_load_addr, sizeof(reuse_section_desc_type));

        desc_load_addr = (void*)((unsigned long)desc_load_addr + sizeof(reuse_section_desc_type));
        for (uint32_t sub_sec_iter = 0; sub_sec_iter < reuse_desc_load.sub_section_cnt; sub_sec_iter++)
        {
            sub_section_desc_t sub_desc_ir;
            memcpy(&sub_desc_load, desc_load_addr, sizeof(sub_section_desc_type));

            /* get io tensor info if this sub-section represents io */
            if ((SECTION_TYPE_INPUT == sub_desc_load.type) ||
                (SECTION_TYPE_OUTPUT == sub_desc_load.type) ||
                (SECTION_TYPE_DUMP == sub_desc_load.type))
            {
                fill_io_tensor_desc_inner<sub_section_desc_type>(reuse_sec_iter,
                    sub_sec_iter, sub_desc_load, info);
            }

            /* get subsection info. */
            sub_desc_ir.offset_in_section = sub_desc_load.offset_in_section_exec;
            section_ir.sub_sections.push_back(sub_desc_ir);

            /* update parameter map element */
            desc_load_addr = (void*)((unsigned long)desc_load_addr + sizeof(sub_section_desc_type));
            for (uint32_t ro_entry_iter = 0; ro_entry_iter < sub_desc_load.offset_in_ro_cnt; ro_entry_iter++)
            {
                uint32_t offset_in_ro = *(uint32_t*)desc_load_addr;
                param.init(offset_in_ro, PARAM_MAP_LOAD_TYPE_REUSE, reuse_sec_iter, sub_sec_iter,
                    sub_desc_load.offset_in_section_exec, sub_desc_load.addr_mask);
                info.param_map.push_back(param);
                desc_load_addr = (void*)((unsigned long)desc_load_addr + sizeof(uint32_t));
            }
        }

        /* update section descriptor */
        section_ir.load_src = nullptr;
        section_ir.size = reuse_desc_load.size;
        info.tbuf_templ.reuse_sections.push_back(section_ir);
    }

finish:
    return ret;
}

aipu_status_t AIRT::MainContext::parse_graph(const void* graph, uint32_t size, graph_info_t& info) const
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;

    if (nullptr == graph)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    if (size < BIN_HDR_SIZE)
    {
        ret = AIPU_STATUS_ERROR_INVALID_SIZE;
        goto finish;
    }

    ret = parse_graph_header(graph, size, info);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }

    if (AIPU_LOADABLE_GRAPH_V0003 == GVERSION_MINOR(info.version))
    {
        ret = parse_bss_section<bss_hdr_v3_t, static_section_desc_v3_t,
                reuse_section_desc_v3_t, sub_section_desc_v3_t>(info);
    }
    else
    {
        ret = AIPU_STATUS_ERROR_GVERSION_UNSUPPORTED;
        goto finish;
    }

    if (!ctrl.match_target_dev(AIPU_ARCH(info.device),
        AIPU_VERSION(info.device), AIPU_CONFIG(info.device)))
    {
        ret = AIPU_STATUS_ERROR_TARGET_NOT_FOUND;
        goto finish;
    }

finish:
    return ret;
}

uint32_t AIRT::MainContext::create_unique_graph_id() const
{
    uint32_t id_candidate = 1;
    while (graphs.count(id_candidate) == 1)
    {
        id_candidate++;
    }
    return id_candidate;
}

aipu_status_t AIRT::MainContext::create_graph_object(const graph_info_t& info, bool map_flag,
    aipu_graph_desc_t* gdesc)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    uint32_t id = 0;
    Graph* p_gobj = nullptr;

    if (nullptr == gdesc)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    /* assumed that info is a valid one returned by parse_graph() */
    id = create_unique_graph_id();
    p_gobj = new Graph(id, ctrl);
    ret = p_gobj->load(info, map_flag);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        delete p_gobj;
        p_gobj = nullptr;
        id = 0;
        goto finish;
    }

    /* success */
    graphs[id] = p_gobj;
    p_gobj->get_graph_desc(gdesc);

finish:
    return ret;
}

aipu_status_t AIRT::MainContext::init()
{
    return ctrl.init(signal_handler);
}

aipu_status_t AIRT::MainContext::deinit()
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;

    if (!is_deinit_ok())
    {
        ret = AIPU_STATUS_ERROR_DEINIT_FAIL;
        goto finish;
    }

    force_deinit();

finish:
    return ret;
}

void AIRT::MainContext::force_deinit()
{
    GraphTable::iterator iter;
    for (iter = graphs.begin(); iter != graphs.end(); iter++)
    {
        iter->second->unload();
    }
    graphs.clear();
    ctrl.deinit();
}

aipu_status_t AIRT::MainContext::config_simulation(const aipu_simulation_config_t* config)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;

    if (nullptr == config)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

#if (defined X86_LINUX) && (X86_LINUX==1)
    ret = ctrl.config_simulation(config);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }
#endif

finish:
    return ret;
}

void AIRT::MainContext::print_parse_result(const graph_info_t& info, const void* graph)
{
#if (defined PRINT_GRAPH_PARSING_INFO) && (PRINT_GRAPH_PARSING_INFO == 1)
    LOG(LOG_DEFAULT, "=====================Graph Parse Results====================");
    LOG(LOG_DEFAULT, "Target device: arch %u, version %u, configuration: %u",
        AIPU_ARCH(info.device),
        AIPU_VERSION(info.device),
        AIPU_CONFIG(info.device));
    LOG(LOG_DEFAULT, "Graph version: %u", info.version);
    LOG(LOG_DEFAULT, "Building tool version: major %u, minor %u, build number: %u",
        BUILD_MAJOR(info.build_version),
        BUILD_MINOR(info.build_version),
        BUILD_NUMBER(info.build_version));
    LOG(LOG_DEFAULT, "Graph fsize: %u", info.gbin_size);
    LOG(LOG_DEFAULT, "Entry: 0x%x", info.entry);
    LOG(LOG_DEFAULT, "Process buffer:");
    LOG(LOG_DEFAULT, "--Instruction: size 0x%x, va 0x%lx (offset 0x%lx)",
        info.pbuf_templ.inst_size, (unsigned long)info.pbuf_templ.inst_src,
        (unsigned long)info.pbuf_templ.inst_src - (unsigned long)graph);
    LOG(LOG_DEFAULT, "--Rodata: size 0x%x, va 0x%lx (offset 0x%lx)",
        info.tbuf_templ.rodata_size, (unsigned long)info.tbuf_templ.rodata_src,
        (unsigned long)info.tbuf_templ.rodata_src - (unsigned long)graph);
    LOG(LOG_DEFAULT, "--Stack: size 0x%x,", info.tbuf_templ.stack_size);
    LOG(LOG_DEFAULT, "--Weight data: size 0x%x, va 0x%lx (offset 0x%lx)",
        info.pbuf_templ.data_size, (unsigned long)info.pbuf_templ.data_src,
        (unsigned long)info.pbuf_templ.data_src - (unsigned long)graph);
    LOG(LOG_DEFAULT, "----in which, %u sections are:", (uint32_t)info.pbuf_templ.static_sections.size());
    for (uint32_t i = 0; i < info.pbuf_templ.static_sections.size(); i++)
    {
        LOG(LOG_DEFAULT, "----section %u: size 0x%x, va 0x%lx (offset = 0x%lx)",
            i, info.pbuf_templ.static_sections[i].size,
            (unsigned long)info.pbuf_templ.static_sections[i].load_src,
            (unsigned long)info.pbuf_templ.static_sections[i].load_src - (unsigned long)graph);
        for (uint32_t j = 0; j < info.pbuf_templ.static_sections[i].sub_sections.size(); j++)
        {
            LOG(LOG_CLOSE, "------sub-section %u: offset_in_section = 0x%x",
                j, info.pbuf_templ.static_sections[i].sub_sections[j].offset_in_section);
            for (uint32_t k = 0; k < info.param_map.size(); k++)
            {
                if ((info.param_map[k].load_type == PARAM_MAP_LOAD_TYPE_STATIC) &&
                    (info.param_map[k].ref_section_iter == i) &&
                    (info.param_map[k].ref_sub_section_iter == j))
                {
                    LOG(LOG_DEFAULT, "--------[section[%u] base pa + 0x%x] =w=> [rodata base pa + 0x%x]",
                        i, info.pbuf_templ.static_sections[i].sub_sections[j].offset_in_section,
                        info.param_map[k].offset_in_map);
                }
            }
        }
    }

    LOG(LOG_DEFAULT, "--Reuse data:");
    LOG(LOG_DEFAULT, "----in which, %u sections are:", (uint32_t)info.tbuf_templ.reuse_sections.size());
    for (uint32_t i = 0; i < info.tbuf_templ.reuse_sections.size(); i++)
    {
        LOG(LOG_DEFAULT, "----section %u: size 0x%x", i, info.tbuf_templ.reuse_sections[i].size);
        for (uint32_t j = 0; j < info.tbuf_templ.reuse_sections[i].sub_sections.size(); j++)
        {
            LOG(LOG_CLOSE, "------sub-section %u: offset_in_section = 0x%x",
                j, info.tbuf_templ.reuse_sections[i].sub_sections[j].offset_in_section);
            for (uint32_t k = 0; k < info.param_map.size(); k++)
            {
                if ((info.param_map[k].load_type == PARAM_MAP_LOAD_TYPE_REUSE) &&
                    (info.param_map[k].ref_section_iter == i) &&
                    (info.param_map[k].ref_sub_section_iter == j))
                {
                    LOG(LOG_DEFAULT, "--------[section[%u] base pa + 0x%x] =w=> [rodata base pa + 0x%x]",
                        i, info.tbuf_templ.reuse_sections[i].sub_sections[j].offset_in_section,
                        info.param_map[k].offset_in_map);
                    break;
                }
            }
        }
    }
    LOG(LOG_DEFAULT, "--Input(s) number: %u", (uint32_t)info.inputs.size());
    for (uint32_t i = 0; i < info.inputs.size(); i++)
    {
        LOG(LOG_DEFAULT, "---Input #%u:", i);
        LOG(LOG_DEFAULT, "------Tensor ID: %u", info.inputs[i].id);
        LOG(LOG_DEFAULT, "------Address: reuse_sections[%u] base pa + 0x%x",
            info.inputs[i].ref_section_iter, info.inputs[i].offset_in_section);
        LOG(LOG_DEFAULT, "------Size: 0x%x", info.inputs[i].size);
    }
    LOG(LOG_DEFAULT, "--Output(s) number: %u", (uint32_t)info.outputs.size());
    for (uint32_t i = 0; i < info.outputs.size(); i++)
    {
        LOG(LOG_DEFAULT, "---Output #%u:", i);
        LOG(LOG_DEFAULT, "------Tensor ID: %u", info.outputs[i].id);
        LOG(LOG_DEFAULT, "------Address: reuse_sections[%u] base pa + 0x%x",
            info.outputs[i].ref_section_iter, info.outputs[i].offset_in_section);
        LOG(LOG_DEFAULT, "------Size: 0x%x", info.outputs[i].size);
    }
    LOG(LOG_DEFAULT, "--Dump(s) number: %u", (uint32_t)info.dumps.size());
    for (uint32_t i = 0; i < info.dumps.size(); i++)
    {
        LOG(LOG_DEFAULT, "---Dump #%u:", i);
        LOG(LOG_DEFAULT, "------Tensor ID: %u", info.dumps[i].id);
        LOG(LOG_DEFAULT, "------Address: reuse_sections[%u] base pa + 0x%x",
            info.dumps[i].ref_section_iter, info.dumps[i].offset_in_section);
        LOG(LOG_DEFAULT, "------Size: 0x%x", info.dumps[i].size);
    }

    LOG(LOG_DEFAULT, "=====================Graph Parse Results====================");
#endif /* PRINT_GRAPH_PARSING_INFO */
}

aipu_status_t AIRT::MainContext::load_graph(const void* graph, uint32_t size, bool map_flag,
    aipu_graph_desc_t* gdesc)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    graph_info_t info;

    if (nullptr == gdesc)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    ret = parse_graph(graph, size, info);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }

    print_parse_result(info, graph);

    ret = create_graph_object(info, map_flag, gdesc);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }

finish:
    return ret;
}

aipu_status_t AIRT::MainContext::unload_graph(const aipu_graph_desc_t* gdesc)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    Graph* p_gobj = nullptr;

    if (nullptr == gdesc)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    if (0 == graphs.count(gdesc->id))
    {
        ret = AIPU_STATUS_ERROR_GRAPH_NOT_EXIST;
        goto finish;
    }
    else
    {
        p_gobj = graphs[gdesc->id];
    }

    if (!p_gobj->is_unload_ok())
    {
        ret = AIPU_STATUS_ERROR_INVALID_OP;
        goto finish;
    }

    ret = p_gobj->unload();
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }

    /* success */
    delete p_gobj;
    p_gobj = nullptr;
    graphs.erase(gdesc->id);

finish:
    return ret;
}

aipu_status_t AIRT::MainContext::alloc_tensor_buffers(const aipu_graph_desc_t* gdesc,
    aipu_buffer_alloc_info_t* info)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    Graph* p_gobj = nullptr;

    if ((nullptr == gdesc) || (nullptr == info))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    if (0 == graphs.count(gdesc->id))
    {
        ret = AIPU_STATUS_ERROR_GRAPH_NOT_EXIST;
        goto finish;
    }
    else
    {
        p_gobj = graphs[gdesc->id];
    }

    ret = p_gobj->alloc_thread_buffer(info);

finish:
    return ret;
}

aipu_status_t AIRT::MainContext::free_tensor_buffers(uint32_t handle)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    Graph* p_gobj = nullptr;
    uint32_t graph_id = Graph::handle2graph_id(handle);

    if (0 == graphs.count(graph_id))
    {
        ret = AIPU_STATUS_ERROR_INVALID_HANDLE;
        goto finish;
    }
    else
    {
        p_gobj = graphs[graph_id];
    }

    if(!p_gobj->is_buffer_exist(handle))
    {
        ret = AIPU_STATUS_ERROR_INVALID_HANDLE;
        goto finish;
    }

    if(!p_gobj->is_buffer_free(handle))
    {
        ret = AIPU_STATUS_ERROR_BUSY_HANDLE;
        goto finish;
    }

    ret = p_gobj->free_thread_buffer(handle);

finish:
    return ret;
}

aipu_status_t AIRT::MainContext::create_new_job(const aipu_graph_desc_t* gdesc,
    uint32_t handle, uint32_t* job_id)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    Graph* p_gobj = nullptr;

    if ((nullptr == gdesc) || (nullptr == job_id))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    if (0 == graphs.count(gdesc->id))
    {
        ret = AIPU_STATUS_ERROR_GRAPH_NOT_EXIST;
        goto finish;
    }
    else
    {
        p_gobj = graphs[gdesc->id];
    }

    /* buffer handle does not exist cannot be used */
    if(!p_gobj->is_buffer_exist(handle))
    {
        ret = AIPU_STATUS_ERROR_INVALID_HANDLE;
        goto finish;
    }

    /* busy buffer handle (under using by another job) cannot be used */
    if(!p_gobj->is_buffer_free(handle))
    {
        ret = AIPU_STATUS_ERROR_BUSY_HANDLE;
        goto finish;
    }

    ret = p_gobj->build_new_job(handle, job_id);

finish:
    return ret;
}

aipu_status_t AIRT::MainContext::flush_job(uint32_t job_id)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    Graph* p_gobj = nullptr;
    uint32_t graph_id = Graph::job_id2graph_id(job_id);

    if (0 == graphs.count(graph_id))
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
        goto finish;
    }
    else
    {
        p_gobj = graphs[graph_id];
    }

    if (!p_gobj->is_job_exist(job_id))
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
        goto finish;
    }

    /* a scheduled job cannot be scheduled again */
    if (!p_gobj->is_job_built(job_id))
    {
        ret = AIPU_STATUS_ERROR_JOB_SCHED;
        goto finish;
    }

    ret = p_gobj->flush_job(job_id);

finish:
    return ret;
}

aipu_status_t AIRT::MainContext::wait_for_job_end(uint32_t job_id, int32_t time_out, aipu_job_status_t* status)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    Graph* p_gobj = nullptr;
    uint32_t graph_id = Graph::job_id2graph_id(job_id);

    if (nullptr == status)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    if (0 == graphs.count(graph_id))
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
        goto finish;
    }
    else
    {
        p_gobj = graphs[graph_id];
    }

    /* waiting for results of an un-scheduled job is meaningless */
    if (p_gobj->is_job_built(job_id))
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_SCHED;
    }
    else if (!p_gobj->is_job_scheduled(job_id) &&
        !p_gobj->is_job_end(job_id))
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
    }
    else
    {
        ret = p_gobj->wait_for_job_end(job_id, time_out, status);
    }

finish:
    return ret;
}

aipu_status_t AIRT::MainContext::clean_job(uint32_t job_id)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    Graph* p_gobj = nullptr;
    uint32_t graph_id = Graph::job_id2graph_id(job_id);

    if (0 == graphs.count(graph_id))
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
        goto finish;
    }
    else
    {
        p_gobj = graphs[graph_id];
    }

    if (!p_gobj->is_job_exist(job_id))
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
        goto finish;
    }

    ret = p_gobj->clean_job(job_id);

finish:
    return ret;
}

aipu_status_t AIRT::MainContext::set_dump_options(uint32_t job_id, uint32_t flag, const char* dir)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    Graph* p_gobj = nullptr;
    uint32_t graph_id = Graph::job_id2graph_id(job_id);

    if (nullptr == dir)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    if (access(dir, W_OK) != 0)
    {
        ret = AIPU_STATUS_ERROR_INVALID_PATH;
        goto finish;
    }

    if (0 == graphs.count(graph_id))
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
        goto finish;
    }
    else
    {
        p_gobj = graphs[graph_id];
    }

    if (!p_gobj->is_job_exist(job_id))
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
        goto finish;
    }

    if ((p_gobj->is_job_scheduled(job_id)) || (p_gobj->is_job_end(job_id)))
    {
        ret = AIPU_STATUS_ERROR_JOB_SCHED;
        goto finish;
    }

    ret = p_gobj->set_dump_options(job_id, flag, dir);

finish:
    return ret;
}

aipu_status_t AIRT::MainContext::get_debug_info(uint32_t job_id, aipu_debug_info_t* info) const
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    uint32_t graph_id = Graph::job_id2graph_id(job_id);
    GraphTable::const_iterator g_iter = graphs.find(graph_id);

    if (nullptr == info)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    if (g_iter == graphs.end())
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
        goto finish;
    }

    if (!g_iter->second->is_job_exist(job_id))
    {
        ret = AIPU_STATUS_ERROR_JOB_NOT_EXIST;
        goto finish;
    }

    ret = g_iter->second->get_debug_info(job_id, info);

finish:
    return ret;
}

aipu_status_t AIRT::MainContext::get_status_msg(aipu_status_t status, const char** msg)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;

    if (nullptr == msg)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    *msg = umd_status_string[status];

finish:
    return ret;
}

aipu_status_t AIRT::MainContext::get_dev_status(uint32_t* value) const
{
    return ctrl.get_dev_status(value);
}

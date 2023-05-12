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
 * @file  device_ctrl.cpp
 * @brief AIPU User Mode Driver (UMD) device control module implementation
 */

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "device_ctrl.h"
#include "uk_interface/aipu_ioctl.h"
#include "uk_interface/aipu_errcode.h"
#include "uk_interface/aipu_signal.h"
#include "uk_interface/aipu_buf_req.h"
#include "uk_interface/aipu_job_desc.h"
#include "uk_interface/aipu_io_req.h"
#include "util/log.h"
#include "util/helper.h"

extern aipu_arch_t arch[AIPU_ARCH_CONFIG_MAX];

AIRT::ring_buf_t AIRT::DeviceCtrl::sched;
AIRT::umd_signal_handler        AIRT::DeviceCtrl::handler;
pthread_mutex_t AIRT::DeviceCtrl::glock = PTHREAD_MUTEX_INITIALIZER;
#if (defined X86_LINUX) && (X86_LINUX==1)
std::map<uint32_t, aipu_arch_t> AIRT::DeviceCtrl::aipu_arch;
std::map<uint32_t, AIRT::simulation_res_t> AIRT::DeviceCtrl::graphs;
#endif

AIRT::DeviceCtrl::DeviceCtrl()
{
    pthread_mutex_init(&glock, NULL);
    sched.head = 0;
    sched.tail = 0;
#if (defined X86_LINUX) && (X86_LINUX==1)
    init_aipu_arch();
#endif
}

AIRT::DeviceCtrl::~DeviceCtrl()
{
}

#if (defined X86_LINUX) && (X86_LINUX==1)
aipu_status_t AIRT::DeviceCtrl::create_simulation_input_file(uint32_t graph_id, const buffer_desc_t& desc,
    char* fname, const char* interfix, uint32_t load_size)
{
    snprintf(fname, FNAME_LEN, "%s/Simulation_Graph0x%x_%s_Base0x%x_Size0x%x.bin",
        output_dir.c_str(), graph_id, interfix, desc.pa - host_aipu_shm_offset, load_size);
    return umd_dump_file_helper(fname, (const void*)desc.va, load_size);
}
#endif

#if (defined X86_LINUX) && (X86_LINUX==1)
uint32_t AIRT::DeviceCtrl::get_sim_data_size(const pbuf_alloc_templ_t& pbuf_templ,
    const tbuf_alloc_templ_t& tbuf_templ) const
{
    uint32_t tot_size = tbuf_templ.stack_size;
    for (uint32_t stensor_iter = 0; stensor_iter < pbuf_templ.static_sections.size(); stensor_iter++)
    {
        tot_size += ALIGN_PAGE(pbuf_templ.static_sections[stensor_iter].size);
    }
    for (uint32_t reuse_iter = 0; reuse_iter < tbuf_templ.reuse_sections.size(); reuse_iter++)
    {
        tot_size += ALIGN_PAGE(tbuf_templ.reuse_sections[reuse_iter].size);
    }
    return tot_size;
}
#endif

#if (defined X86_LINUX) && (X86_LINUX==1)
aipu_status_t AIRT::DeviceCtrl::update_z1_simulation_sys_cfg(uint32_t config, FILE* fp) const
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    char cfg_item[1024];
    std::map<uint32_t, aipu_arch_t>::const_iterator iter = aipu_arch.find(config);
    if (iter == aipu_arch.end())
    {
        ret = AIPU_STATUS_ERROR_TARGET_NOT_FOUND;
        goto finish;
    }

    snprintf(cfg_item, sizeof(cfg_item), "SYS_INFO=%s\n", iter->second.sys_info);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "VREG_NUM=%u\n", iter->second.vreg_num);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "LSRAM_SIZE=%u\n", iter->second.lsram_size_kb);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "LSRAM_BANK=%u\n", iter->second.lsram_bank);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "GSRAM_SIZE=%u\n", iter->second.gsram_size_kb);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "GSRAM_BANK=%u\n", iter->second.gsram_bank);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "DMA_CHAN=%u\n", iter->second.dma_chan);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "HAS_IRAM=%s\n", iter->second.has_iram);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "HAS_DCACHE=%s\n", iter->second.has_dcache);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "ICACHE_SIZE=%u\n", iter->second.icache_size);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "HWA_MAC=%u\n", iter->second.hwa_mac);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "TEC_NUM=%u\n", iter->second.tec_num);
    fputs(cfg_item, fp);

finish:
    return ret;
}
#endif

#if (defined X86_LINUX) && (X86_LINUX==1)
aipu_status_t AIRT::DeviceCtrl::update_simulation_rtcfg_z1_arch(simulation_res_t& sim, const dev_config_t& config,
    FILE* fp, char* cfg_fname)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    char cfg_item[OPT_LEN];
    char inst_bin[FNAME_LEN];
    uint32_t min_pa_odata_base = 0xFFFFFFFF;
    void* min_pa_odata_va = nullptr;
    uint32_t max_pa_odata_base = 0;
    uint32_t max_pa_odata_size = 0;

    if ((fp == NULL) || (cfg_fname == NULL))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    ret = update_z1_simulation_sys_cfg(config.hw_config, fp);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }

    snprintf(inst_bin, sizeof(inst_bin), "%s",
        sim.code_fname);
    snprintf(cfg_item, sizeof(cfg_item), "instr_base=0x%x\n",
        config.code.instruction_base_pa);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "start_pc=0x%x\n",
        config.code.start_pc_pa);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "idata=%s@0x%x\n",
        sim.rodata_fname, config.rodata_base);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "idata2=%s@0x%x\n",
        sim.data_fname, config.stack_base);
    fputs(cfg_item, fp);

    for (uint32_t i = 0; i < sim.outputs.size(); i++)
    {
        if (sim.outputs[i].pa < min_pa_odata_base)
        {
            min_pa_odata_base = sim.outputs[i].pa;
            min_pa_odata_va = sim.outputs[i].va;
        }
        if (sim.outputs[i].pa > max_pa_odata_base)
        {
            max_pa_odata_base = sim.outputs[i].pa;
            max_pa_odata_size = sim.outputs[i].size;
        }
    }

    sim.odata_whole_pa = min_pa_odata_base;
    sim.odata_whole_va = min_pa_odata_va;
    sim.odata_whole_size = max_pa_odata_base + max_pa_odata_size - min_pa_odata_base;
    snprintf(sim.odata_fname, sizeof(sim.odata_fname),
        "%s/Simulation_Graph0x%x_Output_Base0x%x_Size0x%x.bin",
        output_dir.c_str(), sim.graph_id, sim.odata_whole_pa, sim.odata_whole_size);

    snprintf(cfg_item, sizeof(cfg_item), "odata=%s@0x%x\n",
        sim.odata_fname, sim.odata_whole_pa);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "odata_size=0x%x\n", sim.odata_whole_size);
    fputs(cfg_item, fp);
    fclose(fp);

    snprintf(simulation_cmd, sizeof(simulation_cmd), "./%s %s --sys_config=%s",
        simulator.c_str(), inst_bin, cfg_fname);

finish:
    return ret;
}
#endif

#if (defined X86_LINUX) && (X86_LINUX==1)
aipu_status_t AIRT::DeviceCtrl::update_simulation_rtcfg_z2_arch(simulation_res_t& sim, const dev_config_t& config,
    FILE* fp, char* cfg_fname)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    char cfg_item[OPT_LEN];

    if ((fp == NULL) || (cfg_fname == NULL))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    snprintf(cfg_item, sizeof(cfg_item), "LOG_LEVEL=0\n");
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "LOG_FILE=log_default\n");
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "FAST_FWD_INST=0\n");
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "INPUT_INST_CNT=1\n");
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "INPUT_INST_FILE0=%s\n", sim.code_fname);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "INPUT_INST_BASE0=0x%x\n", config.code.instruction_base_pa);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "INPUT_INST_STARTPC0=0x%x\n", config.code.start_pc_pa);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "INT_PC=0x%x\n", config.code.interrupt_pc_pa);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "INPUT_DATA_CNT=2\n");
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "INPUT_DATA_FILE0=%s\n", sim.rodata_fname);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "INPUT_DATA_BASE0=0x%x\n", config.rodata_base);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "INPUT_DATA_FILE1=%s\n", sim.data_fname);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "INPUT_DATA_BASE1=0x%x\n", config.stack_base);
    fputs(cfg_item, fp);
    snprintf(cfg_item, sizeof(cfg_item), "OUTPUT_DATA_CNT=%u\n", (uint32_t)sim.outputs.size());
    fputs(cfg_item, fp);
    for (uint32_t i = 0; i < sim.outputs.size(); i++)
    {
        snprintf(cfg_item, sizeof(cfg_item), "OUTPUT_DATA_FILE%u=%s\n", i, sim.outputs[i].fname);
        fputs(cfg_item, fp);
        snprintf(cfg_item, sizeof(cfg_item), "OUTPUT_DATA_BASE%u=0x%x\n", i, sim.outputs[i].pa);
        fputs(cfg_item, fp);
        snprintf(cfg_item, sizeof(cfg_item), "OUTPUT_DATA_SIZE%u=0x%x\n", i, sim.outputs[i].size);
        fputs(cfg_item, fp);
    }
    snprintf(cfg_item, sizeof(cfg_item), "RUN_DESCRIPTOR=BIN[0]\n");
    fputs(cfg_item, fp);
    fclose(fp);

    snprintf(simulation_cmd, sizeof(simulation_cmd), "./%s %s", simulator.c_str(), cfg_fname);

finish:
    return ret;
}
#endif

#if (defined X86_LINUX) && (X86_LINUX==1)
aipu_status_t AIRT::DeviceCtrl::update_simulation_rtcfg(uint32_t graph_id, const dev_config_t& config)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    FILE* fp = NULL;
    char cfg_fname[512];
    std::map<uint32_t, simulation_res_t>::iterator giter = graphs.find(graph_id);

    if (graphs.end() == giter)
    {
        ret = AIPU_STATUS_ERROR_GRAPH_NOT_EXIST;
        goto finish;
    }

    /* create rodata file */
    ret = create_simulation_input_file(graph_id, giter->second.rodata, giter->second.rodata_fname,
        "Idata0", giter->second.rodata.size);
    if (ret != AIPU_STATUS_SUCCESS)
    {
        goto finish;
    }

    /* create data file */
    ret = create_simulation_input_file(graph_id, giter->second.data, giter->second.data_fname,
        "Idata1", giter->second.data.size);
    if (ret != AIPU_STATUS_SUCCESS)
    {
        goto finish;
    }

    /* create config file */
    snprintf(cfg_fname, sizeof(cfg_fname), "%sarch.cfg", cfg_file_dir.c_str());
    fp = fopen(cfg_fname, "w");
    if (NULL == fp)
    {
        ret = AIPU_STATUS_ERROR_OPEN_FILE_FAIL;
        LOG(LOG_ERR, "Create config file failed: %s!", cfg_fname);
        goto finish;
    }

    if ((config.arch == 0) && (config.hw_version == 1))
    {
        ret = update_simulation_rtcfg_z1_arch(giter->second, config, fp, cfg_fname);
    }
    else if ((config.arch == 0) && (config.hw_version == 2))
    {
        ret = update_simulation_rtcfg_z2_arch(giter->second, config, fp, cfg_fname);
    }
    else
    {
        ret = AIPU_STATUS_ERROR_TARGET_NOT_FOUND;
    }

finish:
    return ret;
}
#endif

#if (defined X86_LINUX) && (X86_LINUX==1)
void AIRT::DeviceCtrl::init_aipu_arch()
{
    aipu_arch[AIPU_CONFIG_0904] = arch[0];
    aipu_arch[AIPU_CONFIG_1002] = arch[1];
    aipu_arch[AIPU_CONFIG_0701] = arch[2];
}
#endif

#if (defined X86_LINUX) && (X86_LINUX==1)
aipu_status_t AIRT::DeviceCtrl::config_simulation(const aipu_simulation_config_t* config)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    int sys_ret = 0;

    if ((nullptr == config) ||
        (nullptr == config->simulator) ||
        (nullptr == config->cfg_file_dir) ||
        (nullptr == config->output_dir))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    sys_ret = access(config->simulator, X_OK) |
              access(config->cfg_file_dir, W_OK) |
              access(config->output_dir, W_OK);
    if (sys_ret != 0)
    {
        ret = AIPU_STATUS_ERROR_INVALID_PATH;
        goto finish;
    }

    simulator = config->simulator;
    cfg_file_dir = config->cfg_file_dir;
    output_dir = config->output_dir;

finish:
    return ret;
}
#endif

#if (defined X86_LINUX) && (X86_LINUX==1)
aipu_status_t AIRT::DeviceCtrl::simulation_alloc_data_buffer(uint32_t graph_id, const pbuf_alloc_templ_t& pbuf_templ,
    pbuf_info_t& pbuf, const tbuf_alloc_templ_t& tbuf_templ, tbuf_info_t& tbuf)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    uint32_t offset = 0;
    buffer_desc_t buf;
    std::map<uint32_t, simulation_res_t>::iterator giter = graphs.find(graph_id);
    if (giter == graphs.end())
    {
        ret = AIPU_STATUS_ERROR_GRAPH_NOT_EXIST;
        goto finish;
    }

    /* rodata */
    ret = malloc_buf(tbuf_templ.rodata_size, 1, &giter->second.rodata);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }
    tbuf.rodata = giter->second.rodata;

    /* data */
    ret = malloc_buf(get_sim_data_size(pbuf_templ, tbuf_templ), 1, &giter->second.data);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }

    /* stack */
    tbuf.stack.va = giter->second.data.va;
    tbuf.stack.pa = giter->second.data.pa;
    tbuf.stack.size = ALIGN_PAGE(tbuf_templ.stack_size);
    tbuf.stack.real_size = tbuf_templ.stack_size;
    /* static data */
    offset = tbuf.stack.size;
    for (uint32_t stensor_iter = 0; stensor_iter < pbuf_templ.static_sections.size(); stensor_iter++)
    {
        buf.va = (void*)((unsigned long)giter->second.data.va + offset);
        buf.pa = giter->second.data.pa + offset;
        buf.size = ALIGN_PAGE(pbuf_templ.static_sections[stensor_iter].size);
        buf.real_size = pbuf_templ.static_sections[stensor_iter].size;
        pbuf.static_buf.push_back(buf);
        offset += buf.size;
    }
    /* reuse data */
    for (uint32_t reuse_iter = 0; reuse_iter < tbuf_templ.reuse_sections.size(); reuse_iter++)
    {
        buf.va = (void*)((unsigned long)giter->second.data.va + offset);
        buf.pa = giter->second.data.pa + offset;
        buf.size = ALIGN_PAGE(tbuf_templ.reuse_sections[reuse_iter].size);
        buf.real_size = tbuf_templ.reuse_sections[reuse_iter].size;
        tbuf.reuse_buf.push_back(buf);
        offset += buf.size;
    }

finish:
    return ret;
}
#endif

#if (defined X86_LINUX) && (X86_LINUX==1)
aipu_status_t AIRT::DeviceCtrl::simulation_free_data_buffer(uint32_t graph_id)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    std::map<uint32_t, simulation_res_t>::iterator giter = graphs.find(graph_id);

    if (graphs.end() == giter)
    {
        ret = AIPU_STATUS_ERROR_GRAPH_NOT_EXIST;
        goto finish;
    }

    if (nullptr == giter->second.data.va)
    {
        goto finish;
    }

    ret = free_buf(&giter->second.data);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }

    giter->second.data.va = nullptr;
    giter->second.data.pa = 0;
    giter->second.data.size = 0;

finish:
    return ret;
}
#endif

#if (defined X86_LINUX) && (X86_LINUX==1)
aipu_status_t AIRT::DeviceCtrl::simulation_set_io_info(uint32_t graph_id, const iobuf_info_t& iobuf)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    std::map<uint32_t, simulation_res_t>::iterator giter = graphs.find(graph_id);
    output_file_desc_t output;

    if (graphs.end() == giter)
    {
        ret = AIPU_STATUS_ERROR_GRAPH_NOT_EXIST;
        goto finish;
    }

    for (uint32_t i = 0; i < iobuf.outputs.number; i++)
    {
        output.id = iobuf.outputs.tensors[i].id;
        output.size = iobuf.outputs.tensors[i].size;
        output.va = iobuf.outputs.tensors[i].va;
        output.pa = iobuf.outputs.pa[i];
        snprintf(output.fname, sizeof(output.fname),
            "%s/Simulation_Graph0x%x_Output%u_Base0x%x_Size0x%x.bin",
            output_dir.c_str(), graph_id, output.id, output.pa, output.size);
        giter->second.outputs.push_back(output);
    }

finish:
    return ret;
}
#endif

aipu_status_t AIRT::DeviceCtrl::init(umd_signal_handler sig_handler)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
#if (defined X86_LINUX) && (X86_LINUX==1)
    simulation_malloc_top = 0;
#endif
    handler = sig_handler;
    new_sa.sa_sigaction = aipu_interrupt_hdl;
    sigemptyset(&new_sa.sa_mask);
    new_sa.sa_flags = SA_SIGINFO;
    new_sa.sa_restorer = NULL;
    sigaction(AIPU_SIG_DONE, &new_sa, NULL);
    sigaction(AIPU_SIG_EXCEPTION, &new_sa, NULL);
#if (defined ARM_LINUX) && (ARM_LINUX==1)
    int kern_ret = 0;
    aipu_cap_t cap;
    fd = open("/dev/aipu", O_RDWR | O_SYNC);
    if (fd <= 0)
    {
        ret = AIPU_STATUS_ERROR_OPEN_DEV_FAIL;
        goto finish;
    }
    kern_ret = ioctl(fd, IPUIOC_REQSHMMAP, &host_aipu_shm_offset);
    if (kern_ret != 0)
    {
        LOG(LOG_ERR, "get host-aipu shared mem offset failed!");
        ret = AIPU_STATUS_ERROR_OPEN_DEV_FAIL;
        goto finish;
    }
    kern_ret = ioctl(fd, IPUIOC_QUERYCAP, &cap);
    if (kern_ret != 0)
    {
        LOG(LOG_ERR, "get AIPU hardware info. failed!");
        ret = AIPU_STATUS_ERROR_OPEN_DEV_FAIL;
        goto finish;
    }
    aipu_arch = 0;    /* ZHOUYI */
    aipu_version = 1; /* Z1 */
    if (AIPU_HWA_FEATURE_0904 == cap.hwa_feature)
    {
        aipu_hw_config = AIPU_CONFIG_0904;
    }
    else if (AIPU_HWA_FEATURE_1002 == cap.hwa_feature)
    {
        aipu_hw_config = AIPU_CONFIG_1002;
    }
    else if (AIPU_HWA_FEATURE_0701 == cap.hwa_feature)
    {
        aipu_hw_config = AIPU_CONFIG_0701;
    }
    else
    {
        ret = AIPU_STATUS_ERROR_DEV_ABNORMAL;
        goto finish;
    }
#else
    fd = 0;
    host_aipu_shm_offset = 0;
    goto finish;
#endif

finish:
    return ret;
}

aipu_status_t AIRT::DeviceCtrl::deinit()
{
    if (fd > 0)
    {
        close(fd);
        fd = 0;
    }
    return AIPU_STATUS_SUCCESS;
}

void AIRT::DeviceCtrl::unload_graph(uint32_t graph_id)
{
#if (defined X86_LINUX) && (X86_LINUX==1)
    graphs.erase(graph_id);
#endif
}

uint32_t AIRT::DeviceCtrl::get_shm_offset() const
{
    return host_aipu_shm_offset;
}

aipu_status_t AIRT::DeviceCtrl::malloc_buf(uint32_t size, uint32_t align, buffer_desc_t* buf)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    buf_request_t buf_req;
    buf_req.size = size;
    buf_req.align_in_page = align;
    buf_req.errcode = AIPU_ERRCODE_NO_ERROR;
    void* ptr = nullptr;
#if (defined ARM_LINUX) && (ARM_LINUX==1)
    int kern_ret = AIPU_ERRCODE_NO_ERROR;
#endif

    if (nullptr == buf)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    if (0 == size)
    {
        ret = AIPU_STATUS_ERROR_INVALID_SIZE;
        goto finish;
    }

#if (defined ARM_LINUX) && (ARM_LINUX==1)
    kern_ret = ioctl(fd, IPUIOC_REQBUF, &buf_req);
    if ((kern_ret != 0) || (buf_req.errcode != AIPU_ERRCODE_NO_ERROR))
    {
        ret = AIPU_STATUS_ERROR_BUF_ALLOC_FAIL;
        LOG(LOG_ERR, "buffer req failed! (errno = %d, errcode = %d)", errno, buf_req.errcode);
        goto finish;
    }
    LOG(LOG_CLOSE, "Successfully request buffer: addr 0x%x, dev_offset 0x%x, size 0x%x.",
        buf_req.desc.phys_addr, buf_req.desc.dev_offset, buf_req.desc.align_size);

    ptr = mmap(NULL, buf_req.desc.align_size, PROT_READ | PROT_WRITE, MAP_SHARED,
        fd, buf_req.desc.dev_offset);
    if (ptr == MAP_FAILED)
    {
        ret = AIPU_STATUS_ERROR_BUF_ALLOC_FAIL;
        LOG(LOG_ERR, "mmap buffer failed! (errno = %d, fd = %d, tot size = 0x%x, offset = 0x%x)",
            errno, fd, buf_req.desc.align_size, buf_req.desc.dev_offset);
        goto finish;
    }
#else
    buf_req.desc.phys_addr  = simulation_malloc_top;
    buf_req.desc.align_size = ALIGN_PAGE(buf_req.size);
    ptr = new char[buf_req.desc.align_size];
    simulation_malloc_top += buf_req.desc.align_size;
#endif

    buf->pa = buf_req.desc.phys_addr;
    buf->va = ptr;
    buf->size = buf_req.desc.align_size;
    buf->real_size = buf_req.size;

finish:
    return ret;
}

aipu_status_t AIRT::DeviceCtrl::free_buf(const buffer_desc_t* buf)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
#if (defined ARM_LINUX) && (ARM_LINUX==1)
    buf_desc_t desc;
    int kern_ret = AIPU_ERRCODE_NO_ERROR;
#endif

    if (nullptr == buf)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    if (0 == buf->size)
    {
        ret = AIPU_STATUS_ERROR_INVALID_SIZE;
        goto finish;
    }

#if (defined ARM_LINUX) && (ARM_LINUX==1)
    desc.phys_addr = buf->pa;
    desc.align_size = buf->size;
    munmap((void*)buf->va, buf->size);
    kern_ret = ioctl(fd, IPUIOC_FREEBUF, &desc);
    if (kern_ret != 0)
    {
        LOG(LOG_ERR, "free buffer ioctl failed! (errno = %d)", errno);
        ret = AIPU_STATUS_ERROR_BUF_FREE_FAIL;
        goto finish;
    }
#else
    if (nullptr != buf->va)
    {
        delete[] (char*)buf->va;
    }
#endif

    LOG(LOG_CLOSE, "buffer is freed: addr 0x%x, size 0x%x", buf->pa, buf->size);

finish:
    return ret;
}

aipu_status_t AIRT::DeviceCtrl::alloc_inst_buffer(uint32_t graph_id, const pbuf_alloc_templ_t& pbuf_templ,
    buffer_desc_t& ibuf_desc)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;

    if (0 == pbuf_templ.inst_size)
    {
        ret = AIPU_STATUS_ERROR_INVALID_SIZE;
        goto finish;
    }

#if (defined ARM_LINUX) && (ARM_LINUX==1)
    ret = malloc_buf(pbuf_templ.inst_size, 1, &ibuf_desc);
#else
    if (graphs.count(graph_id) == 1)
    {
        ret = AIPU_STATUS_ERROR_INVALID_OP;
        goto finish;
    }
    /* reuse code section buffer in virtual space */
    ibuf_desc.va = (void*)pbuf_templ.inst_src;
    ibuf_desc.pa = simulation_malloc_top;
    ibuf_desc.size = ALIGN_PAGE(pbuf_templ.inst_size);
    ibuf_desc.real_size = pbuf_templ.inst_size;
    simulation_malloc_top += ibuf_desc.size;
#endif

finish:
    return ret;
}

void AIRT::DeviceCtrl::load_buffer(volatile void* dest, const void* src, uint32_t bytes)
{
    for (uint32_t i = 0; i < bytes; i++)
    {
        *(volatile char*)((unsigned long)dest + i) = *(const char*)((unsigned long)src + i);
    }
}

aipu_status_t AIRT::DeviceCtrl::load_inst_buffer(uint32_t graph_id, const void* src, uint32_t size,
    buffer_desc_t& ibuf_desc)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
#if (defined X86_LINUX) && (X86_LINUX==1)
    simulation_res_t sim;
#endif

    if ((nullptr == src) || (nullptr == ibuf_desc.va))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    if ((0 == size) || (0 == ibuf_desc.size))
    {
        ret = AIPU_STATUS_ERROR_INVALID_SIZE;
        goto finish;
    }

#if (defined X86_LINUX) && (X86_LINUX==1)
    if (graphs.count(graph_id) == 1)
    {
        ret = AIPU_STATUS_ERROR_INVALID_OP;
        goto finish;
    }

    ret = create_simulation_input_file(graph_id, ibuf_desc, sim.code_fname, "Bin", size);
    if (AIPU_STATUS_SUCCESS != ret)
    {
        goto finish;
    }

    sim.graph_id = graph_id;
    graphs[graph_id] = sim;
#else
    load_buffer(ibuf_desc.va, src, size);
#endif

finish:
    return ret;
}

aipu_status_t AIRT::DeviceCtrl::free_inst_buffer(buffer_desc_t& ibuf_desc)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
#if (defined ARM_LINUX) && (ARM_LINUX==1)
    ret = free_buf(&ibuf_desc);
#endif
    return ret;
}

aipu_status_t AIRT::DeviceCtrl::schedule_job_on_aipu(uint32_t graph_id, uint32_t job_id, const dev_config_t& config,
    void* job_ctx)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    int kern_ret = 0;
    sched_graph_t graph;
#if (defined ARM_LINUX) && (ARM_LINUX==1)
    user_job_t job;
#else
    std::map<uint32_t, simulation_res_t>::iterator giter = graphs.find(graph_id);
#endif

    graph.id = graph_id;
    graph.argv = job_ctx;
    graph.tid = (uint32_t)syscall(__NR_gettid);

    pthread_mutex_lock(&glock);
    LOG(LOG_INFO, "[ %u ] SCHEDULER: set user ctx to tail: %u.",
        graph.tid, sched.tail);
    sched.graphs[sched.tail] = graph;
    if ((++sched.tail) == MAX_SCHED_JOB_NUM)
    {
        sched.tail = 0;
    }
    if (sched.tail == sched.head)
    {
        LOG(LOG_ERR, "[UMD ERROR] scheduled job queue overflow!");
    }
    LOG(LOG_INFO, "[ %u ] SCHEDULER: head: %u, tail: %u.",
        graph.tid, sched.head, sched.tail);

#if (defined ARM_LINUX) && (ARM_LINUX==1)
    if (fd <= 0)
    {
        ret = AIPU_STATUS_ERROR_OPEN_DEV_FAIL;
        goto finish;
    }

    job.desc.job_id = job_id;
    job.desc.start_pc_addr = config.code.start_pc_pa;
    job.desc.intr_handler_addr = config.code.interrupt_pc_pa;
    job.desc.data_0_addr = config.rodata_base;
    job.desc.data_1_addr = config.stack_base;
    job.desc.enable_prof = false;
    job.errcode = AIPU_ERRCODE_NO_ERROR;
    kern_ret = ioctl(fd, IPUIOC_RUNJOB, &job);
    if ((kern_ret != 0) || (job.errcode != AIPU_ERRCODE_NO_ERROR))
    {
        LOG(LOG_ERR, "load aipu job descriptor to KMD failed! (errcode = %d)", job.errcode);
        goto finish;
    }
#endif
    pthread_mutex_unlock(&glock);

#if (defined X86_LINUX) && (X86_LINUX==1)
    if (graphs.end() == giter)
    {
        ret = AIPU_STATUS_ERROR_GRAPH_NOT_EXIST;
        goto finish;
    }

    ret = update_simulation_rtcfg(graph_id, config);
    if (ret != AIPU_STATUS_SUCCESS)
    {
        goto error;
    }

    LOG(LOG_DEFAULT, "%s", simulation_cmd);
    kern_ret = system(simulation_cmd);
    if (kern_ret == -1)
    {
        LOG(LOG_ERR, "Simulation execution failed!");
        goto error;
    }
    else if (WIFSIGNALED(kern_ret))
    {
        LOG(LOG_ERR, "Simulation terminated by signal %d!", WTERMSIG(kern_ret));
        goto error;
    }

    if ((config.arch == 0) && (config.hw_version == 1))
    {
        if (0 != umd_load_file_helper(giter->second.odata_fname,
            giter->second.odata_whole_va, giter->second.odata_whole_size))
        {
            goto error;
        }
    }
    else if ((config.arch == 0) && (config.hw_version == 2))
    {
        for (uint32_t i = 0; i < giter->second.outputs.size(); i++)
        {
            if (0 != umd_load_file_helper(giter->second.outputs[i].fname,
                giter->second.outputs[i].va, giter->second.outputs[i].size))
            {
                goto error;
            }
        }
    }

    aipu_interrupt_hdl(AIPU_SIG_DONE, nullptr, nullptr);
    LOG(LOG_INFO, "Simulation end.");
    goto finish;

error:
    aipu_interrupt_hdl(AIPU_SIG_EXCEPTION, nullptr, nullptr);
#endif

finish:
    return ret;
}

void AIRT::DeviceCtrl::aipu_interrupt_hdl(int sig_num, siginfo_t* info, void* context)
{
    /* DEBUG: to be removed */
#if ((defined RTDEBUG) && (RTDEBUG==1))
    uint32_t tid = (uint32_t)syscall(__NR_gettid);
    LOG(LOG_INFO, "[ %u ] HANDLER: get user ctx from head: %u.",
        tid, sched.head);
#endif

    if (sched.head == sched.tail)
    {
        LOG(LOG_ERR, "[UMD ERROR] scheduled job queue overflow!");
        AIRT::DeviceCtrl::handler(sched.graphs[sched.head].argv, AIPU_SIG_EXCEPTION);
    }
    else
    {
        LOG(LOG_INFO, "[ %u ] HANDLER: sig received, ctx %u, graph id 0x%x, sig 0x%x.",
            tid, sched.graphs[sched.head].tid, sched.graphs[sched.head].id, sig_num);
        AIRT::DeviceCtrl::handler(sched.graphs[sched.head].argv, sig_num);
    }

    if ((++sched.head) == MAX_SCHED_JOB_NUM)
    {
        sched.head = 0;
    }
    LOG(LOG_INFO, "[ %u ] HANDLER: head: %u, tail: %u.",
        tid, sched.head, sched.tail);
}

aipu_status_t AIRT::DeviceCtrl::get_dev_status(uint32_t* value) const
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
#if (defined ARM_LINUX) && (ARM_LINUX==1)
    aipu_io_req_t io_req;
    int kern_ret = 0;

    if (nullptr == value)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    io_req.offset = 0x4;
    io_req.rw = AIPU_IO_READ;
    kern_ret = ioctl(fd, IPUIOC_REQIO, &io_req);
    if ((kern_ret != 0) || (io_req.errcode != AIPU_ERRCODE_NO_ERROR))
    {
        LOG(LOG_ERR, "get AIPU status register value failed! (errcode = %d)", io_req.errcode);
        goto finish;
    }

    /* success */
    *value = io_req.value;
#else
    ret = AIPU_STATUS_ERROR_OP_NOT_SUPPORTED;
    goto finish;
#endif

finish:
    return ret;
}

bool AIRT::DeviceCtrl::match_target_dev(uint32_t arch, uint32_t version, uint32_t hw_config) const
{
#if (defined ARM_LINUX) && (ARM_LINUX==1)
    return (arch == aipu_arch) && (version == aipu_version) && (hw_config == aipu_hw_config);
#else
    return true;
#endif
}

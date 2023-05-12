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
 * @file  standard_api_impl.cpp
 * @brief AIPU User Mode Driver (UMD) Standard API implementation file
 */

#include <stdlib.h>
#include "standard_api_v2.0.0.h"
#include "context/ctx_ref_map.h"
#include "util/helper.h"
#include "util/log.h"

aipu_status_t AIPU_get_status_msg(aipu_status_t status, const char** msg)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    if (nullptr == msg)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    ret = AIRT::MainContext::get_status_msg(status, msg);

finish:
    return ret;
}

aipu_status_t AIPU_init_ctx(aipu_ctx_handle_t** ctx)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;
    aipu_ctx_handle_t* ctx_handle = nullptr;
    uint32_t handle = 0;

    if (nullptr == ctx)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    handle = ctx_map.create_ctx_ref();
    p_ctx = ctx_map.get_ctx_ref(handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->init();
        if (AIPU_STATUS_SUCCESS != ret)
        {
            ctx_map.destroy_ctx_ref(handle);
        }
        else
        {
            /* success */
            ctx_handle = new aipu_ctx_handle_t;
            ctx_handle->handle = handle;
            *ctx = ctx_handle;
        }
    }

finish:
    return ret;
}

aipu_status_t AIPU_deinit_ctx(const aipu_ctx_handle_t* ctx)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;

    if (nullptr == ctx)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->deinit();
        if (AIPU_STATUS_SUCCESS == ret)
        {
            ctx_map.destroy_ctx_ref(ctx->handle);
            delete ctx;
        }
    }

finish:
    return ret;
}

aipu_status_t AIPU_config_simulation(const aipu_ctx_handle_t* ctx, const aipu_simulation_config_t* config)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;

    if ((nullptr == ctx) || (nullptr == config))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->config_simulation(config);
    }

finish:
    return ret;
}

aipu_status_t AIPU_load_graph(const aipu_ctx_handle_t* ctx, const void* graph,
    uint32_t size, aipu_graph_desc_t* gdesc)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;

    if ((nullptr == ctx) || (nullptr == graph) || (nullptr == gdesc))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->load_graph(graph, size, 0, gdesc);
    }

finish:
    return ret;
}

aipu_status_t AIPU_load_graph_helper(const aipu_ctx_handle_t* ctx, const char* graph_file,
    aipu_graph_desc_t* gdesc)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;
    void* graph = nullptr;
    uint32_t gbin_size = 0;

    if ((nullptr == ctx) || (nullptr == graph_file) || (nullptr == gdesc))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = umd_mmap_file_helper(graph_file, &graph, &gbin_size);
        if (AIPU_STATUS_SUCCESS != ret)
        {
            goto finish;
        }
        ret = p_ctx->load_graph(graph, gbin_size, 1, gdesc);
    }

finish:
    return ret;
}

aipu_status_t AIPU_unload_graph(const aipu_ctx_handle_t* ctx, const aipu_graph_desc_t* gdesc)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;

    if ((nullptr == ctx) || (nullptr == gdesc))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->unload_graph(gdesc);
    }

finish:
    return ret;
}

aipu_status_t AIPU_alloc_tensor_buffers(const aipu_ctx_handle_t* ctx, const aipu_graph_desc_t* gdesc,
    aipu_buffer_alloc_info_t* info)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;

    if ((nullptr == ctx) || (nullptr == info) || (nullptr == gdesc))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->alloc_tensor_buffers(gdesc, info);
    }

finish:
    return ret;
}

aipu_status_t AIPU_free_tensor_buffers(const aipu_ctx_handle_t* ctx, uint32_t handle)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;

    if (nullptr == ctx)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->free_tensor_buffers(handle);
    }

finish:
    return ret;
}

aipu_status_t AIPU_create_job(const aipu_ctx_handle_t* ctx, const aipu_graph_desc_t* gdesc,
    uint32_t buf_handle, uint32_t* job_id)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;

    if ((nullptr == ctx) || (nullptr == job_id) || (nullptr == gdesc))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->create_new_job(gdesc, buf_handle, job_id);
    }

finish:
    return ret;
}

aipu_status_t AIPU_flush_job(const aipu_ctx_handle_t* ctx, uint32_t id)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;

    if (nullptr == ctx)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->flush_job(id);
    }

finish:
    return ret;
}

aipu_status_t AIPU_get_job_status(const aipu_ctx_handle_t* ctx, uint32_t id, int32_t time_out,
    aipu_job_status_t* status)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;

    if ((nullptr == ctx) || (nullptr == status))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->wait_for_job_end(id, time_out, status);
    }

finish:
    return ret;
}

aipu_status_t AIPU_finish_job(const aipu_ctx_handle_t* ctx, uint32_t id, int32_t time_out)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;
    aipu_job_status_t status;

    if (nullptr == ctx)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->flush_job(id);
        if (AIPU_STATUS_SUCCESS != ret)
        {
            goto finish;
        }
        ret = p_ctx->wait_for_job_end(id, time_out, &status);
        if ((ret == AIPU_STATUS_SUCCESS) && (status != AIPU_JOB_STATUS_DONE))
        {
            ret = AIPU_STATUS_ERROR_JOB_EXCEPTION;
        }
    }

finish:
    return ret;
}

aipu_status_t AIPU_clean_job(const aipu_ctx_handle_t* ctx, uint32_t id)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;

    if (nullptr == ctx)
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->clean_job(id);
    }

finish:
    return ret;
}

aipu_status_t AIPU_set_dump_options(const aipu_ctx_handle_t* ctx, uint32_t job_id,
    uint32_t flag, const char* dir)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;

    if ((nullptr == ctx) || (nullptr == dir))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->set_dump_options(job_id, flag, dir);
    }

finish:
    return ret;
}

aipu_status_t AIPU_get_debug_info(const aipu_ctx_handle_t* ctx, uint32_t job_id,
    aipu_debug_info_t* info)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;

    if ((nullptr == ctx) || (nullptr == info))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->get_debug_info(job_id, info);
    }

finish:
    return ret;
}

aipu_status_t AIPU_get_dev_status(const aipu_ctx_handle_t* ctx, uint32_t* value)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    AIRT::CtxRefMap& ctx_map = AIRT::CtxRefMap::get_ctx_map();
    AIRT::MainContext* p_ctx = nullptr;

    if ((nullptr == ctx) || (nullptr == value))
    {
        ret = AIPU_STATUS_ERROR_NULL_PTR;
        goto finish;
    }

    p_ctx = ctx_map.get_ctx_ref(ctx->handle);
    if (nullptr == p_ctx)
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    else
    {
        ret = p_ctx->get_dev_status(value);
    }

finish:
    return ret;
}
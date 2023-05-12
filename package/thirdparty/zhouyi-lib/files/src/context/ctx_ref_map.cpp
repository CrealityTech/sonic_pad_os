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
 * @file  ctx_ref_map.cpp
 * @brief AIPU User Mode Driver (UMD) context reference map module implementation
 */

#include "ctx_ref_map.h"

AIRT::CtxRefMap::CtxRefMap()
{
    data.clear();
    pthread_mutex_init(&lock, NULL);
}

AIRT::CtxRefMap::~CtxRefMap()
{
    std::map<uint32_t, MainContext*>::iterator iter;
    pthread_mutex_lock(&lock);
    for (iter = data.begin(); iter != data.end(); iter++)
    {
        iter->second->force_deinit();
        delete iter->second;
        iter->second = nullptr;
    }
    data.clear();
    pthread_mutex_unlock(&lock);
    pthread_mutex_destroy(&lock);
}

uint32_t AIRT::CtxRefMap::create_ctx_ref()
{
    uint32_t handle = 0xFFFFFFFF;

    pthread_mutex_lock(&lock);
    while(nullptr != get_ctx_ref_inner(handle))
    {
        handle--;
    }
    data[handle] = new MainContext;
    pthread_mutex_unlock(&lock);

    return handle;
}

AIRT::MainContext* AIRT::CtxRefMap::get_ctx_ref_inner(uint32_t handle)
{
    MainContext* ctx = nullptr;
    if (data.count(handle) == 1)
    {
        ctx = data[handle];
    }
    return ctx;
}

AIRT::MainContext* AIRT::CtxRefMap::get_ctx_ref(uint32_t handle)
{
    MainContext* ctx = nullptr;
    pthread_mutex_lock(&lock);
    ctx = get_ctx_ref_inner(handle);
    pthread_mutex_unlock(&lock);
    return ctx;
}

aipu_status_t AIRT::CtxRefMap::destroy_ctx_ref(uint32_t handle)
{
    aipu_status_t ret = AIPU_STATUS_SUCCESS;
    MainContext* ctx = nullptr;

    pthread_mutex_lock(&lock);
    ctx = get_ctx_ref_inner(handle);
    if (nullptr != ctx)
    {
        data.erase(handle);
    }
    else
    {
        ret = AIPU_STATUS_ERROR_INVALID_CTX;
    }
    pthread_mutex_unlock(&lock);

    return ret;
}
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
 * @file  ctx_ref_map.h
 * @brief AIPU User Mode Driver (UMD) context reference map module header
 */

#ifndef _CTX_REF_MAP_H_
#define _CTX_REF_MAP_H_

#include <map>
#include <pthread.h>
#include "standard_api_v2.0.0.h"
#include "context.h"

namespace AIRT
{
class CtxRefMap
{
private:
    std::map<uint32_t, MainContext*> data;
    pthread_mutex_t lock;

private:
    MainContext*  get_ctx_ref_inner(uint32_t handle);

public:
    uint32_t      create_ctx_ref();
    MainContext*  get_ctx_ref(uint32_t handle);
    aipu_status_t destroy_ctx_ref(uint32_t handle);

public:
    static CtxRefMap& get_ctx_map()
    {
        static CtxRefMap ctxmap;
        return ctxmap;
    }
    CtxRefMap(const CtxRefMap& ctx) = delete;
    CtxRefMap& operator=(const CtxRefMap& ctx) = delete;
    ~CtxRefMap();

private:
    CtxRefMap();
};
}

#endif /* _CTX_REF_MAP_H_ */
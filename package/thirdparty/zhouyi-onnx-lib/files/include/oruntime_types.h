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

#ifndef _ORUNTIME_TYPES_H_
#define _ORUNTIME_TYPES_H_

#include <stddef.h>
#include <cstdint>
#include <string>

#define QUANTTYPE double

enum ORT_STATUS {
    ORT_ERR_DEV_NOT_AVAILABLE = -14,
    ORT_ERR_CREATE_JOB        = -13,
    ORT_ERR_REGISTER_OP       = -12,
    ORT_ERR_RUNNING           = -11,
    ORT_ERR_INVALID_BIN       = -10,
    ORT_ERR_UNMAP_FAILED      = -9,
    ORT_ERR_MMAP_FAILED       = -8,
    ORT_ERR_OPEN_FILE         = -7,
    ORT_ERR_FILE_NOT_FOUND    = -6,
    ORT_ERR_INVALID_OPERATION = -5,
    ORT_ERR_NULLPTR           = -4,
    ORT_ERR_INIT_CTX          = -3,
    ORT_ERR_CONTEXT           = -2,
    ORT_ERR_PARAM             = -1,
    ORT_SUCCESS               = 0
};

enum class TensorFormat {
    ORT_TENSOR_NHWC
};

enum class TensorDataType : uint8_t {
    ORT_TENSOR_DATA_U8 = 0x1,
    ORT_TENSOR_DATA_F32 = 0x2
};

typedef struct ORTTensorShape {
    union {
        size_t data[4];
        struct {
            size_t n;
            size_t c;
            size_t h;
            size_t w;
        };
    };
    size_t dims;
} TensorShape;

#ifndef ORT_API
#define ORT_API __attribute__((visibility("default")))
#endif

#endif  // _ORUNTIME_TYPES_H_

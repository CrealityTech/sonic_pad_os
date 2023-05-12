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

#ifndef _ORUNTIME_CUSTOMOP_H_
#define _ORUNTIME_CUSTOMOP_H_

#include <iostream>
#include <string>

#include "oruntime_tensor.h"

namespace oruntime {

/**
 * @brief Custom OP all obey on one struct,
 */
class ORT_API ORTCustomOP {
public:
    /**
     * @brief Constructs a ORTCustomOP object with the given arguments.
     */
    explicit ORTCustomOP(const uint32_t id, const std::string name) : _id(id), _name(name) {}

    /**
     * Destorys the ORTCustomOP object.
     */
    virtual ~ORTCustomOP() {}

    /**
     * @brief This is the implement function of custom operations.
     *        If you want to implement custom op, you need to inherit class ORTCustomOP
     *        and implement op_body.
     */
    virtual bool op_body(ORTTENSORS& inputs, ORTTENSORS& outputs) {
        return false;
    }

    /**
     * @brief Return id of ORTCustomOP.
     */
    uint32_t id() {
        return _id;
    }

    /**
     * Return name of ORTCustomOP.
     */
    std::string name() {
        return _name;
    }

protected:
    ORTCustomOP() {}

private:
    uint32_t _id;
    std::string _name;
};

}  // namespace oruntime

#define ORT_COP_NAME(a) AIE_##a##_op
#define ORT_CUSTOMOP(name)                                                \
class ORT_COP_NAME(name)                                                  \
    : public oruntime::ORTCustomOP {                                           \
public:                                                                   \
    ORT_COP_NAME(name)(uint32_t id)                                       \
        : oruntime::ORTCustomOP(id, std::string(#name)) {}                     \
    virtual bool op_body(ORTTENSORS& inputs, ORTTENSORS& outputs);        \
};                                                                        \
bool ORT_COP_NAME(name)::op_body(ORTTENSORS& inputs, ORTTENSORS& outputs) \

#endif  // _ORUNTIME_CUSTOMOP_H_

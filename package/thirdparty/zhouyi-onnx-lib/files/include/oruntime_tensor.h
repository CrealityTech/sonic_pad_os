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

#ifndef _ORUNTIME_TENSOR_H_
#define _ORUNTIME_TENSOR_H_

#include <memory>
#include <vector>

#include "oruntime_types.h"

namespace oruntime {

class ORTTensorImpl;
class QuantOption;

class ORT_API ORTTensor {
public:
    /**
     * @brief Constructor, create an ORTTensor tensor.
     */
    ORT_API ORTTensor(const std::string& name,
                      const TensorShape& shape,
                      TensorFormat format = TensorFormat::ORT_TENSOR_NHWC,
                      TensorDataType type = TensorDataType::ORT_TENSOR_DATA_U8);

    /**
     * @brief Constructor, create an ORTTensor with tensor.
     */
    ORT_API ORTTensor(std::shared_ptr<ORTTensorImpl>& tensor);

    /**
     * @brief Destructor, release tensor resoureces.
     */
    ORT_API virtual ~ORTTensor();

    /**
     * @brief name Get the tensor name.
     * @return     Return the tensor name of type string.
     */
    std::string name() const;

    /**
     * @brief data Get the tensor data pointer.
     * @return     The data pointer.
     */
    void* data() const;

    /**
     * @brief shape        Get the tensor shape.
     * @return TensorShape Shape of the tensor.
     */
    TensorShape shape() const;

    /**
     * @brief format        Get the tensor format.
     * @return TensorFormat Foramt of the tensor.
     */
    TensorFormat format() const;

    /**
     * @brief quant_option Get the quantization parameter options.
     * @return The quantization option of the tensor.
     */
    QuantOption quant_option() const;

    /**
     * @brief data_type       Get the data type.
     * @return TensorDataType Data type(U8 etc.) of the tensor.
     */
    TensorDataType data_type() const;

    /**
     * @brief impl Get internal implementation object of tensor.
     * @return The internal implementation object.
     */
    std::shared_ptr<ORTTensorImpl> impl();

private:
    std::shared_ptr<ORTTensorImpl> m_tensor_impl;

public:
    ORTTensor() = delete;
};

}  // namespace oruntime

typedef std::vector<std::shared_ptr<oruntime::ORTTensor>> ORTTENSORS;

#endif  // _ORUNTIME_TENSOR_H_

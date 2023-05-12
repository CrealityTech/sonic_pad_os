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

#ifndef _ORUNTIME_QUANTIZE_H_
#define _ORUNTIME_QUANTIZE_H_

#include "oruntime_tensor.h"

namespace oruntime {

/**
 * @brief The QuantOption class defines a quantize parameters options.
 */
class QuantOption {
public:
    /**
     * @brief The default constructor.
     */
    QuantOption();

    /**
     * @brief Constructs a QuantOption object with the given arguments.
     * @param scale the parameter for quantization.
     * @param desc the description infomation for quantization.
     * @param type the quantization type.
     * @param available is the quantization option available.
     */
    QuantOption(QUANTTYPE scale, const std::string& desc, int type, bool available);

    /**
     * @brief Constructs a QuantOption object that is a copy of the QuantOption.
     */
    QuantOption(const QuantOption& option);

    /**
     * @brief Destorys the QuantOption object.
     */
    virtual ~QuantOption();

    /**
     * @brief Return the scale paramter for this option.
     */
    QUANTTYPE scale() const;

    /**
     * @brief Return the description information for this option.
     */
    std::string desc() const;

    /**
     * @brief Return the quantization type for this option.
     */
    int type() const;

    /**
     * @brief Return whether this option are available flag.
     */
    bool available() const;

private:
    QUANTTYPE _scale;
    std::string _desc;
    int _type;
    bool _need_quant;
};

/**
 * The ORTQuantizer class defines a quantize tool class.
 */
class ORT_API ORTQuantizer {
public:
    /**
     * @brief The default constructor for ORTQuantizer.
     */
    ORT_API ORTQuantizer();

    /**
     * @brief Destorys the ORTQuantizer object.
     */
    ORT_API virtual ~ORTQuantizer();

    /**
     * ＠brief Use quantization option to quantize the src tensor to dst.
     */
    ORT_API ORT_STATUS quantize(const std::shared_ptr<ORTTensor>& src, std::shared_ptr<ORTTensor>& dst,
                                const QuantOption& option);

    /**
     * ＠brief Use quantization option to dequantize the src tensor to dst.
     */
    ORT_API ORT_STATUS dequantize(const std::shared_ptr<ORTTensor>& src, std::shared_ptr<ORTTensor>& dst,
                                  const QuantOption& option);
};

}  // namespace oruntime

#endif  // _ORUNTIME_QUANTIZE_H_

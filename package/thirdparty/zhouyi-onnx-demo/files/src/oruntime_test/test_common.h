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

#ifndef _TEST_COMMON_H_
#define _TEST_COMMON_H_

#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "oruntime_types.h"
#include "oruntime_api.h"
#include "oruntime_quantize.h"
#include "oruntime_customop.h"

#define _BOOL2STR(cond) ((cond) ? ("true") : ("false"))

inline uint64_t common_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t time = tv.tv_sec * 1000000 + tv.tv_usec;  // microsecond
    return time;
}

inline uint32_t common_shape_length(const TensorShape& shape) {
    uint32_t length = 1;
    for (size_t i = 0; i < shape.dims; i++) {
        length = length * shape.data[i];
    }
    return length;
}

inline std::string common_shape_2_string(const TensorShape& shape) {
    std::string str = "";
    for (size_t i = 0; i < shape.dims; i++) {
        str = str + std::to_string(shape.data[i]);
        if (i != shape.dims -1) {
            str = str + ",";
        }
    }
    return std::string("[") + str + std::string("]");
}

inline bool load_input(std::string& file, std::shared_ptr<oruntime::ORTTensor>& input) {
    if (!input) {
        std::cout << "Error: " << "Illegal input tensor!" << std::endl;
        return false;
    }
    TensorShape shape = input->shape();
    TensorDataType dtype = input->data_type();
    void* input_addr = input->data();
    // std::cout << __FUNCTION__ << ":" << __LINE__ << " addr=" << input_addr << std::endl;
    uint32_t input_len = common_shape_length(shape);
    if (dtype == TensorDataType::ORT_TENSOR_DATA_F32) {
        input_len *= sizeof(float);
    }
    std::cout << "Info: " << " file:" << file.c_str() << std::endl;
    std::cout << "Info: " << "shape:" << common_shape_2_string(shape)
              << " len=" << input_len
              << " dtype=" << static_cast<size_t>(dtype)
              << " addr=" << input_addr  << std::endl;

    // read input file
    std::ifstream inf(file.c_str(), std::ios::in | std::ios::binary);
    if (!inf.is_open()) {
        std::cout << "Error: " << "open input " << file.c_str() << " file failed!!!" << std::endl;
        return false;
    }
    inf.seekg(0, std::ios::end);
    uint64_t flen = inf.tellg();
    inf.seekg(0, std::ios::beg);
    if (flen != input_len) {
        std::cout << "Error: " << "input tensor length not match[" << input_len << "," << flen << "]" << std::endl;
        inf.close();
        return false;
    }
    std::cout << "Info: " << "input size [" << input_len << " - " << flen << "]" << std::endl;

    memset(reinterpret_cast<uint8_t*>(input_addr), 0, input_len);

    inf.read(reinterpret_cast<char*>(input_addr), flen);
    // close file
    inf.close();
    return true;
}

inline bool load_and_quant_inputs(std::vector<std::shared_ptr<oruntime::ORTTensor>>& inputs,
                           std::vector<std::string>& files, bool quantize) {
    if (inputs.size() != files.size()) {
        return false;
    }
    oruntime::ORTQuantizer quantizer;
    for (size_t i = 0; i < inputs.size(); i++) {
        std::string& file                      = files.at(i);
        std::shared_ptr<oruntime::ORTTensor> tensor = inputs.at(i);
        oruntime::QuantOption option                = tensor->quant_option();
        if (quantize) {
            std::shared_ptr<oruntime::ORTTensor> input_data = std::make_shared<oruntime::ORTTensor>(
                std::string("input_data") + std::to_string(i),
                tensor->shape(),
                TensorFormat::ORT_TENSOR_NHWC,
                TensorDataType::ORT_TENSOR_DATA_F32);
            if (!load_input(file, input_data)) {
                return false;
            }

            // quantize input data
            bool quant_ret = quantizer.quantize(input_data, tensor, option);
            if (quant_ret != ORT_SUCCESS) {
                std::cout << "Error: quantize input data fail!" << std::endl;
                return false;
            }
        } else {
            if (!load_input(file, tensor)) {
                return false;
            }
        }
    }
    return true;
}

// dump output data
inline bool dump_file(const std::string& file, std::shared_ptr<oruntime::ORTTensor>& tensor) {
    std::ofstream ofile(file.c_str(), std::ios::out | std::ios::binary);
    if (!ofile.is_open()) {
        std::cout << "Error: " << "open output " << file.c_str() << " file failed!!!" << std::endl;
        return false;
    }

    if (!tensor) {
        std::cout << "Error: " << "Illegal output tensor!" << std::endl;
        return false;
    }
    TensorShape oshape = tensor->shape();
    TensorDataType odtype = tensor->data_type();
    void* output_addr = tensor->data();
    uint32_t output_len = common_shape_length(oshape);
    std::cout << "Info: " << common_shape_2_string(oshape)
              << " len=" << output_len << " dtype=" << static_cast<int>(odtype)
              << " addr=" << output_addr << std::endl;
    if (odtype == TensorDataType::ORT_TENSOR_DATA_F32) {
        ofile.write(reinterpret_cast<char*>(output_addr), output_len * sizeof(float));
    } else if (odtype == TensorDataType::ORT_TENSOR_DATA_U8) {
        ofile.write(reinterpret_cast<char*>(output_addr), output_len);
    }
    ofile.close();
    return true;
}

inline bool checkDirExists(std::string path) {
    int ret = 0;
    if (0 != access(path.c_str(), 0)) {
        // if this folder not exist, create a new one.
        ret = mkdir(path.c_str(), 0755);
    }
    return (ret == 0) ? true : false;
}

inline std::vector<std::string> split(const std::string &str, const char pattern) {
    std::vector<std::string> res;
    std::stringstream input(str);
    std::string temp;
    while (getline(input, temp, pattern)) {
        res.push_back(temp);
    }
    return res;
}

#endif  // _TEST_COMMON_H_

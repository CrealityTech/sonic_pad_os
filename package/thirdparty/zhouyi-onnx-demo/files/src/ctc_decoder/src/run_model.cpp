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

#include "run_model.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>

#include "test_common.h"
#include "oruntime_types.h"
#include "oruntime_api.h"
#include "oruntime_quantize.h"


std::vector<std::shared_ptr<oruntime::ORTTensor>> run_model(std::string& model_file,
                                                           std::vector<std::string>& input_files,
                                                           std::string& ouput_dir) {
    std::string graph_bin = model_file;
    std::string output_dir = ouput_dir;
    bool quantize = true;
    std::vector<std::shared_ptr<oruntime::ORTTensor>> empty{};
    // Step 1: init
    oruntime::ORTGraph aie_graph;
    if (aie_graph.init() != ORT_SUCCESS) {
        std::cout << "Error: aie init fail!" << std::endl;
        return empty;
    }
    if (aie_graph.loadModel(graph_bin) != ORT_SUCCESS) {
        std::cout << "Error: aie load fail fail!" << std::endl;
        return empty;
    }

    // Step 2: build
    if (aie_graph.build() != ORT_SUCCESS) {
        std::cout << "Error: aie build fail!" << std::endl;
        return empty;
    }

    // get input and output
    std::vector<std::shared_ptr<oruntime::ORTTensor>> inputs = aie_graph.getInputTensors();
    std::vector<std::shared_ptr<oruntime::ORTTensor>> outputs = aie_graph.getOutputTensors();
    // input quantize option
    if (inputs.size() != input_files.size()) {
        std::cout << "Error: Input tensor number not match[" << input_files.size()
                  << "," << inputs.size() << "]" << std::endl;
        return empty;
    }
    if (inputs.size() == 0) {
        std::cout << "Error: Invalid input size is zero!" << std::endl;
        return empty;
    }
    // input quantize option
    for (size_t i = 0; i < inputs.size(); i++) {
        std::shared_ptr<oruntime::ORTTensor> tensor = inputs.at(i);
        oruntime::QuantOption option = tensor->quant_option();
    }

    // output quantize option
    for (size_t i = 0; i < outputs.size(); i++) {
        std::shared_ptr<oruntime::ORTTensor> tensor = outputs.at(i);
        oruntime::QuantOption option = tensor->quant_option();
    }

    // Step 3: load input and quantify input data
    oruntime::ORTQuantizer quantizer;
    if (!load_and_quant_inputs(inputs, input_files, quantize)) {
        return empty;
    }

    // Step 4: run
    uint64_t start = common_time();
    if (aie_graph.run() != ORT_SUCCESS) {
         std::cout << "Error: " << "graph run failed!" << std::endl;
        return empty;
    }
    uint64_t end = common_time();

    // Step 5: get output and de-quantify tensor
    std::vector<std::shared_ptr<oruntime::ORTTensor>> vector_list;
    for (size_t i = 0; i < outputs.size(); i++) {
        std::shared_ptr<oruntime::ORTTensor> tensor = outputs.at(i);
        oruntime::QuantOption option = tensor->quant_option();
        std::string name = "output" + std::to_string(i);

        if (quantize) {
            std::string name_f32 = name + "_f32";
            std::shared_ptr<oruntime::ORTTensor> output_f32 = std::make_shared<oruntime::ORTTensor>(
                    name_f32,
                    tensor->shape(),
                    TensorFormat::ORT_TENSOR_NHWC,
                    TensorDataType::ORT_TENSOR_DATA_F32);
            bool quant_ret = quantizer.dequantize(tensor, output_f32, option);
            if (quant_ret != ORT_SUCCESS) {
                std::cout << "Error: dequantize input data fail!" << std::endl;
                return empty;
            }
            vector_list.push_back(output_f32);
        } else {
            vector_list.push_back(tensor);
        }
    }
    return vector_list;
}

bool fill_input_data(std::vector<std::shared_ptr<oruntime::ORTTensor>>& inputs,
                     std::vector<INPUT_BUF>& input_bufs) {
    if (inputs.size() != input_bufs.size()) {
        return false;
    }
    oruntime::ORTQuantizer quantizer;
    for (size_t i = 0; i < inputs.size(); i++) {
        std::shared_ptr<oruntime::ORTTensor> tensor = inputs.at(i);
        oruntime::QuantOption option                = tensor->quant_option();

        // fill data
        if (!tensor) {
            std::cout << "Error: " << "Illegal input tensor!" << std::endl;
            return false;
        }
        TensorShape shape = tensor->shape();
        TensorDataType dtype = tensor->data_type();
        void* input_addr = tensor->data();
        uint32_t input_len = common_shape_length(shape);
        if (dtype == TensorDataType::ORT_TENSOR_DATA_F32) {
            input_len *= sizeof(float);
        }

        INPUT_BUF& data = input_bufs.at(i);
        if (input_len != data.len) {
            std::cout << "input_len=" << input_len << "-" << data.len << std::endl;
            std::cout << "ERR:" << " input length error!" << std::endl;
            return false;
        }
        memcpy(input_addr, data.data, input_len);
    }
    return true;
}


std::vector<std::shared_ptr<oruntime::ORTTensor>> run_model_buf(std::string& model_file,
                                                                std::vector<INPUT_BUF>& input_bufs,
                                                                std::string& ouput_dir) {
    std::vector<std::shared_ptr<oruntime::ORTTensor>> empty{};
    bool quantize = false;
    // Step 1: init
    oruntime::ORTGraph aie_graph;
    if (aie_graph.init() != ORT_SUCCESS) {
        std::cout << "Failed to init." << std::endl;
        return empty;
    }
    if (aie_graph.loadModel(model_file) != ORT_SUCCESS) {
        std::cout << "Failed to load model." << std::endl;
        return empty;
    }

    // Step 2: build
    if (aie_graph.build() != ORT_SUCCESS) {
        std::cout << "Failed to build model." << std::endl;
        return empty;
    }
    // get input and output
    std::vector<std::shared_ptr<oruntime::ORTTensor>> inputs = aie_graph.getInputTensors();
    std::vector<std::shared_ptr<oruntime::ORTTensor>> outputs = aie_graph.getOutputTensors();
    // input quantize option
    if (inputs.size() != input_bufs.size()) {
        std::cout << "Input size not matched." << std::endl;
        return empty;
    }
    if (inputs.size() == 0) {
        std::cout << "Empty input tensor." << std::endl;
        return empty;
    }
    if (outputs.size() == 0) {
        std::cout << "Empty output tensor." << std::endl;
    }
    // input quantize option
    for (size_t i = 0; i < inputs.size(); i++) {
        std::shared_ptr<oruntime::ORTTensor> tensor = inputs.at(i);
        oruntime::QuantOption option = tensor->quant_option();
    }
    // output quantize option
    for (size_t i = 0; i < outputs.size(); i++) {
        std::shared_ptr<oruntime::ORTTensor> tensor = outputs.at(i);
        oruntime::QuantOption option = tensor->quant_option();
        // std::cout << "Output" << i << " option: desc=" << option.desc() << " scale=" << option.scale()
        //           << " available=" << _BOOL2STR(option.available()) << std::endl;
    }
    // Step 3: load input and quantify input data
    oruntime::ORTQuantizer quantizer;
    if (!fill_input_data(inputs, input_bufs)) {
        std::cout << "Failed to fill input data." << std::endl;
        return empty;
    }

    // Step 4: run
//    uint64_t start = common_time();
    if (aie_graph.run() != ORT_SUCCESS) {
        // std::cout << "Error: " << "graph run failed!" << std::endl;
        std::cout << "Failed to run graph." << std::endl;
        return empty;
    }
//    uint64_t end = common_time();

    // Step 5: get output and dequantify tensor
    std::vector<std::shared_ptr<oruntime::ORTTensor>> vector_list;
    for (size_t i = 0; i < outputs.size(); i++) {
        std::shared_ptr<oruntime::ORTTensor> tensor = outputs.at(i);
        oruntime::QuantOption option = tensor->quant_option();
        std::string name = "output" + std::to_string(i);
        // std::string name_u8 = name + "_u8";
        // dump_file(output_dir + "/" + name_u8 + ".bin", tensor);

        if (quantize) {
            std::string name_f32 = name + "_f32";
            std::shared_ptr<oruntime::ORTTensor> output_f32 = std::make_shared<oruntime::ORTTensor>(
                    name_f32,
                    tensor->shape(),
                    TensorFormat::ORT_TENSOR_NHWC,
                    TensorDataType::ORT_TENSOR_DATA_F32);
            bool quant_ret = quantizer.dequantize(tensor, output_f32, option);
            if (quant_ret != ORT_SUCCESS) {
                std::cout << "Failed to de-quantize." << std::endl;
                return empty;
            }
            // dump_file(output_dir + "/" + name_f32 + ".bin", output_f32);
            vector_list.push_back(output_f32);
        } else {
            vector_list.push_back(tensor);
        }
    }
    return vector_list;
}

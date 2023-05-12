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
#include "oruntime_customop.h"

bool aie_shape_is_match(TensorShape& shape1, TensorShape& shape2) {
    bool is_match = true;
    if (shape1.dims != shape2.dims) {
        is_match = false;
        return is_match;
    }
    for (size_t i = 0; i < shape1.dims; i++) {
        if (shape1.data[i] != shape2.data[i]) {
            is_match = false;
            break;
        }
    }
    return is_match;
}

// >>> custom op implementation
ORT_CUSTOMOP(Custom) {
    std::cout << std::endl;
    std::cout << "##### customer op implement #####" << std::endl;
    std::vector<std::shared_ptr<oruntime::ORTTensor>>& itensors = inputs;
    std::vector<std::shared_ptr<oruntime::ORTTensor>>& otensors = outputs;
    std::cout << __FUNCTION__ << "[COP-ADD] input["
              << itensors.size() << "] output[" << otensors.size() << "]" << std::endl;

    if (inputs.size() != 1 || outputs.size() != 1) {
        std::cout << "ERR:"
                  << "[COP-ADD] The parameter size("
                  << inputs.size() << "," << outputs.size() << " is not match ADD operator!" << std::endl;
        return false;
    }

    std::shared_ptr<oruntime::ORTTensor> input0 = inputs.at(0);
    std::shared_ptr<oruntime::ORTTensor> output = outputs.at(0);

    TensorShape shape0 = input0->shape();
    TensorShape shape2 = output->shape();

    if (!aie_shape_is_match(shape0, shape2)) {
        std::cout << "ERR:" << "[COP-ADD] input and output shape is not match!" << std::endl;
        std::cout << "Info:" << common_shape_2_string(shape0) << std::endl;
        std::cout << "Info:" << common_shape_2_string(shape2) << std::endl;
        return false;
    }

    if (input0->data_type() != TensorDataType::ORT_TENSOR_DATA_F32 ||
        output->data_type() != TensorDataType::ORT_TENSOR_DATA_F32) {
        std::cout << "ERR: [COP-ADD]"
                  << " data type is not float type!" << std::endl;
        return false;
    }

    oruntime::QuantOption in_option0 = input0->quant_option();
    double in_scale0 = in_option0.scale();
    std::cout << "INFO: " << __FUNCTION__ << ":"
              << __LINE__ << "input scale " << in_scale0 << std::endl;

    float* in_ptr0 = reinterpret_cast<float*>(input0->data());
    float* out_ptr0 = reinterpret_cast<float*>(output->data());
    uint32_t len = common_shape_length(shape0);

    // compute
    for (size_t i = 0; i < len; i++) {
        float a = in_ptr0[i];
        out_ptr0[i] = a * 2 + 3.14;
    }

    std::cout << std::endl;
    return true;
}
// <<<

void show_usage() {
    std::cout << "Options:\n"
              << "-h, --help\t\t\tshow the help message\n"
              << "--bin=<bin file>\t\tprovide the graph bin file\n"
              << "--input=<input file>\t\tthe network input data file\n"
              << "--quantize=<bool>\t\twhether to quantize and dequantize the input(default value is true).\n"
              << "--outdir=<output file>\t\tthe graph ouput directory\n"
              << std::endl;
}

#define AIE_STR_HELPER(x) #x
#define AIE_STR(x) AIE_STR_HELPER(x)

int main(int argc, char** argv) {
    std::cout << "Info: welcome aipu!!!" << std::endl;
    std::string graph_bin = "./aipu.bin";
    std::string input_bin = "./input.bin";
    std::vector<std::string> input_files;
    std::string output_dir = "./output";
    bool quantize = true;

    auto pickParam = [](std::string s) -> std::string {
        std::string::size_type tmp = s.find_last_of("=");
        if (tmp != std::string::npos) {
            return s.substr(tmp + 1);
        } else {
            return std::string("");
        }
    };

    for (int i = 0; i < argc; i++) {
        std::string tmp(argv[i]);
        if (std::string::npos != tmp.find("-h")) {
            show_usage();
            return 0;
        }
        if (std::string::npos != tmp.find("--bin")) {
            graph_bin = pickParam(tmp);
        }
        if (std::string::npos != tmp.find("--input")) {
            input_bin = pickParam(tmp);
        }
        if (std::string::npos != tmp.find("--outdir")) {
            output_dir = pickParam(tmp);
        }
        if (std::string::npos != tmp.find("--quantize")) {
            std::string quant_str = pickParam(tmp);
            if (quant_str == "true") {
                quantize = true;
            } else if (quant_str == "false") {
                quantize = false;
            }
        }
    }

    if (!checkDirExists(output_dir)) {
        std::cout << "Error: output directory not exist and create fail!" << std::endl;
        return -1;
    }
    input_files = split(input_bin, ',');
    for (size_t i = 0; i < input_files.size(); i++) {
        std::cout << "input file " << i << " " << input_files.at(i) << std::endl;;
    }

    // Step 1: init
    oruntime::ORTGraph ort_graph;
    std::cout << "Info: version v" << ort_graph.version() << std::endl;
    std::cout << "Info: init" << std::endl;
    if (ort_graph.init() != ORT_SUCCESS) {
        std::cout << "Error: aie init fail!" << std::endl;
        return -1;
    }
    std::cout << "Info: loadModel" << std::endl;
    if (ort_graph.loadModel(graph_bin) != ORT_SUCCESS) {
        std::cout << "Error: aie load fail fail!" << std::endl;
        return -1;
    }

    // declare for user defined operator object
    std::shared_ptr<ORT_COP_NAME(Custom)> add_op = std::make_shared<ORT_COP_NAME(Custom)>(2);
    if (ort_graph.registerOperator(2, add_op) != ORT_SUCCESS) {
        std::cout << "Error: aie register op fail!" << std::endl;
        return -1;
    }

    // Step 2: build
    if (ort_graph.build() != ORT_SUCCESS) {
        std::cout << "Error: aie build fail!" << std::endl;
        return -1;
    }

    // get input and output
    std::vector<std::shared_ptr<oruntime::ORTTensor>> inputs = ort_graph.getInputTensors();
    std::vector<std::shared_ptr<oruntime::ORTTensor>> outputs = ort_graph.getOutputTensors();
    std::cout << "Info: input size:" << inputs.size() << " output size=" << outputs.size() << std::endl;

    // input quantize option
    if (inputs.size() != input_files.size()) {
        std::cout << "Error: Input tensor number not match[" << input_files.size()
                  << "," << inputs.size() << "]" << std::endl;
        return -1;
    }
    if (inputs.size() == 0) {
        std::cout << "Error: Invaild input size is zero!" << std::endl;
        return -1;
    }
    // input quantize option
    for (size_t i = 0; i < inputs.size(); i++) {
        std::shared_ptr<oruntime::ORTTensor> tensor = inputs.at(i);
        oruntime::QuantOption option = tensor->quant_option();
        std::cout << "Input" << i << " option: desc=" << option.desc() << " scale=" << option.scale()
                  << " available=" << _BOOL2STR(option.available()) << std::endl;
    }

    // output quantize option
    for (size_t i = 0; i < outputs.size(); i++) {
        std::shared_ptr<oruntime::ORTTensor> tensor = outputs.at(i);
        oruntime::QuantOption option = tensor->quant_option();
        std::cout << "Output" << i << " option: desc=" << option.desc() << " scale=" << option.scale()
                  << " available=" << _BOOL2STR(option.available()) << std::endl;
    }

    // Step 3: load input and quantify input data
    oruntime::ORTQuantizer quantizer;
    if (!load_and_quant_inputs(inputs, input_files, quantize)) {
        return -1;
    }
    if (quantize) {
        for (size_t i = 0; i < inputs.size(); i++) {
            std::shared_ptr<oruntime::ORTTensor> tensor = inputs.at(i);
            std::string dump_name = output_dir + "/input" + std::to_string(i) + "_quant.bin";
            // dump_file(dump_name, tensor);
        }
    }

    // Step 4: run
    uint64_t start = common_time();
    if (ort_graph.run() != ORT_SUCCESS) {
        std::cout << "Error: " << "graph run failed!" << std::endl;
        return -1;
    }
    uint64_t end = common_time();
    std::cout << " run cost time:" << (end - start) << "(us)" << std::endl;

    // Step 5: get output and dequantify tensor
    for (size_t i = 0; i < outputs.size(); i++) {
        std::shared_ptr<oruntime::ORTTensor> tensor = outputs.at(i);
        oruntime::QuantOption option = tensor->quant_option();
        std::string name = "output" + std::to_string(i);
        std::string name_u8 = name + "_u8";
        dump_file(output_dir + "/" + name_u8 + ".bin", tensor);

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
                return -1;
            }
            dump_file(output_dir + "/" + name_f32 + ".bin", output_f32);
        }
    }

    std::cout << "finish!!!" << std::endl;
    return 0;
}

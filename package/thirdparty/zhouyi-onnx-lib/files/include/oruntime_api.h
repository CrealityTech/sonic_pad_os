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

#ifndef _ORUNTIME_API_H_
#define _ORUNTIME_API_H_

#include <string>
#include <vector>

#include "oruntime_tensor.h"
#include "oruntime_customop.h"

namespace oruntime {

/*
 * @brief ORTGraph defines a graph class, user can load a graph(network) and operate on the
 * specified device and get the calculation result.
 */
class ORT_API ORTGraph {
public:
    /**
     * @brief Constructor, create an ORTGraph object.
     */
    ORT_API ORTGraph();
    /**
     * @brief Destructor, release the object and releated resoureces.
     */
    ORT_API virtual ~ORTGraph();

    /**
     * @brief init Initialize ORTGraph and create a running context.
     */
    ORT_API ORT_STATUS init();

    /**
     * @brief deInit Release the running context.
     */
    ORT_API ORT_STATUS deInit();

    /**
     * @brief loadModel Load a neural network(graph) from a locale file path.
     * @param file[in]  The graph bin file path.
     */
    ORT_API ORT_STATUS loadModel(std::string& file);

    /**
     * @brief build Build a network environment that can run this network on the device.
     */
    ORT_API ORT_STATUS build();

    /**
     * @brief run Execute this network on AIPU/CPU device and get results on the output tensors.
     */
    ORT_API ORT_STATUS run();

    /**
     * @brief getInputTensors Get this network input tensors.
     * @return   input tensors of the network.
     */
    ORT_API std::vector<std::shared_ptr<ORTTensor>> getInputTensors();

    /**
     * @brief getOutputTensors Get this network output tensors.
     * @return   output tensors of the network.
     */
    ORT_API std::vector<std::shared_ptr<ORTTensor>> getOutputTensors();

    /**
     * @brief registerOperator The function allow user defined an operator and register to graph.
     * @param [in] id : The operator unique id.
     * @param [in] op : The user defined operator object.
     * @return [out] bool : register sucess is true, otherwise is false.
     */
    ORT_API ORT_STATUS registerOperator(uint32_t id, std::shared_ptr<ORTCustomOP> op);

    /**
     * @biref version Get the version number of the runtime.
     * @return [out] string : the version number string.
     */
    ORT_API std::string version() const;
};

}  // namespace oruntime

#endif  // _ORUNTIME_API_H_

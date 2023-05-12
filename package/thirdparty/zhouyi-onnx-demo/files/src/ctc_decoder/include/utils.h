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


#ifndef RNN_T_UTILS_H_
#define RNN_T_UTILS_H_

#include <vector>
#include <map>
#include <iostream>
#include <fstream>

#define BLANK_INDEX 0
#define HIDDEN_SIZE 20
#define NEG_INF (-9999.9)

float log_sum_exp(const std::vector<float>& prob_list);

std::vector<std::string> split(std::string target, std::string delimiter);

std::map<int, std::string> read_map();

#endif  // RNN_T_UTILS_H_

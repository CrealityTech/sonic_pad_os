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


#include "utils.h"

#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>


float log_sum_exp(const std::vector<float>& prob_list) {
    bool all_neg_inf_flag = true;
    for (float i : prob_list) {
        if (i != NEG_INF) {
            all_neg_inf_flag = false;
        }
    }
    if (all_neg_inf_flag) {
        return NEG_INF;
    }

    // float max_prob = *max_element(prob_list.begin(), prob_list.end());
    auto first = prob_list.begin();
    auto last = prob_list.end();
    auto largest = first;
    ++first;
    for (; first != last; ++first) {
        if (*largest < *first) {
            largest = first;
        }
    }
    float max_prob = *largest;

    float log_sum_prob = 0.0;
    for (float i : prob_list) {
        log_sum_prob += exp(i - max_prob);
    }
    return max_prob + log(log_sum_prob);
}


std::vector<std::string> split(std::string target, std::string delimiter) {
    std::vector<std::string> result;
    size_t pos = 0;
    std::string token;
    while ((pos = target.find(delimiter)) != std::string::npos) {
        token = target.substr(0, pos);
        result.push_back(token);
        target.erase(0, pos + delimiter.length());
    }
    result.push_back(target);
    return result;
}

std::map<int, std::string> read_map(std::string& file) {
    std::ifstream infile(file);
    std::string line;
    std::map<int, std::string> phoneme_map;
    std::map<std::string, int> phoneme_remap;
    int line_count = 0;
    while (std::getline(infile, line)) {
        std::string delimiter = " ";
        std::vector <std::string> splits = split(line, delimiter);
        if (splits.size() > 1) {
            std::map<std::string, int>::iterator it = phoneme_remap.find(splits[0]);
            if (it == phoneme_remap.end()) {
                phoneme_map[line_count] = splits[0];
                phoneme_remap[splits[0]] = line_count;
            }
        }
        line_count += 1;
    }
    return phoneme_map;
}

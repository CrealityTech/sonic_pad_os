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


#ifndef RNN_T_BEAM_DECODE_H_
#define RNN_T_BEAM_DECODE_H_

#include <vector>
#include <map>
#include <string>

void beam_decode(std::vector<std::string> graph_files,
                 std::string input_bin,
                 std::vector<int>& word_list,
                 int beam_width,
                 int seq_len);

std::map<int, std::string> read_map(std::string& file);

#endif  // RNN_T_BEAM_DECODE_H_

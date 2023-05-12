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


#include <iostream>
#include <vector>
#include "include/beam_decode.h"
#include "test_common.h"


void show_usage() {
    std::cout << "Options:\n"
              << "-h, --help\t\t\tshow the help message\n"
              << "--bin=<bin file>\t\tprovide the graph bin file. Order: encoder, decoder, joint\n"
              << "--input=<input file>\t\tthe network input data file\n"
              << "--phone_map=<phone map file>\t\tthe phone map data file\n"
              << "--beam_width=<beam width>\t\tBeam width in beam search, 10 by default.\n"
              << "--seq_len=<sequence length>\t\tMaximum sequence length of input feature, 50 by default.\n"
              << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "Beam decode:" << std::endl;

    std::string graph_bin = "./aipu.bin";
    std::string input_bin = "./input.bin";
    std::string phone_map_file = "./phones.txt";
    int beam_width = 10;
    int seq_len = 50;
    std::vector<std::string> graph_files;

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
        if (std::string::npos != tmp.find("--phone_map")) {
            phone_map_file = pickParam(tmp);
        }
        if (std::string::npos != tmp.find("--beam_width")) {
            beam_width = std::stoi(pickParam(tmp));
        }
        if (std::string::npos != tmp.find("--seq_len")) {
            seq_len = std::stoi(pickParam(tmp));
        }
    }
    graph_files = split(graph_bin, ',');
    for (size_t i = 0; i < graph_files.size(); i++) {
        std::cout << "graph file " << i << " " << graph_files.at(i) << std::endl;
    }
    if (graph_files.size() != 3) {
        std::cout << "There are 3 models in total." << std::endl;
        return 0;
    }

    std::vector<int> word_list;
    beam_decode(graph_files, input_bin, word_list, beam_width, seq_len);

    // print phonemes
    std::map<int, std::string> phoneme_map = read_map(phone_map_file);
    std::cout << "Phoneme indices:" <<std:: endl;
    for (int word : word_list) {
        std::cout << word << " ";
    }
    std::cout << std::endl;
    std::cout << "Phonemes:" << std::endl;
    for (int word : word_list) {
        std::cout << phoneme_map[word] << " ";
    }
    std::cout << std::endl;
    std::cout << "End." << std::endl;
    return 0;
}

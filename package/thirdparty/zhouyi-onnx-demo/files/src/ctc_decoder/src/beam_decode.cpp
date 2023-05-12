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

#include "beam_decode.h"

#include <stdint.h>
#include <algorithm>
#include <sstream>
#include <istream>
#include <fstream>
#include <string>
#include <iterator>
#include <memory>

#include "test_common.h"
#include "utils.h"
#include "sequence.h"
#include "run_model.h"

int vocab_size = 4;

void hidden_state_to_vector(std::shared_ptr<oruntime::ORTTensor> tensor, std::vector<std::vector<float>>& probs) {
    size_t batch = 1;
    size_t seq_length = 1;
    size_t hidden_size = 48;
    float* data_pointer = reinterpret_cast<float *>(tensor->data());
    size_t buff_size = batch * seq_length * hidden_size;

    std::vector<float> float_data {data_pointer, data_pointer + buff_size};
    std::vector<float>::iterator it = float_data.begin();
    for (size_t i = 0; i < seq_length; ++i) {
        std::vector<float> prob {it, it + hidden_size};
        probs.push_back(prob);
    }
    return;
}

void hidden_state_to_vector_nq(std::shared_ptr<oruntime::ORTTensor> tensor,
                               std::vector<std::vector<int8_t>>& output,
                               int seq_length = 1,
                               int hidden_size = 20) {
    size_t batch = 1;
    int8_t* data_pointer = reinterpret_cast<int8_t *>(tensor->data());
    size_t buff_size = batch * seq_length * hidden_size;

    std::vector<int8_t> int_data {data_pointer, data_pointer + buff_size};
    std::vector<int8_t>::iterator it = int_data.begin();
    for (int i = 0; i < seq_length; ++i) {
        std::vector<int8_t> prob {it, it + hidden_size};
        output.push_back(prob);
        it += buff_size;
    }

    return;
}


void generate_next_word(int best_last_word,
                        std::vector<int8_t> x_split,
                        std::vector<float>& next_word_prob,
                        std::vector<std::vector<int8_t>>& hidden_result,
                        std::vector<std::vector<int8_t>> hidden,
                        std::string decoder_model_file,
                        std::string joint_model_file) {
    std::vector<int8_t> word_embedding(48);
    if (best_last_word != -1) {
        if (best_last_word < 0 || best_last_word > 48) {
            std::cout << "Value of best_last_word is wrong!" << std::endl;
        } else {
            word_embedding[best_last_word] = 1;
        }
    }

    if (hidden.size() != 2) {
        std::cout << "hidden must have two items." << std::endl;
    }
    std::vector<int8_t> previous_hidden = hidden[0];
    std::vector<int8_t> previous_cell = hidden[1];

    std::string decoder_output_folder = "./output";

    INPUT_BUF embedding_warp;
    embedding_warp.data = &word_embedding[0];
    embedding_warp.len = 48;
    INPUT_BUF hidden_warp;
    hidden_warp.data = &previous_hidden[0];
    hidden_warp.len = 20;
    INPUT_BUF cell_warp;
    cell_warp.data = &previous_cell[0];
    cell_warp.len = 20;
    std::vector<INPUT_BUF> inputs_buf;
    inputs_buf.push_back(embedding_warp);
    inputs_buf.push_back(hidden_warp);
    inputs_buf.push_back(cell_warp);
    std::vector<std::shared_ptr<oruntime::ORTTensor>> decoder_tensor_list = run_model_buf(decoder_model_file,
                                                                                inputs_buf,
                                                                                decoder_output_folder);

    if (decoder_tensor_list.size() == 1) {
        std::cout << "decoder output should be larger than 1." << std::endl;
    }
    std::vector<std::vector<int8_t>> decoder_output_list;
    hidden_state_to_vector_nq(decoder_tensor_list[0], decoder_output_list, 1, HIDDEN_SIZE);
    std::vector<int8_t> decoder_output = decoder_output_list[0];

    std::vector<std::vector<int8_t>> next_hidden_list;
    hidden_state_to_vector_nq(decoder_tensor_list[1], next_hidden_list, 1, HIDDEN_SIZE);
    std::vector<int8_t> next_hidden = next_hidden_list[0];

    std::vector<std::vector<int8_t>> next_cell_list;
    hidden_state_to_vector_nq(decoder_tensor_list[2], next_cell_list, 1, HIDDEN_SIZE);
    std::vector<int8_t> next_cell = next_cell_list[0];

    // Run joint
    INPUT_BUF joint_x_split_wrap;
    joint_x_split_wrap.data = &x_split[0];
    joint_x_split_wrap.len = HIDDEN_SIZE;
    INPUT_BUF joint_decoder_output_wrap;
    joint_decoder_output_wrap.data = &decoder_output[0];
    joint_decoder_output_wrap.len = HIDDEN_SIZE;
    std::vector<INPUT_BUF> joint_inputs_buf;
    joint_inputs_buf.push_back(joint_x_split_wrap);
    joint_inputs_buf.push_back(joint_decoder_output_wrap);
    std::vector<std::shared_ptr<oruntime::ORTTensor>> joint_tensor_list = run_model_buf(joint_model_file,
                                                                              joint_inputs_buf,
                                                                              decoder_output_folder);

    std::shared_ptr<oruntime::ORTTensor> joint_output = joint_tensor_list[0];
    std::vector<std::vector<float>> next_word_prob_list;
    hidden_state_to_vector(joint_output, next_word_prob_list);
    next_word_prob = next_word_prob_list[0];
    // next word prob, hidden state
    hidden_result.push_back(next_hidden);
    hidden_result.push_back(next_cell);

    return;
}


void beam_decode(std::vector<std::string> graph_files,
                 std::string input_bin,
                 std::vector<int>& word_list,
                 int beam_width,
                 int seq_len) {
    // De-quantization for encoder.
    std::string encoder_model_file = graph_files[0];
    std::string encoder_output_folder = "./output";
    std::vector<std::string> encoder_input_files = {input_bin};
    std::vector<std::shared_ptr<oruntime::ORTTensor>> encoder_tensor_list = run_model(encoder_model_file,
                                                                            encoder_input_files,
                                                                            encoder_output_folder);
    std::shared_ptr<oruntime::ORTTensor> encoder_output = encoder_tensor_list[0];
    int seq_length = seq_len;
    std::vector<std::vector<float>> encoder_embedding;
    hidden_state_to_vector(encoder_output, encoder_embedding);
    std::vector<std::vector<int8_t>> encoder_embedding_int;
    hidden_state_to_vector_nq(encoder_output, encoder_embedding_int, seq_len, HIDDEN_SIZE);

    std::shared_ptr<SequenceList> beam = std::make_shared<SequenceList>();
    std::shared_ptr<Sequence> start = std::make_shared<Sequence>();
    start->set_log_prob_blank(0.0);
    start->set_log_prob_non_blank(NEG_INF);
    beam->append_sequence(start);

    // Loop over time
    for (int t = 0; t < seq_length; ++t) {
        if (t >= encoder_embedding_int.size()) {
            std::cout << "t is larger than encoder_embedding_int.size()" << std::endl;
            continue;
        }
        std::vector<int8_t> x_split = encoder_embedding_int[t];
        std::cout << "time step " << t << std::endl;
        std::shared_ptr<SequenceList> next_beam = std::make_shared<SequenceList>();
        // After update_beam, 1st sequence is the best one.
        std::shared_ptr<Sequence> best_sequence = beam->get_sequence(0);
        int best_last_word = best_sequence->get_last_word();
        std::vector<float> prob_list;
        std::vector<std::vector<int8_t>> hidden;
        generate_next_word(best_last_word,
                           x_split,
                           prob_list,
                           hidden,
                           best_sequence->get_hidden(),
                           graph_files[1],
                           graph_files[2]);

        // Loop over vocab
        for (int j = 0; j < prob_list.size(); ++j) {
            int word_idx = j;
            float word_log_prob_vocab = prob_list[j];
            // Loop over previous beam
            for (int i = 0; i < beam->get_length(); ++i) {
                std::shared_ptr<Sequence> prefix = beam->get_sequence(i);
                float prefix_log_prob_blank = prefix->get_log_prob_blank();
                float prefix_log_prob_non_blank = prefix->get_log_prob_non_blank();
                // If we propose a blank the prefix doesn't change.
                // Only the probability of ending in blank gets updated.
                if (j == BLANK_INDEX) {
                    std::shared_ptr<Sequence> blank_prefix = std::make_shared<Sequence>(prefix->get_word_idx_list(),
                                                                                        hidden);
                    float blank_prefix_log_prob_blank = blank_prefix->get_log_prob_blank();
                    std::vector<float> input_list = {
                            blank_prefix_log_prob_blank,
                            prefix_log_prob_blank + word_log_prob_vocab,
                            prefix_log_prob_non_blank + word_log_prob_vocab,
                    };
                    float prob = log_sum_exp(input_list);
                    blank_prefix->set_log_prob_blank(prob);
                    next_beam->append_sequence(blank_prefix);
                    continue;
                }
                // Extend the prefix by the new character s and add it to
                // the beam. Only the probability of not ending in blank
                // gets updated.
                std::shared_ptr<Sequence> new_prefix = std::make_shared<Sequence>(prefix->get_word_idx_list(), hidden);
                int end_word = new_prefix->get_last_word();
                new_prefix->append_last_word(word_idx);
                new_prefix = next_beam->find_sequence_with_same_words(new_prefix);
                new_prefix->set_hidden(hidden);
                float new_prefix_log_prob_non_blank = new_prefix->get_log_prob_non_blank();
                if (word_idx != end_word) {
                    std::vector<float> input_list = {
                            new_prefix_log_prob_non_blank,
                            prefix_log_prob_blank + word_log_prob_vocab,
                            prefix_log_prob_non_blank + word_log_prob_vocab,
                    };
                    float prob = log_sum_exp(input_list);
                    new_prefix->set_log_prob_non_blank(prob);
                } else {
                    std::vector<float> input_list = {
                            new_prefix_log_prob_non_blank,
                            prefix_log_prob_blank + word_log_prob_vocab,
                    };
                    float prob = log_sum_exp(input_list);
                    new_prefix->set_log_prob_non_blank(prob);
                }
                next_beam->append_sequence(new_prefix);
                // If s is repeated at the end we also update the unchanged
                // prefix. This is the merging case.
                if (word_idx == end_word) {
                    std::shared_ptr<Sequence> duplicate_prefix = next_beam->find_sequence_with_same_words(prefix);
                    float duplicate_prefix_log_prob_non_blank = duplicate_prefix->get_log_prob_non_blank();
                    std::vector<float> input_list = {
                            duplicate_prefix_log_prob_non_blank,
                            prefix_log_prob_non_blank + word_log_prob_vocab,
                    };
                    float prob = log_sum_exp(input_list);
                    duplicate_prefix->set_log_prob_non_blank(prob);
                    next_beam->append_sequence(duplicate_prefix);
                }
            }
        }
        // Sort and trim the beam before moving on to the
        // next time-step.
        beam = next_beam->update_beam(beam_width);
    }
    std::shared_ptr<Sequence> best_sequence = beam->get_sequence(0);
    word_list = best_sequence->get_word_idx_list();
    return;
}

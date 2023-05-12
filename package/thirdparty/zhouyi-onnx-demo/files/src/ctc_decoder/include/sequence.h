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


#ifndef RNN_T_SEQUENCE_H_
#define RNN_T_SEQUENCE_H_

#include <stdint.h>
#include <vector>
#include <map>
#include <string>
#include <memory>

class Sequence {
private:
    std::vector<int> word_idx_list;
    float log_prob_total;
    float log_prob_blank;
    float log_prob_non_blank;
    std::vector<std::vector<int8_t>> hidden;

public:
    Sequence();
    Sequence(const Sequence & copy);
    explicit Sequence(std::vector<int> word_idx_list);
    Sequence(std::vector<int> word_idx_list, std::vector<std::vector<int8_t>> hidden);
    std::vector<int> get_word_idx_list();
    float get_log_prob_total();
    float get_log_prob_blank();
    float get_log_prob_non_blank();
    void set_log_prob_total(float prob);
    void set_log_prob_blank(float prob);
    void set_log_prob_non_blank(float prob);
    int get_last_word();
    bool append_last_word(int word);
    std::vector<std::vector<int8_t>> get_hidden();
    void set_hidden(std::vector<std::vector<int8_t>> hidden);
};

inline std::vector<int> Sequence::get_word_idx_list() {
    return this->word_idx_list;
}
inline float Sequence::get_log_prob_total() {
    return this->log_prob_blank;
}
inline float Sequence::get_log_prob_blank() {
    return this->log_prob_blank;
}
inline float Sequence::get_log_prob_non_blank() {
    return this->log_prob_non_blank;
}

inline void Sequence::set_log_prob_total(float prob) {
    this->log_prob_blank = prob;
}
inline void Sequence::set_log_prob_blank(float prob) {
    this->log_prob_blank = prob;
}
inline void Sequence::set_log_prob_non_blank(float prob) {
    this->log_prob_non_blank = prob;
}

inline std::vector<std::vector<int8_t>> Sequence::get_hidden() {
    return this->hidden;
}

inline void Sequence::set_hidden(std::vector<std::vector<int8_t>> hidden) {
    this->hidden = hidden;
}


class SequenceList {
private:
    std::vector<std::shared_ptr<Sequence>> sequence_list;
    std::map<std::vector<int>, int> loop_up_table;
//    bool construct_loop_up_table();

public:
    SequenceList();
    explicit SequenceList(std::vector<std::shared_ptr<Sequence>> seq_list);
    std::shared_ptr<Sequence> get_sequence(int index);
    std::shared_ptr<Sequence> find_sequence_with_same_words(std::shared_ptr<Sequence> sequence);
    int get_length();
    bool append_sequence(std::shared_ptr<Sequence> sequence);
    std::shared_ptr<SequenceList> update_beam(int beam_width);
};

#endif  // RNN_T_SEQUENCE_H_

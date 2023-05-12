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


#include "sequence.h"

#include <algorithm>
#include <utility>

#include "utils.h"

Sequence::Sequence() : hidden(2, std::vector<int8_t>(20, 0)) {
    this->log_prob_total = NEG_INF;
    this->log_prob_blank = NEG_INF;
    this->log_prob_non_blank = NEG_INF;
}

Sequence::Sequence(const Sequence & copy) : hidden(2, std::vector<int8_t>(20, 0)) {
    this->word_idx_list = copy.word_idx_list;
    this->log_prob_total = copy.log_prob_total;
    this->log_prob_blank = copy.log_prob_blank;
    this->log_prob_non_blank = copy.log_prob_non_blank;
    this->hidden = copy.hidden;
}

Sequence::Sequence(std::vector<int> word_idx_list) {
    this->word_idx_list = word_idx_list;
    this->log_prob_total = NEG_INF;
    this->log_prob_blank = NEG_INF;
    this->log_prob_non_blank = NEG_INF;
}

Sequence::Sequence(std::vector<int> word_idx_list, std::vector<std::vector<int8_t>> hidden) {
    this->word_idx_list = word_idx_list;
    this->log_prob_total = NEG_INF;
    this->log_prob_blank = NEG_INF;
    this->log_prob_non_blank = NEG_INF;
    this->hidden = hidden;
}

bool Sequence::append_last_word(int word) {
    this->word_idx_list.push_back(word);
    return true;
}

int Sequence::get_last_word() {
    if (this->word_idx_list.empty()) {
        return -1;
    } else {
        int len = this->word_idx_list.size();
        return this->word_idx_list[len-1];
    }
}




SequenceList::SequenceList() = default;

SequenceList::SequenceList(std::vector<std::shared_ptr<Sequence>> seq_list) {
    this->sequence_list = seq_list;
}

int SequenceList::get_length() {
    return this->sequence_list.size();
}

std::shared_ptr<Sequence> SequenceList::get_sequence(int index) {
    if (index >= this->sequence_list.size()) {
        std::cout << "SequenceList::get_sequence Index out of range" << std::endl;
        return nullptr;
    }
    return this->sequence_list[index];
}

std::shared_ptr<Sequence> SequenceList::find_sequence_with_same_words(std::shared_ptr<Sequence> sequence) {
    std::map<std::vector<int>, int>::iterator it;
    if ((it = loop_up_table.find(sequence->get_word_idx_list())) == loop_up_table.end()) {
        return std::make_shared<Sequence>(sequence->get_word_idx_list());
    } else {
        int matched_idx = it->second;
        return this->sequence_list[matched_idx];
    }
}

bool SequenceList::append_sequence(std::shared_ptr<Sequence> sequence) {
    std::map<std::vector<int>, int>::iterator it;
    if ((it = loop_up_table.find(sequence->get_word_idx_list())) == loop_up_table.end()) {
        // word indices list is not in current beam list
        auto item = std::pair<std::vector<int>, int>(sequence->get_word_idx_list(), this->sequence_list.size());
        this->sequence_list.push_back(sequence);
        this->loop_up_table.insert(item);
    } else {
        // if word list of given sequence is already in beam list, only update it.
        int matched_idx = it->second;
        this->sequence_list[matched_idx] = sequence;
    }
    return true;
}

bool compareSequence(std::shared_ptr<Sequence> s1, std::shared_ptr<Sequence> s2) {
    float prob_s1 = log_sum_exp({s1->get_log_prob_blank(), s1->get_log_prob_non_blank()});
    float prob_s2 = log_sum_exp({s2->get_log_prob_blank(), s2->get_log_prob_non_blank()});
    return (prob_s1 > prob_s2);
}

std::shared_ptr<SequenceList> SequenceList::update_beam(int beam_width) {
    sort(this->sequence_list.begin(), this->sequence_list.end(), compareSequence);
    int seq_length = this->sequence_list.size() > beam_width ? beam_width : this->sequence_list.size();
    std::vector<std::shared_ptr<Sequence>> seq_list(this->sequence_list.begin(),
                                                    this->sequence_list.begin()+seq_length);
    return std::make_shared<SequenceList>(seq_list);
}

#include "huffman.h"

void HuffmanTree::Build(const std::vector<uint8_t> &code_lengths,
                        const std::vector<uint8_t> &values) {
    if (code_lengths.size() > 16) {
        throw std::invalid_argument("Huffman too big");
    }
    tree_.clear();
    state_ = {1, 0};
    size_t ind = 0;
    for (; state_.first <= code_lengths.size(); ++state_.first) {
        for (uint8_t i = 0; i < code_lengths[state_.first - 1]; ++i) {
            if (ind == values.size() || (state_.second & (1u << (state_.first)))) {
                throw std::invalid_argument("Too much values in Huffman");
            }
            tree_[state_] = values[ind];
            ++ind;
            ++state_.second;
        }
        state_.second <<= 1;
    }
    state_ = {1, 0};
}

bool HuffmanTree::Move(bool bit, int &value) {
    if (tree_.empty() || state_.first > 16) {
        throw std::invalid_argument("Huffman empty");
    }
    state_.second |= bit;
    auto ptr = tree_.find(state_);
    if (ptr != tree_.end()) {
        value = ptr->second;
        // tree_.erase(ptr);
        state_ = {1, 0};
        return true;
    }
    state_.second <<= 1;
    ++state_.first;
    // or maybe throw if state_.first == 17
    return false;
}
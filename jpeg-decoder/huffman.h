#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <map>

// HuffmanTree decoder for DHT section.
class HuffmanTree {
public:
    // code_lengths is the array of size no more than 16 with number of
    // terminated nodes in the Huffman tree.
    // values are the values of the terminated nodes in the consecutive
    // level order.
    void Build(const std::vector<uint8_t>& code_lengths, const std::vector<uint8_t>& values);

    // Moves the state of the huffman tree by |bit|. If the node is terminated,
    // returns true and overwrites |value|. If it is intermediate, returns false
    // and value is unmodified.
    bool Move(bool bit, int& value);

    std::map<std::pair<uint8_t, uint16_t>, uint8_t> tree_;
    std::pair<uint8_t, uint32_t> state_ = {1, 0};
};
#pragma once

#include <string>
#include <cstring>

class StringView {
public:
    StringView(const std::string &new_str, size_t new_begin = 0,
               size_t new_size = std::string::npos) {
        data_ = &new_str[new_begin];
        if (new_size == std::string::npos || new_begin + new_size > new_str.size()) {
            size_ = new_str.size() - new_begin;
        } else {
            size_ = new_size;
        }
    }

    StringView(const char *new_data) : data_(new_data), size_(strlen(new_data)) {
    }

    StringView(const char *new_data, size_t new_size) : data_(new_data), size_(new_size) {
    }

    char operator[](size_t i) const {
        return data_[i];
    }

    [[nodiscard]] size_t Size() const {
        return size_;
    }

private:
    const char *data_;
    size_t size_;
};
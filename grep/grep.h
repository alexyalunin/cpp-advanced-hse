#pragma once

#include <optional>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <experimental>
#include "utf8.h"

using namespace std;
using std::optional;

struct GrepOptions {
    optional<size_t> look_ahead_length;
    size_t max_matches_per_line;

    GrepOptions() {
        max_matches_per_line = 10;
    }

    GrepOptions(size_t look_ahead_length) : GrepOptions() {
        this->look_ahead_length = look_ahead_length;
    }

    GrepOptions(optional<size_t> look_ahead_length, size_t max_matches_per_line) {
        this->look_ahead_length = look_ahead_length;
        this->max_matches_per_line = max_matches_per_line;
    }
};

template <class Visitor>
void Grep(const std::string& path, const std::string& pattern, Visitor visitor,
          const GrepOptions& options) {
    if (std::filesystem::is_regular_file(std::filesystem::path(path))) {
        std::ifstream ifs(path);
        if (!ifs) {
            return;
        }
        std::istreambuf_iterator<char> it(ifs.rdbuf());
        std::istreambuf_iterator<char> eos;
        if (utf8::is_valid(it, eos)) {
            std::aligned_storage_t<sizeof(const char32_t), alignof(const char32_t)> storage[utf8::distance(it, eos)];
            utf8::utf8to32(it, eos, &storage);
            visitor.OnMatch(path, 0, 0, "smth");
        } else {
            visitor.OnError("Not valid UTF-8");
        }
    } else if (std::filesystem::is_directory(std::filesystem::path(path))) {
        for (auto& p : std::filesystem::recursive_directory_iterator(path)) {
            if (std::filesystem::is_regular_file(p.path())) {
                std::ifstream ifs(p.path());
                if (!ifs) {
                    continue;
                }
                std::istreambuf_iterator<char> it(ifs.rdbuf());
                std::istreambuf_iterator<char> eos;
                if (utf8::is_valid(it, eos)) {
                    std::aligned_storage_t<sizeof(const char32_t), alignof(const char32_t)> storage[utf8::distance(it, eos)];
                    utf8::utf8to32(it, eos, &storage);
                    visitor.OnMatch(path, 0, 0, "smth");
                } else {
                    visitor.OnError("Not valid UTF-8");
                }
            }
        }
    }
}

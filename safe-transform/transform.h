#pragma once
#include <iterator>
#include <vector>

template <typename InputIt, typename P, typename F>
void TransformIf(InputIt begin, InputIt end, P p, F f) {
    InputIt iter = begin, changed = begin;
    bool copy_fails = false;
    std::vector<typename std::iterator_traits<InputIt>::value_type> backup;
    backup.reserve(1'000'000);
    try {
        for (; iter != end; ++iter) {
            if (!copy_fails) {
                try {
                    backup.push_back(*iter);
                    ++changed;
                } catch (...) {
                    copy_fails = true;
                }
            }
            if (p(*iter)) {
                f(*iter);
            }
        }
    } catch (...) {
        InputIt it = begin;
        for (size_t ind = 0; it != changed; ++ind, ++it) {
            *it = backup[ind];
        }
        throw;
    }
}
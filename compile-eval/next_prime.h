#pragma once

#include <algorithm>
#include <cstdint>

constexpr int64_t next_prime(int x) {
    for (int64_t i = std::max(2, x); i <= 2 * x; ++i) {
        bool flag = true;
        for (int64_t j = 2; j * j <= i; ++j) {
            if (i % j == 0) {
                flag = false;
                break;
            }
        }
        if (flag) {
            return i;
        }
    }
    return -1;
}
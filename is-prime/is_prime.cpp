#include "is_prime.h"
#include <cmath>
#include <algorithm>
#include <thread>
#include <vector>
#include <atomic>
void Check(uint64_t start, uint64_t end, uint64_t x, std::atomic_bool& ans) {
    for (auto i = start; i <= end; ++i) {
        if (!ans) {
            return;
        } else if (x % i == 0) {
            ans = false;
            return;
        }
    }
}

bool IsPrime(uint64_t x) {
    if (x <= 1) {
        return false;
    } else if (x <= 10) {
        return x == 2 || x == 3 || x == 5 || x == 7;
    }
    uint64_t root = static_cast<uint64_t>(sqrt(x)), t = std::thread::hardware_concurrency();
    if (root <= t) {
        t = 1;
    }
    std::atomic_bool ans = true;
    std::vector<std::thread> threads;
    auto dist = root / t + 1;
    for (uint64_t i = 0; i < t; ++i) {
        threads.emplace_back(Check, 2 + i * (dist), std::min(2 + (i + 1) * (dist), root), x,
                             std::ref(ans));
    }
    for (size_t i = 0; i < t; ++i) {
        threads[i].join();
    }
    return ans;
}
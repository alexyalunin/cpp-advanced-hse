#pragma once

#include <cstdint>

constexpr int64_t hash(const char *s, int64_t p, int64_t mod) {
    return (*s != '\0') ? ((hash(s + 1, p, mod) * p) % mod + *s) % mod : 0;
}
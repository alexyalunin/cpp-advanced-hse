#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include <atomic>
#include <functional>
#include <tuple>
#include <type_traits>
#include <unistd.h>

bool StartsWith(std::string_view str, std::string_view text);

bool EndsWith(std::string_view str, std::string_view text);

std::string_view StripPrefix(std::string_view str, std::string_view prefix);

std::string_view StripSuffix(std::string_view str, std::string_view suffix);

std::string_view ClippedSubstr(std::string_view s, size_t pos, size_t n = std::string::npos);

std::string_view StripAsciiWhitespace(std::string_view str);

std::vector<std::string_view> StrSplit(std::string_view text, std::string_view delim);

std::string ReadN(const std::string &filename, size_t n);

std::string AddSlash(std::string_view path);

std::string_view RemoveSlash(std::string_view path);

std::string_view Dirname(std::string_view path);

std::string_view Basename(std::string_view path);

std::string CollapseSlashes(std::string_view path);

std::string StrJoin(const std::vector<std::string_view> &strings, std::string_view delimiter);

template <typename T>
size_t MyLength(const T &value) {
    if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int> ||
                  std::is_same_v<T, long> || std::is_same_v<T, unsigned long>) {
        return 11;
    } else if constexpr (std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>) {
        return 20;
    } else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view> ||
                         std::is_same_v<std::decay_t<T>, const char *>) {
        return value.size();
    } else {
        return sizeof(T) - 1;
    }
}
template <typename T>
size_t MyWrite(const T &value, std::string &buff, size_t ind) {
    if constexpr (std::is_same_v<T, int>) {
        return std::sprintf(&buff[ind], "%d", value);
    } else if constexpr (std::is_same_v<T, unsigned int>) {
        return std::sprintf(&buff[ind], "%u", value);
    } else if constexpr (std::is_same_v<T, long>) {
        return std::sprintf(&buff[ind], "%ld", value);
    } else if constexpr (std::is_same_v<T, unsigned long>) {
        return std::sprintf(&buff[ind], "%lu", value);
    } else if constexpr (std::is_same_v<T, long long>) {
        return std::sprintf(&buff[ind], "%lld", value);
    } else if constexpr (std::is_same_v<T, unsigned long long>) {
        return std::sprintf(&buff[ind], "%llu", value);
    } else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view> ||
                         std::is_same_v<T, const char *>) {
        for (size_t j = 0; j < value.size(); ++j) {
            buff[ind + j] = value[j];
        }
        return value.size();
    } else {
        for (size_t j = 0; j < sizeof(value) - 1; ++j) {
            buff[ind + j] = value[j];
        }
        return sizeof(value) - 1;
    }
}

std::string StrCat();

template <typename A>
std::string StrCat(const A &value) {
    size_t res = MyLength(value);
    std::string a(res, ' ');
    size_t beg = 0;
    beg += MyWrite(value, a, beg);
    while (beg != a.size()) {
        a.pop_back();
    }
    return a;
}

template <typename A, typename B>
std::string StrCat(const A &value_1, const B &value_2) {
    size_t res = MyLength(value_1) + MyLength(value_2);
    std::string a(res, ' ');
    size_t beg = 0;
    beg += MyWrite(value_1, a, beg);
    beg += MyWrite(value_2, a, beg);
    while (beg != a.size()) {
        a.pop_back();
    }
    return a;
}

template <typename A, typename B, typename C>
std::string StrCat(const A &value_1, const B &value_2, const C &value_3) {
    size_t res = MyLength(value_1) + MyLength(value_2) + MyLength(value_3);
    std::string a(res, ' ');
    size_t beg = 0;
    beg += MyWrite(value_1, a, beg);
    beg += MyWrite(value_2, a, beg);
    beg += MyWrite(value_3, a, beg);
    while (beg != a.size()) {
        a.pop_back();
    }
    return a;
}

template <typename A, typename B, typename C, typename D>
std::string StrCat(const A &value_1, const B &value_2, const C &value_3, const D &value_4) {
    size_t res = MyLength(value_1) + MyLength(value_2) + MyLength(value_3) + MyLength(value_4);
    std::string a(res, ' ');
    size_t beg = 0;
    beg += MyWrite(value_1, a, beg);
    beg += MyWrite(value_2, a, beg);
    beg += MyWrite(value_3, a, beg);
    beg += MyWrite(value_4, a, beg);
    while (beg != a.size()) {
        a.pop_back();
    }
    return a;
}

template <typename A, typename B, typename C, typename D, typename E>
std::string StrCat(const A &value_1, const B &value_2, const C &value_3, const D &value_4,
                   const E &value_5) {
    size_t res = MyLength(value_1) + MyLength(value_2) + MyLength(value_3) + MyLength(value_4) +
                 MyLength(value_5);
    std::string a(res, ' ');
    size_t beg = 0;
    beg += MyWrite(value_1, a, beg);
    beg += MyWrite(value_2, a, beg);
    beg += MyWrite(value_3, a, beg);
    beg += MyWrite(value_4, a, beg);
    beg += MyWrite(value_5, a, beg);
    while (beg != a.size()) {
        a.pop_back();
    }
    return a;
}
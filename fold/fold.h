#pragma once

#include <vector>

struct Sum {
    int operator()(int left, int right) {
        return left + right;
    }
};

struct Prod {
    int operator()(int left, int right) {
        return left * right;
    }
};

struct Concat {
    template <typename T>
    std::vector<T> operator()(std::vector<T>& left, const std::vector<T>& right) {
        left.reserve(left.size() + right.size());
        for (T elem : right) {
            left.push_back(std::move(elem));
        }
        return left;
    }
};

class Length {
private:
    int* ans_;

public:
    explicit Length(int* ptr) : ans_(ptr) {
    }
    template <typename T>
    T operator()(const T&, const T&) {
        ++*ans_;
        return T();
    }
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    while (first != last) {
        init = func(init, *first);
        ++first;
    }
    return init;
}
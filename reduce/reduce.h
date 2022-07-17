#pragma once
#include <vector>
#include <iterator>
#include <thread>
#include <deque>
template <class RandomAccessIterator, class T, class Func>
void Solve(RandomAccessIterator first, RandomAccessIterator last, Func& func, T& ans) {
    while (first != last) {
        ans = func(ans, *first++);
    }
}

template <class RandomAccessIterator, class T, class Func>
T Reduce(RandomAccessIterator first, RandomAccessIterator last, const T& initial_value, Func func) {
    size_t n = last - first, amount = std::thread::hardware_concurrency();
    T ans(initial_value);
    if (n <= amount) {
        amount = 1;
    }
    size_t size = n / amount;
    std::deque<T> res(amount, initial_value);
    std::vector<std::thread> threads;
    threads.reserve(amount);
    for (size_t i = 0; i < amount - 1; ++i) {
        threads.emplace_back([](RandomAccessIterator f, RandomAccessIterator l, Func& ff,
                                T& a) { Solve(f, l, ff, a); },
                             first + i * size, first + (i + 1) * size, std::ref(func),
                             std::ref(res[i]));
    }
    threads.template emplace_back(
        [](RandomAccessIterator f, RandomAccessIterator l, Func& ff, T& a) { Solve(f, l, ff, a); },
        first + (amount - 1) * size, last, std::ref(func), std::ref(res[amount - 1]));
    for (size_t i = 0; i < amount; ++i) {
        threads[i].join();
        ans = func(ans, res[i]);
    }
    return ans;
}
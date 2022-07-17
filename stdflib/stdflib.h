#pragma once
#include <utility>
#include <cstddef>

template <size_t N>
constexpr auto Fib() {
    if constexpr (N < 2) {
        return [] { return N; };
    } else {
        return [] { return Fib<N - 1>()() + Fib<N - 2>()(); };
    }
}

template <size_t N>
constexpr auto Fact() {
    if constexpr (N < 2) {
        return [] { return 1; };
    } else {
        return [] { return Fact<N - 1>()() * N; };
    }
}

template <size_t A, size_t B>
constexpr auto GCD() {
    if constexpr (B == 0) {
        return [] { return A; };
    } else if constexpr (A < B) {
        return GCD<B, A>();
    } else {
        return GCD<B, A % B>();
    }
}

template <size_t A, size_t H = 2>
constexpr auto Prime() -> bool (*)() {
    if constexpr (A < 2) {
        return [] { return false; };
    } else if constexpr (H * H > A) {
        return [] { return true; };
    } else if constexpr (A % H == 0) {
        return [] { return false; };
    } else {
        return Prime<A, H + 1>();
    }
}

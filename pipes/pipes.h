#pragma once

#include <utility>
#include <tuple>

/// You can use template arguments if you want
template <typename F>
struct PipeLine {
    constexpr auto operator|(auto&& f) const {
        auto lamb = [func = std::forward<decltype(f)>(f), ff = func_](auto&& arg) {
            return func(ff(std::forward<decltype(arg)>(arg)));
        };
        return PipeLine<decltype(lamb)>(std::forward<decltype(lamb)>(lamb));  /// 1
    }

    template <class Arg>
    constexpr auto operator()(Arg&& arg) const {
        return func_(std::forward<Arg>(arg));  /// 2
    }

    PipeLine(F&& func) : func_(std::forward<F>(func)) {
    }
    F func_;
};

struct EmptyPlHelper {
    constexpr auto operator|(auto&& f) const {
        return PipeLine<decltype(f)>(std::forward<decltype(f)>(f));  /// 3
    }
};

constexpr auto kEmptyPl{EmptyPlHelper()};
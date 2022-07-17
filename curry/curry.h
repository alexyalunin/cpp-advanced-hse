#pragma once

#include <utility>
#include <tuple>

template <class F>
constexpr auto Curry(F&& f) {
    return [func = std::forward<F>(f)](auto&&... variadic) mutable {
        return func(std::make_tuple(std::forward<decltype(variadic)>(variadic)...));
    };
}

template <class F>
constexpr auto Uncurry(F&& f) {
    return [func = std::forward<F>(f)](auto&& tuple) mutable { return std::apply(func, tuple); };
}

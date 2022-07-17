#pragma once

#include <functional>
#include <utility>

template <class F, class... Args>
constexpr auto BindFront(F&& f, Args&&... args) {
    return [func = std::forward<F>(f),
            ... fixed = std::forward<Args>(args)](auto&&... variadic) mutable {
        return func(std::forward<decltype(fixed)>(fixed)...,
                    std::forward<decltype(variadic)>(variadic)...);
    };
}
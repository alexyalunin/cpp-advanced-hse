#pragma once
#include <iostream>

template <typename T, size_t I, bool = std::is_empty_v<T> && !std::is_final_v<T>>
struct CompressedPairElement {
    CompressedPairElement() : value_(T()){};

    CompressedPairElement(T new_value) : value_(std::forward<T>(new_value)) {
    }

    [[nodiscard]] const T &Get() const {
        return value_;
    }

    T &Get() {
        return value_;
    }

    T value_;
};

template <typename T, size_t I>
struct CompressedPairElement<T, I, true> : public T {
    CompressedPairElement() = default;

    CompressedPairElement(T &&) {
    }

    const T &Get() const {
        return static_cast<const T &>(*this);
    }

    T &Get() {
        return static_cast<T &>(*this);
    }
};

template <typename F, typename S>
class CompressedPair : private CompressedPairElement<F, 0>, private CompressedPairElement<S, 1> {
    using First = CompressedPairElement<F, 0>;
    using Second = CompressedPairElement<S, 1>;

public:
    // Constructors.
    CompressedPair() = default;

    CompressedPair(F first, S second)
        : First(std::forward<F>(first)), Second(std::forward<S>(second)) {
    }

    // Getters for first and second.
    F &GetFirst() {
        return static_cast<First &>(*this).Get();
    }

    const F &GetFirst() const {
        return static_cast<const First &>(*this).Get();
    }

    S &GetSecond() {
        return static_cast<Second &>(*this).Get();
    }

    const S &GetSecond() const {
        return static_cast<const Second &>(*this).Get();
    };
};
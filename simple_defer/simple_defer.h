#pragma once

#include <memory>

#define create_name(f, s) f##s
#define unique_name(f, s) create_name(f, s)
#define defer Defer unique_name(defer_, __COUNTER__) = [&]

template <typename Callback>
class Defer {
public:
    constexpr Defer(Callback&& callback) : callback_(std::forward<Callback>(callback)) {
    }
    constexpr ~Defer() {
        callback_();
    }

private:
    Callback callback_;
};
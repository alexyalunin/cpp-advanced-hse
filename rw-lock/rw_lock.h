#pragma once

#include <atomic>
#include <mutex>
#include <thread>

#include <memory>

#define create_name(f, s) f##s
#define unique_name(f, s) create_name(f, s)
#define defer Defer unique_name(defer_, __LINE__) = [&]

template <typename Callback>
class Defer {
public:
    Defer(Callback&& callback) : callback_(std::forward<Callback>(callback)) {
    }
    ~Defer() {
        callback_();
    }

private:
    Callback callback_;
};

#define BIG 2281337

class RWLock {
public:
    template <typename Func>
    void Read(Func func = {}) {
        while (counter_.fetch_sub(1) <= 0) {
            counter_.fetch_add(1);
            std::this_thread::yield();
        }
        defer {
            counter_.fetch_add(1);
        };
        func();
    }

    template <typename Func>
    void Write(Func func = {}) {
        while (counter_.fetch_sub(BIG) <= BIG - 1) {
            counter_.fetch_add(BIG);
            std::this_thread::yield();
        }
        defer {
            counter_.fetch_add(BIG);
        };
        func();
    }

private:
    std::atomic<int> counter_ = BIG;
};
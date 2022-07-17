#pragma once

#include <linux/futex.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>
#include <queue>
#include <atomic>

class Futex {
public:
    Futex() : locked_(0) {
    }
    void Lock() {
        int expected = 0;
        while (!locked_.compare_exchange_weak(expected, 1)) {
        }
        if (expected) {
            if (expected == 1) {
                expected = locked_.exchange(2);
            }
            while (expected) {
                syscall(SYS_futex, &locked_, FUTEX_WAIT_PRIVATE, 2, nullptr, nullptr, 0);
                expected = locked_.exchange(2);
            }
        }
    }

    void Unlock() {
        if (locked_.exchange(0) == 2) {
            syscall(SYS_futex, &locked_, FUTEX_WAKE_PRIVATE, 1, nullptr, nullptr, 0);
        }
    }

private:
    std::atomic_int locked_;
};

template <class T>
class MPMCBoundedQueue {
public:
    explicit MPMCBoundedQueue(size_t size) {
        max_size_ = size;
    }

    bool Enqueue(const T& value) {
        mutex_.Lock();
        if (queue_.size() == max_size_) {
            mutex_.Unlock();
            return false;
        }
        queue_.push(value);
        mutex_.Unlock();
        return true;
    }

    bool Dequeue(T& data) {
        mutex_.Lock();
        if (queue_.empty()) {
            mutex_.Unlock();
            return false;
        }
        data = std::move(queue_.front());
        queue_.pop();
        mutex_.Unlock();
        return true;
    }

private:
    std::queue<T> queue_;
    Futex mutex_;
    size_t max_size_ = 0;
};

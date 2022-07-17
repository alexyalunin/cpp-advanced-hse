#pragma once

#include <linux/futex.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

#include <mutex>
#include <atomic>

class Mutex {
public:
    Mutex() : locked_(0) {
    }
    void Lock() {
        int expected = 0;
        while (!locked_.compare_exchange_weak(expected, 1)) {
        }
        if (expected) {
            expected = locked_.exchange(2);
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

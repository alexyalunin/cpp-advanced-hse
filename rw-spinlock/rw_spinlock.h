#pragma once

#include <atomic>

struct RWSpinLock {
    RWSpinLock() : amount_(0) {
    }
    void LockRead() {
        while (amount_.fetch_add(2) == 1) {
            amount_.fetch_sub(2);
        }
    }

    void UnlockRead() {
        amount_.fetch_sub(2);
    }

    void LockWrite() {
        while (amount_.fetch_add(1)) {
            amount_.fetch_sub(1);
        }
    }

    void UnlockWrite() {
        amount_.fetch_sub(1);
    }

    std::atomic_int amount_;
};

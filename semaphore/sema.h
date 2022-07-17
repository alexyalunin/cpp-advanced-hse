#pragma once

#include <mutex>
#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <thread>

class DefaultCallback {
public:
    void operator()(int& value) {
        --value;
    }
};

class Semaphore {
public:
    explicit Semaphore(int count) : count_(count) {
        leaver_.emplace_back([this]() mutable {
            while (valid_) {
                std::unique_lock<std::mutex> lock(mutex_);
                left_ = false;
                not_empty_.wait(lock, [this] { return !history_.empty() || !valid_; });
                if (!valid_) {
                    continue;
                }
                one_left_.wait(lock, [this] { return left_ || count_ > 0; });
                auto function = history_.front();
                function(count_);
                history_.pop_front();
            }
        });
    }

    void Leave() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            left_ = true;
            ++count_;
        }
        one_left_.notify_all();
    }

    template <class Func>
    void Enter(Func callback) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            history_.emplace_back(std::move(callback));
        }
        not_empty_.notify_all();
    }

    void Enter() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            history_.emplace_back(DefaultCallback{});
        }
        not_empty_.notify_all();
    }

    ~Semaphore() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            valid_ = false;
        }
        not_empty_.notify_all();
    }

private:
    std::vector<std::jthread> leaver_;
    int count_;
    bool left_ = false;
    bool empty_ = false;
    bool valid_ = true;
    std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable one_left_;
    std::deque<std::function<void(int&)>> history_;
};
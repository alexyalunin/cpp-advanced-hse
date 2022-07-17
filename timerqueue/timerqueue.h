#pragma once

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <set>

template <class T>
class TimerQueue {
public:
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;

public:
    void Add(const T& item, TimePoint at) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            buffer_.insert({at, item});
        }
        wait_first_.notify_one();
    }

    T Pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        wait_first_.wait(lock, [this] { return buffer_.size() > 0; });
        TimePoint maybe = buffer_.begin()->first;
        wait_first_.wait_until(lock, maybe);
        auto top = buffer_.begin()->second;
        buffer_.erase(buffer_.begin());
        return top;
    }

private:
    std::mutex mutex_;
    std::condition_variable wait_first_;
    std::set<std::pair<TimePoint, T>> buffer_;
};

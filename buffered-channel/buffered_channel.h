#pragma once

#include <utility>
#include <optional>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <exception>
template <class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size) : size_(size) {
        // Your code goes here
    }

    void Send(const T& value) {
        // Your code goes here
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (!valid_) {
                throw std::runtime_error("");
            }
            full_.wait(lock, [this] { return buffer_.size() < size_ || !valid_; });
            if (!valid_) {
                throw std::runtime_error("");
            }
            buffer_.push_back(value);
        }
        empty_.notify_one();
    }

    std::optional<T> Recv() {
        // Your code goes here
        std::unique_lock<std::mutex> lock(mutex_);
        empty_.wait(lock, [this] { return !buffer_.empty() || !valid_; });
        if (buffer_.empty()) {
            return std::nullopt;
        }
        auto val = buffer_.front();
        buffer_.pop_front();
        lock.unlock();
        full_.notify_one();
        return val;
    }

    void Close() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            valid_ = false;
        }
        full_.notify_all();
        empty_.notify_all();
    }

private:
    int size_ = 0;
    std::mutex mutex_;
    std::condition_variable full_;
    std::condition_variable empty_;
    bool valid_ = true;
    std::deque<T> buffer_;
};
#pragma once
#pragma GCC optimize("O3")
#include <optional>
#include <mutex>
#include <condition_variable>

template <class T>
class UnbufferedChannel {
public:
    void Send(const T& value) {
        // Your code goes here
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (!valid_) {
                throw std::runtime_error("");
            }
            send_.wait(lock, [this] { return value_empty_ || !valid_; });
            if (!valid_) {
                throw std::runtime_error("");
            }
            value_empty_ = false;
            value_ = value;
            read_.notify_one();
            write_.wait(lock, [this] { return written_ || !valid_; });
            written_ = false;
        }
        send_.notify_one();
    }

    std::optional<T> Recv() {
        // Your code goes here
        std::optional<T> ret;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            read_.wait(lock, [this] { return !value_empty_ || !valid_; });
            value_empty_ = true;
            ret = value_;
            value_.reset();
            written_ = true;
            lock.unlock();
            write_.notify_one();
        }
        return ret;
    }

    void Close() {
        // Your code goes here
        {
            std::unique_lock<std::mutex> lock(mutex_);
            valid_ = false;
        }
        send_.notify_all();
        read_.notify_all();
    }

private:
    std::condition_variable read_;
    std::condition_variable send_;
    std::condition_variable write_;
    std::mutex mutex_;
    std::optional<T> value_;
    bool print_ = true;
    bool value_empty_ = true;
    bool valid_ = true;
    bool written_ = false;
};
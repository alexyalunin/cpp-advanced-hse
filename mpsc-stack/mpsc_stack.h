#pragma once

#include <atomic>
#include <optional>
#include <stdexcept>
#include <utility>

template <class T>
class MPSCStack {
public:
    MPSCStack() : amount_(0), head_(nullptr) {
    }
    // Push adds one element to stack top.
    //
    // Safe to call from multiple threads.
    void Push(const T& value) {
        while (amount_.fetch_add(2) == 1) {
            amount_.fetch_sub(2);
        }
        auto new_head = new Node{value, head_.load()};
        while (!head_.compare_exchange_weak(new_head->next, new_head)) {
        }
        amount_.fetch_sub(2);
    }

    // Pop removes top element from the stack.
    //
    // Not safe to call concurrently.
    std::optional<T> Pop() {
        while (amount_.fetch_add(1)) {
            amount_.fetch_sub(1);
        }
        auto head = head_.exchange(nullptr);
        if (!head) {
            amount_.fetch_sub(1);
            return std::nullopt;
        } else {
            head_ = head->next;
            auto value = std::optional<T>(head->value);
            delete head;
            amount_.fetch_sub(1);
            return value;
        }
    }

    // DequeuedAll Pop's all elements from the stack and calls cb() for each.
    //
    // Not safe to call concurrently with Pop()
    template <class TFn>
    void DequeueAll(const TFn& cb) {
        while (amount_.fetch_add(1)) {
            amount_.fetch_sub(1);
        }
        auto head = head_.exchange(nullptr);
        amount_.fetch_sub(1);
        while (head) {
            cb(head->value);
            auto next = head->next;
            delete head;
            head = next;
        }
    }

    ~MPSCStack() {
        DequeueAll([](auto) {});
    }

private:
    struct Node {
        T value;
        Node* next;
    };
    std::atomic_int amount_;
    std::atomic<Node*> head_;
};
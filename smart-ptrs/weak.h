#pragma once

#include "sw_fwd.h"  // Forward declaration

template <typename T>
class WeakPtr {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    WeakPtr() : ptr_(nullptr), block_(nullptr) {
    }

    WeakPtr(const WeakPtr& other) : ptr_(other.ptr_), block_(other.block_) {
        if (block_) {
            block_->IncWeak();
        }
    }
    WeakPtr(WeakPtr&& other) noexcept : ptr_(other.ptr_), block_(other.block_) {
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }

    template <typename S>
    WeakPtr(const WeakPtr<S>& other) : ptr_(other.ptr_), block_(other.block_) {
        if (block_) {
            block_->IncWeak();
        }
    }
    template <typename S>
    WeakPtr(WeakPtr&& other) noexcept : ptr_(other.ptr_), block_(other.block_) {
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }

    // Demote `SharedPtr`
    // #2 from https://en.cppreference.com/w/cpp/memory/weak_ptr/weak_ptr
    WeakPtr(const SharedPtr<T>& other) : ptr_(other.ptr_), block_(other.block_) {
        if (block_) {
            block_->IncWeak();
        }
    }

    template <typename S>
    WeakPtr(const SharedPtr<S>& other) : ptr_(other.ptr_), block_(other.block_) {
        if (block_) {
            block_->IncWeak();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    WeakPtr& operator=(const WeakPtr& other) {
        if (this == &other) {
            return *this;
        }
        this->~WeakPtr();
        ptr_ = other.ptr_;
        block_ = other.block_;
        if (block_) {
            block_->IncWeak();
        }
        return *this;
    }
    WeakPtr& operator=(WeakPtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        this->~WeakPtr();
        ptr_ = other.ptr_;
        block_ = other.block_;
        other.ptr_ = nullptr;
        other.block_ = nullptr;
        return *this;
    }

    template <typename S>
    WeakPtr& operator=(const WeakPtr<S>& other) {
        this->~WeakPtr();
        ptr_ = other.ptr_;
        block_ = other.block_;
        if (block_) {
            block_->IncWeak();
        }
        return *this;
    }
    template <typename S>
    WeakPtr& operator=(WeakPtr<S>&& other) noexcept {
        this->~WeakPtr();
        ptr_ = other.ptr_;
        block_ = other.block_;
        other.ptr_ = nullptr;
        other.block_ = nullptr;
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~WeakPtr() {
        if (block_) {
            block_->DecWeak();
            ptr_ = nullptr;
            auto obj = block_;
            block_ = nullptr;
            if (obj->strong_ == 0 && obj->weak_ == 0) {
                obj->OnZeroWeak();
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    void Reset() {
        this->~WeakPtr();
    }
    void Swap(WeakPtr& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(block_, other.block_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    [[nodiscard]] size_t UseCount() const {
        if (block_) {
            return block_->GetStrong();
        }
        return 0;
    }
    [[nodiscard]] bool Expired() const {
        if (block_) {
            return block_->GetStrong() == 0;
        }
        return true;
    }
    [[nodiscard]] SharedPtr<T> Lock() const {
        if (Expired()) {
            return SharedPtr<T>(nullptr);
        }
        block_->IncStrong();
        return SharedPtr<T>(ptr_, block_);
    }

    friend class SharedPtr<T>;
    T* ptr_ = nullptr;
    ControlBlockBase* block_ = nullptr;
};
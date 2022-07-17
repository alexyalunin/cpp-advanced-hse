#pragma once

#include "sw_fwd.h"  // Forward declaration

#include <cstddef>  // std::nullptr_t
#include <iostream>
// https://en.cppreference.com/w/cpp/memory/shared_ptr

struct EnableSharedBase {};

template <typename T>
class EnableSharedFromThis : public EnableSharedBase {
public:
    SharedPtr<T> SharedFromThis() {
        return SharedPtr<T>(self_);
    }
    [[nodiscard]] SharedPtr<const T> SharedFromThis() const {
        return self_;
    }

    WeakPtr<T> WeakFromThis() noexcept {
        return self_;
    }
    [[nodiscard]] WeakPtr<const T> WeakFromThis() const noexcept {
        return self_;
    }
    friend class SharedPtr<T>;
    friend class WeakPtr<T>;
    WeakPtr<T> self_;
};

template <typename T>
class SharedPtr {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    SharedPtr() : ptr_(nullptr), block_(nullptr) {
    }
    explicit SharedPtr(std::nullptr_t) : ptr_(nullptr), block_(nullptr) {
    }
    explicit SharedPtr(T* ptr) : ptr_(ptr), block_(new ControlBlockPointer<T>(ptr)) {
        if constexpr (std::is_convertible_v<T*, EnableSharedBase*>) {
            EnableSharedFromThisHelper(ptr);
        }
    }

    template <typename S>
    explicit SharedPtr(S* ptr) : ptr_(ptr), block_(new ControlBlockPointer<S>(ptr)) {
        if constexpr (std::is_convertible_v<T*, EnableSharedBase*>) {
            EnableSharedFromThisHelper(ptr);
        }
    }
    SharedPtr(T* ptr, ControlBlockBase* block) : ptr_(ptr), block_(block) {
        if constexpr (std::is_convertible_v<T*, EnableSharedBase*>) {
            EnableSharedFromThisHelper(ptr);
        }
    }

    SharedPtr(const SharedPtr& other) : ptr_(other.Get()), block_(other.GetBlock()) {
        if (block_) {
            block_->IncStrong();
        }
    }
    SharedPtr(SharedPtr&& other) noexcept : ptr_(other.Get()), block_(other.GetBlock()) {
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }

    template <typename S>
    SharedPtr(const SharedPtr<S>& other) : ptr_(other.Get()), block_(other.GetBlock()) {
        if (block_) {
            block_->IncStrong();
        }
    }
    template <typename S>
    SharedPtr(SharedPtr<S>&& other) noexcept : ptr_(other.Get()), block_(other.GetBlock()) {
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }

    // Aliasing constructor
    // #8 from https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
    template <typename Y>
    SharedPtr(const SharedPtr<Y>& other, T* ptr) : ptr_(ptr), block_(other.GetBlock()) {
        if (block_) {
            block_->IncStrong();
        }
    }

    // Promote `WeakPtr`
    // #11 from https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
    explicit SharedPtr(const WeakPtr<T>& other) : ptr_(other.ptr_), block_(other.block_) {
        if (other.Expired()) {
            throw BadWeakPtr();
        }
        block_->IncStrong();
    }

    template <typename S>
    explicit SharedPtr(const WeakPtr<S>& other) : ptr_(other.ptr_), block_(other.block_) {
        if (other.Expired()) {
            throw BadWeakPtr();
        }
        block_->IncStrong();
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    SharedPtr& operator=(const SharedPtr& other) {
        if (this == &other) {
            return *this;
        }
        this->~SharedPtr();
        ptr_ = other.Get();
        block_ = other.GetBlock();
        if (block_) {
            block_->IncStrong();
        }
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        this->~SharedPtr();
        ptr_ = other.Get();
        block_ = other.GetBlock();
        other.ptr_ = nullptr;
        other.block_ = nullptr;
        return *this;
    }
    template <typename S>
    SharedPtr& operator=(const SharedPtr<S>& other) {
        this->~SharedPtr();
        ptr_ = other.Get();
        block_ = other.GetBlock();
        if (block_) {
            block_->IncStrong();
        }
        return *this;
    }
    template <typename S>
    SharedPtr& operator=(SharedPtr<S>&& other) noexcept {
        this->~SharedPtr();
        ptr_ = other.Get();
        block_ = other.GetBlock();
        other.ptr_ = nullptr;
        other.block_ = nullptr;
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~SharedPtr() {
        if (GetBlock()) {
            block_->DecStrong();
            if (UseCount() == 0) {
                if (block_->GetWeak() == 0) {
                    block_->OnZeroStrong();
                    ptr_ = nullptr;
                    block_->OnZeroWeak();
                    block_ = nullptr;
                    return;
                }
                block_->OnZeroStrong();
                ptr_ = nullptr;
                block_ = nullptr;
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    void Reset() {
        this->~SharedPtr();
    }
    void Reset(T* ptr) {
        if (Get() == ptr) {
            return;
        }
        this->~SharedPtr();
        ptr_ = ptr;
        block_ = new ControlBlockPointer<T>(ptr);
    }

    template <typename S>
    void Reset(S* ptr) {
        if (Get() == ptr) {
            return;
        }
        this->~SharedPtr();
        ptr_ = ptr;
        block_ = new ControlBlockPointer<S>(ptr);
    }
    void Swap(SharedPtr& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(block_, other.block_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    [[nodiscard]] T* Get() const {
        return ptr_;
    }

    [[nodiscard]] ControlBlockBase* GetBlock() const {
        return block_;
    }
    T& operator*() const {
        return *ptr_;
    }
    T* operator->() const {
        return ptr_;
    }
    [[nodiscard]] size_t UseCount() const {
        if (GetBlock()) {
            return GetBlock()->GetStrong();
        }
        return 0;
    }
    explicit operator bool() const {
        return ptr_ != nullptr;
    }

    T* ptr_;
    ControlBlockBase* block_;
    template <typename Y>
    void EnableSharedFromThisHelper(EnableSharedFromThis<Y>* ptr) {
        if (ptr->self_.Expired()) {
            ptr->self_ = *this;
        }
    }
};

// Allocate memory only once
template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    auto block = new ControlBlockEmplace<T>(std::forward<Args>(args)...);
    return SharedPtr<T>(block->GetPtr(), block);
}

template <typename T, typename U>
inline bool operator==(const SharedPtr<T>& left, const SharedPtr<U>& right) {
    return left.GetBlock() == right.GetBlock();
}

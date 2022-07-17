#pragma once

#include "compressed_pair.h"

#include <cstddef>
#include <memory>  // std::nullptr_t

// Primary template
template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    explicit UniquePtr(T *ptr = nullptr) : unique_(ptr, Deleter()) {
    }

    UniquePtr(T *ptr, Deleter deleter) : unique_(ptr, std::forward<Deleter>(deleter)) {
    }

    UniquePtr(UniquePtr &&other) noexcept {
        unique_.GetFirst() = other.unique_.GetFirst();
        unique_.GetSecond() = std::forward<Deleter>(other.GetDeleter());
        other.unique_.GetFirst() = nullptr;
    }

    template <typename S, typename SDeleter>
    UniquePtr(UniquePtr<S, SDeleter> &&other) noexcept {
        unique_.GetFirst() = other.unique_.GetFirst();
        unique_.GetSecond() = std::forward<SDeleter>(other.GetDeleter());
        other.unique_.GetFirst() = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    UniquePtr &operator=(UniquePtr &&other) noexcept {
        if (this == &other) {
            return *this;
        }
        if (unique_.GetFirst()) {
            unique_.GetSecond()(unique_.GetFirst());
        }
        unique_.GetFirst() = other.unique_.GetFirst();
        unique_.GetSecond() = std::forward<Deleter>(other.GetDeleter());
        other.unique_.GetFirst() = nullptr;
        return *this;
    }

    template <typename S, typename SDeleter>
    UniquePtr &operator=(UniquePtr<S, SDeleter> &&other) noexcept {
        if (unique_.GetFirst()) {
            unique_.GetSecond()(unique_.GetFirst());
        }
        unique_.GetFirst() = other.unique_.GetFirst();
        unique_.GetSecond() = std::forward<SDeleter>(other.GetDeleter());
        other.unique_.GetFirst() = nullptr;
        return *this;
    }

    UniquePtr &operator=(std::nullptr_t) {
        if (unique_.GetFirst()) {
            unique_.GetSecond()(unique_.GetFirst());
        }
        unique_.GetFirst() = nullptr;
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~UniquePtr() {
        if (unique_.GetFirst()) {
            unique_.GetSecond()(unique_.GetFirst());
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    T *Release() {
        T *ptr = unique_.GetFirst();
        unique_.GetFirst() = nullptr;
        return ptr;
    }

    void Reset(T *ptr = nullptr) {
        if (unique_.GetFirst() == ptr) {
            return;
        }
        T *old = unique_.GetFirst();
        unique_.GetFirst() = ptr;
        if (old) {
            unique_.GetSecond()(old);
        }
    }

    void Swap(UniquePtr &other) {
        std::swap(unique_, other.unique_);
    }

    template <typename S, typename SDeleter>
    void Swap(UniquePtr<S, SDeleter> &other) {
        std::swap(unique_, other.unique_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    [[nodiscard]] T *Get() const {
        return unique_.GetFirst();
    }

    Deleter &GetDeleter() {
        return unique_.GetSecond();
    }

    [[nodiscard]] const Deleter &GetDeleter() const {
        return unique_.GetSecond();
    }

    explicit operator bool() const {
        return unique_.GetFirst() != nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Single-object dereference operators

    std::add_lvalue_reference_t<T> operator*() const {
        return *unique_.GetFirst();
    }

    T *operator->() const {
        return unique_.GetFirst();
    }
    CompressedPair<T *, Deleter> unique_;
};

// Specialization for arrays
template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    explicit UniquePtr(T *ptr = nullptr) : unique_(ptr, Deleter()) {
    }

    UniquePtr(T *ptr, Deleter deleter) : unique_(ptr, std::forward<Deleter>(deleter)) {
    }

    UniquePtr(UniquePtr &&other) noexcept {
        unique_.GetFirst() = other.unique_.GetFirst();
        unique_.GetSecond() = std::forward<Deleter>(other.GetDeleter());
        other.unique_.GetFirst() = nullptr;
    }

    template <typename S, typename SDeleter>
    UniquePtr(UniquePtr<S, SDeleter> &&other) noexcept {
        unique_.GetFirst() = other.unique_.GetFirst();
        unique_.GetSecond() = std::forward<SDeleter>(other.GetDeleter());
        other.unique_.GetFirst() = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    UniquePtr &operator=(UniquePtr &&other) noexcept {
        if (this == &other) {
            return *this;
        }
        if (unique_.GetFirst()) {
            unique_.GetSecond()(unique_.GetFirst());
        }
        unique_.GetFirst() = other.unique_.GetFirst();
        unique_.GetSecond() = std::forward<Deleter>(other.GetDeleter());
        other.unique_.GetFirst() = nullptr;
        return *this;
    }

    template <typename S, typename SDeleter>
    UniquePtr &operator=(UniquePtr<S, SDeleter> &&other) noexcept {
        if (unique_.GetFirst()) {
            unique_.GetSecond()(unique_.GetFirst());
        }
        unique_.GetFirst() = other.unique_.GetFirst();
        unique_.GetSecond() = std::forward<SDeleter>(other.GetDeleter());
        other.unique_.GetFirst() = nullptr;
        return *this;
    }

    UniquePtr &operator=(std::nullptr_t) {
        if (unique_.GetFirst()) {
            unique_.GetSecond()(unique_.GetFirst());
        }
        unique_.GetFirst() = nullptr;
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~UniquePtr() {
        if (unique_.GetFirst()) {
            unique_.GetSecond()(unique_.GetFirst());
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    T *Release() {
        T *ptr = unique_.GetFirst();
        unique_.GetFirst() = nullptr;
        return ptr;
    }

    void Reset(T *ptr = nullptr) {
        if (unique_.GetFirst() == ptr) {
            return;
        }
        T *old = unique_.GetFirst();
        unique_.GetFirst() = ptr;
        if (old) {
            unique_.GetSecond()(old);
        }
    }

    void Swap(UniquePtr &other) {
        std::swap(unique_, other.unique_);
    }

    template <typename S, typename SDeleter>
    void Swap(UniquePtr<S, SDeleter> &other) {
        std::swap(unique_, other.unique_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    [[nodiscard]] T *Get() const {
        return unique_.GetFirst();
    }

    T &operator[](size_t ind) {
        return unique_.GetFirst()[ind];
    }

    const T &operator[](size_t ind) const {
        return unique_.GetFirst()[ind];
    }

    Deleter &GetDeleter() {
        return unique_.GetSecond();
    }

    [[nodiscard]] const Deleter &GetDeleter() const {
        return unique_.GetSecond();
    }

    explicit operator bool() const {
        return unique_.GetFirst() != nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Single-object dereference operators

    std::add_lvalue_reference_t<T> operator*() const {
        return *unique_.GetFirst();
    }

    T *operator->() const {
        return unique_.GetFirst();
    }
    CompressedPair<T *, Deleter> unique_;
};

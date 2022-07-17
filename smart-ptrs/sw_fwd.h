#pragma once

#include <exception>
#include <type_traits>
#include <memory>

class BadWeakPtr : public std::exception {};

template <typename T>
class SharedPtr;

template <typename T>
class WeakPtr;

struct ControlBlockBase {
    size_t strong_ = 1;
    size_t weak_ = 0;
    void IncStrong() {
        ++strong_;
    }
    void IncWeak() {
        ++weak_;
    }
    void DecStrong() {
        --strong_;
    }
    void DecWeak() {
        --weak_;
    }
    [[nodiscard]] size_t GetStrong() const {
        return strong_;
    }
    [[nodiscard]] size_t GetWeak() const {
        return weak_;
    }
    virtual ~ControlBlockBase() = default;
    virtual void OnZeroStrong() = 0;
    virtual void OnZeroWeak() = 0;
};

template <typename T>
struct ControlBlockPointer : ControlBlockBase {
    explicit ControlBlockPointer(T* new_ptr) : ptr_(new_ptr) {
    }
    T* ptr_ = nullptr;
    void OnZeroStrong() override {
        auto old = ptr_;
        ptr_ = nullptr;
        delete old;
    }
    void OnZeroWeak() override {
        delete this;
    }
};

template <typename T>
struct ControlBlockEmplace : ControlBlockBase {

    template <typename... Args>
    explicit ControlBlockEmplace(Args&&... args) {
        new (&storage_) T(std::forward<Args>(args)...);
    }
    T* GetPtr() {
        return reinterpret_cast<T*>(&storage_);
    }
    std::aligned_storage_t<sizeof(T), alignof(T)> storage_;
    void OnZeroStrong() override {
        GetPtr()->~T();
    }
    void OnZeroWeak() override {
        delete this;
    }
};
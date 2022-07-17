#pragma once
#include <type_traits>
#include <utility>

template <typename Callback>
class CallbackStorage {
public:
    CallbackStorage(Callback callback) {
        new (&storage_) Callback(std::forward<Callback>(callback));
        exists_ = true;
    }
    void Cancel() && {
        if (exists_) {
            std::move(*reinterpret_cast<Callback*>(&storage_)).~Callback();
            exists_ = false;
        }
    }
    void Invoke() && {
        if (exists_) {
            std::move (*reinterpret_cast<Callback*>(&storage_))();
            std::move(*reinterpret_cast<Callback*>(&storage_)).~Callback();
            exists_ = false;
        }
    }

private:
    bool exists_;
    std::aligned_storage_t<sizeof(Callback), alignof(Callback)> storage_;
};

template <typename Callback>
class Defer final {
public:
    Defer(Callback callback) : storage_(std::forward<Callback>(callback)) {
    }

    void Cancel() && {
        std::move(storage_).Cancel();
    }
    void operator()() && {
        std::move(storage_).Invoke();
    }
    void Invoke() && {
        std::move(storage_).Invoke();
    }
    ~Defer() {
        std::move(storage_).Invoke();
    }

private:
    CallbackStorage<Callback> storage_;
};
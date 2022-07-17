#pragma once

#include <exception>
#include <stdexcept>
#include <cstring>
template <class T>
class Try {
public:
    Try() = default;
    Try(T value) : value_(std::forward<T>(value)) {
        valid_val_ = true;
    }
    template <typename E>
    Try(E exc) {
        try {
            throw exc;
        } catch (...) {
            exc_ = std::current_exception();
        }
        valid_exc_ = true;
    }
    Try(std::exception_ptr exc) : exc_(std::move(exc)) {
        valid_exc_ = true;
    }
    const T& Value() const {
        if (valid_exc_) {
            std::rethrow_exception(exc_);
        }
        if (!valid_val_) {
            throw std::invalid_argument("Object is empty");
        }
        return value_;
    }

    void Throw() {
        if (valid_exc_) {
            std::rethrow_exception(exc_);
        }
        throw std::invalid_argument("No exception");
    }

    bool IsFailed() const {
        return valid_exc_;
    }

private:
    bool valid_val_ = false;
    bool valid_exc_ = false;
    T value_;
    std::exception_ptr exc_;
};
template <>
class Try<void> {
public:
    Try() = default;
    template <typename E>
    Try(E exc) {
        try {
            throw exc;
        } catch (...) {
            exc_ = std::current_exception();
        }
        valid_exc_ = true;
    }
    Try(std::exception_ptr exc) : exc_(std::move(exc)) {
        valid_exc_ = true;
    }
    void Throw() {
        if (valid_exc_) {
            std::rethrow_exception(exc_);
        }
        throw std::invalid_argument("No exception");
    }
    bool IsFailed() const {
        return valid_exc_;
    }

private:
    bool valid_exc_ = false;
    std::exception_ptr exc_;
};

template <class Function, class... Args>
auto TryRun(Function func, Args... args) {
    using ReturnType = decltype(func(args...));
    if constexpr (!std::is_same_v<void, ReturnType>) {
        try {
            return Try<ReturnType>(std::forward<ReturnType>(func(std::forward<Args>(args)...)));
        } catch (std::exception& exc) {
            return Try<ReturnType>(std::current_exception());
        } catch (const char* exc) {
            return Try<ReturnType>(std::invalid_argument(exc));
        } catch (int exc) {
            return Try<ReturnType>(std::invalid_argument(std::strerror(exc)));
        } catch (...) {
            return Try<ReturnType>(std::invalid_argument("Unknown exception"));
        }
    } else {
        try {
            func(args...);
            return Try<void>{};
        } catch (std::exception& exc) {
            return Try<ReturnType>(std::current_exception());
        } catch (const char* exc) {
            return Try<ReturnType>(std::invalid_argument(exc));
        } catch (int exc) {
            return Try<ReturnType>(std::invalid_argument(std::strerror(exc)));
        } catch (...) {
            return Try<ReturnType>(std::invalid_argument("Unknown exception"));
        }
    }
}
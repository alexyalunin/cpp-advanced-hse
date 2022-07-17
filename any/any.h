#pragma once
#include <memory>

class Any {
public:
    class InnerBase {
    public:
        [[nodiscard]] virtual const std::type_info& Type() const = 0;
        virtual ~InnerBase() = default;
    };

    template <class T>
    class Inner : public InnerBase {
    public:
        Inner(T val) : val_(std::forward<T>(val)) {
        }

        const std::type_info& Type() const override {
            return typeid(T);
        }

        T val_;
    };

    Any() {
        ptr_ = nullptr;
    }

    template <class T>
    Any(T value) : ptr_(new Inner<T>(std::forward<T>(value))) {
    }

    template <class T>
    Any& operator=(T value) {
        Any temp(value);
        Swap(temp);
        return *this;
    }

    Any(const Any& rhs) {
        ptr_ = rhs.ptr_;
    }
    Any& operator=(const Any& rhs) {
        if (this == &rhs) {
            return *this;
        }
        ptr_ = rhs.ptr_;
        return *this;
    }
    ~Any() = default;

    bool Empty() const {
        return !ptr_;
    }

    void Clear() {
        ptr_ = nullptr;
    }
    void Swap(Any& rhs) {
        std::swap(ptr_, rhs.ptr_);
    }

    template <class T>
    const T& GetValue() const {
        if (ptr_->Type() == typeid(T)) {
            return static_cast<Inner<T>&>(*ptr_).val_;
        } else {
            throw std::bad_cast();
        }
    }

private:
    std::shared_ptr<InnerBase> ptr_;
};
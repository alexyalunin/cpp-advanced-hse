#pragma once

#include <algorithm>

class ListHook {
public:
    ListHook() {
        prev_ = nullptr;
        next_ = nullptr;
    }

    bool IsLinked() const {
        return prev_ != nullptr || next_ != nullptr;
    }

    void Unlink() {
        if (IsLinked()) {
            prev_->next_ = next_;
            next_->prev_ = prev_;
            prev_ = next_ = nullptr;
        }
    }

    // Must unlink element from list
    virtual ~ListHook() {
        Unlink();
    }

    ListHook(const ListHook &) = delete;

private:
    ListHook *prev_;
    ListHook *next_;

    template <class T>
    friend class List;

    // that helper function might be useful
    void LinkBefore(ListHook *other) {
        Unlink();
        other->prev_->next_ = this;
        prev_ = other->prev_;
        next_ = other;
        other->prev_ = this;
    }
};

template <typename T>
class List {
    friend class Iterator;

public:
    class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
        friend class List;

        const List *const master_;
        ListHook *cur_;

        Iterator(const List *const master, ListHook *cur) : master_(master), cur_(cur) {
        }

    public:
        Iterator &operator++() {
            cur_ = cur_->next_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator it = *this;
            ++*this;
            return it;
        }

        T &operator*() const {
            return *dynamic_cast<T *>(cur_);
        }

        T *operator->() const {
            return dynamic_cast<T *>(cur_);
        }

        bool operator==(const Iterator &rhs) const {
            return master_ == rhs.master_ && cur_ == rhs.cur_;
        }

        bool operator!=(const Iterator &rhs) const {
            return !(*this == rhs);
        }
    };

    List() {
        dummy_.prev_ = &dummy_;
        dummy_.next_ = &dummy_;
    }

    List(const List &) = delete;

    List(List &&other) {
        dummy_ = std::move(other.dummy_);
        other.dummy_.prev_ = &other.dummy_;
        other.dummy_.next_ = &other.dummy_;
    }

    // must unlink all elements from list
    ~List() {
        while (dummy_.next_ != &dummy_) {
            dummy_.next_->Unlink();
        }
    }

    List &operator=(const List &) = delete;

    List &operator=(List &&other) {
        if (this == &other) {
            return *this;
        }
        this->~List();
        dummy_ = std::move(other.dummy_);
        other.dummy_.next_ = &other.dummy_;
        other.dummy_.prev_ = &other.dummy_;
        return *this;
    }

    bool IsEmpty() const {
        return dummy_.prev_ == &dummy_;
    }

    // that method is allowed to be O(n)
    size_t Size() const {
        size_t cur = 0;
        ListHook *hook = dummy_.next_;
        while (hook != &dummy_) {
            ++cur;
            hook = hook->next_;
        }
        return cur;
    }

    // note that IntrusiveList doesn't own elements,
    // and never copies or moves T
    void PushBack(T *elem) {
        static_cast<ListHook *>(elem)->LinkBefore(&dummy_);
    }

    void PushFront(T *elem) {
        static_cast<ListHook *>(elem)->LinkBefore(dummy_.next_);
    }

    T &Front() {
        return *dynamic_cast<T *>(dummy_.next_);
    }

    const T &Front() const {
        return *dynamic_cast<const T *>(dummy_.next_);
    }

    T &Back() {
        return *dynamic_cast<T *>(dummy_.prev_);
    }

    const T &Back() const {
        return *dynamic_cast<const T *>(dummy_.prev_);
    }

    void PopBack() {
        dummy_.prev_->Unlink();
    }

    void PopFront() {
        dummy_.next_->Unlink();
    }

    Iterator Begin() {
        return Iterator(this, dummy_.next_);
    }

    Iterator End() {
        return Iterator(this, &dummy_);
    }

    // complexity of this function must be O(1)
    Iterator IteratorTo(T *element) {
        return Iterator(this, element);
    }

private:
    ListHook dummy_;
};

template <typename T>
typename List<T>::Iterator begin(List<T> &list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T> &list) {  // NOLINT
    return list.End();
}
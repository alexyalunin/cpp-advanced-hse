#pragma once

#include <initializer_list>
#include <algorithm>

class Deque {
public:
    Deque() = default;

    Deque(const Deque &rhs) {
        buff_capacity_ = rhs.buff_capacity_;
        buff_head_ = rhs.buff_head_;
        buff_tail_ = rhs.buff_tail_;
        buff_size_ = rhs.buff_size_;
        cur_size_ = rhs.cur_size_;
        head_ind_ = rhs.head_ind_;
        tail_ind_ = rhs.tail_ind_;
        if (rhs.buffer_ == nullptr) {
            buffer_ = nullptr;
            return;
        }
        buffer_ = new int *[buff_capacity_];
        if (buff_head_ == buff_tail_) {
            buffer_[buff_head_] = new int[128];
            for (size_t j = head_ind_; j <= tail_ind_; ++j) {
                buffer_[buff_head_][j] = rhs.buffer_[buff_head_][j];
            }
            return;
        }
        for (size_t i = buff_head_; i <= buff_tail_; ++i) {
            buffer_[i] = new int[128];
            if (i == buff_head_) {
                for (size_t j = head_ind_; j < 128; ++j) {
                    buffer_[i][j] = rhs.buffer_[i][j];
                }
            } else if (i == buff_tail_) {
                for (size_t j = 0; j <= tail_ind_; ++j) {
                    buffer_[i][j] = rhs.buffer_[i][j];
                }
            } else {
                for (size_t j = 0; j < 128; ++j) {
                    buffer_[i][j] = rhs.buffer_[i][j];
                }
            }
        }
    }

    Deque(Deque &&rhs) noexcept {
        Swap(rhs);
    }

    explicit Deque(size_t size) {
        for (size_t i = 0; i < size; ++i) {
            PushBack(0);
        }
    }

    Deque(std::initializer_list<int> list) {
        for (auto i : list) {
            PushBack(i);
        }
    }

    Deque &operator=(const Deque &rhs) {
        if (this == &rhs) {
            return *this;
        }
        Clear();
        buff_capacity_ = rhs.buff_capacity_;
        buff_head_ = rhs.buff_head_;
        buff_tail_ = rhs.buff_tail_;
        buff_size_ = rhs.buff_size_;
        cur_size_ = rhs.cur_size_;
        head_ind_ = rhs.head_ind_;
        tail_ind_ = rhs.tail_ind_;
        if (rhs.buffer_ == nullptr) {
            buffer_ = nullptr;
            return *this;
        }
        buffer_ = new int *[buff_capacity_];
        if (buff_head_ == buff_tail_) {
            buffer_[buff_head_] = new int[128];
            for (size_t j = head_ind_; j <= tail_ind_; ++j) {
                buffer_[buff_head_][j] = rhs.buffer_[buff_head_][j];
            }
            return *this;
        }
        for (size_t i = buff_head_; i <= buff_tail_; ++i) {
            buffer_[i] = new int[128];
            if (i == buff_head_) {
                for (size_t j = head_ind_; j < 128; ++j) {
                    buffer_[i][j] = rhs.buffer_[i][j];
                }
            } else if (i == buff_tail_) {
                for (size_t j = 0; j <= tail_ind_; ++j) {
                    buffer_[i][j] = rhs.buffer_[i][j];
                }
            } else {
                for (size_t j = 0; j < 128; ++j) {
                    buffer_[i][j] = rhs.buffer_[i][j];
                }
            }
        }
        return *this;
    }

    void Swap(Deque &rhs) {
        std::swap(buffer_, rhs.buffer_);
        std::swap(cur_size_, rhs.cur_size_);
        std::swap(head_ind_, rhs.head_ind_);
        std::swap(tail_ind_, rhs.tail_ind_);
        std::swap(buff_size_, rhs.buff_size_);
        std::swap(buff_head_, rhs.buff_head_);
        std::swap(buff_tail_, rhs.buff_tail_);
        std::swap(buff_capacity_, rhs.buff_capacity_);
    }

    void PushBack(int value) {
        ++cur_size_;
        if (buffer_ == nullptr) {
            buff_capacity_ = 2;
            buff_tail_ = 1;
            tail_ind_ = 0;
            head_ind_ = 0;
            buff_head_ = 1;
            buff_size_ = 1;
            buffer_ = new int *[buff_capacity_];
            buffer_[buff_tail_] = new int[128];
            buffer_[buff_tail_][tail_ind_] = value;
            return;
        }
        if (tail_ind_ < 127) {
            ++tail_ind_;
            buffer_[buff_tail_][tail_ind_] = value;
            return;
        }
        if (buff_tail_ < buff_capacity_ - 1) {
            tail_ind_ = 0;
            ++buff_size_;
            ++buff_tail_;
            buffer_[buff_tail_] = new int[128];
            buffer_[buff_tail_][tail_ind_] = value;
            return;
        }
        size_t new_buff_head = buff_capacity_ / 2;
        buff_capacity_ *= 2;
        int **new_buffer = new int *[buff_capacity_];
        for (size_t i = new_buff_head, j = buff_head_; j <= buff_tail_; ++i, ++j) {
            new_buffer[i] = buffer_[j];
        }
        delete[] buffer_;
        buffer_ = new_buffer;
        buff_head_ = new_buff_head;
        buff_tail_ = buff_head_ + buff_size_;
        tail_ind_ = 0;
        ++buff_size_;
        buffer_[buff_tail_] = new int[128];
        buffer_[buff_tail_][tail_ind_] = value;
    }

    void PopBack() {
        --cur_size_;
        if (cur_size_ == 0) {
            Clear();
            return;
        }
        if (tail_ind_ > 0) {
            --tail_ind_;
            return;
        }
        delete[] buffer_[buff_tail_];
        --buff_size_;
        --buff_tail_;
        tail_ind_ = 127;
    }

    void PushFront(int value) {
        ++cur_size_;
        if (buffer_ == nullptr) {
            buff_capacity_ = 2;
            buff_tail_ = 1;
            tail_ind_ = 0;
            head_ind_ = 0;
            buff_head_ = 1;
            buff_size_ = 1;
            buffer_ = new int *[buff_capacity_];
            buffer_[buff_head_] = new int[128];
            buffer_[buff_head_][head_ind_] = value;
            return;
        }
        if (head_ind_ > 0) {
            --head_ind_;
            buffer_[buff_head_][head_ind_] = value;
            return;
        }
        if (buff_head_ > 0) {
            head_ind_ = 127;
            ++buff_size_;
            --buff_head_;
            buffer_[buff_head_] = new int[128];
            buffer_[buff_head_][head_ind_] = value;
            return;
        }
        size_t new_buff_head = buff_capacity_ / 2;
        buff_capacity_ *= 2;
        int **new_buffer = new int *[buff_capacity_];
        for (size_t i = new_buff_head, j = buff_head_; j <= buff_tail_; ++i, ++j) {
            new_buffer[i] = buffer_[j];
        }
        delete[] buffer_;
        buffer_ = new_buffer;
        buff_head_ = new_buff_head - 1;
        buff_tail_ = buff_head_ + buff_size_;
        head_ind_ = 127;
        ++buff_size_;
        buffer_[buff_head_] = new int[128];
        buffer_[buff_head_][head_ind_] = value;
    }

    void PopFront() {
        --cur_size_;
        if (cur_size_ == 0) {
            Clear();
            return;
        }
        if (head_ind_ < 127) {
            ++head_ind_;
            return;
        }
        delete[] buffer_[buff_head_];
        --buff_size_;
        ++buff_head_;
        head_ind_ = 0;
    }

    int &operator[](size_t ind) {
        if (127 - head_ind_ >= ind) {
            return buffer_[buff_head_][head_ind_ + ind];
        }
        ind -= 127 - head_ind_ + 1;
        size_t buff = ind / 128;
        ind %= 128;
        return buffer_[buff_head_ + buff + 1][ind];
    }

    int operator[](size_t ind) const {
        if (127 - head_ind_ >= ind) {
            return buffer_[buff_head_][head_ind_ + ind];
        }
        ind -= 127 - head_ind_ + 1;
        size_t buff = ind / 128;
        ind %= 128;
        return buffer_[buff_head_ + buff + 1][ind];
    }

    [[nodiscard]] size_t Size() const {
        return cur_size_;
    }

    void Clear() {
        if (buff_size_ != 0) {
            for (size_t i = buff_head_; i <= buff_tail_; ++i) {
                delete[] buffer_[i];
            }
        }
        if (buffer_ != nullptr) {
            delete[] buffer_;
            buffer_ = nullptr;
        }
        cur_size_ = buff_size_ = buff_capacity_ = head_ind_ = tail_ind_ = buff_head_ = buff_tail_ =
            0;
    }

    ~Deque() {
        Clear();
    }

private:
    int **buffer_ = nullptr;
    size_t cur_size_ = 0, buff_size_ = 0, head_ind_ = 0, tail_ind_ = 0, buff_head_ = 0,
           buff_tail_ = 0, buff_capacity_ = 0;
};
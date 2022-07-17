#pragma once

#include <vector>
#include <string>

struct State {
    int ref_count;  // сколько векторов делят этот State между собой.
    std::vector<std::string> vec;
};

class COWVector {
public:
    COWVector() {
        state_ = new State;
        state_->ref_count = 1;
    }

    ~COWVector() {
        state_->ref_count -= 1;
        if (state_->ref_count == 0) {
            delete state_;
        }
    }

    COWVector(const COWVector &other) {
        state_ = other.state_;
        state_->ref_count += 1;
    }

    COWVector &operator=(const COWVector &other) {
        if (state_ == other.state_) {
            return *this;
        }
        state_->ref_count -= 1;
        if (state_->ref_count == 0) {
            delete state_;
        }
        state_ = other.state_;
        state_->ref_count += 1;
        return *this;
    }

    size_t Size() const {
        return state_->vec.size();
    }

    void Resize(size_t size) {
        if (size == state_->vec.size()) {
            return;
        }
        if (state_->ref_count == 1) {
            state_->vec.resize(size);
            return;
        }
        state_->ref_count -= 1;
        auto ptr = new State;
        ptr->ref_count = 1;
        ptr->vec = state_->vec;
        state_ = ptr;
        state_->vec.resize(size);
    }

    const std::string &Get(size_t at) {
        return state_->vec[at];
    }

    const std::string &Back() {
        return state_->vec.back();
    }

    void PushBack(const std::string &value) {
        if (state_->ref_count == 1) {
            state_->vec.push_back(value);
            return;
        }
        state_->ref_count -= 1;
        auto ptr = new State;
        ptr->ref_count = 1;
        ptr->vec = state_->vec;
        state_ = ptr;
        state_->vec.push_back(value);
    }

    void Set(size_t at, const std::string &value) {
        if (state_->ref_count == 1) {
            state_->vec[at] = value;
            return;
        }
        state_->ref_count -= 1;
        auto ptr = new State;
        ptr->ref_count = 1;
        ptr->vec = state_->vec;
        state_ = ptr;
        state_->vec[at] = value;
    }

private:
    State *state_;
};

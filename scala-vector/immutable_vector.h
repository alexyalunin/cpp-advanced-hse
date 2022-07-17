#pragma once
#include <initializer_list>
#include <cstddef>
#include <utility>
#include <vector>
#include <memory>
#include <deque>

const std::vector<size_t> kAmount = {0, 32, 1'024, 32'768, 1'048'576, 33'554'432, 1'073'741'824};

template <typename T>
struct Node {
    std::vector<std::shared_ptr<Node>> children_;
    T value_;
    Node() {
        children_.reserve(32);
    }
    Node(size_t depth, size_t max_depth, const T& value = T()) {
        if (depth != max_depth) {
            children_.resize(32, std::make_shared<Node<T>>(depth + 1, max_depth, value));
        } else {
            value_ = value;
        }
    }
    Node(const T& value) : value_(value) {
    }
};

template <typename T>
class ImmutableVector {
public:
    ImmutableVector() = default;

    explicit ImmutableVector(size_t count, const T& value = T()) : csz_(count) {
        while (count > kAmount[depth_]) {
            ++depth_;
        }
        size_t cur_depth = 0;
        root_ = std::make_shared<Node<T>>(cur_depth, depth_, value);
    }

    template <typename Iterator>
    ImmutableVector(Iterator first, Iterator last) {
        for (Iterator it = first; it != last; ++it) {
            *this = PushBack(*it);
        }
    }

    ImmutableVector(const std::initializer_list<T>& l) {
        for (const auto& it : l) {
            *this = PushBack(it);
        }
    }

    ImmutableVector(size_t csz, size_t depth, std::shared_ptr<Node<T>> root)
        : csz_(csz), depth_(depth), root_(std::move(root)) {
    }

    ImmutableVector Set(size_t index, const T& value) {
        std::deque<size_t> indexes;
        std::vector<std::shared_ptr<Node<T>>> ps;
        auto ptr = root_;
        ps.push_back(root_);
        size_t ind;
        while (indexes.size() != depth_ - 1) {
            ind = index / kAmount[depth_ - indexes.size() - 1];
            index %= kAmount[depth_ - indexes.size() - 1];
            indexes.push_back(ind);
            ptr = ptr->children_[ind];
            ps.push_back(ptr);
        }
        indexes.push_front(0);
        ptr = std::make_shared<Node<T>>(value);
        auto new_ptr = std::make_shared<Node<T>>();
        while (!ps.empty()) {
            new_ptr->children_ = ps.back()->children_;
            new_ptr->children_[index] = ptr;
            index = indexes.back();
            ps.pop_back();
            indexes.pop_back();
            ptr = new_ptr;
            new_ptr = std::make_shared<Node<T>>();
        }
        return ImmutableVector<T>(csz_, depth_, ptr);
    }
    const T& Get(size_t index) const {
        auto ptr = root_;
        size_t cur_d = 0, ind;
        while (cur_d != depth_ - 1) {
            ind = index / kAmount[depth_ - cur_d - 1];
            index %= kAmount[depth_ - cur_d - 1];
            ++cur_d;
            ptr = ptr->children_[ind];
        }
        return ptr->children_[index]->value_;
    }

    ImmutableVector PushBack(const T& value) {
        if (csz_ < kAmount[depth_]) {
            ImmutableVector<T> ans = Set(csz_, value);
            ++ans.csz_;
            return ans;
        }
        ImmutableVector<T> ans(csz_ + 1);
        ans.root_->children_[0] = root_;
        return ans.Set(csz_, value);
    }

    ImmutableVector PopBack() {
        if (csz_ - 1 > kAmount[depth_ - 1]) {
            ImmutableVector<T> ans = *this;
            --ans.csz_;
            return ans;
        }
        ImmutableVector<T> ans(csz_ - 1);
        ans.root_ = root_->children_[0];
        return ans;
    }

    size_t Size() const {
        return csz_;
    }

private:
    friend class Node<T>;
    size_t csz_ = 0;
    size_t depth_ = 1;
    std::shared_ptr<Node<T>> root_ = std::make_shared<Node<T>>(0, 1);
};
#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>

template <class K, class V, int MaxSize = 8>
class ConstexprMap {
public:
    constexpr ConstexprMap() = default;

    constexpr V& operator[](const K& key) {
        if (!Find(key) && size_ == MaxSize) {
            throw std::exception();
        }
        if (Find(key)) {
            return map_[FindInd(key)].second;
        } else {
            map_[size_].first = key;
            ++size_;
            return map_[size_ - 1].second;
        }
    }

    constexpr const V& operator[](const K& key) const {
        if (!Find(key)) {
            throw std::exception();
        }
        return map_[FindInd(key)].second;
    }

    constexpr bool Erase(const K& key) {
        if (!Find(key)) {
            return false;
        }
        --size_;
        for (size_t i = FindInd(key); i < size_; ++i) {
            map_[i].first = map_[i + 1].first;
            map_[i].second = map_[i + 1].second;
        }
        return true;
    }

    constexpr bool Find(const K& key) const {
        for (size_t i = 0; i < size_; ++i) {
            if (map_[i].first == key) {
                return true;
            }
        }
        return false;
    }

    constexpr size_t FindInd(const K& key) const {
        for (size_t i = 0; i < size_; ++i) {
            if (map_[i].first == key) {
                return i;
            }
        }
        return MaxSize;
    }

    constexpr size_t Size() const {
        return size_;
    }

    constexpr std::pair<K, V>& GetByIndex(size_t pos) {
        return map_[pos];
    }

    constexpr const std::pair<K, V>& GetByIndex(size_t pos) const {
        return map_[pos];
    }

private:
    std::size_t size_ = 0;
    std::array<std::pair<K, V>, MaxSize> map_;
};
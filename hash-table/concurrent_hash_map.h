#pragma once

#include <exception>
#include <vector>
#include <mutex>
#include <list>
#include <functional>

template <class K, class V, class Hash = std::hash<K>>
class ConcurrentHashMap {
public:
    ConcurrentHashMap(const Hash& hasher = Hash()) : ConcurrentHashMap(kUndefinedSize, hasher) {
    }

    explicit ConcurrentHashMap(int expected_size, const Hash& hasher = Hash())
        : ConcurrentHashMap(expected_size, kDefaultConcurrencyLevel, hasher) {
    }

    ConcurrentHashMap(int expected_size, int expected_threads_count, const Hash& hasher = Hash())
        : hash_(hasher),
          mutexes_(
              std::vector<std::mutex>(std::max(expected_threads_count, kDefaultConcurrencyLevel))) {
        if (expected_size > static_cast<int>(mutexes_.size())) {
            buckets_.resize(mutexes_.size() * (expected_size / mutexes_.size()));
        } else {
            buckets_.resize(mutexes_.size());
        }
    }

    bool Insert(const K& key, const V& value) {
        {
            std::lock_guard<std::mutex> lock(mutexes_[GetLockInd(key)]);
            size_t bind = GetBucketInd(key);
            auto it = buckets_[bind].begin();
            while (it != buckets_[bind].end()) {
                if (it->first == key) {
                    break;
                }
                ++it;
            }
            if (it != buckets_[bind].end()) {
                return false;
            } else if (buckets_[bind].size() < bsz_) {
                buckets_[bind].emplace_back(key, value);
                return true;
            }
        }
        for (size_t i = 0; i < mutexes_.size(); ++i) {
            mutexes_[i].lock();
        }
        std::vector<std::list<std::pair<K, V>>> buckets(buckets_.size() * 10);
        for (size_t i = 0; i < mutexes_.size(); ++i) {
            for (size_t j = i; j < buckets_.size(); j += mutexes_.size()) {
                for (auto it = buckets_[j].begin(); it != buckets_[j].end(); ++it) {
                    size_t ind = hash_(it->first) % buckets.size();
                    buckets[ind].emplace_back(it->first, it->second);
                }
            }
        }
        size_t bind = hash_(key) % buckets.size();
        if (buckets[bind].size() == bsz_) {
            ++bsz_;
        }
        buckets[bind].emplace_back(key, value);
        buckets_ = buckets;
        for (int i = mutexes_.size() - 1; i > -1; --i) {
            mutexes_[i].unlock();
        }
        return true;
    }

    bool Erase(const K& key) {
        std::lock_guard<std::mutex> lock(mutexes_[GetLockInd(key)]);
        size_t bind = GetBucketInd(key);
        auto it = buckets_[bind].begin();
        while (it != buckets_[bind].end()) {
            if (it->first == key) {
                break;
            }
            ++it;
        }
        if (it != buckets_[bind].end()) {
            buckets_[bind].erase(it);
            return true;
        }
        return false;
    }

    void Clear() {
        for (size_t i = 0; i < mutexes_.size(); ++i) {
            mutexes_[i].lock();
        }
        for (size_t i = 0; i < mutexes_.size(); ++i) {
            for (size_t j = i; j < buckets_.size(); j += mutexes_.size()) {
                buckets_[j].clear();
            }
        }
        for (int i = mutexes_.size() - 1; i > -1; --i) {
            mutexes_[i].unlock();
        }
    }

    std::pair<bool, V> Find(const K& key) const {
        std::lock_guard<std::mutex> lock(mutexes_[GetLockInd(key)]);
        size_t bind = GetBucketInd(key);
        for (auto& [k, v] : buckets_[bind]) {
            if (k == key) {
                auto ans = std::make_pair(true, v);
                return ans;
            }
        }
        return std::make_pair(false, V());
    }

    const V At(const K& key) const {
        std::lock_guard<std::mutex> lock(mutexes_[GetLockInd(key)]);
        size_t bind = GetBucketInd(key);
        for (auto& [k, v] : buckets_[bind]) {
            if (k == key) {
                return v;
            }
        }
        throw std::out_of_range("");
    }

    size_t Size() const {
        for (size_t i = 0; i < mutexes_.size(); ++i) {
            mutexes_[i].lock();
        }
        size_t size = 0;
        for (size_t i = 0; i < mutexes_.size(); ++i) {
            for (size_t j = i; j < buckets_.size(); j += mutexes_.size()) {
                size += buckets_[j].size();
            }
        }
        for (int i = mutexes_.size() - 1; i > -1; --i) {
            mutexes_[i].unlock();
        }
        return size;
    }

    static const int kDefaultConcurrencyLevel;
    static const int kUndefinedSize;

    size_t GetBucketInd(const K& key) const {
        return hash_(key) % buckets_.size();
    };
    size_t GetLockInd(const K& key) const {
        return hash_(key) % mutexes_.size();
    };

private:
    size_t bsz_ = 10;
    Hash hash_;
    mutable std::vector<std::mutex> mutexes_;
    std::vector<std::list<std::pair<K, V>>> buckets_;
};

template <class K, class V, class Hash>
const int ConcurrentHashMap<K, V, Hash>::kDefaultConcurrencyLevel = 10;

template <class K, class V, class Hash>
const int ConcurrentHashMap<K, V, Hash>::kUndefinedSize = 10;
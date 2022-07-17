#include "lru_cache.h"

LruCache::LruCache(size_t max_size) {
    limit_ = max_size;
}

void LruCache::Set(const std::string &key, const std::string &value) {
    if (lru_unmap_.find(key) != lru_unmap_.end()) {
        lru_list_.erase(lru_unmap_[key]);
        lru_list_.emplace_back(key, value);
        lru_unmap_[key] = --lru_list_.end();
    } else if (lru_unmap_.size() != limit_) {
        lru_list_.emplace_back(key, value);
        lru_unmap_[key] = --lru_list_.end();
    } else {
        lru_unmap_.erase(lru_list_.begin()->first);
        lru_list_.pop_front();
        lru_list_.emplace_back(key, value);
        lru_unmap_[key] = --lru_list_.end();
    }
}

bool LruCache::Get(const std::string &key, std::string *value) {
    if (lru_unmap_.find(key) != lru_unmap_.end()) {
        *value = lru_unmap_[key]->second;
        lru_list_.erase(lru_unmap_[key]);
        lru_list_.emplace_back(key, *value);
        lru_unmap_[key] = --lru_list_.end();
        return true;
    }
    return false;
}

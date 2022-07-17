#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

using std::string;

std::vector<std::unique_ptr<string>> Duplicate(const std::vector<std::shared_ptr<string>>& items) {
    std::vector<std::unique_ptr<string>> out(items.size());
    for (size_t i = 0; i < items.size(); ++i) {
        out[i] = std::make_unique<string>(*items[i]);
    }
    return out;
}

std::vector<std::shared_ptr<string>> DeDuplicate(
    const std::vector<std::unique_ptr<string>>& items) {
    std::vector<std::shared_ptr<string>> out(items.size());
    std::unordered_map<string, size_t> place;
    for (size_t i = 0; i < items.size(); ++i) {
        if (place.find(*items[i]) != place.end()) {
            out[i] = out[place[*items[i]]];
        } else {
            place[*items[i]] = i;
            out[i] = std::make_shared<string>(*items[i]);
        }
    }
    return out;
}
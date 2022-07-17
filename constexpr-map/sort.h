#pragma once

#include <constexpr_map.h>

#include <type_traits>

template <class K, class V, int S>
constexpr auto Sort(const ConstexprMap<K, V, S>& map) {
    ConstexprMap<K, V, S> ans;
    for (size_t i = 0; i < map.Size(); ++i) {
        ans[map.GetByIndex(i).first] = map.GetByIndex(i).second;
    }
    if constexpr (std::is_integral_v<K>) {
        for (size_t i = 0; i < map.Size(); ++i) {
            size_t k = i;
            for (size_t j = i + 1; j < map.Size(); ++j) {
                if (ans.GetByIndex(j).first > ans.GetByIndex(k).first) {
                    k = j;
                }
            }
            if (k != i) {
                const K temp_key = ans.GetByIndex(i).first;
                const V temp_value = ans.GetByIndex(i).second;
                ans.GetByIndex(i).first = ans.GetByIndex(k).first;
                ans.GetByIndex(i).second = ans.GetByIndex(k).second;
                ans.GetByIndex(k).first = temp_key;
                ans.GetByIndex(k).second = temp_value;
            }
        }
    } else {
        for (size_t i = 0; i < map.Size(); ++i) {
            size_t k = i;
            for (size_t j = i + 1; j < map.Size(); ++j) {
                if (ans.GetByIndex(j).first < ans.GetByIndex(k).first) {
                    k = j;
                }
            }
            if (k != i) {
                const K temp_key = ans.GetByIndex(i).first;
                const V temp_value = ans.GetByIndex(i).second;
                ans.GetByIndex(i).first = ans.GetByIndex(k).first;
                ans.GetByIndex(i).second = ans.GetByIndex(k).second;
                ans.GetByIndex(k).first = temp_key;
                ans.GetByIndex(k).second = temp_value;
            }
        }
    }
    return ans;
}
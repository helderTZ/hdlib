#pragma once

#include <cstddef>
#include <list>
#include <unordered_map>
#include <optional>
#include <utility>

namespace hd {

template <typename TKey, typename TValue>
class LRUCache {
private:
    size_t size;
    const size_t capacity;
    std::list<std::pair<TKey, TValue>> list;
    std::unordered_map<TKey, typename std::list<std::pair<TKey, TValue>>::iterator> map;

public:
    LRUCache(const size_t size) : size(0U), capacity(size), list{}, map{} {}

    void set(TKey key, TValue value) {
        auto it = map.find(key);
        if (it != map.end()) {
            list.erase((*it).second);
            map[key] = list.insert(list.end(), std::make_pair(key, value));
            size++;
        } else {
            if (size == capacity) {
                std::pair<TKey, TValue> head = *list.begin();
                list.erase(list.begin());
                map.erase(head.first);
            }
            map[key] = list.insert(list.end(), std::make_pair(key, value));
            size++;
        }
    }

    std::optional<TValue> get(TKey key) {
        auto it = map.find(key);
        if (it != map.end()) {
            TValue value = (*it).second->second;
            list.erase((*it).second);
            map[key] = list.insert(list.end(), std::make_pair(key, value));
            return value;
        }

        return std::nullopt;
    }
};

}   // namespace hd

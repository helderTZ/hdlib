#pragma once

#include <cstddef>
#include <vector>
#include <optional>
#include <cmath>

namespace hd {

template <typename T, typename Predicate>
class heap {
private:
    size_t capacity;
    size_t size;
    Predicate pred;
    std::vector<T> nodes;

    std::pair<size_t, size_t> get_children(size_t index) const {
        return std::make_pair(index * 2 + 1, index * 2 + 2);
    }

    size_t get_parent(size_t index) const {
        return std::floor((index - 1) / 2);
    }

    void heapify(size_t index) {
        auto [left, right] = get_children(index);
        size_t m = index;
        if (left < size && pred(nodes[left], nodes[m])) {
            m = left;
        }
        if (right < size && pred(nodes[right], nodes[m])) {
            m = right;
        }
        if (m != index) {
            std::swap(nodes[index], nodes[m]);
            heapify(m);
        }
    }

public:
    heap() : capacity(64U), size(0U), pred(Predicate{}), nodes(64U, 0) {}
    heap(size_t capacity, Predicate pred)
        : capacity(capacity), size(0U), pred(pred), nodes(capacity, 0) {}
    
    std::optional<T> top() {
        return size > 0 ? std::optional{nodes[0]} : std::nullopt;
    }

    void push(T value) {
        if (size == nodes.size()) {
            nodes.push_back(value);
            size++;
        } else {
            nodes[size++] = value;
        }

        if (size > 1) {
            size_t i = size-1;
            while (i != 0 && !pred(nodes[get_parent(i)], nodes[i])) {
                std::swap(nodes[i], nodes[get_parent(i)]);
                i = get_parent(i);
            }
        }
    }

    void pop() {
        if (size > 0) {
            std::swap(nodes[0], nodes[size-1]);
            size--;
            heapify(0);
        }
    }

    void pop_n(size_t n) {
        for(size_t i = 0; i < n; i++) {
            pop();
        }
    }
    
    std::vector<T> top_n(size_t n) {
        std::vector<T> result;
        result.reserve(n);
        for(size_t i = 0; i < n; i++) {
            result.push_back(top());
            pop();
        }
        return result;
    }
};

template <typename T>
using minheap = class heap<T, std::less<T>>;

template <typename T>
using maxheap = class heap<T, std::greater<T>>;

template<typename Container, typename Predicate>
heap<typename Container::value_type, Predicate> make_heap(Container container, Predicate pred) {
    heap h(container.size(), pred);
    for (auto it = container.cbegin(); it != container.cend(); ++it) {
        h.push(*it);
    }

    return h;
}

template<typename Container>
minheap<typename Container::value_type> make_minheap(Container container) {
    return make_heap(container, std::less<typename Container::value_type>());
}

template<typename Container>
maxheap<typename Container::value_type> make_maxheap(Container container) {
    return make_heap(container, std::greater<typename Container::value_type>());
}


}   // namespace hd

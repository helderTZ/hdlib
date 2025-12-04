#pragma once

#include <vector>
#include <cstdint>
#include <numeric>
#include <bitset>

namespace hd {

/**
 * @brief A container that implements a sparse array.
 *
 * @details A sparse array only occupies memory for 'valid' values.
 * Valid values are non-default constructed values, eg: 0 for integers.
 * This means that a sparse array of 1 million integers where only one is
 * different than zero, will only occupy the memory of a single integer
 * (+ metadata which amounts to aprox 12.5%, or 1/8th of sizeof(int)*1e6).
 * Implementation follows closely from https://goog-sparsehash.sourceforge.net/doc/implementation.html
 */
template <typename T>
class sparse_array  {
    static constexpr size_t M = 48U;

    struct Group {
        std::vector<T> v;
        uint64_t bitmap;

        inline void set_bit(const size_t index) {
            bitmap |= 1 << index;
        }

        inline void clear_bit(const size_t index) {
            bitmap &= ~(1 << index);
        }

        inline bool test_bit(const size_t index) {
            return bitmap & (1 << index);
        }

        typename std::vector<T>::iterator find(const size_t index) {
            if (!lookup(index)) return v.end();

            size_t ones = 0;
            for (size_t i = 0; i <= index; i++) {
                if (test_bit(i)) { ones++; }
            }

            return v.begin() + ones - 1;
        }

        inline bool lookup(const size_t index) {
            return test_bit(index);
        }

        void insert(const size_t index, const T value) {
            auto it = find(index);
            if (lookup(index)) {
                *it = value;
            } else {
                v.insert(it, value);
            }
            set_bit(index);
        }

        void erase(const size_t index) {
            if (!lookup(index)) {
                return;
            } else {
                v.erase(find(index));
                clear_bit(index);
            }
        }

        size_t size() const {
            return sizeof(v) + sizeof(bitmap) + v.size() * sizeof(T);
        }

        size_t count() const {
            return std::bitset<M>(bitmap).count();
        }
    };

    std::vector<Group> groups;

public:
    sparse_array(const size_t size) : groups(size / M + 1U) {}

    T find(const size_t  index) {
        auto it = groups[index / M].find(index % M);
        if (it == groups[index / M].v.end()) return T{};
        return *it;
    }

    void insert(const size_t index, const T value) {
        return groups[index / M].insert(index % M, value);
    }

    void erase(const size_t index) {
        return groups[index / M].erase(index % M);
    }

    size_t size() const {
        return sizeof(groups) +
            std::accumulate(groups.cbegin(), groups.cend(), 0,
                [](size_t acc, const auto& g) { return acc + g.size(); });
    }

    size_t count() const {
        return std::accumulate(groups.cbegin(), groups.cend(), 0,
            [](size_t acc, const auto& g) { return acc + g.count(); });
    }

    class dense_iterator {
        size_t index;
        sparse_array<T>& sa;
    public:
        dense_iterator(const size_t index, sparse_array<T>& sa)
            : index(index)
            , sa(sa)
        {}

        dense_iterator& operator++() {
            index++;
            return *this;
        }

        dense_iterator operator++(int) {
            dense_iterator old = *this;
            operator++();
            return old;
        }

        T operator*() { return sa.find(index); }

        bool operator==(const dense_iterator& other) {
            return index == other.index;
        }
        bool operator!=(const dense_iterator& other) {
            return index != other.index;
        }
    };

    dense_iterator begin() { return dense_iterator(0U, *this); }
    dense_iterator end() { return dense_iterator(count(), *this); }
};

}   // namespace hd

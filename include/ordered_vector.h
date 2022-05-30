#pragma once

#include <algorithm>

namespace hd {

template <typename T>
class ordered_vector {
public:

    class iterator;
    class const_iterator;
    class reverse_iterator;

    class iterator {
    public:

        friend class const_iterator;
        friend class reverse_iterator;

        iterator(T* ptr) : ptr(ptr) {}

        iterator operator++() {
            ptr++;
            return iterator(ptr);
        }

        iterator operator++(int i) {
            ptr+=i;
            return iterator(ptr);
        }


        iterator operator--() {
            ptr--;
            return iterator(ptr);
        }

        iterator operator--(int i) {
            ptr-=i;
            return iterator(ptr);
        }
        iterator operator+(int i) {
            return iterator(ptr+i);
        }

        iterator operator-(int i) {
            return iterator(ptr-i);
        }

        T& operator*() {
            return *ptr;
        }

        bool operator==(const iterator& other) {
            return ptr == other.ptr;
        }

        bool operator==(const const_iterator& other) {
            return ptr == other.ptr;
        }

        bool operator==(const reverse_iterator& other) {
            return ptr == other.ptr;
        }

        bool operator!=(const iterator& other) {
            return ptr != other.ptr;
        }

        bool operator!=(const const_iterator& other) {
            return ptr != other.ptr;
        }

        bool operator!=(const reverse_iterator& other) {
            return ptr != other.ptr;
        }

    private:
        T* ptr;
    };

    class const_iterator {
    public:

        friend class iterator;
        friend class reverse_iterator;

        const_iterator(T* ptr) : ptr(ptr) {}

        const_iterator operator++() {
            ptr++;
            return const_iterator(ptr);
        }

        const_iterator operator++(int i) {
            ptr+=i;
            return const_iterator(ptr);
        }

        const_iterator operator--() {
            ptr--;
            return const_iterator(ptr);
        }

        const_iterator operator--(int i) {
            ptr-=i;
            return const_iterator(ptr);
        }

        const_iterator operator+(int i) {
            return const_iterator(ptr+i);
        }

        const_iterator operator-(int i) {
            return const_iterator(ptr-i);
        }

        T& operator*() {
            return *ptr;
        }

        bool operator==(const iterator& other) {
            return ptr == other.ptr;
        }

        bool operator==(const const_iterator& other) {
            return ptr == other.ptr;
        }

        bool operator==(const reverse_iterator& other) {
            return ptr == other.ptr;
        }

        bool operator!=(const iterator& other) {
            return ptr != other.ptr;
        }

        bool operator!=(const const_iterator& other) {
            return ptr != other.ptr;
        }

        bool operator!=(const reverse_iterator& other) {
            return ptr != other.ptr;
        }

    private:
        T* ptr;
    };

    class reverse_iterator {
    public:

        friend class _iterator;
        friend class const_iterator;

        reverse_iterator(T* ptr) : ptr(ptr) {}

        reverse_iterator operator++() {
            ptr--;
            return reverse_iterator(ptr);
        }

        reverse_iterator operator++(int i) {
            ptr-=i;
            return reverse_iterator(ptr);
        }

        reverse_iterator operator--() {
            ptr++;
            return reverse_iterator(ptr);
        }

        reverse_iterator operator--(int i) {
            ptr+=i;
            return reverse_iterator(ptr);
        }

        reverse_iterator operator+(int i) {
            return reverse_iterator(ptr-i);
        }

        reverse_iterator operator-(int i) {
            return reverse_iterator(ptr+i);
        }

        T& operator*() {
            return *ptr;
        }

        bool operator==(const iterator& other) {
            return ptr == other.ptr;
        }

        bool operator==(const const_iterator& other) {
            return ptr == other.ptr;
        }

        bool operator==(const reverse_iterator& other) {
            return ptr == other.ptr;
        }

        bool operator!=(const iterator& other) {
            return ptr != other.ptr;
        }

        bool operator!=(const const_iterator& other) {
            return ptr != other.ptr;
        }

        bool operator!=(const reverse_iterator& other) {
            return ptr != other.ptr;
        }

    private:
        T* ptr;
    };

    ordered_vector() {
        _data = (T*) malloc(sizeof(T) * _capacity);
    }

    ~ordered_vector() {
        free(_data);
    }

    iterator begin() const {
        return iterator(&_data[0]);
    }

    iterator end() const {
        return iterator(&_data[_size]);
    }

    const_iterator cbegin() const {
        return const_iterator(&_data[0]);
    }

    const_iterator cend() const {
        return const_iterator(&_data[_size]);
    }

    reverse_iterator rbegin() const {
        return reverse_iterator(&_data[_size-1]);
    }

    reverse_iterator rend() const {
        return reverse_iterator(&_data[-1]);
    }

    void push_back(T elem) {
        if (_size == 0) {
            _data[_size++] = elem;
            return;
        }

        if (_size == _capacity) {
            _capacity *= 2;
            _data = (T*) realloc(_data, sizeof(T) * _capacity);
        }

        // find where to place new element
        auto [it, idx] = find_idx_if([&](T& t) { return elem < t; });
        if (it == end()) {
            _data[_size++] = elem;
            return;
        }

        // move greater elements to the right
        for(auto reverse_it = rbegin(); reverse_it != (it-1); ++reverse_it) {
            *(reverse_it-1) = *reverse_it;
        }

        // we now have space for the new element
        _data[idx] = elem;
        _size += 1;
    }

    T& operator[](int idx) {
        return _data[idx];
    }

    T* data() const {
        return _data;
    }

    size_t size() {
        return _size;
    }

    size_t capacity() {
        return _capacity;
    }

    T& min() {
        return _data[0];
    }

    T& max() {
        return _data[_size-1];
    }

private:
    template <typename Pred>
    auto find_idx_if(Pred f) {
        auto it = begin();
        for (size_t i = 0; i < _size; ++i) {
            if (f(_data[i])) {
                return std::pair<iterator, T>(it, i);
                ++it;
            }
            ++it;
        }
        return std::pair<iterator, T>(end(), _data[_size]);
    }

private:
    T* _data = nullptr;
    size_t _capacity = 64;
    size_t _size = 0;
};

}   // namespace hd
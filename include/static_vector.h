#pragma once

#include <cstdlib>
#include <algorithm>

namespace hd {

template <typename T, size_t CAP>
class static_vector {
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

    void push_back(const T& elem) {
        if (_size < CAP) {
            _data[_size++] = elem;
        }
    }

    T& operator[](size_t i) {
        return _data[i];
    }

    T* data() {
        return _data;
    }

    size_t size() {
        return _size;
    }

    size_t capacity() {
        return CAP;
    }

private:
    T _data[CAP];
    size_t _size = 0;
};

}   // namespace hd
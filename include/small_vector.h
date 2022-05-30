#pragma once

#include <cstdlib>
#include <algorithm>

namespace hd {

template <typename T, size_t CAP = 64>
class small_vector {
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

    small_vector() {}

    ~small_vector() {
        if (_extended_data != nullptr) {
            free(_extended_data);
        }
    }

    iterator begin() const {
        if (_extended_data == nullptr) {
            return iterator(&_data[0]);
        }
        return iterator(&_extended_data[0]);
    }

    iterator end() const {
        if (_extended_data == nullptr) {
            return iterator(&_data[_size]);
        }
        return iterator(&_extended_data[_size]);
    }

    const_iterator cbegin() const {
        if (_extended_data == nullptr) {
            return const_iterator(&_data[0]);
        }
        return const_iterator(&_extended_data[0]);
    }

    const_iterator cend() const {
        if (_extended_data == nullptr) {
            return const_iterator(&_data[_size]);
        }
        return const_iterator(&_extended_data[_size]);
    }

    reverse_iterator rbegin() const {
        if (_extended_data == nullptr) {
            return reverse_iterator(&_data[_size-1]);
        }
        return reverse_iterator(&_extended_data[_size-1]);
    }

    reverse_iterator rend() const {
        if (_extended_data == nullptr) {
            return reverse_iterator(&_data[-1]);
        }
        return reverse_iterator(&_extended_data[-1]);
    }

    void push_back(T& elem) {
        // still using small buffer
        if (_extended_data == nullptr) {
            if (_size < CAP) {
                _data[_size++] = elem;
            } else {
                _extended_data = (T*) malloc(sizeof(T) * CAP*2);
                std::copy(_data, &_data[CAP], _extended_data);
                _extended_data[_size++] = elem;
                _capacity = CAP*2;
            }
        // not using small buffer
        } else {
            if (_size < _capacity) {
                _extended_data[_size++] = elem;
            } else {
                _capacity *= 2;
                _extended_data = (T*) realloc(_extended_data, sizeof(T) * _capacity);
                _extended_data[_size] = elem;
                _size += 1;
            }
        }
    }

    T& operator[](size_t i) {
        if (_extended_data == nullptr) {
            return _data[i];
        }
        return _extended_data[i];

    }

    T* data() {
        if (_extended_data == nullptr) {
            return _data;
        }
        return _extended_data;
    }

    size_t size() {
        return _size;
    }

    size_t capacity() {
        return _capacity;
    }

private:
    T _data[CAP];
    T* _extended_data = nullptr;
    size_t _capacity = CAP;
    size_t _size = 0;
};

}   // namespace hd
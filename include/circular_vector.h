#pragma once

#include <algorithm>

namespace hd {

template <typename T>
class circular_vector {
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

    circular_vector(size_t size) {
        _capacity = size;
        _begin = (T*) malloc(sizeof(T) * _capacity);
        _last = _begin + _capacity;
        _curr = _begin;
    }

    ~circular_vector() {
        free(_data);
    }

    iterator begin() const {
        return iterator(_begin);
    }

    iterator end() const {
        return iterator(_last+1);
    }

    const_iterator cbegin() const {
        return const_iterator(_begin);
    }

    const_iterator cend() const {
        return const_iterator(_last+1);
    }

    reverse_iterator rbegin() const {
        return reverse_iterator(_last);
    }

    reverse_iterator rend() const {
        return reverse_iterator(_begin-1);
    }

    void push_back(T elem) {
        *_curr = elem;
        _curr++;
        _items++;
        if (_curr == _last) {
            _curr = _begin;
        }
    }

    void pop() {
        if (_items > 0) {
            ~(*_curr);
            if (_curr != _begin) {
                _curr--;
            }
            _items--;
        }
    }

    void resize(size_t size) {
        if (size > _capacity) {
            new_begin = (T*) malloc(sizeof(T) * _capacity);
            for (T* curr = _begin; curr < _last; curr++) {
                new_begin[]
            }
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

private:
    T* _begin = nullptr;
    T* _last = nullptr;
    T* _curr = nullptr;
    size_t _items = 0;
    size_t _capacity = 0;
};

}   // namespace hd
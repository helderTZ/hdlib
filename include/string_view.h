#pragma once

#include <cstdlib>
#include <iterator>

namespace hd {

class string_view {
    const char* _cstr;
    size_t _size;

public:
    string_view(const char* str) : _cstr(str), _size(0U) {
        while (*str) {
            _size++;
            str++;
        }
    }

    string_view(char* str) : _cstr(str), _size(0U) {
        while (*str) {
            _size++;
            str++;
        }
    }

    string_view(const char* str, size_t size) : _cstr(str), _size(size) {}

    string_view (char* str, size_t size) : _cstr(str), _size(size) {}

    template<size_t Size>
    string_view(char (&str)[Size]) : _cstr(str), _size(Size) {}

    template<size_t Size>
    string_view(const char (&str)[Size]) : _cstr(str), _size(Size) {}

    string_view(char* begin, char* end) : _cstr(begin), _size(0U) {
        while (begin != end) {
            _size++;
            begin++;
        }
    }

    const char* data() const { return _cstr; }

    size_t size() const { return _size; }

    string_view substring(size_t begin, size_t end) const {
        return string_view(&_cstr[begin], static_cast<size_t>(std::distance(&_cstr[begin], &_cstr[end])));
    }
};

}   // namespace hd

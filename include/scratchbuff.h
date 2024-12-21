#pragma once

#include <cstdlib>
#include <cstring>

namespace hd {

struct Marker {
    void* _at;
    Marker(void* ptr) : _at{ptr} {}
};

constexpr size_t SCRATCHBUFF_LEN_DEFAULT = 8*1024;

template<size_t Len = SCRATCHBUFF_LEN_DEFAULT>
class scratchbuff {
    void* _begin;
    void* _end;
    void* _curr;

public:
    scratchbuff()
        : _begin{malloc(Len)}
        , _end{(char*)_begin + Len}
        , _curr{_begin}
    {}

    ~scratchbuff() {
        free(_begin);
    }

    void* alloc(size_t size) {
        if ((char*)_curr + size > _end) {
            return nullptr;
        }
        void* ptr = _curr;
        _curr = (char*)_curr + size;
        return ptr;
    }

    Marker set_marker() {
        return Marker(_curr);
    }

    void reset() {
        _curr = _begin;
    }

    void reset(Marker marker) {
        if (marker._at >= _begin && marker._at < _end) {
            _curr = marker._at;
        }
    }
};

}   // namespace hd

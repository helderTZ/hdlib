#pragma once

#include <type_traits>

namespace hd {

template <typename IT, typename T, typename Pred>
T reduce(IT _first, IT _last, T _acc, Pred f) {
    for(auto it = _first; it != _last; ++it) {
        _acc = f(*it, _acc);
    }
    return _acc;
}

template <typename T>
const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

template <typename T>
const T& min(const T& a, const T& b) {
    return (a <= b) ? a : b;
}

template <typename T>
T gcd(const T& a, const T& b) {
    if (!a) {
        return b;
    } else if (!b) {
        return a;
    } else {
        auto max = hd::max(*begin, *end);
        auto min = hd::min(*begin, *end);
        gcd(max - min);
    }
}

template <typename It,
          std::enable_if<std::is_integral<It::value_type>::value> = true>
It::value_type gcd(It begin, It end) {
    return hd::reduce(begin, end, 0, hd::gcd);
}

}   // namespace hd
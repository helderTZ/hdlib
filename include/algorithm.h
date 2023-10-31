#pragma once

#include <type_traits>
#include <stdexcept>
#include <complex>
#define _USE_MATH_DEFINES
#include <cmath>

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
        // auto max = hd::max(*begin, *end);
        // auto min = hd::min(*begin, *end);
        auto max = hd::max(a, b);
        auto min = hd::min(a, b);
        gcd(max - min);
    }
}

template <typename It,
          std::enable_if_t<std::is_integral<typename It::value_type>::value, bool> = true>
typename It::value_type gcd(It begin, It end) {
    return hd::reduce(begin, end, 0, hd::gcd);
}

template <typename It
        , std::enable_if_t<
            std::is_integral<typename It::value_type>::value ||
            std::is_enum<typename It::value_type>::value
        , bool> = true
        >
bool has_duplicates(It begin, It end) {
    long int unique = 0;
    for (It it = begin; it != end; ++it) {
        if (unique & (1U << static_cast<int>(*it)) != 0) {
            return false;
        } else {
            unique |= 1U << static_cast<int>(*it);
        }
    }
    return true;
}

// Adapted from: Rosetta code FFT python recursive impl
// https://rosettacode.org/wiki/Fast_Fourier_transform#Python
template<typename T,
    std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
void fft(std::complex<T>* out, std::complex<T>* in, size_t n, size_t stride=1) {
    using namespace std::complex_literals;

    if (n <= 1) {
        out[0] = in[0];
        return;
    }

    fft(out,       in,          n/2, stride*2);
    fft(out + n/2, in + stride, n/2, stride*2);

    for (size_t k = 0; k < n/2; ++k) {
        T t = static_cast<T>(k/n);
        std::complex<T> v = std::exp(-2.0 * 1.0i * M_PI * t) * out[k + n/2];
        std::complex<T> e = out[k];
        out[k      ] = e + v;
        out[k + n/2] = e - v;
    }
}

template<typename T>
typename T::value_type& last(T& container) {
    if (container.size() < 1) {
        throw std::out_of_range("container is empty");
    }
    return *std::prev(container.end());
}

// TODO
// auto enumerate(const auto& data) {
//     return [i=0](const auto& value) mutable {
//         return std::make_pair(i++, value);
//     };
// }

// TODO
// https://stackoverflow.com/a/46282024/13499951
// a tiny simple range library

}   // namespace hd
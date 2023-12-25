#pragma once

#include <netinet/in.h>

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <limits>

#include <vector>
#include <array>
#include <algorithm>
#include <type_traits>

#include <biggerints.h>

namespace hd {

namespace impl {

    // get padded length of input
    // in bits: L + 1 + K + 64
    // L is size of input in bits
    // padded length must be a multiple of 512
    // padding starts with a bit '1' followed by K zeros
    // must have space at the end for a 64 bit integer that specifies the size
    uint64_t get_padded_length(size_t len) {
        uint64_t length_paded = len + 1 + 8;

        while ( 0 != (length_paded*8 % 512) ) {
            length_paded += 1;
        }

        return length_paded;
    }

    std::vector<uint8_t> get_padded_input(void* data, size_t len) {
        uint64_t padded_length = get_padded_length(len);
        uint8_t* data_ = static_cast<uint8_t*>(data);
        std::vector<uint8_t> padded_input(data_, data_ + len);
        padded_input.reserve(padded_length);

        // add bit '1' followed by K '0' bits
        padded_input.push_back(0x80);
        for (size_t i = len+1; i < padded_length-4; ++i) {
            padded_input.push_back(0);
        }

        // add the length as a 64 bit number
        padded_input.push_back(static_cast<uint8_t>((padded_length & 0x00000000000000FF) >>  0));
        padded_input.push_back(static_cast<uint8_t>((padded_length & 0x000000000000FF00) >>  8));
        padded_input.push_back(static_cast<uint8_t>((padded_length & 0x0000000000FF0000) >> 16));
        padded_input.push_back(static_cast<uint8_t>((padded_length & 0x00000000FF000000) >> 24));
        padded_input.push_back(static_cast<uint8_t>((padded_length & 0x000000FF00000000) >> 32));
        padded_input.push_back(static_cast<uint8_t>((padded_length & 0x0000FF0000000000) >> 40));
        padded_input.push_back(static_cast<uint8_t>((padded_length & 0x00FF000000000000) >> 48));
        padded_input.push_back(static_cast<uint8_t>((padded_length & 0xFF00000000000000) >> 56));

        return padded_input;
    }

    // bitwise rotate right
    // adapted from: https://stackoverflow.com/a/28303634/13499951
    uint32_t ror(uint32_t val, uint32_t shift) {
        shift %= 32;
        uint32_t t1 = val >> shift ;
        uint32_t t2 = val << ((32-shift) % 32);
        return t1 | t2;
    }

    uint32_t gamma0(uint32_t a) {
        return ror(a, 7) ^ ror(a, 18) ^ ror(a, 3);
    }

    uint32_t gamma1(uint32_t a) {
        return ror(a, 17) ^ ror(a, 19) ^ ror(a, 10);
    }

    uint32_t sigma0(uint32_t a) {
        return ror(a, 6) ^ ror(a, 11) ^ ror(a, 25);
    }

    uint32_t sigma1(uint32_t a) {
        return ror(a, 2) ^ ror(a, 13) ^ ror(a, 22);
    }

    uint32_t ch(uint32_t a, uint32_t b, uint32_t c) {
        return (a & b) ^ (~a & c);
    }

    uint32_t maj(uint32_t a, uint32_t b, uint32_t c) {
        return (a & b) ^ (a & c) ^ (b & c);
    }


    // Taken from: https://stackoverflow.com/a/56705703/13499951
    // trying my make a custom binary operator that does does a half adder
    #define define const struct
    #define operator(ReturnType, OperatorName, FirstOperandType, SecondOperandType)                                             \
        OperatorName ## _ {} OperatorName;                                                                                      \
        template <typename T>                                                                                                   \
        struct OperatorName ## Proxy {                                                                                          \
            public:                                                                                                             \
            OperatorName ## Proxy(const T& t) : t_(t) {}                                                                        \
            const T& t_;                                                                                                        \
            static ReturnType _ ## OperatorName ## _(const FirstOperandType a, const SecondOperandType b);                      \
        };                                                                                                                      \
        template <typename T> OperatorName ## Proxy<T> operator<(const T& lhs, const OperatorName ## _& rhs [[maybe_unused]]) { \
            return OperatorName ## Proxy<T>(lhs);                                                                               \
        }                                                                                                                       \
        ReturnType operator>(const OperatorName ## Proxy<FirstOperandType>& lhs, const SecondOperandType& rhs) {                \
            return OperatorName ## Proxy<FirstOperandType>::_ ## OperatorName ## _(lhs.t_, rhs);                                \
        }                                                                                                                       \
        template <typename T> inline ReturnType OperatorName ## Proxy<T>::_ ## OperatorName ## _(const FirstOperandType a, const SecondOperandType b)

    define operator(uint32_t, binadd, uint32_t, uint32_t) { // Arguments are the return type, the name of the operator, the left operand type and the right operand type, respectively
        uint64_t result = static_cast<uint64_t>( static_cast<uint64_t>(static_cast<uint64_t>(a) + static_cast<uint64_t>(b)) % static_cast<uint64_t>(1UL<<32UL) );
        return static_cast<uint32_t>(result & 0xFFFFFFFFU);
    }

    // #define binadd <binadd> // Finally, you have to define a macro to avoid to put the < and > operator at the start and end of the operator name

}   // namespace impl

// FIXME: does not give the same result as python or sha256sum
//        there must be a bug somewhere
//        (is it in the addition? is it a normal addition?)
//        the size of the digest is fixed, should be 32 byte long
// adapted from: https://en.wikipedia.org/wiki/SHA-2#Pseudocode
// also check out https://foss.heptapod.net/pypy/pypy/-/blob/branch/default/lib_pypy/_sha256.py
// also check out https://medium.com/@domspaulo/python-implementation-of-sha-256-from-scratch-924f660c5d57
// also check out https://github.com/B-Con/crypto-algorithms/blob/master/sha256_test.c
std::vector<uint8_t> sha256(void* const data, const size_t len) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"

    using namespace impl;

    uint32_t H[] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    constexpr uint32_t K[] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    std::vector<uint8_t> padded_input = get_padded_input(data, len);

    // for each 512-bit chunk
    for (size_t i = 0; i < padded_input.size(); i += 64) {
        std::array<uint32_t, 64> w;

        // copy first 16 32-bit words into w
        memcpy(w.begin(), &padded_input[i], 16*sizeof(uint32_t));

        // extend the first 16 32-bit words into the rest of the array
        for (size_t j = 16; j < 64; ++j) {
            uint32_t s0 = gamma0(w[j-15]);
            uint32_t s1 = gamma1(w[j-2]);
            // w[j] = w[j-16] + s0 + w[j-7] + s1;
            w[j] = w[j-16] <binadd> s0 <binadd> w[j-7] <binadd> s1;
        }

        uint32_t a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7];

        for (size_t j = 0; j < 64; ++j) {
            uint32_t S1 = sigma1(e);
            uint32_t Ch = ch(e, f, g);
            // uint32_t t1 = h + S1 + Ch + K[j] + w[j];
            uint32_t t1 = h <binadd> S1 <binadd> Ch <binadd> K[j] <binadd> w[j];
            uint32_t S0 = sigma0(a);
            uint32_t Maj = maj(a, b, c);
            // uint32_t t2 = S0 + Maj;
            uint32_t t2 = S0 <binadd> Maj;

            h = g;
            g = f;
            f = e;
            // e = d + t1;
            e = d <binadd> t1;
            d = c;
            c = b;
            b = a;
            // a = t1 + t2;
            a = t1 <binadd> t2;
        }

        // H[0] += a;
        // H[1] += b;
        // H[2] += c;
        // H[3] += d;
        // H[4] += e;
        // H[5] += f;
        // H[6] += g;
        // H[7] += h;

        H[0] = H[0] <binadd> a;
        H[1] = H[1] <binadd> b;
        H[2] = H[2] <binadd> c;
        H[3] = H[3] <binadd> d;
        H[4] = H[4] <binadd> e;
        H[5] = H[5] <binadd> f;
        H[6] = H[6] <binadd> g;
        H[7] = H[7] <binadd> h;
    }

    std::vector<uint8_t> digest;
    for (size_t i = 0; i < 8; ++i) {
        uint32_t val = ntohl(H[i]);
        digest.push_back(static_cast<uint8_t>((val & 0x000000FF) >>  0));
        digest.push_back(static_cast<uint8_t>((val & 0x0000FF00) >>  8));
        digest.push_back(static_cast<uint8_t>((val & 0x00FF0000) >> 16));
        digest.push_back(static_cast<uint8_t>((val & 0xFF000000) >> 24));
    }

    return digest;

#pragma GCC diagnostic pop
}

template <typename T> T fnv_1a(void* const data, const size_t len);

template<>
uint32_t fnv_1a<uint32_t>(void* const data, const size_t len) {
    constexpr uint32_t FNV_OFFSET_BASIS = 0x811c9dc5U;
    constexpr uint32_t FNV_PRIME = 0x01000193U;
    
    uint32_t digest = FNV_OFFSET_BASIS;
    for (size_t i = 0; i < len; ++i) {
        digest ^= reinterpret_cast<uint8_t*>(data)[i];
        digest *= FNV_PRIME;
    }

    return digest;
}

template<>
uint64_t fnv_1a<uint64_t>(void* const data, const size_t len) {
    constexpr uint64_t FNV_OFFSET_BASIS = 0xcbf29ce484222325UL;
    constexpr uint64_t FNV_PRIME = 0x100000001b3UL;
    
    uint64_t digest = FNV_OFFSET_BASIS;
    for (size_t i = 0; i < len; ++i) {
        digest ^= reinterpret_cast<uint8_t*>(data)[i];
        digest *= FNV_PRIME;
    }

    return digest;
}

template<>
uint128_t fnv_1a<uint128_t>(void* const data, const size_t len) {
    constexpr uint128_t FNV_OFFSET_BASIS = { 7113472399480571277ULL, 7809847782465536322ULL }; // 144066263297769815596495629667062367629
    constexpr uint128_t FNV_PRIME = { 315ULL, 16777216ULL}; // 309485009821345068724781371

    uint128_t digest = FNV_OFFSET_BASIS;
    for (size_t i = 0; i < len; ++i) {
        uint8_t byte = reinterpret_cast<uint8_t*>(data)[i];
        digest ^= static_cast<uint128_t>(uint128_t{0, byte});
        digest *= FNV_PRIME;
    }

    return digest;
}

template<>
uint256_t fnv_1a<uint256_t>(void* const data, const size_t len) {
    constexpr uint256_t FNV_OFFSET_BASIS = { {1162971903282775349ULL, 14461431585808235443ULL}, {3285590903173248716ULL, 15935580172955504694ULL} }; // 100029257958052580907070968620625704837092796014241193945225284501741471925557
    constexpr uint256_t FNV_PRIME = { {0ULL, 355ULL}, {1099511627776ULL, 0ULL} }; // 374144419156711147060143317175368453031918731002211

    uint256_t digest = FNV_OFFSET_BASIS;
    for (size_t i = 0; i < len; ++i) {
        uint8_t byte = reinterpret_cast<uint8_t*>(data)[i];
        digest ^= static_cast<uint256_t>(uint256_t{uint128_t{byte, 0}, uint128_t{0, 0}});
        digest *= FNV_PRIME;
    }

    return digest;
}

}   // namespace hd
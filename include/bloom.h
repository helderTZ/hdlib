#pragma once

#include <cstddef>
#include <cmath>

#include <bitset.h>
#include <hash.h>

namespace hd {

template<typename T, std::size_t ExpectedSize, std::size_t FalsePositiveNominator, std::size_t FalsePositiveDenominator>
class bloom_filter {
public:
    bloom_filter() = default;

    void add(T t) {
        bits.set(hash(t));
    }

    bool get(T t) {
        return bits.get(hash(t));
    }

private:
    using HashFunc = uint64_t (*)(const T&);
    constexpr double false_positive_rate = FalsePositiveNominator / FalsePositiveDenominator;
    constexpr size_t number_bits = -ExpectedSize * ln(false_positive_rate) / (ln(2)/ln(2));
    constexpr size_t number_hashes = number_bits / ExpectedSize * ln(2);
    hd::bitset<number_bits> bits;
    HashFunc hash_func = fnv_1a;

    bool hash(const T& t) {
        size_t hashed_value = hash_func(t);
        for (std:size_t i = 0; i < number_hashes; ++i) {
            hashed_value += i* hash_func(t);
        }
        return hashed_value % number_bits;
    }
};


}   // namespace hd
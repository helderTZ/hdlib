#pragma once

#include <cstddef>
#include <cmath>
#include <string>

#include <bitset.h>
#include <hash.h>
#include <biggerints.h>

namespace hd {

template<typename T>
class bloom_filter {
public:
    bloom_filter() = default;

    // in order to use N hashes, divide the digest in N chunks
    void add(T t) {
        uint256_t hashed_value = hash_func(reinterpret_cast<void*>(&t), sizeof(t));
        for (size_t i = 0; i < NUM_HASHES; ++i) {
            uint32_t partial_hash = static_cast<uint32_t>(*(reinterpret_cast<uint32_t*>(&hashed_value) + i)) % NUM_BITS;
            bits.set(partial_hash);
        }
    }

    bool get(T t) {
        uint256_t hashed_value = hash_func(reinterpret_cast<void*>(&t), sizeof(t));
        return bits.get(*reinterpret_cast<uint32_t*>(&hashed_value) % NUM_BITS);
    }

    std::string internal_state() {
        return bits.to_string();
    }

private:
    using HashFunc = uint256_t (*)(void*, const size_t);
    constexpr static size_t NUM_BITS = 8U;
    constexpr static size_t NUM_HASHES = 4U;
    constexpr static size_t DIGEST_SIZE = 64U;
    hd::bitset<NUM_BITS> bits;
    HashFunc hash_func = fnv_1a<uint256_t>;
};

// TODO: generic bloom filter (it has an arbitrary number of hashes in order to fulfill the required false_positive_rate)
// template<typename T, std::size_t ExpectedSize, std::size_t FalsePositiveNominator, std::size_t FalsePositiveDenominator>
// class bloom_filter {
// public:
//     bloom_filter() {
//         static_assert(number_hashes / 256 > 0 && number_hashes % 256 == 0);
//     }

//     void add(T t) {
//         for (size_t i = 0; i < number_hashes; ++i) {
//             std::vector<uint8_t> hashed_value = hash(t);
//             for (size_t i = 0; i < number_hashes; ++i) {
//                 partial_hash = reinterpret_cast<uint8_t*>(hashed_value.data()) && 
//                 bits.set(hash(t && ));
//         }
//     }

//     bool get(T t) {
//         return bits.get(hash(t));
//     }

// private:
//     using HashFunc = uint64_t (*)(const T&);
//     constexpr double false_positive_rate = FalsePositiveNominator / FalsePositiveDenominator;
//     constexpr size_t number_bits = -ExpectedSize * ln(false_positive_rate) / (ln(2)/ln(2));
//     constexpr size_t number_hashes = number_bits / ExpectedSize * ln(2);
//     hd::bitset<number_bits> bits;
//     HashFunc hash_func = fnv_1a;

//     bool hash(const T& t) {
//     }
// };


}   // namespace hd
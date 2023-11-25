#pragma once

#include <type_traits>
#include <limits>

namespace hd {

// Need a constexpr ceil (only available in C++23)
// Adapted from https://stackoverflow.com/a/66146159/13499951
template<typename T,
         std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
constexpr size_t ceil(T value) {
    const int64_t integer = static_cast<int64_t>(value);
    return value > integer ? integer+1 : integer;
}

template<size_t N>
class bitset {
private:
    std::array<uint8_t, ceil(static_cast<float>(N) / (sizeof(uint8_t)*8))> bytes;

    enum BIT_POSITION : uint8_t {
        ZERO  = 1U << 0U,
        ONE   = 1U << 1U,
        TWO   = 1U << 2U,
        THREE = 1U << 3U,
        FOUR  = 1U << 4U,
        FIVE  = 1U << 5U,
        SIX   = 1U << 6U,
        SEVEN = 1U << 7U,
    };

public:
    bitset() {
        std::fill(bytes.begin(), bytes.end(), 0);
    }

    bitset(uint8_t bits) {
        bytes[0] = bits;
    }

    bitset(uint16_t bits) {
        static_assert(N >= 16);
        bytes[0] = static_cast<uint8_t>((bits & 0x00FF) >> 0);
        bytes[1] = static_cast<uint8_t>((bits & 0xFF00) >> 8);
    }

    bitset(uint32_t bits) {
        static_assert(N >= 32);
        bytes[0] = static_cast<uint8_t>((bits & 0x000000FF) >> 0);
        bytes[1] = static_cast<uint8_t>((bits & 0x0000FF00) >> 8);
        bytes[2] = static_cast<uint8_t>((bits & 0x00FF0000) >> 16);
        bytes[3] = static_cast<uint8_t>((bits & 0xFF000000) >> 24);
    }

    bitset(uint64_t bits) {
        static_assert(N >= 64);
        bytes[0] = static_cast<uint8_t>((bits & 0x00000000000000FF) >> 0);
        bytes[1] = static_cast<uint8_t>((bits & 0x000000000000FF00) >> 8);
        bytes[2] = static_cast<uint8_t>((bits & 0x0000000000FF0000) >> 16);
        bytes[3] = static_cast<uint8_t>((bits & 0x00000000FF000000) >> 24);
        bytes[4] = static_cast<uint8_t>((bits & 0x000000FF00000000) >> 32);
        bytes[5] = static_cast<uint8_t>((bits & 0x0000FF0000000000) >> 40);
        bytes[6] = static_cast<uint8_t>((bits & 0x00FF000000000000) >> 48);
        bytes[7] = static_cast<uint8_t>((bits & 0xFF00000000000000) >> 56);
    }

    //FIXME
    bitset(const char* str) {
        const char* c = str;
        size_t idx = 0;
        size_t bit_idx = 0;
        size_t byte_idx = 0;
        while (*c != '\0' && idx < N) {
            uint8_t byte = 0;
            for (int b = 0; b < 8 && *c != '\0' && idx < N; ++b) {
                assert(*c == '1' || *c == '0');
                bit_idx = idx % 8; 
                byte_idx = idx / 8;
                if (*c == '1') {
                    switch (bit_idx) {
                        case 0: byte = ZERO; break;
                        case 1: byte = ONE; break;
                        case 2: byte = TWO; break;
                        case 3: byte = THREE; break;
                        case 4: byte = FOUR; break;
                        case 5: byte = FIVE; break;
                        case 6: byte = SIX; break;
                        case 7: byte = SEVEN; break;
                    }
                }
                c++;
                idx++;
            }
            bytes[byte_idx] = byte;
        }
    }

    void set() {
        std::fill(bytes.begin(), bytes.end(), std::numeric_limits<uint8_t>::max());
    }

    void set(size_t idx) {
        size_t byte_idx = idx / 8;
        size_t bit_idx = idx % 8;

        switch (bit_idx) {
            case 0: bytes[byte_idx] |= ZERO;  break;
            case 1: bytes[byte_idx] |= ONE;   break;
            case 2: bytes[byte_idx] |= TWO;   break;
            case 3: bytes[byte_idx] |= THREE; break;
            case 4: bytes[byte_idx] |= FOUR;  break;
            case 5: bytes[byte_idx] |= FIVE;  break;
            case 6: bytes[byte_idx] |= SIX;   break;
            case 7: bytes[byte_idx] |= SEVEN; break;
            default: assert(false); // unreachable
        }
    }

    void clr() {
        std::fill(bytes.begin(), bytes.end(), 0);
    }

    void clr(size_t idx) {
        size_t byte_idx = idx / 8;
        size_t bit_idx = idx % 8;

        switch (bit_idx) {
            case 0: bytes[byte_idx] &= ~ZERO;  break;
            case 1: bytes[byte_idx] &= ~ONE;   break;
            case 2: bytes[byte_idx] &= ~TWO;   break;
            case 3: bytes[byte_idx] &= ~THREE; break;
            case 4: bytes[byte_idx] &= ~FOUR;  break;
            case 5: bytes[byte_idx] &= ~FIVE;  break;
            case 6: bytes[byte_idx] &= ~SIX;   break;
            case 7: bytes[byte_idx] &= ~SEVEN; break;
            default: assert(false); // unreachable
        }
    }

    bool get(size_t idx) const {
        size_t byte_idx = idx / 8;
        size_t bit_idx = idx - byte_idx;

        switch (bit_idx) {
            case 0: return bytes[byte_idx] & ZERO;
            case 1: return bytes[byte_idx] & ONE;
            case 2: return bytes[byte_idx] & TWO;
            case 3: return bytes[byte_idx] & THREE;
            case 4: return bytes[byte_idx] & FOUR;
            case 5: return bytes[byte_idx] & FIVE;
            case 6: return bytes[byte_idx] & SIX;
            case 7: return bytes[byte_idx] & SEVEN;
            default: assert(false); // unreachable
        }
    }

    void flip() {
        std::transform(bytes.begin(), bytes.end(), bytes.begin(),
            [](auto byte){ return ~byte; });
    }

    void flip(size_t idx) {
        size_t byte_idx = idx / 8;
        size_t bit_idx = idx - byte_idx;

        switch (bit_idx) {
            case 0: bytes[byte_idx] ^= ZERO;  break;
            case 1: bytes[byte_idx] ^= ONE;   break;
            case 2: bytes[byte_idx] ^= TWO;   break;
            case 3: bytes[byte_idx] ^= THREE; break;
            case 4: bytes[byte_idx] ^= FOUR;  break;
            case 5: bytes[byte_idx] ^= FIVE;  break;
            case 6: bytes[byte_idx] ^= SIX;   break;
            case 7: bytes[byte_idx] ^= SEVEN; break;
            default: assert(false); // unreachable
        }
    }

    std::string to_string() const {
        std::string repr;
        size_t bit_idx = 0;
        for (size_t i = 0; i < bytes.size(); ++i) {
            uint8_t byte = bytes[i];
            for (size_t j = 0; bit_idx < N && j < 8; ++j) {
                repr += std::to_string(byte % 2);
                byte /= 2;
                bit_idx++;
            }
        }

        // reverse string so bits are in MSB -> LSB order
        for (size_t i = 0; i < repr.size()/2; ++i) {
            std::swap(repr[i], repr[repr.size()-1-i]);
        }

        return "0b" + repr;
    }

    size_t size_bits() {
        return N;
    }

    size_t size_in_memory_bits() {
        return bytes.size()*8;
    }

    size_t size_bytes_underlying() {
        return bytes.size();
    }

};

}   // namespace hd
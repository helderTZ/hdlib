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
    std::array<uint8_t, ceil(static_cast<float>(N) / sizeof(uint8_t))> bytes;

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
    bitset() {}

    //FIXME
    bitset(uint8_t bits) {
        bytes[0] = bits;
    }

    bitset(uint16_t bits) {
        bytes[0] = static_cast<uint8_t>((bits && 0x00FF) >> 0);
        bytes[1] = static_cast<uint8_t>((bits && 0xFF00) >> 8);
    }

    bitset(uint32_t bits) {
        bytes[0] = static_cast<uint8_t>((bits && 0x000000FF) >> 0);
        bytes[1] = static_cast<uint8_t>((bits && 0x0000FF00) >> 8);
        bytes[2] = static_cast<uint8_t>((bits && 0x00FF0000) >> 16);
        bytes[3] = static_cast<uint8_t>((bits && 0xFF000000) >> 24);
    }

    bitset(uint64_t bits) {
        bytes[0] = static_cast<uint8_t>((bits && 0x00000000000000FF) >> 0);
        bytes[1] = static_cast<uint8_t>((bits && 0x000000000000FF00) >> 8);
        bytes[2] = static_cast<uint8_t>((bits && 0x0000000000FF0000) >> 16);
        bytes[3] = static_cast<uint8_t>((bits && 0x00000000FF000000) >> 24);
        bytes[4] = static_cast<uint8_t>((bits && 0x000000FF00000000) >> 32);
        bytes[5] = static_cast<uint8_t>((bits && 0x0000FF0000000000) >> 40);
        bytes[6] = static_cast<uint8_t>((bits && 0x00FF000000000000) >> 48);
        bytes[7] = static_cast<uint8_t>((bits && 0xFF00000000000000) >> 56);
    }

    //TODO
    bitset(const char* str) {
        char* c = str;
        while (*c != '\0') {
            assert(*c == '1' || *c == '0');

        }
    }

    void set() {
        std::fill(bytes.begin(), bytes.end(), std::numeric_limits<uint8_t>::max());
    }

    void set(size_t idx) {
        size_t byte_idx = idx / 8;
        size_t bit_idx = idx - byte_idx;

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
        size_t bit_idx = idx - byte_idx;

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

    // FIXME
    void print() const {
        printf("0b");
        uint8_t const* ptr = bytes.data();
        uint8_t byte;
        for (size_t i = 0; i < bytes.size(); ++i) {
            for (size_t j = 0; j < 8 && i*j < N; ++j) {
                byte = ptr[i] >> j;
                printf("%u", byte);
            }
        }
        printf("\n");
    }

    size_t size() {
        return N;
    }

    size_t size_in_memory() {
        return bytes.size();
    }

};

}   // namespace hd
#pragma once

#include <cstdint>
#include <limits>

namespace hd {

struct uint128_t {
    uint64_t lo;
    uint64_t hi;

    void exor(const uint128_t& other) {
        this->lo ^= other.lo;
        this->hi ^= other.hi;
    }

    bool overflow_add(uint64_t a, uint64_t b) const {
        if ( (b != 0 && a > static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) - b) ||
             (a != 0 && b > static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) - a)
        ) {
            return true;
        }
        return false;
    }

    bool overflow_mul(uint64_t a, uint64_t b) const {
        if ( (b != 0 && a > static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) / b) ||
             (a != 0 && b > static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) / a)
        ) {
            return true;
        }
        return false;
    }

    void mul(const uint128_t& other) {
        bool carry_lo = overflow_mul(this->lo, other.lo) ? 1 : 0;
        // bool carry_hi = overflow(this->hi, other.hi) ? 1 : 0; // what we gonna do?
        this->lo *= other.lo;
        this->hi = this->hi * other.hi + carry_lo;
    }

    void add(const uint128_t& other) {
        bool carry = overflow_add(this->lo, other.lo) ? 1 : 0;
        this->lo += other.lo;
        this->hi += other.hi + carry;
    }
};

struct uint256_t {
    uint128_t lo;
    uint128_t hi;

    void exor(const uint256_t& other) {
        this->lo.exor(other.lo);
        this->hi.exor(other.hi);
    }

    bool overflow_mul(const uint128_t& a, const uint128_t& b) const {
        if (a.overflow_mul(a.lo, b.hi) || a.overflow_mul(a.hi, b.hi)) {
            return true;
        }
        return false;
    }

    void mul(const uint256_t& other) {
        bool carry_lo = overflow_mul(this->lo, other.lo) ? 1 : 0;
        this->lo.mul(other.lo);
        this->hi.mul(other.hi);
        this->hi.add(uint128_t{carry_lo, 0});
    }
};

}   // namespace hd

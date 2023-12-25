#pragma once

#include <cstdint>
#include <limits>

namespace hd {

struct uint128_t {
    uint64_t hi;
    uint64_t lo;

    uint128_t operator+(const uint128_t& other) {
        uint128_t result;
        bool carry = overflow_add(this->lo, other.lo) ? 1 : 0;
        result.lo = this->lo + other.lo;
        result.hi = this->hi + other.hi + carry;
        return result;
    }

    uint128_t operator+=(const uint128_t& other) {
        bool carry = overflow_add(this->lo, other.lo) ? 1 : 0;
        this->lo += other.lo;
        this->hi += other.hi + carry;
        return *this;
    }

    uint128_t operator*(const uint128_t& other) {
        uint128_t result;
        bool carry_lo = overflow_mul(this->lo, other.lo) ? 1 : 0;
        result.lo = this->lo * other.lo;
        result.hi = this->hi * other.hi + carry_lo;
        return result;
    }

    uint128_t operator*=(const uint128_t& other) {
        bool carry_lo = overflow_mul(this->lo, other.lo) ? 1 : 0;
        this->lo *= other.lo;
        this->hi *= other.hi + carry_lo;
        return *this;
    }

    uint128_t operator^(const uint128_t& other) {
        uint128_t result;
        result.lo = this->lo ^ other.lo;
        result.hi = this->hi ^ other.hi;
        return result;
    }

    uint128_t operator^=(const uint128_t& other) {
        this->lo ^= other.lo;
        this->hi ^= other.hi;
        return *this;
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
};

struct uint256_t {
    uint128_t hi;
    uint128_t lo;

    uint256_t operator+(const uint256_t& other) {
        uint256_t result;
        bool carry = overflow_add(this->lo, other.lo) ? 1 : 0;
        result.lo = this->lo + other.lo;
        result.hi = this->hi + other.hi + uint128_t{carry, 0};
        return result;
    }

    uint256_t operator+=(const uint256_t& other) {
        bool carry = overflow_add(this->lo, other.lo) ? 1 : 0;
        this->lo += other.lo;
        this->hi += other.hi;
        this->hi += uint128_t{0, carry};
        return *this;
    }

    uint256_t operator*(const uint256_t& other) {
        uint256_t result;
        bool carry = overflow_mul(this->lo, other.lo) ? 1 : 0;
        result.lo = this->lo * other.lo;
        result.hi = (this->hi * other.hi) + uint128_t{carry, 0};
        return result;
    }

    uint256_t operator*=(const uint256_t& other) {
        bool carry = overflow_mul(this->lo, other.lo) ? 1 : 0;
        this->lo *= other.lo;
        this->hi *= other.hi;
        this->hi += uint128_t{0, carry};
        return *this;
    }

    uint256_t operator^(const uint256_t& other) {
        uint256_t result;
        result.lo = this->lo ^ other.lo;
        result.hi = this->hi ^ other.hi;
        return result;
    }

    uint256_t operator^=(const uint256_t& other) {
        this->lo ^= other.lo;
        this->hi ^= other.hi;
        return *this;
    }

    bool overflow_mul(const uint128_t& a, const uint128_t& b) const {
        if (a.overflow_mul(a.lo, b.lo) || a.overflow_mul(a.hi, b.hi)) {
            return true;
        }
        return false;
    }

    bool overflow_add(const uint128_t& a, const uint128_t& b) const {
        if (a.overflow_add(a.lo, b.lo) || a.overflow_add(a.hi, b.hi)) {
            return true;
        }
        return false;
    }
};

}   // namespace hd

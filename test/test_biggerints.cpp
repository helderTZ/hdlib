#include "gtest/gtest.h"
#include "biggerints.h"

#include <iostream>
#include <string>
#include <algorithm>

std::string to_string(const hd::uint128_t& t) {
    std::string str;

    for (int64_t i = 63; i >= 0; --i) {
        str += std::to_string( (t.hi & (1 << i)) >> i );
    }

    for (int64_t i = 63; i >= 0; --i) {
        str += std::to_string( (t.lo & (1 << i)) >> i );
    }

    return str;
}

std::string to_string(const hd::uint256_t& t) {
    std::string str;
    str += to_string(t.hi);
    str += to_string(t.lo);
    return str;
}


TEST(biggerints, uint128_t_add) {
    hd::uint128_t num1 = {0xFFFF0000ULL, 0xFFFFULL}; // 18446462598732906495
    hd::uint128_t num2 = {0x0000FFFFULL, 0xFFFFULL}; // 281470681808895

    std::cout << "num1: " << to_string(num1) << "\n";
    std::cout << "num2: " << to_string(num2) << "\n";
    num1 = num1 + num2;
    std::cout << "num1: " << to_string(num1) << "\n";

    EXPECT_EQ(num1.lo, 0x1FFFEULL);
    EXPECT_EQ(num1.hi, 0xFFFFFFFFULL);
}

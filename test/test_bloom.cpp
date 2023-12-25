#include "gtest/gtest.h"
#include "bloom.h"

#include <iostream>

TEST(bloom, it_works) {
    hd::bloom_filter<int> bloom;

    bloom.add(0);
    bloom.add(20);
    bloom.add(400);
    bloom.add(6000);

    std::cout << bloom.internal_state() << "\n";

    EXPECT_TRUE(bloom.get(0));
    EXPECT_TRUE(bloom.get(20));
    EXPECT_TRUE(bloom.get(400));
    EXPECT_TRUE(bloom.get(6000));
    // EXPECT_FALSE(bloom.get(155)); // false positive
    // EXPECT_FALSE(bloom.get(377)); // false positive
    // EXPECT_FALSE(bloom.get(10)); // false positive
}
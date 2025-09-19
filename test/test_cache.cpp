#include "gtest/gtest.h"
#include "cache.h"

TEST(cache, lru) {
    hd::LRUCache<int, std::string> cache{2};

    EXPECT_FALSE(cache.get(0).has_value());

    cache.set(1, "one");
    EXPECT_EQ(cache.get(1).value(), "one");

    cache.set(2, "two");
    EXPECT_EQ(cache.get(1).value(), "one");
    EXPECT_EQ(cache.get(2).value(), "two");

    cache.set(3, "three");
    EXPECT_EQ(cache.get(3).value(), "three");
    EXPECT_EQ(cache.get(2).value(), "two");
    EXPECT_FALSE(cache.get(1).has_value());
}
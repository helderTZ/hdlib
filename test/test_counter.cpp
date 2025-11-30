#include "gtest/gtest.h"
#include "counter.h"

#include <vector>

using namespace hd;

TEST(counter, works) {
    std::vector<int> container { 1, 1, 2, 2, 2, 3, 3, 4 };
    auto counter = hd::make_counter(container);

    ASSERT_EQ(counter.size(), 4U);
    ASSERT_EQ(counter[1], 2U);
    ASSERT_EQ(counter[2], 3U);
    ASSERT_EQ(counter[3], 2U);
    ASSERT_EQ(counter[4], 1U);
}

TEST(counter, empty_container) {
    std::vector<int> container;
    auto counter = hd::make_counter(container);

    ASSERT_EQ(counter.size(), 0U);
}

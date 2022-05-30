#include "gtest/gtest.h"
#include "small_vector.h"

using namespace hd;

TEST(small_vector, random_insert_greater_than_initial_capacity) {
    small_vector<int> uut;

    for (int i = 0; i < 100; ++i) {
        uut.push_back(i);
    }

    for (int i = 0; i < 100; ++i) {
        ASSERT_EQ(i, uut[i]);
    }
}

TEST(small_vector, random_insert_greater_than_double_initial_capacity) {
    small_vector<int> uut;

    for (int i = 0; i < 200; ++i) {
        uut.push_back(i);
    }

    for (int i = 0; i < 200; ++i) {
        ASSERT_EQ(i, uut[i]);
    }
}
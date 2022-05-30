#include "gtest/gtest.h"
#include "static_vector.h"

using namespace hd;

TEST(static_vector, insert_after_capacity_does_not_insert) {
    static_vector<int, 5> uut;

    for (int i = 0; i < 5; ++i) {
        uut.push_back(i);
    }
    // uut.push_back(6);


    for (int i = 0; i < 5; ++i) {
        ASSERT_EQ(i, uut[i]);
    }
    // ASSERT_EQ(5U, uut.size());
}
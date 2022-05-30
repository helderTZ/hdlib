#include "gtest/gtest.h"
#include "ordered_vector.h"

using namespace hd;

TEST(ordered_vector, random_insert_comes_out_ordered) {
    ordered_vector<int> uut;

    uut.push_back(5);
    uut.push_back(8);
    uut.push_back(3);
    uut.push_back(7);

    ASSERT_EQ(3, uut[0]);
    ASSERT_EQ(5, uut[1]);
    ASSERT_EQ(7, uut[2]);
    ASSERT_EQ(8, uut[3]);
}
#include "gtest/gtest.h"
#include "algorithm.h"

#include <vector>

using namespace hd;

enum UUT {
    ONE,
    TWO,
    THREE,
};

TEST(has_duplicates, has_duplicates) {
    std::vector<UUT> uut;

    uut.push_back(ONE);
    uut.push_back(TWO);
    uut.push_back(THREE);
    uut.push_back(ONE);

    ASSERT_TRUE(has_duplicates(uut.begin(), uut.end()));
}

TEST(has_duplicates, does_not_have_duplicates) {
    std::vector<UUT> uut;

    uut.push_back(ONE);
    uut.push_back(TWO);
    uut.push_back(THREE);

    ASSERT_FALSE(has_duplicates(uut.begin(), uut.end()));
}
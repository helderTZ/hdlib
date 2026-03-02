#include "gtest/gtest.h"
#include "bimap.h"

using namespace hd;

TEST(bimap, base) {
    hd::bimap<int, char> uut;

    uut.emplace(10, 'a');
    uut.emplace('b', 20);
    uut.emplace(30, 'c');

    EXPECT_EQ(uut[10], 'a');
    EXPECT_EQ(uut[20], 'b');
    EXPECT_EQ(uut[30], 'c');

    EXPECT_EQ(uut['a'], 10);
    EXPECT_EQ(uut['b'], 20);
    EXPECT_EQ(uut['c'], 30);
}

TEST(bimap, assignment) {
    hd::bimap<int, char> uut;

    uut[10] = 'a';
    uut['b'] = 20;
    uut[30] = 'c';

    EXPECT_EQ(uut[10], 'a');
    EXPECT_EQ(uut[20], 'b');
    EXPECT_EQ(uut[30], 'c');

    EXPECT_EQ(uut['a'], 10);
    EXPECT_EQ(uut['b'], 20);
    EXPECT_EQ(uut['c'], 30);
}
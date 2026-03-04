#include "gtest/gtest.h"
#include <ios>
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

TEST(bimap, same_type) {
    hd::bimap<int, int> uut;

    uut[1].left() = 10;
    uut[2].left() = 20;
    uut[3].left() = 30;

    EXPECT_EQ(uut[10].right(), 1);
    EXPECT_EQ(uut[20].right(), 2);
    EXPECT_EQ(uut[30].right(), 3);

    EXPECT_EQ(uut[1].left(), 10);
    EXPECT_EQ(uut[2].left(), 20);
    EXPECT_EQ(uut[3].left(), 30);
}

TEST(bimap, erase) {
    hd::bimap<int, float> uut;

    uut[1] = 1.0f;
    uut[2] = 2.0f;
    uut[3] = 3.0f;
    uut.erase(2);

    EXPECT_EQ(uut[1], 1.0f);
    EXPECT_EQ(uut[2], 0.0f);    // default constructed on operator[] access
    EXPECT_EQ(uut[3], 3.0f);
}

TEST(bimap, erase_same_types) {
    hd::bimap<int, int> uut;

    uut[1].left() = 10;
    uut[2].left() = 20;
    uut[3].left() = 30;
    uut.erase(20, decltype(uut)::right{});

    EXPECT_EQ(uut[1].left(), 10);
    EXPECT_EQ(uut[2].left(), 0);    // default constructed on operator[] access
    EXPECT_EQ(uut[3].left(), 30);
}
#include "gtest/gtest.h"
#include "bitset.h"

using namespace hd;

TEST(bitset, set_and_get) {
    bitset<2> uut;

    uut.set();
    ASSERT_TRUE(uut.get(0));
    ASSERT_TRUE(uut.get(1));

    uut.clr();
    ASSERT_FALSE(uut.get(0));
    ASSERT_FALSE(uut.get(1));

    uut.set(0);
    uut.clr(1);
    ASSERT_TRUE(uut.get(0));
    ASSERT_FALSE(uut.get(1));

    uut.set(1);
    uut.clr(0);
    ASSERT_FALSE(uut.get(0));
    ASSERT_TRUE(uut.get(1));
}

TEST(bitset, flip) {
    bitset<5> uut;

    uut.set();
    uut.clr(2);
    uut.clr(4);
    EXPECT_TRUE(uut.get(0));
    EXPECT_TRUE(uut.get(1));
    EXPECT_FALSE(uut.get(2));
    EXPECT_TRUE(uut.get(3));
    EXPECT_FALSE(uut.get(4));

    uut.flip();
    EXPECT_FALSE(uut.get(0));
    EXPECT_FALSE(uut.get(1));
    EXPECT_TRUE(uut.get(2));
    EXPECT_FALSE(uut.get(3));
    EXPECT_TRUE(uut.get(4));
}

TEST(bitset, construcctors) {
    bitset<8> uut_8b{(uint8_t)0b00110011};
    EXPECT_EQ(uut_8b.get(0), false);
    EXPECT_EQ(uut_8b.get(1), false);
    EXPECT_EQ(uut_8b.get(2), true);
    EXPECT_EQ(uut_8b.get(3), true);
    EXPECT_EQ(uut_8b.get(4), false);
    EXPECT_EQ(uut_8b.get(5), false);
    EXPECT_EQ(uut_8b.get(6), true);
    EXPECT_EQ(uut_8b.get(7), true);
}
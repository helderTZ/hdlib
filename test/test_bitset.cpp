#include "gtest/gtest.h"
#include "bitset.h"

using namespace hd;

TEST(bitset, set_and_get) {
    bitset<2> uut;

    EXPECT_EQ(uut.size_bits(), 2);
    EXPECT_EQ(uut.size_in_memory_bits(), 8);
    EXPECT_EQ(uut.size_bytes_underlying(), 1);

    uut.set();
    ASSERT_EQ(uut.to_string(), "0b11");
    ASSERT_TRUE(uut.get(0));
    ASSERT_TRUE(uut.get(1));

    uut.clr();
    ASSERT_EQ(uut.to_string(), "0b00");
    ASSERT_FALSE(uut.get(0));
    ASSERT_FALSE(uut.get(1));

    uut.set(0);
    uut.clr(1);
    ASSERT_EQ(uut.to_string(), "0b01");
    ASSERT_TRUE(uut.get(0));
    ASSERT_FALSE(uut.get(1));

    uut.set(1);
    uut.clr(0);
    ASSERT_EQ(uut.to_string(), "0b10");
    ASSERT_FALSE(uut.get(0));
    ASSERT_TRUE(uut.get(1));
}

TEST(bitset, flip) {
    bitset<5> uut;

    EXPECT_EQ(uut.size_bits(), 5);
    EXPECT_EQ(uut.size_in_memory_bits(), 8);
    EXPECT_EQ(uut.size_bytes_underlying(), 1);

    uut.set();
    uut.clr(2);
    uut.clr(4);
    ASSERT_EQ(uut.to_string(), "0b01011");
    EXPECT_TRUE(uut.get(0));
    EXPECT_TRUE(uut.get(1));
    EXPECT_FALSE(uut.get(2));
    EXPECT_TRUE(uut.get(3));
    EXPECT_FALSE(uut.get(4));

    uut.flip();
    ASSERT_EQ(uut.to_string(), "0b10100");
    EXPECT_FALSE(uut.get(0));
    EXPECT_FALSE(uut.get(1));
    EXPECT_TRUE(uut.get(2));
    EXPECT_FALSE(uut.get(3));
    EXPECT_TRUE(uut.get(4));
}

TEST(bitset, constructors) {
    // bitset<8> uut_str{"11110000"};
    // ASSERT_EQ(uut_str.to_string(), "0b11110000");

    bitset<8> uut_u8{(uint8_t)0b00110011};
    EXPECT_EQ(uut_u8.to_string(), "0b00110011");

    bitset<16> uut_u16{(uint16_t)0b0011001100001111};
    EXPECT_EQ(uut_u16.to_string(), "0b0011001100001111");

    // bitset<32> uut_u32{(uint32_t)0b00110011000011111010010100111100};
    // EXPECT_EQ(uut_u32.to_string(), "0b00110011000011111010010100111100");

    // bitset<64> uut_u64{(uint64_t)0b0011001100001111101001010011110000110011000011111010010100111100};
    // EXPECT_EQ(uut_u64.to_string(), "0b0011001100001111101001010011110000110011000011111010010100111100");
}
#include "gtest/gtest.h"
#include "scratchbuff.h"

#include <iostream>

using namespace hd;


TEST(scratchbuff, alloc) {
    scratchbuff uut;

    int* i = (int*)uut.alloc(sizeof(int));
    ASSERT_TRUE(i != nullptr);

    *i = 10;
    ASSERT_EQ(*i, 10);
}

TEST(scratchbuff, markers) {
    scratchbuff uut;

    int* a = (int*)uut.alloc(sizeof(int));
    Marker marker = uut.set_marker();

    int* b = (int*)uut.alloc(sizeof(int) * 4);
    int* c = (int*)uut.alloc(sizeof(int) * 4);
    ASSERT_EQ((char*)b, (char*)a + sizeof(int));
    ASSERT_EQ((char*)c, (char*)a + sizeof(int) + sizeof(int) * 4);

    uut.reset(marker);
    b = (int*)uut.alloc(sizeof(int) * 4);
    ASSERT_EQ((char*)b, (char*)a + sizeof(int));
}

TEST(scratchbuff, reset) {
    scratchbuff uut;

    int* a = (int*)uut.alloc(sizeof(int) * 10);
    uut.reset();
    int* b = (int*)uut.alloc(sizeof(int));
    ASSERT_EQ(a, b);
}
#include "gtest/gtest.h"
#include "heap.h"

TEST(heap, minheap) {
    hd::minheap<int> minh;

    minh.push(10);
    EXPECT_EQ(minh.top().value(), 10);

    minh.push(20);
    EXPECT_EQ(minh.top().value(), 10);

    minh.push(5);
    EXPECT_EQ(minh.top().value(), 5);

    minh.pop();
    EXPECT_EQ(minh.top().value(), 10);

    minh.pop();
    EXPECT_EQ(minh.top().value(), 20);

    minh.pop();
    EXPECT_FALSE(minh.top().has_value());
}

TEST(heap, maxheap) {
    hd::maxheap<int> maxh;

    maxh.push(10);
    EXPECT_EQ(maxh.top().value(), 10);

    maxh.push(20);
    EXPECT_EQ(maxh.top().value(), 20);

    maxh.push(5);
    EXPECT_EQ(maxh.top().value(), 20);

    maxh.pop();
    EXPECT_EQ(maxh.top().value(), 10);

    maxh.pop();
    EXPECT_EQ(maxh.top().value(), 5);

    maxh.pop();
    EXPECT_FALSE(maxh.top().has_value());
}
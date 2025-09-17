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

TEST(heap, minheap_top_n) {
    hd::minheap<int> h;

    h.push(10);
    h.push(20);
    h.push(5);

    auto v = h.top_n(5);
    EXPECT_EQ(v.size(), 3U);
    EXPECT_EQ(v[0], 5);
    EXPECT_EQ(v[1], 10);
    EXPECT_EQ(v[2], 20);
}

TEST(heap, maxheap_top_n) {
    hd::maxheap<int> h;

    h.push(10);
    h.push(20);
    h.push(5);

    auto v = h.top_n(5);
    EXPECT_EQ(v.size(), 3U);
    EXPECT_EQ(v[0], 20);
    EXPECT_EQ(v[1], 10);
    EXPECT_EQ(v[2], 5);
}

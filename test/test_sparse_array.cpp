#include "gtest/gtest.h"
#include "sparse_array.h"

TEST(sparse_array, works) {
    hd::sparse_array<int> sa(10);

    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(sa.find(i), 0);
    }

    sa.insert(2, 10);
    EXPECT_EQ(sa.find(2), 10);
    sa.insert(3, 10);
    EXPECT_EQ(sa.find(3), 10);
    sa.insert(5, 10);
    EXPECT_EQ(sa.find(5), 10);

    sa.erase(5);
    EXPECT_EQ(sa.find(5), 0);

    EXPECT_EQ(sa.find(0), 0);
    EXPECT_EQ(sa.find(1), 0);
    EXPECT_EQ(sa.find(2), 10);
    EXPECT_EQ(sa.find(3), 10);
    EXPECT_EQ(sa.find(4), 0);
    EXPECT_EQ(sa.find(5), 0);
    EXPECT_EQ(sa.find(6), 0);
    EXPECT_EQ(sa.find(7), 0);
    EXPECT_EQ(sa.find(8), 0);
    EXPECT_EQ(sa.find(9), 0);
    EXPECT_EQ(sa.find(10), 0);
}

TEST(sparse_array, iterator) {
    hd::sparse_array<int> sa(10);

    for (auto it = sa.begin(); it != sa.end(); it++) {
        EXPECT_EQ(*it, 0);
    }

    sa.insert(2, 20);
    sa.insert(3, 30);
    sa.insert(5, 50);

    size_t index = 0;
    for (auto it = sa.begin(); it != sa.end(); it++) {
        if (index == 2) EXPECT_EQ(*it, 20);
        else if (index == 3) EXPECT_EQ(*it, 30);
        else if (index == 5) EXPECT_EQ(*it, 50);
        else EXPECT_EQ(*it, 0);
        index++;
    }

    sa.erase(5);
    index = 0;
    for (auto it = sa.begin(); it != sa.end(); it++) {
        if (index == 2) EXPECT_EQ(*it, 20);
        else if (index == 3) EXPECT_EQ(*it, 30);
        else EXPECT_EQ(*it, 0);
        index++;
    }
}

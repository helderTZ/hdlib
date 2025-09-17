#include "gtest/gtest.h"
#include "sort.h"

#include <random>
#include <vector>

TEST(sort, mergesort) {
    std::vector<int> a{1, 3, 2, 7, 6};
    std::vector<int> b{1, 3, 2, 7, 6};
    hd::mergesort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    ASSERT_EQ(a, b);
}

TEST(sort, mergesort2) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist;
    std::vector<int> a;
    for (int i = 0; i < 1000; i++) a.push_back(dist(rng));
    std::vector<int> b = a;
    hd::mergesort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    ASSERT_EQ(a, b);
}

TEST(sort, mergesort_container) {
    std::vector<int> a{1, 3, 2, 7, 6};
    std::vector<int> b{1, 3, 2, 7, 6};
    hd::mergesort(a);
    std::sort(b.begin(), b.end());

    ASSERT_EQ(a, b);
}


// TEST(sort, quicksort) {
//     std::vector<int> a{1, 3, 2, 7, 6};
//     std::vector<int> b{1, 3, 2, 7, 6};
//     hd::quicksort(a.begin(), a.end());
//     std::sort(b.begin(), b.end());

//     ASSERT_EQ(a, b);
// }

// TEST(sort, quicksort2) {
//     std::random_device dev;
//     std::mt19937 rng(dev());
//     std::uniform_int_distribution<int> dist;
//     std::vector<int> a;
//     for (int i = 0; i < 1000; i++) a.push_back(dist(rng));
//     std::vector<int> b = a;
//     hd::quicksort(a.begin(), a.end());
//     std::sort(b.begin(), b.end());

//     ASSERT_EQ(a, b);
// }
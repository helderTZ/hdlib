#include "gtest/gtest.h"
#include "timeit.h"

#include <iostream>

TEST(timeit, sleep1s) {
    auto ns = hd::timeit([](){ sleep(1); });
    std::cout << "sleep(1) took " << ns.count();
}
#include "gtest/gtest.h"
#include "jthread.h"

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

TEST(jthread, it_works) {

    using namespace std::chrono_literals;

    std::vector<hd::jthread<void(void)>> threads;
    threads.reserve(4);

    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([=]() {
            std::cout << ">>> Thread " << i << " started ...\n ";
            std::this_thread::sleep_for(i*200ms);
            std::cout << "<<< Thread " << i << " finished.\n";
        });
    }
}

TEST(jthread, movable) {
    hd::jthread<void(void)> t1([]() { std::cout << "T1\n"; });
    hd::jthread<void(void)> t2([]() { std::cout << "T2\n"; });
    {
        hd::jthread<void(void)> t3 = std::move(t1);
        std::swap(t1, t2);
    }
}
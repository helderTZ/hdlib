#include "gtest/gtest.h"
#include "latch.h"

#include <mutex>
#include <vector>
#include <thread>
#include <algorithm>

TEST(latch, simple_test) {
    const size_t NR_THREADS = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    hd::latch checkpoint1{NR_THREADS};
    hd::latch checkpoint2{NR_THREADS};
    std::vector<int> landmarks;
    std::mutex mtx;

    EXPECT_EQ(landmarks.size(), 0U);

    for (size_t i = 0; i < NR_THREADS; i++) {
        threads.emplace_back([&]() {
            {
                std::scoped_lock lk(mtx);
                landmarks.push_back(1);
            }
            checkpoint1.arrive_and_wait();
            EXPECT_EQ(landmarks.size(), NR_THREADS);
            checkpoint2.arrive_and_wait();
            {
                std::scoped_lock lk(mtx);
                landmarks.push_back(1);
            }
        });
    }

    std::for_each(threads.begin(), threads.end(), [](auto& t) { t.join(); });

    EXPECT_EQ(landmarks.size(), NR_THREADS*2);
}
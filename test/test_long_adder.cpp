#include "gtest/gtest.h"
#include "long_adder.h"

#include <condition_variable>
#include <mutex>
#include <vector>
#include <thread>
#include <atomic>

TEST(long_adder, simple_test) {
    hd::long_adder acc;
    std::vector<std::thread> threads;

    EXPECT_EQ(acc.sum(), 0U);

    for (size_t i = 0; i < std::thread::hardware_concurrency(); i++) {
        threads.emplace_back([&]() {
            uint64_t& a = acc.register_thread();
            for (int i = 0; i < 1'000'000; i++) { a++; }
        });
    }

    std::for_each(threads.begin(), threads.end(), [](auto& t) { t.join(); });

    EXPECT_EQ(acc.sum(), 1'000'000U * std::thread::hardware_concurrency());
}

TEST(long_adder, just_atomics_to_compare_execution_time) {
    std::vector<std::thread> threads;
    std::atomic<uint64_t> acc;
    acc.store(0U);

    EXPECT_EQ(acc.load(), 0U);

    for (size_t i = 0; i < std::thread::hardware_concurrency(); i++) {
        threads.emplace_back([&]() {
            for (int i = 0; i < 1'000'000; i++) { acc.fetch_add(1U); }
        });
    }

    std::for_each(threads.begin(), threads.end(), [](auto& t) { t.join(); });

    EXPECT_EQ(acc.load(), 1'000'000U * std::thread::hardware_concurrency());
}
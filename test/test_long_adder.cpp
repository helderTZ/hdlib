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
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<unsigned int> registered_threads;
    registered_threads.store(0U);

    EXPECT_EQ(acc.sum(), 0U);

    for (size_t i = 0; i < std::thread::hardware_concurrency(); i++) {
        threads.emplace_back([&]() {
            {
                std::unique_lock<std::mutex> lock(mtx);
                acc.register_thread();
                registered_threads.fetch_add(1U, std::memory_order::memory_order_relaxed);
                if (registered_threads.load(std::memory_order::memory_order_relaxed) == std::thread::hardware_concurrency()) {
                    lock.unlock();
                    cv.notify_all();
                } else {
                    cv.wait(lock, [&]() { return registered_threads.load(std::memory_order::memory_order_relaxed) == std::thread::hardware_concurrency(); });
                }
            }
            for (int i = 0; i < 1'000'000; i++) { acc.inc(); }
        });
    }

    std::for_each(threads.begin(), threads.end(), [](auto& t) { t.join(); });

    EXPECT_EQ(acc.sum(), 1'000'000U * std::thread::hardware_concurrency());
}
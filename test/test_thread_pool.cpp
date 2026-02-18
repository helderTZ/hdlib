#include "gtest/gtest.h"
#include "thread_pool.h"

#include <atomic>

using namespace hd;

TEST(thread_pool, threads_run_to_completion) {
    std::atomic<int> counter{};
    {
        hd::thread_pool pool(4);
        auto f1 = pool.enqueue([&]() { counter.fetch_add(1); });
        auto f2 = pool.enqueue([&]() { counter.fetch_add(1); });
        auto f3 = pool.enqueue([&]() { counter.fetch_add(1); });
        auto f4 = pool.enqueue([&]() { counter.fetch_add(1); });
    }

    ASSERT_EQ(counter.load(), 4);
}

TEST(thread_pool, threads_return_value) {    
    hd::thread_pool pool(4);
    auto f1 = pool.enqueue([&]() { return 1; });
    auto f2 = pool.enqueue([&]() { return 2; });
    auto f3 = pool.enqueue([&]() { return 3; });
    auto f4 = pool.enqueue([&]() { return 4; });

    ASSERT_EQ(f1.get(), 1);
    ASSERT_EQ(f2.get(), 2);
    ASSERT_EQ(f3.get(), 3);
    ASSERT_EQ(f4.get(), 4);
}
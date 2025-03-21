#include "gtest/gtest.h"
#include "parallel.h"

#include <atomic>
#include <limits>
#include <cstdint>

std::atomic<uint16_t> G_sum = 0;

class ParallelTest : public testing::Test
{
protected:
    void SetUp() override
    {
        G_sum.store(0U);
    }

    void  TearDown() override
    {
    }
};

TEST_F(ParallelTest, single_thread_small) {
    hd::parfor(static_cast<uint16_t>(0U), static_cast<uint16_t>(10U), [](uint16_t) {
        G_sum.fetch_add(1U);
    });

    ASSERT_EQ(G_sum.load(), 10U);
}

TEST_F(ParallelTest, single_thread) {
    hd::parfor(static_cast<uint16_t>(0U), std::numeric_limits<uint16_t>::max(), [](uint16_t) {
        G_sum.fetch_add(1U);
    }, 1);

    ASSERT_EQ(G_sum.load(), std::numeric_limits<uint16_t>::max());
}

TEST_F(ParallelTest, two_threads) {
    hd::parfor(static_cast<uint16_t>(0U), std::numeric_limits<uint16_t>::max(), [](uint16_t) {
        G_sum.fetch_add(1U);
    }, 2);

    ASSERT_EQ(G_sum.load(), std::numeric_limits<uint16_t>::max());
}

TEST_F(ParallelTest, multiple_threads) {
    hd::parfor(static_cast<uint16_t>(0U), std::numeric_limits<uint16_t>::max(), [](uint16_t) {
        G_sum.fetch_add(1U);
    });

    ASSERT_EQ(G_sum.load(), std::numeric_limits<uint16_t>::max());
}

TEST_F(ParallelTest, two_threads_offset) {
    hd::parfor(static_cast<uint16_t>(10U), std::numeric_limits<uint16_t>::max(), [](uint16_t) {
        G_sum.fetch_add(1U);
    }, 2);

    ASSERT_EQ(G_sum.load(), std::numeric_limits<uint16_t>::max()-10U);
}
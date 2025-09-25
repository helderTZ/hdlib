#pragma once

#include <atomic>
#include <cstdint>
#include <thread>

namespace hd {

class latch {
private:
    std::atomic<uint64_t> _counter;

public:
    latch(uint64_t initial_value)
        : _counter(initial_value)
        {}

    void count_down() {
        _counter.fetch_sub(1U, std::memory_order_release);
    }

    bool try_wait() {
        return _counter.load(std::memory_order_acquire) == 0U;
    }

    void wait() {
        while (_counter.load(std::memory_order_acquire) != 0U) {
            std::this_thread::yield();
        }
    }

    void arrive_and_wait() {
        count_down();
        wait();
    }
};

}   // namespace hd

#pragma once

#include <unordered_map>
#include <atomic>
#include <thread>
#include <algorithm>
#include <numeric>

namespace hd {

class long_adder {
private:
    std::unordered_map<std::thread::id, std::atomic<uint64_t>> m;

public:
    long_adder() = default;

    void register_thread() {
        m[std::this_thread::get_id()] = 0U;
    }

    void inc() {
        m[std::this_thread::get_id()].fetch_add(1U, std::memory_order_relaxed);
    }

    uint64_t sum() {
        uint64_t acc = 0U;
        for (const auto& [id, val] : m) {
            acc += val.load(std::memory_order_relaxed);
        }
        return acc;
    }
};

}   // namespace hd

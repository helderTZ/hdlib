#pragma once

#include <unordered_map>
#include <mutex>
#include <thread>

namespace hd {

class long_adder {
private:
    std::mutex mtx;
    std::unordered_map<std::thread::id, uint64_t> m;

public:
    long_adder() = default;

    uint64_t& register_thread() {
        std::scoped_lock lk(mtx);
        m[std::this_thread::get_id()] = 0U;
        return m[std::this_thread::get_id()];
    }

    void inc() {
        m[std::this_thread::get_id()]++;
    }

    uint64_t sum() {
        uint64_t acc = 0U;
        for (const auto& [id, val] : m) {
            acc += val;
        }
        return acc;
    }
};

}   // namespace hd

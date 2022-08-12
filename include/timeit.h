#pragma once

#include <chrono>
#include <functional>

namespace hd {

std::chrono::duration<std::chrono::nanoseconds> timeit(std::function<void()>&& func) {
    using std::chrono::steady_clock;
    using std::chrono::nanoseconds;
    auto start = steady_clock::now;
    func();
    auto stop = steady_clock::now;
    return std::chrono::duration_cast<nanoseconds>(stop - start);
}

} // namespace hd

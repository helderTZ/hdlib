#pragma once

#include <chrono>
#include <functional>

namespace hd {

/**
 * @brief Times function execution
 * 
 * Returns nanoseconds spent running \p func.
 * 
 * @param func      function to time (will be executed)
 * @return size_t   elapsed time in nanoseconds
 */
auto timeit(std::function<void()>&& func) {
    using std::chrono::steady_clock;
    using std::chrono::nanoseconds;
    using std::chrono::duration_cast;

    auto start = steady_clock::now();
    func();
    auto stop = steady_clock::now();
    return duration_cast<nanoseconds>(stop - start).count();
}

} // namespace hd

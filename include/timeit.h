#pragma once

#include <time.h>

// #include <chrono>
#include <functional>

namespace hd {

/** i don't understand how to work with chrono, can't make this compile */
// auto timeit(std::function<void()>&& func) {
//     using std::chrono::steady_clock;
//     using std::chrono::nanoseconds;
//     using std::chrono::duration_cast;

//     auto start = steady_clock::now;
//     func();
//     auto stop = steady_clock::now;
//     return duration_cast<nanoseconds>(stop - start);
// }

/**
 * @brief Times function execution
 * 
 * Returns nanoseconds spent running \p func.
 * 
 * @param func      function to time (will be executed)
 * @return size_t   elapsed time in nanoseconds
 */
size_t timeit(std::function<void()>&& func) {
    struct timespec start, stop;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    func();
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop);

    return (stop.tv_nsec - start.tv_nsec)
            + ( (stop.tv_sec * 1e9) - (start.tv_sec * 1e9) );
}

} // namespace hd

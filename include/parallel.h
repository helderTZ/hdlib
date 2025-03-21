#pragma once

#include <algorithm>
#include <thread>

namespace hd {

template<typename I, typename Fn>
void parfor(const I begin, const I end, Fn&& fn, size_t parts = std::thread::hardware_concurrency())
{
    std::vector<std::thread> threads;
    threads.reserve(parts);

    for(size_t i = 0; i < parts; ++i)
    {
        I start = i * (end-begin) / parts + begin;
        I stop = (i+1) * (end-begin) / parts + begin;

        threads.emplace_back(std::thread([start, stop, fn = std::move(fn)]() {
            for (I j = start; j < stop; ++j)
            {
                fn(j);
            }
        }));
    }

    std::for_each(threads.begin(), threads.end(), [](auto& t) { t.join(); });
}

}   // namespace hd

#pragma once

#include <unordered_map>
#include <iterator>
#include <utility>

namespace hd {

/**
 * @brief Analog to Counter from python
 *
 * @param container collection of non-distinct values
 * @return std::unordered_map with the frequencies of each value in @param container
 */
template<typename Container>
std::unordered_map<typename std::iterator_traits<decltype(std::declval<Container>().begin())>::value_type, size_t>
make_counter(const Container& container) {
    using T = typename std::iterator_traits<decltype(std::declval<Container>().begin())>::value_type;

    std::unordered_map<T, size_t> frequencies;
    for (const auto& item : container) {
        if (frequencies.find(item) != frequencies.end()) {
            frequencies[item]++;
        } else {
            frequencies[item] = 1U;
        }
    }

    return frequencies;
}

}   // namespace hd

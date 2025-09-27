#pragma once

#include <algorithm>
#include <type_traits>
#include <iterator>
#include <random>

namespace hd {

namespace impl {

struct partition_algo {};
struct hoare : public partition_algo {};
struct lomuto : public partition_algo {};

// Lomuto's partition algorithm
template <typename Iterator, typename Predicate>
Iterator partition(Iterator begin, Iterator end, Predicate pred, const lomuto& algo = {}) {
    typename std::iterator_traits<Iterator>::value_type pivot = *std::prev(end);
    Iterator i = begin;
    for (auto j = begin; j != end; std::advance(j, 1)) {
        if (pred(*j, pivot)) {
            std::iter_swap(i, j);
        }
        std::advance(i, 1);
    }

    std::iter_swap(i, std::prev(end));

    return i;
}

// Hoare's partition algorithm
template <typename Iterator, typename Predicate>
Iterator partition(Iterator begin, Iterator end, Predicate pred, const hoare& algo = {}) {
    static std::mt19937 rng(std::random_device{}());

    size_t size = std::distance(begin, end);
    
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, size);

    // typename std::iterator_traits<Iterator>::value_type pivot = *std::next(begin, dist(rng));
    typename std::iterator_traits<Iterator>::value_type pivot = *std::next(begin, size/2);
    Iterator i = begin;
    Iterator j = std::prev(end);

    while (true) {
        while (pred(*i, pivot)) {
            i = std::next(i);
        }

        while (!pred(*j, pivot)) {
            j = std::prev(j);
        }

        if (std::distance(i, j) <= 0) {
            return j;
        }

        std::iter_swap(i, j);
    }
}

}   // namespace impl

template <typename Iterator, typename Predicate>
void quicksort(Iterator begin, Iterator end, Predicate pred) {    
    size_t size = std::distance(begin, end);

    if (size <= 1) return;
    if (size == 2) {
        Iterator next = std::next(begin);
        if (!pred(*begin, *next)) {
            std::iter_swap(begin, next);
        }
        return;
    }
    
    Iterator pivot = impl::partition(begin, end, pred, impl::lomuto{});
    quicksort(begin, pivot, pred);
    quicksort(std::next(pivot), end, pred);
}

template <typename Iterator>
void quicksort(Iterator begin, Iterator end) {
    quicksort(begin, end, std::less<typename std::iterator_traits<Iterator>::value_type>());
}

namespace impl {

template<typename Iterator, typename Predicate>
void merge(Iterator begin1, Iterator end1, Iterator begin2, Iterator end2, Predicate pred) {
    std::vector<typename std::iterator_traits<Iterator>::value_type> temp;
    temp.reserve(std::distance(begin1, end1) + std::distance(begin2, end2));

    Iterator l = begin1;
    Iterator r = begin2;
    while (l != end1 && r != end2) {
        if (!pred(*l, *r)) {
            temp.push_back(*r);
            r++;
        } else {
            temp.push_back(*l);
            l++;
        }
    }

    while (l != end1) {
        temp.push_back(*l);
        l++;
    }

    while (r != end2) {
        temp.push_back(*r);
        r++;
    }

    Iterator it = begin1;
    std::for_each(temp.cbegin(), temp.cend(), [&](const auto i) { *it = i; it++; });
}

}   // namespace impl

template<typename Iterator, typename Predicate>
Iterator mergesort(Iterator begin, Iterator end, Predicate pred) {
    size_t size = std::distance(begin, end);
    auto mid = std::next(begin, size / 2);

    Iterator left = begin;
    Iterator right = mid;
    if (size > 2) {
        left = mergesort(begin, mid, pred);
        right = mergesort(mid, end, pred);
    }

    impl::merge(left, mid, right, end, pred);
    return left;
}

template<typename Iterator>
Iterator mergesort(Iterator begin, Iterator end) {
    return mergesort(begin, end, std::less<typename std::iterator_traits<Iterator>::value_type>());
}

template<typename Container, typename Predicate>
typename Container::iterator mergesort(Container& container, Predicate pred) {
    return mergesort(container.begin(), container.end(), pred);
}

template<typename Container>
typename Container::iterator mergesort(Container& container) {
    return mergesort(container.begin(), container.end(), std::less<typename Container::value_type>());
}

}

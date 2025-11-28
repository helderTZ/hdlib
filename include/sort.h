#pragma once

#include <algorithm>
#include <type_traits>
#include <iterator>
#include <random>

namespace hd {

namespace impl {

// Lomuto's partition algorithm
struct lomuto {
    template <typename Iterator, typename Predicate>
    Iterator operator()(Iterator begin, Iterator end, Predicate pred) {
        Iterator pivot = std::prev(end);
        Iterator i = begin; // always points to the first element > pivot
        for (auto j = begin; j != pivot; j++) {
            if (pred(*j, *pivot)) {
                std::iter_swap(i, j);
                i++;
            }
        }

        std::iter_swap(i, pivot);
        return i;
    }
};

// Hoare's partition algorithm
struct hoare {
    template <typename Iterator, typename Predicate>
    Iterator operator()(Iterator begin, Iterator end, Predicate pred) {
        typename std::iterator_traits<Iterator>::value_type pivot = *begin;
        Iterator i = begin;
        Iterator j = std::prev(end);

        if (i == j) {
            return i;
        }

        while (true) {
            // stop when ! (*i < pivot) → i.e., when *i >= pivot
            while (pred(*i, pivot)) { i++; }

            // stop when (*j <= pivot) → i.e., when !(pivot < *j)
            while (pred(pivot, *j)) { j--; }

            // when j >= i we finished
            if (i == j || std::next(j) == i) {
                return j;
            }

            std::iter_swap(i, j);
            i++;
            j--;
        }
    }
};

}   // namespace impl

template <typename Iterator, typename Predicate, typename PartitionAlgo>
Iterator quicksort(Iterator begin, Iterator end, Predicate pred, PartitionAlgo partition) {
    // empty or single element
    if (begin == end || std::next(begin) == end) return begin;

    // two elements
    if (std::next(begin, 2) == end) {
        Iterator next = std::next(begin);
        if (!pred(*begin, *next)) {
            std::iter_swap(begin, next);
        }
        return begin;
    }

    Iterator pivot = partition(begin, end, pred);
    quicksort(begin, pivot, pred, partition);
    quicksort(std::next(pivot), end, pred, partition);

    // unreachable
    return begin;
}

template <typename Iterator, typename PartitionAlgo = impl::lomuto>
Iterator quicksort(Iterator begin, Iterator end) {
    return quicksort(begin, end, std::less<typename std::iterator_traits<Iterator>::value_type>(), PartitionAlgo{});
}

template <typename Container, typename PartitionAlgo = impl::lomuto>
typename Container::iterator quicksort(Container& container) {
    return quicksort(container.begin(), container.end(), std::less<typename std::iterator_traits<typename Container::iterator>::value_type>(), PartitionAlgo{});
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

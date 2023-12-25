# hdlib

Collection of utilities I find useful.
Some are actually useful, others are reimplementations of standard library I do just for fun.

## Algorithms
### Generic
- [x] min
- [x] max
- [x] gcd
- [x] reduce
- [x] has_duplicates
- [x] fft
- [x] last
- [x] enumerate (python-like enumerate)

### String
- [x] has_whitespace
- [x] is_alphanumeric
- [x] is_palindrome
- [x] is_anagram
- [x] to_lower
- [x] to_upper
- [x] starts_with
- [x] ends_with
- [x] contains
- [x] lev (levenshtein distance)

## Containers
- [x] ordered_vector
- [x] small_vector
- [x] static_vector

## Others
- [x] channel (golang-like channels)
- [x] lifetime (class that traces constructions, copies, moves, ...)
- [x] timeit (for measuring a function's duration)
- [x] hashes (sha256, fnv_1a)
- [x] bloom filter
- [x] biggerints (bigger integer types such as `uint128_t` and `uint256_t`)

## Build instructions

Using CMake:
```console
$ mkdir build && cd build && cmake ..
```

Using Bazel:
```console
$ bazel build //:hdlib
$ bazel build //... # to build tests
```

## TODO

- ranges
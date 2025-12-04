project(hdlib)
cmake_minimum_required(VERSION 3.16)

set(CXX_STANDARD 17)

add_library(hdlib INTERFACE)
target_include_directories(hdlib INTERFACE include)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

add_compile_options (-Wall -Wextra -std=c++17)

function(build_unit_test test_name)
    message(${test_name})
    add_executable(${test_name} test/${test_name}.cpp)
    target_link_libraries(${test_name} hdlib gtest_main)
    target_compile_options(${test_name} PRIVATE -fsanitize=address -g -O0)
    target_link_options(${test_name} PRIVATE -fsanitize=address)
    gtest_discover_tests(${test_name})
endfunction()

build_unit_test(test_ordered_vector)
build_unit_test(test_small_vector)
build_unit_test(test_static_vector)
build_unit_test(test_timeit)
build_unit_test(test_string_utils)
build_unit_test(test_algorithm)
build_unit_test(test_channel)
build_unit_test(test_hash)
build_unit_test(test_bloom)
build_unit_test(test_biggerints)
build_unit_test(test_defer)
build_unit_test(test_signals)
build_unit_test(test_property)
build_unit_test(test_scratchbuff)
build_unit_test(test_string_view)
build_unit_test(test_jthread)
build_unit_test(test_sort)
build_unit_test(test_heap)
build_unit_test(test_parallel)
build_unit_test(test_trie)
build_unit_test(test_cache)
build_unit_test(test_long_adder)
build_unit_test(test_latch)
build_unit_test(test_counter)
build_unit_test(test_sparse_array)

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

add_executable(test_ordered_vector test/test_ordered_vector.cpp)
target_link_libraries(test_ordered_vector hdlib gtest_main)
gtest_discover_tests(test_ordered_vector)

add_executable(test_small_vector test/test_small_vector.cpp)
target_link_libraries(test_small_vector hdlib gtest_main)
gtest_discover_tests(test_small_vector)

add_executable(test_static_vector test/test_static_vector.cpp)
target_link_libraries(test_static_vector hdlib gtest_main)
gtest_discover_tests(test_static_vector)

add_executable(test_timeit test/test_timeit.cpp)
target_link_libraries(test_timeit hdlib gtest_main)
gtest_discover_tests(test_timeit)

add_executable(test_string_utils test/test_string_utils.cpp)
target_link_libraries(test_string_utils hdlib gtest_main)
gtest_discover_tests(test_string_utils)

add_executable(test_algorithm test/test_algorithm.cpp)
target_link_libraries(test_algorithm hdlib gtest_main)
gtest_discover_tests(test_algorithm)

add_executable(test_channel test/test_channel.cpp)
target_link_libraries(test_channel hdlib gtest_main)
gtest_discover_tests(test_channel)

add_executable(test_bitset test/test_bitset.cpp)
target_link_libraries(test_bitset hdlib gtest_main)
gtest_discover_tests(test_bitset)

add_executable(test_hash test/test_hash.cpp)
target_link_libraries(test_hash hdlib gtest_main)
gtest_discover_tests(test_hash)
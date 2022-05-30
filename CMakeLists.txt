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
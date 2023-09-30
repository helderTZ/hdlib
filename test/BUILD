cc_test(
    name = "test_hdlib",
    size = "small",
    srcs = [
        "test_ordered_vector.cpp",
        "test_small_vector.cpp",
        "test_static_vector.cpp",
        "test_string_utils.cpp",
    ],
    deps = [
        "//:hdlib",
        "@googletest//:gtest_main",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "test_algorithm",
    size = "small",
    srcs = [
        "test_algorithm.cpp",
    ],
    deps = [
        "//:hdlib",
        "@googletest//:gtest_main",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "test_timeit",
    srcs = [
        "test_timeit.cpp",
    ],
    deps = [
        "//:hdlib",
    ],
    visibility = ["//visibility:public"],
)

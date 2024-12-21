#include "gtest/gtest.h"
#include "string_view.h"

#include <string>
#include <cstring>

using namespace hd;

TEST(sv, basic_functionality) {
    string_view sv("hello there");
    EXPECT_EQ(sv.size(), 11U);
    EXPECT_EQ(strncmp(sv.data(), "hello there", sv.size()), 0);

    string_view sv2 = sv.substring(0, 6);
    EXPECT_EQ(sv2.size(), 6U);
    EXPECT_EQ(strncmp(sv2.data(), "hello ", sv2.size()), 0);
}

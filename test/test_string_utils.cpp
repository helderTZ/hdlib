#include "gtest/gtest.h"
#include "string_utils.h"

#include <string>

using namespace hd;

TEST(string_utils, has_whitespace) {
    EXPECT_FALSE(has_whitespace("nowhitespace"));
    EXPECT_TRUE(has_whitespace("has whitespace"));
}

TEST(string_utils, is_alphanumeric) {
    EXPECT_FALSE(is_alphanumeric("not alphanumeric!"));
    EXPECT_TRUE(is_alphanumeric("iamalphanumeric21"));
}

TEST(string_utils, is_palindrome) {    
    EXPECT_FALSE(is_palindrome("RAFT"));
    EXPECT_FALSE(is_palindrome("BORAT"));
    EXPECT_TRUE(is_palindrome("ANA"));
    EXPECT_TRUE(is_palindrome("DOGMAIAMGOD"));
}

TEST(string_utils, is_anagram) {
    EXPECT_FALSE(is_anagram("BLAH", "BLEH"));
    EXPECT_FALSE(is_anagram("DEBITCARD", "BAQCREDIT"));
    EXPECT_TRUE(is_anagram("DEBITCARD", "BADCREDIT"));
}

TEST(string_utils, to_upper) {
    std::string uut = "Hello There";
    to_upper(uut);
    EXPECT_STREQ(uut.c_str(), "HELLO THERE");
}

TEST(string_utils, to_lower) {
    std::string uut = "Hello There";
    to_lower(uut);
    EXPECT_STREQ(uut.c_str(), "hello there");
}

TEST(string_utils, starts_with) {
    EXPECT_TRUE(starts_with("some_file.cpp", "some"));
    EXPECT_FALSE(starts_with("some_file.cpp", ".cpp"));
}

TEST(string_utils, ends_with) {
    EXPECT_TRUE(ends_with("some_file.cpp", ".cpp"));
    EXPECT_FALSE(ends_with("some_file.cpp", ".h"));
}

TEST(string_utils, contains) {
    EXPECT_TRUE(contains("some_file.cpp", ".cpp"));
    EXPECT_FALSE(contains("some_file.cpp", ".h"));
    EXPECT_FALSE(contains(".cpp", "longname.cpp"));
}

TEST(string_utils, reverse) {
    std::string uut0 = "";
    std::string uut1 = "a";
    std::string uut2 = "reverse me";
    std::string uut3 = "reverse me!";
    reverse(uut0);
    reverse(uut1);
    reverse(uut2);
    reverse(uut3);
    EXPECT_EQ(uut0, std::string(""));
    EXPECT_EQ(uut1, std::string("a"));
    EXPECT_EQ(uut2, std::string("em esrever"));
    EXPECT_EQ(uut3, std::string("!em esrever"));
}

TEST(string_utils, lev) {
    EXPECT_EQ(lev("add", "daddy"), 2);
    EXPECT_EQ(lev("", "foo"), 3);
    EXPECT_EQ(lev("foo", ""), 3);
    EXPECT_EQ(lev("foo", "bar"), 3);
    EXPECT_EQ(lev("", ""), 0);
}
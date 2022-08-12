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
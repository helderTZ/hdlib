#include "gtest/gtest.h"
#include "trie.h"

TEST(trie, test1) {
    hd::Trie trie;

    trie.insert("apple");
    EXPECT_TRUE(trie.search("apple"));
    EXPECT_FALSE(trie.search("app"));
    EXPECT_TRUE(trie.startsWith("app"));

    trie.insert("app");
    EXPECT_TRUE(trie.search("app"));
}
#include "gtest/gtest.h"
#include "hash.h"

#include <sstream>
#include <iomanip>
#include <string>

using namespace hd;

template<typename T>
std::string to_hex(T& container) {
    char ss[container.size()*4];
    int n = 0;
    for (auto& v : container) {
        n += sprintf(ss+n, "%02x", v);
    }
    return std::string(ss);
}

TEST(hash, sha256_it_works) {
    const char* msg = "The quick brown fox jumps over the lazy dog";
    std::vector<uint8_t> expected = {
        0x73, 0x0e, 0x10, 0x9b, 0xd7, 0xa8, 0xa3, 0x2b, 0x1c, 0xb9, 0xd9, 0xa0, 0x9a, 0xa2,
        0x32, 0x5d, 0x24, 0x30, 0x58, 0x7d, 0xdb, 0xc0, 0xc3, 0x8b, 0xad, 0x91, 0x15, 0x25
    };

    auto digest = sha256((void*)msg, strlen(msg));

    EXPECT_EQ(digest.size(), expected.size());
    EXPECT_EQ(digest, expected) << "digest   : " << to_hex(digest) << "\nexpected : " << to_hex(expected);
}

TEST(hash, sha256_empty_str) {
    const char* msg = "";
    std::vector<uint8_t> expected = {
        0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4,
        0xc8, 0x99, 0x6f, 0xb9, 0x24, 0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b,
        0x93, 0x4c, 0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55
    };

    auto digest = sha256((void*)msg, strlen(msg));

    EXPECT_EQ(digest.size(), expected.size());
    EXPECT_EQ(digest, expected) << "digest   : " << to_hex(digest) << "\nexpected : " << to_hex(expected);
}

TEST(hash, it_works) {
    const char* msg = "hello world";
    uint64_t expected = 0x779a65e7023cd2e7U;

    auto digest = fnv_1a((void*)msg, strlen(msg));

    EXPECT_EQ(digest, expected) << "digest   : " << digest << "\nexpected : " << expected;
}

TEST(hash, fnv1a_empty_str) {
    const char* msg = "";
    uint64_t expected = 14695981039346656037U;

    auto digest = fnv_1a((void*)msg, strlen(msg));

    EXPECT_EQ(digest, expected) << "digest   : " << digest << "\nexpected : " << expected;
}
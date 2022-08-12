#pragma once

#include <string>
#include <string_view>
#include <stack>

namespace hd {

bool has_whitespace(std::string_view str) {
    return std::any_of(
        str.begin(),
        str.end(),
        [](char c) {
            return c == ' '
                || c == '\n'
                || c == '\r';
        }
    );
}

bool is_palindrome(std::string_view word) {
    size_t len = word.size();
    for (size_t i = 0; i < len / 2; ++i) {
        if (word[i] != word[len-1 - i]) {
            return false;
        }
    }

    return true;
}

bool is_anagram(std::string_view word1, std::string_view word2) {
    if (word1.size() != word2.size()) {
        return false;
    }

    std::stack<char> letters;
    std::for_each(word1.begin(), word1.end(),
        [&](char c) { letters.push(c); }
    );

    for (size_t i = 0; i < word1.size(); ++i) {
        char ch = letters.top();
        letters.pop();
        if (word2.find(ch) == std::string_view::npos) {
            return false;
        }
    }

    return letters.size() == 0;
}

}   // namespace hd
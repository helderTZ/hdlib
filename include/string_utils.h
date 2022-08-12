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

bool is_alphanumeric(std::string_view str) {
    return std::all_of(
        str.begin(),
        str.end(),
        [](char c) {
            bool is_alpha_upper = c >= 'A' && c <= 'Z';
            bool is_alpha_lower = c >= 'a' && c <= 'z';
            bool is_digit = c >= '0' && c <= '9';
            return is_alpha_lower
                || is_alpha_upper
                || is_digit;
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

    std::stack<char> letters1;
    std::vector<char> letters2;
    std::for_each(word1.begin(), word1.end(),
        [&](char c) { letters1.push(c); }
    );
    std::for_each(word2.begin(), word2.end(),
        [&](char c) { letters2.push_back(c); }
    );

    for (size_t i = 0; i < word1.size(); ++i) {
        char ch = letters1.top();
        letters1.pop();
        auto it = find(letters2.begin(), letters2.end(), ch);
        if (it == letters2.end()) {
            return false;
        } else {
            letters2.erase(it);
        }
    }

    return letters1.size() == 0;
}

}   // namespace hd
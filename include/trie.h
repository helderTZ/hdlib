#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace hd {

class Trie {
private:
    struct Node {
        Node() : children{}, leaf{false} {}
        std::unordered_map<char, Node*> children;
        bool leaf;
    };

    char to_lowercase(char c) const {
        constexpr char UPPER_TO_LOWER = 32;
        if (c >= 'A' && c <= 'Z') {
            return c + UPPER_TO_LOWER;
        }
        return c;
    }

    // unordered_map only has contains in c++20
    bool contains(const std::unordered_map<char, Node*>& m, char key) {
        return m.find(key) != m.end();
    }

    void free_node(Node* node) {
        for (auto [c, n] : node->children) {
            if (n != nullptr) {
                free_node(n);
                node->children[c] = nullptr;
            }
        }
        if (node != nullptr) {
            delete node;
        }
    }

    Node* root;

public:
    Trie() : root(new Node()) {}

    ~Trie() {
        free_node(root);
    }
    
    void insert(std::string word) {
        Node* node = root;
        size_t i = 0;
        for (; i < word.size(); i++) {
            if (!contains(node->children, to_lowercase(word[i]))) break;
            if (i == word.size()-1) node->leaf = true;
            node = node->children[to_lowercase(word[i])];
        }
        for (; i < word.size(); i++) {
            node->children[to_lowercase(word[i])] = new Node();
            if (i == word.size()-1) node->leaf = true;
            node = node->children[to_lowercase(word[i])];
        }
    }
    
    bool search(std::string word) {
        Node* node = root;
        if (!node) return false;

        bool reached_leaf = false;
        for (const char c : word) {
            if (node->children[to_lowercase(c)]) {
                reached_leaf = node->leaf;
                node = node->children[to_lowercase(c)];
            } else {
                return false;
            }
        }

        return reached_leaf;
    }
    
    bool startsWith(std::string prefix) {
        Node* node = root;
        if (!node) return false;

        for (const char c : prefix) {
            if (node->children[to_lowercase(c)]) {
                node = node->children[to_lowercase(c)];
            } else {
                return false;
            }
        }

        return true;
    }
};

}   // namespace hd

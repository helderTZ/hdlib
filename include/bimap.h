#pragma once

#include <unordered_map>
#include <list>
#include <type_traits>

namespace hd {

template <typename A, typename B>
class bimap {
private:
    using iterator = typename std::list<std::pair<A, B>>::iterator;
    std::list<std::pair<A, B>> storage;
    std::unordered_map<A, iterator> mapByA;
    std::unordered_map<B, iterator> mapByB;

    std::pair<B, A> reverse(std::pair<A, B> p) {
        return {p.second, p.first};
    }

    iterator last() {
        return (++storage.rbegin()).base();
    }

public:

    struct ProxyB {
        bimap& bm;
        iterator it;

        ProxyB& operator=(const B& b) {
            A a = it->first;
            bm.mapByA.erase(it->first);
            bm.mapByB.erase(it->second);
            bm.storage.erase(it);
            bm.storage.push_back({a, b});
            bm.mapByA[a] = bm.last();
            bm.mapByB[b] = bm.last();
            return *this;
        }

        operator B() const { return it->second; }
    };

    struct ProxyA {
        bimap& bm;
        iterator it;

        ProxyA& operator=(const A& a) {
            B b = it->second;
            bm.mapByA.erase(it->first);
            bm.mapByB.erase(it->second);
            bm.storage.erase(it);
            bm.storage.push_back({a, b});
            bm.mapByA[a] = bm.last();
            bm.mapByB[b] = bm.last();
            return *this;
        }

        operator A() const { return it->first; }
    };

    friend ProxyA;
    friend ProxyB;

    // TODO: U not needed?
    template <typename U,
              typename std::enable_if<std::is_same<U, A>::value && std::is_same<U, B>::value>::type = true>
    void insert(std::pair<A, B> p) {
        if (!mapByA.find(p.first) != mapByA.end()) {
            storage.push_back(p);
            mapByA[p.first] = last();
            mapByB[p.second] = last();
        }
    }

    template <typename std::enable_if<!std::is_same<A, B>::value, bool>::type = true>
    void insert(std::pair<A, B> p) {
        if (!mapByA.find(p.first) != mapByA.end()) {
            storage.push_back(p);
            mapByA[p.first] = last();
            mapByB[p.second] = last();
        }
    }

    template <typename std::enable_if<!std::is_same<A, B>::value, bool>::type = true>
    void insert(std::pair<B, A> p) {
        if (!mapByA.find(p.second) != mapByA.end()) {
            storage.push_back(reverse(p));
            mapByA[p.second] = last();
            mapByB[p.first] = last();
        }
    }

    void emplace(const A a, const B b) {
        if (mapByA.find(a) != mapByA.end()) {
            (*mapByA[a]).second = b;
            (*mapByB[b]).first = a;
            return;
        }

        storage.push_back({a, b});
        mapByA[a] = last();
        mapByB[b] = last();
    }

    void emplace(const B b, const A a) {
        if (mapByB.find(b) != mapByB.end()) {
            (*mapByA[a]).second = b;
            (*mapByB[b]).first = a;
            return;
        }

        storage.push_back({a, b});
        mapByA[a] = last();
        mapByB[b] = last();
    }

    ProxyB operator[](const A& key) {
        if (mapByA.find(key) != mapByA.end()) {
            return ProxyB{*this, mapByA[key]};
        }
        storage.push_back({key, {}});
        return ProxyB{*this, last()};
    }

    ProxyA operator[](const B& key) {
        if (mapByB.find(key) != mapByB.end()) {
            return ProxyA{*this, mapByB[key]};
        }
        storage.push_back({{}, key});
        return ProxyA{*this, last()};
    }

    void mutate(const A& key, B& value) {
        if (mapByA.find(key) != mapByA.end()) {
            (*mapByA[key]).second = value;
            (*mapByB[value]).first = key;
            return;
        }

        storage.push_back({key, value});
        mapByA[key] = last();
        mapByB[value] = last();
    }
};

}
#pragma once

#include <unordered_map>
#include <list>
#include <type_traits>

namespace hd {

template<typename T>
struct always_false : std::false_type {};

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

    template <typename T>
    struct Selector {
        bimap& bm;
        T t;

        ProxyA left() {
            if (bm.mapByB.find(t) != bm.mapByB.end()) {
                return ProxyA{bm, bm.mapByB[t]};
            }
            bm.storage.push_back({{}, t});
            return ProxyA{bm, bm.last()};
        }

        ProxyB right() {
            if (bm.mapByA.find(t) != bm.mapByA.end()) {
                return ProxyB{bm, bm.mapByA[t]};
            }
            bm.storage.push_back({t, {}});
            return ProxyB{bm, bm.last()};
        }
    };

    friend ProxyA;
    friend ProxyB;
    friend Selector<A>;
    friend Selector<B>;

    template <typename Pair>
    void insert(Pair p) {
        if constexpr (std::is_same<A, B>::value) {
            static_assert(std::is_same<Pair, std::pair<A, A>>::value, "invalid pair for A==B");
            if (!mapByA.find(p.first) != mapByA.end()) {
                storage.push_back(p);
                mapByA[p.first] = last();
                mapByB[p.second] = last();
            }

        } else if constexpr (std::is_same<Pair, std::pair<A, B>>::value) {
            if (!mapByA.find(p.first) != mapByA.end()) {
                storage.push_back(p);
                mapByA[p.first] = last();
                mapByB[p.second] = last();
            }

        } else if constexpr (std::is_same<Pair, std::pair<B, A>>::value) {
            if (!mapByA.find(p.second) != mapByA.end()) {
                storage.push_back(reverse(p));
                mapByA[p.second] = last();
                mapByB[p.first] = last();
            }

        } else {
            static_assert(always_false<Pair>::value, "invalid pair type");
        }
    }

    // template <
    //     typename T = A,
    //     typename std::enable_if<std::is_same<T, B>::value, int>::type = 0>
    // void insert(std::pair<A, A> p) {
    //     if (!mapByA.find(p.first) != mapByA.end()) {
    //         storage.push_back(p);
    //         mapByA[p.first] = last();
    //         mapByB[p.second] = last();
    //     }
    // }

    // template <
    //     typename T = A,
    //     typename std::enable_if<!std::is_same<T, B>::value, int>::type = 0>
    // void insert(std::pair<A, B> p) {
    //     if (!mapByA.find(p.first) != mapByA.end()) {
    //         storage.push_back(p);
    //         mapByA[p.first] = last();
    //         mapByB[p.second] = last();
    //     }
    // }

    // template <
    //     typename T = A,
    //     typename std::enable_if<!std::is_same<T, B>::value, int>::type = 0>
    // void insert(std::pair<B, A> p) {
    //     if (!mapByA.find(p.second) != mapByA.end()) {
    //         storage.push_back(reverse(p));
    //         mapByA[p.second] = last();
    //         mapByB[p.first] = last();
    //     }
    // }

    template <typename U, typename V>
    void emplace(U u, V v) {
        if constexpr (std::is_same<A, B>::value) {
            static_assert(std::is_same<A, U>::value && std::is_same<B, V>::value, "different types");
            if (mapByA.find(u) != mapByA.end()) {
                (*mapByA[u]).second = v;
                (*mapByB[v]).first = u;
                return;
            }
            storage.push_back({u, v});
            mapByA[u] = last();
            mapByB[v] = last();

        } else if constexpr (std::is_same<A, U>::value && std::is_same<B, V>::value) {
            if (mapByA.find(u) != mapByA.end()) {
                (*mapByA[u]).second = v;
                (*mapByB[v]).first = u;
                return;
            }
            storage.push_back({u, v});
            mapByA[u] = last();
            mapByB[v] = last();

        } else if constexpr (std::is_same<B, U>::value && std::is_same<A, V>::value) {
            if (mapByB.find(v) != mapByB.end()) {
                (*mapByA[u]).second = u;
                (*mapByB[v]).first = v;
                return;
            }
            storage.push_back({v, u});
            mapByA[v] = last();
            mapByB[u] = last();

        } else {
            static_assert(always_false<std::pair<U, V>>::value, "invalid types");
        }
    }

    // template <
    //     typename T = A,
    //     typename std::enable_if<std::is_same<T, B>::value, int>::type = 0>
    // void emplace(const T a, const T b) {
    //     if (mapByA.find(a) != mapByA.end()) {
    //         (*mapByA[a]).second = b;
    //         (*mapByB[b]).first = a;
    //         return;
    //     }

    //     storage.push_back({a, b});
    //     mapByA[a] = last();
    //     mapByB[b] = last();
    // }

    // template <
    //     typename T = A,
    //     typename std::enable_if<!std::is_same<T, B>::value, int>::type = 0>
    // void emplace(const A a, const B b) {
    //     if (mapByA.find(a) != mapByA.end()) {
    //         (*mapByA[a]).second = b;
    //         (*mapByB[b]).first = a;
    //         return;
    //     }

    //     storage.push_back({a, b});
    //     mapByA[a] = last();
    //     mapByB[b] = last();
    // }

    // template <
    //     typename T = A,
    //     typename std::enable_if<!std::is_same<T, B>::value, int>::type = 0>
    // void emplace(const B b, const A a) {
    //     if (mapByB.find(b) != mapByB.end()) {
    //         (*mapByA[a]).second = b;
    //         (*mapByB[b]).first = a;
    //         return;
    //     }

    //     storage.push_back({a, b});
    //     mapByA[a] = last();
    //     mapByB[b] = last();
    // }

    template <
        typename T = A,
        typename std::enable_if<std::is_same<T, B>::value, int>::type = 0>
    Selector<T> operator[](const T& key) {
        return Selector<T>{*this, key};
    }

    template <
        typename T = A,
        typename std::enable_if<!std::is_same<T, B>::value, int>::type = 0>
    ProxyA operator[](const B& key) {
        if (mapByB.find(key) != mapByB.end()) {
            return ProxyA{*this, mapByB[key]};
        }
        storage.push_back({{}, key});
        return ProxyA{*this, last()};
    }

    template <
        typename T = A,
        typename std::enable_if<!std::is_same<T, B>::value, int>::type = 0>
    ProxyB operator[](const A& key) {
        if (mapByA.find(key) != mapByA.end()) {
            return ProxyB{*this, mapByA[key]};
        }
        storage.push_back({key, {}});
        return ProxyB{*this, last()};
    }
};

}
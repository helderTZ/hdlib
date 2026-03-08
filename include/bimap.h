#pragma once

#include <unordered_map>
#include <vector>
#include <type_traits>

namespace hd {

template<typename T>
struct always_false : std::false_type {};

template <typename A, typename B>
class bimap {
private:
    std::vector<std::pair<A, B>> storage;
    std::unordered_map<A, size_t> mapByA;
    std::unordered_map<B, size_t> mapByB;

    std::pair<B, A> reverse(std::pair<A, B> p) {
        return {p.second, p.first};
    }

    size_t last() {
        return storage.size() - 1UL;
    }

    void erase_left(const A& a) {
        const size_t index = mapByA[a];
        const B b = storage[index].second;
        auto [a_last, b_last] = storage[last()];
        swap(storage[index], storage[last()]);
        storage.pop_back();
        mapByA[a_last] = index;
        mapByB[b_last] = index;
        mapByA.erase(a);
        mapByB.erase(b);
    }

    void erase_right(const B& b) {
        const size_t index = mapByA[b];
        const A a = storage[index].first;
        auto [a_last, b_last] = storage[last()];
        swap(storage[index], storage[last()]);
        storage.pop_back();
        mapByA[a_last] = index;
        mapByB[b_last] = index;
        mapByA.erase(a);
        mapByB.erase(b);
    }

public:

    struct ProxyB {
        bimap& bm;
        size_t index;

        ProxyB& operator=(const B& b) {
            const A a = bm.storage[index].first;

            if (bm.storage.size() == 1) {
                bm.storage.pop_back();
                bm.storage.push_back({a, b});
                bm.mapByA[a] = bm.last();
                bm.mapByB[b] = bm.last();
                return *this;
            }

            bm.mapByA.erase(bm.storage[index].first);
            bm.mapByB.erase(bm.storage[index].second);
            auto [a_last, b_last] = bm.storage[bm.last()];
            swap(bm.storage[index], bm.storage[bm.last()]);
            bm.storage.pop_back();
            bm.storage.push_back({a, b});
            bm.mapByA[a] = bm.last();
            bm.mapByB[b] = bm.last();
            bm.mapByA[a_last] = index;
            bm.mapByB[b_last] = index;
            return *this;
        }

        operator B() const { return bm.storage[index].second; }
    };

    struct ProxyA {
        bimap& bm;
        size_t index;

        ProxyA& operator=(const A& a) {
            const B b = bm.storage[index].second;

            if (bm.storage.size() == 1) {
                bm.storage.pop_back();
                bm.storage.push_back({a, b});
                bm.mapByA[a] = bm.last();
                bm.mapByB[b] = bm.last();
                return *this;
            }

            bm.mapByA.erase(bm.storage[index].first);
            bm.mapByB.erase(bm.storage[index].second);
            auto [a_last, b_last] = bm.storage[bm.last()];
            swap(bm.storage[index], bm.storage[bm.last()]);
            bm.storage.pop_back();
            bm.storage.push_back({a, b});
            bm.mapByA[a] = bm.last();
            bm.mapByB[b] = bm.last();
            bm.mapByA[a_last] = index;
            bm.mapByB[b_last] = index;
            return *this;
        }

        operator A() const { return bm.storage[index].first; }
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
                storage[mapByA[u]].second = v;
                storage[mapByB[v]].first = u;
                return;
            }
            storage.push_back({u, v});
            mapByA[u] = last();
            mapByB[v] = last();

        } else if constexpr (std::is_same<A, U>::value && std::is_same<B, V>::value) {
            if (mapByA.find(u) != mapByA.end()) {
                storage[mapByA[u]].second = v;
                storage[mapByB[v]].first = u;
                return;
            }
            storage.push_back({u, v});
            mapByA[u] = last();
            mapByB[v] = last();

        } else if constexpr (std::is_same<B, U>::value && std::is_same<A, V>::value) {
            if (mapByB.find(v) != mapByB.end()) {
                storage[mapByA[u]].second = u;
                storage[mapByB[v]].first = v;
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

    struct left  {};
    struct right {};

    template <typename T, typename Dir = left>
    void erase(const T& t, [[maybe_unused]] const Dir dir = Dir{}) {
        if constexpr (std::is_same<A, B>::value) {
            auto it_a = mapByA.find(t);
            auto it_b = mapByB.find(t);
            if (it_a == mapByA.end() && it_b == mapByB.end()) {
                return;
            }
            if (it_a != mapByA.end() && it_b == mapByB.end()) {
                erase_left(t);
            } else if (it_a == mapByA.end() && it_b != mapByB.end()) {
                erase_right(t);
            } else if constexpr (std::is_same<Dir, left>::value) {
                erase_left(t);
            } else if constexpr (std::is_same<Dir, right>::value) {
                erase_right(t);
            }
        } else if constexpr (std::is_same<T, A>::value) {
            erase_left(t);

        } else if constexpr (std::is_same<T, B>::value) {
            erase_right(t);

        } else {
            static_assert(always_false<T>::value, "invalid type");
        }
    }

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

    template <typename T>
    bool contains(const T& key) {
        if constexpr (std::is_same<A, B>::value) {
            return mapByA.find(key) != mapByA.end() || mapByB.find(key) != mapByB.end();
        } else if constexpr (std::is_same<T, A>::value) {
            return mapByA.find(key) != mapByA.end();
        } else if constexpr (std::is_same<T, B>::value) {
            return mapByB.find(key) != mapByB.end();
        } else {
            static_assert(always_false<T>::value, "invalid type");
        }
    }

};

}
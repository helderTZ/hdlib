#pragma once

#include <functional>

namespace hd {

// From: https://stackoverflow.com/a/17624752/13499951
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define NAMED_DEFER(name, fn) Deferred defer_##name((fn));
#define DEFER(fn) Deferred CONCAT(defer_, __COUNTER__)((fn));

class Deferred {
private:
    std::function<void()> _fn;

public:
    Deferred(std::function<void()> fn)
        : _fn(fn) {}

    ~Deferred() { _fn(); }
};

}   // namespace hd
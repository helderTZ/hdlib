#pragma once

#include <thread>
#include <functional>

namespace hd {

template<typename Fn>
class jthread
{
private:
    std::thread _t;

public:
    jthread(std::function<Fn>&& fn) : _t(fn) {}

    jthread(const jthread& other) = delete;
    jthread operator=(const jthread& other) = delete;

    jthread(jthread&& other)
    {
        _t = std::move(other._t);
    }
    
    jthread& operator=(jthread&& other)
    {
        _t = std::move(other._t);
        return *this;
    }

    ~jthread() {
        if (_t.joinable()) _t.join();
    }

    void swap(jthread& other)
    {
        std::swap(_t, other._t);
    }
};

}   // namespace hd

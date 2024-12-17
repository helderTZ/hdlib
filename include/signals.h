#pragma once

#include <vector>
#include <functional>

namespace hd {

class signal {
public:
    void connect(std::function<void(void)> cb) {
        _callbacks.push_back(cb);
    }

    void emit() {
        for (const auto& cb : _callbacks) {
            std::invoke(cb);
        }
    }

private:
    std::vector<std::function<void(void)>> _callbacks;
};

}   // namespace hd

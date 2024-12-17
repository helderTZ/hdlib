#pragma once

#include <vector>
#include <map>
#include <functional>

namespace hd {

template <typename T> class property;

template <typename T>
class slot {
public:
    slot() : _callbacks{} {}

    void connect(std::function<void(void)> callback) {
        _callbacks.push_back(callback);
    }

    friend class property<T>;

private:
    std::vector<std::function<void(void)>> _callbacks;
};

template <typename T>
class property {
public:
    property()
        : _value{}
        , _onValueChange_slot{}
        , _onSpecificValue_slot{}
        {}
    property(T t) : _value(t), _onValueChange_slot{} {}

    property<T>& operator=(const T& other) {
        if (_value != other) {
            _value = other;
            for (const auto& cb : _onValueChange_slot._callbacks) {
                std::invoke(cb);
            }
            for (const auto& [value, callbacks] : _onSpecificValue_slot) {
                if (other == value) {
                    for (const auto& cb : callbacks._callbacks) {
                        std::invoke(cb);
                    }
                }
            }
        }
        return *this;
    }

    T value() {
        return _value;
    }

    slot<T>& onValueChange() {
        return _onValueChange_slot;
    }

    slot<T>& onSpecificValue(T t) {
        auto it = _onSpecificValue_slot.find(t);
        if (it != _onSpecificValue_slot.end()) {
            return (*it).second;
        } else {
            return _onSpecificValue_slot[t];
        }
    }

private:
    T _value;
    slot<T> _onValueChange_slot;
    std::map<T, slot<T>> _onSpecificValue_slot;
};

}   // namespace hd

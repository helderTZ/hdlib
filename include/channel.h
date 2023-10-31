#pragma once

#include <optional>
#include <utility>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace hd {

// Adapted from: https://medium.com/@techhara/c-implement-channel-from-scratch-73eaca14d457

template<typename T>
class Channel;

template<typename T>
std::pair<Channel<T>, Channel<T>> make_channel() {
    auto ptr = std::make_shared<typename Channel<T>::State>();
    Channel<T> sender{ptr, true};
    Channel<T> receiver{std::move(ptr), false};
    return std::make_pair(std::move(sender), std::move(receiver));
}

template<typename T>
class Channel {
public:
    Channel(Channel&) = delete;
    Channel(Channel&&) = default;
    Channel& operator=(Channel&) = delete;
    Channel& operator=(Channel&&) = default;

    ~Channel() { close(); }

    bool send(T t) {
        while(true) {
            if (!is_sender || !state) break;
            std::scoped_lock lock{state->mtx};
            if (state->closed) break;
            state->queue.push(std::move(t));
            return true;
        }
        return false;
    }

    std::optional<T> recv() {
        while(true) {
            if (is_sender || !state) break;
            std::unique_lock lock{state->mtx};
            state->cv.wait(lock,
                [this](){ return !state->queue.empty() || state->closed;});
            if (state->queue.empty()) break;
            auto x = std::move(state->queue.front());
            state->queue.pop();
            return x;
        }
        return std::nullopt;
    }

    bool close() {
        while(true) {
            if (!state) break;
            std::scoped_lock lock{state->mtx};
            if (state->closed) break;
            state->closed = true;
            state->cv.notify_one();
            return true;
        }
        return false;
    }

private:
    struct State {
        std::queue<T> queue;
        std::mutex mtx;
        std::condition_variable cv;
        bool closed = false;
    };

    explicit Channel(std::shared_ptr<State> state, bool is_sender)
        : state{std::move(state)}, is_sender{is_sender} {}

    friend std::pair<Channel<T>, Channel<T>> make_channel<T>();

    std::shared_ptr<State> state;
    bool is_sender;
};

// TODO: make a mpsc (Multiple Producer Single Consumer)

}   // namespace hd
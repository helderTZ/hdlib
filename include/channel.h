#pragma once

#include <optional>
#include <utility>
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <memory>

namespace hd {

// Adapted from: https://medium.com/@techhara/c-implement-channel-from-scratch-73eaca14d457

template<typename T>
class ChannelTx;

template<typename T>
class ChannelRx;

template<typename T>
class MpscChannelTx;

template<typename T>
class MpscChannelRx;

namespace impl {
    template<typename T>
    struct State {
        std::queue<T> queue;
        std::mutex mtx;
        std::condition_variable cv;
        bool closed = false;
    };

    template<typename T>
    struct MpcsState {
        std::queue<T> queue;
        // std::shared_mutex mtx;
        // std::condition_variable_any cv;
        std::mutex mtx;
        std::condition_variable cv;
        int producers = 0;
        bool closed = false;
    };
};

template<typename T>
std::pair<ChannelTx<T>, ChannelRx<T>> make_channel() {
    auto ptr = std::make_shared<impl::State<T>>();
    ChannelTx<T> sender{ptr};
    ChannelRx<T> receiver{std::move(ptr)};
    return std::make_pair(std::move(sender), std::move(receiver));
}

template<typename T>
std::pair<MpscChannelTx<T>, MpscChannelRx<T>> make_mpsc() {
    auto ptr = std::make_shared<impl::MpcsState<T>>();
    MpscChannelTx<T> sender{ptr};
    MpscChannelRx<T> receiver{std::move(ptr)};
    return std::make_pair(std::move(sender), std::move(receiver));
}

template<typename T>
class ChannelTx {
public:
    ChannelTx(ChannelTx&) = delete;
    ChannelTx(ChannelTx&&) = default;
    ChannelTx& operator=(ChannelTx&) = delete;
    ChannelTx& operator=(ChannelTx&&) = default;

    ~ChannelTx() { close(); }

    bool send(T t) {
        if (!state) return false;

        std::scoped_lock lock{state->mtx};

        if (state->closed) return false;

        state->queue.push(std::move(t));
        return true;
    }

    bool close() {
        if (!state) return false;

        std::scoped_lock lock{state->mtx};

        if (state->closed) return false;

        state->closed = true;
        state->cv.notify_all();
        return true;
    }

private:
    explicit ChannelTx(std::shared_ptr<impl::State<T>> state)
        : state{std::move(state)} {}

    friend std::pair<ChannelTx<T>, ChannelRx<T>> make_channel<T>();

    std::shared_ptr<impl::State<T>> state;
};

template<typename T>
class ChannelRx {
public:
    ChannelRx(ChannelRx&) = delete;
    ChannelRx(ChannelRx&&) = default;
    ChannelRx& operator=(ChannelRx&) = delete;
    ChannelRx& operator=(ChannelRx&&) = default;

    ~ChannelRx() { close(); }

    std::optional<T> recv() {
        if (!state) return std::nullopt;

        std::unique_lock lock{state->mtx};
        state->cv.wait(lock,
            [this](){ return !state->queue.empty() || state->closed; });

        if (state->queue.empty()) return std::nullopt;

        auto x = std::move(state->queue.front());
        state->queue.pop();
        return x;
    }

    bool close() {
        if (!state) return false;

        std::scoped_lock lock{state->mtx};

        if (state->closed) return false;

        state->closed = true;
        state->cv.notify_all();
        return true;
    }

private:
    explicit ChannelRx(std::shared_ptr<impl::State<T>> state)
        : state{std::move(state)} {}

    friend std::pair<ChannelTx<T>, ChannelRx<T>> make_channel<T>();

    std::shared_ptr<impl::State<T>> state;
};

template<typename T>
class MpscChannelTx {
public:
    MpscChannelTx(MpscChannelTx& other) {
        std::scoped_lock lock{other.state->mtx};
        state = other.state;
        state->producers++;
    }

    MpscChannelTx(MpscChannelTx&&) = default;

    MpscChannelTx& operator=(MpscChannelTx& other) {
        {
            std::scoped_lock lock{other.state->mtx};
            state = other.state;
            state->producers++;
        }
        return *this;
    }

    MpscChannelTx& operator=(MpscChannelTx&&) = default;

    MpscChannelTx<T> copy() {
        MpscChannelTx<T> tx{*this};
        return tx;
    }

    ~MpscChannelTx() { close(); }

    bool send(T t) {
        if (!state) return false;

        std::scoped_lock lock{state->mtx};

        if (state->closed) return false;

        state->queue.push(std::move(t));
        return true;
    }

    bool is_closed() {
        return state->closed;
    }

    bool close() {
        if (!state) return false;

        std::unique_lock lock{state->mtx};

        if (state->closed) return false;

        state->producers--;
        if (state->producers <= 0) {
            state->closed = true;
            state->cv.notify_all();
        }
        return true;
    }

private:
    explicit MpscChannelTx(std::shared_ptr<impl::MpcsState<T>> state)
        : state{std::move(state)} {}

    friend std::pair<MpscChannelTx<T>, MpscChannelRx<T>> make_mpsc<T>();

    std::shared_ptr<impl::MpcsState<T>> state;
};

template<typename T>
class MpscChannelRx {
public:
    MpscChannelRx(MpscChannelRx&) = delete;
    MpscChannelRx(MpscChannelRx&&) = default;
    MpscChannelRx& operator=(MpscChannelRx&) = delete;
    MpscChannelRx& operator=(MpscChannelRx&&) = default;

    ~MpscChannelRx() { close(); }

    std::optional<T> recv() {
        if (!state) return std::nullopt;

        std::unique_lock lock{state->mtx};
        state->cv.wait(lock,
            [this](){ return !state->queue.empty() || state->closed; });

        if (state->queue.empty()) return std::nullopt;

        auto x = std::move(state->queue.front());
        state->queue.pop();
        return x;
    }

    bool is_closed() {
        return state->closed;
    }

    bool close() {
        if (!state) return false;

        std::scoped_lock lock{state->mtx};

        if (state->closed) return false;

        state->closed = true;
        state->cv.notify_all();
        return true;
    }

private:
    explicit MpscChannelRx(std::shared_ptr<impl::MpcsState<T>> state)
        : state{std::move(state)} {}

    friend std::pair<MpscChannelTx<T>, MpscChannelRx<T>> make_mpsc<T>();

    std::shared_ptr<impl::MpcsState<T>> state;
};


}   // namespace hd
#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>

namespace hd {

class thread_pool {
public:
    thread_pool(size_t num_threads) {
        for (size_t i{}; i < num_threads; i++) {
            threads.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(mtx);

                        cv.wait(lock, [this]() {
                            return stop || !tasks.empty();
                        });

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    ~thread_pool() {
        {
            std::scoped_lock<std::mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();

        std::for_each(threads.begin(), threads.end(),
            [](auto& t) { t.join(); }
        );
    };

    template<typename Fn, typename... Args>
    std::future<typename std::invoke_result_t<Fn, Args...>> enqueue(Fn&& fn, Args&&... args) {
        using return_type = typename std::invoke_result_t<Fn, Args...>;

        // move fn and args to the capture list so we don't have dangling references
        auto task_fn = [
            fn = std::forward<Fn>(fn),
            args = std::make_tuple(std::forward<Args>(args)...)
        ]() mutable {
            return std::apply(std::move(fn), std::move(args));
        };

        // using shared_ptr makes it copyable and provides lifetime extension
        // makes it exist as long as it is in the queue
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::move(task_fn));

        std::future<return_type> result = task->get_future();

        {
            std::unique_lock<std::mutex> lock(mtx);
            tasks.emplace([task]() { (*task)(); });
        }

        cv.notify_one();
        return result;
    }

private:
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop{false};
    std::vector<std::thread> threads;
};

};

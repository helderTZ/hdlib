#include "gtest/gtest.h"
#include "channel.h"

#include <vector>
#include <thread>
#include <chrono>

using namespace hd;

TEST(channel, send_recv) {
    std::vector<int> values;
    auto [tx, rx] = make_channel<int>();

    std::thread t1([&](decltype(tx) tx) {
        for (int i = 0; i < 100; ++i) {
            tx.send(i);
        }
    }, std::move(tx));

    std::thread t2([&](decltype(rx) rx) {
        for (int i = 0; i < 100; ++i) {
            values.push_back(std::move(*rx.recv()));
        }
    }, std::move(rx));

    t1.join();
    t2.join();

    for (int i = 0; i < 100; ++i) {
        ASSERT_EQ(values[i], i);
    }
}

// TODO: make a mpsc
// TEST(mpsc, send_recv) {

// }
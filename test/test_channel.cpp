#include "gtest/gtest.h"
#include "channel.h"

#include <vector>
#include <thread>
#include <chrono>
#include <set>

using namespace hd;

TEST(channel, send_recv) {
    std::vector<int> values;
    auto [tx, rx] = make_channel<int>();

    std::thread t1([&](auto&& tx) {
        for (int i = 0; i < 100; ++i) {
            tx.send(i);
        }
    }, std::move(tx));

    std::thread t2([&](auto&& rx) {
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

TEST(channel, DISABLED_send_recv_mpsc) {
    std::set<int> values;
    auto [tx, rx] = make_mpsc<int>();

    auto sender = [&](auto tx, int base) {
        for (int i = base; i < base+100; ++i) {
            tx.send(i);
        }
    };

    auto receiver = [&](auto&& rx) {
        while (true) {
            auto i = rx.recv();
            if (!i) break;
            values.insert(std::move(*i));
        }
    };

    std::thread t1(sender, std::move(tx.copy()), 0);
    std::thread t2(sender, std::move(tx.copy()), 100);
    std::thread t3(receiver, std::move(rx));

    t1.join();
    t2.join();
    t3.join();

    for (int i = 0; i < 200; ++i) {
        printf("%d ", *values.find(i));
    }
    printf("\n");

    for (int i = 0; i < 200; ++i) {
        ASSERT_EQ(*values.find(i), i);
    }
}
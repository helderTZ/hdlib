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

//FIXME: something still fishy here...
//       producers is initialized to 0
//       shouldn't it be 1? -> causes deadlock...
TEST(channel, send_recv_mpsc_single) {
    auto channels = make_mpsc<int>();
    auto& tx = std::get<0>(channels);
    auto& rx = std::get<1>(channels);

    // std::thread producer([&tx]() {
    //     for (int i = 0; i < 100; ++i) {
    //         tx.send(i);
    //     }
    // });
    for (int i = 0; i < 100; ++i) {
        tx.send(i);
    }

    std::thread consumer([rx = std::move(rx)] () mutable {
        for (int i = 0; i < 100; ++i) {
            ASSERT_EQ(*rx.recv(), i);
        }
    });

    // producer.join();
    consumer.join();
}

TEST(channel, send_recv_mpsc_multiple) {
    std::vector<std::thread> threads;
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

    for (int i = 0; i < 10; ++i) {
        threads.push_back(
            std::thread(sender, std::move(tx.copy()), i*100)
        );
    }
    std::thread receiver_thread(receiver, std::move(rx));

    for (int i = 0; i < 10; ++i) {
        threads[i].join();
    }
    receiver_thread.join();

    for (int i = 0; i < 1000; ++i) {
        ASSERT_EQ(*values.find(i), i);
    }
}
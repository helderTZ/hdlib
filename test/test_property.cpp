#include "gtest/gtest.h"
#include "property.h"

#include <iostream>

using namespace hd;

static size_t counter = 0U;

TEST(property, basic_functionality) {
    hd::property<int> uut;

    uut.onValueChange().connect([&]() {
        counter++;
        std::cout << "Received signal\n";
    });

    ASSERT_EQ(uut.value(), 0);
    uut = 1;
    ASSERT_EQ(counter, 1U);
    ASSERT_EQ(uut.value(), 1);

    counter = 0U;
}

TEST(property, specific_value) {
    hd::property<int> uut;

    uut.onSpecificValue(10).connect([&]() {
        counter = 10;
        std::cout << "Received signal\n";
    });

    uut = 1;
    ASSERT_EQ(counter, 0U);
    ASSERT_EQ(uut.value(), 1);

    uut = 10;
    ASSERT_EQ(counter, 10U);
    ASSERT_EQ(uut.value(), 10);
}
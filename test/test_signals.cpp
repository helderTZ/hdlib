#include "gtest/gtest.h"
#include "signals.h"

#include <iostream>

using namespace hd;

static size_t counter1 = 0U;
static size_t counter2 = 100U;

TEST(property, basic_functionality) {
    hd::signal uut;

    uut.connect([&]() { counter1++; });
    uut.connect([&]() { counter2--; });

    uut.emit();
    ASSERT_EQ(counter1, 1U);
    ASSERT_EQ(counter2, 99U);
}

#include "gtest/gtest.h"
#include "algorithm.h"

#include <vector>

using namespace hd;

enum UUT {
    ONE=2,
    TWO,
    THREE,
};

TEST(has_duplicates, has_duplicates_enum) {
    std::vector<UUT> uut;

    uut.push_back(ONE);
    uut.push_back(TWO);
    uut.push_back(THREE);
    uut.push_back(ONE);

    ASSERT_TRUE(has_duplicates(uut.begin(), uut.end()));
}

TEST(has_duplicates, has_duplicates_int) {
    std::vector<int> uut;

    uut.push_back(1);
    uut.push_back(2);
    uut.push_back(3);
    uut.push_back(1);

    ASSERT_TRUE(has_duplicates(uut.begin(), uut.end()));
}

//FIXME: has_duplicates not working
TEST(has_duplicates, DISABLED_does_not_have_duplicates_enum) {
    std::vector<UUT> uut;

    uut.push_back(ONE);
    uut.push_back(TWO);
    uut.push_back(THREE);

    ASSERT_FALSE(has_duplicates(uut.begin(), uut.end()));
}

TEST(last, works_for_pod) {
    std::vector<int> uut;

    uut.push_back(1);
    uut.push_back(2);
    uut.push_back(3);

    ASSERT_EQ(3, last(uut));
}

TEST(last, works_for_pointers) {
    std::vector<int*> uut;
    int arr[3] = {1, 2, 3};

    uut.push_back(&arr[0]);
    uut.push_back(&arr[1]);
    uut.push_back(&arr[2]);

    ASSERT_EQ(&arr[2], last(uut));
    ASSERT_EQ(arr[2], *last(uut));
}

#include "lifetime.h"
unsigned int ObjLifetimeInspector::id = 0;

TEST(last, check_for_unnecessary_copies) {
    std::vector<ObjLifetimeInspector> uut;
    uut.reserve(3); // need reserve so we donºt trigger a regrow of the vector,
                    // which would cause an extra copy (the creation of the objwct
                    // and the copy/move to the new location)

    uut.emplace_back("a");
    uut.emplace_back("b");
    uut.emplace_back("c");

    auto& _ = last(uut);
    static_cast<void>(_);

    ASSERT_EQ(3, ObjLifetimeInspector::id);
}
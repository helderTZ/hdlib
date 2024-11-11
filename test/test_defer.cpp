#include "gtest/gtest.h"
#include "defer.h"

#include <iostream>
#include <new>

using namespace hd;

class DestructorInspector {
private:
    static unsigned int _counter;
public:
    DestructorInspector() 
    {
        _counter++;
    }
    ~DestructorInspector()
    {
        std::cout << "I have been called: " << _counter << "\n";
        _counter--;
    }
};

unsigned int DestructorInspector::_counter{0};

TEST(defer, defer_straightforward_usage_multiple) {
    DestructorInspector* a = new DestructorInspector();
    DestructorInspector* b = new DestructorInspector();
    DEFER([&]() { delete a; });
    DEFER([&]() { delete b; });
}

TEST(defer, named_defer_straightforward_usage_multiple) {
    DestructorInspector* a = new DestructorInspector();
    DestructorInspector* b = new DestructorInspector();
    NAMED_DEFER(delete_a, [&]() { delete a; });
    NAMED_DEFER(delete_b, [&]() { delete b; });
}
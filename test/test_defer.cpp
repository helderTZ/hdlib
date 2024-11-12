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
    {
        DestructorInspector* a = new DestructorInspector();
        DestructorInspector* b = new DestructorInspector();
        DEFER([&]() { delete a; });
        DEFER([&]() { delete b; });
    }

    {
        DestructorInspector* c = new DestructorInspector();
        DestructorInspector* d = new DestructorInspector();
        DEFER([&]() { delete c; });
        DEFER([&]() { delete d; });
    }
}

TEST(defer, named_defer_straightforward_usage_multiple) {
    {
        DestructorInspector* a = new DestructorInspector();
        DestructorInspector* b = new DestructorInspector();
        NAMED_DEFER(deleta_a, [&]() { delete a; });
        NAMED_DEFER(deleta_b, [&]() { delete b; });
    }

    {
        DestructorInspector* c = new DestructorInspector();
        DestructorInspector* d = new DestructorInspector();
        NAMED_DEFER(delete_c, [&]() { delete c; });
        NAMED_DEFER(delete_d, [&]() { delete d; });
    }
}
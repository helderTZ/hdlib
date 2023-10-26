#include "lifetime.h"

#include <string>
#include <utility>

unsigned int hd::ObjLifetimeInspector::id = 0;

class UUT : public hd::ObjLifetimeInspector {
public:
    UUT() : hd::ObjLifetimeInspector() {}
    UUT(std::string name) : hd::ObjLifetimeInspector(name) {}
};

int main() {
    UUT uut0;
    UUT uut1{"aaaa"};
    UUT uut2(uut1);
    UUT uut3 = uut1;
    UUT uut4(std::move(uut2));
    UUT uut5 = std::move(uut3);
}
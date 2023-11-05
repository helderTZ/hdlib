#pragma once

#include <cstdio>
#include <string>
#include <utility>

namespace hd {

class ObjLifetimeInspector {
public:
    static unsigned int id;
    std::string name;
    unsigned int owned_id;

    ObjLifetimeInspector() : name(""), owned_id(id++){
        printf("No-arg constructor called, '%s', id: %u\n",
            name.c_str(), owned_id);
    }
    ObjLifetimeInspector(std::string name_) : name(name_), owned_id(id++) {
        printf("Constructor called for '%s', id: %u\n",
            name.c_str(), owned_id);
    }
    ~ObjLifetimeInspector() {
        printf("Destructor called for '%s', id: %u\n",
            name.c_str(), owned_id);
    }
    ObjLifetimeInspector(const ObjLifetimeInspector& other) {
        name = other.name;
        owned_id = id++;
        printf("Copy constructor called for '%s', id: %u\n",
            name.c_str(), owned_id);
    }
    ObjLifetimeInspector(ObjLifetimeInspector&& other) {
        name = std::move(other.name);
        owned_id = id++;
        printf("Move constructor called for '%s', id: %u\n",
            name.c_str(), owned_id);
    }
    ObjLifetimeInspector& operator=(const ObjLifetimeInspector& other) {
        name = other.name;
        owned_id = id++;
        printf("Copy assignment operator called for '%s', id: %u\n",
            name.c_str(), owned_id);
        return *this;
    }
    ObjLifetimeInspector& operator=(ObjLifetimeInspector&& other) {
        name = std::move(other.name);
        owned_id = id++;
        printf("Move assignment operator called for '%s', id: %u\n",
            name.c_str(), owned_id);
        return *this;
    }
};

}   // namespace hd
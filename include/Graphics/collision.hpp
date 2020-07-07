#pragma once

#include <unordered_set>
#include "Graphics/hash.hpp"

struct Physical {
    Point* obj;
    float mass;
    bool fixed;
    Point* operator->(){
        return obj;
    }
};

class CHandler {
    const float elastic;
    std::unordered_set<Physical> tangible;
    std::vector<std::vector<Physical>> get_check() const;
    public:
        CHandler();
        CHandler(bool elastic);
        void operator()() const;
        void add(Point* v, float mass = 1.0f, bool fixed = false);
        void add(Physical t);
        void remove(Point* v);
        void remove(Physical t);
        void collision(Physical& obj1, Physical& obj2) const;
};

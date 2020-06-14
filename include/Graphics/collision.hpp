#pragma once

#include <set>
#include <unordered_set>
#include "Graphics/hash.hpp"
#include "Graphics/geometry.hpp"

struct Physical {
    Point* obj;
    float mass;
    bool fixed;
    bool operator<(const Physical& p){
        if(obj->dim() == p.obj->dim()) return obj->isSpace() < p.obj->isSpace();
        return obj->dim() < p.obj->dim();
    }
};

static bool operator<(const Physical& p1, const Physical& p2){
    if(p1.obj->dim() == p2.obj->dim()) return p1.obj->isSpace() < p2.obj->isSpace();
    return p1.obj->dim() < p2.obj->dim();
}

class CHandler {
    std::vector<Physical> _tangible;
    std::vector<std::vector<Physical>> get_check() const;
    public:
        CHandler();
        void operator()() const;
        void add(Point* v, float mass = 1.0f, bool fixed = false);
        void add(Physical t);
        void collision(Physical& obj1, Physical& obj2) const;
        void handle(Polygon* s1, Polygon* s2) const;
        void handle(Polygon* s1, Polyhedron* s2) const;
        void handle(Polyhedron* s1, Polygon* s2) const;
        void handle(Polyhedron* s1, Polyhedron* s2) const;
};

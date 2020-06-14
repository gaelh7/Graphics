#pragma once

#include <set>
#include <unordered_set>
#include "Graphics/hash.hpp"
#include "Graphics/geometry.hpp"

struct Physical {
    Point* obj;
    float mass;
    bool fixed;
    Point* operator->(){
        return obj;
    }
};

template<>
struct std::hash<Physical>{
    size_t operator()(const Physical& p) const {
        return std::hash<Point>()(*p.obj);
    }
};

static bool operator==(const Physical& p1, const Physical& p2){
    std::hash<Physical> h;
    return h(p1) == h(p2);
}

class CHandler {
    std::unordered_set<Physical> tangible;
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

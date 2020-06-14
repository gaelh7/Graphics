#pragma once

#include <glm/gtx/hash.hpp>
#include "Graphics/geometry.hpp"

namespace std{
    template<>
    struct hash<Point>{
        size_t operator()(const Point& p){
            size_t seed = p.vertices.size();
            for(std::shared_ptr<Point> p: p.vertices){
                seed ^= std::hash<glm::vec3>()(p->pos);
            }
            return seed;
        }
    };
}

static bool operator==(const Point& p1, const Point& p2){
    std::hash<Point> h;
    return h(p1) == h(p2);
}
